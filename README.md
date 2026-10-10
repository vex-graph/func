# func — a personal, compile-time opcode orchestrator

Build explicitly with `./func build SOURCE --syntax surface -o ARTIFACT`.
The launcher delegates native compilation to installed Clang; it never runs the
artifact. Requires Python 3.11+ and a C23-capable Clang; no dependency downloads.

## Current State

**Initial implemented slice — not production-ready.** The general orchestration
design is still unfinished.

**Role:** a **personal**, standalone C23 tool — a compile-time **opcode
dispatcher** and compiled mini-language. Like `b`, it depends on nothing in the
ecosystem; it is an orchestrator you point at anything.

**Implemented:** standalone C23 source decoder (`src/func.c`) and a Python
build-only launcher (`func`). Numeric and surface literal calls share a flat row
representation, emit C23 and compile a real native artifact. IDs are `1=add`
(arity two), `2=mul` (arity two), `3=print` (arity one). Every operation prints
its result. Integer literals and native arithmetic are checked for overflow.
Rejected source or compiler failure preserves the previous artifact.

**Specified only:** the full object/class inventory in `CLASSES.md`, extensible
opcode registry, references, control flow and general tool orchestration.

**Proof scope:** registered offline owners under `tests/func` exercise real
native artifacts, both spellings, malformed/range/arity boundaries, growth,
failure preservation/retry and C decoder ASan/UBSan. See the current shared
checklist for executed evidence; no Linux/Windows/macOS 14 runtime or general
backend-trust claim is inferred.

## What it is
`func` is a personal, dependency-free orchestrator with a **compiled**, id-based
opcode model. Every function has its own **numeric id**, and a program is a flat
sequence of numbers. Reading left to right, an id names an opcode; the id's
arity says how many following values are its parameters.

```text
1, 34, 7      # 1 = add(), arity 2   ->   add(34, 7)   ->   41
```

That sequence compiles to native code (`add(34, 7);`) — never an interpreted
runtime — so an orchestration ships as a binary, not an interpreter dragged into
a host. Future control flow uses the same mechanism with a body (`for(()())`,
`while(()())`). Merging ecosystem pieces (`darling`, `sesh`, …) remains a future
goal, not a capability of the literal arithmetic slice.

## Shape (like b)
- `func` Python build-only launcher · `src/func.c` standalone native source emitter
- Flat private instruction rows; no public runtime classes or ecosystem headers
- README / CONTRIBUTING / CLASSES

## Laws that govern work here
- Constitution: the [canonical preferences.md Gist](https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a); one real, Git-ignored workspace-root `../../preferences.md`.
- Local lawbook: [func-preferences.md](func-preferences.md); actual workspace
  constitution/test paths are `../../preferences.md` and `../../tests/`.
- **Standalone Autonomy Law**: zero ecosystem dependency; it borrows no project header.
- **Platform Support Floor Law** (arm64 / macOS 14+), **Build & Naming Conventions Law** (`-Wall -Wextra -Werror`, C23), **Single Class Per File Law**, **toString Law**, **Test Segregation Law** (`tests/func`).
- Commits land in THIS repo root; never push unless asked.

## Scope and Limitations

**Scope (intended):** a personal compile-time opcode orchestrator — register
opcodes, compose them with `for`/`while`/params, compile to a native artifact,
and merge ecosystem pieces into one.

**Deliberately not covered:** no ecosystem dependency, no interpreted runtime,
no package manager, no new C compiler. External tools (when invoked) run as
bounded children (the Bounded Wait Law).

**Known limits and gaps:** only literal add/mul/print are implemented. No references,
variables, loops, arbitrary external operations, registration API or `run` command.
The input ceiling is 1 MiB and emitted C has a separate 64 MiB safety budget.
Compiler/decoder stages have caller-selected deadlines and capped pipe output;
trusted compilers/parents are not a sandbox against detached malicious processes.
OOM fault injection, full source-limit Cartesian coverage, leak instrumentation,
Windows and non-host baseline runtime proof remain gaps. `CLASSES.md` is a future
inventory, not a shipped API; no interpreted Func VM exists.
