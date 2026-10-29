/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _LINUX_KSU_SELINUX_H
#define _LINUX_KSU_SELINUX_H

#include <linux/types.h>

struct selinux_state;
struct selinux_state *ksu_selinux_state_for_query(struct selinux_state *live);
int ksu_selinux_check_context(const char *name, const char *value, size_t size);
int ksu_selinux_hide_capture(void);

#endif
