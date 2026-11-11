# challenge-1 submission catalog

Every submission archive produced during the challenge-1 campaign. Parent inferred by closest content among earlier-mtime variants. V/P/status matched by sha256 of tarball bytes against flow `report.json` `submission_sha256` field. 137/138 tarballs matched to a flow report (sub-0's report was the bare `runs/out/`, no sha256 stored).

Per-submission rebuild diffs: `challenge-1-sub-diffs.md`.

## Status legend

- `scored` — passed all flow stages, V and P reported
- `rejected@build` — Verilator lint/elab failed
- `rejected@public` — cocotb tests failed with assertions bound
- `rejected@mutation` or `@ppa` — failed at later stage
- `no-report` — tarball exists but no flow run output captured

## Catalog (chronological by tarball mtime)

| # | sub | parent | status | V | P | tar md5 |
|---|-----|--------|--------|---|---|----------|
| 1 | `sub-0` | `BASELINE` | no-report | — | — | `3e321c92356a` |
| 2 | `sub-1` | `sub-0` | rejected@public | — | — | `ce474574573b` |
| 3 | `sub-1-ppa` | `sub-0` | scored | 0.900 | 0.4712 | `a06b0e437a2a` |
| 4 | `sub-2` | `sub-0` | scored | 0.000 | 0.5053 | `bbfd2bc9dc1b` |
| 5 | `sub-3` | `sub-2` | scored | 0.000 | 0.5620 | `9f10b5292266` |
| 6 | `sub-5` | `sub-3` | scored | 0.000 | 0.5674 | `39d7e5b588a7` |
| 7 | `sub-4` | `sub-5` | scored | 0.925 | 0.0000 | `a7eecfe8e151` |
| 8 | `sub-6` | `sub-4` | scored | 0.900 | 0.0000 | `0fde29c00869` |
| 9 | `sub-7` | `sub-4` | scored | 0.950 | 0.0000 | `b9b9a0fc0cde` |
| 10 | `sub-8` | `sub-4` | scored | 0.975 | 0.0000 | `fb1e8510678e` |
| 11 | `sub-9` | `sub-8` | scored | 0.975 | 0.0000 | `e30208605949` |
| 12 | `sub-10` | `sub-8` | scored | 0.975 | 0.0000 | `6650000677f1` |
| 13 | `sub-11` | `sub-10` | scored | 1.000 | 0.0000 | `9c945633b47b` |
| 14 | `sub-fifo-rm` | `sub-10` | scored | 0.000 | 0.5612 | `92f28cf4ec6a` |
| 15 | `sub-fuse-lim` | `sub-11` | scored | 0.000 | 0.5424 | `1de12b5e7cdf` |
| 16 | `sub-acc-narrow` | `sub-11` | scored | 0.000 | 0.5518 | `5800d8bce749` |
| 17 | `sub-12` | `sub-10` | scored | 0.975 | 0.0000 | `71a3035ac0a1` |
| 18 | `sub-fir-parallel` | `sub-12` | scored | 0.000 | 0.0000 | `6bf88f525ec3` |
| 19 | `sub-gain3` | `sub-12` | scored | 1.000 | 0.0000 | `8cb73491e827` |
| 20 | `sub-13` | `sub-gain3` | scored | 1.000 | 0.0000 | `8cb73491e827` |
| 21 | `sub-fir-pipe` | `sub-13` | scored | 0.975 | 0.5773 | `a6e1b114c3ca` |
| 22 | `sub-parallel-a` | `sub-13` | scored | 1.000 | 0.0000 | `8cb73491e827` |
| 23 | `sub-parallel-b` | `sub-13` | scored | 1.000 | 0.0000 | `8cb73491e827` |
| 24 | `sub-w1` | `sub-13` | scored | 0.975 | 0.6039 | `6b22d092a27c` |
| 25 | `sub-w2` | `sub-13` | rejected@build | — | — | `fc8d9edb0e7c` |
| 26 | `sub-w3` | `sub-13` | scored | 1.000 | 0.5849 | `6ec3e997515a` |
| 27 | `sub-w4` | `sub-13` | scored | 1.000 | 0.5663 | `2c16f0c08318` |
| 28 | `sub-w5` | `sub-13` | scored | 0.925 | 0.5163 | `6dc15b247c59` |
| 29 | `sub-14` | `sub-w1` | scored | 0.975 | 0.6039 | `6b22d092a27c` |
| 30 | `sub-w2b` | `sub-w2` | scored | 1.000 | 0.5645 | `3023c4e35e69` |
| 31 | `sub-w6` | `sub-13` | scored | 1.000 | 0.5887 | `9d280f5001fd` |
| 32 | `sub-w7` | `sub-13` | scored | 0.975 | 0.6063 | `76a399fda186` |
| 33 | `sub-15` | `sub-w7` | scored | 0.975 | 0.6063 | `76a399fda186` |
| 34 | `sub-w8` | `sub-15` | rejected@public | — | — | `475294315557` |
| 35 | `sub-w9` | `sub-15` | scored | 0.975 | 0.6033 | `975cec815a4b` |
| 36 | `sub-e1` | `sub-15` | scored | 0.975 | 0.0000 | `d09ce041ed3d` |
| 37 | `sub-e2` | `sub-15` | scored | 0.975 | 0.0000 | `479fbe4d1275` |
| 38 | `sub-e3` | `sub-15` | scored | 1.000 | 0.0000 | `83db68566fb0` |
| 39 | `sub-e4` | `sub-15` | scored | 0.975 | 0.0000 | `eb475937f334` |
| 40 | `sub-e5` | `sub-15` | scored | 0.975 | 0.0000 | `79b0ab38a129` |
| 41 | `sub-e6` | `sub-15` | scored | 1.000 | 0.0000 | `d169eede8359` |
| 42 | `sub-e7` | `sub-15` | scored | 1.000 | 0.0000 | `b7a50a24dd1c` |
| 43 | `sub-e8` | `sub-15` | scored | 0.975 | 0.0000 | `c56e80d3df7e` |
| 44 | `sub-g1` | `sub-15` | scored | 1.000 | 0.5980 | `b88dc61c1db9` |
| 45 | `sub-g2` | `sub-15` | scored | 0.975 | 0.6076 | `c8ff330ccf47` |
| 46 | `sub-g3` | `sub-15` | scored | 0.950 | 0.5799 | `b88f8a21a35e` |
| 47 | `sub-g4` | `sub-15` | scored | 1.000 | 0.6063 | `8a4e7738b043` |
| 48 | `sub-16` | `sub-g2` | scored | 0.975 | 0.6076 | `c8ff330ccf47` |
| 49 | `sub-g5` | `sub-15` | scored | 1.000 | 0.5915 | `107e155e4017` |
| 50 | `sub-g6` | `sub-16` | scored | 1.000 | 0.5846 | `5c67240f98b5` |
| 51 | `sub-g7` | `sub-16` | scored | 1.000 | 0.5940 | `5a82a1156d3b` |
| 52 | `sub-g8` | `sub-16` | scored | 0.975 | 0.6029 | `625c579965c7` |
| 53 | `sub-e9` | `sub-16` | scored | 0.975 | 0.0000 | `773db3302755` |
| 54 | `sub-e10` | `sub-16` | scored | 0.950 | 0.0000 | `efb558305e1a` |
| 55 | `sub-e11` | `sub-16` | scored | 1.000 | 0.0000 | `c8f528de6706` |
| 56 | `sub-e12` | `sub-16` | scored | 1.000 | 0.0000 | `87e08ed806ef` |
| 57 | `sub-e14` | `sub-15` | scored | 0.950 | 0.0000 | `8a72146cd1a9` |
| 58 | `sub-e15` | `sub-15` | scored | 1.000 | 0.0000 | `e65da40a422d` |
| 59 | `sub-e16` | `sub-15` | scored | 1.000 | 0.0000 | `02ee507286c4` |
| 60 | `sub-h1` | `sub-16` | scored | 1.000 | 0.5928 | `8bf52edd92eb` |
| 61 | `sub-h3` | `sub-15` | scored | 1.000 | 0.5962 | `06e4deabe77b` |
| 62 | `sub-h4` | `sub-16` | scored | 0.975 | 0.6076 | `15be3efb64c0` |
| 63 | `sub-v17` | `sub-15` | scored | 0.975 | 0.6076 | `b6aff2c162ba` |
| 64 | `sub-v18` | `sub-15` | scored | 1.000 | 0.6076 | `d216adf0840e` |
| 65 | `sub-v19` | `sub-16` | scored | 0.950 | 0.6076 | `4695d5111384` |
| 66 | `sub-v20` | `sub-16` | scored | 1.000 | 0.6118 | `9fcec2a6cda1` |
| 67 | `sub-v21` | `sub-16` | scored | 0.950 | 0.5976 | `be209deff8be` |
| 68 | `sub-v22` | `sub-16` | scored | 1.000 | 0.6033 | `2c83c464d2fb` |
| 69 | `sub-v23` | `sub-16` | scored | 0.950 | 0.5988 | `da2f479a4382` |
| 70 | `sub-v24` | `sub-16` | scored | 0.975 | 0.6037 | `255eb6275fda` |
| 71 | `sub-v25` | `sub-v20` | scored | 0.975 | 0.6128 | `91d23c61ec82` |
| 72 | `sub-v26` | `sub-v20` | scored | 0.975 | 0.6018 | `db4894271e55` |
| 73 | `sub-v27` | `sub-v20` | scored | 0.950 | 0.6118 | `eb83fa8b0134` |
| 74 | `sub-v28` | `sub-v20` | scored | 0.975 | 0.6143 | `ca2cb9fea26b` |
| 75 | `sub-v29` | `sub-v25` | scored | 0.975 | 0.6097 | `eb72deb23d27` |
| 76 | `sub-v30` | `sub-v28` | scored | 0.950 | 0.6143 | `944710fecd63` |
| 77 | `sub-v31` | `sub-v28` | scored | 0.975 | 0.6143 | `d40e6dc49fc2` |
| 78 | `sub-v32` | `sub-v20` | scored | 0.975 | 0.6143 | `4d7a60e3813c` |
| 79 | `sub-v33` | `sub-v28` | scored | 0.975 | 0.6033 | `b3af43750bff` |
| 80 | `sub-v34` | `sub-v28` | scored | 0.950 | 0.6033 | `d70a73e5fc4d` |
| 81 | `sub-v35` | `sub-v28` | scored | 0.950 | 0.5907 | `1bb7c4836ee9` |
| 82 | `sub-v36` | `sub-v28` | scored | 0.950 | 0.5948 | `2cff4da0e1bd` |
| 83 | `sub-e21` | `sub-15` | scored | 0.975 | 0.6143 | `45f766e3fbdc` |
| 84 | `sub-e22` | `sub-15` | scored | 1.000 | 0.6143 | `8afb1c1c4425` |
| 85 | `sub-e23` | `sub-v19` | scored | 0.975 | 0.6143 | `c13d851766c6` |
| 86 | `sub-e24` | `sub-v28` | scored | 0.975 | 0.6143 | `aaba368cd3cd` |
| 87 | `sub-e25` | `sub-e23` | scored | 0.975 | 0.6143 | `5a9c9f1aef2f` |
| 88 | `sub-e26` | `sub-15` | scored | 1.000 | 0.6118 | `221779f5ef04` |
| 89 | `sub-e27` | `sub-e23` | scored | 0.975 | 0.6118 | `329a47896747` |
| 90 | `sub-e28` | `sub-v20` | scored | 1.000 | 0.6118 | `e6f737113d37` |
| 91 | `sub-e29` | `sub-e25` | scored | 1.000 | 0.6118 | `71bde62fc842` |
| 92 | `sub-e30` | `sub-e28` | scored | 0.975 | 0.6118 | `4552c0304f9c` |
| 93 | `sub-e31` | `sub-15` | scored | 1.000 | 0.6143 | `ff03b2bcd6d8` |
| 94 | `sub-e32` | `sub-e22` | scored | 0.950 | 0.6143 | `a037317a80c5` |
| 95 | `sub-e33` | `sub-e22` | scored | 1.000 | 0.6143 | `b78d2078876c` |
| 96 | `sub-e34` | `sub-e22` | scored | 0.925 | 0.6143 | `2d9b051c4268` |
| 97 | `sub-v39` | `sub-e22` | scored | 1.000 | 0.6192 | `38ed9ccf2f16` |
| 98 | `sub-v40` | `sub-e22` | scored | 1.000 | 0.6044 | `d55cb8acc5da` |
| 99 | `sub-v41` | `sub-e22` | scored | 0.950 | 0.6143 | `6dd43beaf781` |
| 100 | `sub-v42` | `sub-e22` | scored | 0.975 | 0.6143 | `5d2263dfcd6a` |
| 101 | `sub-v43` | `sub-e22` | scored | 1.000 | 0.6042 | `fa83404552e9` |
| 102 | `sub-v44` | `sub-e22` | scored | 0.975 | 0.6045 | `ef317c3a9c03` |
| 103 | `sub-v45` | `sub-e22` | scored | 0.950 | 0.6009 | `a5013def99b6` |
| 104 | `sub-v46` | `sub-e22` | scored | 0.975 | 0.6143 | `ab64fae62b3b` |
| 105 | `sub-v47` | `sub-v39` | scored | 0.975 | 0.6192 | `7284ecf99f80` |
| 106 | `sub-v48` | `sub-e22` | scored | 1.000 | 0.6192 | `bbb6feadc623` |
| 107 | `sub-v49` | `sub-15` | scored | 1.000 | 0.6192 | `aba37e3c15da` |
| 108 | `sub-v50` | `sub-e22` | rejected@build | — | — | `35cb3e71eb06` |
| 109 | `sub-v51` | `sub-15` | scored | 0.975 | 0.6192 | `9b5059533f00` |
| 110 | `sub-v52` | `sub-v39` | rejected@public | — | — | `a97dc7eb6c0f` |
| 111 | `sub-v53` | `sub-v39` | scored | 0.950 | 0.6192 | `1b0c4ae763dd` |
| 112 | `sub-v54` | `sub-e33` | scored | 0.950 | 0.6192 | `168e366485a8` |
| 113 | `sub-v55` | `sub-e22` | scored | 0.975 | 0.6101 | `b0bf46d48672` |
| 114 | `sub-v56` | `sub-e22` | scored | 1.000 | 0.6192 | `7bb72d65a480` |
| 115 | `sub-v57` | `sub-v39` | scored | 0.950 | 0.6192 | `3666a72c442a` |
| 116 | `sub-v58` | `sub-v39` | scored | 0.975 | 0.6183 | `08ebe4eec41f` |
| 117 | `sub-v59` | `sub-e22` | scored | 1.000 | 0.6101 | `2611534b0afd` |
| 118 | `sub-v60` | `sub-e22` | scored | 0.975 | 0.6101 | `0fe7fed8c85a` |
| 119 | `sub-v61` | `sub-v55` | scored | 0.975 | 0.6087 | `e67b92e7e94e` |
| 120 | `sub-v62` | `sub-v55` | scored | 0.975 | 0.6146 | `d88674a18360` |
| 121 | `sub-v63` | `sub-v39` | scored | 0.975 | 0.6154 | `7b44d096e3f7` |
| 122 | `sub-v64` | `sub-e22` | scored | 0.975 | 0.6192 | `f903fffc3665` |
| 123 | `sub-v65` | `sub-v39` | scored | 1.000 | 0.6179 | `453bd28eb972` |
| 124 | `sub-v66` | `sub-v39` | scored | 0.975 | 0.6139 | `3098cacc5597` |
| 125 | `sub-e35` | `sub-15` | scored | 0.975 | 0.6146 | `d281c4ba34ec` |
| 126 | `sub-e36` | `sub-e33` | scored | 0.950 | 0.6146 | `75c8a43c60b0` |
| 127 | `sub-e37` | `sub-v62` | scored | 0.975 | 0.6146 | `0ef1537384de` |
| 128 | `sub-e38` | `sub-15` | scored | 0.975 | 0.6146 | `cd1b6eb35ff2` |
| 129 | `sub-v67` | `sub-e37` | scored | 1.000 | 0.6146 | `2a3792ea1246` |
| 130 | `sub-v68` | `sub-e37` | scored | 1.000 | 0.6146 | `75b10d1bdb64` |
| 131 | `sub-v69` | `sub-e37` | scored | 0.950 | 0.6168 | `a8d225bd7696` |
| 132 | `sub-v70` | `sub-e37` | scored | 1.000 | 0.6146 | `f86952edf426` |
| 133 | `sub-v71` | `sub-v67` | scored | 0.975 | 0.6146 | `fc96c0ca2553` |
| 134 | `sub-v72` | `sub-v71` | scored | 0.950 | 0.6146 | `5cbe739a15ef` |
| 135 | `sub-v73` | `sub-v71` | scored | 1.000 | 0.6146 | `a91ca5cfd2c9` |
| 136 | `sub-v74` | `sub-v71` | scored | 0.975 | 0.6146 | `9c3c6ebcf10f` |
| 137 | `sub-v75` | `sub-e36` | scored | 0.975 | 0.6146 | `2d323b627bb0` |
| 138 | `sub-best` | `sub-v73` | scored | 1.000 | 0.6146 | `a91ca5cfd2c9` |

## Rebuild notes

- Tarballs `sub-0`..`sub-16` form the **core promoted chain** (baseline + seeded fixes + CR + Tier A/B PPA wins).
- `sub-w*` `sub-g*` `sub-h*` `sub-e*` `sub-v*` are P-perturbation / Explorer variants layered on sub-16.
- `sub-acc-narrow`, `sub-fifo-rm`, `sub-fir-parallel`, `sub-fuse-lim`, `sub-gain3` are pre-16 PPA experiments in `runs/ppa-candidates/`.
- For exact per-variant diff, see matching section in `challenge-1-sub-diffs.md`.
