# ss2 report

STATUS: partial

Reviewed all 23 target histories and fresh full-function listings under the recorded flags. Ran 31 bounded source probes across 20 targets; left three exhausted construct families unchanged (HercAdvanceTurn, CommsScreenRender, TerrainSetTypes). No new STRICT functions. The two retained changes are diagnostics, not acceptance progress.

## Retained files and promotion dry runs

python tools/promote.py candidates/ss2/io/resource.c --tu src/io/resource.c --as 'io\resource.c' --profile ot-oe

Summary: dry run OK; 17 existing STRICT functions, new 0, lost 0. ResMgr::Init (0047c5f0) stays 208 bytes; capture each path once for its stored path and subsequent calls: D183 -> D135. Assertions remain on lines 69 and 81. Log: build/ss2/promote_resource.txt.

python tools/promote.py candidates/ss2/sim/apply.c --tu src/sim/apply.c --as 'sim\apply.c' --profile ot-oe

Summary: dry run OK; 3 existing STRICT functions, new 0, lost 0. ApplyVisit (0045c220) stays 2032 bytes; signed char average restores original MOVSX at +7cb: D1632 -> D1603. ApplyOne stays 1056 bytes/D641. Log: build/ss2/promote_apply.txt.

## Target results

D = differing bytes in diagnostic function comparison, never a STRICT count. A rejected probe's D may decrease while size is wrong. All rejected probes remain under build/ss2/probes; only the two files above differ from src.

| VA | Function | Retained result and attempt |
|---|---|---|
| 0043a2d5 | HercAdvanceTurn | 706 B/D4. Confirmed shield clamp result-home difference; prior single-helper and explicit-clamp families exhausted. No new evidence; no replay. |
| 00414204 | CommsScreenRender | 612 B/D6. Confirmed word-switch conversion homes; prior width/cast/enum/accessor/alias families exhausted. No replay. |
| 004507d0 | SimApplyLeadership | 352 B/D8. Inline penalty helper: 352 B/D219, rejected. |
| 0042afb0 | GridPointToIndex | 496 B/D68. Member lookup unchanged; copied center gives D122. Restored. |
| 0047cfc0 | MenuDrawTextAt | 192 B/D84. Narrow and unsigned boolean divisor conversions give D167/D157. Restored. |
| 00433d40 | TerrainSetTypes | 272 B/D94. Confirmed byte-index zero-extension construct; prior accessor/type/capture families exhausted. No replay. |
| 004352e0 | DrawTerrainRows | 576 B/D146. Compound width calculation shrinks to 560 B/D382 (anchored). Restored. |
| 0047c5f0 | ResMgr::Init | **208 B/D135 retained**, path capture; three test-only/reuse follow-ups D181. |
| 00456d80 | ActorListAdd | 518 B/D263. Separate index decrement grows to 519 B/D472. Restored. |
| 00434070 | TerrainTriplet | 384 B/D293. Direction-row walk shrinks to 368 B/D303 and loses shared-tail neighbor. Restored. |
| 00433a80 | TerrainHeightRange | 512 B/D312. Histogram-first update gives D311 but wrong frame remains. Restored. |
| 0044b9c0 | SimSetupWorld | 480 B/D318. Typed copy addresses unchanged. Candidate already has all three REP MOVSD copies, differently scheduled. Restored. |
| 00433e50 | TerrainSetWater | 544 B/D351. Explicit dtor expression adds cleanup and grows to 560 B/D369; invalid alongside automatic cleanup. Rejected. |
| 004341f0 | TerrainFeatures | 1344 B/D451. Unsigned maximum-percentage comparison D452. Restored. |
| 00426814 | Flic::DecodeFrame | 781 B/D569. Four zero-store/snapshot forms give 780..785 bytes/D610..704. Restored. |
| 004324d0 | BuildTerrainPath | 928 B/D579. Point Set/Add gives 944 B/D816 and loses GridSetSelection. Restored. |
| 0045be00 | ApplyOne | 1056 B/D641. Conditional slot initialization D904. Restored. |
| 00454cd0 | SimBuildDrawQueue | 1184 B/D641. Long dirty capture stays D641, changes homes but no proof. Restored. |
| 0047cc50 | RenderResourceGlyph | 816 B/D660. Positive range conjunction unchanged; last-column capture D676. Restored. |
| 00462920 | HotbriefProcessOpposingActions | 1040 B/D751. Final comparison return grows inlined caller to 1056 B/D778. Restored. |
| 00457af0 | AnimActionCommitTrajectory | 1184 B/D991. Product capture shrinks to 1089 B/D967; point pointers restore 0x68 frame but D1018. Restored. |
| 0045c220 | ApplyVisit | **2032 B/D1603 retained**, explicit signed average. Heavy predicate helper grows to 2048 B/D1686. |
| 004682dc | ScriptStep | 3295 B/D2882. Timer LHS capture grows to 3309 B/D2915. Restored. |

## Evidence and limits

- candidates/ss2/attempts.jsonl contains 27 family/audit records covering all 23 targets, created through tools/notes.py add.
- build/ss2/<VA>_notes.txt, _parked.txt, _base.txt preserve history, core-difference output and fresh full listings.
- build/ss2/batch1.json through batch4.json and probes/*/result.txt preserve every source change and compile result. Two formerly UNPLACED diagnostic functions were also compared with explicit --at; see their anchored.txt.
- Several advertised CORE DIFFERENCE lines are instruction-alignment or relocation-display effects. For example SimSetupWorld's allegedly absent REP MOVSD operations are present later in the candidate. Full source/record-lifetime reconstruction remains necessary for these targets; equal extent size alone does not establish one isolated faulty operation.
- No new callee/global bindings, no twin helper retained, no --publish, no git changes. Writes confined to candidates/ss2 and build/ss2; compiler/tool temporary directories redirected inside build/ss2 by run.py, using the unmodified pinned tools.
