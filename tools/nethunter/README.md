# Reviver NetHunter (lancelot)

Experimental kernel support on branch `nethunter`, based on Reviver
`d1b030cbcf95c2b98733f1c32cc50a1438e402ba` (Linux 4.19.325).
Kali userspace, NetHunter APKs, firmware installation and Android USB gadget
configuration are separate installation steps; building this kernel does not
install them. Nova/SUSFS/KPM/NoMount and SELinux are retained.

## Build

From the kernel repository:

```sh
./tools/nethunter/build.sh
```

This merges `vendor/mt6768_defconfig`, `vendor/lancelot.config`, then
`vendor/lancelot-nethunter.config`, validates every requested option plus the
root features, exports preinit UAPI and builds the KPM-injected `Image.gz-dtb`.
Output is `out-nethunter/arch/arm64/boot/Image.gz-dtb`. It does not clean `out`,
package a ZIP, overwrite AnyKernel3, sign, flash or publish anything.
`NETHUNTER_OUT` and `NETHUNTER_JOBS` override the output directory and job count.
The workspace's existing `../build.sh` still builds the standard variant;
use the command above for NetHunter.

Drivers are built in, avoiding an uninstalled module dependency. External
adapters still need OTG power and their redistributable firmware in a path
served by Android's firmware loader. Firmware is not bundled here. Examples:
`rtw88/rtw8812a_fw.bin`, `rtw88/rtw8814a_fw.bin`, `rtw88/rtw8822b_fw.bin`,
`rt2870.bin`, `mt7601u.bin`, and ath9k_htc firmware. Use the exact firmware
requested by the selected driver's MODULE_FIRMWARE declaration / runtime log.
The current Android ROM must supply policy and USB configfs setup for HID,
mass storage, network gadgets and Bluetooth userspace access.

## Features

- Configfs HID, mass storage, serial/ACM, RNDIS and NCM; HID raw access.
- mac80211 monitor/injection extensions; USB ath9k_htc, carl9170, rt2800usb,
  mt7601u, mt76x0u/mt76x2u, rtl8187, rtl8xxxu and zd1211rw.
- Kali's rtw88 USB backport: RTL8723DU, 8821CU, 8822BU/CU, 8821AU/8811AU,
  8812AU and 8814AU. PCI/SDIO backport choices are marked BROKEN because
  only USB is integrated; shared MMC code and identifiers are untouched.
- External USB Bluetooth HCI, RFCOMM, BNEP, HIDP and virtual HCI.
- USB serial/Ethernet adapters, TUN, bridge/veth, SysV IPC, netfilter queue,
  redirect, TPROXY, socket/address/u32 matches.

Internal MediaTek WLAN does not use mac80211. These patches do not establish
internal-radio injection or monitor support. Existing vendor WLAN is retained.
Do not claim Wi-Fi, HID, adapter injection or Bluetooth runtime support until
those paths have been tested with the actual ROM, firmware and hardware.

## Official patch review

Source and patch SHA-256 values: [source-lock.json](source-lock.json).
Official kernel-builder pin: `e49544f3dea44724f354aa6fce76a59e781b18e7`.

| Official 4.19 patch | Disposition |
| --- | --- |
| `add-wifi-injection-4.19.patch` | Integrated with style cleanup and `CONFIG_NETHUNTER` gating across cfg80211, mac80211, rtl8187 and zd1211rw. Added an explicit monitor-vif guard before accessing the monitor union. |
| `add-rtw88-drivers-4.19.patch` | Imported rtw88 sources, manually wired Kconfig/Kbuild. Only USB enabled. Omitted host signing keys/certificates, OpenSSL config and unused firmware conversion utility. No replacement of shared SDIO IDs/MMC quirks. Omitted the new RX_FLAG_NO_PSDU flag on 4.19, where mac80211 has no consumer (the only use is the disabled 8703B SDIO path). Removed implicit-int warning suppression. |
| `fix-yylloc.dtc-lexer.lex.c_shipped.patch` | Inapplicable: shipped lexer absent; the existing generated-lexer build already succeeds. |
| `0001-prima-fix-netdev-notifier-crash.patch` | Inapplicable: Qualcomm prima driver absent on this MediaTek device. |

Legacy pre-4.x HID/CD-ROM and Qualcomm injection patches are inapplicable.
The older rt2800 RX-filter patch is unnecessary here: the existing 4.19 driver
already clears DROP_NOT_TO_ME when CONFIG_MONITORING is active.
No global permissive SELinux, device-permission loosening, regulatory bypass,
or internal WLAN firmware replacement is introduced. Monitor-channel changes
can interrupt managed connections sharing the same external radio; this is
one reason to keep the optional extensions gated and the branch experimental.

Official guidance:
- https://www.kali.org/docs/nethunter/nethunter-kernel-1-patching/
- https://www.kali.org/docs/nethunter/nethunter-kernel-2-config-1/
- https://gitlab.com/kalilinux/nethunter/build-scripts/kali-nethunter-kernel-builder

## Validation

See [VALIDATION.md](VALIDATION.md) for build provenance and remaining checks.
Before release: boot/root/Enforcing and root extensions; internal Wi-Fi
reconnect/hotspot, mobile data, USB ADB/MTP/tethering/charging, suspend/idle
battery, camera/audio. Then test USB adapter enumeration/firmware, monitor,
channel changes/injection, HID and network gadgets, Bluetooth and disconnect
handling. Build success cannot establish device or adapter compatibility.
