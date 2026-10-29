// SPDX-License-Identifier: GPL-2.0
/*
 * CPU spoof extension. io-only: ops = NULL, no sticky enable bit.
 *
 * GET  -> supported=true, value=0 (handler presence is the signal).
 * SET  -> -EOPNOTSUPP (no sticky bit).
 * IO   -> MIDR/lpj/hwcap/vDSO rewrite via userspace ksu_set_spoof_cpu_cmd.
 *
 * Forced-by-config is not wired: a force_extension=cpu_spoof:N entry
 * is rejected with a dmesg warning and clears the forced-pending bit.
 * The default force semantics are not obvious (per-device MIDR).
 */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/cpumask.h>
#include <linux/percpu.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/clocksource.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/version.h>


#include <asm/cpu.h>
#include <asm/hwcap.h>
#include <linux/math64.h>

/* Reviver 4.19 has separate HWCAP words and vDSO clock_mode == 0 means
 * direct arch-timer reads. Never index beyond elf_hwcap or clear CPU errata
 * capabilities based on a spoofed MIDR: those still describe real hardware.
 */
static DEFINE_MUTEX(ksu_cpu_spoof_mutex);

static int set_spoof_cpu(const struct ksu_set_spoof_cpu_cmd *cmd)
{
    struct cpuinfo_arm64 *info;
    unsigned long *hwcap, *hwcap2, *lpj;
    unsigned long base;
    u64 loops;

    if (!cmd || cmd->cpu_index >= nr_cpu_ids ||
        !cpu_possible(cmd->cpu_index))
        return -EINVAL;
    base = find_kernel_symbol_exact("cpu_data");
    hwcap = (void *)find_kernel_symbol_exact("elf_hwcap");
    hwcap2 = (void *)find_kernel_symbol_exact("elf_hwcap2");
    lpj = (void *)find_kernel_symbol_exact("loops_per_jiffy");
    if (!base || !hwcap || !hwcap2 || !lpj)
        return -ENXIO;
    info = per_cpu_ptr((struct cpuinfo_arm64 *)base, cmd->cpu_index);
    loops = div_u64((u64)cmd->bogomips * 500000, 100 * HZ);
    if (loops > ULONG_MAX)
        return -ERANGE;

    mutex_lock(&ksu_cpu_spoof_mutex);
    WRITE_ONCE(info->reg_midr, cmd->midr);
    if (!cmd->cpu_index) {
        WRITE_ONCE(*lpj, (unsigned long)loops);
        WRITE_ONCE(*hwcap, cmd->hwcap);
        WRITE_ONCE(*hwcap2, cmd->hwcap2);
    }
    mutex_unlock(&ksu_cpu_spoof_mutex);
    return 0;
}

int ksu_extension_cpu_spoof_io(u32 id, void __user *raw)
{
    struct ksu_set_spoof_cpu_cmd cmd;

    if (copy_from_user(&cmd, raw, sizeof(cmd)))
        return -EFAULT;

    return set_spoof_cpu(&cmd);
}
