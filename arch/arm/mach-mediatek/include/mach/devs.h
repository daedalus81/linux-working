#ifndef __DEVS_H__
#define __DEVS_H__

#include <linux/platform_device.h>
#include <mach/board.h>

#define CFG_DEV_UART1
#ifndef CONFIG_MT7623_FPGA
#define CFG_DEV_UART2
#define CFG_DEV_UART3
#define CFG_DEV_UART4
#endif

/* MTK platform devices */

extern struct platform_device *mtk_uart_device[];

extern struct platform_device mt_device_usb;
extern struct platform_device mt_device_usb11;
extern struct platform_device usbacm_temp_device;

extern struct platform_device mt_device_msdc0;
extern struct platform_device mt_device_msdc1;
extern struct platform_device mt_device_msdc2;
extern struct platform_device mt_device_msdc3;

extern struct platform_device mt_device_i2c0;
extern struct platform_device mt_device_i2c1;
extern struct platform_device mt_device_i2c2;

extern struct platform_device mt_spi_device;

extern struct platform_device mtk_device_fb;

extern struct platform_device audio_device0;
extern struct platform_device audio_device1;
extern struct platform_device *demo_audio_devices[];

extern struct platform_device mtk_nand_dev;

/*
 * Define constants.
 */

#define MTK_UART_SIZE	SZ_256
#define MTK_MSDC_SIZE	SZ_4K

/*
 * Define function prototype.
 */

extern int mt_board_init(void);

//extern unsigned int *get_modem_size_list(void);
//extern unsigned int get_nr_modem(void);

#endif  /* !__MT6575_DEVS_H__ */

