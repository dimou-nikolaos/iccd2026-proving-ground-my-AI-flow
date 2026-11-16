# checkpoint 2026-11-16 — ONE-SHOT SUBMISSION DECISION

Operator: only ONE submission allowed. Pick = **runs/sub.tar.gz (drop7v3, md5 092ca181e03abce1647667c1895e5d32) UNMODIFIED.**

## Why (measured, same-hash roll, out-c3-drop7v3-full: build/public/mutation/ppa all pass)
- V=1.0 AND P=0.484 on the SAME archive hash — the only archive in the whole campaign (24 netlists,
  31 scored runs) that has both. V/P deterministic per archive hash (s12b proof) → server reproduces.
- All other V=1.0 archives strictly lower P (drop5a 0.484 same-RTL but older suite; a5 0.4745;
  lot2 0.4704; drop3r4 0.4625; a1 0.4599). Re-rolls of the main netlist all roll V<1.0 (b4 0.95,
  drop7a 0.95, drop7v4 0.975) → NO re-roll is better. NO re-rolls performed (none needed).
- Requirements respected: byte-identical ports/filelist/constraint.sdc, synthesizable subset,
  95/95 own tests + 0 assertion fires + public TB, full flow status=scored.
- C unknown (server never ran it); all candidates share that risk; drop7v3 = most-audited +
  complete REQ matrix = safest. Projected score 100×C×(0.5+0.5×0.484) = 74.2×C.

## DO NOT: re-pack, touch, or "improve" runs/sub.tar.gz before upload (hash = scores).
Verify at upload time: md5sum runs/sub.tar.gz == 092ca181e03abce1647667c1895e5d32.
