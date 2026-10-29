# Reviver vendor snapshot

Kernel: SukiSU Ultra builtin `85d3b0483df224f1dcf6f2880b3181d3f2ca4e0f`.
Manager/userspace: main `e67b14e6bbbbd3e67f24df658a01d3e73a64159c`.
Canonical UAPI: `kernel/include/uapi/`, shared through the `uapi` symlink.

This is a vendored tree with local 4.19 integration changes, not a nested Git
checkout. Local changes include version metadata, a legacy SELinux query view,
CPU spoofing adaptations, preinit ownership/build fixes and UAPI alignment.
See `../Documentation/reviver/sukisu-integration.md` and
`../scripts/reviver/source-lock.json`. Do not run setup.sh over this snapshot.

A clean kernel/KPM/preinit build and ZIP packaging passed at source commit
88ec3e9dd713; device/runtime and manager-build validation remain outstanding.
