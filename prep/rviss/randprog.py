"""
Constrained-random RV32IM(+CR-D3) program generator (challenge-3 prep).

Two flavours, both self-contained (they end in an infinite self-loop and never
call the OS):

1. **trap-free mix** (default): a random mix of ALU / shift / M / Zba-Zbb /
   load / store / branch ops inside one scratch region, hazard-dense by
   construction (destination-reuse bias) so forwarding, load-use stalls and
   branch flushes get exercised. `test_randprog` asserts these produce *no*
   trap, which is what makes them a clean baseline for final-state comparison.

2. **trap / CSR / policy-corner mix** (`trap_mode=True`, or `trap_config()`):
   adds a machine-mode trap handler, the misaligned / sub-word / illegal-encoding
   / ebreak / CSR-corner / divide-corner instruction classes, and a jal/jalr
   call-return block. This is where seeded core deviations live in practice:
   trap-entry policy, CSR write policy, misalignment policy and M-extension
   corner results are exactly the spec-silences a competition design exploits.
   Traps are *counted and logged to data memory* by the emitted handler, so a
   DUT-vs-ISS comparison sees them as ordinary memory writes and a runaway
   trap loop terminates after `max_traps`.

Compare DUT vs `rv32_iss.RV32ISS` either per-commit (retire stream) or on final
architectural state. With `trap_mode=True` the ISS `RV32Config` must be set to
whatever the released spec says (misalignment, mtval, read-only CSR writes).

Usage
-----
    from randprog import GenConfig, trap_config, generate
    src = generate(GenConfig(seed=1, n_instr=200))            # trap-free
    src = generate(trap_config(seed=2, n_instr=150))          # corners
"""

from __future__ import annotations

import random
from dataclasses import dataclass, replace
from typing import List, Optional

from asm import assemble

# Register pools (ABI names). s0/s1 are reserved as data base pointers.
# sp/gp/tp are reserved for the trap handler (see `handler_text`) and are never
# used by the random body, so handler state survives arbitrary body clobbering.
VALUE_REGS = ["t0", "t1", "t2", "t3", "t4", "t5", "t6",
              "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
              "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11"]
BASE_REGS = ["s0", "s1"]
HANDLER_REGS = ("sp", "gp", "tp")

ALU_R = ["add", "sub", "xor", "or", "and", "slt", "sltu"]
SHIFT_R = ["sll", "srl", "sra"]
ALU_I = ["addi", "xori", "ori", "andi", "slti", "sltiu"]
SHIFT_I = ["slli", "srli", "srai"]
M_OPS = ["mul", "mulh", "mulhsu", "mulhu", "div", "divu", "rem", "remu"]
CR_OPS = ["sh1add", "sh2add", "sh3add", "min", "minu", "max", "maxu"]
BRANCH = ["beq", "bne", "blt", "bge", "bltu", "bgeu"]

SUB_WORD_LOADS = ["lb", "lh", "lbu", "lhu"]
SUB_WORD_STORES = ["sb", "sh"]
MISALIGN_DELTAS = [-3, -1, 1, 2, 3]

# Encodings that no RV32 implementation may accept (all-zero, all-ones, and
# custom/unused major opcodes). A core must raise illegal instruction on these.
ILLEGAL_WORDS = [0x0000_0000, 0xFFFF_FFFF, 0x0000_004B, 0x0000_005B, 0x0000_001B]

CSR_RW = ["csrrw", "csrrs", "csrrc", "csrrwi", "csrrsi", "csrrci"]
# mtvec is deliberately absent: the generator's own trap handler depends on it.
CSR_WRITABLE = ["mscratch", "mepc", "mie", "mstatus"]
CSR_READONLY = ["misa", "mhartid", "mcycle", "minstret", "mcause", "mtval"]


def _align_up(value: int, gran: int) -> int:
    return ((value + gran - 1) // gran) * gran


@dataclass
class GenConfig:
    seed: int = 0
    n_instr: int = 200
    data_base: int = 0x8000
    data_words: int = 64
    include_cr: bool = False
    hazard_rate: float = 0.5      # probability the next dst reuses the previous dst
    branch_rate: float = 0.06
    loadstore_rate: float = 0.20
    m_rate: float = 0.10
    shift_rate: float = 0.10
    init_values: bool = True      # emit stores that preload the scratch region

    # --- trap / CSR / policy corners (0 keeps the program trap-free) --------
    trap_mode: bool = False       # emit the handler + mtvec setup + corner classes
    csr_rate: float = 0.0
    misalign_rate: float = 0.0    # sub-word and misaligned accesses
    illegal_rate: float = 0.0     # guaranteed-illegal encodings
    ebreak_rate: float = 0.0
    div_corner_rate: float = 0.0  # divide-by-zero and INT_MIN/-1
    call_rate: float = 0.0        # jal -> subroutine -> jalr ra return
    max_traps: int = 24
    log_base: Optional[int] = None       # trap log window (default: past data)
    handler_base: Optional[int] = None   # handler address (default: past program)
    write_ro_csr: bool = False           # also write read-only CSRs (policy probe)


def trap_config(seed: int = 0, n_instr: int = 200, **over) -> GenConfig:
    """Preset that exercises the classes where seeded core deviations live."""
    cfg = GenConfig(seed=seed, n_instr=n_instr, include_cr=True, trap_mode=True,
                    csr_rate=0.08, misalign_rate=0.12, illegal_rate=0.03,
                    ebreak_rate=0.02, div_corner_rate=0.10, call_rate=0.05,
                    write_ro_csr=True)
    return replace(cfg, **over)


class _Gen:
    def __init__(self, cfg: GenConfig):
        self.cfg = cfg
        # Any corner class that can trap requires the handler + vector setup;
        # turn trap_mode on automatically rather than generating a program that
        # dies on the first trap.
        if (cfg.illegal_rate or cfg.ebreak_rate or cfg.csr_rate or cfg.misalign_rate) \
                and not cfg.trap_mode:
            self.cfg = replace(cfg, trap_mode=True)
        self.rng = random.Random(cfg.seed)
        self.lines: List[str] = []
        self.label_id = 0
        self.last_dst: Optional[str] = None
        self.data_offsets = list(range(0, cfg.data_words * 4, 4))
        self.emitted = 0          # rough instruction count, for reporting
        # Handler address is fixed before generation so the prologue can bake it
        # into mtvec: worst case 3 instructions per generated op (li / div corners)
        # plus the data-init prologue. The gap is nop padding, never executed.
        self._handler_base = self.cfg.handler_base if self.cfg.handler_base is not None \
            else _align_up(4 * (self.cfg.n_instr * 3 + 8 * self.cfg.data_words + 32), 0x100)

    # -- emit helpers ----------------------------------------------------
    def emit(self, text: str, cost: int = 1) -> None:
        self.lines.append("        " + text)
        self.emitted += cost

    def label(self, name: str) -> None:
        self.lines.append(f"{name}:")

    def new_label(self) -> str:
        self.label_id += 1
        return f"L{self.label_id}"

    def dst(self) -> str:
        if self.last_dst and self.rng.random() < self.cfg.hazard_rate:
            return self.last_dst
        return self.rng.choice(VALUE_REGS)

    def src(self) -> str:
        # Bias toward the previous destination to create RAW hazards.
        if self.last_dst and self.rng.random() < 0.45:
            return self.last_dst
        return self.rng.choice(VALUE_REGS)

    def base(self) -> str:
        return self.rng.choice(BASE_REGS)

    def off(self) -> int:
        return self.rng.choice(self.data_offsets)

    # -- layout ----------------------------------------------------------
    @property
    def log_base(self) -> int:
        """Trap log window: above the code, above the data scratch region."""
        if self.cfg.log_base is not None:
            return self.cfg.log_base
        top = max(self.cfg.data_base + self.cfg.data_words * 4, self._handler_base + 0x400)
        return _align_up(top + 0x100, 0x1000)

    @property
    def handler_base(self) -> int:
        return self._handler_base

    def check_layout(self) -> None:
        """Code, data scratch and trap log must not overlap (unified memory view)."""
        code_end = self._handler_base + 0x400
        if code_end > self.cfg.data_base:
            raise ValueError(
                f"handler at {self._handler_base:#x}+0x400 reaches data_base "
                f"{self.cfg.data_base:#x}: raise data_base or lower n_instr")
        if self.log_base < code_end:
            raise ValueError(f"log_base {self.log_base:#x} overlaps code")

    # -- prologue / epilogue ---------------------------------------------
    def prologue(self) -> None:
        self.emit(f"li s0, {self.cfg.data_base:#x}")
        self.emit(f"li s1, {self.cfg.data_base + self.cfg.data_words * 2:#x}", 2)
        if self.cfg.trap_mode:
            # Trap vector + zeroed log counter. sp/gp/tp belong to the handler.
            # The vector uses the numeric handler address (a forward label in `li`
            # would silently encode 0 - asm.py rejects that now).
            self.emit(f"li sp, {self.handler_base:#x}", 2)
            self.emit("csrw mtvec, sp")
            self.emit(f"li sp, {self.log_base:#x}", 2)
            self.emit("sw x0, 0(sp)")
        if self.cfg.init_values:
            for i, off in enumerate(self.data_offsets):
                val = self.rng.getrandbits(32)
                self.emit(f"li t0, {val:#x}", 2)
                self.emit(f"sw t0, {off}(s0)")

    # -- instruction classes ---------------------------------------------
    def gen_alu(self) -> None:
        r = self.rng.random()
        if r < 0.45:
            op = self.rng.choice(ALU_R)
            self.emit(f"{op} {self.dst()}, {self.src()}, {self.src()}")
        elif r < 0.65:
            op = self.rng.choice(SHIFT_R)
            self.emit(f"{op} {self.dst()}, {self.src()}, {self.src()}")
        elif r < 0.85:
            op = self.rng.choice(ALU_I)
            imm = self.rng.randint(-2048, 2047)
            self.emit(f"{op} {self.dst()}, {self.src()}, {imm}")
        else:
            op = self.rng.choice(SHIFT_I)
            imm = self.rng.randint(0, 31)
            self.emit(f"{op} {self.dst()}, {self.src()}, {imm}")

    def gen_m(self) -> None:
        op = self.rng.choice(M_OPS)
        self.emit(f"{op} {self.dst()}, {self.src()}, {self.src()}")

    def gen_cr(self) -> None:
        op = self.rng.choice(CR_OPS)
        self.emit(f"{op} {self.dst()}, {self.src()}, {self.src()}")

    def gen_loadstore(self) -> None:
        if self.rng.random() < 0.5:
            self.emit(f"lw {self.dst()}, {self.off()}({self.base()})")
        else:
            self.emit(f"sw {self.src()}, {self.off()}({self.base()})")

    def gen_branch(self) -> None:
        op = self.rng.choice(BRANCH)
        skip = self.new_label()
        self.emit(f"{op} {self.src()}, {self.src()}, {skip}")
        for _ in range(self.rng.randint(1, 3)):
            self.gen_alu()
        self.label(skip)

    def gen_misaligned(self) -> None:
        """Sub-word accesses, sometimes at an unaligned address.

        Misalignment policy (fault / ignore / support) is the single most common
        spec-silence in a core, and the only way to find which one the RTL does
        is to generate unaligned addresses on purpose.
        """
        base, off = self.base(), self.off()
        delta = self.rng.choice(MISALIGN_DELTAS)
        addr = off + delta
        r = self.rng.random()
        if r < 0.30:
            self.emit(f"{self.rng.choice(SUB_WORD_LOADS)} {self.dst()}, {addr}({base})")
        elif r < 0.55:
            self.emit(f"{self.rng.choice(SUB_WORD_STORES)} {self.src()}, {addr}({base})")
        elif r < 0.80:
            # misaligned *word* access (2 mod 4) - the classic fault case
            self.emit(f"lw {self.dst()}, {off + 2}({base})")
        else:
            self.emit(f"sw {self.src()}, {off + 2}({base})")

    def gen_csr(self) -> None:
        op = self.rng.choice(CSR_RW)
        imm_form = op.endswith("i")
        r = self.rng.random()
        if r < 0.55:
            name = self.rng.choice(CSR_WRITABLE)
        elif r < 0.85:
            name = self.rng.choice(CSR_READONLY)
        elif self.cfg.write_ro_csr and r < 0.95:
            name = self.rng.choice(["misa", "mcause", "mtval", "mhartid", "mcycle"])
        else:
            name = "0x7C0"              # non-existent CSR: must raise illegal
        rd = self.dst()
        if imm_form:
            val = self.rng.choice([0, 1, 2, 4, 8, 0x1F, 0x10, 3])
            self.emit(f"{op} {rd}, {name}, {val}")
        else:
            self.emit(f"{op} {rd}, {name}, {self.src()}")

    def gen_illegal(self) -> None:
        word = self.rng.choice(ILLEGAL_WORDS)
        self.emit(f".word 0x{word:08x}")

    def gen_ebreak(self) -> None:
        self.emit("ebreak")

    def gen_div_corner(self) -> None:
        """Divide corners: the M-extension's spec-mandated non-obvious results."""
        op = self.rng.choice(["div", "divu", "rem", "remu"])
        d, s = self.dst(), self.src()
        r = self.rng.random()
        if r < 0.45:
            self.emit(f"{op} {d}, {s}, x0")                 # zero divisor
        elif r < 0.80:
            tmp = self.rng.choice(["t0", "t1"])
            self.emit(f"li {tmp}, 0x80000000", 2)           # INT_MIN dividend
            self.emit("li t6, -1", 2)
            self.emit(f"{op} {d}, {tmp}, t6")               # INT_MIN / -1
        else:
            self.emit("li t5, 0x80000000", 2)
            self.emit(f"{op} {d}, t5, {s}")                 # INT_MIN / <random>

    def gen_call(self) -> None:
        """jal into a fixed subroutine and back through ra (link/return path)."""
        self.emit("jal ra, sub1")
        self.emit(f"addi {self.dst()}, ra, 0")               # observe the link value

    # -- trap handler + subroutine block ---------------------------------
    def handler_text(self) -> str:
        log = self.log_base
        maxt = self.cfg.max_traps
        return f"""
        .org {self.handler_base:#x}
handler:
        li   sp, {log:#x}
        lw   gp, 0(sp)
        addi gp, gp, 1
        sw   gp, 0(sp)
        li   tp, {maxt}
        bgeu gp, tp, done
        slli gp, gp, 2
        add  sp, sp, gp
        csrr tp, mcause
        sw   tp, 0x10(sp)
        csrr tp, mepc
        sw   tp, 0x210(sp)
        csrr tp, mtval
        sw   tp, 0x410(sp)
        csrr tp, mcause
        bge  tp, x0, adj_mepc
        j    do_mret
adj_mepc:
        csrr tp, mepc
        addi tp, tp, 4
        csrw mepc, tp
do_mret:
        mret
sub1:
        addi t1, t1, 3
        xor  t2, t2, t1
        ret
"""

    # -- dispatch --------------------------------------------------------
    def gen_one(self) -> None:
        cfg = self.cfg
        r = self.rng.random()
        acc = 0.0
        for rate, gen in ((cfg.branch_rate, self.gen_branch),
                          (cfg.loadstore_rate, self.gen_loadstore),
                          (cfg.m_rate, self.gen_m),
                          (0.08 if cfg.include_cr else 0.0, self.gen_cr),
                          (cfg.misalign_rate, self.gen_misaligned),
                          (cfg.csr_rate, self.gen_csr),
                          (cfg.div_corner_rate, self.gen_div_corner),
                          (cfg.call_rate, self.gen_call),
                          (cfg.illegal_rate, self.gen_illegal),
                          (cfg.ebreak_rate, self.gen_ebreak)):
            if rate <= 0:
                continue
            acc += rate
            if r < acc:
                gen()
                return
        self.gen_alu()

    def generate(self) -> str:
        self.check_layout()
        self.prologue()
        for _ in range(self.cfg.n_instr):
            self.gen_one()
        self.label("done")
        self.emit("j done", 2)
        if self.cfg.trap_mode or self.cfg.call_rate or self.cfg.illegal_rate:
            self.lines.append(self.handler_text().rstrip())
        return "\n".join(self.lines) + "\n"


def generate(cfg: Optional[GenConfig] = None) -> str:
    return _Gen(cfg or GenConfig()).generate()


def generate_program(cfg: Optional[GenConfig] = None):
    cfg = cfg or GenConfig()
    return assemble(generate(cfg), base=0)
