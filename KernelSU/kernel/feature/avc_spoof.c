// SPDX-License-Identifier: GPL-2.0-only
/* Share the existing SUSFS AVC formatter toggle with the manager feature API. */
#ifdef CONFIG_KSU_SUSFS
static int ksu_avc_spoof_get(u64 *value)
{
    *value = susfs_get_avc_log_spoofing_enabled();
    return 0;
}

static int ksu_avc_spoof_set(u64 value)
{
    if (value > 1)
        return -EINVAL;
    susfs_set_avc_log_spoofing_enabled(value != 0);
    return 0;
}

static const struct ksu_feature_handler ksu_avc_spoof_handler = {
    .feature_id = KSU_FEATURE_AVC_SPOOF,
    .name = "avc_spoof",
    .get_handler = ksu_avc_spoof_get,
    .set_handler = ksu_avc_spoof_set,
};

static void __init ksu_avc_spoof_init(void)
{
    if (ksu_register_feature_handler(&ksu_avc_spoof_handler))
        pr_err("Failed to register avc_spoof feature handler\n");
}
#endif
