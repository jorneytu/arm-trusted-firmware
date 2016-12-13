/*
 * Copyright (c) 2013-2016, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <asm_macros.S>
#include <platform_def.h>

	.globl	plat_secondary_cold_boot_setup
	.globl	plat_is_my_cpu_primary
	.globl  platform_mem_init
	.globl	plat_my_core_pos

	/* -----------------------------------------------------
	 * void plat_secondary_cold_boot_setup (void);
	 *
	 * This function performs any platform specific actions
	 * needed for a secondary cpu after a cold reset e.g
	 * mark the cpu's presence, mechanism to place it in a
	 * holding pen etc.
	 * TODO: Should we read the PSYS register to make sure
	 * that the request has gone through.
	 * -----------------------------------------------------
	 */
func plat_secondary_cold_boot_setup
	ldr		x0, =secondary_cortex_box
	ldr		x1, =0xdeadbeaf
	mrs		x3, mpidr_el1
	and		x3, x3, #0xff
	lsl		x3, x3, #0x03 
1:	ldr		x2, [x0, x3]
	cmp		x1, x2
	b.eq	2f
	str		x1, [x0, x3]
	br		x2	
2:	wfe
	b		1b
endfunc plat_secondary_cold_boot_setup

func plat_is_my_cpu_primary
	mrs		x1, mpidr_el1
	and		x1, x1, #0xff
	cmp		x1, #0
	cset	x0, eq
	ret
endfunc plat_is_my_cpu_primary

func platform_mem_init
	ret
endfunc platform_mem_init

	/* -----------------------------------------------------
	 *  unsigned int plat_my_core_pos(void)
	 *  This function uses the plat_arm_calc_core_pos()
	 *  definition to get the index of the calling CPU.
	 * -----------------------------------------------------
	 */
func plat_my_core_pos
	mrs	x0, mpidr_el1
	and	x0, x0, #0xff
	ret
endfunc plat_my_core_pos


.data
.align	3
.globl secondary_cortex_box
secondary_cortex_box:
.rept 4
			.quad	0xdeadbeaf
.endr
