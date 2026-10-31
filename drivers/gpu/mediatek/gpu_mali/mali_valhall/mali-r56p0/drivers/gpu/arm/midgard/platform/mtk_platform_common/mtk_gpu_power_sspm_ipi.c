// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2018 MediaTek Inc.
 */

#include "mtk_gpu_power_sspm_ipi.h"
#include <linux/module.h>

void MTKGPUPower_model_stop(void)
{
}
EXPORT_SYMBOL(MTKGPUPower_model_stop);

void MTKGPUPower_model_start(unsigned int interval_ns)
{
}
EXPORT_SYMBOL(MTKGPUPower_model_start);

void MTKGPUPower_model_start_swpm(unsigned int interval_ns)
{
}
EXPORT_SYMBOL(MTKGPUPower_model_start_swpm);

void MTKGPUPower_model_suspend(void)
{
}
EXPORT_SYMBOL(MTKGPUPower_model_suspend);

void MTKGPUPower_model_resume(void)
{
}
EXPORT_SYMBOL(MTKGPUPower_model_resume);

int MTKGPUPower_model_init(void)
{
	return 0;
}

void MTKGPUPower_model_sspm_enable(void)
{
}
