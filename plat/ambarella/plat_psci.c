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

#include <arch_helpers.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>
#include <gicv2.h>
#include <mmio.h>
#include <plat_arm.h>
#include <platform.h>
#include <psci.h>
#include <delay_timer.h>

extern long secondary_cortex_box[];
static uintptr_t sec_entrypoint_save = 0;

void ambarella_cpu_standby(plat_local_state_t cpu_state)
{

	NOTICE("%s %d\n", __func__, __LINE__);
}

static int ambarella_boot_secondary_cortex(u_register_t mpidr)
{
	switch(mpidr) {
	case 0:
		break;
	case 1:
		mmio_clrbits_32(CORTEX_RESET_REG, 1 << 6);
		break;
	case 2:
		mmio_clrbits_32(CORTEX_RESET_REG, 1 << 18);
		break;
	case 3:
		mmio_clrbits_32(CORTEX_RESET_REG, 1 << 19);
		break;
	default:
		NOTICE("%s %d\n", __func__, __LINE__);
		panic();
	}
	return 0;
}

static int ambarella_pwr_domain_on(u_register_t mpidr)
{

	ambarella_boot_secondary_cortex(mpidr);
	mdelay(20);
	secondary_cortex_box[mpidr] = sec_entrypoint_save;
	flush_dcache_range((uint64_t)secondary_cortex_box, 4 * sizeof(long));

	__asm__ volatile("SEV":::);

	return PSCI_E_SUCCESS;
}

static void ambarella_pwr_domain_off(const psci_power_state_t *target_state)
{
	NOTICE("%s %d\n", __func__, __LINE__);
}


static void ambarella_pwr_domain_suspend(const psci_power_state_t *target_state)
{
	NOTICE("%s %d\n", __func__, __LINE__);
}

static void ambarella_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
}


static void ambarella_pwr_domain_suspend_finish(const psci_power_state_t *target_state)
{
	NOTICE("%s %d\n", __func__, __LINE__);
}

static void __dead2 ambarella_system_off(void)
{
	NOTICE("%s %d\n", __func__, __LINE__);
	panic();
}

static void __dead2 ambarella_system_reset(void)
{
	mmio_setbits_32(SOFT_OR_DLL_RESET_REG, 0x7);
	panic();
}

int ambarella_validate_power_state(unsigned int power_state,
				psci_power_state_t *req_state)
{
	NOTICE("%s %d\n", __func__, __LINE__);
	return PSCI_E_SUCCESS;
}

int ambarella_validate_ns_entrypoint(unsigned long ns_entrypoint)
{
	return PSCI_E_SUCCESS;
}

void ambarella_get_sys_suspend_power_state(psci_power_state_t *req_state)
{
	NOTICE("%s %d\n", __func__, __LINE__);
}

/*******************************************************************************
 * Export the platform handlers to enable psci to invoke them
 ******************************************************************************/
static const struct plat_psci_ops ambarella_psci_ops = {
	.cpu_standby			= ambarella_cpu_standby,
	.pwr_domain_on			= ambarella_pwr_domain_on,
	.pwr_domain_off			= ambarella_pwr_domain_off,
	.pwr_domain_suspend		= ambarella_pwr_domain_suspend,
	.pwr_domain_on_finish		= ambarella_pwr_domain_on_finish,
	.pwr_domain_suspend_finish	= ambarella_pwr_domain_suspend_finish,
	.system_off			= ambarella_system_off,
	.system_reset			= ambarella_system_reset,
	.validate_power_state		= ambarella_validate_power_state,
	.validate_ns_entrypoint		= ambarella_validate_ns_entrypoint,
	.get_sys_suspend_power_state	= ambarella_get_sys_suspend_power_state,
};


/*******************************************************************************
 * Export the platform specific power ops.
 ******************************************************************************/
int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const struct plat_psci_ops **psci_ops)
{
	sec_entrypoint_save = sec_entrypoint;
	*psci_ops = &ambarella_psci_ops;

	return 0;
}
