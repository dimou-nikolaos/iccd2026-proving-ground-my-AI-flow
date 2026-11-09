"""
RV32IM + Zicsr golden instruction-set simulator (challenge-3 prep).

Purpose
-------
Architectural-state-accurate reference for `rv_core` differential testing.
NOT cycle-accurate: it models instruction retirement order and architectural
effects (registers, memory, CSRs, traps), which is what seeded RTL deviations
are almost always visible in.

Implemented ISA
---------------
- RV32I base (all opcodes, FENCE/FENCE.I as no-ops, ECALL/EBREAK/MRET/WFI)
- M extension (all 8: MUL/MULH/MULHSU/MULHU/DIV/DIVU/REM/REMU)
- Zicsr (CSRRW/S/C + immediate forms)
- Machine-mode traps and interrupts (mstatus/mie/mip/mtvec/mepc/mcause/mtval/mscratch)
- Zba/Zbb CR-D3 instructions (SH1ADD/SH2ADD/SH3ADD/MIN/MINU/MAX/MAXU), enabled by
  default but switchable so the ISS also models the *baseline* (they trap as illegal).

Configurable policy (things the released spec may define differently)
--------------------------------------------------------------------
- misaligned: "fault" (spec default), "ignore" (mask low address bits), or "support"
- mtval_illegal: "instr" (mtval=encoding, spec default) or "zero"
- ro_csr_write: "illegal" (default) or "ignore"
- mpp_on_mret: value written to mstatus.MPP on MRET (M-only cores usually 3)
- zba_zbb: True/False (CR-D3 implemented or not)

Usage
-----
    iss = RV32ISS()
    iss.load_words(0x0, words)          # little-endian 32-bit words
    iss.run(max_steps=100_000)
    print(hex(iss.regs[10]), iss.csrs["mepc"])
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple

MASK32 = 0xFFFF_FFFF


def sext(value: int, bits: int) -> int:
    """Sign-extend `value` (already masked to `bits`) to 32 bits."""
    sign = 1 << (bits - 1)
    value &= (1 << bits) - 1
    return (value ^ sign) - sign


def u32(value: int) -> int:
    return value & MASK32


def s32(value: int) -> int:
    value &= MASK32
    return value - (1 << 32) if value & 0x8000_0000 else value


# ---------------------------------------------------------------------------
# Trap / interrupt causes
# ---------------------------------------------------------------------------
CAUSE_INSTR_MISALIGNED = 0
CAUSE_INSTR_ACCESS = 1
CAUSE_ILLEGAL_INSTR = 2
CAUSE_BREAKPOINT = 3
CAUSE_LOAD_MISALIGNED = 4
CAUSE_LOAD_ACCESS = 5
CAUSE_STORE_MISALIGNED = 6
CAUSE_STORE_ACCESS = 7
CAUSE_ECALL_M = 11

CAUSE_NAMES = {
    CAUSE_INSTR_MISALIGNED: "instr_misaligned",
    CAUSE_INSTR_ACCESS: "instr_access_fault",
    CAUSE_ILLEGAL_INSTR: "illegal_instruction",
    CAUSE_BREAKPOINT: "breakpoint",
    CAUSE_LOAD_MISALIGNED: "load_misaligned",
    CAUSE_LOAD_ACCESS: "load_access_fault",
    CAUSE_STORE_MISALIGNED: "store_misaligned",
    CAUSE_STORE_ACCESS: "store_access_fault",
    CAUSE_ECALL_M: "ecall_m",
}

# mstatus bit positions
MSTATUS_MIE = 1 << 3
MSTATUS_MPIE = 1 << 7
MSTATUS_MPP = 3 << 11

MIP_MSI = 1 << 3
MIP_MTI = 1 << 7
MIP_MEI = 1 << 11

# Interrupt priority order (highest first), per privileged spec ordering.
IRQ_PRIORITY = [11, 3, 7, 9, 1, 5]


class Trap(Exception):
    """Raised by instruction execution to request a synchronous trap."""

    def __init__(self, cause: int, tval: int = 0):
        super().__init__(f"trap cause={cause} tval=0x{tval:08x}")
        self.cause = cause
        self.tval = u32(tval)


@dataclass
class Commit:
    """Architectural effect of one retired (or trapping) instruction."""

    pc: int
    instr: int
    rd: Optional[int] = None
    wdata: int = 0
    mem_writes: List[Tuple[int, int, int]] = field(default_factory=list)  # (addr, size, value)
    trap_cause: Optional[int] = None
    trap_epc: Optional[int] = None
    csr_writes: List[Tuple[int, int]] = field(default_factory=list)  # (csr_addr, value)


@dataclass
class RV32Config:
    misaligned: str = "fault"        # "fault" | "ignore" | "support"
    mtval_illegal: str = "instr"     # "instr" | "zero"
    ro_csr_write: str = "illegal"    # "illegal" | "ignore"
    mpp_on_mret: int = 3             # M-only cores typically keep MPP=3
    zba_zbb: bool = True             # CR-D3 instructions enabled
    mtvec_vectored: bool = True      # honour mtvec[1:0]==1 for interrupts
    counter_csrs: bool = True        # expose mcycle/minstret as read-only


class RV32ISS:
    # ------------------------------------------------------------------
    # Construction / reset
    # ------------------------------------------------------------------
    def __init__(self, mem_size: int = 1 << 20, config: Optional[RV32Config] = None):
        self.cfg = config or RV32Config()
        self.mem = bytearray(mem_size)
        self._reset_state()

    def _reset_state(self) -> None:
        self.regs = [0] * 32
        self.pc = 0
        self.cycles = 0
        self.instret = 0
        self.halted = False
        self.exit_code: Optional[int] = None

        # Minimal machine-mode CSR file. writable_mask controls which bits can change.
        self.csr_reset: Dict[str, int] = {
            "mstatus": 0,
            "misa": 0x4000_1100,   # MXL=1 (RV32), I (bit 8), M (bit 12)
            "mie": 0,
            "mtvec": 0,
            "mscratch": 0,
            "mepc": 0,
            "mcause": 0,
            "mtval": 0,
            "mip": 0,
            "mhartid": 0,
            "mvendorid": 0,
            "marchid": 0,
            "mimpid": 0,
            "mcycle": 0,
            "minstret": 0,
        }
        self.csr_writable: Dict[str, int] = {
            "mstatus": MSTATUS_MIE | MSTATUS_MPIE | MSTATUS_MPP,
            "misa": 0,
            "mie": MIP_MSI | MIP_MTI | MIP_MEI,
            "mtvec": MASK32,
            "mscratch": MASK32,
            "mepc": MASK32 & ~0x3,
            "mcause": 0,
            "mtval": 0,
            "mip": 0,               # external sources drive this
            "mhartid": 0,
            "mvendorid": 0,
            "marchid": 0,
            "mimpid": 0,
            "mcycle": 0,
            "minstret": 0,
        }
        self.csr_addr: Dict[int, str] = {
            0x300: "mstatus",
            0x301: "misa",
            0x304: "mie",
            0x305: "mtvec",
            0x340: "mscratch",
            0x341: "mepc",
            0x342: "mcause",
            0x343: "mtval",
            0x344: "mip",
            0xF14: "mhartid",
            0xF11: "mvendorid",
            0xF12: "marchid",
            0xF13: "mimpid",
            0xB00: "mcycle",
            0xB02: "minstret",
        }
        self.csrs: Dict[str, int] = dict(self.csr_reset)
        self.irq_shadow = 0          # instructions during which IRQs are masked after MRET
        self.external_mip = 0        # bitmask driven by testbench irq lines
        self.trace: List[Commit] = []
        self._trap_epc = 0
        self._current_commit: Optional[Commit] = None

    def reset(self) -> None:
        self._reset_state()

    # ------------------------------------------------------------------
    # Memory helpers (little-endian, byte-addressed)
    # ------------------------------------------------------------------
    def load_bytes(self, addr: int, data: bytes) -> None:
        self.mem[addr:addr + len(data)] = data

    def load_words(self, addr: int, words: List[int]) -> None:
        for i, w in enumerate(words):
            self.write_mem(addr + 4 * i, u32(w), 4, check=False)

    def read_mem(self, addr: int, size: int) -> int:
        addr &= MASK32
        data = self.mem[addr:addr + size]
        if len(data) != size:
            raise Trap(CAUSE_LOAD_ACCESS, addr)
        return int.from_bytes(data, "little")

    def write_mem(self, addr: int, value: int, size: int, check: bool = True) -> None:
        addr &= MASK32
        if check:
            if addr + size > len(self.mem):
                raise Trap(CAUSE_STORE_ACCESS, addr)
            if not (0 <= addr and addr + size <= len(self.mem)):
                raise Trap(CAUSE_STORE_ACCESS, addr)
        self.mem[addr:addr + size] = (value & ((1 << (8 * size)) - 1)).to_bytes(size, "little")

    def fetch32(self, addr: int) -> int:
        if addr + 4 > len(self.mem):
            raise Trap(CAUSE_INSTR_ACCESS, addr)
        return int.from_bytes(self.mem[addr:addr + 4], "little")

    # ------------------------------------------------------------------
    # CSR helpers
    # ------------------------------------------------------------------
    def read_csr(self, addr: int) -> int:
        name = self.csr_addr.get(addr)
        if name is None:
            raise Trap(CAUSE_ILLEGAL_INSTR, 0)
        if name == "mcycle":
            return self.cycles & MASK32
        if name == "minstret":
            return self.instret & MASK32
        return self.csrs.get(name, 0) & MASK32

    def write_csr(self, addr: int, value: int) -> None:
        name = self.csr_addr.get(addr)
        if name is None:
            raise Trap(CAUSE_ILLEGAL_INSTR, 0)
        mask = self.csr_writable.get(name, 0)
        if mask == 0 and self.cfg.ro_csr_write == "illegal":
            raise Trap(CAUSE_ILLEGAL_INSTR, 0)
        if name in ("mcycle", "minstret"):
            return  # read-only counters
        self.csrs[name] = (self.csrs.get(name, 0) & ~mask) | (u32(value) & mask)

    def get_reg(self, idx: int) -> int:
        return 0 if idx == 0 else self.regs[idx]

    def set_reg(self, idx: int, value: int) -> None:
        if idx != 0:
            self.regs[idx] = u32(value)

    # ------------------------------------------------------------------
    # Interrupts
    # ------------------------------------------------------------------
    def set_external_irq(self, bit: int, level: bool) -> None:
        if level:
            self.external_mip |= (1 << bit)
        else:
            self.external_mip &= ~(1 << bit)

    def pending_irq(self) -> Optional[int]:
        mip = (self.csrs.get("mip", 0) | self.external_mip) & MASK32
        mie = self.csrs.get("mie", 0) & MASK32
        if not (self.csrs["mstatus"] & MSTATUS_MIE):
            return None
        active = mip & mie
        if not active:
            return None
        for bit in IRQ_PRIORITY:
            if active & (1 << bit):
                return bit
        return None

    # ------------------------------------------------------------------
    # Traps
    # ------------------------------------------------------------------
    def take_trap(self, cause: int, tval: int, epc: int) -> None:
        mstatus = self.csrs["mstatus"]
        mpie = MSTATUS_MPIE if (mstatus & MSTATUS_MIE) else 0
        mstatus = (mstatus & ~(MSTATUS_MIE | MSTATUS_MPIE | MSTATUS_MPP))
        mstatus |= mpie | (3 << 11)
        self.csrs["mstatus"] = mstatus
        self.csrs["mepc"] = epc & ~0x3
        self.csrs["mcause"] = cause & MASK32
        self.csrs["mtval"] = u32(tval)

        mtvec = self.csrs["mtvec"]
        base = mtvec & ~0x3
        if self.cfg.mtvec_vectored and (mtvec & 0x3) == 1 and (cause & 0x8000_0000):
            self.pc = u32(base + 4 * (cause & 0x7FFF_FFFF))
        else:
            self.pc = u32(base)

        if self._current_commit is not None:
            self._current_commit.trap_cause = cause
            self._current_commit.trap_epc = epc

    # ------------------------------------------------------------------
    # Execution
    # ------------------------------------------------------------------
    def step(self) -> Optional[Commit]:
        """Execute one instruction. Returns the Commit record."""
        if self.halted:
            return None

        shadowed = self.irq_shadow > 0
        if shadowed:
            self.irq_shadow -= 1

        # Interrupts are taken at instruction boundaries, before the next instruction.
        if not shadowed:
            irq = self.pending_irq()
            if irq is not None:
                self._current_commit = Commit(pc=self.pc, instr=0)
                self.take_trap(0x8000_0000 | irq, 0, self.pc)
                self._current_commit.trap_epc = self.pc
                self.cycles += 1
                self.trace.append(self._current_commit)
                commit = self._current_commit
                self._current_commit = None
                return commit

        pc = self.pc
        try:
            instr = self.fetch32(pc)
        except Trap as t:
            self._current_commit = Commit(pc=pc, instr=0)
            self.take_trap(t.cause, t.tval, pc)
            self.cycles += 1
            self.trace.append(self._current_commit)
            commit = self._current_commit
            self._current_commit = None
            return commit

        self._current_commit = Commit(pc=pc, instr=instr)
        self._trap_epc = pc
        self.pc = u32(pc + 4)

        try:
            self.execute(instr)
            self.instret += 1
            self._current_commit.rd = self._current_commit.rd  # already set by execute
        except Trap as t:
            # Revert PC to the trapping instruction's address for mepc.
            self.pc = u32(pc)
            self.take_trap(t.cause, t.tval, pc)
            # pc is now the handler; don't restore.
            self._current_commit.trap_epc = pc
            if t.cause == CAUSE_ILLEGAL_INSTR and self.cfg.mtval_illegal == "instr":
                self.csrs["mtval"] = instr
                self._current_commit.trap_epc = pc

        self.cycles += 1
        commit = self._current_commit
        self.trace.append(commit)
        self._current_commit = None
        return commit

    def run(self, max_steps: int = 1_000_000, stop_pc: Optional[int] = None) -> None:
        steps = 0
        while not self.halted and steps < max_steps:
            if stop_pc is not None and self.pc == stop_pc:
                break
            self.step()
            steps += 1

    # -- helpers used by execute ---------------------------------------
    def _write_rd(self, rd: int, value: int) -> None:
        if rd != 0:
            self.regs[rd] = u32(value)
            if self._current_commit is not None:
                self._current_commit.rd = rd
                self._current_commit.wdata = u32(value)

    def _check_load_align(self, addr: int, size: int) -> int:
        if self.cfg.misaligned == "fault" and (addr % size) != 0:
            raise Trap(CAUSE_LOAD_MISALIGNED, addr)
        if self.cfg.misaligned == "ignore":
            return addr & ~(size - 1)
        return addr

    def _check_store_align(self, addr: int, size: int) -> int:
        if self.cfg.misaligned == "fault" and (addr % size) != 0:
            raise Trap(CAUSE_STORE_MISALIGNED, addr)
        if self.cfg.misaligned == "ignore":
            return addr & ~(size - 1)
        return addr

    def _do_load(self, addr: int, size: int, signed: bool) -> int:
        addr = self._check_load_align(addr, size)
        if self.cfg.misaligned == "support":
            value = self.read_mem_unaligned(addr, size)
        else:
            value = self.read_mem(addr, size)
        return sext(value, 8 * size) if signed else value

    def read_mem_unaligned(self, addr: int, size: int) -> int:
        data = bytearray()
        for i in range(size):
            if addr + i >= len(self.mem):
                raise Trap(CAUSE_LOAD_ACCESS, addr)
            data.append(self.mem[addr + i])
        return int.from_bytes(bytes(data), "little")

    def _do_store(self, addr: int, size: int, value: int) -> None:
        addr = self._check_store_align(addr, size)
        if self.cfg.misaligned == "support":
            for i in range(size):
                if addr + i >= len(self.mem):
                    raise Trap(CAUSE_STORE_ACCESS, addr)
                self.mem[addr + i] = (value >> (8 * i)) & 0xFF
        else:
            self.write_mem(addr, value, size)
        if self._current_commit is not None:
            self._current_commit.mem_writes.append((addr, size, value & ((1 << (8 * size)) - 1)))

    # ------------------------------------------------------------------
    # Decode / execute
    # ------------------------------------------------------------------
    def execute(self, instr: int) -> None:
        opcode = instr & 0x7F
        rd = (instr >> 7) & 0x1F
        funct3 = (instr >> 12) & 0x7
        rs1 = (instr >> 15) & 0x1F
        rs2 = (instr >> 20) & 0x1F
        funct7 = (instr >> 25) & 0x7F
        a = self.get_reg(rs1)
        b = self.get_reg(rs2)

        # ---------------- LUI / AUIPC ----------------
        if opcode == 0x37:  # LUI
            self._write_rd(rd, instr & 0xFFFF_F000)
            return
        if opcode == 0x17:  # AUIPC
            self._write_rd(rd, self._trap_epc + (instr & 0xFFFF_F000))
            return

        # ---------------- Jumps ----------------
        if opcode == 0x6F:  # JAL
            imm = self._imm_j(instr)
            target = u32(self._trap_epc + imm)
            self._check_target(target)   # trap before any architectural side effect
            self._write_rd(rd, u32(self._trap_epc + 4))
            self.pc = target
            return
        if opcode == 0x67:  # JALR
            if funct3 != 0:
                raise Trap(CAUSE_ILLEGAL_INSTR, instr)
            imm = sext((instr >> 20) & 0xFFF, 12)
            target = u32((a + imm) & ~1)
            if target % 4 != 0:
                raise Trap(CAUSE_INSTR_MISALIGNED, target)
            self._write_rd(rd, u32(self._trap_epc + 4))
            self.pc = target
            return

        # ---------------- Branches ----------------
        if opcode == 0x63:
            imm = self._imm_b(instr)
            taken = False
            if funct3 == 0:    # BEQ
                taken = a == b
            elif funct3 == 1:  # BNE
                taken = a != b
            elif funct3 == 4:  # BLT
                taken = s32(a) < s32(b)
            elif funct3 == 5:  # BGE
                taken = s32(a) >= s32(b)
            elif funct3 == 6:  # BLTU
                taken = a < b
            elif funct3 == 7:  # BGEU
                taken = a >= b
            else:
                raise Trap(CAUSE_ILLEGAL_INSTR, instr)
            if taken:
                target = u32(self._trap_epc + imm)
                self.pc = target
                if target % 4 != 0:
                    raise Trap(CAUSE_INSTR_MISALIGNED, target)
            return

        # ---------------- Loads ----------------
        if opcode == 0x03:
            imm = sext((instr >> 20) & 0xFFF, 12)
            addr = u32(a + imm)
            if funct3 == 0:    # LB
                self._write_rd(rd, self._do_load(addr, 1, True))
            elif funct3 == 1:  # LH
                self._write_rd(rd, self._do_load(addr, 2, True))
            elif funct3 == 2:  # LW
                self._write_rd(rd, self._do_load(addr, 4, True))
            elif funct3 == 4:  # LBU
                self._write_rd(rd, self._do_load(addr, 1, False))
            elif funct3 == 5:  # LHU
                self._write_rd(rd, self._do_load(addr, 2, False))
            else:
                raise Trap(CAUSE_ILLEGAL_INSTR, instr)
            return

        # ---------------- Stores ----------------
        if opcode == 0x23:
            imm = sext(((instr >> 25) << 5) | ((instr >> 7) & 0x1F), 12)
            addr = u32(a + imm)
            if funct3 == 0:    # SB
                self._do_store(addr, 1, b)
            elif funct3 == 1:  # SH
                self._do_store(addr, 2, b)
            elif funct3 == 2:  # SW
                self._do_store(addr, 4, b)
            else:
                raise Trap(CAUSE_ILLEGAL_INSTR, instr)
            return

        # ---------------- OP-IMM ----------------
        if opcode == 0x13:
            imm = sext(instr >> 20, 12)
            shamt = (instr >> 20) & 0x1F
            if funct3 == 0:    # ADDI
                self._write_rd(rd, a + imm)
            elif funct3 == 2:  # SLTI
                self._write_rd(rd, int(s32(a) < imm))
            elif funct3 == 3:  # SLTIU
                self._write_rd(rd, int(a < u32(imm)))
            elif funct3 == 4:  # XORI
                self._write_rd(rd, a ^ u32(imm))
            elif funct3 == 6:  # ORI
                self._write_rd(rd, a | u32(imm))
            elif funct3 == 7:  # ANDI
                self._write_rd(rd, a & u32(imm))
            elif funct3 == 1:  # SLLI
                if funct7 != 0:
                    raise Trap(CAUSE_ILLEGAL_INSTR, instr)
                self._write_rd(rd, a << shamt)
            elif funct3 == 5:  # SRLI / SRAI
                if funct7 == 0x00:
                    self._write_rd(rd, a >> shamt)
                elif funct7 == 0x20:
                    self._write_rd(rd, s32(a) >> shamt)
                else:
                    raise Trap(CAUSE_ILLEGAL_INSTR, instr)
            else:
                raise Trap(CAUSE_ILLEGAL_INSTR, instr)
            return

        # ---------------- OP ----------------
        if opcode == 0x33:
            if funct7 == 0x00:
                if funct3 == 0:    # ADD
                    self._write_rd(rd, a + b)
                elif funct3 == 1:  # SLL
                    self._write_rd(rd, a << (b & 0x1F))
                elif funct3 == 2:  # SLT
                    self._write_rd(rd, int(s32(a) < s32(b)))
                elif funct3 == 3:  # SLTU
                    self._write_rd(rd, int(a < b))
                elif funct3 == 4:  # XOR
                    self._write_rd(rd, a ^ b)
                elif funct3 == 5:  # SRL
                    self._write_rd(rd, a >> (b & 0x1F))
                elif funct3 == 6:  # OR
                    self._write_rd(rd, a | b)
                elif funct3 == 7:  # AND
                    self._write_rd(rd, a & b)
                return
            if funct7 == 0x20:
                if funct3 == 0:    # SUB
                    self._write_rd(rd, a - b)
                elif funct3 == 5:  # SRA
                    self._write_rd(rd, s32(a) >> (b & 0x1F))
                else:
                    raise Trap(CAUSE_ILLEGAL_INSTR, instr)
                return
            if funct7 == 0x01:  # M extension
                if funct3 == 0:    # MUL
                    self._write_rd(rd, (s32(a) * s32(b)) & MASK32)
                elif funct3 == 1:  # MULH
                    self._write_rd(rd, ((s32(a) * s32(b)) >> 32) & MASK32)
                elif funct3 == 2:  # MULHSU
                    self._write_rd(rd, ((s32(a) * b) >> 32) & MASK32)
                elif funct3 == 3:  # MULHU
                    self._write_rd(rd, ((a * b) >> 32) & MASK32)
                elif funct3 == 4:  # DIV
                    self._write_rd(rd, self._div(a, b))
                elif funct3 == 5:  # DIVU
                    self._write_rd(rd, (a // b) if b else MASK32)
                elif funct3 == 6:  # REM
                    self._write_rd(rd, self._rem(a, b))
                elif funct3 == 7:  # REMU
                    self._write_rd(rd, (a % b) if b else a)
                return
            if funct7 == 0x10 and self.cfg.zba_zbb:  # Zba SHxADD
                if funct3 == 2:    # SH1ADD
                    self._write_rd(rd, (a << 1) + b)
                elif funct3 == 4:  # SH2ADD
                    self._write_rd(rd, (a << 2) + b)
                elif funct3 == 6:  # SH3ADD
                    self._write_rd(rd, (a << 3) + b)
                else:
                    raise Trap(CAUSE_ILLEGAL_INSTR, instr)
                return
            if funct7 == 0x05 and self.cfg.zba_zbb:  # Zbb MIN/MAX
                if funct3 == 4:    # MIN
                    self._write_rd(rd, a if s32(a) < s32(b) else b)
                elif funct3 == 5:  # MINU
                    self._write_rd(rd, a if a < b else b)
                elif funct3 == 6:  # MAX
                    self._write_rd(rd, a if s32(a) > s32(b) else b)
                elif funct3 == 7:  # MAXU
                    self._write_rd(rd, a if a > b else b)
                else:
                    raise Trap(CAUSE_ILLEGAL_INSTR, instr)
                return
            raise Trap(CAUSE_ILLEGAL_INSTR, instr)

        # ---------------- FENCE ----------------
        if opcode == 0x0F:
            return  # FENCE / FENCE.I are no-ops in this model

        # ---------------- SYSTEM ----------------
        if opcode == 0x73:
            if funct3 == 0:
                if instr == 0x0000_0073:  # ECALL
                    raise Trap(CAUSE_ECALL_M, 0)
                if instr == 0x0010_0073:  # EBREAK
                    raise Trap(CAUSE_BREAKPOINT, self.pc)
                if instr == 0x3020_0073:  # MRET
                    mstatus = self.csrs["mstatus"]
                    mie = MSTATUS_MIE if (mstatus & MSTATUS_MPIE) else 0
                    # MIE <- MPIE, MPIE <- 1, MPP <- configured
                    mstatus = (mstatus & ~(MSTATUS_MIE | MSTATUS_MPIE | MSTATUS_MPP))
                    mstatus |= mie | MSTATUS_MPIE | (self.cfg.mpp_on_mret << 11)
                    self.csrs["mstatus"] = mstatus
                    self.pc = self.csrs["mepc"]
                    self.irq_shadow = 1
                    return
                if instr == 0x1050_0073:  # WFI (treated as NOP)
                    return
                raise Trap(CAUSE_ILLEGAL_INSTR, instr)

            # CSR instructions
            csr = (instr >> 20) & 0xFFF
            old = self.read_csr(csr)
            src = a if funct3 & 0x4 == 0 else rs1  # immediate forms use zimm in rs1 field
            if funct3 == 1:    # CSRRW
                new = src
            elif funct3 == 2:  # CSRRS
                new = old | src
            elif funct3 == 3:  # CSRRC
                new = old & ~src
            elif funct3 == 5:  # CSRRWI
                new = src
            elif funct3 == 6:  # CSRRSI
                new = old | src
            elif funct3 == 7:  # CSRRCI
                new = old & ~src
            else:
                raise Trap(CAUSE_ILLEGAL_INSTR, instr)

            if funct3 in (1, 5) or rs1 != 0:  # CSRRW always writes; R/S/C only if rs1 != 0
                self.write_csr(csr, new)
                if self._current_commit is not None:
                    self._current_commit.csr_writes.append((csr, u32(new)))
            self._write_rd(rd, old)
            return

        raise Trap(CAUSE_ILLEGAL_INSTR, instr)

    def _check_target(self, target: int) -> None:
        if target % 4 != 0:
            raise Trap(CAUSE_INSTR_MISALIGNED, target)

    @staticmethod
    def _div(a: int, b: int) -> int:
        if b == 0:
            return MASK32
        sa, sb = s32(a), s32(b)
        if sa == -(1 << 31) and sb == -1:
            return 0x8000_0000
        q = abs(sa) // abs(sb)
        if (sa < 0) != (sb < 0):
            q = -q
        return q & MASK32

    @staticmethod
    def _rem(a: int, b: int) -> int:
        if b == 0:
            return a
        sa, sb = s32(a), s32(b)
        if sa == -(1 << 31) and sb == -1:
            return 0
        r = abs(sa) % abs(sb)
        if sa < 0:
            r = -r
        return r & MASK32

    # ------------------------------------------------------------------
    # Immediate decoders
    # ------------------------------------------------------------------
    @staticmethod
    def _imm_j(instr: int) -> int:
        imm = (((instr >> 31) & 0x1) << 20) | (((instr >> 12) & 0xFF) << 12)
        imm |= (((instr >> 20) & 0x1) << 11) | (((instr >> 21) & 0x3FF) << 1)
        return sext(imm, 21)

    @staticmethod
    def _imm_b(instr: int) -> int:
        imm = (((instr >> 31) & 0x1) << 12) | (((instr >> 7) & 0x1) << 11)
        imm |= (((instr >> 25) & 0x3F) << 5) | (((instr >> 8) & 0xF) << 1)
        return sext(imm, 13)
