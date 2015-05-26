/*
 * Copyright (C) 2011-2014 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/pm.h>
#include <linux/bug.h>

#if defined(CONFIG_TRUSTONIC_TEE) || defined(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT)
#include <linux/memblock.h>
#include <linux/bootmem.h>
#include <mach/mtk_boot_share_page.h>
#endif

#include <asm/page.h>
#include <asm/smp_scu.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/mach/map.h>
#include <asm/mach-types.h>
#include <asm/hardware/cache-l2x0.h>

#include <asm/rt2880/rt_mmap.h>

#include <mach/mt_reg_base.h>
#include <mach/irqs.h>

extern void arm_machine_restart(char mode, const char *cmd);
extern struct sys_timer mt6582_timer;
extern void mt6582_timer_init(void);
extern struct smp_operations mt65xx_smp_ops;
extern void mt_fixup(struct tag *tags, char **cmdline, struct meminfo *mi);
extern void mt_reserve(void);

void __init mt_init(void)
{
	/* enable bus out of order command queue to enhance boot time */
	volatile unsigned int opt;
	opt = readl(IOMEM(MCU_BIU_BASE));
	opt |= 0x1;
	writel(opt, IOMEM(MCU_BIU_BASE));
	dsb();
}

#if defined(CONFIG_TRUSTONIC_TEE) || defined(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT)
void __init mt_init_early(void)
{
	int ret;

	ret = reserve_bootmem(__pa(BOOT_SHARE_BASE), 0x1000, BOOTMEM_EXCLUSIVE);
	if (ret < 0)
	{
		printk(KERN_WARNING "reserve_bootmem BOOT_SHARE_BASE failed %d\n", ret);
	}
}
#endif

static struct map_desc mt_io_desc[] __initdata =
{
	{
		.virtual = INFRA_BASE,
		.pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(INFRA_BASE)),
		.length = (SZ_1M - SZ_4K),
		.type = MT_DEVICE
	},
	/* Skip the mapping of 0xF0130000~0xF013FFFF to protect access from APMCU */
	{
		.virtual = (DEBUGTOP_BASE - SZ_4K),
		.pfn = __phys_to_pfn(IO_VIRT_TO_PHYS((DEBUGTOP_BASE - SZ_4K))),
		.length = (0x30000 + SZ_4K),
		.type = MT_DEVICE
	},
	{
		.virtual = (DEBUGTOP_BASE + 0x40000),
		.pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(DEBUGTOP_BASE + 0x40000)),
		.length = 0xC0000,
		.type = MT_DEVICE
	},
	{
		.virtual = MCUSYS_CFGREG_BASE,
		.pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(MCUSYS_CFGREG_BASE)),
		.length = SZ_2M,
		.type = MT_DEVICE
	},
	{
		.virtual = AP_DMA_BASE,
		.pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(AP_DMA_BASE)),
		.length = SZ_2M + SZ_1M,
		.type = MT_DEVICE
	},
	{
		/* virtual 0xF2000000, physical 0x00200000 */
		.virtual = SYSRAM_BASE,
		.pfn = __phys_to_pfn(0x00200000),
		.length = SZ_128K,
		.type = MT_MEMORY_NONCACHED
	},
	{
		.virtual = G3D_CONFIG_BASE,
		.pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(G3D_CONFIG_BASE)),
		.length = SZ_128K,
		.type = MT_DEVICE
	},
	{
		.virtual = DISPSYS_BASE,
		.pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(DISPSYS_BASE)),
		.length = SZ_16M,
		.type = MT_DEVICE
	},
	{
		.virtual = IMGSYS_CONFG_BASE,
		.pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(IMGSYS_CONFG_BASE)),
		.length = SZ_16M,
		.type = MT_DEVICE
	},
	{
		.virtual = VDEC_GCON_BASE,
		.pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(VDEC_GCON_BASE)),
		.length = SZ_16M,
		.type = MT_DEVICE
	},
	{
		/* virtual 0xF7000000, physical 0x08000000 */
		.virtual = DEVINFO_BASE,
		.pfn = __phys_to_pfn(0x08000000),
		.length = SZ_64K,
		.type = MT_DEVICE
	},
	{
		.virtual = CONN_BTSYS_PKV_BASE,
		.pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(CONN_BTSYS_PKV_BASE)),
		.length = SZ_1M,
		.type = MT_DEVICE
	},
	{
		/* virtual 0xF9000000, physical 0x00100000 */
		.virtual = INTER_SRAM,
		.pfn = __phys_to_pfn(0x00100000),
		.length = SZ_64K,
		.type = MT_MEMORY_NONCACHED
	},
	{
		.virtual = HIFSYS_BASE,
		.pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(HIFSYS_BASE)),
		.length = SZ_16M,
		.type = MT_DEVICE
	},
	{
		.virtual = ETHDMASYS_BASE,
		.pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(ETHDMASYS_BASE)),
		.length = SZ_16M,
		.type = MT_DEVICE
	},
#if 0
	{
		.virtual = BDP_DISPSYS_BASE,
		.pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(BDP_DISPSYS_BASE)),
		.length = SZ_32K + SZ_16K,
		.type = MT_DEVICE
	},
#endif
};

void __init mt_map_io(void)
{
	iotable_init(mt_io_desc, ARRAY_SIZE(mt_io_desc));
}

MACHINE_START(MT7623, "MT7623")
.atag_offset    = 0x00000100,
	.map_io         = mt_map_io,
	.init_irq       = mt_init_irq,
	.smp            = smp_ops(mt65xx_smp_ops),
	.init_time      = mt6582_timer_init,
#if defined(CONFIG_TRUSTONIC_TEE) || defined(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT)
	.init_early     = mt_init_early,
#endif
	.init_machine   = mt_init,
	.fixup          = mt_fixup,
	.restart        = arm_machine_restart,
	.reserve        = mt_reserve,
	MACHINE_END
