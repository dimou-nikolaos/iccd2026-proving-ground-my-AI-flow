"""
Catalogue of *deliberately wrong* cores, for negative-testing the differential
harness itself.

Why: before trusting "no divergence" as evidence that the released `rv_core` is
clean, you need the opposite result on record - the same harness, run against a
core that is wrong in a *known* way, must diverge. That is the same discipline as
the SVA `make negative` targets (`prep/sva`, `prep/rvcore`), applied to the
ISS-side method. It also tells you, on release day, which predicted deviation
classes your generated programs actually reach.

Two families:
  - `policy.*`: plain `RV32Config` combinations. These are legitimate spec
    ambiguities (the released spec may define them either way) - match whichever
    the spec says, and use them to check your programs *exercise* the choice.
  - `seed.*`: subclasses that break exactly one architectural rule each,
    modelled on the deviation catalogue in `docs/designs/challenge-3-playbook.md`.

Limit (honest): every entry here is an *architectural* deviation. Pure pipeline
bugs - a missing load-use stall, a branch flush that kills the wrong instruction,
a forwarded value taken from the wrong stage - are NOT expressible in a serial
ISS, because architecturally they are invisible until they corrupt a retirement.
Those need the per-stage SVA observer (`prep/rvcore/rv_stage_sva.sv`) plus the
retire-stream lockstep, which is why both layers exist.
"""

from __future__ import annotations

from typing import Callable, Dict, Optional

from rv32_iss import (MASK32, RV32Config, RV32ISS, sext, u32)

Factory = Callable[[], RV32ISS]


# ---------------------------------------------------------------------------
# Policy variants (legitimate spec ambiguity - match whichever the spec says)
# Each factory has the same call signature as RV32ISS: factory(mem_size=...).
# ---------------------------------------------------------------------------
def policy_factory(**cfg_kwargs) -> Factory:
    def factory(mem_size: int = 1 << 20) -> RV32ISS:
        return RV32ISS(mem_size=mem_size, config=RV32Config(**cfg_kwargs))
    return factory


def policy_misalign_support(mem_size: int = 1 << 20) -> RV32ISS:
    return RV32ISS(mem_size=mem_size, config=RV32Config(misaligned="support"))


def policy_misalign_ignore(mem_size: int = 1 << 20) -> RV32ISS:
    return RV32ISS(mem_size=mem_size, config=RV32Config(misaligned="ignore"))


def policy_no_zba_zbb(mem_size: int = 1 << 20) -> RV32ISS:
    """CR-D3 not implemented: SH1ADD/SH2ADD/SH3ADD/MIN/MINU/MAX/MAXU trap illegal."""
    return RV32ISS(mem_size=mem_size, config=RV32Config(zba_zbb=False))


def policy_mtval_zero(mem_size: int = 1 << 20) -> RV32ISS:
    return RV32ISS(mem_size=mem_size, config=RV32Config(mtval_illegal="zero"))


def policy_ro_csr_ignored(mem_size: int = 1 << 20) -> RV32ISS:
    """Writes to read-only CSRs silently ignored instead of raising illegal."""
    return RV32ISS(mem_size=mem_size, config=RV32Config(ro_csr_write="ignore"))


def policy_mret_mpp_zero(mem_size: int = 1 << 20) -> RV32ISS:
    return RV32ISS(mem_size=mem_size, config=RV32Config(mpp_on_mret=0))


def policy_no_vectored_mtvec(mem_size: int = 1 << 20) -> RV32ISS:
    return RV32ISS(mem_size=mem_size, config=RV32Config(mtvec_vectored=False))


def policy_no_counter_csrs(mem_size: int = 1 << 20) -> RV32ISS:
    """mcycle/minstret read as 0 instead of counting."""
    iss = RV32ISS(mem_size=mem_size, config=RV32Config(counter_csrs=False))
    old_read = iss.read_csr

    def read_csr(addr: int) -> int:
        if addr in (0xB00, 0xB02):
            return 0
        return old_read(addr)

    iss.read_csr = read_csr          # instance attribute shadows the method
    return iss


# ---------------------------------------------------------------------------
# Seeded architectural deviations (one rule broken per class)
# ---------------------------------------------------------------------------
class NoSignExtendLoads(RV32ISS):
    """Signed loads (lb/lh) come back zero-extended."""

    def _do_load(self, addr: int, size: int, signed: bool) -> int:
        value = super()._do_load(addr, size, signed)
        return value & ((1 << (8 * size)) - 1)


class SraIsLogical(RV32ISS):
    """sra / srai implemented as a logical shift right."""

    def execute(self, instr: int) -> None:
        op = instr & 0x7F
        f3 = (instr >> 12) & 7
        f7 = (instr >> 25) & 0x7F
        if f3 == 5 and f7 == 0x20 and op in (0x33, 0x13):
            rd = (instr >> 7) & 0x1F
            sh = (instr >> 20) & 0x1F
            self._write_rd(rd, self.get_reg((instr >> 15) & 0x1F) >> sh)
            return
        super().execute(instr)


class DivByZeroReturnsZero(RV32ISS):
    """RV rule: div-by-0 -> -1, rem-by-0 -> dividend. Here both give 0."""

    @staticmethod
    def _div(a: int, b: int) -> int:
        return 0 if b == 0 else RV32ISS._div(a, b)

    @staticmethod
    def _rem(a: int, b: int) -> int:
        return 0 if b == 0 else RV32ISS._rem(a, b)


class DivOverflowZero(RV32ISS):
    """INT_MIN / -1: spec gives INT_MIN (div) and 0 (rem). Here div=0, rem=dividend."""

    @staticmethod
    def _div(a: int, b: int) -> int:
        if a == 0x8000_0000 and b == MASK32:
            return 0
        return RV32ISS._div(a, b)

    @staticmethod
    def _rem(a: int, b: int) -> int:
        if a == 0x8000_0000 and b == MASK32:
            return u32(a)
        return RV32ISS._rem(a, b)


class MulhsuOperandsSwapped(RV32ISS):
    """mulhsu multiplies *both* operands as signed."""

    def execute(self, instr: int) -> None:
        if (instr & 0x7F) == 0x33 and ((instr >> 12) & 7) == 2 and ((instr >> 25) & 0x7F) == 1:
            rd = (instr >> 7) & 0x1F
            a = sext(self.get_reg((instr >> 15) & 0x1F), 32)
            b = sext(self.get_reg((instr >> 20) & 0x1F), 32)
            self._write_rd(rd, (a * b) >> 32)
            return
        super().execute(instr)


class BranchesUnsigned(RV32ISS):
    """blt/bge compare their operands unsigned (relational mutation)."""

    def execute(self, instr: int) -> None:
        if (instr & 0x7F) != 0x63:
            super().execute(instr)
            return
        f3 = (instr >> 12) & 7
        if f3 not in (4, 5):
            super().execute(instr)
            return
        a = self.get_reg((instr >> 15) & 0x1F)
        b = self.get_reg((instr >> 20) & 0x1F)
        taken = (a < b) if f3 == 4 else (a >= b)      # unsigned instead of signed
        if taken:
            self.pc = u32(self.pc + self._imm_b(instr))


class JalDropsLink(RV32ISS):
    """jal/jalr do not write the return address to rd."""

    def execute(self, instr: int) -> None:
        if (instr & 0x7F) not in (0x6F, 0x67):
            super().execute(instr)
            return
        rd = (instr >> 7) & 0x1F
        super().execute(instr)
        if rd:
            self.regs[rd] = 0
            if self._current_commit is not None:
                self._current_commit.rd = rd
                self._current_commit.wdata = 0


class StoreByteClobbersWord(RV32ISS):
    """sb/sh overwrite the whole aligned word instead of one byte lane."""

    def _do_store(self, addr: int, size: int, value: int) -> None:
        if size >= 4:
            super()._do_store(addr, size, value)
            return
        aligned = addr & ~3
        self.mem[aligned:aligned + 4] = u32(value).to_bytes(4, "little")
        if self._current_commit is not None:
            self._current_commit.mem_writes.append((aligned, size, u32(value)))


class TrapMepcOffByOne(RV32ISS):
    """mepc ends up pointing past the trapping instruction."""

    def take_trap(self, cause: int, tval: int, epc: int) -> None:
        super().take_trap(cause, tval, epc)
        self.csrs["mepc"] = u32(self.csrs["mepc"] + 4)
        if self._current_commit is not None:
            self._current_commit.trap_epc = u32(epc + 4)


class EbreakNoTrap(RV32ISS):
    """ebreak executes as a nop instead of raising a breakpoint exception."""

    def execute(self, instr: int) -> None:
        if instr == 0x0010_0073:
            return
        super().execute(instr)


class Sh3addWrongShift(RV32ISS):
    """sh3add = (rs1<<2)+rs2 instead of (rs1<<3)+rs2 (CR-D3 decode off-by-one)."""

    def execute(self, instr: int) -> None:
        if ((instr & 0x7F) == 0x33 and ((instr >> 25) & 0x7F) == 0x10
                and ((instr >> 12) & 7) == 6 and self.cfg.zba_zbb):
            rd = (instr >> 7) & 0x1F
            a = self.get_reg((instr >> 15) & 0x1F)
            b = self.get_reg((instr >> 20) & 0x1F)
            self._write_rd(rd, ((a << 2) + b) & MASK32)
            return
        super().execute(instr)


class MinMaxSwapped(RV32ISS):
    """min<->max and minu<->maxu (one funct3 bit inverted)."""

    def execute(self, instr: int) -> None:
        if ((instr & 0x7F) == 0x33 and ((instr >> 25) & 0x7F) == 5):
            f3 = (instr >> 12) & 7
            if f3 in (4, 5, 6, 7):
                rd = (instr >> 7) & 0x1F
                a, b = self.get_reg((instr >> 15) & 0x1F), self.get_reg((instr >> 20) & 0x1F)
                signed = f3 in (4, 6)
                want_max = f3 in (4, 5)                       # swapped vs the ISA
                aa, bb = (sext(a, 32), sext(b, 32)) if signed else (a, b)
                self._write_rd(rd, max(aa, bb) if want_max else min(aa, bb))
                return
        super().execute(instr)


# ---------------------------------------------------------------------------
# Registry used by the detection-matrix test
# ---------------------------------------------------------------------------
# Entries the *serial, cycle-independent* differential method cannot reach, with
# the reason. The matrix test skips these but prints them: "my harness saw no
# divergence" is only meaningful for classes it can actually reach.
NOT_REACHABLE: Dict[str, str] = {
    # needs mip/irq pins driven, which the generator deliberately does not do
    # (the released core's interrupt wiring is unknown until release day)
    "policy.no_vectored_mtvec": "requires interrupt input driving",
    # mcycle/minstret are cycle-accurate quantities: RTL cycle counts do not
    # match ISS step counts, so lockstep must exclude them. Check counter
    # semantics with directed tests against the spec instead.
    "policy.no_counter_csrs": "cycle counters are not comparable ISS-vs-RTL",
}

CORE_DEVIATIONS: Dict[str, Factory] = {
    "policy.misalign_support": policy_misalign_support,
    "policy.misalign_ignore": policy_misalign_ignore,
    "policy.no_zba_zbb": policy_no_zba_zbb,
    "policy.mtval_zero": policy_mtval_zero,
    "policy.ro_csr_ignored": policy_ro_csr_ignored,
    "policy.mret_mpp_zero": policy_mret_mpp_zero,
    "policy.no_vectored_mtvec": policy_no_vectored_mtvec,
    "policy.no_counter_csrs": policy_no_counter_csrs,
    "seed.no_sign_extend_loads": NoSignExtendLoads,
    "seed.sra_is_logical": SraIsLogical,
    "seed.div_by_zero_returns_zero": DivByZeroReturnsZero,
    "seed.div_overflow_zero": DivOverflowZero,
    "seed.mulhsu_operands_swapped": MulhsuOperandsSwapped,
    "seed.branches_unsigned": BranchesUnsigned,
    "seed.jal_drops_link": JalDropsLink,
    "seed.store_byte_clobbers_word": StoreByteClobbersWord,
    "seed.trap_mepc_off_by_one": TrapMepcOffByOne,
    "seed.ebreak_no_trap": EbreakNoTrap,
    "seed.sh3add_wrong_shift": Sh3addWrongShift,
    "seed.minmax_swapped": MinMaxSwapped,
}
