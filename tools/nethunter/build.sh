#!/bin/bash
# Build the NetHunter variant without cleaning the standard output/installer.
set -euo pipefail
KERNEL_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$KERNEL_DIR"
export PATH="$(cd ../clang/bin && pwd):$PATH"
OUT="${NETHUNTER_OUT:-out-nethunter}"
JOBS="${NETHUNTER_JOBS:-$(nproc)}"
FLAGS=(ARCH=arm64 O="$OUT" LLVM=1 LLVM_IAS=1 CC=clang
       HOSTCC=clang HOSTCXX=clang++ HOSTLDFLAGS=-fuse-ld=lld
       CLANG_TRIPLE=aarch64-linux-gnu- CROSS_COMPILE=aarch64-linux-gnu-)
make "${FLAGS[@]}" vendor/mt6768_defconfig
scripts/kconfig/merge_config.sh -O "$OUT" -m "$OUT/.config" \
    arch/arm64/configs/vendor/lancelot.config \
    arch/arm64/configs/vendor/lancelot-nethunter.config
make "${FLAGS[@]}" olddefconfig
python3 tools/nethunter/verify-config.py "$OUT/.config"
make "${FLAGS[@]}" headers_install
make "${FLAGS[@]}" -j"$JOBS" Image.gz-dtb
sha256sum "$OUT/.config" "$OUT/arch/arm64/boot/Image.gz-dtb"
