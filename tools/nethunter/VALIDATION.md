# NetHunter build validation — 2026-11-20

Implementation commit: `eafd273bc94bc3a60fe840671da6083d7bd72be1`. Built with Proton
clang13, toolchain `9fb011b183fe7e69b04b873ef6533b4b077e3c5e`. Kali kernel-builder pin and
patch hashes are in [source-lock.json](source-lock.json). Root driver pins are
unchanged from the base; see `KernelSU/REVIVER-source-lock.json`.

A full build in a new `out-nethunter/` directory and a final incremental build
both exited0. The implementation was committed after the first build; the final
build ran against that committed implementation with a clean source tree.

- Release: `4.19.325-reviver-nethunter`.
- Image: `out-nethunter/arch/arm64/boot/Image.gz-dtb` (13,931,789 bytes).
- SHA-256: `4d8f6306a184382cde93a715497d9cd79b3e027e89f737a2b5e296607898d0a6`.
- Resolved config SHA-256: `bafd68ccfb4975d626e43359ceda000e935f0014ab627de213c11474c551ba41`.
- All93 fragment/root configuration requirements passed; Nova/SUSFS/KPM,
  NoMount, kallsyms and SELinux retained.
- KPM injection ran successfully. Gzip decompresses exactly to Image.kpm;
  final image equals gzip + mt6768.dtb + lancelot.dtbo, with both DT magic values
  checked. Six root extension handlers and selected USB driver symbols linked.
- Standard configuration regeneration has identical option values to the prior
  standard build. NetHunter/mac80211/rtw88 remain disabled there. No fresh
  standard image build was performed during this task.
- Prior standard out/ artifacts and AnyKernel3 installer/image hashes unchanged.
- Full integration diff passes `git diff --check`. The handwritten integration
  patch passes checkpatch with0 errors and0 warnings; this does not certify the
  entire imported upstream driver against local checkpatch style.

Compiler warning comparison:56 distinct source/message pairs,43 also present
in the prior standard-build log. The13 additionally observed pairs are in
unchanged KernelPatch host/backend code rebuilt in this fresh output tree;
absence in a cached prior build is not evidence of a NetHunter regression.
No compiler warnings were emitted from the imported rtw88 driver or modified
wireless paths. Existing device-tree warnings remain. No warnings were silenced
to obtain this result.

Local complete evidence: workspace `diagnostics/nethunter-20261120/`, including
build.log, build-final.log, resolved.config, manifest.json, source checkout,
standard-config-diff.json, warnings-comparison.txt and verify-artifacts.py.
The implementation commit plus pinned toolchain/root/patch sources identify the
source used; documentation-only follow-ups do not alter the built code.

No ZIP was packaged, no userspace/firmware was installed, no Git push occurred,
and no phone was queried, flashed or rebooted. Boot, internal Wi-Fi, external
adapter firmware/monitor/injection, HID, Bluetooth, charging and suspend remain
untested. Follow the runtime checklist in [README.md](README.md) before release.
