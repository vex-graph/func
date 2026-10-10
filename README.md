# func — a personal, compile-time opcode orchestrator

Future builds belong to [b](https://github.com/vex-graph/b). No runnable target
exists yet.

## Current State

**Draft — not finalized.** Documentation only; the scope below may change.

**Role:** a **personal**, standalone C23 tool — a compile-time **opcode
dispatcher** and compiled mini-language. Like `b`, it depends on nothing in the
ecosystem; it is an orchestrator you point at anything.

**Implemented and proven:** nothing. This is a **documentation-only blueprint**:
`README.md`, `CLASSES.md`, `CONTRIBUTING.md`, `func-preferences.md`, `LICENSE`
and `.gitignore`. No `src/`, header or build target.

**Specified only:** the object/class inventory in `CLASSES.md` (the first things
to build) and the forward contracts in [func-preferences.md](func-preferences.md).

**Platforms proven:** none.

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
a host. Control flow uses the same mechanism with a body (`for(()())`,
`while(()())`). `func` can merge ecosystem pieces (`darling`, `sesh`, …) into one
artifact without depending on them.

## Shape (like b)
- `func` launcher · `func.c` dispatch · `func.h` contracts · `opcodes/` registry
- `annotation.h` zero-runtime markers
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

**Known limits and gaps:** zero implementation — the object inventory in
`CLASSES.md` is the design, not shipped behavior; no platform is proven and no
`tests/func/` partition exists.
