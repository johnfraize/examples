# Linux driver refresher — VENDER prep

A starter character driver to get your fingers back in kernel space, then a
ramp toward the topics that actually map to VENDER (line-rate networking, FPGA,
nanosecond latency) — and to your own résumé story.

## Quick start (on your Linux box)
```sh
sudo apt install build-essential linux-headers-$(uname -r)   # or distro equiv
make
sudo insmod cdev_demo.ko
dmesg | tail            # confirm load + major
./test                  # exercise read/write/ioctl/mmap
sudo rmmod cdev_demo
```

## Run `./test` without root (udev rule)
By default `device_create()` makes `/dev/cdev_demo` `root:root 0600`, so `./test`
needs sudo. Install the bundled rule to set the node `0666`:
```sh
sudo make install-udev   # cp 99-cdev_demo.rules -> /etc/udev/rules.d/, reload + re-trigger
ls -l /dev/cdev_demo     # now crw-rw-rw-
./test                   # runs as a normal user
```
udev only applies the rule when the node is *created*, so `install-udev`
re-triggers udev for an already-loaded module; otherwise `rmmod` + `insmod`.
Note: `sudo` needs a real TTY — run it in an interactive shell, not a non-TTY
wrapper.

## What the starter already exercises
- `cdev` + `class`/`device_create` registration (udev auto-creates `/dev/cdev_demo`)
- `file_operations`: read/write with `copy_to/from_user`
- `unlocked_ioctl` with a proper `_IO/_IOR/_IOW` contract
- **Zero-copy `mmap`** of a `vmalloc_user` buffer — *this is the one to dwell on.*

## The progression (rebuild the muscle in order of VENDER relevance)

1. **mmap / zero-copy (DONE in starter).** Talk track: this is exactly the
   FPGA-BAR / packet-ring idiom — userspace touches device memory with no copy
   in the hot path. Ties straight to your 10Gbit DPI engine.

2. **Interrupts + top/bottom half.** Add a `request_threaded_irq` (or simulate
   with a kernel timer / hrtimer firing into a workqueue or tasklet). Practice
   articulating *why* you keep the hard-IRQ handler tiny and defer work — the
   latency-determinism story VENDER lives in.

3. **DMA.** `dma_alloc_coherent` for a coherent ring buffer; understand
   streaming DMA (`dma_map_single`) + the sync calls, cache coherence, and why
   it matters at line rate.

4. **Locking / concurrency.** spinlock vs mutex vs RCU, per-CPU data, and
   lock-free hot paths — your "pin the flow to one core" design in driver terms.

5. **PCI or platform probe (skeleton DONE — `pcie_demo.c`).** A `pci_driver`
   with `.probe`: enable → `pci_set_master` → `pci_request_regions` → BAR map
   (`pci_iomap`) → `dma_set_mask_and_coherent` → `dma_alloc_coherent` ring →
   `pci_alloc_irq_vectors` (MSI/MSI-X) → `request_threaded_irq`. Registers but
   does not `.probe()` here (no matching card) — the idiom is the deliverable.

6. **sysfs attribute.** `DEVICE_ATTR_RW` with show/store, read and written from
   `/sys`. Small, but it's the config-plane counterpart to the ioctl contract and
   it comes up constantly.

7. **Device tree → `.probe()` (needs a Pi, not this box).** The one thing x86
   can't teach: here platform devices come from ACPI/PCI, so there's no natural
   DT binding to write. On a Pi you write a real overlay, `dtc -@` it to a
   `.dtbo`, drop it in `/boot/firmware/overlays/`, add `dtoverlay=` to
   `config.txt`, and watch `.probe()` fire. A `platform_driver` with an
   `of_device_id` table (`.compatible = "jf,demo"`) + `MODULE_DEVICE_TABLE(of,…)`.
   The moment worth having is the chain: DT node → `compatible` match → bus binds
   → `.probe()` runs — and debugging why it *didn't*.

8. **A deliberate bug.** Sleep in atomic context, or a double-free, and watch
   **lockdep** and **KASAN** catch it. Reading a real oops trace is its own skill
   and it doesn't transfer from reading about it.

## Debug tooling to actually use
```sh
dmesg -w                                   # live kernel log
sudo cat /sys/kernel/debug/tracing/trace   # ftrace
echo 'module cdev_demo +p' | sudo tee /sys/kernel/debug/dynamic_debug/control
```

## Refresher gotchas worth re-internalizing
- No floating point / no large stack in kernel; `GFP_KERNEL` may sleep,
  `GFP_ATOMIC` in IRQ context.
- `copy_to/from_user` can fault — always check the return.
- Modern API drift: `class_create()` dropped its owner arg in 6.4 (guarded in
  the .c); ioctl is `unlocked_ioctl`; `pr_info`/`dev_info` over `printk`.
- Build = Kbuild out-of-tree against `/lib/modules/$(uname -r)/build`.

## Log the failures
The driver is the easy part — what's actually worth having is the record of what
broke and how long it took to find. `failure-log.md` has the predictable-failure
table (vermagic, missing `MODULE_LICENSE`, probe never firing, hang on `rmmod`)
and the entry format. `interesting-driver-quirks.md` collects the API-churn
specifics.
