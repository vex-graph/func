# func — Repo-Local Living Preferences

## Constitution Link

The complete [workspace constitution](https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a)
governs this repository. Local path: `../../preferences.md`. Read it first, then
this file, `../../tests/test-preferences.md`, the current checklist and actual
implementation. The initial literal compiler is implemented in `src/func.c` plus
the Python build-only launcher `func`; general classes in `CLASSES.md` remain an
inventory, not an API.

## Law Index (Binding Matrix)

| Law Title | Scope | Enforcement |
| :--- | :--- | :--- |
| Standalone Opcode Tool Law | Personal tooling dependency boundary | Standalone native owner build; general orchestration remains future |
| Native Opcode Compilation Law | Numeric/surface input and generated artifacts | Registered arity, malformed-input, compilation and artifact owners |
| Bounded Tool Invocation Law | Compiler and external child execution | Registered deadline/output/failure/cleanup owners; no hostile-process sandbox claim |

### Standalone Opcode Tool Law

func is a personal standalone tool, not an R1–R5 library. It borrows no ecosystem
headers or runtime implementation. Harness and other consumers invoke it as a
tool; invoking func does not permit them to include its internals. Orchestrating
ecosystem artifacts by explicit invocation does not transfer their ownership.
The full launcher/dispatch/contracts/opcodes class layout in CLASSES.md remains
planned. The current procedural emitter/launcher is standalone and borrows no
ecosystem headers. Editor indexing belongs to the workspace.

### Native Opcode Compilation Law

Each registered operation has a numeric opcode ID and declared arity. The raw
numeric stream and surface syntax lower to one flat program representation. The
first slice uses private behaviorless rows rather than claiming a public
FuncProgram API. IDs and operand
ranges are validated cold before emitting native C23 and delegating compilation;
unknown IDs, missing/extra operands, invalid references and arithmetic/range
overflow reject with diagnostics and preserved prior valid output. No interpreted
VM ships. Future FuncRuntime helpers support compiled code, not stream interpretation.

Opcode IDs are language operation identities, not ecosystem object type IDs.
The future registry is owner-defined and growable. The first implemented IDs are
add=1/arity2, mul=2/arity2 and print=3/arity1, with signed decimal int64 literal
operands only. They emit checked native arithmetic and print results in order;
references/control flow/registration remain absent. Cold row storage grows;
runtime
performance or allocation guarantees need their own implemented contract and proof.

### Bounded Tool Invocation Law

Compiler/build/run operations use literal argument vectors and bounded child
supervision, with cancellation and explicit exit status/output truncation. Never
use system() or concatenate untrusted text into a shell command. A bounded reap
slice is not proof of bounded whole-job completion: deadlines, cancellation and
cleanup must be stated and tested. Build is not implicit run, upload, installation
or publication permission. Generated outputs stay outside production source.

## Proof and Current Gaps

Registered owners live in the independent `../../tests/func/` tree, mirroring production
units and registered with an executable runner. Prove both input forms, every
advertised arity, intended compile rejection, artifact execution/results, compiler
failure preservation/retry and bounded child lifecycle. Resource-owning classes
also obey the Deliberate Exhaustion and Backend Trust Law: pressure, growth,
exhaustion/recovery and legal occupancy proof precede readiness claims.

`python3 ../../tests/func/run.py` exercises literal decoding/native compilation,
artifact execution, hostile rejection/growth and C ASan/UBSan. Every build is
explicit and never runs the result; each stage has an overall deadline and capped
output. Stable trusted output parent and compiler are preconditions, not sandbox
claims. Python supervision is cold host work, not a shipped interpreted language.
OOM/leak fault instrumentation, general orchestration, runtime platforms beyond
the lab host and full Harness integration remain gaps. Documentation checks alone
confer no runtime readiness.
