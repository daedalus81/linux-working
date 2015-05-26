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

static u64 mtkfb_dmamask = ~(u32)0;

static struct resource resource_fb[] = {
	{
		.start	= 0, /* Will be redefined later */
		.end	= 0,
		.flags	= IORESOURCE_MEM
	}
};

struct platform_device mtk_device_fb = {
	.name 		= "mtkfb",
	.id   		= 0,
	.num_resources 	= ARRAY_SIZE(resource_fb),
	.resource      	= resource_fb,
	.dev = {
		.dma_mask = &mtkfb_dmamask,
		.coherent_dma_mask = 0xffffffff,
	},
};
