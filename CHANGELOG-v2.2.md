# Reviver Kernel v2.2

Changes since v2.1 (`dfed91a24f3c`), November 17, 2026:

- Synced MT6768 upstream, including hotspot automatic-channel-selection and Wi-Fi DMA initialization fixes.
- Updated SukiSU Ultra to **SukiSU Nova 3.0.0** with mount-view filtering, file-reference fixes and safer root-command/profile handling. Preserved local 4.19 CPU spoof, KPM and legacy SELinux integration.
- Updated **SUSFS 2.3.0 → 2.4.0**, adding per-app hidden-path visibility controls, UID-filter updates and completed native/compat directory hooks.
- Updated manager/ksud source with built-in Zygisk, matching SUSFS controls, non-GKI fixes and refreshed appearance/theme support. Manager/ksud requires a separate build/install; it is not an APK embedded in the kernel ZIP.
- Updated CPU scheduling: corrected 6+2 topology boundaries, OPP-aware utilization clamping, 20 clamp buckets, linear scheduler scaling and pending schedutil updates.
- Improved compaction cancellation, zram locking, DMA allocation cleanup, scatterlist handling and other memory/RCU paths.
- Updated GPU/media wakeups and sync-fence handling; reduced driver diagnostics/debug/test overhead and disabled per-cgroup pressure accounting.
- Updated HVDCP/JEITA charging policy and current limits, including a requested 1 A limit in affected paths at >=35°C or >=4.3 V battery voltage. Restored the FUSB303 Type-C mode callback declaration.
- Refreshed Nova identity, pinned source/version metadata, release documentation and build/device-validation records.

Linux remains **4.19.325**, Mali remains **r56p0**, and NoMount remains **v22**; those versions are not new v2.2 upgrades.

The signed kernel ZIP passed build/signature/artifact checks. Initial device queries confirm boot, root/SELinux Enforcing, six supported extensions and responding SUSFS/KPM/NoMount backends. Broader functional testing remains open; the known scheduler warning also appears in the October 29 baseline. See [build provenance](KernelSU/BUILD-20261117.md) and [device findings](KernelSU/DEVICE-20261117.md).
