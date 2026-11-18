# nr4 near-miss round

All five targets remain open at their original byte counts. Ten hypothesis families covered 122 full-TU builds with the pinned compiler. All four final promotion dry runs report zero losses. The accepted and unmatched peers retained their baseline status, extent size and differing-byte count. The gate reported no new bindings or twin-helper warnings.

## Results by function

| Function | Final extent | Triage and experiments | Remaining difference |
|---|---:|---|---|
| `00419d52 f_00419d52` | 402 B, 6 differ | Earlier work varied the timer and direction captures independently. The comparable `CommsAnimate` loop suggested testing prefix updates together with typed playback fields and direction capture. A second family paired image/available-byte captures with the timer exit and reused the existing stream reader. Sharing the reader preserved D6. | The original holds direction in EDX and the incremented frame in EBP. The candidate uses ESI and EDI. The decrementing arm already matches. |
| `004194c4 f_004194c4` | 945 B, 4 differ | Earlier declaration and statistic-helper probes left a tradeoff with the name loop. Both statistic callees now have matched definitions in `src/core/mcphmisc.c`. Tested their exact declarations with result type and mission/name capture, then all six alignment-store orders, both state-byte store orders and helper versus direct calls. | The original saves mobility in EDI and free power in ESI, then pushes them in order. The candidate saves mobility in ESI and free power in EDI. Both calls resolve to the right addresses. |
| `0046718b IntelStateSeek` | 553 B, 6 differ | Earlier unit/metadata accessor probes restored the separate read using the unit base. Tested the shared `ActorRecord::action` and `AnimAction::IsMoving` with three case-3 exits. Also passed the action and a captured state to one predicate, varying pointer/reference and early exits. The best shared-member variant was 533 B with 328 differences and omitted the final state reload. | At +0xbb the original adds 0x20 before copying EAX to EDX. Its fifth comparison reloads from [EDX+5]. The baseline copies before the add and reloads from [EDX+0x25]. |
| `00414204 CommsScreenRender` | 612 B, 6 differ | Earlier switch-width, alias and receiver changes left D6. Tested switch expression together with shared redraw versus per-arm returns, then case-1 flags/exit with case-3 blink and button captures. Used the bound `mainSelectedMission` name throughout. | The original zeroes ESI, copies AX into SI and indexes through ESI. The candidate masks and indexes EAX. The resolved differences are exactly +0x1d through +0x21 and +0x25. The table-address operand matches in this build. |
| `004028d0 Shell::UpdateBioderm` | 825 B, 9 differ | Earlier reason-capture and break/return changes left D9. Tested `RestoreMenu` input capture/accessor together with three case-5 exits, then hit-test normalization together with escape-result fallback. A named input pointer gave at best D25 and cost a STRICT peer. | Six local bytes exchange EBX (original input pointer) and EDI (original reason). Three branch displacement bytes at +0x262, +0x311 and +0x32d reflect a four-byte difference in distance to `Shell::ProcessInput+0xc82` at 00402182. The j3 briefing-order control traced those three to the earlier `Shell::Update` stride. |

## Retained source and dry runs

`hbupgrad.c` uses the two proven `int (GameSession *, int)` declarations and calls the existing `UpgradeReadStream` implementation. The redundant `UpgradeStreamStep` body was removed. `commscrn.c` uses the bound short `mainSelectedMission` at 004c975c. The Intel and Shell candidates are byte-for-byte copies of their starting sources.

```text
python tools/promote.py candidates/nr4/shell/hbupgrad.c --tu src/shell/hbupgrad.c --as 'shell\hbupgrad.c' --profile plain
src/shell/hbupgrad.c: 60 strict functions (13082 bytes), new 0, lost 0

python tools/promote.py candidates/nr4/shell/commscrn.c --tu src/shell/commscrn.c --as 'shell\commscrn.c' --profile plain-oe
src/shell/commscrn.c: 39 strict functions (10691 bytes), new 0, lost 0

python tools/promote.py candidates/nr4/logic/intel.c --tu src/logic/intel.c --as 'logic\intel.c' --profile plain-oe
src/logic/intel.c: 14 strict functions (4906 bytes), new 0, lost 0

python tools/promote.py candidates/nr4/shell/shell.c --tu src/shell/shell.c --as 'shell\shell.c' --profile plain-oe
src/shell/shell.c: 47 strict functions (15990 bytes), new 0, lost 0
```

[Attempt ledger](attempts.jsonl) · [Resolved byte offsets, original/candidate](../../build/nr4/residues.txt) · [Variant templates and ranked results](../../build/nr4/)
