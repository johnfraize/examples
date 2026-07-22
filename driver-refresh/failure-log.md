# Failure log

The driver itself is the easy part. The useful artifact is this: what broke, what
I thought it was, what it actually was, and how long it cost. Log every one.

**Format:** *symptom → what I suspected → what it actually was → time lost.*

## Predictable failures (the ones worth pre-loading)

| Symptom | Usual cause |
|---|---|
| `insmod: invalid module format` | `vermagic` / headers don't match the running kernel |
| Unknown symbol, module tainted | missing `MODULE_LICENSE("GPL")` |
| Compile error on `class_create` | 6.4+ dropped the owner (`THIS_MODULE`) argument |
| Compile error on `.remove` | platform driver `.remove` return type changed to `void` |
| Module loads, **probe never fires** | overlay not in `config.txt`, not copied to `overlays/`, or a `compatible` typo |
| `/dev` node missing | `class_create`/`device_create` skipped — udev has nothing to act on |
| `/dev` node present but permission denied | node is `root:root 0600`; needs the udev rule (see README) |
| Machine hangs on `rmmod` | lock held, or a sleeping call in atomic context |
| No sound, card absent | I2S clock/format mismatch in the machine driver |

## Entries

<!-- symptom → suspected → actual → time lost -->
