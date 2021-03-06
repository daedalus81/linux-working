/*
 * This file is generated automatically according to the design of silicon.
 * Don't modify it directly.
 */

#ifndef __MT_REG_BASE
#define __MT_REG_BASE

#define SMI_MMU_TOP_BASE            0xF0205000
#define SMILARB2_BASE               0xF5001000

/* on-chip SRAM */
#define INTER_SRAM                  0xF9000000

/* infrasys */
//#define TOPRGU_BASE                 0xF0000000
#define INFRA_BASE                  0xF0000000
#define TOPCLKGEN_BASE				INFRA_BASE
#define INFRACFG_BASE               0xF0001000
#define INFRACFG_AO_BASE            0xF0001000
#define FPGA_PWRCTRL_BASE			0xF0001E84
#define FHCTL_BASE                  0xF0002000
#define PERICFG_BASE                0xF0003000
#define DRAMC0_BASE                 0xF0004000
#define DDRPHY_BASE                 0xF000F000
#define DRAMC_NAO_BASE              0xF020E000
#define GPIO_BASE                   0xF0005000
#define GPIO1_BASE                  0xF020C000
#define TOPSM_BASE                  0xF0006000
#define SPM_BASE                    0xF0006000
#define TOPRGU_BASE                 0xF0007000
#define AP_RGU_BASE                 TOPRGU_BASE
#define APMCU_GPTIMER_BASE          0xF0008000
#define HACC_BASE                   0xF000A000
#define AP_CIRQ_EINT                0xF000B000
#define SMI1_BASE                   0xF000C000
#define MIPI_CONFIG_BASE            0xF0010000
// APB Module LVDS ANA
#define LVDS_ANA_BASE (0xF0010400)


#define KP_BASE                     0xF0011000
#if 0
#define DEVICE_APC_0_BASE           0xF0010000
#define DEVICE_APC_1_BASE           0xF0011000
#define DEVICE_APC_2_BASE           0xF0012000
#define DEVICE_APC_3_BASE           0xF0013000
#define DEVICE_APC_4_BASE           0xF0014000
#define SMI0_BASE                   0xF0208000
#endif
#define EINT_BASE                   0xF000B000


#define DEBUGTOP_BASE               0xF0100000
#define MCUSYS_CFGREG_BASE          0xF0200000
#define SRAMROM_BASE                0xF0202000
#define EMI_BASE                    0xF0203000
#define EFUSEC_BASE                 0xF0206000
#define MCU_BIU_BASE                0xF0208000
#define APMIXED_BASE                0xF0209000
#define APMIXEDSYS_BASE             0xF0209000
#define AP_CCIF_BASE                0xF020A000
#define MD_CCIF_BASE                0xF020B000
#define INFRA_TOP_MBIST_CTRL_BASE   0xF020D000
#define DRAMC_NAO_BASE              0xF020E000
#define CORTEXA7MP_BASE             0xF0210000
#define GIC_CPU_BASE    (CORTEXA7MP_BASE + 0x2000)
#define GIC_DIST_BASE   (CORTEXA7MP_BASE + 0x1000)
//#define SMI_LARB_BASE             0xF0211000
//#define MCUSYS_AVS_BASE           0xF0212000

/* perisys */
/*avalaible*/
#define AP_DMA_BASE                 0xF1000000
#define AUXADC_BASE                 0xF1001000
#define UART1_BASE                  0xF1002000
#define UART2_BASE                  0xF1003000
#define UART3_BASE                  0xF1004000
#define UART4_BASE                  0xF1005000
#define PWM_BASE                    0xF1006000
#define I2C0_BASE                   0xF1007000
#define I2C1_BASE                   0xF1008000
#define I2C2_BASE                   0xF1009000
#define SPI0_BASE                   0xF100A000
#define BTIF_BASE                   0xF100C000
#define NFI_BASE                    0xF100D000
#define NFIECC_BASE                 0xF100E000
#define NLI_ARB_BASE                0xF100F000
#define I2C3_BASE                   0xF1010000 //FIXME 6582 take off
#define SPI1_BASE                   0xF100A000
#define THERMAL_BASE                0xF100B000

// APB Module pmic_wrap
#define PWRAP_BASE					0xF000D000

#if 0
//#define IRDA_BASE                 0xF1007000
#define I2C4_BASE                   0xF1014000
#define I2CDUAL_BASE                0xF1015000
#define ACCDET_BASE                 0xF1016000
#define AP_HIF_BASE                 0xF1017000
#define MD_HIF_BASE                 0xF1018000
#define GCPU_BASE                   0xF101B000
#define GCPU_NS_BASE                0xF01C000
#define GCPU_MMU_BASE               0xF01D000
#define SATA_BASE                   0xF01E000
#define CEC_BASE                    0xF01F000
//#define SPI1_BASE                 0xF1022000
#endif

#define USB1_BASE                   0xF1270000
#define USB2_BASE                   0xF1200000
#define USB_BASE                    0xF1200000
#define USB_SIF_BASE                0xF1210000
//#define USB3_BASE                 0xF1220000
#define MSDC_0_BASE                 0xF1230000
#define MSDC_1_BASE                 0xF1240000
#define MSDC_2_BASE                 0xF1250000
#define MSDC_3_BASE                 0xF12C0000
#define MSDC_4_BASE                 0xF1270000
//#define ETHERNET_BASE             0xF1290000

//#define ETB_BASE                  0xF0111000
//#define ETM_BASE                  0xF017C000


/* SMI common subsystem */
#define SYSRAM_BASE                 0xF2000000
#define AUDIO_REG_BASE              0xF2030000
#define MFG_AXI_BASE                0xF2060000
#define CONN_MCU_CONFIG_BASE		0xF8070000
#define AUDIO_BASE                  0xF1200000 //0xF2071000
#define MMSYS1_CONFIG_BASE          0xF2080000
#define SMI_LARB0_BASE              0xF2081000
// APB Module smi
#define SMI_BASE					0xF4011000
#define SMI_LARB1_BASE              0xF2082000
#define SMI_LARB2_BASE              0xF2083000
#define VDEC_GCON_BASE              0xF6000000 //0xF4000000
#define VDEC_BASE                   0xF4020000
#define VENC_TOP_BASE               0xF7000000
#define VENC_BASE                   0xF7002000
#define JPGENC_BASE                 0xF500A000
#define R_DMA0_BASE                 0xF2086000
#define R_DMA1_BASE                 0xF2087000
#define VDO_ROT0_BASE               0xF2088000
#define RGB_ROT0_BASE               0xF2089000
#define VDO_ROT1_BASE               0xF208A000
#define RGB_ROT1_BASE               0xF208B000
//#define DPI_BASE                    0xF208C000
#define BRZ_BASE                    0xF208D000
#define JPG_DMA_BASE                0xF208E000
#define OVL_DMA_BASE                0xF208F000
#define CSI2_BASE                   0xF2092000
#define CRZ_BASE                    0xF2093000
#define VRZ0_BASE                   0xF2094000
#define IMGPROC_BASE                0xF2095000
#define EIS_BASE                    0xF2096000
#define SPI_BASE                    0xF2097000
#define SCAM_BASE                   0xF2098000
#define PRZ0_BASE                   0xF2099000
#define PRZ1_BASE                   0xF209A000
#define JPG_CODEC_BASE              0xF209B000
//#define DSI_BASE                    0xF209C000
#define TVC_BASE                    0xF209D000
#define TVE_BASE                    0xF209E000
#define TV_ROT_BASE                 0xF209F000
#define RGB_ROT2_BASE               0xF20A0000
//#define LCD_BASE                    0xF20A1000
#define FD_BASE                     0xF20A2000
#define MIPI_CONFG_BASE             0xF20A3000
#define VRZ1_BASE                   0xF20A4000
#define MMSYS2_CONFG_BASE           0xF20C0000
#define SMI_LARB3_BASE              0xF20C1000
#define MFG_APB_BASE                0xF20C4000
#define G2D_BASE                    0xF20C6000

#define DISPSYS_BASE				0xF4000000
#define ROT_BASE					0xF4001000
#define SCL_BASE					0xF4002000
#define OVL_BASE					0xF4007000
#define WDMA0_BASE					0xF4009000
#define WDMA1_BASE					0xF4005000
#define RDMA0_BASE					0xF4008000
//#define RDMA1_BASE					0xF4007000
#define BLS_BASE					0xF400A000
//#define GAMMA_BASE					0xF400000
#define COLOR_BASE					0xF400B000
#define TDSHP_BASE					0xF4006000
#define LCD_BASE					0xF4012000	//only exists on FPGA
#define DSI_BASE					0xF400C000
#define DPI_BASE					0xF400D000

#define DPI1_BASE					0xF4014000

// LVDS TX
#define LVDS_TX_BASE				0xF4016200

#define SMILARB1_BASE				0xF4010000
#define DISP_MUTEX_BASE				0xF400E000
#define DISP_CMDQ_BASE				0xF400F000

/* imgsys */
#define IMGSYS_CONFG_BASE           0xF5000000
#define CAMINF_BASE                 IMGSYS_CONFG_BASE

/* G3DSYS */
#define G3D_CONFIG_BASE             0xF3000000
#define MALI_BASE                   0xF3040000

#define DEVINFO_BASE                0xF8000000

#endif
