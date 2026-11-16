"""
Program shrinker: turn a large failing random program into a minimal repro.

Why this matters on release day
-------------------------------
Differential testing against `rv32_iss.py` finds a divergence in a 500-instruction
random program. That is a *symptom*, not a repro: the seeded RTL deviation you are
hunting is buried under 499 instructions of noise, and you cannot reason about it,
cannot re-check it quickly, and cannot turn it into a directed test. A delta-debug
shrinker gives you a 3-line program that still shows the same divergence, which is
usually enough to name the bug and write the assertion that kills it.

Design
------
`keep(candidate_source) -> bool` is the only required input: True means "this
candidate still reproduces the failure". The shrinker never assumes what the
failure is, so it works for ISS-vs-ISS, ISS-vs-RTL (driven from a saved trace),
and any other pair.

Everything else is about keeping the repro *about the same bug* while shrinking:

- **stability**: shrinking moves PCs (deleted instructions) and changes values
  (minimized immediates). So the pass criterion should be the *structural*
  divergence signature from `diff.py`, not "the same register value at the same PC".
  `make_keep()` below builds that predicate for you.
- **validity**: candidates must assemble; assembly errors are filtered before the
  (expensive) oracle runs.
- **passes**: n-wise delta debugging first (removes whole instructions), then
  literal minimization (off-by-one / boundary values fall out), then register
  canonicalization (few distinct registers => obvious dataflow). Repeated until
  fixed point.

Passes preserve the instruction *set*: this shrinker never changes opcodes, so the
shrunk program exercises the same ISA features as the original.
"""

from __future__ import annotations

import re
import time
from dataclasses import dataclass, field
from typing import Callable, Dict, Iterable, List, Optional, Sequence, Set, Tuple

from asm import REGS, Program, assemble
from diff import Divergence  # noqa: F401  (re-exported for typing convenience)

Keep = Callable[[str], bool]

# Register pool used when canonicalizing (never x0; ordinary temporaries).
POOL = ["t0", "t1", "t2", "t3", "t4", "t5", "t6", "a0", "a1", "a2", "a3"]

# Interesting constants, in the order we try to *replace* a literal with them:
# smallest first, then boundary values, then powers of two. "orig +/- 1" is added
# per-literal because off_by_one is one of the six mutation classes.
BASE_CONSTANTS: Tuple[int, ...] = (
    0, 1, 2, 3, 4, 5, 7, 8, 15, 16, 17, 31, 32, 33, 63, 64, 65,
    127, 128, 255, 256, 1023, 2047, 2048,
    0x7FFF, 0x8000, 0x8001, 0xFFFF, 0x1_0000,
    0x3FFF_FFFF, 0x4000_0000, 0x7FFF_FFFF, 0x8000_0000, 0xFFFF_FFFF,
)

# Not a digit inside a register/label name: t0, a2, s1, sh2add must not be
# touched, so require non-word characters (or string edges) around a literal.
# Values tried when re-spelling a literal without changing its encoding, in
# ascending |value|: small positives, small negatives, and the field-edge values
# (12-bit immediates, 5-bit shifts, byte/halfword masks).
SPELL_VALUES: Tuple[int, ...] = tuple(sorted(
    (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15, 16, 31, 32, 63, 64, 127, 128,
     255, 256, 511, 1023, 2047, 2048, 0x800, 0x7FF, 0xFFF,
     -1, -2, -3, -4, -7, -8, -15, -16, -31, -32, -64, -128, -255, -256,
     -512, -1024, -2048, -0x800, -0x8000, -0x8000_0000, -1 << 31),
    key=lambda v: (abs(v), v)))

_NUM_RE = re.compile(r"(?<![A-Za-z_0-9'])-?(?:0[xX][0-9a-fA-F]+|\d+)(?![A-Za-z_0-9'])")
_TOKEN_RE = re.compile(r"[A-Za-z_][A-Za-z_0-9']*")


@dataclass
class ShrinkConfig:
    max_passes: int = 4
    max_calls: int = 4000
    max_lines: Optional[int] = None      # starting cap (0-size programs from big traces)
    reg_pools: Tuple[int, ...] = (1, 2, 3, 4)
    protect_regs: Tuple[str, ...] = ()   # never renamed (e.g. a handler's base regs)
    constants: Tuple[int, ...] = BASE_CONSTANTS
    verbose: bool = False


@dataclass
class ShrinkResult:
    source: str
    lines: List[str]
    original_lines: int
    calls: int
    cache_hits: int
    seconds: float
    passes: List[Tuple[str, int, int]] = field(default_factory=list)
    stopped_on_budget: bool = False

    @property
    def reduction(self) -> str:
        return f"{self.original_lines} -> {len(self.lines)} lines"

    def report(self) -> str:
        rows = "\n".join(f"  {name:<18} {a:>4} -> {b:>4}" for name, a, b in self.passes)
        return (f"shrunk {self.reduction} in {self.seconds:.1f}s "
                f"({self.calls} oracle calls, {self.cache_hits} cached)\n{rows}")


def split_source(src: str) -> List[str]:
    """One chunk per meaningful source line (labels kept with their line)."""
    out: List[str] = []
    for raw in src.splitlines():
        line = re.split(r"(#|//|;)", raw, maxsplit=1)[0].strip()
        if line:
            out.append(line)
    return out


def join_source(lines: Sequence[str]) -> str:
    return "\n".join("        " + ln for ln in lines) + "\n"


class Shrinker:
    def __init__(self, keep: Keep, cfg: Optional[ShrinkConfig] = None, base: int = 0):
        self.keep = keep
        self.cfg = cfg or ShrinkConfig()
        self.base = base
        self.memo: Dict[str, bool] = {}
        self.calls = 0
        self.cache_hits = 0
        self.budget_hit = False

    # -- oracle ----------------------------------------------------------
    def reproduces(self, lines: Sequence[str]) -> bool:
        """True if these lines still fail. Assembly errors reject the candidate."""
        text = "\n".join(lines)
        hit = self.memo.get(text)
        if hit is not None:
            self.cache_hits += 1
            return hit
        if self.calls >= self.cfg.max_calls:
            self.budget_hit = True
            return False
        try:
            assemble(text, base=self.base)
        except Exception:
            self.memo[text] = False
            return False
        self.calls += 1
        try:
            ok = bool(self.keep(text))
        except Exception:
            ok = False
        self.memo[text] = ok
        return ok

    # -- pass 1: n-wise delta debugging ----------------------------------
    def ddmin(self, lines: List[str]) -> List[str]:
        """Textbook delta debugging: drop one chunk at a time, doubling the
        number of chunks (so chunks get *smaller*) until only single lines
        remain and none of them can be dropped."""
        cur = list(lines)
        n = 2
        while not self.budget_hit:
            chunk = max(1, len(cur) // n)
            chunks = [cur[i:i + chunk] for i in range(0, len(cur), chunk)]
            progress = False
            for j in range(len(chunks)):
                cand = [x for k, ch in enumerate(chunks) if k != j for x in ch]
                if cand and self.reproduces(cand):
                    cur = cand
                    progress = True
                    break           # re-sweep at the same granularity
            if not progress:
                if chunk == 1:
                    break           # 1-minimal
                n = min(2 * n, max(2, len(cur)))
        return cur

    def _delete_each(self, lines: List[str]) -> List[str]:
        cur = list(lines)
        i = 0
        while i < len(cur) and not self.budget_hit:
            cand = cur[:i] + cur[i + 1:]
            if cand and self.reproduces(cand):
                cur = cand
            else:
                i += 1
        return cur

    def _delete_pairs(self, lines: List[str]) -> List[str]:
        """Two-line removals: catches producer/consumer pairs (li + sw) that
        ddmin cannot remove one line at a time."""
        cur = list(lines)
        i = 0
        while i + 1 < len(cur) and not self.budget_hit:
            cand = cur[:i] + cur[i + 2:]
            if cand and self.reproduces(cand):
                cur = cand
            else:
                i += 1
        return cur

    # -- pass 2: literal minimization ------------------------------------
    def minimize_literals(self, lines: List[str]) -> Tuple[List[str], int]:
        out = list(lines)
        changed = 0
        for idx, line in enumerate(out):
            if line.lower().startswith(".word"):
                continue          # data word: value is the payload, keep structure
            parent_words = self._words(out)
            for m in reversed(list(_NUM_RE.finditer(line))):
                start, end = m.span()
                text = line[start:end]
                value = _parse_int(text)
                if value is None:
                    continue
                for repl in self._candidates(value):
                    cand_line = line[:start] + repl + line[end:]
                    if cand_line == line:
                        continue
                    cand = out[:idx] + [cand_line] + out[idx + 1:]
                    words = self._words(cand)
                    if words is None:
                        continue
                    if words == parent_words and abs(_parse_int(repl)) >= abs(value):
                        # Encoding unchanged: immediate fields are narrower than the
                        # literal, so a bigger number can assemble to the same
                        # instruction. Rejected unless it also shortens the text,
                        # otherwise the shrinker wanders to huge equivalent values.
                        continue
                    if self.reproduces(cand):
                        out, parent_words = cand, words
                        line = cand_line
                        changed += 1
        return out, changed

    def _words(self, lines: Sequence[str]):
        try:
            return assemble("\n".join(lines), base=self.base).words
        except Exception:
            return None

    def _candidates(self, value: int) -> Iterable[str]:
        seen: Set[str] = set()
        pool = list(self.cfg.constants)
        pool += [value - 1, value + 1, value // 2, value & (value - 1)]
        if value:
            pool.append(1 << (abs(value).bit_length() - 1))
        ordered = sorted({v for v in pool if abs(v) <= 0xFFFF_FFFF},
                         key=lambda v: (abs(v), v))
        for v in ordered:
            s = _format_int(v, value)
            if s not in seen:
                seen.add(s)
                yield s

    # -- pass 3: register canonicalization -------------------------------
    def canonicalize_regs(self, lines: List[str]) -> Tuple[List[str], Optional[int]]:
        for pool_size in self.cfg.reg_pools:
            mapping = self._merge_map(lines, pool_size)
            if not mapping:
                continue
            cand = [self._rename(line, mapping) for line in lines]
            if self.reproduces(cand):
                return cand, pool_size
        return lines, None

    def _merge_map(self, lines: Sequence[str], pool_size: int) -> Dict[str, str]:
        """Map every used register onto a small pool, in first-appearance order.

        Deliberately *aliases* registers when the pool is smaller than the number
        of used registers: that is the point (a 25-register repro becomes 3 and
        the dataflow becomes visible). Aliasing changes the program, so the
        candidate only survives if the same divergence is still reproduced.
        """
        protect = set(self.cfg.protect_regs) | {"x0", "zero"}
        mapping: Dict[str, str] = {}
        pool = [r for r in POOL if r not in protect][:max(1, pool_size)]
        for line in lines:
            for tok in _TOKEN_RE.findall(line):
                if tok in REGS and tok not in protect and tok not in mapping:
                    mapping[tok] = pool[len(mapping) % len(pool)]
        if len(mapping) <= 1:
            return {}
        return mapping

    @staticmethod
    def _rename(line: str, mapping: Dict[str, str]) -> str:
        def sub(m: "re.Match[str]") -> str:
            tok = m.group(0)
            return mapping.get(tok, tok) if tok in REGS else tok
        return _TOKEN_RE.sub(sub, line)

    # -- pass 4: spelling cleanup ---------------------------------------
    def _recanonicalize(self, lines: List[str]) -> List[str]:
        """Rewrite each literal with the smallest value that encodes identically.

        e.g. `sw t0, 4097(t1)` -> `sw t0, 1(t1)` (the offset field is 12 bits).
        Pure spelling - the instruction words are unchanged, so no oracle call -
        and it is what makes the output readable instead of merely minimal.
        """
        out = list(lines)
        base_words = self._words(out)
        if base_words is None:
            return out
        for idx, line in enumerate(out):
            for m in reversed(list(_NUM_RE.finditer(line))):
                start, end = m.span()
                value = _parse_int(line[start:end])
                if value is None:
                    continue
                for cand_val in SPELL_VALUES:
                    text = _format_int(cand_val, value)
                    cand_line = line[:start] + text + line[end:]
                    if cand_line == line:
                        break
                    cand = out[:idx] + [cand_line] + out[idx + 1:]
                    if self._words(cand) == base_words:
                        out, line = cand, cand_line
                        break
        return out

    # -- driver ----------------------------------------------------------
    def shrink(self, src: str) -> ShrinkResult:
        t0 = time.time()
        lines = split_source(src)
        if self.cfg.max_lines:
            lines = lines[: self.cfg.max_lines]
        start = len(lines)
        result = ShrinkResult(source="", lines=lines, original_lines=start,
                              calls=0, cache_hits=0, seconds=0.0)
        cur = lines
        # Probe the original first: this is what pins the reference signature, so
        # the repro is about the bug we started with, not about some other latent
        # difference that only appeared after the noise was removed.
        self.reproduces(cur)
        for p in range(self.cfg.max_passes):
            before_lines = len(cur)
            before_state = tuple(cur)
            cur = self.ddmin(cur)
            cur = self._delete_pairs(cur)
            cur = self._delete_each(cur)
            cur = self._drop_redundant_nops(cur)
            lit_before = len(cur)
            cur, lit_changes = self.minimize_literals(cur)
            cur, pool = self.canonicalize_regs(cur)
            cur = self.ddmin(cur)          # re-sweep: new values may free lines
            note = f"round[{p}]"
            if pool:
                note += f" regs->{pool}"
            if lit_changes:
                note += f" lit:{lit_changes}"
            if lit_before != len(cur):
                note += f" lines:{lit_before}->{len(cur)}"
            result.passes.append((note, before_lines, len(cur)))
            if tuple(cur) == before_state:
                break                      # fixed point
            if self.budget_hit:
                result.stopped_on_budget = True
                break
        cur = self._recanonicalize(cur)
        if not self.reproduces(cur):
            # A pass corrupted the repro (should not happen: every accepted
            # candidate is checked). Fall back to the last known-good text.
            cur = lines
        result.lines = cur
        result.source = join_source(cur)
        result.calls = self.calls
        result.cache_hits = self.cache_hits
        result.seconds = time.time() - t0
        return result

    @staticmethod
    def _drop_redundant_nops(lines: List[str]) -> List[str]:
        out: List[str] = []
        n = 0
        for ln in lines:
            if ln.strip() in ("nop", "addi x0, x0, 0"):
                n += 1
                if n > 1:
                    continue
            else:
                n = 0
            out.append(ln)
        return out


def _parse_int(text: str) -> Optional[int]:
    try:
        return int(text, 0)
    except ValueError:
        return None


def _format_int(value: int, like: int) -> str:
    """Keep the candidate in the notation the original literal used."""
    if abs(like) >= 0x10 or abs(value) >= 0x100:
        return hex(value)
    return str(value)


# ---------------------------------------------------------------------------
# Oracle builders
# ---------------------------------------------------------------------------
RunPair = Callable[[str], Optional[Divergence]]


def make_keep(run_pair: RunPair, use_signature: bool = True) -> Keep:
    """Build a `keep` predicate from a function that returns the first divergence.

    run_pair(source) must return a `diff.Divergence` (or None when the candidate
    does *not* reproduce). Exceptions from the models count as "no repro".

    use_signature=True pins the *structural* signature of the original failure
    (divergence kind + instruction class + effect kind). This is what makes the
    result a repro of the same bug rather than of some other latent difference
    that happened to appear once the noise was removed. The reference signature is
    taken from the program passed to `shrink()` first (the shrinker probes the
    original before trying any candidate) and is exposed as `keep.signature`, so
    a caller can assert the repro stayed on-topic.
    """
    ref_sig: List[Optional[Tuple]] = [None]

    def keep(text: str) -> bool:
        try:
            div = run_pair(text)
        except Exception:
            return False
        if div is None:
            return False
        if not use_signature:
            return True
        if ref_sig[0] is None:
            ref_sig[0] = div.signature
            keep.signature = div.signature       # type: ignore[attr-defined]
            return True
        return div.signature == ref_sig[0]

    keep.signature = None                        # type: ignore[attr-defined]
    return keep


def shrink_program(src: str, keep: Keep, cfg: Optional[ShrinkConfig] = None,
                   base: int = 0) -> ShrinkResult:
    return Shrinker(keep, cfg, base).shrink(src)
