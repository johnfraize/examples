# HPR Technical Interview — Embedded C / Linux Kernel Prep Plan

**Role:** Senior SWE, HPR (Hyannisport Research) — ultra-low-latency HFT.
**Tech round expected:** C (not C++), embedded-flavored, likely a **Linux kernel
module / device driver**. HPR ships **FPGA PCIe cards** (Riskbot®/Omnibot™), so the
probable center of gravity is **PCIe driver → DMA → mmap-to-userspace → low latency**.

**Your edge:** custom-FPGA Linux driver, IXP2800 microcode, 10Gbit data-plane TCP
stack. This plan **sharpens recall**, it does not teach from scratch.

**Cadence:** ~45–60 min/day, sustainable. Front-loaded: most-likely topics first.
**Hands-on kernel reps run on the Linux box, NOT the Mac.** Keep the toolchain warm.

Legend: 📖 review · 🛠️ hands-on rep · 🎯 interview angle

---

## SCHEDULED RUNWAY (operative plan)

**CONFIRMED: technical review with Principal Engineer — Wed 6/17, 11:00 AM** (morning,
John's strongest time). 45 min. 6 prep days available.

| Prep date | Plan day(s) | Focus |
|-----------|-------------|-------|
| Thu 6/11  | Day 1 | C depth & undefined behavior |
| Fri 6/12  | Day 2 | Bit manipulation & memory layout |
| Sat 6/13  | Day 3 + 5 | SPSC ring buffer (hands-on) + warm kernel toolchain (build/load a module) |
| Sun 6/14  | Days 6 + 7 | Char device + kernel concurrency (hands-on driver, Linux box) |
| Mon 6/15  | Day 8 | Interrupts / DMA / PCIe — **bullseye**, full day |
| Tue 6/16  | Day 9 + mock | mmap-to-userspace + latency, then full mock & weak-spot drill |
| Wed 6/17 AM | — | Light review, lock 2–3 stories, walk in fresh |

Driver is built incrementally Sat→Mon: hello-world → char device → spinlock → DMA/PCIe
→ mmap. By Tue's mock the whole path has been compiled & loaded, not just read.

---

## PHASE 1 — C & embedded core (Days 1–4) — do these even if time is short

### Day 1 — C language depth & undefined behavior
- 📖 Pointers & pointer arithmetic, function pointers, `const`/`volatile`/`restrict`,
  storage classes (static/extern) & linkage, integer promotion/conversion rules,
  UB (signed overflow, strict aliasing, uninitialized reads).
- 📖 `volatile` for HW registers — what it does, what it does NOT (not atomic, not a barrier).
- 🛠️ Write from memory: `memcpy`, `memmove` (explain the difference), `strlen`. Edge cases.
- 🎯 Have a UB war story ready — compilers exploit UB; embedded bugs hide there.

### Day 2 — Bit manipulation & memory layout
- 📖 Set/clear/toggle/test bit; masks; `stdint.h` types; endianness + byte-swap; struct
  padding/packing/alignment; bitfields (+ portability caveats); unions for type-punning.
- 🛠️ Code: count set bits, reverse bits in a u32, byte-swap u32/u64, extract a bitfield
  from a register value. Parse a packed network header from a `uint8_t*` buffer.
- 🎯 "Read a 32-bit register where bits 4–7 are a status field" — be fluent.

### Day 3 — Ring buffers & lock-free SPSC
- 📖 Circular buffer design; power-of-2 sizing & mask vs modulo; full/empty
  disambiguation; single-producer/single-consumer lock-free ring (head/tail + barriers).
- 🛠️ Implement a fixed-size SPSC ring in C, no locks, correct barrier placement.
- 🎯 Very likely whiteboard problem for an HFT data path. Nail it.

### Day 4 — Embedded patterns & no-malloc discipline
- 📖 Memory-mapped I/O; ISR constraints (no sleeping, short, reentrancy); top/bottom-half
  split; fixed pools / no dynamic alloc on hot path; state machines; fixed-point vs float;
  watchdogs; DMA concept; cache coherency (flush/invalidate).
- 🛠️ Code a table-driven state machine (e.g., a small protocol parser).
- 🎯 "Why no malloc in an ISR / hot path?" — latency determinism, fragmentation, allocator locks.

---

## PHASE 2 — Linux kernel modules & drivers (Days 5–9) — the probable main event

### Day 5 — Module mechanics
- 📖 `module_init`/`module_exit`, `MODULE_LICENSE`, Kbuild Makefile, `insmod`/`rmmod`/
  `modprobe`, `dmesg`, `printk` levels, kernel vs user address space.
- 🛠️ Linux box: build/load/unload a hello-world module; print to dmesg; clean rmmod.
- 🎯 Kernel APIs drift across versions — verify against the kernel you'll discuss.

### Day 6 — Character devices & the user/kernel boundary
- 📖 `file_operations`, `cdev`/`register_chrdev`, open/read/write/release,
  `copy_to_user`/`copy_from_user` (why), `ioctl`, major/minor, device nodes.
- 🛠️ Char driver exposing a buffer; read/write via copy_*_user; test with cat/echo + a C program.
- 🎯 Explain exactly what crosses the boundary and how you validate user input.

### Day 7 — Concurrency & synchronization in the kernel
- 📖 Spinlock vs mutex (and WHEN — can't sleep holding a spinlock or in IRQ context),
  `spin_lock_irqsave`, atomics, per-CPU vars, RCU (concept), seqlocks, wait queues,
  completions. Barriers: `rmb/wmb/mb`, `smp_*`, `READ_ONCE`/`WRITE_ONCE`.
- 🛠️ Add spinlock-protected concurrent access to Day 6's driver; reason about IRQ vs process context.
- 🎯 "Spinlock vs mutex, and why you can't sleep in an interrupt" — cold.

### Day 8 — Interrupts, DMA & memory-mapped devices (HPR bullseye)
- 📖 `request_irq`/threaded IRQs, top/bottom half (tasklets vs workqueues vs softirqs),
  `ioremap`, MMIO accessors (`readl`/`writel`), DMA mapping API (`dma_alloc_coherent`,
  streaming vs coherent), DMA + cache coherency, PCIe probe (`pci_driver`, BARs, MSI/MSI-X).
- 🛠️ Skeleton a PCIe driver: probe → map BAR → request IRQ → DMA ring → completion in bottom half.
- 🎯 The bullseye for an FPGA-card shop. Tie it to your real FPGA-driver experience.

### Day 9 — mmap-to-userspace & the latency path
- 📖 Driver `mmap` to map device/DMA memory into userspace (zero-copy); polling vs
  interrupt-driven completion; busy-poll for lowest latency; why HFT often runs the data
  path in userspace over an mmap'd region while the driver just sets it up.
- 🛠️ Add an `mmap` fop exposing a kernel buffer; userspace program reads it with no per-access syscall.
- 🎯 "Get market data from an FPGA card to a userspace strategy with minimum latency?"
  → mmap + DMA ring + busy-poll. THE story.

---

## PHASE 3 — Latency, debugging & mock (Days 10–12)

### Day 10 — Measurement, debugging & kernel tooling
- 📖 `rdtsc`/`ktime` measurement, histograms & percentiles (p50/p99/p99.9), `ftrace`,
  `perf`, `/proc` & `/sys`, oops/panic & stack-trace reading, kgdb, printk cost on hot
  paths, `WARN_ON`/`BUG_ON`.
- 🛠️ Add a latency histogram to your driver path; walk a sample oops trace.
- 🎯 "How do you chase a tail-latency spike?" — methodology answer.

### Day 11 — Integration + domain skim
- 🛠️ One-paragraph design: FPGA card → kernel driver (DMA ring, IRQ) → mmap → userspace
  busy-poll strategy, with latency-critical decisions called out.
- 📖 Light HFT domain (20 min, vocabulary only): order book, bid/ask, market-data multicast
  feeds, FIX. Jim said domain not required — this is fluency insurance.
- 🎯 Rehearse the bridge: recent Sonos SoC port (current, hands-on) → deep line-rate past
  (IXP2800, 10Gbit TCP data plane) as depth evidence.

### Day 12 — Mock & weak-spot drill
- 🛠️ Full mock tech round: 1 C/systems coding problem (ring buffer or bit/pointer task),
  1 kernel design discussion (driver + concurrency + DMA), 1 "hardest bug" story.
- 📖 Re-drill whatever cracked. Lock 2–3 crisp stories.

---

## If they call EARLY (3–4 days notice)
Non-negotiables: **Day 1, 2, 3, then 8 (DMA/PCIe) and 9 (mmap latency).** That's ~80% of
the signal for this shop.

## If it STRETCHES OUT (maintenance mode after Day 12)
~20 min/day: one coding rep + reload one kernel topic, rotating. Keep the toolchain warm.
Don't let it go cold.
