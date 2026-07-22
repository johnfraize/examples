# simplestrings in a container (Pi gateway proof-of-concept)

Runs `simplestrings.c` (a stdin->stdout line-reverse filter) inside a minimal
container on a Raspberry Pi, demonstrating the containerized-Linux gateway
pattern: static binary in a `scratch` image, cgroup limits, read-only rootfs,
device passthrough, multi-container isolation.

## Build and run (on the Pi, armv7l)

```sh
# build the scratch image (compiles the static binary inside the build stage)
docker build -t simplestrings:pi .

# run it — pipe a line in, get it reversed out
echo "hello world" | docker run --rm -i simplestrings:pi     # -> dlrow olleh

# how small the image is (basically just the binary — no OS layers)
docker images simplestrings:pi
```

## Run it the way the gateway would

```sh
echo "hello" | docker run --rm -i \
  --memory=16m --cpus=0.5 \   # cgroup limits — one function can't starve others
  --read-only \               # read-only rootfs
  simplestrings:pi

# device passthrough — where an XBee/Zigbee /dev/ttyUSB0 would be handed in
echo "hi" | docker run --rm -i --device=/dev/ttyUSB0 simplestrings:pi
```

## Two isolated functions (BT vs Zigbee pattern)

```sh
docker build -t simplestrings:pi .
docker compose up          # radio-a + radio-b, each capped, on one box
```

## Notes

- **Static-linked** (`gcc -static`) so it needs no libc in the image — runs in
  `scratch` and sidesteps the `GLIBC_2.xx not found` host/target mismatch.
- **Build on the Pi** (native armv7l) or cross-build with
  `docker buildx build --platform linux/arm/v7`. Building on the Mac/x86 host
  gives the wrong arch.
- Honest framing: a **Pi proof-of-concept**, not a shipped containerized
  gateway.

Full writeup: `~/repos/howto/embedded-linux/run-a-static-binary-in-a-container-on-the-pi.md`
