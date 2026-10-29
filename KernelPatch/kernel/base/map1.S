/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2023 bmax121. All Rights Reserved.
 */

#define __ASSEMBLY__
#include "setup.h"

.text
.balign 4
.global _paging_init
.type _paging_init, %function
_paging_init:
    stp x29, x30, [sp, #-KP_KERNEL_FRAME_SIZE]!
    stp x19, x20, [sp, #16]
    stp x21, x22, [sp, #32]
    stp x23, x24, [sp, #48]
    stp x25, x26, [sp, #64]
    stp x27, x28, [sp, #80]
    mov x29, sp
    mov x0, sp
    bl _paging_init_body
    // Only used if start_init fails before the map anchor is restored.
    ldp x19, x20, [sp, #16]
    ldp x21, x22, [sp, #32]
    ldp x23, x24, [sp, #48]
    ldp x25, x26, [sp, #64]
    ldp x27, x28, [sp, #80]
    ldp x29, x30, [sp], #KP_KERNEL_FRAME_SIZE
    ret
.size _paging_init, . - _paging_init
