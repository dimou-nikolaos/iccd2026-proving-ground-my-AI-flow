"""A small RISC-V assembler for writing test programs in Python."""

TOHOST = 0xFFFF0000      # a store here ends the program
DEVICE_IRQ = 0xFFFF0004  # a store here drives irq_ext (bit 0) and irq_timer (bit 1)
SIG_BASE = 0x00004000    # dump() stores registers here


def _r(f7, rs2, rs1, f3, rd, op):
    return f7 << 25 | rs2 << 20 | rs1 << 15 | f3 << 12 | rd << 7 | op


def _i(imm, rs1, f3, rd, op):
    return (imm & 0xFFF) << 20 | rs1 << 15 | f3 << 12 | rd << 7 | op


def _s(imm, rs2, rs1, f3, op):
    return ((imm >> 5) & 0x7F) << 25 | rs2 << 20 | rs1 << 15 | f3 << 12 | (imm & 0x1F) << 7 | op


def _b(imm, rs2, rs1, f3):
    return ((imm >> 12) & 1) << 31 | ((imm >> 5) & 0x3F) << 25 | rs2 << 20 | rs1 << 15 | f3 << 12 \
        | ((imm >> 1) & 0xF) << 8 | ((imm >> 11) & 1) << 7 | 0x63


def _j(imm, rd):
    return ((imm >> 20) & 1) << 31 | ((imm >> 1) & 0x3FF) << 21 | ((imm >> 11) & 1) << 20 \
        | ((imm >> 12) & 0xFF) << 12 | rd << 7 | 0x6F


class Asm:
    def __init__(self, base=0):
        self.base = base
        self.items = []
        self.labels = {}

    def here(self):
        return self.base + 4 * len(self.items)

    def label(self, name):
        self.labels[name] = self.here()

    def word(self, value):
        self.items.append(value & 0xFFFFFFFF)

    def build(self):
        out = []
        for i, it in enumerate(self.items):
            out.append(it(self.base + 4 * i) if callable(it) else it)
        return out

    def _target(self, target, pc):
        return (self.labels[target] if isinstance(target, str) else target) - pc


def _add_ops():
    for name, f7, f3 in [("add", 0, 0), ("sub", 0x20, 0), ("sll", 0, 1), ("slt", 0, 2), ("sltu", 0, 3),
                         ("xor", 0, 4), ("srl", 0, 5), ("sra", 0x20, 5), ("or_", 0, 6), ("and_", 0, 7),
                         ("mul", 1, 0), ("mulh", 1, 1), ("mulhsu", 1, 2), ("mulhu", 1, 3),
                         ("div", 1, 4), ("divu", 1, 5), ("rem", 1, 6), ("remu", 1, 7)]:
        setattr(Asm, name, lambda s, rd, rs1, rs2, _f7=f7, _f3=f3: s.word(_r(_f7, rs2, rs1, _f3, rd, 0x33)))
    for name, f3 in [("addi", 0), ("slti", 2), ("sltiu", 3), ("xori", 4), ("ori", 6), ("andi", 7)]:
        setattr(Asm, name, lambda s, rd, rs1, imm, _f3=f3: s.word(_i(imm, rs1, _f3, rd, 0x13)))
    for name, f7, f3 in [("slli", 0, 1), ("srli", 0, 5), ("srai", 0x20, 5)]:
        setattr(Asm, name, lambda s, rd, rs1, sh, _f7=f7, _f3=f3: s.word(_r(_f7, sh, rs1, _f3, rd, 0x13)))
    for name, f3 in [("lb", 0), ("lh", 1), ("lw", 2), ("lbu", 4), ("lhu", 5)]:
        setattr(Asm, name, lambda s, rd, rs1, imm, _f3=f3: s.word(_i(imm, rs1, _f3, rd, 0x03)))
    for name, f3 in [("sb", 0), ("sh", 1), ("sw", 2)]:
        setattr(Asm, name, lambda s, rs2, rs1, imm, _f3=f3: s.word(_s(imm, rs2, rs1, _f3, 0x23)))
    for name, f3 in [("beq", 0), ("bne", 1), ("blt", 4), ("bge", 5), ("bltu", 6), ("bgeu", 7)]:
        setattr(Asm, name, lambda s, rs1, rs2, t, _f3=f3:
                s.items.append(lambda pc, a=s, r1=rs1, r2=rs2, tg=t, f=_f3: _b(a._target(tg, pc), r2, r1, f)))
    for name, f3 in [("csrrw", 1), ("csrrs", 2), ("csrrc", 3), ("csrrwi", 5), ("csrrsi", 6), ("csrrci", 7)]:
        setattr(Asm, name, lambda s, rd, csr, rs1, _f3=f3: s.word(_i(csr, rs1, _f3, rd, 0x73)))


_add_ops()


def _lui(self, rd, imm):
    self.word((imm & 0xFFFFF) << 12 | rd << 7 | 0x37)


def _auipc(self, rd, imm):
    self.word((imm & 0xFFFFF) << 12 | rd << 7 | 0x17)


def _jal(self, rd, target):
    self.items.append(lambda pc, a=self, t=target, r=rd: _j(a._target(t, pc), r))


def _jalr(self, rd, rs1, imm=0):
    self.word(_i(imm, rs1, 0, rd, 0x67))


def _li(self, rd, value):
    """Load a 32-bit constant in two instructions."""
    value &= 0xFFFFFFFF
    low = value & 0xFFF
    if low >= 0x800:
        low -= 0x1000
    self.lui(rd, ((value - low) >> 12) & 0xFFFFF)
    self.addi(rd, rd, low)


def _la(self, rd, target):
    """Load the address of a label."""
    def hi(pc, a=self, t=target, r=rd):
        addr = a.labels[t]
        low = addr & 0xFFF
        low = low - 0x1000 if low >= 0x800 else low
        return ((addr - low) & 0xFFFFF000) | r << 7 | 0x37

    def lo(pc, a=self, t=target, r=rd):
        low = a.labels[t] & 0xFFF
        low = low - 0x1000 if low >= 0x800 else low
        return _i(low, r, 0, r, 0x13)
    self.items.append(hi)
    self.items.append(lo)


def _dump(self, regs):
    """Store the listed registers to SIG_BASE, in order. Uses x31 as the pointer."""
    self.li(31, SIG_BASE)
    for i, r in enumerate(regs):
        self.sw(r, 31, 4 * i)


def _halt(self):
    self.li(31, TOHOST)
    self.sw(0, 31, 0)
    self.label(f"_halt{len(self.items)}")
    self.jal(0, self.here())


def _set_irq(self, bits, tmp=30):
    self.li(31, DEVICE_IRQ)
    self.addi(tmp, 0, bits)
    self.sw(tmp, 31, 0)


Asm.lui, Asm.auipc, Asm.jal, Asm.jalr, Asm.li, Asm.la = _lui, _auipc, _jal, _jalr, _li, _la
Asm.dump, Asm.halt, Asm.set_irq = _dump, _halt, _set_irq
Asm.nop = lambda s: s.addi(0, 0, 0)
Asm.ecall = lambda s: s.word(0x00000073)
Asm.ebreak = lambda s: s.word(0x00100073)
Asm.mret = lambda s: s.word(0x30200073)
Asm.wfi = lambda s: s.word(0x10500073)
Asm.fence = lambda s: s.word(0x0000000F)

MSTATUS, MISA, MIE, MTVEC, MSCRATCH, MEPC, MCAUSE, MTVAL, MIP = (
    0x300, 0x301, 0x304, 0x305, 0x340, 0x341, 0x342, 0x343, 0x344)
MCYCLE, MINSTRET, MCYCLEH, MINSTRETH, MHARTID = 0xB00, 0xB02, 0xB80, 0xB82, 0xF14
