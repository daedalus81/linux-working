#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/usb/musb.h>
#include <linux/musbfsh.h>

/* MTK USB Host */
static u64 usb_dmamask = DMA_BIT_MASK(32);
static struct musb_hdrc_config musb_config_mtk = {
	.multipoint     = true,
	.dyn_fifo       = true,
	.soft_con       = true,
	.dma            = true,
	.num_eps        = 16,
	.dma_channels   = 8,
};

#ifdef CONFIG_USB_MTK_OTG
static struct musb_hdrc_platform_data usb_data = {
	.mode           = MUSB_OTG,
	.config         = &musb_config_mtk,
};
#else
static struct musb_hdrc_platform_data usb_data = {
	.mode           = MUSB_PERIPHERAL,
	.config         = &musb_config_mtk,
};
#endif

struct platform_device mt_device_usb = {
	.name		  = "mt_usb",
	.id		  = -1,
	.dev = {
		.platform_data          = &usb_data,
		.dma_mask               = &usb_dmamask,
		.coherent_dma_mask      = DMA_BIT_MASK(32),
		//.release		= musbfsh_hcd_release,
	},
};

/* MTK USB11 Host */
#if defined(CONFIG_MTK_USBFSH)
static u64 usb11_dmamask = DMA_BIT_MASK(32);
static struct musbfsh_hdrc_config musbfsh_config_mtk = {
	.multipoint     = false,
	.dyn_fifo       = true,
	.soft_con       = true,
	.dma            = true,
	.num_eps        = 16,
	.dma_channels   = 8,
};

static struct musbfsh_hdrc_platform_data usb_data_mtk = {
	.mode           = 1,
	.config         = &musbfsh_config_mtk,
};

struct platform_device mt_device_usb11 = {
	.name           = "mt_usb11",
	.id             = -1,
	.dev = {
		.platform_data          = &usb_data_mtk,
		.dma_mask               = &usb11_dmamask,
		.coherent_dma_mask      = DMA_BIT_MASK(32),
		//.release		= musbfsh_hcd_release,
	},
};
#endif

#if defined(CONFIG_USB_MTK_ACM_TEMP)
struct platform_device usbacm_temp_device = {
	.name	  ="USB_ACM_Temp_Driver",
	.id		  = -1,
};
#endif

