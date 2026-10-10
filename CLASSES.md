# func — Objects & Classes to Build First

> **Forward class inventory.** The initial procedural literal compiler now lives
> in `src/func.c` and the build-only `func` launcher. It implements numeric/surface
> add/mul/print, flat private rows and native C emission/compilation, not the public
> classes listed below. The remaining inventory states future work; cite every
> law by Title and do not mistake a class row for an implemented API.

## The core idea — one line

Every function has its own **numeric id**. A program is a **flat sequence of
numbers**. Reading it left to right: an id names an opcode; the id's **arity**
says how many following values are its parameters; the opcode runs on them.

```text
1, 34, 7          # 1 = add(), arity 2   ->   add(34, 7)   ->   41
```

That sequence **compiles to native code** — `add(34, 7);` — never an interpreted
runtime. IDs can be used anywhere a value fits, including math:

```text
1, 34, 7          # 34 + 7   = 41
2, 41, 3          # 41 * 3   = 123      (2 = mul())
```

Control flow is the same mechanism with a body sub-sequence:

```text
for(()())          # the id `for` runs its body sequence a number of times
while(()())        # the id `while` re-runs its body while a condition holds
```

Two front-ends produce the **same** program: the raw numeric stream
(`1, 34, 7`) and the surface form (`add(34, 7)` / `()()` markers). Both lower to
one `FuncProgram`.

Two structural choices follow from the laws:

- **Flat, index-based storage** for the program and the opcode table (the
  Data-Oriented Storage Law): arrays of values and nodes, no pointer-chasing.
- **One class per file** (Single Class Per File Law); a program's node rows are
  its **slot record**, not separate behavioral classes.

---

## Phase 0 — Skeleton

| Class | Responsibility | Key state / ops | Law notes |
|:---|:---|:---|:---|
| `FuncDiagnostic` | A decode/compile/run error with location + message | `line`, `col`, `message`; `_toString` | THROW Law (cold), toString Law |
| `FuncHome` | External build state (like `B_HOME`) | root path; `FuncHome_root()`, `FuncHome_build()` | Standalone Autonomy Law |
| `FuncCli` | Command dispatch (`help`, `opcodes`, `run`, `build`) | parse argv → dispatch | Build & Naming Conventions Law |

## Phase 1 — The Opcode Center (ids)

| Class | Responsibility | Key state / ops | Law notes |
|:---|:---|:---|:---|
| `Opcode` | One numbered operation: its **id**, name, **arity**, native entry, flags | `id`, `name`, `arity`, `entry` | Single Class Per File Law; WHAT Law for `entry` |
| `OpcodeRegistry` | The id-indexed opcode table; `get(id)`, `find(name)`, `add` | flat `Opcode` rows indexed by id | Data-Oriented Storage Law; No Hardcoding Law (grows) |
| `Value` | A decoded constant operand (int/float/string) or a result reference | `kind`, `asInt`, `asFloat`, `asText` | Value Boundary Matrix Law (boundaries live here) |

**First because** the id table is the whole language: the stream is meaningless
without `id -> opcode + arity`.

## Phase 2 — The Stream & Program (flat)

| Class | Responsibility | Key state / ops | Law notes |
|:---|:---|:---|:---|
| `FuncStream` | The raw flat value sequence (`1, 34, 7`) plus a cursor | `values`, `count`, `cursor` | Data-Oriented Storage Law |
| `FuncReader` | Decode the stream using the registry's arities: id + operands → nodes | `read(stream, registry) -> program` | Cold-Strict, Hot-Minimal Validation Law (reject unknown id / short operands) |
| `FuncProgram` | A compiled unit: flat node array + opcode refs + entry | `nodes`, `count`, `entry` | Data-Oriented Storage Law |
| `FuncNode` | **SLOT RECORD** row: `kind` (`CALL`/`FOR`/`WHILE`), opcode index, operand range, body range | behaviorless | slot record under `FuncProgram`; zero `Class_*` |
| `FuncSource` | A surface source file plus its marker index | `path`, `text`, `markers` | validate once (Cold-Strict) |
| `FuncMarker` | A `()()` / `for(()())` surface occurrence | `name`, `params`, `offset` | Authorial Intent Law (the spelling is deliberate) |

## Phase 3 — Front-end (both forms → one program)

| Class | Responsibility | Key state / ops | Law notes |
|:---|:---|:---|:---|
| `FuncLexer` | Tokenize either the numeric stream or the surface form | cursor + current token | Single Class Per File Law |
| `FuncParser` | Parse a surface script (`add(34,7)`, `for(()())`, `while(()())`) into a stream/program | `parse(source) -> program` | Determinism and Reproducibility Law |

## Phase 4 — Backend (compile, never interpret)

| Class | Responsibility | Key state / ops | Law notes |
|:---|:---|:---|:---|
| `FuncCompiler` | Resolve ids → opcodes and arities, bind operands, build the final `FuncProgram` | `compile(program) -> program` | Compile-time only; no interpreter |
| `FuncEmitter` | Emit native C for the program (e.g. `add(34, 7);`) with control flow | `emit(program) -> source` | Build & Naming Conventions Law (emits C23) |
| `FuncBuilder` | Drive the C compiler to build the emitted source | argv assembly; `build(artifact)` | Bounded Wait Law; Standalone Autonomy Law |
| `FuncArtifact` | The produced native binary/module + metadata | `path`, `kind`, `symbols` | toString Law |

## Phase 5 — Runtime (linked into compiled scripts)

| Class | Responsibility | Key state / ops | Law notes |
|:---|:---|:---|:---|
| `FuncRuntime` | The opcode dispatch table + control-flow helpers a compiled script links | dispatch table (opaque) | Cold-Strict, Hot-Minimal Validation Law |
| `FuncContext` | Per-run state: arguments, environment, IO handles | `args`, `env` | WHAT Law for opaque handles |

## Phase 6 — Project & Tools

| Class | Responsibility | Key state / ops | Law notes |
|:---|:---|:---|:---|
| `FuncProject` | A workspace of scripts + opcode sources | `root`, `scripts` | No Hardcoding Law |
| `Tool` | An external callable (command + args + capture policy) | `argv`, `capture` | Bounded Wait Law |
| `ToolRun` | One bounded child run (pid, status, output) | `pid`, `exitCode`, `done` | Bounded Wait Law (100 ms reap, SIGTERM) |

---

## First-build order

1. **`Opcode` → `OpcodeRegistry` → `Value`** — the id table (id + arity).
2. **`FuncStream` → `FuncReader` → `FuncProgram` (+ `FuncNode` slot record).**
3. **`FuncLexer` → `FuncParser`** — surface `add(34,7)`, `for(()())`, `while(()())`.
4. **`FuncCompiler` → `FuncEmitter` → `FuncBuilder` → `FuncArtifact`.**
5. **`FuncRuntime` → `FuncContext`** — run a compiled script.
6. **`FuncProject`, `Tool`/`ToolRun`, `FuncHome`, `FuncCli`** — CLI and projects.

**First milestone:** register opcode `1 = add` (arity 2), feed the stream
`1, 34, 7`, compile it with `FuncBuilder`, run the native artifact, and assert
the result is `41`.

## Naming and collisions

- Prefix program/compiler classes with `Func*`.
- `Opcode`, `Value`, `Param` stay short — they are `func`'s own vocabulary.
- No `FuncVM`: a compiled design has no interpreter.

## Deliberately not classes

- **No VM / interpreter** — the id stream compiles to native code.
- **No package manager / compiler** — `FuncBuilder` delegates to the installed C
  compiler, like `b`.
- **No ecosystem headers** — `func` depends on nothing; merging is by invocation,
  not inclusion (Standalone Autonomy Law).
