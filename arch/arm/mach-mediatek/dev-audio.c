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

static u64 aud_dmamask = 0xffffffffUL;
struct platform_device audio_device0 = {
	.name  = "audio_device",
	.id    = 0,
	.dev   = {
		.dma_mask = &aud_dmamask,
		.coherent_dma_mask =  0xffffffffUL
	}
};

static u64 aud_btcvsd_dmamask = 0xffffffffUL;
struct platform_device audio_device1 = {
	.name  = "audio_mtk_btcvsd",
	.id    = 0,
	.dev   = {
		.dma_mask = &aud_btcvsd_dmamask,
		.coherent_dma_mask =  0xffffffffUL
	}
};

static struct platform_device audio_device2 = {
	.name = "mt7623-audio",
	.id = PLATFORM_DEVID_NONE,
};

static struct platform_device mt_dai_device = {
	.name = "mt7623-dai",
	.id = PLATFORM_DEVID_NONE,
};

static struct platform_device demo_machine_device = {
	.name = "demo-machine",
	.id = PLATFORM_DEVID_NONE,
	.dev = {
		//.platform_data = &audio_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
};

static struct platform_device dummy_codec_device = {
	.name = "dummy-codec",
	.id = PLATFORM_DEVID_NONE,
};

struct platform_device *demo_audio_devices[] __initdata = {
	&audio_device2,
	&mt_dai_device,
	&dummy_codec_device,
	&demo_machine_device
};

