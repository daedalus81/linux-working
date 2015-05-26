#include <generated/autoconf.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/ioport.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/memblock.h>
#include <asm/setup.h>
#include <asm/mach/arch.h>
#include <linux/sysfs.h>
#include <asm/io.h>
#include <linux/spi/spi.h>
#include <linux/amba/bus.h>
#include <linux/amba/clcd.h>
#include <linux/musb/musb.h>
#include <linux/musbfsh.h>
#include "mach/memory.h"
#include "mach/irqs.h"
#include <mach/mt_reg_base.h>
#include <mach/devs.h>
#include <mach/mt_boot.h>
#include <linux/version.h>
#include <mach/mtk_ccci_helper.h>
#include <mach/mtk_eemcs_helper.h>
#include <mach/mtk_memcfg.h>
#include <mach/dfo_boot.h>
#include <mach/dfo_boot_default.h>
#include <mach/board.h>
#include <linux/aee.h>
#include <linux/mrdump.h>
#ifdef CONFIG_MTK_MTD_NAND
#include <mach/nand_device_list.h>
#endif

extern BOOTMODE get_boot_mode(void);
extern u32 get_devinfo_with_index(u32 index);
extern u32 g_devinfo_data[];
extern u32 g_devinfo_data_size;
extern void adjust_kernel_cmd_line_setting_for_console(char*, char*);
extern void show_sdk_patch_info(void);

unsigned int mtk_get_max_DRAM_size(void);
resource_size_t get_actual_DRAM_size(void);
#ifdef CONFIG_MTK_MTD_NAND
struct tag_pt_info otp_info;
struct tag_pt_info bmt_info;
unsigned int flash_number;
flashdev_info *gen_FlashTable_ptr;
flashdev_info gen_FlashTable_p[MAX_FLASH];
#endif

struct {
	u32 base;
	u32 size;
} bl_fb = {0, 0};

static int use_bl_fb = 0;
#define PT_ABTC_ATAG
#define ATAG_OTP_INFO       0x54430004
#define ATAG_BMT_INFO       0x54430005

static struct platform_device mtk_device_btif = {
	.name			= "mtk_btif",
	.id				= -1,
};

extern unsigned long max_pfn;


#if defined(CONFIG_MTK_FB)
char temp_command_line[1024] = {0};
extern unsigned int DISP_GetVRamSizeBoot(char* cmdline);
#define RESERVED_MEM_SIZE_FOR_FB (DISP_GetVRamSizeBoot((char*)&temp_command_line))
extern void   mtkfb_set_lcm_inited(bool isLcmInited);
#else
#define RESERVED_MEM_SIZE_FOR_FB (0x400000)
#endif


//FIX-ME: marked for early porting
#if defined(CONFIG_FIQ_DEBUGGER)
extern void fiq_uart_fixup(int uart_port);
extern struct platform_device mt_fiq_debugger;
#endif

struct platform_device gpio_dev =
{
	.name = "mt-gpio",
	.id   = -1,
};

struct platform_device fh_dev =
{
	.name = "mt-freqhopping",
	.id   = -1,
};


#ifdef MTK_MULTIBRIDGE_SUPPORT
static struct platform_device mtk_multibridge_dev = {
	.name = "multibridge",
	.id   = 0,
};
#endif
#ifdef CONFIG_MTK_HDMI_SUPPORT
static struct platform_device mtk_hdmi_dev = {
	.name = "hdmitx",
	.id   = 0,
};
#endif

#ifdef MTK_MT8193_SUPPORT
static struct platform_device mtk_ckgen_dev = {
	.name = "mt8193-ckgen",
	.id   = 0,
};
#endif


#if defined(CONFIG_MTK_ACCDET)
struct platform_device accdet_device = {
	.name	  ="Accdet_Driver",
	.id		  = -1,
	//.dev    ={
	//.release = accdet_dumy_release,
	//}
};
#endif

#ifdef CONFIG_MTK_KEYPAD
static struct platform_device kpd_pdev = {
	.name	= "mtk-kpd",
	.id	= -1,
};
#endif

#ifdef CONFIG_RFKILL
struct platform_device mt_rfkill_device = {
	.name   = "mt-rfkill",
	.id     = -1,
};
#endif

#if defined(CONFIG_KEYBOARD_HID)
static struct platform_device mt_hid_dev = {
	.name = "hid-keyboard",
	.id   = -1,
};
#endif

#if defined(MTK_WFD_SUPPORT)
static struct platform_device mt_uibc_dev = {
	.name = "uibc",
	.id   = -1,
};
#endif

static struct platform_device mtk_tpd_dev = {
	.name = "mtk-tpd",
	.id   = -1,
};

#if defined(CUSTOM_KERNEL_OFN)
static struct platform_device ofn_driver =
{
	.name = "mtofn",
	.id   = -1,
};
#endif

#ifdef CONFIG_CPU_FREQ
static struct platform_device cpufreq_pdev = {
	.name = "mt-cpufreq",
	.id   = -1,
};
#endif

struct platform_device thermal_pdev = {
	.name = "mtk-thermal",
	.id   = -1,
};

struct platform_device mtk_therm_mon_pdev = {
	.name = "mtk-therm-mon",
	.id   = -1,
};

struct platform_device ptp_pdev = {
	.name = "mt-ptp",
	.id   = -1,
};

struct platform_device spm_mcdi_pdev = {
	.name = "mtk-spm-mcdi",
	.id   = -1,
};

/*=======================================================================*/
/* MT6589 USIF-DUMCHAR                                                          */
/*=======================================================================*/

static struct platform_device dummychar_device =
{
	.name           = "dummy_char",
	.id             = 0,
};

/*=======================================================================*/
/* MASP                                                                  */
/*=======================================================================*/
static struct platform_device masp_device =
{
	.name           = "masp",
	.id             = -1,
};


static u64 mtk_smi_dmamask = ~(u32)0;
static struct platform_device mtk_smi_dev = {
	.name		  = "MTK_SMI",
	.id		  = 0,
	.dev              = {
		.dma_mask = &mtk_smi_dmamask,
		.coherent_dma_mask = 0xffffffffUL
	}
};

#if 0
static u64 mtk_m4u_dmamask = ~(u32)0;
static struct platform_device mtk_m4u_dev = {
	.name		  = "M4U_device",
	.id		  = 0,
	.dev              = {
		.dma_mask = &mtk_m4u_dmamask,
		.coherent_dma_mask = 0xffffffffUL
	}
};
#endif

/*=======================================================================*/
/* MT6573 GPS module                                                    */
/*=======================================================================*/
/* MT3326 GPS */
#ifdef CONFIG_MTK_GPS
struct platform_device mt3326_device_gps = {
	.name	       = "mt3326-gps",
	.id            = -1,
	.dev = {
		.platform_data = &mt3326_gps_hw,
	},
};
#endif

/*=======================================================================*/
/*=======================================================================*/
/* Commandline filter                                                    */
/* This function is used to filter undesired command passed from LK      */
/*=======================================================================*/
static void cmdline_filter(struct tag *cmdline_tag, char *default_cmdline)
{
	const char *undesired_cmds[] = {
		"console=",
		"root=",
		"lk_t=",
		"pl_t=",
	};

	int i;
	int ck_f = 0;
	char *cs,*ce;

	cs = cmdline_tag->u.cmdline.cmdline;
	ce = cs;
	while((__u32)ce < (__u32)tag_next(cmdline_tag)) {

		while(*cs == ' ' || *cs == '\0') {
			cs++;
			ce = cs;
		}

		if (*ce == ' ' || *ce == '\0') {
			for (i = 0; i < sizeof(undesired_cmds)/sizeof(char *); i++){
				if (memcmp(cs, undesired_cmds[i], strlen(undesired_cmds[i])) == 0) {
					ck_f = 1;
					break;
				}
			}

			if(ck_f == 0){
				*ce = '\0';
				//Append to the default command line
				strcat(default_cmdline, " ");
				strcat(default_cmdline, cs);
			}
			ck_f = 0;
			cs = ce + 1;
		}
		ce++;
	}
	if (strlen(default_cmdline) >= COMMAND_LINE_SIZE)
	{
		panic("Command line length is too long.\n\r");
	}
}
/*=======================================================================*/
/* Parse the framebuffer info						 */
/*=======================================================================*/
static int __init parse_tag_videofb_fixup(const struct tag *tags)
{
	bl_fb.base = tags->u.videolfb.lfb_base;
	bl_fb.size = tags->u.videolfb.lfb_size;
	use_bl_fb++;
	return 0;
}

static int __init parse_tag_devinfo_data_fixup(const struct tag *tags)
{
	int i=0;
	int size = tags->u.devinfo_data.devinfo_data_size;
	for (i=0;i<size;i++){
		g_devinfo_data[i] = tags->u.devinfo_data.devinfo_data[i];
	}

	/* print chip id for debugging purpose */
	printk("tag_devinfo_data_rid, index[%d]:0x%x\n", 12,g_devinfo_data[12]);
	printk("tag_devinfo_data size:%d\n", size);
	g_devinfo_data_size = size;
	return 0;
}

#ifdef CONFIG_MTK_MTD_NAND
int __init parse_tag_partition_fixup(const struct tag *tags)
{

	if(tags->hdr.tag == ATAG_OTP_INFO)
	{
		otp_info.size = tags->u.tag_pt_info.size;
		otp_info.start_address = tags->u.tag_pt_info.start_address;
	}
	if(tags->hdr.tag == ATAG_BMT_INFO)
	{
		bmt_info.size = tags->u.tag_pt_info.size;
		bmt_info.start_address = tags->u.tag_pt_info.start_address;
	}
	return 0;
}

int __init parse_tag_flashnum_fixup(const struct tag *tags)
{

	flash_number = tags->u.tag_nand_number.number;
	//gen_FlashTable_p = (flashdev_info*) kzalloc(flash_number*sizeof(flashdev_info), GFP_KERNEL);
	gen_FlashTable_ptr = gen_FlashTable_p;
	return 0;
}

int __init parse_tag_flash_fixup(const struct tag *tags)
{
	int* p = (int*)&tags->u.gen_FlashTable_p;
	int i;
	memcpy(gen_FlashTable_ptr, &tags->u.gen_FlashTable_p, sizeof(flashdev_info));
	gen_FlashTable_ptr++;
	return 0;
}
#endif

extern unsigned int mtkfb_parse_dfo_setting(void *dfo_tbl, int num);

resource_size_t bl_mem_sz = 0;
void mt_fixup(struct tag *tags, char **cmdline, struct meminfo *mi)
{
	struct tag *cmdline_tag = NULL;
	struct tag *reserved_mem_bank_tag = NULL;
	struct tag *none_tag = NULL;

	resource_size_t max_limit_size = CONFIG_MAX_DRAM_SIZE_SUPPORT -
		RESERVED_MEM_MODEM;
	resource_size_t avail_dram = 0;
	unsigned char md_inf_from_meta[4] = {0};
#ifdef MTK_TABLET_PLATFORM
	struct machine_desc *mdesc = NULL;
#endif

#if defined(CONFIG_MTK_FB)
	struct tag *temp_tags = tags;
	for (; temp_tags->hdr.size; temp_tags = tag_next(temp_tags))
	{
		if(temp_tags->hdr.tag == ATAG_CMDLINE)
			cmdline_filter(temp_tags, (char*)&temp_command_line);
	}
#endif

	show_sdk_patch_info();
	printk(KERN_ALERT"Load default dfo data...\n");

	parse_ccci_dfo_setting(&dfo_boot_default, DFO_BOOT_COUNT);
	parse_eemcs_dfo_setting(&dfo_boot_default, DFO_BOOT_COUNT);

	for (; tags->hdr.size; tags = tag_next(tags)) {
		if (tags->hdr.tag == ATAG_MEM) {
			bl_mem_sz += tags->u.mem.size;

			/*
			 * Modify the memory tag to limit available memory to
			 * CONFIG_MAX_DRAM_SIZE_SUPPORT
			 */
			if (max_limit_size > 0) {
				if (max_limit_size >= tags->u.mem.size) {
					max_limit_size -= tags->u.mem.size;
					avail_dram += tags->u.mem.size;
				} else {
					tags->u.mem.size = max_limit_size;
					avail_dram += max_limit_size;
					max_limit_size = 0;
				}
				// By Keene:
				// remove this check to avoid calcuate pmem size
				// before we know all dram size
				// Assuming the minimum size of memory bank is 256MB
				//if (tags->u.mem.size >= (TOTAL_RESERVED_MEM_SIZE)) {
				reserved_mem_bank_tag = tags;
				//}
			} else {
				tags->u.mem.size = 0;
			}
		}
		else if (tags->hdr.tag == ATAG_CMDLINE) {
			cmdline_tag = tags;
		} else if (tags->hdr.tag == ATAG_BOOT) {
			g_boot_mode = tags->u.boot.bootmode;
		} else if (tags->hdr.tag == ATAG_VIDEOLFB) {
			parse_tag_videofb_fixup(tags);
		}else if (tags->hdr.tag == ATAG_DEVINFO_DATA){
			parse_tag_devinfo_data_fixup(tags);
		}
		else if(tags->hdr.tag == ATAG_META_COM)
		{
			g_meta_com_type = tags->u.meta_com.meta_com_type;
			g_meta_com_id = tags->u.meta_com.meta_com_id;
		}
#ifdef CONFIG_MTK_MTD_NAND
		else if(tags->hdr.tag == ATAG_OTP_INFO)
		{
			parse_tag_partition_fixup(tags);
			printk("\n <bayi> Parse tag for partition	\n");
		}
		else if(tags->hdr.tag == ATAG_BMT_INFO)
		{
			parse_tag_partition_fixup(tags);
			printk("\n <bayi> Parse tag for BMT \n");
		}

		else if(tags->hdr.tag == ATAG_FLASH_NUMBER_INFO)
		{
			parse_tag_flashnum_fixup(tags);
			printk("\n <bayi> Parse tag flash number	\n");
		}
		else if(tags->hdr.tag == ATAG_FLASH_INFO)
		{
			parse_tag_flash_fixup(tags);
			printk("\n <bayi> Parse tag flash infomation	\n");
		}
#endif
		else if (tags->hdr.tag == ATAG_DFO_DATA) {
			parse_ccci_dfo_setting(&tags->u.dfo_data, DFO_BOOT_COUNT);
			parse_eemcs_dfo_setting(&tags->u.dfo_data, DFO_BOOT_COUNT);
#if defined(CONFIG_MTK_FB) && (1 == CONFIG_MTK_FB)
			mtkfb_parse_dfo_setting(&tags->u.dfo_data, DFO_BOOT_COUNT);
#endif
		}
		else if(tags->hdr.tag == ATAG_MDINFO_DATA) {
			printk(KERN_ALERT "Get MD inf from META\n");
			printk(KERN_ALERT "md_inf[0]=%d\n",tags->u.mdinfo_data.md_type[0]);
			printk(KERN_ALERT "md_inf[1]=%d\n",tags->u.mdinfo_data.md_type[1]);
			printk(KERN_ALERT "md_inf[2]=%d\n",tags->u.mdinfo_data.md_type[2]);
			printk(KERN_ALERT "md_inf[3]=%d\n",tags->u.mdinfo_data.md_type[3]);
			md_inf_from_meta[0]=tags->u.mdinfo_data.md_type[0];
			md_inf_from_meta[1]=tags->u.mdinfo_data.md_type[1];
			md_inf_from_meta[2]=tags->u.mdinfo_data.md_type[2];
			md_inf_from_meta[3]=tags->u.mdinfo_data.md_type[3];
		}
	}

#ifdef MTK_TABLET_PLATFORM
	for_each_machine_desc(mdesc)
		if (8127 == mdesc->nr)
			break;

	if (mdesc)
	{
		unsigned int i = get_devinfo_with_index(3);
		switch ((i >> 24) & 0xF)
		{
			case 0x0:
				break;
			case 0xC:
				break;
		}
	}
#endif

	if ((g_boot_mode == META_BOOT) || (g_boot_mode == ADVMETA_BOOT)) {
		/*
		 * Always use default dfo setting in META mode.
		 * We can fix abnormal dfo setting this way.
		 */
		printk(KERN_ALERT"(META mode) Load default dfo data...\n");
		parse_ccci_dfo_setting(&dfo_boot_default, DFO_BOOT_COUNT);
		parse_meta_md_setting(md_inf_from_meta);
		parse_eemcs_dfo_setting(&dfo_boot_default, DFO_BOOT_COUNT);
		parse_ext_meta_md_setting(md_inf_from_meta);
	}

	kernel_mem_sz = avail_dram; // keep the DRAM size (limited by CONFIG_MAX_DRAM_SIZE_SUPPORT)
	/*
	 * If the maximum memory size configured in kernel
	 * is smaller than the actual size (passed from BL)
	 * Still limit the maximum memory size but use the FB
	 * initialized by BL
	 */
	if (bl_mem_sz >= (CONFIG_MAX_DRAM_SIZE_SUPPORT - RESERVED_MEM_MODEM)) {
		use_bl_fb++;
	}

	if(tags->hdr.tag == ATAG_NONE)
		none_tag = tags;
	if (cmdline_tag != NULL) {
#ifdef CONFIG_FIQ_DEBUGGER
		char *console_ptr;
		int uart_port;
#endif
		char *br_ptr;
		// This function may modify ttyMT3 to ttyMT0 if needed
		adjust_kernel_cmd_line_setting_for_console(cmdline_tag->u.cmdline.cmdline, *cmdline);
#ifdef CONFIG_FIQ_DEBUGGER
		if ((console_ptr=strstr(*cmdline, "ttyMT")) != 0)
		{
			uart_port = console_ptr[5] - '0';
			if (uart_port > 3)
				uart_port = -1;

			fiq_uart_fixup(uart_port);
		}
#endif

		cmdline_filter(cmdline_tag, *cmdline);
		if ((br_ptr = strstr(*cmdline, "boot_reason=")) != 0) {
			/* get boot reason */
			g_boot_reason = br_ptr[12] - '0';
		}
		/* Use the default cmdline */
		memcpy((void*)cmdline_tag,
				(void*)tag_next(cmdline_tag),
				/* ATAG_NONE actual size */
				(uint32_t)(none_tag) - (uint32_t)(tag_next(cmdline_tag)) + 8);
	}
}

/*=======================================================================*/
/* DISP DEV                                                              */
/*=======================================================================*/
static u64 disp_dmamask = ~(u32)0;
static struct platform_device disp_device = {
	.name	 = "mtk_disp",
	.id      = 0,
	.dev     = {
		.dma_mask = &disp_dmamask,
		.coherent_dma_mask = 0xffffffff,
	},
	.num_resources = 0,
};


static struct platform_device mtk_leds_device = {
	.name = "leds-mtk",
	.id = -1
};
/*=======================================================================*/
/* NFC                                                                          */
/*=======================================================================*/
static struct platform_device mtk_nfc_6605_dev = {
	.name   = "mt6605",
	.id     = -1,
};

//#ifdef CONFIG_MTK_WIFI
/*=======================================================================*/
/* MT6572/82 WIFI module                                                 */
/*=======================================================================*/
struct platform_device mt_device_wifi = {
	.name	       = "mt-wifi",
	.id            = -1,
};
//#endif

/*=======================================================================*/
/* Unused Memory Allocation                                              */
/*=======================================================================*/
#ifdef MTK_USE_RESERVED_EXT_MEM
static struct platform_device mt_extmem = {
	.name           = "mt-extmem",
	.id             = 0,
};
#endif
/*=======================================================================*/
/* MT8127 Board Device Initialization                                    */
/* Sim switch driver                                                         */
/*=======================================================================*/
#if defined (CUSTOM_KERNEL_SSW)
static struct platform_device ssw_device = {
	.name = "sim-switch",
	.id = -1};
#endif

#define GPIO_MISC_CTRL_REG	(GPIO_BASE + 0x5B10)
void init_gpio_misc_ctrl(void)
{
	u32 tmp;
	tmp = __raw_readl(GPIO_MISC_CTRL_REG);
	tmp &= ~(1<<4);
	tmp |= (1<<4);
	__raw_writel(tmp, GPIO_MISC_CTRL_REG);
}

__init int mt_board_init(void)
{
	int i = 0, retval = 0;

#if defined(CONFIG_MTK_SERIAL)
	for (i = 0; i < ARRAY_SIZE(mtk_uart_device); i++){
		retval = platform_device_register(&mtk_uart_device[i]);
		printk("register uart device\n");
		if (retval != 0){
			return retval;
		}
	}
#endif


#ifdef CONFIG_FIQ_DEBUGGER
	retval = platform_device_register(&mt_fiq_debugger);
	if (retval != 0){
		return retval;
	}
#endif

#if defined(CONFIG_MTK_MTD_NAND)
	retval = platform_device_register(&mtk_nand_dev);
	if (retval != 0) {
		printk(KERN_ERR "register nand device fail\n");
		return retval;
	}
#endif

	retval = platform_device_register(&gpio_dev);
	if (retval != 0){
		return retval;
	}
	retval = platform_device_register(&fh_dev);
	if (retval != 0){
		return retval;
	}

#if defined(CONFIG_SND_SOC_MT8127)
	retval = platform_add_devices(mtk_soc_audio_devices, ARRAY_SIZE(mtk_soc_audio_devices));
	if (retval != 0){
		printk("platform_add_devices(mtk_soc_audio_devices) fail:%d\n", retval);
		return retval;
	}
#elif defined(CONFIG_SND_SOC_MT7623)
	printk("platform_add_devices(demo_audio_devices)\n");
	retval = platform_add_devices(demo_audio_devices, ARRAY_SIZE(demo_audio_devices));
	if (retval != 0){
		printk("platform_add_devices(demo_audio_devices) fail:%d\n", retval);
		return retval;
	}
#endif

#ifdef CONFIG_MTK_KEYPAD
	retval = platform_device_register(&kpd_pdev);
	if (retval != 0) {
		return retval;
	}
#endif


#if defined(CONFIG_KEYBOARD_HID)
	retval = platform_device_register(&mt_hid_dev);
	if (retval != 0){
		return retval;
	}
#endif

#if defined(MTK_WFD_SUPPORT)
	retval = platform_device_register(&mt_uibc_dev);
	if (retval != 0){
		return retval;
	}
#endif

#if defined(CONFIG_MTK_I2C)
	for (i = 0; i < ARRAY_SIZE(mt_device_i2c); i++){
		retval = platform_device_register(&mt_device_i2c[i]);
		if (retval != 0){
			return retval;
		}
	}
#endif

#if defined(CONFIG_MTK_MMC)
	for (i = 0; i < ARRAY_SIZE(mt_device_msdc); i++){
		retval = platform_device_register(&mt_device_msdc[i]);
		if (retval != 0){
			return retval;
		}
	}
#endif

#if defined(CONFIG_MTK_SOUND)
	retval = platform_device_register(&AudDrv_device);
	printk("AudDrv_driver_device \n!");
	if (retval != 0){
		return retval;
	}

	retval = platform_device_register(&AudDrv_device2);
	printk("AudioMTKBTCVSD AudDrv_device2 \n!");
	if (retval != 0){
		printk("AudioMTKBTCVSD AudDrv_device2 Fail:%d \n", retval);
		return retval;
	}
#endif


#ifdef MTK_MULTIBRIDGE_SUPPORT
	retval = platform_device_register(&mtk_multibridge_dev);
	printk("multibridge_driver_device \n!");
	if (retval != 0){
		return retval;
	}
#endif

	retval = platform_device_register(&mtk_device_btif);
	printk("mtk_device_btif register ret %d", retval);
	if (retval != 0){
		return retval;
	}

	//=====SMI/M4U devices===========
	printk("register MTK_SMI device\n");
	retval = platform_device_register(&mtk_smi_dev);
	if (retval != 0) {
		return retval;
	}


	//===========================

#ifdef MTK_MT8193_SUPPORT
	printk("register 8193_CKGEN device\n");
	retval = platform_device_register(&mtk_ckgen_dev);
	if (retval != 0){

		printk("register 8193_CKGEN device FAILS!\n");
		return retval;
	}
#endif
	//
	//=======================================================================
	// DISP DEV
	//=======================================================================

	retval = platform_device_register(&disp_device);
	if (retval != 0){
		return retval;
	}
#if defined(CONFIG_MTK_LEDS)
	retval = platform_device_register(&mtk_leds_device);
	if (retval != 0)
		return retval;
	printk("bei:device LEDS register\n");
#endif

#ifdef CONFIG_MTK_HDMI_SUPPORT
	retval = platform_device_register(&mtk_hdmi_dev);
	if (retval != 0){
		return retval;
	}
#endif


#if defined(CONFIG_MTK_SPI)
	platform_device_register(&mt_spi_device);
#endif

#if defined(CONFIG_MTK_ACCDET)


	retval = platform_device_register(&accdet_device);
	printk("register accdet device\n");

	if (retval != 0)
	{
		printk("platform_device_accdet_register error:(%d)\n", retval);
		return retval;
	}
	else
	{
		printk("platform_device_accdet_register done!\n");
	}

#endif

#if defined(CONFIG_USB_MTK_ACM_TEMP)

	retval = platform_device_register(&usbacm_temp_device);
	printk("register usbacm temp device\n");

	if (retval != 0)
	{
		printk("platform_device_usbacm_register error:(%d)\n", retval);
		return retval;
	}
	else
	{
		printk("platform_device_usbacm_register done!\n");
	}

#endif

#if defined(CONFIG_MTK_USBFSH)
	printk("register musbfsh device\n");
	retval = platform_device_register(&mt_device_usb11);
	if (retval != 0){
		printk("register musbfsh device fail!\n");
		return retval;
	}
#endif

#if defined(CONFIG_USB_MTK_HDRC)
	printk("mt_device_usb register\n");
	retval = platform_device_register(&mt_device_usb);
	if (retval != 0){
		printk("mt_device_usb register fail\n");
		return retval;
	}
#endif

#if defined(CONFIG_MTK_TOUCHPANEL)
	retval = platform_device_register(&mtk_tpd_dev);
	if (retval != 0) {
		return retval;
	}
#endif

#if defined(CUSTOM_KERNEL_OFN)
	retval = platform_device_register(&ofn_driver);
	if (retval != 0){
		return retval;
	}
#endif

#if (defined(CONFIG_MTK_MTD_NAND) || defined(CONFIG_MTK_MMC))
	retval = platform_device_register(&dummychar_device);
	if (retval != 0){
		return retval;
	}
#endif


	retval = platform_device_register(&thermal_pdev);
	if (retval != 0) {
		return retval;
	}

	retval = platform_device_register(&mtk_therm_mon_pdev);
	if (retval != 0) {
		return retval;
	}

	retval = platform_device_register(&ptp_pdev);
	if (retval != 0) {
		return retval;
	}

	retval = platform_device_register(&spm_mcdi_pdev);
	if (retval != 0) {
		return retval;
	}

#ifdef CONFIG_RFKILL
	retval = platform_device_register(&mt_rfkill_device);
	if (retval != 0){
		return retval;
	}
#endif

#if defined(CONFIG_MTK_GPS)
	retval = platform_device_register(&mt3326_device_gps);
	if (retval != 0){
		return retval;
	}
#endif

	retval = platform_device_register(&mtk_nfc_6605_dev);
	printk("mtk_nfc_6605_dev register ret %d", retval);
	if (retval != 0){
		return retval;
	}

	retval = platform_device_register(&mt_device_wifi);
	if (retval != 0){
		return retval;
	}

#if defined (CUSTOM_KERNEL_SSW)
	retval = platform_device_register(&ssw_device);
	if (retval != 0) {
		return retval;
	}
#endif

#ifdef CONFIG_MTK_USE_RESERVED_EXT_MEM
	retval = platform_device_register(&mt_extmem);

	printk("%s[%d] ret: %d\n", __FILE__, __LINE__, retval);
	if (retval != 0){
		return retval;
	}
#endif

	retval = platform_device_register(&masp_device);
	if (retval != 0){
		return retval;
	}

	// Disable binding control
	// for GPIO22/EINT0
	init_gpio_misc_ctrl();

	return 0;
}

// return the actual physical DRAM size
unsigned int mtk_get_max_DRAM_size(void)
{
	return kernel_mem_sz + RESERVED_MEM_MODEM;
}

resource_size_t get_actual_DRAM_size(void)
{
	return bl_mem_sz;
}
EXPORT_SYMBOL(get_actual_DRAM_size);

unsigned int get_phys_offset(void)
{
	return PHYS_OFFSET;
}
EXPORT_SYMBOL(get_phys_offset);

#include <asm/sections.h>
void get_text_region (unsigned int *s, unsigned int *e)
{
	*s = (unsigned int)_text, *e=(unsigned int)_etext ;
}
EXPORT_SYMBOL(get_text_region) ;

void __weak mtk_wcn_consys_memory_reserve(void)
{
	printk(KERN_ERR"weak reserve function: %s", __FUNCTION__);
}

void __weak eemcs_memory_reserve(void)
{
	printk(KERN_ERR"calling weak function %s\n", __FUNCTION__);
}

void mt_reserve(void)
{
	//    aee_dram_console_reserve_memory();
	mrdump_reserve_memory();

#if defined(CONFIG_MTK_RAM_CONSOLE_USING_DRAM)
	memblock_reserve(CONFIG_MTK_RAM_CONSOLE_DRAM_ADDR, CONFIG_MTK_RAM_CONSOLE_DRAM_SIZE);
#endif

	/*
	 * Dynamic reserved memory (by arm_memblock_steal)
	 *
	 * *** DO NOT CHANGE THE RESERVE ORDER ***
	 *
	 * New memory reserve functions should be APPENDED to old funtions
	 */
	mtk_wcn_consys_memory_reserve();
	ccci_md_mem_reserve();
#if defined(CONFIG_MTK_EEMCS_DEVICES)
	eemcs_memory_reserve();
#endif
	/* Last line of dynamic reserve functions */
}
