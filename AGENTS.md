# Agent Instructions for `tulipcc`

## Submodule Setup

To initialize and update submodules for this repo, use:

- `tulip/shared/grab_submodules.sh`

Do not use ad-hoc/manual recursive submodule commands when this script is available.

## Micropython Submodule Status

The `micropython` submodule is expected to appear dirty after bootstrap/build steps (for example, local warning-flag patching done by `tulip/shared/grab_submodules.sh`).

Treat this as expected local state and ignore `micropython` dirtiness in `git status` unless you are intentionally working on `micropython`.

## AMY Submodule Policy

We constantly update the `amy` submodule to work with `tulipcc`.

Before pushing anything in `tulipcc` to `main`, the `amy` submodule must be pinned to the latest known working git version.

If an agent makes changes to `amy`, the agent must follow this exact sequence:

1. Open a PR in the `amy` repository for those `amy` changes.
2. Run `make test` in `amy`.
3. In `tulipcc`, point the `amy` submodule to the AMY PR commit and test `tulipcc`.
4. If both `amy` tests and `tulipcc` tests pass, merge the `amy` PR.
5. Pin the `amy` submodule in `tulipcc` to that merged `amy` commit.
6. Commit the submodule pointer update in `tulipcc`.
7. Merge/push `tulipcc` to `main`.

## Testing `tulipcc`

The easiest ways to test `tulipcc` are:

- `./build.sh` in `tulip/macos`
- `./build.sh` in `tulip/web`
- `./build.sh` in `tulip/amyboardweb`

Note:

- `./build.sh` in `tulip/web` and `tulip/amyboardweb` keep a server running.
