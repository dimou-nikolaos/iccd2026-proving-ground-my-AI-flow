"""Unit tests for prep/dma_model.py (host python, no cocotb needed).

These tests validate the golden DMA model itself, so that on release day the model can be
trusted when it disagrees with the RTL. Run:

    python3 prep/test_dma_model.py
"""

import unittest

from dma_model import (
    DMAModel, ModelConfig, RegMap,
    CTRL_START, CTRL_IRQ_EN, CTRL_DESC_EN, CTRL_PACED,
    STATUS_BUSY, STATUS_DONE, STATUS_DESC_DONE,
)


def make_model(**cfg_kw):
    m = DMAModel(cfg=ModelConfig(**cfg_kw))
    m.apb_write(m.map.off("int_enable"), 0xFFFF_FFFF)
    return m


def program(m, ch, src, dst, words, extra_ctrl=0):
    m.apb_write(m.map.off("src", ch), src)
    m.apb_write(m.map.off("dst", ch), dst)
    m.apb_write(m.map.off("size", ch), words)
    m.apb_write(m.map.off("ctrl", ch), CTRL_START | CTRL_IRQ_EN | extra_ctrl)


class TestBasicTransfer(unittest.TestCase):
    def test_four_words(self):
        m = make_model()
        m.poke(0x1000, bytes(range(16)))
        program(m, 0, 0x1000, 0x2000, 4)
        m.run_until_idle()
        self.assertEqual(m.peek(0x2000), m.peek(0x1000))
        self.assertEqual(m.peek(0x200C), m.peek(0x100C))
        self.assertEqual(m.peek(0x2010), 0)
        self.assertEqual(m.ch[0].words_done, 4)

    def test_size_1_2_3(self):
        for n in (1, 2, 3):
            with self.subTest(n=n):
                m = make_model()
                m.poke(0x1000, bytes(range(64)))
                program(m, 0, 0x1000, 0x2000, n)
                m.run_until_idle()
                self.assertEqual(m.ch[0].words_done, n)
                for i in range(n):
                    self.assertEqual(m.peek(0x2000 + 4 * i), m.peek(0x1000 + 4 * i),
                                     f"word {i} of {n}")
                self.assertEqual(m.peek(0x2000 + 4 * n), 0, "wrote one word too many")

    def test_status_and_irq(self):
        m = make_model()
        m.poke(0x1000, b"\x11\x22\x33\x44")
        program(m, 0, 0x1000, 0x2000, 1)
        m.run_until_idle()
        status, err = m.apb_read(m.map.off("status", 0))
        self.assertFalse(err)
        self.assertTrue(status & STATUS_DONE)
        self.assertFalse(status & STATUS_BUSY)
        self.assertEqual(m.irq, 0b0001)

    def test_w1c_clear(self):
        m = make_model()
        m.poke(0x1000, b"\x11\x22\x33\x44")
        program(m, 0, 0x1000, 0x2000, 1)
        m.run_until_idle()
        self.assertEqual(m.irq, 0b0001)
        m.apb_write(m.map.off("status", 0), STATUS_DONE)
        status, _ = m.apb_read(m.map.off("status", 0))
        self.assertFalse(status & STATUS_DONE)
        self.assertEqual(m.irq, 0)

    def test_irq_masked(self):
        m = DMAModel()
        m.poke(0x1000, b"\x11\x22\x33\x44")
        m.apb_write(m.map.off("src", 0), 0x1000)
        m.apb_write(m.map.off("dst", 0), 0x2000)
        m.apb_write(m.map.off("size", 0), 1)
        m.apb_write(m.map.off("ctrl", 0), CTRL_START)      # no IRQ enable
        m.run_until_idle()
        status, _ = m.apb_read(m.map.off("status", 0))
        self.assertTrue(status & STATUS_DONE)
        self.assertEqual(m.irq, 0, "irq must be gated by the enable bit")

    def test_unmapped_offsets(self):
        m = DMAModel()
        _, slverr = m.apb_read(0x7F0)
        self.assertTrue(slverr)
        _, slverr = m.apb_write(0x7F0, 0)
        self.assertTrue(slverr)

    def test_count_minus_one_encoding(self):
        m = DMAModel(cfg=ModelConfig(size_is_count_minus_one=True))
        m.apb_write(m.map.off("int_enable"), 0xFFFF_FFFF)
        m.poke(0x1000, bytes(range(16)))
        program(m, 0, 0x1000, 0x2000, 3)          # SIZE=3 means 4 words
        m.run_until_idle()
        self.assertEqual(m.ch[0].words_done, 4)


class TestArbitration(unittest.TestCase):
    def _txn_order(self, m):
        return [(t["kind"], t["ch"]) for t in m.txns]

    def test_priority_order(self):
        m = make_model()
        m.poke(0x1000, bytes(range(16)))
        m.poke(0x1400, bytes(range(16, 32)))
        program(m, 1, 0x1000, 0x2000, 2)          # lower priority channel first
        program(m, 0, 0x1400, 0x2400, 2)          # higher priority
        m.set_prio([0, 1, 2, 3])
        m.run_until_idle()
        chans = [ch for _, ch in self._txn_order(m)]
        self.assertEqual(chans[0], 0, "highest-priority channel must start first")

    def test_priority_swap(self):
        m = make_model()
        m.poke(0x1000, bytes(range(16)))
        m.poke(0x1400, bytes(range(16, 32)))
        program(m, 1, 0x1000, 0x2000, 2)
        program(m, 0, 0x1400, 0x2400, 2)
        m.set_prio([1, 0, 2, 3])
        m.run_until_idle()
        self.assertEqual(self._txn_order(m)[0][1], 1)

    def test_rearbitration_per_word_with_pacing(self):
        """A paced channel that drops its request mid-transfer must yield the engine."""
        m = make_model()
        m.poke(0x1000, bytes(range(32)))
        m.poke(0x1400, bytes(range(64, 96)))
        program(m, 0, 0x1000, 0x2000, 4, extra_ctrl=CTRL_PACED)
        program(m, 1, 0x1400, 0x2400, 4)
        m.set_prio([0, 1, 2, 3])
        m.set_request(0, True)
        while m.ch[0].words_done == 0:          # let ch0 complete exactly one word
            m.tick()
        m.set_request(0, False)                 # yield the engine mid-transfer
        for _ in range(30):
            m.tick()
        self.assertGreater(m.ch[1].words_done, 0,
                           "engine did not serve ch1 while ch0 was stalled")
        self.assertEqual(m.ch[0].words_done, 1,
                         "paced ch0 advanced without its request line")

    def test_request_resume(self):
        m = make_model()
        m.poke(0x1000, bytes(range(16)))
        program(m, 0, 0x1000, 0x2000, 2, extra_ctrl=CTRL_PACED)
        m.set_request(0, False)
        m.tick()
        self.assertEqual(m.ch[0].words_done, 0)
        m.set_request(0, True)
        m.run_until_idle()
        self.assertEqual(m.ch[0].words_done, 2)


class TestDescriptors(unittest.TestCase):
    def _write_desc(self, m, addr, src, dst, words, nxt, more):
        m.write32(addr + 0x0, src)
        m.write32(addr + 0x4, dst)
        m.write32(addr + 0x8, words)
        m.write32(addr + 0xC, nxt)
        m.write32(addr + 0x10, 1 if more else 0)

    def test_two_descriptor_chain(self):
        m = make_model()
        m.poke(0x1000, bytes(range(16)))
        m.poke(0x1400, bytes(range(64, 80)))
        d0, d1 = 0x3000, 0x3020
        self._write_desc(m, d0, 0x1000, 0x2000, 4, d1, True)
        self._write_desc(m, d1, 0x1400, 0x2400, 4, 0, False)

        m.apb_write(m.map.off("desc_ptr", 0), d0)
        m.apb_write(m.map.off("ctrl", 0), CTRL_START | CTRL_IRQ_EN | CTRL_DESC_EN)
        m.run_until_idle()

        for i in range(4):
            self.assertEqual(m.peek(0x2000 + 4 * i), m.peek(0x1000 + 4 * i))
            self.assertEqual(m.peek(0x2400 + 4 * i), m.peek(0x1400 + 4 * i))
        status, _ = m.apb_read(m.map.off("status", 0))
        self.assertTrue(status & STATUS_DONE)
        self.assertTrue(status & STATUS_DESC_DONE, "CR-D2 descriptor-complete missing")
        self.assertEqual(m.irq & 0b1, 0b1)

    def test_chain_end(self):
        m = make_model()
        m.poke(0x1000, bytes(range(16)))
        d0 = 0x3000
        self._write_desc(m, d0, 0x1000, 0x2000, 1, 0, False)
        m.apb_write(m.map.off("desc_ptr", 0), d0)
        m.apb_write(m.map.off("ctrl", 0), CTRL_START | CTRL_IRQ_EN | CTRL_DESC_EN)
        m.run_until_idle()
        status, _ = m.apb_read(m.map.off("status", 0))
        self.assertTrue(status & STATUS_DONE)
        self.assertFalse(status & STATUS_BUSY)
        self.assertEqual(m.ch[0].words_done, 1)


if __name__ == "__main__":
    unittest.main(verbosity=2)
