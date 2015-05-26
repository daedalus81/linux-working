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

static struct resource mt_spi_resources[] = {
	[0] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(SPI1_BASE), 0x0028),
	[1] = DEFINE_RES_IRQ(MT_SPI1_IRQ_ID),
};

struct platform_device mt_spi_device = {
	.name	= "mt-spi",
	.num_resources	= ARRAY_SIZE(mt_spi_resources),
	.resource	= mt_spi_resources
};
