/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
*/

#include <linux/module.h>

/* Symbol export */
int soidle3_enter(int cpu)
{
	return 0;
}
EXPORT_SYMBOL(soidle3_enter);

int soidle_enter(int cpu)
{
	return 0;
}
EXPORT_SYMBOL(soidle_enter);

int dpidle_enter(int cpu)
{
	return 0;
}
EXPORT_SYMBOL(dpidle_enter);
