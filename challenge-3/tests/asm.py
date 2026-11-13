"""
Minimal assembler / instruction encoder for RV32IM + Zicsr + Zba/Zbb (challenge-3 prep).

Why: the public testbench ships a "small Python assembler", but its coverage is unknown
and it will not emit the CR-D3 instructions on day 1. This encoder covers the full set
we need for directed and random program generation, independent of the released kit.

Features
--------
- RV32I base, M extension, Zicsr, SYSTEM, FENCE
- CR-D3: sh1add/sh2add/sh3add/min/minu/max/maxu
- Labels, `.org`, `.word`, `.align`
- Pseudo-ops: li, la, mv, nop, j, jr, ret, beqz, bnez, bgt, ble, bgtu, bleu,
  neg, not, seqz, snez, sltz, sgtz, fence.i

Usage
-----
    from asm import assemble
    prog = assemble('''
        .org 0
        start:  li   a0, 5
                li   a1, 7
                add  a2, a0, a1
                j    start
    ''', base=0x0000_0000)
    prog.words        # [(addr, word), ...]
    prog.labels       # {'start': 0}
"""

from __future__ import annotations

import re
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Sequence, Tuple

MASK32 = 0xFFFF_FFFF

REGS = {
    "zero": 0, "ra": 1, "sp": 2, "gp": 3, "tp": 4, "t0": 5, "t1": 6, "t2": 7,
    "s0": 8, "fp": 8, "s1": 9, "a0": 10, "a1": 11, "a2": 12, "a3": 13,
    "a4": 14, "a5": 15, "a6": 16, "a7": 17, "s2": 18, "s3": 19, "s4": 20,
    "s5": 21, "s6": 22, "s7": 23, "s8": 24, "s9": 25, "s10": 26, "s11": 27,
    "t3": 28, "t4": 29, "t5": 30, "t6": 31,
}
for _i in range(32):
    REGS[f"x{_i}"] = _i

CSRS = {
    "mstatus": 0x300, "misa": 0x301, "mie": 0x304, "mtvec": 0x305,
    "mscratch": 0x340, "mepc": 0x341, "mcause": 0x342, "mtval": 0x343,
    "mip": 0x344, "mhartid": 0xF14, "mcycle": 0xB00, "minstret": 0xB02,
    "mcycleh": 0xB80, "minstreth": 0xB82,
}

R_TYPE = {
    "add": (0x00, 0), "sub": (0x20, 0), "sll": (0x00, 1), "slt": (0x00, 2),
    "sltu": (0x00, 3), "xor": (0x00, 4), "srl": (0x00, 5), "sra": (0x20, 5),
    "or": (0x00, 6), "and": (0x00, 7),
    "mul": (0x01, 0), "mulh": (0x01, 1), "mulhsu": (0x01, 2), "mulhu": (0x01, 3),
    "div": (0x01, 4), "divu": (0x01, 5), "rem": (0x01, 6), "remu": (0x01, 7),
    "sh1add": (0x10, 2), "sh2add": (0x10, 4), "sh3add": (0x10, 6),
    "min": (0x05, 4), "minu": (0x05, 5), "max": (0x05, 6), "maxu": (0x05, 7),
}

I_TYPE = {"addi": 0, "slti": 2, "sltiu": 3, "xori": 4, "ori": 6, "andi": 7}
SHIFT_I = {"slli": (0x00, 1), "srli": (0x00, 5), "srai": (0x20, 5)}
LOAD = {"lb": 0, "lh": 1, "lw": 2, "lbu": 4, "lhu": 5}
STORE = {"sb": 0, "sh": 1, "sw": 2}
BRANCH = {"beq": 0, "bne": 1, "blt": 4, "bge": 5, "bltu": 6, "bgeu": 7}
CSR_OPS = {"csrrw": 1, "csrrs": 2, "csrrc": 3, "csrrwi": 5, "csrrsi": 6, "csrrci": 7}

NOP = 0x0000_0013


def reg(tok: str) -> int:
    tok = tok.strip()
    if tok in REGS:
        return REGS[tok]
    if tok.startswith("x") and tok[1:].isdigit() and 0 <= int(tok[1:]) < 32:
        return int(tok[1:])
    raise ValueError(f"bad register: {tok}")


def csr(tok: str) -> int:
    tok = tok.strip()
    if tok in CSRS:
        return CSRS[tok]
    return int(tok, 0)


def _u32(v: int) -> int:
    return v & MASK32


def _check_range(v: int, lo: int, hi: int, what: str) -> int:
    if not (lo <= v <= hi):
        raise ValueError(f"{what} out of range [{lo},{hi}]: {v}")
    return v


# ---------------------------------------------------------------------------
# Instruction encoders
# ---------------------------------------------------------------------------
def enc_r(funct7: int, rs2: int, rs1: int, funct3: int, rd: int, opcode: int = 0x33) -> int:
    return ((funct7 & 0x7F) << 25) | ((rs2 & 0x1F) << 20) | ((rs1 & 0x1F) << 15) | \
           ((funct3 & 0x7) << 12) | ((rd & 0x1F) << 7) | (opcode & 0x7F)


def enc_i(imm: int, rs1: int, funct3: int, rd: int, opcode: int) -> int:
    imm &= 0xFFF
    return ((imm & 0xFFF) << 20) | ((rs1 & 0x1F) << 15) | ((funct3 & 0x7) << 12) | \
           ((rd & 0x1F) << 7) | (opcode & 0x7F)


def enc_s(imm: int, rs2: int, rs1: int, funct3: int) -> int:
    imm &= 0xFFF
    return (((imm >> 5) & 0x7F) << 25) | ((rs2 & 0x1F) << 20) | ((rs1 & 0x1F) << 15) | \
           ((funct3 & 0x7) << 12) | ((imm & 0x1F) << 7) | 0x23


def enc_b(imm: int, rs2: int, rs1: int, funct3: int) -> int:
    imm &= 0x1FFF
    b12 = (imm >> 12) & 1
    b11 = (imm >> 11) & 1
    b10_5 = (imm >> 5) & 0x3F
    b4_1 = (imm >> 1) & 0xF
    return (b12 << 31) | (b10_5 << 25) | ((rs2 & 0x1F) << 20) | ((rs1 & 0x1F) << 15) | \
           ((funct3 & 0x7) << 12) | (b4_1 << 8) | (b11 << 7) | 0x63


def enc_u(imm20: int, rd: int, opcode: int) -> int:
    return ((imm20 & 0xFFFFF) << 12) | ((rd & 0x1F) << 7) | (opcode & 0x7F)


def enc_j(imm: int, rd: int) -> int:
    imm &= 0x1F_FFFF
    b20 = (imm >> 20) & 1
    b10_1 = (imm >> 1) & 0x3FF
    b11 = (imm >> 11) & 1
    b19_12 = (imm >> 12) & 0xFF
    return (b20 << 31) | (b19_12 << 12) | (b11 << 20) | (b10_1 << 21) | \
           ((rd & 0x1F) << 7) | 0x6F


# ---------------------------------------------------------------------------
# Two-pass assembler
# ---------------------------------------------------------------------------
@dataclass
class Program:
    base: int
    words: List[Tuple[int, int]] = field(default_factory=list)
    labels: Dict[str, int] = field(default_factory=dict)
    source_map: Dict[int, str] = field(default_factory=dict)

    def image(self) -> bytes:
        if not self.words:
            return b""
        lo = min(a for a, _ in self.words)
        hi = max(a for a, _ in self.words) + 4
        buf = bytearray(hi - lo)
        for addr, word in self.words:
            off = addr - lo
            buf[off:off + 4] = _u32(word).to_bytes(4, "little")
        return bytes(buf)

    def load_into(self, iss, addr: Optional[int] = None) -> None:
        if not self.words:
            return
        start = min(a for a, _ in self.words) if addr is None else addr
        iss.load_bytes(start, self.image())


class Assembler:
    def __init__(self, base: int = 0):
        self.base = base
        self.items: List[Tuple[int, int, Optional[str], int]] = []  # (addr, idx, label, word)
        self.labels: Dict[str, int] = {}
        self.addr = base
        self.source_map: Dict[int, str] = {}
        self._pending: List[Tuple[int, str, List[str]]] = []

    # -- parsing helpers -------------------------------------------------
    @staticmethod
    def _clean(line: str) -> str:
        line = re.split(r"(#|//|;)", line, maxsplit=1)[0]
        return line.strip()

    def _parse(self, src: str) -> None:
        for raw in src.splitlines():
            line = self._clean(raw)
            if not line:
                continue
            while ":" in line.split()[0] if line.split() else False:
                label, _, rest = line.partition(":")
                self.labels[label.strip()] = self.addr
                line = rest.strip()
                if not line:
                    break
            if not line:
                continue
            parts = line.replace(",", " ").split()
            mnem = parts[0].lower()
            args = parts[1:]

            if mnem.startswith("."):
                self._directive(mnem, args, line)
                continue
            for m, a, text in self._expand(mnem, args):
                self.source_map[self.addr] = text
                self.items.append((self.addr, len(self.items), None, 0))
                self._pending.append((self.addr, m, a))
                self.addr += 4

    _pending: List[Tuple[int, str, List[str]]]

    def _directive(self, mnem: str, args: List[str], line: str) -> None:
        if mnem == ".org":
            target = self._num(args[0], allow_label=False)
            if target < self.addr:
                raise ValueError(f".org backwards: {target:#x} < {self.addr:#x}")
            while self.addr < target:
                self._pending.append((self.addr, "nop", []))
                self.source_map[self.addr] = "nop (padding)"
                self.items.append((self.addr, len(self.items), None, 0))
                self.addr += 4
        elif mnem == ".word":
            value = self._num(args[0])
            self._pending.append((self.addr, "__word", [str(value)]))
            self.items.append((self.addr, len(self.items), None, 0))
            self.addr += 4
        elif mnem == ".align":
            n = int(args[0], 0)
            while self.addr % (1 << n) != 0:
                self._pending.append((self.addr, "nop", []))
                self.items.append((self.addr, len(self.items), None, 0))
                self.addr += 4
        else:
            raise ValueError(f"unknown directive {mnem}")

    def _num(self, tok: str, allow_label: bool = True) -> int:
        tok = tok.strip()
        if tok in self.labels and allow_label:
            return self.labels[tok]
        try:
            return int(tok, 0)
        except ValueError:
            if allow_label:
                # forward reference: resolved in pass 2
                return 0
            raise

    # -- pseudo expansion -----------------------------------------------
    def _expand(self, mnem: str, args: List[str]) -> List[Tuple[str, List[str], str]]:
        def one(m: str, a: List[str]) -> Tuple[str, List[str], str]:
            return (m, a, f"{m} {' '.join(a)}")

        if mnem == "nop":
            return [one("addi", ["x0", "x0", "0"])]
        if mnem == "mv":
            return [one("addi", [args[0], args[1], "0"])]
        if mnem == "not":
            return [one("xori", [args[0], args[1], "-1"])]
        if mnem == "neg":
            return [one("sub", [args[0], "x0", args[1]])]
        if mnem == "seqz":
            return [one("sltiu", [args[0], args[1], "1"])]
        if mnem == "snez":
            return [one("sltu", [args[0], "x0", args[1]])]
        if mnem == "sltz":
            return [one("slt", [args[0], args[1], "x0"])]
        if mnem == "sgtz":
            return [one("slt", [args[0], "x0", args[1]])]
        if mnem == "j":
            return [one("jal", ["x0", args[0]])]
        if mnem == "jr":
            return [one("jalr", ["x0", args[0], "0"])]
        if mnem == "ret":
            return [one("jalr", ["x0", "ra", "0"])]
        if mnem == "jal":
            # jal rd, label  OR  jal label
            if len(args) == 1:
                return [one("jal", ["ra", args[0]])]
            return [one("jal", [args[0], args[1]])]
        if mnem == "call":
            return [one("jal", ["ra", args[0]])]
        if mnem == "li":
            rd, imm = args[0], args[1]
            value = self._num(imm)
            if value == 0 and imm not in self.labels:
                # Not a number and not a known label: a forward reference. `li`
                # bakes the value into lui/addi during expansion, so a forward
                # label here would silently become 0. Refuse instead.
                try:
                    int(imm, 0)
                except ValueError:
                    raise ValueError(
                        f"li {rd}, {imm}: forward label reference is not supported "
                        f"(use a numeric address, or define '{imm}' earlier)")
            imm = value & MASK32
            if imm & 0xFFFF_F800:
                hi = ((imm + 0x800) >> 12) & 0xFFFFF
                lo = imm - ((hi << 12) & MASK32)
                lo = ((lo + 0x800) & 0xFFF) - 0x800
                return [one("lui", [rd, str(hi)]),
                        one("addi", [rd, rd, str(lo)])]
            return [one("addi", [rd, "x0", str(imm)])]
        if mnem == "la":
            return self._expand("li", args)
        if mnem == "beqz":
            return [one("beq", [args[0], "x0", args[1]])]
        if mnem == "bnez":
            return [one("bne", [args[0], "x0", args[1]])]
        if mnem == "bgt":
            return [one("blt", ["x0", args[0], args[1]])]
        if mnem == "ble":
            return [one("bge", ["x0", args[0], args[1]])]
        if mnem == "bgtu":
            return [one("bltu", ["x0", args[0], args[1]])]
        if mnem == "bleu":
            return [one("bgeu", ["x0", args[0], args[1]])]
        if mnem == "fence.i":
            return [one("fence.i", [])]
        # CSR pseudo-ops
        if mnem == "csrr":
            return [one("csrrs", [args[0], args[1], "x0"])]
        if mnem == "csrw":
            return [one("csrrw", ["x0", args[0], args[1]])]
        if mnem == "csrs":
            return [one("csrrs", ["x0", args[0], args[1]])]
        if mnem == "csrc":
            return [one("csrrc", ["x0", args[0], args[1]])]
        if mnem == "csrwi":
            return [one("csrrwi", ["x0", args[0], args[1]])]
        if mnem == "csrsi":
            return [one("csrrsi", ["x0", args[0], args[1]])]
        if mnem == "csrci":
            return [one("csrrci", ["x0", args[0], args[1]])]
        return [one(mnem, args)]

    # -- encoding --------------------------------------------------------
    def _encode(self, mnem: str, args: List[str], pc: int) -> int:
        m = mnem.lower()
        if m == "nop":
            return NOP
        if m == "__word":
            return self._num(args[0]) & MASK32

        # R-type
        if m in R_TYPE:
            f7, f3 = R_TYPE[m]
            return enc_r(f7, reg(args[2]), reg(args[1]), f3, reg(args[0]))

        # I-type arithmetic
        if m in I_TYPE:
            return enc_i(self._num(args[2]), reg(args[1]), I_TYPE[m], reg(args[0]), 0x13)

        # shifts
        if m in SHIFT_I:
            f7, f3 = SHIFT_I[m]
            shamt = _check_range(_u32(self._num(args[2])), 0, 31, "shift amount")
            return enc_i((f7 << 5) | shamt, reg(args[1]), f3, reg(args[0]), 0x13)

        # loads
        if m in LOAD:
            rd, off, rs1 = self._mem_args(args)
            return enc_i(off, rs1, LOAD[m], rd, 0x03)

        # stores
        if m in STORE:
            rs2, off, rs1 = self._mem_args(args, store=True)
            return enc_s(off, rs2, rs1, STORE[m])

        # branches
        if m in BRANCH:
            rs1 = reg(args[0])
            if len(args) == 3:
                rs2 = reg(args[1])
                target = self._num(args[2])
            else:
                raise ValueError(f"bad branch operands: {args}")
            off = target - pc
            if off % 2 != 0 or not (-4096 <= off <= 4094):
                raise ValueError(f"branch offset out of range: {off}")
            return enc_b(off, rs2, rs1, BRANCH[m])

        if m == "jal":
            rd = reg(args[0])
            target = self._num(args[1])
            off = target - pc
            if off % 2 != 0 or not (-1048576 <= off <= 1048574):
                raise ValueError(f"jal offset out of range: {off}")
            return enc_j(off, rd)

        if m == "jalr":
            rd = reg(args[0])
            if len(args) == 2:
                rs1, off = reg(args[1]), 0
            else:
                # two accepted forms: jalr rd, rs1, imm  /  jalr rd, imm(rs1)
                a2 = args[2]
                mm = re.match(r"(-?\w+)\s*\(\s*(\w+)\s*\)", a2)
                if mm:
                    off, rs1 = self._num(mm.group(1)), reg(mm.group(2))
                else:
                    rs1, off = reg(args[1]), self._num(args[2])
            return enc_i(off, rs1, 0, rd, 0x67)

        if m == "lui":
            return enc_u(self._num(args[1]) & 0xFFFFF, reg(args[0]), 0x37)
        if m == "auipc":
            return enc_u(self._num(args[1]) & 0xFFFFF, reg(args[0]), 0x17)

        if m in CSR_OPS:
            rd = reg(args[0])
            c = csr(args[1])
            if len(args) == 2:
                src = 0
            else:
                src = _check_range(self._num(args[2]) if CSR_OPS[m] >= 5 else reg(args[2]),
                                   0, 31 if CSR_OPS[m] >= 5 else 31, "csr source")
            return enc_i(c, src, CSR_OPS[m], rd, 0x73)

        if m == "ecall":
            return 0x0000_0073
        if m == "ebreak":
            return 0x0010_0073
        if m == "mret":
            return 0x3020_0073
        if m == "wfi":
            return 0x1050_0073
        if m == "fence":
            return 0x0FF0_000F
        if m == "fence.i":
            return 0x0000_100F

        raise ValueError(f"unknown mnemonic: {mnem}")

    def _mem_args(self, args: List[str], store: bool = False):
        """Parse `rd, imm(rs1)` or `rd, rs1, imm` / `rs2, imm(rs1)` forms."""
        first = reg(args[0])
        if len(args) == 2:
            mm = re.match(r"(-?\w+)\s*\(\s*(\w+)\s*\)", args[1])
            if mm:
                off, base = self._num(mm.group(1)), reg(mm.group(2))
            else:
                raise ValueError(f"bad memory operand: {args}")
        else:
            base, off = reg(args[1]), self._num(args[2])
        return first, off, base


def assemble(src: str, base: int = 0, pass1: Optional[Assembler] = None) -> Program:
    """Assemble source text. Two passes (labels resolved in the second)."""
    asm = Assembler(base)
    asm._pending = []
    # Pass 1: build labels (instruction sizes are fixed except pseudo li, handled in expand)
    asm._parse(src)
    # Pass 2: encode with labels known
    words: List[Tuple[int, int]] = []
    for addr, mnem, args in asm._pending:
        word = asm._encode(mnem, args, addr)
        words.append((addr, word))
    prog = Program(base=base, words=words, labels=dict(asm.labels),
                   source_map=dict(asm.source_map))
    return prog


def encode(text: str, pc: int = 0, labels: Optional[Dict[str, int]] = None) -> List[int]:
    """Encode one or more instructions without labels (handy for random generation)."""
    asm = Assembler(0)
    asm._pending = []
    asm.labels = dict(labels or {})
    for i, line in enumerate(text.splitlines()):
        line = Assembler._clean(line)
        if not line:
            continue
        parts = line.replace(",", " ").split()
        for m, a, _ in asm._expand(parts[0], parts[1:]):
            asm._pending.append((pc + 4 * i, m, a))
    return [asm._encode(m, a, at) for at, m, a in asm._pending]
