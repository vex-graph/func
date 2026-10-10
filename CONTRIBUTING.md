# Contributing to func

The [canonical workspace constitution](https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a)
is supreme. Read it before [func-preferences.md](func-preferences.md), the shared
test laws/checklist, and the owning implementation. Workspace paths are
`../../preferences.md` and `../../tests/`; standalone readers use the canonical
Gist and report unavailable dependency/test checkouts as reading gaps.

func is currently a source-free personal tooling blueprint. [CLASSES.md](CLASSES.md)
describes intended classes, not a shipped API. [README.md](README.md) records
current competency and limitations. Builds belong to b/native compiler tooling;
implementation readiness requires the owner's executable proof.

Keep owner tests separate under the shared `tests/func/` partition when source
lands. Prove actual behavior and hostile/failure paths before readiness claims;
resource owners require deliberate exhaustion/recovery and legal concurrent
occupancy evidence. Commit locally in this repository under the Git Workflow Law.
Never push without an explicit one-off instruction.
