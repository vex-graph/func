# func — Repo-Local Living Preferences

## Constitution Link

The complete [workspace constitution](https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a)
governs this repository. Local path: `../../preferences.md`. Read it first, then
this file, `../../tests/test-preferences.md`, the current checklist and actual
implementation. These are forward contracts: func currently has no production
source or executable owner tests. `CLASSES.md` is an inventory, not an API.

## Law Index (Binding Matrix)

| Law Title | Scope | Enforcement |
| :--- | :--- | :--- |
| Standalone Opcode Tool Law | Personal tooling dependency boundary | Future standalone build and source-owner proof |
| Native Opcode Compilation Law | Numeric/surface input and generated artifacts | Future arity, malformed-input, compilation and artifact owners |
| Bounded Tool Invocation Law | Compiler and external child execution | Future cancellation, output, failure and cleanup owners |

### Standalone Opcode Tool Law

func is a personal standalone tool, not an R1–R5 library. It borrows no ecosystem
headers or runtime implementation. Harness and other consumers invoke it as a
tool; invoking func does not permit them to include its internals. Orchestrating
ecosystem artifacts by explicit invocation does not transfer their ownership.
The launcher/dispatch/contracts/opcodes layout in CLASSES.md is planned only.
IDE CMake remains a source-free metadata entry, not build/runtime proof.

### Native Opcode Compilation Law

Each registered operation has a numeric opcode ID and declared arity. The raw
numeric stream and surface syntax lower to one flat FuncProgram. IDs and operand
ranges are validated cold before emitting native C23 and delegating compilation;
unknown IDs, missing/extra operands, invalid references and arithmetic/range
overflow reject with diagnostics and preserved prior valid output. No interpreted
VM ships. Future FuncRuntime helpers support compiled code, not stream interpretation.

Opcode IDs are language operation identities, not ecosystem object type IDs.
The registry is owner-defined and growable; examples such as add=1 are illustrative,
not an implemented ABI assignment. Cold compilation may grow storage; runtime
performance or allocation guarantees need their own implemented contract and proof.

### Bounded Tool Invocation Law

Compiler/build/run operations use literal argument vectors and bounded child
supervision, with cancellation and explicit exit status/output truncation. Never
use system() or concatenate untrusted text into a shell command. A bounded reap
slice is not proof of bounded whole-job completion: deadlines, cancellation and
cleanup must be stated and tested. Build is not implicit run, upload, installation
or publication permission. Generated outputs stay outside production source.

## Proof and Current Gaps

Future owners live in the independent `../../tests/func/` tree, mirroring production
units and registered with an executable runner. Prove both input forms, every
advertised arity, intended compile rejection, artifact execution/results, compiler
failure preservation/retry and bounded child lifecycle. Resource-owning classes
also obey the Deliberate Exhaustion and Backend Trust Law: pressure, growth,
exhaustion/recovery and legal occupancy proof precede readiness claims.

There is no implementation, platform proof, compiler integration or Harness
integration yet. Documentation checks alone confer no runtime readiness.
