# Reviver SukiSU Nova / SUSFS integration

Updated 2026-11-17. **Kernel compilation, signed ZIP packaging and artifact verification passed at source `40133b4dce55`; manager compilation and device testing remain pending.** See [build report](BUILD-20261117.md). Nothing was pushed and no device operation was performed.

## Provenance

[REVIVER-source-lock.json](REVIVER-source-lock.json) pins the hybrid vendor snapshot: Nova builtin kernel `4bec532dd447e2852aa8932afa768255350c103a`, Nova main manager/ksud `1439545f12bffe0263690ba862139b70ee5f20cf`, and SUSFS `784ac9e6fd49bb02499b5b60c57ccc9d0c680204` on `gki-android12-5.10`. SUSFS reports v2.4.0 NON-GKI. KernelPatch stays at `1c5b6089`; NoMount remains v22.

Applied the upstream deltas through a three-way merge against the previous builtin `85d3b048`, main `e67b14e6` and SUSFS `8930474a` pins, retaining local adaptations. BuiltZygisk `dfe0b4d2` and its four pinned native dependencies are flattened into `external/BuiltZygisk/`, with licenses retained and no nested repositories. Upstream CI workflows are omitted from this vendored tree.

## Identity and offline versions

Default manager package: `com.sukisu.nova` in kernel, Gradle and ksud. Java/Kotlin sources use the new Nova namespace. Official manager signing checks and dynamic manager handling remain in place. Ultra app settings do not automatically migrate across Android package names; verify manager recognition and module/profile settings after switching.

Version metadata never uses the enclosing kernel's Git commit count or network lookups:

- Kernel: `KSU_VERSION=30007`, `v3.0.0-4bec532d-reviver@builtin`. This follows upstream builtin's 30000 + pinned main count4008 -4001 formula.
- Manager/ksud: code35004, name3.0.0-1439545f-reviver (manager name includes the usual leading v). This follows their separate 30000 +4008 +996 formula. Kernel and app version codes intentionally differ as upstream does; UAPI compatibility is checked separately.
- `kernel/reviver-version.mk`, manager Gradle metadata and ksud build.rs hold these fixed values.
- Manager KPM tools remain pinned to `eec45788`, now through the Nova asset URL, and the existing local/preseed path remains supported.

## 4.19 adaptations retained

- **CPU spoof:** keep the actual local ARM64 implementation and register it on this 4.19 target through `KSU_CAN_USE_CPU_SPOOF`. Use separate `elf_hwcap` / `elf_hwcap2`; retain real CPU errata controls. Upstream generic source would require5.10+ and is not substituted here.
- **KPM:** keep the working explicit ARM64 save/restore frame, native bridge and pre-compression backend injection. Effective capability gate remains >=4.4. Backend source/pin is unchanged.
- **Preinit:** keep local `preinit/preinit.c`, runner and blob build dependencies used with SUSFS, including target UAPI export. The upstream common runner updates are present but do not replace the active SUSFS preinit path.
- **SELinux:** keep `feature/selinux_hide_legacy.c` and the manual query hooks. Live policy still governs enforcement. Original-policy snapshot lifetime is unchanged. AVC resets use the new sequence argument with0 on the legacy path; domain SID refresh stays outside the policy lock.
- New upstream SUSFS type/rule bookkeeping calls are gated to5.10+ in sepolicy.c. The legacy4.19 query snapshot remains the hiding mechanism. This avoids adding mutex/allocating bookkeeping under legacy policy locking; no second query/enforcement hook is installed.
- **UAPI:** root version6, extension ABI2 and `uapi -> ./kernel/include/uapi` remain unchanged.

## SUSFS 2.4.0 wiring

Updated fs/susfs.c and both SUSFS headers from the pinned branch, retaining the4.19 fsnotify callback/group/mark adaptations and deferred cleanup. Added no-op ratelimited log macros for logging-disabled builds.

`CMD_SUSFS_SET_SUS_PATH_UID_FILTER` (0x55554) now dispatches in the builtin kernel, calls the SUSFS setter and refreshes its package bitmap from package-observer task work. The same payload and command exist in Nova main ksud and manager, including persistence/boot restoration. With a configured allow-list, selected packages can see configured SUS paths while other application UIDs cannot. Empty selection hides the paths from all application UIDs.

Priority UID filtering is now explicitly guarded by `CONFIG_KSU_SUSFS_PRIORITY_UID_FILTER` across implementation, headers, dispatch, observer and getpriority/setpriority/ioprio syscall hooks. The lancelot fragment enables it. The existing4.19 pathname/readdir hooks already use the new single-inode signature. Existing mount/stat/map/redirect hooks are retained.

Mount hiding now uses upstream's post-open file-permission sequence-record filter, rather than returning ENOENT for proc mount files. The local proc_mounts prefix matches the filter's private view. This feature now defaults off as upstream does; enable and validate it through the manager if needed. The file-wrapper reference fix and userspace profile/path/root-shell fixes are included.

## Build and test handoff

Run `./build.sh` from the surrounding workspace for a clean signed ZIP. It does not build the manager/ksud. The initial source update left existing artifacts intact; the subsequent authorized build refreshed the image and AnyKernel3 package. See the build report for hashes and backups.

Configuration was resolved in a separate diagnostics directory, confirming KSU, ADBROOT, KPM, full kallsyms, all requested SUSFS features including PRIORITY_UID_FILTER, lancelot audio and NoMount. Static checks cover shared command/struct layouts, retained local source hashes, source dependencies and formatting. That initial source-only check did not establish compile/runtime success; subsequent kernel compile/package validation is recorded in the build report. Runtime testing remains pending. First-party `git diff --check` and shell syntax checks pass. Imported third-party whitespace is preserved. Checkpatch on the imported kernel integration (including the generated common runner) reports674 style errors and1921 warnings; this is recorded evidence, not a clean checkpatch result or compiler failure.

For a manager source build, follow the vendored manager/ksud build tooling and supply its target binaries/assets. BuiltZygisk source and dependencies are included; the kernel build does not produce its userspace assets. No manager APK or Zygisk payload was built during this task.

Device checks: boot/reboot, enforcing SELinux, Nova manager recognition, root grant/deny/profiles, all six extension capabilities, real KPM backend query/module behavior, CPU spoof, SUSFS path allow-list plus persistence and package-change refresh, priority UID filter, mount-file reads, NoMount/modules and safe mode. Also test hotspot, graphics, audio, camera, charging/USB and suspend. Keep the existing complete boot backups for recovery.

## Completed directory hook port

The first user build exposed obsolete susfs_get_data_path calls in readdir.c. Commit40133b4dce55 removes the retired backing-path handling from all5 native/compat wrappers, retaining the current inode/UID filter against the enumerated file superblock. The corrected source links and packages successfully; see BUILD-20261117.md.
