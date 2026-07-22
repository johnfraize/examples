# Audio-ring hack — VENDER prep (Day 1 setup, build on Day 2–3)

Goal: a hands-on talk-track piece for the **driver-dev round** (that interviewer
writes drivers) + a demoable **optimization** win for the optimization round.
Reuses the zero-copy mmap already working in `cdev_demo.c`.

## The 3-line plan
1. **Driver:** extend `cdev_demo.c`'s single `vmalloc_user` mmap buffer into an
   **audio-frame ring buffer** — N fixed-size frame slots + `head`/`tail` indices
   (producer/consumer), exposed via the existing `mmap` path. Reuse the hrtimer as
   the "frame ready" tick that bumps `head` and wakes the waitqueue.
2. **Userspace:** a consumer modeled on `~/repos/guitar/guitar.c` (ALSA `snd_pcm`
   loop) that mmaps the ring, reads frames lock-free (tail chases head), and plays
   them out via `aplay`/ALSA. No `copy_to_user` in the hot path = the zero-copy story.
3. **Optimization win (separate, pure userspace):** fix the real bug noted in
   `guitar/README.md` — guitar **double-copies output even when in/out channel
   counts already match** (`overCopy` is set unconditionally). Detect channel match
   and skip the conversion copy (straight `memcpy`/pass-through). Measure before/after.

## Why it scores
- mmap ring + head/tail + lock-free producer/consumer = **exactly the driver-dev
  round's domain** (market-data/order-entry rings). Talk track: "same idiom as an
  FPGA packet ring."
- The double-copy fix = a concrete "I made it faster by removing a copy" story for
  the optimization-heavy rounds. Eliminating an unnecessary copy in the hot path is
  the VENDER mindset in miniature.

## Build order
- **Day 2:** ring-buffer the driver (head/tail in the mmap region), rebuild, `insmod`,
  confirm `/dev/cdev_demo` + dmesg.
- **Day 3:** wire the guitar-style userspace consumer to the mmap ring; then do the
  double-copy fix in `guitar.c` and benchmark.
- **Day 4:** end-to-end demo + write the 3-sentence "what I built / why it's fast" pitch.

## Alternative angle: ASoC on a Pi (only if the above is done)

Different shape of the same theme — instead of a userspace ALSA consumer over the
mmap ring, go down into the audio subsystem itself:

- A **PCM5102A I2S DAC** board is a few dollars. Real I2S, real sound out.
- Minimal **ASoC machine driver** wiring the Pi's I2S to the codec.
- Add a **custom kcontrol** — your own mixer control, showing up in `alsamixer`.

Seeing a control you wrote appear in `alsamixer` is an unmistakably audio-domain
artifact, and it rides on the device-tree work (README progression item 7) since
the machine driver binds the same way. Failure mode to expect: card absent / no
sound = I2S clock or format mismatch.

## Reminders (from CLAUDE.md / README)
- Build **on the Linux box** with `linux-headers-$(uname -r)` — not the Mac/container.
- The ioctl contract is **duplicated** in `cdev_demo.c` and `test.c` — change both.
- Kernel: no FP, no large stack; `GFP_KERNEL` may sleep vs `GFP_ATOMIC` in IRQ;
  always check `copy_to/from_user` returns.
- guitar needs `libasound2-dev`.
