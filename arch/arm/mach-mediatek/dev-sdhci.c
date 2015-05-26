#include <linux/device.h>
#include <linux/platform_device.h>

#include <mach/devs.h>
#include <mach/mt_reg_base.h>

#include <linux/mtd/nand.h>
#include <linux/platform-data/mtk_msdc.h>


#if defined(CFG_DEV_MSDC0)
static struct resource mt_resource_msdc0[] = {
	[0] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(MSDC_0_BASE), MTK_MSDC_SIZE),
	/* FPGA Power control */
	[1] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(FPGA_PWRCTRL_BASE), SZ_8),
	/* EFUSE control */
	[2] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(EFUSEC_BASE), MTK_EFUSE_SIZE),
	/* GPIO control */
	[3] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(GPIO_BASE), SZ_4K),
	/* TOPCKGEN control */
	[4] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(TOPCLKGEN_BASE), SZ_4K),
	[5] = DEFINE_RES_IRQ(MT_MSDC0_IRQ_ID),
};
#endif

#if defined(CFG_DEV_MSDC1)
static struct resource mt_resource_msdc1[] = {
	[0] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(MSDC_1_BASE), MTK_MSDC_SIZE),
	/* FPGA Power control */
	[1] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(FPGA_PWRCTRL_BASE), SZ_8),
	/* GPIO control */
	[2] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(GPIO_BASE), SZ_4K),
	/* TOPCKGEN control */
	[3] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(TOPCLKGEN_BASE), SZ_4K),
	[4] = DEFINE_RES_IRQ(MT_MSDC1_IRQ_ID),
};
#endif

#if defined(CFG_DEV_MSDC2)
static struct resource mt_resource_msdc2[] = {
	[0] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(MSDC_2_BASE), MTK_MSDC_SIZE),
	/* FPGA Power control */
	[1] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(FPGA_PWRCTRL_BASE), SZ_8),
	/* GPIO control */
	[2] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(GPIO_BASE), SZ_4K),
	/* TOPCKGEN control */
	[3] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(TOPCLKGEN_BASE), SZ_4K),
	[4] = DEFINE_RES_IRQ(MT_MSDC2_IRQ_ID),
};
#endif

#if defined(CFG_DEV_MSDC3)
static struct resource mt_resource_msdc3[] = {
	[0] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(MSDC_3_BASE), MTK_MSDC_SIZE),
	/* FPGA Power control */
	[1] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(FPGA_PWRCTRL_BASE), SZ_8),
	/* EFUSE control */
	[2] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(EFUSEC_BASE), SZ_4),
	/* GPIO control */
	[3] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(GPIO_BASE), SZ_4K),
	/* TOPCKGEN control */
	[4] = DEFINE_RES_MEM(IO_VIRT_TO_PHYS(TOPCLKGEN_BASE), SZ_4K),
	[5] = DEFINE_RES_IRQ(MT_MSDC3_IRQ_ID),
};
#endif

#define MSDC_SDCARD_FLAG  (MSDC_SYS_SUSPEND | MSDC_WP_PIN_EN | MSDC_CD_PIN_EN | MSDC_REMOVABLE | MSDC_HIGHSPEED)
#define MSDC_SDIO_FLAG    (MSDC_EXT_SDIO_IRQ | MSDC_HIGHSPEED)

#if defined(CFG_DEV_MSDC0)
#if defined(CONFIG_MTK_WCN_CMB_SDIO_SLOT) && (CONFIG_MTK_WCN_CMB_SDIO_SLOT == 0)
static struct msdc_hw msdc0_hw = {
	.clk_src        = MSDC_CLKSRC_200MHZ,
	.cmd_edge       = MSDC_SMPL_FALLING,
	.rdata_edge     = MSDC_SMPL_FALLING,
	.wdata_edge     = MSDC_SMPL_FALLING,
	.clk_drv        = 0,
	.cmd_drv        = 0,
	.dat_drv        = 0,
	.data_pins      = 4,
	.data_offset    = 0,
	//MT6620 use External IRQ, wifi uses high speed. here wifi manage his own suspend and resume, does not support hot plug
	.flags          = MSDC_SDIO_FLAG,//MSDC_SYS_SUSPEND | MSDC_WP_PIN_EN | MSDC_CD_PIN_EN | MSDC_REMOVABLE,(this flag is for SD card)
	.dat0rddly	= 0,
	.dat1rddly	= 0,
	.dat2rddly	= 0,
	.dat3rddly	= 0,
	.dat4rddly	= 0,
	.dat5rddly	= 0,
	.dat6rddly	= 0,
	.dat7rddly	= 0,
	.datwrddly	= 0,
	.cmdrrddly	= 0,
	.cmdrddly	= 0,
	.host_function	= MSDC_SDIO,
	.boot		= 0,
	.request_sdio_eirq = mtk_wcn_cmb_sdio_request_eirq,
	.enable_sdio_eirq  = mtk_wcn_cmb_sdio_enable_eirq,
	.disable_sdio_eirq = mtk_wcn_cmb_sdio_disable_eirq,
	.register_pm       = mtk_wcn_cmb_sdio_register_pm,
};
#else
static struct msdc_hw msdc0_hw = {
	.clk_src        = MSDC_CLKSRC_200MHZ,
	.cmd_edge       = MSDC_SMPL_FALLING,
	.rdata_edge 	= MSDC_SMPL_FALLING,
	.wdata_edge 	= MSDC_SMPL_FALLING,
	.clk_drv        = 4,
	.cmd_drv        = 2,
	.dat_drv        = 2,
	.data_pins      = 8,
	.data_offset    = 0,
	.flags          = MSDC_SYS_SUSPEND | MSDC_HIGHSPEED /*| MSDC_UHS1 | MSDC_DDR*/,
	.dat0rddly	= 0xa,
	.dat1rddly	= 0,
	.dat2rddly	= 0,
	.dat3rddly	= 0,
	.dat4rddly	= 0,
	.dat5rddly	= 0,
	.dat6rddly	= 0,
	.dat7rddly	= 0,
	.datwrddly	= 0,
	.cmdrrddly	= 0,
	.cmdrddly	= 0,
	.host_function	= MSDC_EMMC,
	.boot		= MSDC_BOOT_EN,
};
#endif
#endif

#if defined(CFG_DEV_MSDC1)
#if defined(CONFIG_MTK_WCN_CMB_SDIO_SLOT) && (CONFIG_MTK_WCN_CMB_SDIO_SLOT == 1)
static struct msdc_hw msdc1_hw = {
	.clk_src        = MSDC_CLKSRC_200MHZ,
	.cmd_edge       = MSDC_SMPL_FALLING,
	.rdata_edge     = MSDC_SMPL_FALLING,
	.wdata_edge     = MSDC_SMPL_FALLING,
	.clk_drv        = 4,
	.cmd_drv        = 3,
	.dat_drv        = 3,
	.clk_drv_sd_18	= 4,               /* sdr104 mode */
	.cmd_drv_sd_18	= 4,
	.dat_drv_sd_18	= 4,
	.clk_drv_sd_18_sdr50	= 4,       /* sdr50 mode */
	.cmd_drv_sd_18_sdr50	= 4,
	.dat_drv_sd_18_sdr50	= 4,
	.clk_drv_sd_18_ddr50	= 4,       /* ddr50 mode */
	.cmd_drv_sd_18_ddr50	= 4,
	.dat_drv_sd_18_ddr50	= 4,
	.data_pins      = 4,
	.data_offset    = 0,
	.flags          = MSDC_SDIO_FLAG,
	.dat0rddly	= 0,
	.dat1rddly	= 0,
	.dat2rddly	= 0,
	.dat3rddly	= 0,
	.dat4rddly	= 0,
	.dat5rddly	= 0,
	.dat6rddly	= 0,
	.dat7rddly	= 0,
	.datwrddly	= 0,
	.cmdrrddly	= 0,
	.cmdrddly       = 0,
	.host_function	= MSDC_SDIO,
	.boot           = 0,
	.cd_level       = MSDC_CD_LOW,

};
#else
static struct msdc_hw msdc1_hw = {
	.clk_src        = MSDC_CLKSRC_200MHZ,
	.cmd_edge       = MSDC_SMPL_FALLING,
	.rdata_edge     = MSDC_SMPL_FALLING,
	.wdata_edge     = MSDC_SMPL_FALLING,
	.clk_drv        = 6,
	.cmd_drv        = 6,
	.dat_drv        = 6,
	.clk_drv_sd_18	= 3,               /* sdr104 mode */
	.cmd_drv_sd_18	= 3,
	.dat_drv_sd_18	= 2,
	.clk_drv_sd_18_sdr50	= 3,       /* sdr50 mode */
	.cmd_drv_sd_18_sdr50	= 3,
	.dat_drv_sd_18_sdr50	= 2,
	.clk_drv_sd_18_ddr50	= 3,       /* ddr50 mode */
	.cmd_drv_sd_18_ddr50	= 3,
	.dat_drv_sd_18_ddr50	= 2,
	.data_pins      = 4,
	.data_offset    = 0,
#ifdef CUST_EINT_MSDC1_INS_NUM
	.flags          = MSDC_SYS_SUSPEND | MSDC_WP_PIN_EN  |   MSDC_CD_PIN_EN |   MSDC_REMOVABLE | MSDC_HIGHSPEED /*| MSDC_UHS1 |MSDC_DDR |MSDC_SD_NEED_POWER*/,

#else
	.flags          = MSDC_SYS_SUSPEND | MSDC_WP_PIN_EN | MSDC_HIGHSPEED /*| MSDC_UHS1 |MSDC_DDR*/,
#endif
	.dat0rddly	= 0,
	.dat1rddly	= 0,
	.dat2rddly	= 0,
	.dat3rddly	= 0,
	.dat4rddly	= 0,
	.dat5rddly	= 0,
	.dat6rddly	= 0,
	.dat7rddly	= 0,
	.datwrddly	= 0,
	.cmdrrddly	= 0,
	.cmdrddly	= 0,
	.host_function	= MSDC_SD,
	.boot		= 0,
	.cd_level	= MSDC_CD_LOW,
};
#endif
#endif

#if defined(CFG_DEV_MSDC2)
#if defined(CONFIG_MTK_WCN_CMB_SDIO_SLOT) && (CONFIG_MTK_WCN_CMB_SDIO_SLOT == 2)
/* MSDC2 settings for MT66xx combo connectivity chip */
static struct msdc_hw msdc2_hw = {
	.clk_src        = MSDC_CLKSRC_200MHZ,
	.cmd_edge       = MSDC_SMPL_FALLING,
	.rdata_edge     = MSDC_SMPL_FALLING,
	.wdata_edge     = MSDC_SMPL_FALLING,
	.clk_drv        = 0,
	.cmd_drv        = 0,
	.dat_drv        = 0,
	.data_pins      = 4,
	.data_offset    = 0,
	//MT6620 use External IRQ, wifi uses high speed. here wifi manage his own suspend and resume, does not support hot plug
	.flags          = MSDC_SDIO_FLAG,//MSDC_SYS_SUSPEND | MSDC_WP_PIN_EN | MSDC_CD_PIN_EN | MSDC_REMOVABLE,
	.dat0rddly	= 0,
	.dat1rddly	= 0,
	.dat2rddly	= 0,
	.dat3rddly	= 0,
	.dat4rddly	= 0,
	.dat5rddly	= 0,
	.dat6rddly	= 0,
	.dat7rddly	= 0,
	.datwrddly	= 0,
	.cmdrrddly	= 0,
	.cmdrddly	= 0,
	.host_function	= MSDC_SDIO,
	.boot		= 0,
	.request_sdio_eirq = mtk_wcn_cmb_sdio_request_eirq,
	.enable_sdio_eirq  = mtk_wcn_cmb_sdio_enable_eirq,
	.disable_sdio_eirq = mtk_wcn_cmb_sdio_disable_eirq,
	.register_pm       = mtk_wcn_cmb_sdio_register_pm,
};
#endif
#endif

#if defined(CFG_DEV_MSDC3)
#if defined(CONFIG_MTK_WCN_CMB_SDIO_SLOT) && (CONFIG_MTK_WCN_CMB_SDIO_SLOT == 3)
/* MSDC3 settings for MT66xx combo connectivity chip */
static struct msdc_hw msdc3_hw = {
	.clk_src        = MSDC_CLKSRC_200MHZ,
	.cmd_edge       = MSDC_SMPL_FALLING,
	.rdata_edge     = MSDC_SMPL_FALLING,
	.wdata_edge     = MSDC_SMPL_FALLING,
	.clk_drv        = 0,
	.cmd_drv        = 0,
	.dat_drv        = 0,
	.data_pins      = 4,
	.data_offset    = 0,
	//MT6620 use External IRQ, wifi uses high speed. here wifi manage his own suspend and resume, does not support hot plug
	.flags          = MSDC_SDIO_FLAG,//MSDC_SYS_SUSPEND | MSDC_WP_PIN_EN | MSDC_CD_PIN_EN | MSDC_REMOVABLE,
	.dat0rddly	= 0,
	.dat1rddly	= 0,
	.dat2rddly	= 0,
	.dat3rddly	= 0,
	.dat4rddly	= 0,
	.dat5rddly	= 0,
	.dat6rddly	= 0,
	.dat7rddly	= 0,
	.datwrddly	= 0,
	.cmdrrddly	= 0,
	.cmdrddly	= 0,
	.host_function	= MSDC_SDIO,
	.boot		= 0,
	.request_sdio_eirq = mtk_wcn_cmb_sdio_request_eirq,
	.enable_sdio_eirq  = mtk_wcn_cmb_sdio_enable_eirq,
	.disable_sdio_eirq = mtk_wcn_cmb_sdio_disable_eirq,
	.register_pm       = mtk_wcn_cmb_sdio_register_pm,
};
#endif
#endif

#if defined(CFG_DEV_MSDC4)
#if defined(CONFIG_MTK_WCN_CMB_SDIO_SLOT) && (CONFIG_MTK_WCN_CMB_SDIO_SLOT == 4)
static struct msdc_hw msdc4_hw = {
	.clk_src        = MSDC_CLKSRC_200MHZ,
	.cmd_edge       = MSDC_SMPL_FALLING,
	.rdata_edge     = MSDC_SMPL_FALLING,
	.wdata_edge     = MSDC_SMPL_FALLING,
	.clk_drv        = 0,
	.cmd_drv        = 0,
	.dat_drv        = 0,
	.data_pins      = 4,
	.data_offset    = 0,
	//MT6620 use External IRQ, wifi uses high speed. here wifi manage his own suspend and resume, does not support hot plug
	.flags          = MSDC_SDIO_FLAG,//MSDC_SYS_SUSPEND | MSDC_WP_PIN_EN | MSDC_CD_PIN_EN | MSDC_REMOVABLE,(this flag is for SD card)
	.dat0rddly	= 0,
	.dat1rddly	= 0,
	.dat2rddly	= 0,
	.dat3rddly	= 0,
	.dat4rddly	= 0,
	.dat5rddly	= 0,
	.dat6rddly	= 0,
	.dat7rddly	= 0,
	.datwrddly	= 0,
	.cmdrrddly	= 0,
	.cmdrddly	= 0,
	.host_function	= MSDC_SDIO,
	.boot		= 0,
	.request_sdio_eirq = mtk_wcn_cmb_sdio_request_eirq,
	.enable_sdio_eirq  = mtk_wcn_cmb_sdio_enable_eirq,
	.disable_sdio_eirq = mtk_wcn_cmb_sdio_disable_eirq,
	.register_pm       = mtk_wcn_cmb_sdio_register_pm,
};
#else
static struct msdc_hw msdc4_hw = {
	.clk_src        = MSDC_CLKSRC_200MHZ,
	.cmd_edge       = MSDC_SMPL_FALLING,
	.rdata_edge     = MSDC_SMPL_FALLING,
	.wdata_edge     = MSDC_SMPL_FALLING,
	.clk_drv        = 1,
	.cmd_drv        = 0,
	.dat_drv        = 0,
	.data_pins      = 8,
	.data_offset    = 0,
	.flags          = MSDC_SYS_SUSPEND | MSDC_HIGHSPEED | MSDC_UHS1 |MSDC_DDR,
	.dat0rddly	= 0,
	.dat1rddly	= 0,
	.dat2rddly	= 0,
	.dat3rddly	= 0,
	.dat4rddly	= 0,
	.dat5rddly	= 0,
	.dat6rddly	= 0,
	.dat7rddly	= 0,
	.datwrddly	= 0,
	.cmdrrddly	= 0,
	.cmdrddly	= 0,
	.host_function	= MSDC_EMMC,
	.boot		= MSDC_BOOT_EN,
};
#endif
#endif

struct platform_device mt_device_msdc0 = {
    {
        .name           = "mtk-msdc",
        .id             = 0,
        .num_resources  = ARRAY_SIZE(mt_resource_msdc0),
        .resource       = mt_resource_msdc0,
        .dev = {
            .platform_data = &msdc0_hw,
        },
    },
};

struct platform_device mt_device_msdc1 = {
    {
        .name           = "mtk-msdc",
        .id             = 1,
        .num_resources  = ARRAY_SIZE(mt_resource_msdc1),
        .resource       = mt_resource_msdc1,
        .dev = {
            .platform_data = &msdc1_hw,
        },
    },
};

struct platform_device mt_device_msdc2 = {
    {
        .name           = "mtk-msdc",
        .id             = 2,
        .num_resources  = ARRAY_SIZE(mt_resource_msdc2),
        .resource       = mt_resource_msdc2,
        .dev = {
            .platform_data = &msdc2_hw,
        },
    },
};

struct platform_device mt_device_msdc3 = {
    {
        .name           = "mtk-msdc",
        .id             = 3,
        .num_resources  = ARRAY_SIZE(mt_resource_msdc3),
        .resource       = mt_resource_msdc3,
        .dev = {
            .platform_data = &msdc3_hw,
        },
    },
};

/* MT6575 NAND Driver */
#if defined(CONFIG_MTK_MTD_NAND)
struct mtk_nand_host_hw mtk_nand_hw = {
    	.nfi_bus_width          = 8,
	.nfi_access_timing	= NFI_DEFAULT_ACCESS_TIMING,
	.nfi_cs_num		= NFI_CS_NUM,
	.nand_sec_size		= 512,
	.nand_sec_shift		= 9,
	.nand_ecc_size		= 2048,
	.nand_ecc_bytes		= 32,
	.nand_ecc_mode		= NAND_ECC_HW,
};
#endif

