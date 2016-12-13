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

#include <debug.h>
#include <generic_delay_timer.h>
#include <mmio.h>
#include <platform.h>
#include <xlat_tables.h>

const mmap_region_t plat_arm_mmap[] = {
#if 0 
	{ APB, DEVICE0_BASE, DEVICE0_SIZE, MT_DEVICE | MT_RW | MT_SECURE },
	{ DEVICE1_BASE, DEVICE1_BASE, DEVICE1_SIZE, MT_DEVICE | MT_RW | MT_SECURE },
	{ CRF_APB_BASE, CRF_APB_BASE, CRF_APB_SIZE, MT_DEVICE | MT_RW | MT_SECURE },
#endif
	{ BL32_BASE, BL32_BASE, 0x200000, MT_MEMORY | MT_RW | MT_SECURE },
	{ DDRC_BASE, DDRC_BASE, DDRC_SIZE, MT_DEVICE | MT_RW | MT_SECURE },
	{ AHB_BASE, AHB_BASE, AHB_SIZE, MT_DEVICE | MT_RW | MT_SECURE },
	{ APB_BASE, APB_BASE, APB_SIZE, MT_DEVICE | MT_RW | MT_SECURE },
	{ AXI_BASE, AXI_BASE, AXI_SIZE, MT_DEVICE | MT_RW | MT_SECURE },
	{0}
};

