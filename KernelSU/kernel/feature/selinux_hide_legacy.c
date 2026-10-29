// SPDX-License-Identifier: GPL-2.0-only
/* Read-only stock-policy view for Reviver's selinux_state/selinux_ss ABI.
 * Keep enforcement and every permission check on the live policy. Only the
 * explicit selinuxfs query hooks below use the snapshot. The snapshot lives
 * for the lifetime of this built-in driver, including while toggled off.
 */
#include <linux/ksu_selinux.h>

static DEFINE_MUTEX(ksu_legacy_hide_mutex);
static struct selinux_state ksu_legacy_query_state;
static bool ksu_legacy_snapshot_ready;
bool ksu_selinux_hide_running __read_mostly;
bool ksu_selinux_hide_enabled __read_mostly;

int ksu_selinux_hide_capture(void)
{
    struct selinux_ss *ss;
    struct policy_file fp;
    void *data = NULL;
    size_t len;
    int ret;

    mutex_lock(&ksu_legacy_hide_mutex);
    if (ksu_legacy_snapshot_ready) {
        ret = 0;
        goto out;
    }
    ss = kzalloc(sizeof(*ss), GFP_KERNEL);
    if (!ss) {
        ret = -ENOMEM;
        goto out;
    }
    rwlock_init(&ss->policy_rwlock);
    mutex_init(&ss->status_lock);
    ret = security_read_policy(&selinux_state, &data, &len);
    if (ret)
        goto free_ss;
    /* policydb_write omits these Android vendor configuration bits. */
    if (len >= 24) {
        __le32 *config = (__le32 *)((char *)data + 20);
        u32 value = le32_to_cpu(*config);

        if (selinux_state.ss->policydb.android_netlink_route)
            value |= POLICYDB_CONFIG_ANDROID_NETLINK_ROUTE;
        if (selinux_state.ss->policydb.android_netlink_getneigh)
            value |= POLICYDB_CONFIG_ANDROID_NETLINK_GETNEIGH;
        *config = cpu_to_le32(value);
    }
    fp.data = data;
    fp.len = len;
    ret = policydb_read(&ss->policydb, &fp);
    vfree(data);
    data = NULL;
    if (ret)
        goto free_ss;
    ss->policydb.len = len;
    ss->sidtab = kzalloc(sizeof(*ss->sidtab), GFP_KERNEL);
    if (!ss->sidtab) {
        ret = -ENOMEM;
        goto destroy_policy;
    }
    ret = policydb_load_isids(&ss->policydb, ss->sidtab);
    if (ret)
        goto free_sidtab;
    /* Query services take policy class numbers, not a live AVC mapping. */
    ss->latest_granting = selinux_state.ss->latest_granting;
    ksu_legacy_query_state = selinux_state;
    ksu_legacy_query_state.ss = ss;
    smp_store_release(&ksu_legacy_snapshot_ready, true);
    pr_info("selinux_hide: captured original 4.19 policy\n");
    ret = 0;
    goto out;

free_sidtab:
    kfree(ss->sidtab);
destroy_policy:
    policydb_destroy(&ss->policydb);
free_ss:
    vfree(data);
    kfree(ss);
out:
    mutex_unlock(&ksu_legacy_hide_mutex);
    return ret;
}

struct selinux_state *ksu_selinux_state_for_query(struct selinux_state *live)
{
    if (current_uid().val >= 10000 && READ_ONCE(ksu_selinux_hide_running) &&
        smp_load_acquire(&ksu_legacy_snapshot_ready))
        return &ksu_legacy_query_state;
    return live;
}

/* Called after the real setprocattr permission check. A context absent from
 * the stock policy must not be accepted merely because KSU added it. */
int ksu_selinux_check_context(const char *name, const char *value, size_t size)
{
    struct selinux_state *view = ksu_selinux_state_for_query(&selinux_state);
    u32 sid;

    if (view == &selinux_state || strcmp(name, "current") || !size ||
        !value[0] || value[0] == '\n')
        return 0;
    return security_context_to_sid(view, value, size, &sid, GFP_KERNEL);
}

static int ksu_legacy_hide_get(u64 *value)
{
    *value = READ_ONCE(ksu_selinux_hide_enabled);
    return 0;
}

static int ksu_legacy_hide_set(u64 value)
{
    int ret = 0;

    mutex_lock(&ksu_legacy_hide_mutex);
    if (value && !smp_load_acquire(&ksu_legacy_snapshot_ready)) {
        ret = -EAGAIN;
        goto out;
    }
    WRITE_ONCE(ksu_selinux_hide_enabled, !!value);
    WRITE_ONCE(ksu_selinux_hide_running, !!value);
out:
    mutex_unlock(&ksu_legacy_hide_mutex);
    return ret;
}

void __init ksu_selinux_hide_init(void)
{
    static const struct ksu_feature_handler handler = {
        .feature_id = KSU_FEATURE_SELINUX_HIDE,
        .name = "selinux_hide",
        .get_handler = ksu_legacy_hide_get,
        .set_handler = ksu_legacy_hide_set,
    };

    ksu_register_feature_handler(&handler);
}

void ksu_selinux_hide_handle_second_stage(void)
{
    if (smp_load_acquire(&ksu_legacy_snapshot_ready)) {
        struct page *live = selinux_kernel_status_page(&selinux_state);
        struct page *copy = selinux_kernel_status_page(&ksu_legacy_query_state);

        if (live && copy) {
            mutex_lock(&selinux_state.ss->status_lock);
            memcpy(page_address(copy), page_address(live),
                   sizeof(struct selinux_kernel_status));
            mutex_unlock(&selinux_state.ss->status_lock);
        }
    }
}

void ksu_selinux_hide_handle_post_fs_data(void)
{
    /* The query snapshot deliberately remains the original boot policy. */
}

void ksu_selinux_hide_drop_backup_if_unused(void)
{
    /* Built-in query hooks can retain references. Keep it until reboot. */
}

void ksu_selinux_hide_exit(void)
{
    ksu_legacy_hide_set(0);
    ksu_unregister_feature_handler(KSU_FEATURE_SELINUX_HIDE);
}
