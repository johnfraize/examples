# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this repo is

A personal collection of self-contained C/C++ examples and coding-challenge
solutions — mostly interview / algorithm practice (prefix sums, hashmaps,
tokenizers, trees, sorting, threads, etc.). Each topic lives in its own directory
(or as a loose top-level `.c`/`.cpp`) and is an **independent program**. There is
no shared library, no root build, and no cross-directory dependencies to speak of
(the one exception: `hashmap/` includes `../BinarySearchTree/BinarySearchTree.h`).

Treat each directory/file as its own tiny project. The problem statements it's
working from live in `challenges.txt` (top level) and per-directory `README.md`
files; scratch notes are in `notes.md`.

## Building and running

There is **no unified build**. A given example builds one of three ways — check
in this order:

1. **Inline header comment** (the current convention, e.g. `subarrays/*.c`,
   `simplestrings.c`). The source's top doc-comment carries the exact
   `Build:` and `Run:` lines — use them verbatim. Example:
   ```sh
   cc -Wall -O2 -o subarray_sum_k subarray_sum_k.c
   ./subarray_sum_k < test1.txt
   ```
2. **`build.sh`** in the directory (e.g. `lambdas/`, `trees/`, `json/`,
   `threads/`) — a one-line `gcc`/`g++` invocation. Note per-dir flags:
   `threads` needs `-pthread`, `json` links `-l jansson` (external lib, see
   `json/README.md`).
3. **`Makefile`** (`hashmap/`, `BinarySearchTree/`) — `make <target>`, C++11.

When adding a new practice program, follow convention #1: open with a doc-comment
giving the problem statement, constraints, worked examples, and the `Build:`/`Run:`
commands. The `subarrays/*.c` files are the model for this style.

### Testing
Testing is manual, not a framework. Programs typically read a case from **stdin**
and print an answer to stdout; sample cases are committed as `testN.txt` / `t_*.txt`
files you redirect in (`./prog < test1.txt`). `sort/test.sh` shows the pattern of
running with args and eyeballing against an expected line. "Passing" means output
matches the examples in the source's doc-comment.

Some directories keep **sanitizer builds** alongside the normal binary — e.g.
`subarrays/ssk_asan`, `nc_asan` are AddressSanitizer builds (`-fsanitize=address`)
of the same source, used to debug memory bugs. Rebuild these when changing the
corresponding `.c`.

## Repo hygiene

- `*.c~` / `*.md~` are Emacs backup files, and bare binaries (`a.out`, `ssk_asan`,
  `subarray_sum_k`, `*.dSYM/`) are build artifacts. Both are untracked noise —
  don't commit them, and don't treat them as source.
- Commit messages here are terse and topical (e.g.
  `subarray_sum_k: O(n) via from-scratch 2-level radix-trie hashmap`) — describe
  the algorithm/approach, not just "added file".

## `driver-refresh/` is a separate sub-project

`driver-refresh/` is an out-of-tree **Linux kernel character driver** with its own
`CLAUDE.md`, `README.md`, Kbuild `Makefile`, and study plans (`tech-prep.md`).
It builds/runs only on a Linux box with kernel headers (not on this Mac), and has
its own invariants. Read `driver-refresh/CLAUDE.md` before touching anything in it.
