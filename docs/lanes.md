# Lane assignments (integrator-owned)

One owner per module at a time. Candidates live in `candidates/<lane>/`, reports in `candidates/<lane>/REPORT.md`.
Model: `luna` = gpt-6-luna xhigh, `sol` = gpt-6.1-sol high (Codex; all new lanes from 2026-11-09 09:10, user decision); `opus` = Claude Opus 5.5 subagent.

| Lane | Model | Modules (start VA: file) | State (2026-11-09 midday) |
|---|---|---|---|
| w1 | luna | assert.c, strpool.c, memory.c, io\file.c | done; tail handed to b1 |
| b1 | sol | w1 tail; 00425978 framework; gui\flic.c; gui\menu.c; 0042a5fc/0042a0e0; shell\shell.c | done (mplaunch 38; 8 DIFF + initializer open) |
| w2 | luna | io\resource.c | done (19) |
| w3 | luna | logic\lactor.c, miss.c, bestloc.c | done |
| w4 | luna | multi\linkapp.c, link.c, chat.c | done; 16 withdrawn (fake array, docs/blocked.md) |
| w5 | luna | terr\tgrid.c, tfog.c, tfast.c | done |
| w6 | luna | shell\missbrf.c, hercbay.c, pop\psystem.c | done |
| w7 | luna | sim\actorlst.c, actoradd.c, worldact.c | done |
| w8 | luna | sim\target.c, logic\lsys.c | done |
| w9 | luna | logic\lactors.c, logic\intel.c | done; continued by s18 |
| w10 | luna | sim\anim.c, sim\animbmp.c | done; continued by s17 |
| w11 | luna | shell\hotseat.c, pop\pherc.c | done |
| w12 | luna | sim\simhot.c, logic\logic.c + sim\hotbrief.c | done; continued by s17/s18 |
| w13 | luna | terr\tsurface.c, terr\tarea.c + tdisplay.c | done (14; 39 parked); continued by s14 |
| w14 | luna | shell\hbupgrad.c | done; continued by s6 |
| w15 | luna | shell\pilotgui.c | done (33: tu_0040ffb8 14, pilotgui 19; 6 parked) |
| s1 | sol | multi\mstart.c | done (39) |
| s2 | sol | 0044950c sim\sim.c + sim\simdo.c | done (67 + 4 small TUs; 36 parked) |
| s3 | sol | 0043b7e0 (unnamed, 79 functions) | done (65; 14 parked) |
| s4 | sol | 00409408, 0043ef34 (unnamed) | done (27; 4 parked) |
| s5 | sol | 00435dec, 0041d774 | done (49; 26 parked) |
| s6 | sol | shell\hbupgrad.c (from w14) | done (42; 18 parked) |
| s7 | sol | 0044688c, 00446848 | done (pop\ppilot.c 16, pherc 2; 3 parked) |
| s8 | sol | 00433640, pop\pdeath.c | done (18: terr\tbuild.c 10, tu_004349c0 4, pdeath 4; 7 parked) |
| s9 | sol | 0040d2b0, 0040e794, 0040f4d8 | done (54, none parked) |
| s10 | sol | 00454210, 004556f0 | done (27; 10 parked) |
| s11 | sol | 00438374, 004394e0, 0043a294 | done (54; 14 parked) |
| s12 | sol | 004413e0, 004425d4, 00443100 | done (30; 17 parked) |
| s13 | sol | 00407204..00409408 | done (40, none parked) |
| s14 | sol | terr\tdisplay.c, tarea.c, tsurface.c (from w13); 0042cf40, 0042e210 | done (40; 46 parked) |
| s15 | sol | 0041c7d8..0041d774, 0046b918 | done (30; 15 parked) |
| s16 | sol | 00417efc..00418ef0, 004481c0 | done (33; 7 parked) |
| s17 | sol | sim\anim.c, animbmp.c, simhot.c (from w10/w12); 00460800 | done (26; 25 parked) |
| s18 | sol | logic\logic.c + sim\hotbrief.c, logic\lactors.c, intel.c (from w12/w9); 00464d7c | done (43; 29 parked) |
| s19 | sol | 004742bc..00477dd8 | done (115 in 15 TUs; 11 parked) |
| s20 | sol | 0045a050..0045a600, 0045ad00..0045bc50, 0045cc00..0045dbd0 | done (21: sim\animfluf.c, sim\weapai.c, 5 unnamed; 18 parked) |
| s21 | sol | 00415554..00415a88, 00426cdc..00427e0c, 00429708..0042a0e0 | done (50; 7 wait on GuiPoint COMDAT naming, blocked.md) |
| s22 | sol | 004323c0..00432f40, 00435084..00435dec, 0043e3ac..0043ef34 | done (23; 11 parked) |
| s23 | sol | 00443e88..004451e8 (around pop\psystem.c) | done (22; 4 parked) |
| s24 | sol | 0047ab81..0047c280 | done (50; 5 parked) |
| s25 | sol | 0047d400..004816cc | done (77 in 13 TUs; 6 parked; runtime/asm exceptions listed in its report) |
| s26 | sol | 0040bec0..0040d2b0, pop\pherc.c (2nd pass) | done (29: shell\biomenu.c, pherc +4; 11 parked) |
| s27 | sol | 2nd pass: shell\hercbuy.c, shell\hotseat.c, multi\link.c | done |
| s28 | sol | 2nd pass: logic\lgen.c + sinterp.c, terr\tgrid.c | done (lgen +5, tgrid +10; sinterp blocked, blocked.md) |
| s29 | sol | 2nd pass: shell\shell.c, io\resource.c | done (resource +3, shell +1 over 3 rounds; ProcessInput/Update/UpdateMainMenu parked) |
| s30 | sol | 2nd pass: 00435dec, 0043b7e0 | done (+8; 24 parked) |
| s31 | sol | 2nd pass: shell\hbupgrad.c, 0046b918 | done (+9; 23 parked) |
| s32 | sol | 2nd pass: sim\sim.c region (0044950c) | done (+4; 30 parked) |
| s33 | sol | 2nd pass: terr\tdisplay.c, tarea.c, tsurface.c and neighbours | done (+14 in candidates; 32 parked) |
| s34 | sol | 2nd pass: sim\anim.c, logic\logic.c + hotbrief.c, lactors.c, intel.c | done |
| s35 | sol | 2nd pass: shell\mplaunch.c, main\WIN\winmain.c, shell\pilotgui.c, shell\commscrn.c | done |
| s36 | sol | 2nd pass: sim\actorlst.c, simgame.c, explo.c, 0045cc00 | done |
| s37 | sol | 2nd pass: logic\miss.c, lsys.c, multi\linkapp.c, chat.c | done |
| s38 | sol | 2nd pass: unnamed 0041d774, 004413e0, 00433640, 00435130, 0047f060/0047f5e0, 00480ee0 | done (+5) |
| r1 | sol | research: systemic blockers of parked functions | done (nested-default rule; inventory in codegen_rules.md) |
| s39 | sol | classify unmatched non-game code (runtime library, assembly) | done (config/nongame.json) |
| s40 | sol | integration debt: GuiPoint COMDAT, MCI owner, sinterp verify divergence, aliases | done (all three resolved; alias census candidates/s40/ALIASES.md) |
| r2 | sol | research: register/stack allocation cluster | done (+4; no general fix; rows in codegen_rules.md) |
| s41 | sol | 3rd pass: sim\sim.c region | done |
| s42 | sol | 3rd pass: shell\hbupgrad.c, pop\pherc.c, 0043b7e0 | done (+2) |
| s43 | sol | big functions: 0043ef34 region, sim\explo.c, apply.c, simhot.c | done |
| s44 | sol | logic\lgen.c + sinterp.c, 00452950, 00454210 | done |
| s45 | sol | terr\tsurface.c recursive helpers (inline_recursion hypothesis) | done (+3; recursive helper still 5984 vs 7456) |
| s46 | sol | hand-assembly routines (asm profile, assembler identification) | done; integrator found TASM -ml -m5: 11 of 12 STRICT |
| s47 | sol | TU completion: 17 TUs blocked only at TU level | done (11 COMPLETE published; 6 need bindings/migrations) |
| l1 | luna | shell\hercbuy.c | done (19) |
| o1 | opus | multi\linkdlg.c, linkq.c, message.c | done |
| o2 | opus | sim\apply.c, fxout.c, explo.c | done |
| o3 | opus | logic\lgen.c, logic\sinterp.c | done |
| o4 | opus | shell\commscrn.c | done (32) |
| o5 | opus | multi\minipkt.c | done (33) |
| o6 | opus | sim\simgame object, tu_00452950 | done (36); multi-file objects open |
| o7 | opus | main\WIN\winmain.c | done (15) |
| v1 | sol | adversarial gate audit (rounds 1-2) | done; all findings fixed |
| integrator | - | rate, input, ply, timer probes | partial |

Unassigned large modules: sim\sim.c + simdo.c (0044950c, 30 KB), shell\mplaunch.c (0041f648), 0043b7e0, 00409408,
0043ef34, 00435dec, 0041d774, 0044688c, 00433640, pop\pdeath.c, and the parked tails of finished lanes.
| s48-s51 | sol | near-miss functions (<= 16 differing bytes), 4 disjoint TU sets (build/prompts/nm*.txt) | running |
| s52 | sol | TU completion round 2 (5 TUs + linkdlg/chat literals + pdeath migration plan) | done (pdeath split into pdeath.c + tu_00449148.c; 61 TUs complete) |
| s49 | sol | near-miss set 1 | done (RenderClipLine asm, hbupgrad +1, worldact +1) |
| s48 | sol | near-miss set 0 | done (0 new; exact residues in candidates/s48/REPORT.md) |
| s51 | sol | near-miss set 3 | done (0 new; causes in candidates/s51/REPORT.md) |
| s50 | sol | near-miss set 2 | done (+8 incl. producers: hercbuy, menu, resource, tu_00417f34) |
| s53 | sol | path strings in _DATA (static char File[] idiom), 11 TUs | done (all 11 at oracle address; memory.c split open) |
| r3 | sol | research: register allocator model from Open Watcom cg source (build/ref/ow) | done (model valid on probes; 0 new; rows in codegen_rules.md) |
| s54 | sol | non-allocation blockers: switch layout, shared tails, EBP frames, control flow (88 functions, build/prompts/s54_targets.txt) | running |
