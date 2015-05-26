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

static struct resource mt_resource_i2c0[] = {
    {
        .start  = IO_VIRT_TO_PHYS(I2C0_BASE),
        .end    = IO_VIRT_TO_PHYS(I2C0_BASE) + 0x70,
        .flags  = IORESOURCE_MEM,
    },
    {
        .start  = MT_I2C0_IRQ_ID,
        .flags  = IORESOURCE_IRQ,
    },
};

static struct resource mt_resource_i2c1[] = {
    {
        .start  = IO_VIRT_TO_PHYS(I2C1_BASE),
        .end    = IO_VIRT_TO_PHYS(I2C1_BASE) + 0x70,
        .flags  = IORESOURCE_MEM,
    },
    {
        .start  = MT_I2C1_IRQ_ID,
        .flags  = IORESOURCE_IRQ,
    },
};

static struct resource mt_resource_i2c2[] = {
    {
        .start  = IO_VIRT_TO_PHYS(I2C2_BASE),
        .end    = IO_VIRT_TO_PHYS(I2C2_BASE) + 0x70,
        .flags  = IORESOURCE_MEM,
    },
    {
        .start  = MT_I2C2_IRQ_ID,
        .flags  = IORESOURCE_IRQ,
    },
};

struct platform_device mt_device_i2c0 = {
    {
        .name           = "mt-i2c",
        .id             = 0,
        .num_resources  = ARRAY_SIZE(mt_resource_i2c0),
        .resource       = mt_resource_i2c0,
    },
};

struct platform_device mt_device_i2c1 = {
    {
        .name           = "mt-i2c",
        .id             = 1,
        .num_resources  = ARRAY_SIZE(mt_resource_i2c1),
        .resource       = mt_resource_i2c1,
    },
};

struct platform_device mt_device_i2c2 = {
    {
        .name           = "mt-i2c",
        .id             = 2,
        .num_resources  = ARRAY_SIZE(mt_resource_i2c2),
        .resource       = mt_resource_i2c2,
    },
};

