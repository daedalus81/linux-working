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

 /* Serial port registrations */
#define MTK_UART_NAME "mtk-uart"

static struct resource mt_uart1_resource[] = {
	[0] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(UART1_BASE), MTK_UART_SIZE),
	[1] = DEFINE_RES_IRQ(MT_UART1_IRQ_ID),
};

static struct resource mt_uart2_resource[] = {
	[0] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(UART2_BASE), MTK_UART_SIZE),
	[1] = DEFINE_RES_IRQ(MT_UART2_IRQ_ID),
};

static struct resource mt_uart3_resource[] = {
	[0] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(UART3_BASE), MTK_UART_SIZE),
	[1] = DEFINE_RES_IRQ(MT_UART3_IRQ_ID),
};

static struct resource mt_uart4_resource[] = {
	[0] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(UART4_BASE), MTK_UART_SIZE),
	[1] = DEFINE_RES_IRQ(MT_UART4_IRQ_ID),
};

struct platform_device mtk_uart_device[] __initdata = {
	[0] = {
		.name		= MTK_UART_NAME,
		.id		= 0,
		.resources	= mt_uart1_resource,
		.nr_resources	= ARRAY_SIZE(mt_uart1_resource),
	},
	[1] = {
		.name		= MTK_UART_NAME,
		.id		= 1,
		.resources	= mt_uart2_resource,
		.nr_resources	= ARRAY_SIZE(mt_uart2_resource),
	},
	[2] = {
		.name		= MTK_UART_NAME,
		.id		= 2,
		.resources	= mt_uart3_resource,
		.nr_resources	= ARRAY_SIZE(mt_uart3_resource),
	},
	[3] = {
		.name		= MTK_UART_NAME,
		.id		= 3,
		.resources	= mt_uart4_resource,
		.nr_resources	= ARRAY_SIZE(mt_uart4_resource),
	},
};
