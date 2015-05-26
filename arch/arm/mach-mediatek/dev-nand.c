#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>

#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <mach/hardware.h>
#include <mach/map.h>

#include <plat/devs.h>

static struct resource mtk_resource_nand[] = {
	[0] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(NFI_BASE), 0x1A0),
	[1] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(NFIECC_BASE), 0x150),
	[2] = DEFINE_RES_IRQ(MT_NFI_IRQ_ID),
	[3] = DEFINE_RES_IRQ(MT_NFIECC_IRQ_ID),
};

struct platform_device mtk_nand_dev = {
	.name = "mtk-nand",
	.id   = 0,
	.num_resources	= ARRAY_SIZE(mtk_resource_nand),
	.resource		= mtk_resource_nand,
	.dev            = {
		.platform_data = &mtk_nand_hw,
	},
};
