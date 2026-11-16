"""
Commit-stream diff: the comparison primitive behind core differential testing.

Both sides of the comparison reduce to the same shape: an *ordered list of
architectural effects per retired instruction*. The ISS produces `Commit`
objects; a real core observed from a testbench produces the same information
from its writeback/store/CSR-visible ports. So divergence *classification*
lives here once, and is reused by:

  - the shrinker (`shrink.py`), which needs a stable signature to keep a
    minimal repro about the same bug, and
  - day-1 lockstep tests (`prep/rvcore/test_stage.py` style), where the first
    diverging commit is what you want printed.

Signature design matters: shrinking deletes instructions (so PCs move) and
minimizes immediates (so values change). A signature that contains a PC or a
value therefore never survives shrinking. So the signature is structural:
(kind of divergence, coarse class of the diverging instruction, kind of the
diverging effect). It is deliberately value-free and address-free.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import List, Optional, Sequence, Tuple

from rv32_iss import CAUSE_NAMES, Commit

# ---------------------------------------------------------------------------
# Coarse instruction classes (stable under shrinking)
# ---------------------------------------------------------------------------
_R_FUNCT3 = {0: "add", 1: "sll", 2: "slt", 3: "sltu", 4: "xor", 5: "srl", 6: "or", 7: "and"}
_M_FUNCT3 = {0: "mul", 1: "mulh", 2: "mulhsu", 3: "mulhu", 4: "div", 5: "divu", 6: "rem", 7: "remu"}
_Z_SHADD = {2: "sh1add", 4: "sh2add", 6: "sh3add"}       # funct7 = 0b0010000
_Z_MINMAX = {4: "min", 5: "minu", 6: "max", 7: "maxu"}   # funct7 = 0b0000101
_I_FUNCT3 = {0: "addi", 1: "slli", 2: "slti", 3: "sltiu", 4: "xori", 5: "srli", 6: "ori", 7: "andi"}
_LOAD_FUNCT3 = {0: "lb", 1: "lh", 2: "lw", 4: "lbu", 5: "lhu"}
_STORE_FUNCT3 = {0: "sb", 1: "sh", 2: "sw"}
_BRANCH_FUNCT3 = {0: "beq", 1: "bne", 4: "blt", 5: "bge", 6: "bltu", 7: "bgeu"}
_CSR_FUNCT3 = {1: "csrrw", 2: "csrrs", 3: "csrrc", 5: "csrrwi", 6: "csrrsi", 7: "csrrci"}


def instr_class(instr: Optional[int]) -> str:
    """Coarse decode class of an instruction: opcode-shaped, operand-free.

    Used as the "which instruction are we talking about" part of a divergence
    signature, so it must not depend on register numbers or immediates.
    """
    if instr is None:
        return "?"
    op = instr & 0x7F
    f3 = (instr >> 12) & 7
    f7 = (instr >> 25) & 0x7F
    if op == 0x33:
        if f7 == 0 or f7 == 0x20:
            return f"R:{_R_FUNCT3[f3]}"
        if f7 == 1:
            return f"M:{_M_FUNCT3[f3]}"
        if f7 == 0x10 and f3 in _Z_SHADD:
            return f"Z:{_Z_SHADD[f3]}"
        if f7 == 5 and f3 in _Z_MINMAX:
            return f"Z:{_Z_MINMAX[f3]}"
        return "R:unknown"
    if op == 0x13:
        if f3 in (1, 5):
            return f"I:{_I_FUNCT3[f3]}:{'srai' if f7 == 0x20 else 'sh'}"
        return f"I:{_I_FUNCT3[f3]}"
    if op == 0x03:
        return f"load:{_LOAD_FUNCT3.get(f3, f3)}"
    if op == 0x23:
        return f"store:{_STORE_FUNCT3.get(f3, f3)}"
    if op == 0x63:
        return f"branch:{_BRANCH_FUNCT3[f3]}"
    if op == 0x6F:
        return "jal"
    if op == 0x67:
        return "jalr"
    if op == 0x37:
        return "lui"
    if op == 0x17:
        return "auipc"
    if op == 0x73:
        if f3 == 0:
            if instr == 0x0000_0073:
                return "ecall"
            if instr == 0x0010_0073:
                return "ebreak"
            if instr == 0x3020_0073:
                return "mret"
            if instr == 0x1050_0073:
                return "wfi"
            return "system:bad"
        return f"csr:{_CSR_FUNCT3.get(f3, f3)}"
    if op == 0x0F:
        return "fence"
    return f"illegal:op{op:02x}"


# ---------------------------------------------------------------------------
# Events: the normalized per-instruction architectural effect
# ---------------------------------------------------------------------------
@dataclass(frozen=True)
class Ev:
    """One architectural effect of a retired instruction.

    kind: "rd"   -> a = register index, b = value
          "mem"  -> a = address,      b = size in bytes, c = value
          "csr"  -> a = csr address,  b = value
          "trap" -> a = cause,        b = mepc
    """

    kind: str
    a: int = 0
    b: int = 0
    c: int = 0

    def brief(self) -> str:
        if self.kind == "rd":
            return f"rd:x{self.a}={self.b:#010x}"
        if self.kind == "mem":
            return f"mem[{self.a:#x}:{self.b}B]={self.c:#010x}"
        if self.kind == "csr":
            return f"csr[{self.a:#03x}]={self.b:#010x}"
        if self.kind == "trap":
            name = CAUSE_NAMES.get(self.a, f"cause{self.a}")
            return f"trap({name},epc={self.b:#x})"
        return f"{self.kind}({self.a:#x},{self.b:#x},{self.c:#x})"


@dataclass
class Step:
    """Normalized view of one retired instruction."""

    pc: int
    instr: Optional[int]
    events: Tuple[Ev, ...] = ()

    def brief(self) -> str:
        words = " ".join(e.brief() for e in self.events)
        enc = f"{self.instr:08x}" if self.instr else "????????"
        return f"{self.pc:#06x} {enc} {instr_class(self.instr)}: {words}"


def commit_events(c: Commit, csr_trap_writes: bool = False) -> Tuple[Ev, ...]:
    """Turn an ISS `Commit` into normalized events.

    Trap-induced CSR writes (mepc/mcause/mtval/mstatus) are folded into the
    `trap` event by default, because on the RTL side they are usually not
    observable per-instruction, and the trap itself is the interesting effect.
    Pass csr_trap_writes=True to keep them (useful when checking a core's
    trap-entry CSR updates specifically).
    """
    ev: List[Ev] = []
    if c.rd:
        ev.append(Ev("rd", c.rd, c.wdata))
    for addr, size, value in c.mem_writes:
        ev.append(Ev("mem", addr, size, value))
    trap_csrs = {0x300, 0x341, 0x342, 0x343}
    for addr, value in c.csr_writes:
        if not csr_trap_writes and addr in trap_csrs:
            continue
        ev.append(Ev("csr", addr, value))
    if c.trap_cause is not None:
        ev.append(Ev("trap", c.trap_cause, c.trap_epc or 0))
    return tuple(ev)


def _instr_of(c: Commit) -> Optional[int]:
    """Instruction word of a commit, or None for an interrupt pseudo-commit.

    An interrupt is recorded by the ISS as instr=0 with a trap cause in the
    interrupt range; mark those None so they are not confused with a real fetch
    of the word 0x0000_0000 (which is an illegal instruction and must classify
    as such).
    """
    if c.instr == 0 and c.trap_cause is not None and c.trap_cause & 0x8000_0000:
        return None
    return c.instr


def steps_from_commits(commits: Sequence[Commit], csr_trap_writes: bool = False) -> List[Step]:
    return [Step(c.pc, _instr_of(c), commit_events(c, csr_trap_writes)) for c in commits]


# ---------------------------------------------------------------------------
# Divergence
# ---------------------------------------------------------------------------
@dataclass
class Divergence:
    """First difference between two commit streams.

    kind:
      "instr"   different instruction fetched at the same commit index
      "pc"      different address executed at the same commit index
      "event"   same instruction, different architectural effect - includes the
                trap-vs-no-trap case, which the signature's third element names
      "length"  one stream ended / has extra commits
    signature: stable across shrinking - (kind, instruction class, effect kind).
      The effect kind is "rd"/"mem"/"csr"/"trap" possibly prefixed with "+"
      (effect only in dut) or "-" (effect only in ref).
    """

    index: int
    kind: str
    want: Optional[Step]
    got: Optional[Step]
    detail: str
    signature: Tuple

    def brief(self) -> str:
        w = self.want.brief() if self.want else "<none>"
        g = self.got.brief() if self.got else "<none>"
        return f"commit #{self.index} {self.kind}: {self.detail}\n  ref : {w}\n  dut : {g}"

    @property
    def pc(self) -> Optional[int]:
        if self.want is not None:
            return self.want.pc
        return self.got.pc if self.got else None


def _diff_pair(a: Step, b: Step, i: int) -> Optional[Divergence]:
    """Compare one commit index of two streams."""
    if a.pc != b.pc:
        return Divergence(i, "pc", a, b,
                          f"pc {a.pc:#x} vs {b.pc:#x} "
                          f"({instr_class(a.instr)} vs {instr_class(b.instr)})",
                          ("pc", instr_class(a.instr), instr_class(b.instr)))
    if a.instr is not None and b.instr is not None and a.instr != b.instr:
        return Divergence(i, "instr", a, b,
                          f"fetched {a.instr:08x} vs {b.instr:08x}",
                          ("instr", instr_class(a.instr), instr_class(b.instr)))
    if a.events != b.events:
        detail, eff = _event_diff(a, b)
        return Divergence(i, "event", a, b, detail, ("event", instr_class(a.instr), eff))
    return None


def first_divergence(ref: Sequence[Step], dut: Sequence[Step],
                     max_steps: Optional[int] = None) -> Optional[Divergence]:
    """Compare two normalized commit streams, in order. None = identical."""
    n = min(len(ref), len(dut)) if max_steps is None else min(len(ref), len(dut), max_steps)
    for i in range(n):
        d = _diff_pair(ref[i], dut[i], i)
        if d is not None:
            return d
    if len(ref) != len(dut):
        a = ref[n] if n < len(ref) else None
        b = dut[n] if n < len(dut) else None
        return Divergence(n, "length", a, b,
                          f"stream lengths {len(ref)} vs {len(dut)}",
                          ("length", instr_class(a.instr) if a else "?",
                           instr_class(b.instr) if b else "?"))
    return None


def compare_run(ref, dut, max_steps: int = 100_000, csr_trap_writes: bool = False
                ) -> Optional[Divergence]:
    """Step two ISS-like models in lockstep and return the first divergence.

    This is the oracle the shrinker should use: it is much faster than running
    both models to completion and diffing the traces, because it stops at the
    divergence and because it stops as soon as *both* sides are spinning (every
    generated program ends in a self-loop). Same result as
    `first_divergence(steps_from_commits(a.trace), steps_from_commits(b.trace))`
    for the common case where the streams have equal length up to the spin.
    """
    prev = None
    for i in range(max_steps):
        ca, cb = ref.step(), dut.step()
        if ca is None or cb is None:
            return None
        d = _diff_pair(Step(ca.pc, _instr_of(ca), commit_events(ca, csr_trap_writes)),
                       Step(cb.pc, _instr_of(cb), commit_events(cb, csr_trap_writes)), i)
        if d is not None:
            return d
        if prev is not None and ref.pc == prev[0] and dut.pc == prev[1]:
            return None          # both sides spinning: nothing left to compare
        prev = (ref.pc, dut.pc)
    return None


def _event_diff(a: Step, b: Step) -> Tuple[str, str]:
    """Describe the first differing event; return (human detail, effect kind for signature)."""
    kinds = sorted({e.kind for e in a.events} | {e.kind for e in b.events})
    for kind in ("trap", "rd", "mem", "csr"):
        ea = [e for e in a.events if e.kind == kind]
        eb = [e for e in b.events if e.kind == kind]
        if ea != eb:
            if not ea:
                return (f"dut has extra {eb[0].brief()}", f"+{kind}")
            if not eb:
                return (f"dut missing {ea[0].brief()}", f"-{kind}")
            if len(ea) != len(eb):
                return (f"{kind} count {len(ea)} vs {len(eb)}", f"{kind}n")
            return (f"{kind}: ref {ea[0].brief()} vs dut {eb[0].brief()}", kind)
    return (f"events differ ({', '.join(kinds)})", "events")
