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
| w9 | luna | logic\lactors.c, logic\intel.c | running |
| w10 | luna | sim\anim.c, sim\animbmp.c | running |
| w11 | luna | shell\hotseat.c, pop\pherc.c | done |
| w12 | luna | sim\simhot.c, logic\logic.c + sim\hotbrief.c | running |
| w13 | luna | terr\tsurface.c, terr\tarea.c + tdisplay.c | running |
| w14 | luna | shell\hbupgrad.c | done; continued by s6 |
| w15 | luna | shell\pilotgui.c | running |
| s1 | sol | multi\mstart.c | done (39) |
| s2 | sol | 0044950c sim\sim.c + sim\simdo.c | running |
| s3 | sol | 0043b7e0 (unnamed, 79 functions) | done (65; 14 parked) |
| s4 | sol | 00409408, 0043ef34 (unnamed) | running |
| s5 | sol | 00435dec, 0041d774 | running |
| s6 | sol | shell\hbupgrad.c (from w14) | done (42; 18 parked) |
| s7 | sol | 0044688c, 00446848 | running |
| s8 | sol | 00433640, pop\pdeath.c | running |
| s9 | sol | 0040d2b0, 0040e794, 0040f4d8 | running |
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
