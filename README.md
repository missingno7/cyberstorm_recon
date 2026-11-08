# MissionForce: CyberStorm reconstruction

Readable C++ source that Watcom C/C++ 10.6 compiles back to the exact bytes of `CSTORM.EXE` 1.10A (Dynamix,
1996). Bring your own copy of the game in `assets/` (see `config/oracles.json` for the expected hashes);
originals and toolchains are not distributed.

Start with [AGENTS.md](AGENTS.md), then [docs/evidence.md](docs/evidence.md) (reconnaissance),
[docs/toolchain.md](docs/toolchain.md), [docs/verification.md](docs/verification.md),
[docs/codegen_rules.md](docs/codegen_rules.md) and [docs/status.md](docs/status.md).

Requirements: Windows, Python 3.11 with `capstone`, Watcom C/C++ 10.6 under `C:\tools\watcom-10.6`.
