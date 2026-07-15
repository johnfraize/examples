# Containerize simplestrings.c for the Pi (armv7l) gateway pattern.
#
# Multi-stage: a throwaway build stage compiles a *static* binary, the final
# image is `scratch` (empty) + the binary. Static-linking means no libc in the
# image and no `GLIBC_2.xx not found` at runtime.
#
# IMPORTANT: build this ON THE PI (native armv7l) so the binary matches the
# target. Building on the Mac/x86 host produces the wrong arch. From the repo
# root on the Pi:
#     docker build -t simplestrings:pi .
#     echo "hello" | docker run --rm -i simplestrings:pi     # -> olleh
#
# (Cross-build from an x86/arm64 host instead? Use buildx:
#     docker buildx build --platform linux/arm/v7 -t simplestrings:pi --load . )

# ---- build stage: full toolchain, discarded after ----
FROM debian:bookworm-slim AS build
RUN apt-get update \
 && apt-get install -y --no-install-recommends gcc libc6-dev \
 && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY simplestrings.c .
RUN gcc -static -O2 -Wall -o simplestrings simplestrings.c

# ---- final stage: nothing but the binary ----
FROM scratch
COPY --from=build /src/simplestrings /simplestrings
ENTRYPOINT ["/simplestrings"]
