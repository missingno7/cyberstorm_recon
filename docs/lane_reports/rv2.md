# rv2 structural review of CSTORM.EXE 1.10A

18 November 2026. Fresh `verify.py` returned VERIFY OK, 2,094 / ~2,413 C/C++ functions, 330,277 / ~520,056 bytes, 17 / 17 assembly functions, 21 / 21 resources, and 75 / 185 complete TUs (89,798 main-code bytes). All proposed changes below are dry runs. Project totals remain those of verify.

| Priority | Integrator action | Measured payoff |
|---|---|---|
| 1 | Review hercbuy, qstart, strpool and the GDI status-text TU | Four currently incomplete objects pass the object checks, including the global checks, with 11,310 main-code bytes. Every recorded function retains its VA and size. A separate lactor identity fix immediately adds two STRICT functions / 44 bytes. |
| 2 | Adopt the linked-range comparison and start an exact external-symbol worklist | Two complete decoder objects plus their inter-object fill reproduce 282 bytes and all eight ordered HIGHLOW records under two declared external placements. A separate C++ range already reproduces 324 contiguous bytes across an object boundary. The all-object link still has 2,351 undefined names. |
| 3 | Fix switch-table ownership at object boundaries and at scan-key tables | Two present object failures (1,051 code bytes) are solely the next object's table. SearchGetCost and ScriptFormatArgs are two false waits. One funcmap orphan lies inside a word table. One counted STRICT predecessor includes another function's 44 key bytes. |
| 4 | Review the four core names and misc/format | Five concrete renames, 33 existing STRICT functions / 6,599 bytes retained. The candidates introduce whole-segment proof for the five _DATA contributions. |
| 5 | Audit the advertised COMPLETE objects for omitted segments and BSS overlap | One 56-byte BSS collision is reproducible across versions. Two linked runs omit 168 and 136 initialized bytes respectively. A candidate accounts for 136 of the first 168 bytes. |
| 6 | Repair the LogicActor identity and prioritize real shared-tail roots | One 24-byte initializer is blocked by an incorrect matched ctor identity. One two-byte length difference in mphbase holds four otherwise-correct functions (2,680 bytes). |

## Four source fixes that reach the object gate

**Herc purchase screen, 6,816 main-code bytes.** The original main code ends at 00417ee4. The deleting helpers for MenuPrefixButton and MenuLabel occupy 00417ee4 and 00417efc (21 bytes each, aligned to four). The screen destructor follows at 00417f14. The current source makes it an out-of-line member at main-segment offset 6,816, placing it over the first deleting helper in a contiguous link. Its own separate function comparison succeeds. A second class, HercBuyInitScreen, has its ctor/dtor names bound to exactly the real screen ctor/dtor (0041673e/00417f14). Define the global with the actual screen class and the destructor in-class. This moves its body to a COMDAT and eliminates the spurious main-code gap.

The ctor at 0041673e passes the controls at 0048eb48 and count 0x31, then constructs the Flic at this+0x5d. The candidate owns the 49 relocated control pointers, the writable file path used by both asserts, and the two-long tab record {0,70} read through 00417852. The file path begins at 0048eb38. The entire 232-byte _DATA segment runs from 0048eb2c to the hbmanage header at 0048ec14. The two asserts retain lines 185 and 785. The screen's last accessed byte is +0xfe7. Its start 004b416c plus 0xfe8 equals the independently placed next BSS at 004b5154. The BSS contribution now covers exactly 004b38b4..004b5154. Names and the unobserved portions of the class are working declarations. Watcom also emits an unplaced optional type signature. The object check excludes pick-any COMDAT copies.

The existing data gate treats a size increase as a loss. Hercbuy requires the exact exception shown in its receipt. I checked the old 12-byte _DATA prefix is at the same VA, the full replacement is STRICT, and every prior recorded data segment and function is covered. That exception is specific to this candidate.

**Quick start, 3,115 main-code bytes.** The two local strings occupy CONST 0048ab54 and 0048ab5f. The third “Unable to load mission file” operand at 0041d59c addresses 004a4578, surrounded by other initialized error messages. A literal in this object appends a third string to CONST and prevents whole-segment equality. An external array at the actual target leaves the 22-byte CONST STRICT. Dry run implies the new working name missionLoadErrorText. Its ultimate definition still belongs on the image-link worklist.

**String pool, 682 bytes.** The compiler emitted three anonymous copies of the file path in a 45-byte CONST. All three oracle fixups (0047c2e1, 0047c42a, 0047c489) point to the same writable path at 004a49e8. One static char array initialized by __FILE__ preserves the assert immediates and proves all 15 bytes of _DATA.

**GDI status text, 697 bytes.** Only “Modaerne” belongs in the object's CONST at 0048bed4. The two copies of the padded CyberStorm window title used at 004766c8 and 004767db both address 004a4728, also used by help and winmain. Reuse the existing helpDefaultTitle binding. The resulting nine-byte CONST is STRICT. Keep the old filename pending independent evidence for main/hint.c.

## Linked ranges and what still prevents a full link

**Two complete objects, eight relocations.** Run `python -B candidates/rv2/link_range.py`. It compiles the two ledger Flic assembly sources through cc.py and links them in order with the pinned WLINK. The comparison covers 0047ec9c..0047edb6 including two zero alignment bytes at 0047ed0e..0047ed10. The emitted range starts at 00401000 in the probe PE. All 282 bytes equal the oracle with SHA-256 `b95a1260a433d544372aad574d400a4811b5a1ea89d1b245aa2412f0525e1377`. The ordered HIGHLOW offsets in decimal are 140, 154, 162, 179, 185, 195, 252, 263. WLINK resolves every symbol without undefsok. The unexecuted test fixture uses the two recorded external target VAs packedBlocksLeft=004a4bb4 and packedTailBits=004a4bb8 and an explicitly sized uninitialized reservation. It contains no code in the comparison. Moving both targets by +4 changes eight bytes while all relocation sites still agree. This is a **hybrid code-range proof**. The natural placement of those two globals and the object's absolute code RVA remain open.

**A second cross-object control with no fixture.** Fresh linkprobe run 12 has an exact 324-byte subrange at linked 00401110, original 0047efd0..0047f114. It contains PointInConvexPolygon, nine bytes of linker fill, Crc16Byte and Crc16. Both objects' OMF fixup inventories are empty inside this window, both PE relocation inventories are empty there, and the byte sequence is unique in the linked code section. SHA-256 `7f3e1c8c07cebd466bf0514ee5144bb3224560e257de171a24220818741aca42`. Reversing the two input objects removes that sequence entirely (314 differing bytes at the old offset). Five unresolved externals, including VectorAtan, occur outside the window in the first object. The test covers three complete functions and one seam, not both complete objects.

**Current placement evidence.** Fresh linkprobe -v reports 38 TUs in 15 adjacent runs and 56 / 64 relative segment offsets equal. Reapplying tucheck's full criteria independently produced the same 15 runs on this snapshot. The eight discrepancies are:

- Run 1 has three _TEXT positions +0x88 from the slider's first-definition UpgradeBar/UpgradeArrow dtor COMDATs. The original kept earlier copies at 004155xx. Its _DATA is 0xa8 short, detailed below.
- Run 4 has one _DATA position 0x88 short across the intermediate palette object. The code positions agree.
- Run 8 has two _TEXT positions +0x14 after the settings object's FileRecord ctor COMDAT (0x12 bytes plus alignment).
- Run 14, RenderClipLine to DecodeResourceBlock, has original +0xfc, linked +0xf0. The ASM contribution is 238 bytes. The original begins at 00481194, four modulo sixteen; the stand-alone probe begins at zero modulo sixteen. It inserts two fill bytes where the original inserts fourteen. The source of the discrepancy is the run's starting alignment. Match the first object's address modulo the largest alignment when testing offsets.

The probe counts a first segment observation as an automatic equality, uses unique fixup-masked byte hits for placement, skips unlocated segments and BSS, and excludes COMDATs from the layout inventory. It does not compare linked fixup values. The implementation also omits data_gaps when selecting eligible objects and does not check WLINK's return code before testing whether an output exists. Use fresh outputs, reject failed links and run the same eligibility tests as verify. Keep compared-byte coverage, ordered relocations and measured segment deltas separate.

**Natural link debt.** I linked all 185 fresh objects as explicit FILE inputs as a diagnostic, without undefsok. The map and log have 2,351 distinct undefined-reference warnings and 3,972 file/reference diagnostic lines (exit 1). Explicitly adding NT CLIB3S, PLIBMT3S, MATH387S, NOEMU387 and the four NT import libraries produces the same unresolved set. The default link was already extracting runtime members. This ordering intentionally tests name closure; it does not reproduce the engine archive extraction. Its map selects cstrtww2. Forcing _wstart_ on top of that selected member produces “multiple starting addresses”. Check the original startup fixups and make the startup/member selection explicit before comparing headers.

The database contains 5,624 bindings, including 3,513 hypotheses, and 905 addresses with multiple keys. Across all fresh OMF inputs, 765 referenced external spellings have no same-spelling supplied global PUBLIC yet have a ledger-bound address with a **different** supplied PUBLIC. These span 625 addresses and 5,770 input fixups, including discarded COMDAT candidates. Counts overlap the WLINK warnings and do not measure missing functions. Examples include 145 references to DebugTextAppend(char*) versus the supplied const-char version at 00476014, 102 C++ AssertFail references versus the C-linkage definition at 0047d5a0, and hundreds of array/scalar/object spellings for the GuiCanvas at 00513888 and GameSession at 004c934c. No conflicting VAs were found for one exact unscoped global mangling in the scanned bindings. Distinct manglings at one address are the dominant problem. Reconcile declarations with definitions, preserving each caller's register/stack ABI and default arguments. Rebuild affected callers after each group. Use exact OMF names in a closure report. A blanket address-to-name substitution would conflate distinct class signatures and overloads.

For part A, recover explicit object order and missing data-only contributions, check BSS end and start alignment, and require first-occurrence COMDAT names/bytes, import entries and every external fixup of the chosen range. For part B, build a real engine library with the recovered member set and let WLINK extract it alongside the runtime. The different extraction order observed in cs10 is positive evidence for this model (see format below). For a whole-file claim also compare PE section order and sizes, imports/thunks, ordered relocation stream, startup/header settings, and resource binding. Resources already rebuild 21/21. Bind timestamp and the final 88 uninitialized slack bytes remain explicit exceptions. The older last sentence about unreproduced resources in verification.md is stale.

**Effort estimate.** The bounded hybrid milestone is reproducible now. Allow roughly one to three integrator days to generalize it with public-to-map attribution, seam/alignment checks and positive and negative cases in the gate. A C++ run with real data imports should be chosen by smallest **unresolved exact-name set**, accounting for retained COMDATs. Several days for the first such run is a planning estimate conditional on finding the data owners. Whole-image closure is a multi-week-or-longer reconstruction effort even before final headers and resource slack. The unresolved names, 110 incomplete objects and unknown archive extraction make a tighter estimate unjustified.

## Boundary and identity corrections

**Two additional STRICT functions hidden from the parked queue.** parked.py and blockers.py both skip unplaced extents. The fresh reports contain nine more main-code extents with no VA: two bestloc, four lactor, one linkg and two anim. The surrounding placed publics identify small intervals with the same number of original starts. Diagnostic placement left seven DIFF. In lactor, however, both 22-byte wrappers are exact. The first loads the actor's state and calls the matched ActorCrouch at 00455d60 (call site 00464003); the second calls ActorStand at 00455e50 (site 00464019). Their existing independently inferred caller names in lactors.c are ActorRandomAction at 00463ff6 and ActorFinalize at 0046400c. Replacing all four provisional spellings with the existing exact declarations proves both functions without --at, --find-dependent placement or new alias bindings. ActorCanAct and ActorSelect still fail when diagnostically placed at 00463d66 and 00463de8. Add an UNPLACED category outside the 253 placed functions and show its bracketing publics. The other five candidate starts for follow-up are 00465d31, 00465f1c, 00474419, 004577d0 and 00458840. Do not infer that all 66 map starts without a placed extent lack source.

**Cross-object leading jump tables.** tucheck.check_reports walks from the object end to the next *function start* and demands zeros throughout. At 004275ba the old object ends, two zeros follow, then eight HIGHLOW dwords at 004275bc feed the first function of the next TU at 004275dc. At 0042a6c9 there are three zeros and seven HIGHLOW dwords at 0042a6cc feeding RichTextCommand at 0042a6e8. The receiving OMF first-public offsets are 32 and 28 respectively. RichTextCommand already proves its table STRICT. PlyMatch is DIFF. Both predecessors otherwise pass, for 846 and 205 bytes. Use the identified next object's leading segment boundary as the end of the zero-fill test. Require independent oracle jump references, table length/sites, target containment and legal fill, retain the receiving function's table obligation, and reject an extra nonzero byte before the table. If the gate requires a fully STRICT receiver first, the 205-byte case can ship immediately.

SearchGetCost at 0042d370 has all 84 bytes equal, and ends at 0042d3c4. Twelve zeros precede *another object's* five-pointer table at 0042d3d0; compiler filler then reaches first public 0042d3f0. The cmp report currently expects the predecessor to reach 0042d3f0. Validate the receiver's leading table/padding independently. SearchGetCost need not wait for a source rewrite in that TU. Two other code extents still fail in its own object.

**A false function inside switch keys.** At 00468316 ScriptStep loads ECX=0x25, EDI=00468200, executes repne scasw and jumps via [ecx*4+00468248]. There are 72 key bytes up to the pointer table and 37 relocated destinations after it. The mapped “orphan” 00468208 is eight bytes into those keys, with zero callers and zero pointer references. ScriptFormatArgs returns at 004681fc followed by three bytes of compiler fill. compare reports zero differing bytes but compares through 00468248 and complains that the original ends at 00468208. Recover both the key interval and pointer interval from the consumer, retire the orphan, and check keys and destinations under the switch owner. Account for the scanner's default slot instead of treating 37 as the physical word count.

Four more occurrences of this instruction form have key/table starts 0040146c/0040149c, 0040967c/004096a8, 00469720/0046974c and 00479730/00479770. PromoScreen::Open at 004094b9 is counted at 495 bytes through 004096a8, including 44 bytes of the following switch's keys. Retain these bytes in coverage under the consuming switch and explicitly migrate the extent in the ledger. Negative tests should change a key while leaving all branch pointers untouched, and change a destination while leaving the keys untouched.

**The first three “game” starts are runtime and fill.** Pinned NT/CLIB3S.LIB member segdefns has a seven-byte BEGTEXT segment `cc eb fd 90 90 90 90` with public ___begtext at +3. The first seven oracle bytes match exactly and nine zeros align shell's initializer at 00401010. cstrtwwt's fixup at member +0xf names ___begtext, and the oracle at 0047fea7 holds the relocated pointer 00401003. Classify 00401000, 00401001, 00401003 as runtime/glue in the census and account for the linker fill separately. libscan's eight-byte minimum anchor misses this seven-byte member. Conversely 004765b1 is a **game** function mislabeled as NT/CLIB3S:getmbcp. Its 16 bytes read the same helpFont global that the adjacent GDI init/close uses; the GDI source is STRICT (and present in the ledger). The libscan match masked that operand. Remove that library identity and require independently consistent target data for singleton short runtime matches. Apply these changes before recalculating the denominator.

**The LogicActor constructor and destructor are reversed.** At 0048e0d8 the 17-byte descriptor has default ctor +1 = 00464d74, copy ctor +5 = runtime fallback 0047e008, dtor +9 = 00464d6c and stride +13 = 0xa8. All three pointer sites are relocated. The OMF uses those exact field roles. The constructor-array routine at 0047e068 consumes slot +1. Both game bodies are `8b 44 24 04 c3`. The ledger calls 00464d6c LogicActor::$ct and leaves the actual ctor 00464d74 without that identity. Hence the descriptor is DIFF and initializer 00464054 is 24/24 with one UNBOUND name. Rebind these together and prove the descriptor, registration code, both support bodies and initializer in one transaction. Do not bind the failing signature by its address alone. Prior g4/s18/s34 trials found that adding a destructor naïvely generates extra registration; this audit independently confirms the failure mechanism. This is the sole current relocation-only main extent.

**Other COMDAT identities.** At 0048df88 only the 0x28-byte-element SimValueGadget descriptor proves STRICT (simalert and simgad). SimImageGadget copies in pherc/pmove are DIFF by four bytes; SimMapGadget copies in mapalert/simalert/simgad by three. Pointer +9 is 0044d710. PilotImageButton's descriptor at 0048cdb8 has different stride 0x2c and dtor 0041235c, also the bound SimImageGadget dtor. Reconcile SimImageGadget with PilotImageButton and test that descriptor. Clear the unsupported SimMapGadget binding until its own ctor/dtor/stride locate it. A function's accepted TU can supply a COMDAT copy whose bytes really lie after an earlier object. Audit all emitting TUs and the first kept definition before physically moving functions. The two “no TU” XI targets in objcensus, 00461d90 and 00464054, are already emitted by hotbrief and lactors respectively. The accepted-function hull omits their first non-STRICT extent.

## COMPLETE can conceal missing data

tucheck.data_gaps only compares the segment if **both** members of an adjacent pair emit and place it. A middle object that declares all its data extern bypasses the check. There are zero reported order outliers and zero reported pairwise gaps on the baseline. check_reports also returns before assigning a code placement when any main extent fails, so ordering and contiguity findings on mixed objects are lower bounds.

- Slider run. Original _DATA offset to palfade +0xb8 versus linked +0x10. The middle object 00429ea4 reads menuReservedColor at 0048f40c and menuPaletteColors[33] at 0048f410. Defining the 34 four-byte colors proves _DATA 0048f40c..0048f494 and improves the **measured linked** offset to +0x98. Eight more dwords at 0048f494..0048f4b4 are {0x110,0x240,0x500,0xca0,0x1b00,0x3500,0x6000,0xb400}; no direct relocation to an address inside them was found. Keep ownership open. With the candidate, tucheck newly flags both this TU and palfade because it can now see the remaining 32 bytes. Review this result separately from the four completions. Preserving the old COMPLETE label would hide the image debt.
- Mapalert run. Original +0x1fc8 versus linked +0x1f40. After animfluf's path/gap, the interval 004a24e4..004a256c has 136 bytes before the mapalert header. ActorPalette (0045a290 object) reads pulseColors at 004a24ec, waveColors at 004a24fc, flashColors at 004a250c and rampColors at 004a251c; the last is indexed modulo 20. These are currently extern in an otherwise COMPLETE TU. Recover their sizes and ownership, particularly the two leading unreferenced dwords and the unused entries between named bases. Test the whole segment and restart the linked run. General fix for both examples: compare consecutive **placed contributions in a contiguous code run** across intervening empty segments; permit a real data-only member with explicit evidence.
- Chat BSS. Of 81 placed _BSS contributions, one pair overlaps. chatLogPending is at 004f64b8 and chatLogText at 004f64bc. Declaring char[256] ends the chat BSS at 004f65bc, 56 bytes into mstart's connectionLock at 004f6584. Independently in cs10, the corresponding reference at 004694ec reads 004eef34 and mstart's references at 00469a15/3f/5a address 004eeffc. The separation is 200 bytes in both builds. The char[200] candidate exactly abuts the lock, retaining all 15 functions. This is a placement-supported capacity; no length constant in chat's sprintf proves the original C array bound separately. Add an overlap diagnostic using BSS base **plus size**, even when the next TU has parked code.

Across all fresh code extents (including emitted COMDATs) there are 66 game-map starts / 9,081 map bytes with no placed extent. This is an inventory query, not additional verified progress. It includes the false ScriptStep orphan and the startup entries. Largest examples are 0043b044 (908), 00458840 (656) and 004577d0 (528). Check seed evidence and ownership before scheduling them as new source bodies.

## Path ownership after the actorver cut

Fresh objcensus -v finds 145 paths, 73 game XI records, 0 RENAME, 0 SPLIT, 31 NO-TU and 0 ORDER. It only includes C/C++ files with accepted code in its path lookup. Two of the 31 already exist as data-only sources. There are **29 actually missing files**, down one from the old inventory because sim/actorver.c now exists. All 31 reported rows are accounted for below. The direct cs11 and cs10 relocation census found no direct pointer to the five proposed rename strings themselves.

| Path(s) and cs11 path VA | Code range or association, evidence and decision |
|---|---|
| core/mcpbrfs.c 00494234 | 0043e3ac..0043ecf8. Begins immediately after mcp's established 208-byte _DATA and code end. Briefing formatter reads the mcp report tokens; the final jump ends at 0043ecf6, followed by two alignment zeros. Two functions and the old 318-byte CONST survive. |
| core/mcpio.c 00494244 | 0043ecf8..0043ef34. LoadSession and the adjacent file read/write routines serialize the packed session. Last ret at 0043ef30 plus three zero bytes. Four functions retained. |
| core/mcpcamp.c 00494254 | 0043ef34..004413e0. GenerateMissions reads [eax*4+00494264] at 00440b55 and passes the result to MissionRecordInitialize. The path and the three entries {1,2,0} exactly fill 00494254..00494270. The previous extern missionFinalIndex becomes the initialized table. Ret at 004413de, one alignment zero. This is the independently referenced interior anchor of the four-way pairing. |
| core/mcphmisc.c 00494270 | 004413e0..004422f0. Inventory/equipment operations precede the now separately published actorver at 004422f0 (path 00494280). Four distinct code ranges and four paths now lie between the anchored mcp and actorver objects; the old five-to-four ambiguity in n4 no longer applies. |
| misc/format.c 004a4a24 | FormatSetBuffer at 0047d400 and BriefFormat at 0047d420. In cs11, the MCI header at 004a4a18 precedes the format path, then debug/assert. In cs10, the MCI code stays before assert (e.g. 0047d180 -> 00477580; AssertFail 0047d5a0 -> 00477920), while the uniquely masked BriefFormat body moves **after** assert to 00478250. The format path likewise moves to 0049e304, after both assert and graph/pal and before io/file. This relative-order reversal discriminates MCI from the formatter. The rename preserves both functions and the single-byte CONST. |
| gui/comma.c 0048f4dc | One comma-formatting function, 0042ac40..0042ace4 plus linker fill to tgrid at 0042acf0. Six callers, one path after wrapper. Name the object when that function has source. No accepted donor to rename. |
| terr/tmask.c 004905b0, terr/tcell.c 004905c0 | Three separate ranges 004323c0, 004324d0 and 00432930, separated by zero linker fill, for two paths. Mask/raise or selection are viable tmask candidates; movement at 00432930 is plausible tcell. Post-path tables are used by tfog. No direct path references in either build. Identify the pathless member first. |
| terr/tfeature.c 00490630, terr/triver.c 00490640, terr/ttable.c 00490650 | Three consecutive path strings, one XI at 004349c0 before tfast at 00434c80. The height helpers and the stub at 00434c70 leave a possible separate member or data-only owners. No direct pointer in either build decides the split. Check the stub's member and data-only input order. |
| terr/clifftex.c 00490690, terr/bmprgb.c 004906a0 | Cliff span candidate at 00435130, between hand-assembly contributions. Raster table 004906b0 follows bmprgb and is read by the assembly blitter at 00435b5c. An external reference alone does not assign the table to the consumer. Establish the table's owner before naming the C++ span TU. |
| core/herc.c 00493d5c, core/hercsh.c 00493d80, core/hercsim.c 00493fe4 | At least five fragments at 00438374, 004394d8, 004394e0, 0043a294 and 0043b7e0. The second is a separately compiled identical return body. Post-herc data has a 004394e0 user; post-hercsh data is shared by six TUs and post-hercsim data by three. No direct path pointers in either build. Check full table placement and duplicate-function folding before collapsing two fragments to fit the path count. |
| sim/simbug.c 004a0484, logic/slog.c 004a3c9c | Both files already prove 13-byte _DATA segments. Slog has zero space between adjacent named code anchors at 00469c78. Simbug's alternative code owner is 00454180..00454210, currently in simmisc. Record a data-only object result when no-code status has independent evidence; keep simbug's possible function separate from that decision. |
| multi/gameparm.c 004a4060, main/main.c 004a4080 | Options 00474900 and shell lifecycle 00475688 are plausible. DLL adapter 00474978, movie 00474fb8 and other TUs before help make the interval ambiguous. The many consumers of data after main/main.c include winmain, help and pack/unpack. Need owner definitions or a member map. |
| main/hint.c 004a421c, main/prefs.c 004a44dc, main/mcursor.c 004a44f8 | Status text 00476570, settings 00476860 and cursor 00476b3c fit behavior. Both builds put CSTORM.SET immediately after prefs (004a44ec / 0049dc7c). FormatVersion at 0047682d and settings at 00476860 also remain adjacent in cs10 at 00470c99 and 00470ccc. This supports the settings association but does not establish an original split there. Prove the leading version data and the eight bytes following hint, or demonstrate independent object boundaries. The GDI data fix works under its existing name. |
| main/WIN/menus.c 004a4854, main/testrbx.c 004a4868 | Menu object 004774b8..00477dd8 before winmain XI, two paths. The last ret ends at 00477dd5 followed by three zero bytes. A data-only testrbx would explain the count; no justification for inventing an extra function. |
| graph/dirty.c 004a4978 | Clip/dirty-region operations 0047ac80..0047b260 are plausible. Timer and module-anchor code intervene before blitWIN. References after this path are also from blitWIN. Need an independently placed data item or library pull trace. |
| graph/font.c 004a4a08 | Resource TU currently includes a font loader at 0047cba0 (rebases fields 7, 8, 9), MeasureResourceTextWidth at 0047cc10 and glyph/text code through 0047d0e0. MCI at 0047d0e0 is independently fixed by XI and its 004a4a18 header. The suggested cut at 0047cb80 is ambiguous: the generic release there is called from StrPool too, and 0047cb74..0047cb80 is **compiler LEA filler**, not zero linker fill; 0047cb92..0047cba0 and 0047cc0c..0047cc10 also have instruction filler. Preserve the preceding accepted extent sizes. Determine the last resource-owned public or discarded COMDAT before moving the font routines. |
| graph/pal.c 004a4b90 | Palette object 0047d9d0 is the leading candidate. In cs10 its palette-copy code (0047da70 -> 00477df0) stays before the relocated formatter, as does graph/pal's path 0049e2f8. Keyboard (0047dd80 -> 00478db0) and RNG (0047e150 -> 00478330) end up **after** the formatter and are excluded by the path order. Mouse remains in the pre-format interval. Seek a palette-owned data item to distinguish it conclusively. |
| graph/gfx/g_func.c 004a5498 | WinG loader 00480390 is plausible; geometry, CRC, clock, critical-section and assembly objects also lie between memory and g_cds. References to the preceding unclaimed initialized area span multiple members. Need archive-member/data ownership. |
| io/resblock.c 004a54d0, graph/gfx/g_pal.c 004a59d4 | Archive backend 00481360..004816cc fits resblock. Two paths but only one identified game C++ range after unpack in this group. Pointers around 004a59c8 lead to runtime code, including 004842a0. Separate runtime/member data ownership before declaring the last path data-only or assigning it game functions. |

## WAIT cases and the cheapest remaining targets

The fresh parked classifier found 253 placed non-STRICT main functions. Its filters give 146 “no:invariant”, 22 “no:neighbour” and 85 workable. blockers -v gives 230 roots, 20 outgoing-branch waits, two table waits, one binding-only. It lists every intervening non-STRICT root as a potential blocker. I decoded the differing transfers and compared the *landing offset in the destination* in the two objects. A correct length can still leave the shared tail at the wrong offset.

| Waiting functions | Actual dependency and order of work |
|---|---|
| ShellMultiplayerOpen, RefreshMultiplayerControls, MultiplayerHost, MultiplayerWait | All branches land inside MultiplayerAttemptStart, two bytes later in the candidate at the corresponding epilogues. Root 0041eca2 is 896 bytes versus original 894. Restore the epilogue positions first. Four dependents total 913+1,339+234+194 bytes. |
| f_0043ac9c, HercDefense | Branch to the same offsets (+103 in HercBuildDamage, +98 in HercBuildDamageCopy). Both displacements are +4. Check cumulative public offsets across the interval; HercConditionDiagram at 0043ab6e alone is one byte short, so a one-byte edit is insufficient evidence for both releases. |
| f_0043cab9, f_0043e37c | Same target offsets in f_0043bac3 and f_0043c167, but cumulative distance is -7 and +5. The overlapping mcp roots in blockers.txt must be handled as a group. |
| f_0043e21a | Wrong **destination function**: target 0043dcf5, f_0043dbb2+323, resolves to f_0043d2b9+262 in the compiled object, distance error -2353. Source block sharing must select the original survivor. |
| MenuDrawText | Both transfers land in RenderResourceGlyph. Target +809 versus candidate +798, despite equal 816-byte function size. Restore the terminal block position. |
| _WinMain@16, MenuSend, PacketList::Reset | Original and candidate land at the same internal offsets in WndProc (+383), MessageActorUpdate (+209) and the minipkt init (+125). Respective span deltas -53, -11 and -8. Work WndProc, MessageDispatch and PacketApply per blockers. |
| MenuNetwork::DropPeer, ReportDrawOther | Shared targets CheckLeader+62 and SimPilotDraw+157 agree internally. Span deltas -77 and -2. Work StopMultiplayer/CompactPlayers for the former and ReportDrawUnit for the latter. |
| Shell::UpdateSelection, Shell::WithinBudget | Tails inside Shell::ProcessInput are 15 bytes earlier relative to that function; the net branch delta is -3. Restore target placement and sizes of intervening functions, especially Shell::Update. |
| Shell::UpdateBiodermPanel, Shell::UpdateMultiplayerMenu | Both targets are Shell::UpdateStorage+154; distances differ by +12. Two roots in this span, UpdateHercUpgrade and UpdateMainMenuSelection. Inspect cumulative size. |
| ReleaseState | ActorFinishMove tail +552 versus +553. Fix that one-byte local landing difference. |
| SearchGetCost, ScriptFormatArgs | Correct the map/table ownership rules described above before assigning these to a code lane. |

Of the 110 incomplete objects, **58** currently report only non-STRICT code, **44** report non-STRICT code and at least one other failure, and **8** have no failed emitted main-code extent. Those eight are the four source fixes, the two leading-table predecessors and the two zero-code sources. Seven objects explicitly report BSS-UNPLACED (lgen, mplaunch, animout, simactor, simmain, simout and target). A successful placement can still overlap another object, as chat demonstrates.

Triage order after the four source fixes: the 205-byte wrapper predecessor with an already-STRICT receiver; the 846-byte predecessor and SearchGetCost with explicit table-boundary tests; then the data-only check for slog and the LogicActor identity transaction. For traditional one-function closures, simturn/SimApplyLeadership (4 differing bytes, 2,500 object bytes) and explo/ExploPosition (6, 3,920) are better fresh assignments than repeating known invariant register swaps. Animbmp/AnimBitmapFrame (3, 1,685) and tfast/BuildFlags (7, 1,026) are close but have existing register/slot-invariance evidence. Do not quantify likely effort from differing-byte count alone.

## Reproduction and receipts

The unmodified tools default to shared build directories. For this review `build/rv2/run.py` redirected only scratch locations; the logical tool arguments below are shown exactly. Use the normal commands from the repository root. No publish or retire operation was run. `build/rv2/check_candidates.py` independently rebuilt the candidates, checked each donor ledger function's VA **and size**, checked old data coverage, and ran check_reports plus global order and gap checks. For projected placements it used only in-memory dry-run owner bindings and qstart's implied external. The five renames take all old functions from their respective donors. Their “new” counts denote transfers.

```text
python tools/promote.py candidates/rv2/shell/hercbuy.c --tu src/shell/hercbuy.c --as 'shell\hercbuy.c' --profile plain-oe --accept-loss data:_DATA
src/shell/hercbuy.c: 35 strict functions (6858 bytes), new 0, lost 1 (accepted: data:_DATA)
```

```text
python tools/promote.py candidates/rv2/shell/qstart.c --tu src/shell/qstart.c --as 'shell\qstart.c' --profile plain-oe
src/shell/qstart.c: 13 strict functions (3127 bytes), new 0, lost 0
```

```text
python tools/promote.py candidates/rv2/misc/strpool.c --tu src/misc/strpool.c --as 'misc\strpool.c' --profile ot
src/misc/strpool.c: 7 strict functions (682 bytes), new 0, lost 0
```

```text
python tools/promote.py candidates/rv2/_unknown/tu_00476570.c --tu src/_unknown/tu_00476570.c --as '_unknown\tu_00476570.c' --profile plain-oe
src/_unknown/tu_00476570.c: 8 strict functions (697 bytes), new 0, lost 0
```

```text
python tools/promote.py candidates/rv2/core/mcpbrfs.c --tu src/core/mcpbrfs.c --as 'core\mcpbrfs.c' --profile plain --from src/_unknown/tu_0043e3ac.c --at mcpbrfsSourceFile=00494234
src/core/mcpbrfs.c: 2 strict functions (2378 bytes), new 2, lost 0
takes 2 functions from src/_unknown/tu_0043e3ac.c (file is retired)
```

```text
python tools/promote.py candidates/rv2/core/mcpio.c --tu src/core/mcpio.c --as 'core\mcpio.c' --profile plain --from src/_unknown/tu_0043ecf8.c --at mcpioSourceFile=00494244
src/core/mcpio.c: 4 strict functions (569 bytes), new 4, lost 0
takes 4 functions from src/_unknown/tu_0043ecf8.c (file is retired)
```

```text
python tools/promote.py candidates/rv2/core/mcpcamp.c --tu src/core/mcpcamp.c --as 'core\mcpcamp.c' --profile plain-oe --from src/_unknown/tu_0043ef34.c --at mcpcampSourceFile=00494254
src/core/mcpcamp.c: 13 strict functions (1413 bytes), new 13, lost 0
takes 13 functions from src/_unknown/tu_0043ef34.c (file is retired)
```

```text
python tools/promote.py candidates/rv2/core/mcphmisc.c --tu src/core/mcphmisc.c --as 'core\mcphmisc.c' --profile plain-oe --from src/_unknown/tu_004413e0.c --at mcphmiscSourceFile=00494270
src/core/mcphmisc.c: 12 strict functions (2125 bytes), new 12, lost 0
takes 12 functions from src/_unknown/tu_004413e0.c (file is retired)
```

```text
python tools/promote.py candidates/rv2/misc/format.c --tu src/misc/format.c --as 'misc\format.c' --profile ot --from src/_unknown/tu_0047d400.c --at formatSourceFile=004a4a24
src/misc/format.c: 2 strict functions (114 bytes), new 2, lost 0
takes 2 functions from src/_unknown/tu_0047d400.c (file is retired)
```

```text
python tools/promote.py candidates/rv2/multi/chat.c --tu src/multi/chat.c --as 'multi\chat.c' --profile plain
src/multi/chat.c: 15 strict functions (1769 bytes), new 0, lost 0
```

```text
python tools/promote.py candidates/rv2/_unknown/tu_00429ea4.c --tu src/_unknown/tu_00429ea4.c --as '_unknown\tu_00429ea4.c' --profile plain
src/_unknown/tu_00429ea4.c: 2 strict functions (571 bytes), new 0, lost 0
```

```text
python tools/promote.py candidates/rv2/logic/lactor.c --tu src/logic/lactor.c --as 'logic\lactor.c' --profile plain
src/logic/lactor.c: 14 strict functions (736 bytes), new 2, lost 0
```

The palette candidate intentionally exposes the 32-byte open gap. With all twelve candidates replayed together the projected gap set contains both tu_00429ea4.c and palfade.c. The four new object closures pass in either replay. Apply the palette change as its own evidence correction.

Other diagnostic trap: symbols.scope_of_module extracts a TU scope only when the OMF THEADR contains /build/cc/. An otherwise valid CSTORM_SCRATCH under build/rv2/cc falls back to basename, loses scoped bindings and gave 70 instead of 75 complete in a control. build/rv2/build/cc restores the documented scope and verify totals. Parse the source portion relative to the configured scratch root or explicitly pass compile-as scope.

Raw evidence in build/rv2 includes verify.txt, rows.json, reports.json, parked.tsv, blockers.txt, waits.txt, symaudit.txt, census.txt, secondary.txt, scans.txt, candidates.txt, linkprobe.txt, all.lnk/log/map and libs.lnk/log/map. The hybrid reproducer under candidates/rv2 is independent of this scratch directory. tools/objcensus.py -v, tucheck.py -v, blockers.py -v and linkprobe.py -v can regenerate the underlying observations. For repeatable image checks prefer the map and bytes over the prose in older lane reports.

## Appendix — all 110 incomplete TUs

Baseline from the same fresh OMF reports used by tucheck. Main-code counts include only emitted failed extents. Candidate code size is the size of the current main segment. A dash means that check reported no additional failure; early return on non-STRICT code limits which geometry checks ran. Exact extents and relocation lists are in reports.json.

| TU (under src/) | Main bytes | Failed main extents | Other reported blockers |
|---|---:|---:|---|
| _unknown/tu_0042726c.c | 846 | 0 | bytes 004275ba..004275dc after the object are not zero fill (missing function?) |
| _unknown/tu_004275bc.c | 1,079 | 1/1 | — |
| _unknown/tu_0042a5fc.c | 205 | 0 | bytes 0042a6c9..0042a6e8 after the object are not zero fill (missing function?) |
| _unknown/tu_0042cf40.c | 1,172 | 3/7 | — |
| _unknown/tu_0042d3d0.c | 3,621 | 3/4 | — |
| _unknown/tu_004324d0.c | 1,107 | 1/4 | — |
| _unknown/tu_004349c0.c | 691 | 2/6 | — |
| _unknown/tu_00435130.c | 1,928 | 3/7 | — |
| _unknown/tu_00435dec.c | 8,782 | 11/47 | — |
| _unknown/tu_00438374.c | 4,435 | 3/27 | data CONST: ITEMS |
| _unknown/tu_004394e0.c | 3,506 | 1/20 | data CONST: ITEMS |
| _unknown/tu_0043a294.c | 3,468 | 4/20 | — |
| _unknown/tu_0043ef34.c | 9,366 | 4/17 | data CONST: ITEMS |
| _unknown/tu_004413e0.c | 3,850 | 2/14 | data CONST: ITEMS |
| _unknown/tu_0045ad00.c | 2,760 | 4/11 | — |
| _unknown/tu_00474fb8.c | 1,716 | 1/11 | data CONST: ITEMS |
| _unknown/tu_00476570.c | 697 | 0 | data CONST: ITEMS |
| _unknown/tu_00476ddc.c | 754 | 1/4 | data CONST: ITEMS |
| _unknown/tu_004774b8.c | 1,808 | 2/15 | — |
| _unknown/tu_0047d0e0.c | 777 | 2/9 | — |
| _unknown/tu_0047d9d0.c | 939 | 1/10 | — |
| _unknown/tu_0047e150.c | 551 | 1/6 | — |
| _unknown/tu_00481360.c | 876 | 1/5 | — |
| core/mcp.c | 10,942 | 11/78 | data CONST: ITEMS |
| graph/blitWIN.c | 1,201 | 2/11 | — |
| graph/gfx/g_cds.c | 1,646 | 1/6 | data CONST: ITEMS |
| graph/vs.c | 2,931 | 2/26 | data _DATA: UNPLACED |
| gui/alert.c | 1,866 | 1/12 | — |
| gui/flic.c | 3,094 | 4/18 | — |
| gui/menu.c | 6,394 | 3/61 | — |
| io/file.c | 1,705 | 1/18 | data _DATA: ITEMS |
| io/resource.c | 2,992 | 4/21 | data CONST: DIFF / data _DATA: ITEMS |
| logic/bestloc.c | 1,671 | 3/7 | data CONST: UNPLACED |
| logic/intel.c | 5,550 | 1/15 | data CONST: ITEMS |
| logic/lactor.c | 1,192 | 4/16 | — |
| logic/lactors.c | 3,351 | 1/21 | — |
| logic/lfire.c | 1,866 | 1/6 | — |
| logic/lgen.c | 3,989 | 3/11 | data CONST: ITEMS / data _DATA: ITEMS / BSS _BSS: BSS-UNPLACED |
| logic/logic.c | 3,551 | 2/16 | data CONST: DIFF |
| logic/miss.c | 2,145 | 1/13 | data CONST: ITEMS |
| logic/sinterp.c | 9,004 | 4/25 | data CONST: ITEMS |
| logic/slog.c | 0 | empty | code unplaced |
| main/WIN/winmain.c | 11,617 | 6/23 | data CONST: ITEMS |
| misc/strpool.c | 682 | 0 | data CONST: ITEMS |
| multi/link.c | 1,611 | 1/12 | — |
| multi/linkapp.c | 1,132 | 1/11 | data CONST: ITEMS |
| multi/linkg.c | 1,492 | 1/18 | — |
| multi/message.c | 5,585 | 2/32 | — |
| multi/minipkt.c | 9,689 | 2/36 | — |
| multi/mstart.c | 6,660 | 3/41 | data CONST: ITEMS |
| noise/noise.c | 5,281 | 2/22 | — |
| pop/pherc.c | 5,544 | 7/21 | data CONST: ITEMS |
| pop/pinfo.c | 1,700 | 1/10 | data CONST: ITEMS |
| pop/pmove.c | 2,859 | 1/10 | — |
| pop/ppilot.c | 6,450 | 3/16 | data CONST: ITEMS |
| pop/preport.c | 2,997 | 2/13 | — |
| pop/pshield.c | 3,211 | 3/17 | data CONST: ITEMS |
| pop/psystem.c | 1,490 | 1/10 | — |
| pop/pweapon.c | 3,449 | 2/16 | — |
| pop/pworld.c | 844 | 1/9 | — |
| shell/commscrn.c | 11,133 | 2/32 | — |
| shell/hbmanage.c | 3,963 | 2/23 | data CONST: ITEMS |
| shell/hbupgrad.c | 14,534 | 3/59 | data CONST: ITEMS |
| shell/hercbuy.c | 6,845 | 0 | data CONST: ITEMS / extent W?$dt:HercBuyCtor$n()_ not contiguous / bytes 00417f01..00417f14 after the object are not zero fill (missing function?) / original starts inside the object without an extent: ['00417ee4', '00417efc'] |
| shell/hotseat.c | 5,517 | 3/19 | — |
| shell/mphbase.c | 7,825 | 5/25 | — |
| shell/mplaunch.c | 19,511 | 2/47 | data CONST: ITEMS / BSS _BSS: BSS-UNPLACED |
| shell/pilotgui.c | 7,032 | 1/25 | data CONST: ITEMS |
| shell/qstart.c | 3,115 | 0 | data CONST: ITEMS |
| shell/shell.c | 24,838 | 13/46 | data CONST: ITEMS |
| sim/actor.c | 2,770 | 2/15 | — |
| sim/actoradd.c | 742 | 1/4 | data CONST: ITEMS |
| sim/actorlst.c | 2,736 | 2/14 | data CONST: ITEMS |
| sim/actorver.c | 738 | 1/3 | — |
| sim/anim.c | 7,025 | 5/29 | — |
| sim/animbmp.c | 1,685 | 1/4 | — |
| sim/animout.c | 2,688 | 1/6 | BSS _BSS: BSS-UNPLACED |
| sim/apply.c | 4,010 | 2/5 | data _DATA: ITEMS |
| sim/explo.c | 3,920 | 1/23 | — |
| sim/exptbl.c | 1,097 | 2/3 | — |
| sim/fxout.c | 7,344 | 5/11 | — |
| sim/hotbrief.c | 2,156 | 2/9 | data CONST: ITEMS / data CONST2: UNPLACED / data _DATA: ITEMS |
| sim/opfire.c | 1,841 | 2/4 | — |
| sim/proj.c | 1,145 | 2/5 | — |
| sim/sim.c | 6,416 | 1/9 | — |
| sim/simactor.c | 3,422 | 3/9 | BSS _BSS: BSS-UNPLACED |
| sim/simbug.c | 0 | empty | code unplaced |
| sim/simdo.c | 10,674 | 8/38 | data CONST: ITEMS |
| sim/simfire.c | 2,483 | 1/2 | — |
| sim/simglob.c | 1,043 | 1/6 | — |
| sim/simhot.c | 4,320 | 3/14 | data CONST: ITEMS |
| sim/simmain.c | 5,185 | 2/5 | BSS _BSS: BSS-UNPLACED |
| sim/simmini.c | 2,105 | 2/6 | — |
| sim/simmisc.c | 1,437 | 1/9 | — |
| sim/simmulti.c | 1,918 | 2/13 | — |
| sim/simout.c | 4,107 | 3/7 | data CONST: ITEMS / BSS _BSS: BSS-UNPLACED |
| sim/simturn.c | 2,500 | 1/13 | — |
| sim/target.c | 996 | 1/5 | BSS _BSS: BSS-UNPLACED |
| sim/weapai.c | 3,567 | 3/14 | data CONST: ITEMS |
| sim/weapfx.c | 465 | 1/2 | — |
| sim/worldact.c | 1,022 | 1/9 | data CONST: ITEMS |
| terr/tarea.c | 923 | 2/5 | — |
| terr/tbuild.c | 4,990 | 5/15 | data CONST: ITEMS |
| terr/tdisplay.c | 4,656 | 4/24 | — |
| terr/tfast.c | 1,026 | 1/10 | — |
| terr/tfog.c | 1,778 | 2/14 | — |
| terr/tgrid.c | 3,138 | 4/20 | — |
| terr/tlos.c | 2,199 | 3/5 | — |
| terr/trender.c | 2,704 | 3/17 | — |
| terr/tsurface.c | 10,387 | 5/21 | data CONST: ITEMS |
