# Blocked or parked functions

One line per function: VA, label, TU, what was tried (with flags), best hypothesis, date.

(none yet)

## Cleanup debt (integrator, 2026-11-09)
Published sources that call runtime internals through invented names (bindings kept so verify passes; rewrite
with the C++ construct, then drop the alias): `BriefModuleConstruct`/`HercModuleConstruct`/`PopObjectConstruct`
= `__wcpp_2_mod_register_` (static-init registration, compiler generated), `MemoryRelease` = `operator delete[]`,
`PopConstructorArray` = `__wcpp_2_ctor_array_`, `MemoryAllocationRetry` = `operator new[]`.
Affected TUs: src/shell/missbrf.c, src/shell/hercbay.c, src/pop/psystem.c, src/terr/tfog.c, src/terr/tfast.c.
