# Reviver Kernel

Custom Linux 4.19.325 kernel for **Xiaomi Redmi 9 / lancelot** (MediaTek MT6768 family, ARM64, non-GKI).

## Features

- SukiSU Ultra with KPM support
- SUSFS v2.3.0, backported for this 4.19 tree
- NoMount v20
- WireGuard
- SELinux Enforcing
- Dated kernel version: `4.19.325-reviver-YYYYMMDD`

## SukiSU Ultra Manager

Get the manager from the official [SukiSU Ultra releases](https://github.com/SukiSU-Ultra/SukiSU-Ultra/releases).

## Build and install

This repository is the `kernel/` directory in the Reviver workspace. The sibling
`clang/` toolchain and `AnyKernel3/` installer are required by the workspace
`build.sh` launcher. Run `./build.sh` from the workspace to build and package a
flashable ZIP, then install that ZIP from recovery and reboot.

The date in the kernel version is set automatically by `build.sh`.
See [the agent guide](AGENTS.md) for the layout and incremental build commands.

## Device validation

The SukiSU/KPM integration boots LineageOS 23.2 (Android 16) on the test lancelot
with SELinux enforcing. Root access, all six extension capability queries,
KPM backend queries, SUSFS queries and the NoMount version interface were checked.
Individual third-party modules and every hardware function have not been tested.
See [the integration notes](Documentation/reviver/sukisu-integration.md) for pins
and [device verification](Documentation/reviver/device-validation-20261029.md)
for the exact checks and remaining limits.

## Credits

- SukiSU Ultra and KernelSU contributors
- SUSFS contributors
- KernelPatch contributors
- NoMount and WireGuard contributors
- Linux kernel contributors
- mt6768-S and contributors for the upstream kernel source
