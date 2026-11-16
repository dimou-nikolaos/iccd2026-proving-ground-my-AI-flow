#!/usr/bin/env python3
"""Produce reroll variants of the bank tar: bump the marker comment in rtl/dma_apb.sv
   so tar bytes (and thus the flow's mutation seed) change; RTL semantics unchanged."""
import tarfile, io, sys, time

def variant(src, out, marker):
    with tarfile.open(src,'r:gz') as s, tarfile.open(out,'w:gz') as d:
        for m in s.getmembers():
            if m.name=='rtl/dma_apb.sv':
                b=s.extractfile(m).read().decode()
                import re
                b=re.sub(r'// explorer-seed-reroll \S+', f'// explorer-seed-{marker}', b)
                data=b.encode(); ti=tarfile.TarInfo(m.name); ti.size=len(data); ti.mtime=m.mtime; ti.mode=0o644; ti.type=tarfile.REGTYPE
                d.addfile(ti, io.BytesIO(data))
            else:
                d.addfile(m, s.extractfile(m) if m.isfile() else None)

if __name__=='__main__':
    variant('runs/sub-best.tar.gz', sys.argv[1], sys.argv[2])
