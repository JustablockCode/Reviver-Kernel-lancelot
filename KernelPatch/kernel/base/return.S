/* SPDX-License-Identifier: GPL-2.0-or-later */
#define __ASSEMBLY__
#include "setup.h"

// Runs from permanent payload text after start() restores the map anchor.
.text
.balign 4
.global kp_return_to_kernel
.type kp_return_to_kernel, %function
kp_return_to_kernel:
    mov sp, x0
    ldp x19, x20, [sp, #16]
    ldp x21, x22, [sp, #32]
    ldp x23, x24, [sp, #48]
    ldp x25, x26, [sp, #64]
    ldp x27, x28, [sp, #80]
    ldp x29, x30, [sp], #KP_KERNEL_FRAME_SIZE
    ret
.size kp_return_to_kernel, . - kp_return_to_kernel
