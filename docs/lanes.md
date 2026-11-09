# Lane assignments (integrator-owned)

One owner per module at a time. Candidates live in `candidates/<lane>/`, reports in `candidates/<lane>/REPORT.md`.

| Lane | Modules (start VA: file) | State (2026-11-09) |
|---|---|---|
| w1 | 0047d4c0 debug\assert.c, 0047c280 misc\strpool.c, 0047eb00 io\memory.c, 0047e450 io\file.c | done; parked tail handed to b1 |
| b1 (sol) | w1's tail; then 00425978 (unknown), 004260c4 gui\flic.c | running |
| w2 | 0047c530 io\resource.c | running (round 2) |
| w3 | 00463b18 logic\lactor.c, 004654cc logic\miss.c, 00465d30 logic\bestloc.c | done; 14 parked |
| w4 | 0046d538 multi\linkapp.c, 0046cdbc multi\link.c, 0046e570 multi\chat.c | running (round 3) |
| w5 | 0042acf0 terr\tgrid.c, 00432f40 terr\tfog.c, 00434c80 terr\tfast.c | running (round 3) |
| w6 | 00415074 shell\missbrf.c, 00415a88 shell\hercbay.c, 00443f58 pop\psystem.c | running (round 2) |
| w7 | 004561d0 sim\actorlst.c, 00456ca0 sim\actoradd.c, 00456f90 sim\worldact.c | done; 14 parked |
| w8 | 0045a600 sim\target.c, 0046ac20 logic\lsys.c | running (round 2) |
| w10 | 00457390 sim\anim.c, 004599b0 sim\animbmp.c | running |
| o1 (Claude Opus 5.5 trial) | 0046d9ac multi\linkdlg.c, 004706b0 multi\message.c | running (model comparison) |
| s1 (sol, control) | 0046ec5c multi\mstart.c | running (model comparison) |
| w9 | 00464054 logic\lactors.c, 0046639c logic\intel.c | running |
| integrator | 0042a598 (rate), 004279fc (input), 004297d4 (ply), 0047abc0 (timer) | probes, partial |
