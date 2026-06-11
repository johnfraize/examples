# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

An out-of-tree Linux **character device driver** (`cdev_demo.ko`) plus a userspace
exerciser, written as interview-prep practice for low-latency / FPGA-PCIe kernel
work. The driver deliberately demonstrates the full hot-path idiom for high-perf
device drivers: `cdev`/`class` registration → `file_operations` → `unlocked_ioctl`
→ **zero-copy `mmap` of a `vmalloc_user` buffer** (the FPGA-BAR / packet-ring pattern).

`tech-prep.md` is the study roadmap that drives what gets built next
(interrupts → DMA → locking → PCIe/platform probe). Treat it as the backlog: when
extending the driver, the README "progression" section and the plan describe the
intended next features and the talk-track each is meant to exercise.

## Build / run / test

Kernel build is Kbuild out-of-tree against the running kernel's headers — it must
run **on a Linux box with `linux-headers-$(uname -r)` installed**, not in a
container or on a Mac.

```sh
make                    # builds cdev_demo.ko (kernel) AND the userspace `test` binary
sudo insmod cdev_demo.ko
dmesg | tail            # confirm load; note "major=N"; udev creates /dev/cdev_demo
./test                  # exercises write/read, both ioctls, and the mmap path
sudo rmmod cdev_demo
make clean
```

`make module` / `make test` build the two halves independently. There is no test
framework — `test.c` is the validation harness; "passing" means its printed
read-backs match what was written (including the value written through the mmap).

## Critical invariant: the ioctl contract is duplicated

The `_IO`/`_IOR`/`_IOW` definitions (`DEMO_IOC_MAGIC 'D'`, `RESET`/`GET_LEN`/`SET_LEN`)
exist in **both** `cdev_demo.c` and `test.c`. They are not shared via a header — if
you change one, change the other or the userspace exerciser silently breaks.

## Kernel-version sensitivity

This builds against whatever kernel is running, and kernel APIs drift. The code
already guards `class_create()` (lost its `owner` arg in 6.4) via
`LINUX_VERSION_CODE`. When adding APIs, expect similar drift and guard accordingly.
Standard kernel constraints apply and are the point of the exercise: no floating
point / no large stack, `GFP_KERNEL` may sleep vs `GFP_ATOMIC` in IRQ context, and
every `copy_to/from_user` return must be checked (it can fault).
