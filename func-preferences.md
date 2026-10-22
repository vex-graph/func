# func — Repo-Local Living Preferences

## Constitution Link

The complete [workspace constitution](https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a) governs this repository. Local path: `../../gist/PREFERENCES.md`. Read it first, then this file, `../../tests/test-preferences.md`, current `../../gist/test-checklist.md`, and implementation. The initial literal compiler is `src/func.c` plus Python build-only launcher `func`; `CLASSES.md` inventories planned general classes, not APIs.

## Law Index (Binding Matrix)

| Law Title | Scope | Enforcement |
| :--- | :--- | :--- |
| Standalone Opcode Tool Law | Personal tooling dependency boundary | Standalone native owner build; general orchestration remains future |
| Native Opcode Compilation Law | Numeric/surface input and generated artifacts | Registered arity, malformed-input, compilation and artifact owners |
| Bounded Tool Invocation Law | Compiler and external child execution | Registered deadline/output/failure/cleanup owners; no hostile-process sandbox claim |

### Standalone Opcode Tool Law

func is a personal standalone tool, not an R1–R6 library; it borrows no ecosystem headers/runtime. Harness and others invoke it, never include internals; explicit artifact orchestration transfers no ownership. Its full launcher/dispatch/contracts/opcodes class layout in CLASSES.md is planned; the procedural emitter/launcher is standalone. Editor indexing belongs to the workspace.

### Native Opcode Compilation Law

Each operation has a numeric opcode ID and arity. Raw streams and surface syntax lower to one flat representation using private behaviorless rows, not a public FuncProgram API. Cold validation precedes native C23 emission/compilation: unknown IDs, missing/extra operands, invalid references and arithmetic/range overflow reject diagnostically, preserving prior valid output. No interpreted VM ships; future FuncRuntime helpers support compiled code, not stream interpretation.

IDs identify language operations, not ecosystem types. The future owner-defined registry grows. Implemented: add=1/arity2, mul=2/arity2, print=3/arity1; operands are signed decimal int64 literals only. Emit checked native arithmetic and print results in order. References, control flow and registration are absent. Cold rows grow; runtime performance/allocation guarantees require an implemented contract and proof.

### Bounded Tool Invocation Law

Compiler/build/run use literal argv and bounded child supervision, cancellation, explicit exit status and output truncation. Never use system() or concatenate untrusted text into shell commands. Reap slices do not bound whole jobs: state and test deadlines, cancellation and cleanup. Build grants no implicit run, upload, installation or publication. Outputs stay outside production source.

## Proof and Current Gaps

Independent `../../tests/func/` owners mirror production units and run through an executable runner. Prove both input forms, all advertised arities, intended compile rejection, artifact execution/results, compiler-failure preservation/retry and bounded child lifecycle. Resource-owning classes need pressure, growth, exhaustion/recovery and legal occupancy proof under the Deliberate Exhaustion and Backend Trust Law before readiness claims.

`python3 ../../tests/func/run.py` exercises literal decoding/native compilation, artifact execution, hostile rejection/growth and C ASan/UBSan. Builds are explicit (never run), deadline-bounded with capped stage output. Trusted stable output parent/compiler are preconditions, not sandbox claims. Python supervision is cold host work, not a shipped interpreted language. Gaps: OOM/leak instrumentation, general orchestration, platforms beyond the lab host and full Harness integration. Documentation checks confer no runtime readiness.
