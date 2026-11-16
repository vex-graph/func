# func — Repo-Local Living Preferences

## Constitution Link

gist.md (`../../../metadata/gists/preferences/gist.md`) is the compressed law list; all universal laws bind this repository. Read one on demand: `./tools/prefs law "<title>"`. Universal laws are inherited, never restated.

Tier: tool. Owns: personal standalone opcode tool. Borrows: no ecosystem headers or runtime.

## Exclusive Laws

| Law Title | Scope | Enforcement |
| **Standalone Opcode Tool Law** | personal tooling dependency boundary | Mandatory for `func` |
| **Native Opcode Compilation Law** | numeric/surface input and generated artifacts | Mandatory for `func` |
| **Bounded Tool Invocation Law** | compiler and external child execution | Mandatory for `func` |

### Standalone Opcode Tool Law

* `func` is a personal standalone tool, not a `lifetime` / `core` / `app` library; it borrows no ecosystem headers or runtime.
* Harness and others invoke it, never include internals.
* Explicit artifact orchestration transfers no ownership.

### Native Opcode Compilation Law

* Each operation has a numeric opcode ID and arity.
* Raw streams and surface syntax lower to one flat representation using private behaviorless rows, not a public `FuncProgram` API.
* Cold validation precedes native C23 emission/compilation: unknown IDs, missing/extra operands, invalid references and arithmetic/range overflow reject diagnostically, preserving prior valid output.
* No interpreted VM ships.

### Bounded Tool Invocation Law

* Compiler/build/run use literal argv and bounded child supervision, cancellation, explicit exit status and output truncation.
* Never use `system()` or concatenate untrusted text into shell commands.
* Reap slices bound jobs; state and test deadlines, cancellation and cleanup.
* Build grants no implicit run, upload, installation or publication.
* Outputs stay outside production source.

## Repo-Local Extensions

* None — no managed exceptions to universal laws.

## Readiness Cross-Reference

* Feature readiness: func readiness gist.
* Open blockers: ecosystem blockers gist.
* Proof lawbook: `tests/test-preferences.md` — `./tools/prefs test "<title>"`.
* Evidence ledger: `metadata/gists/test-checklist.md` — `python3 tools/test_checklist.py check`.
