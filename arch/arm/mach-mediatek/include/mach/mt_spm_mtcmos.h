#ifndef _MT_SPM_MTCMOS_
#define _MT_SPM_MTCMOS_

#include <linux/kernel.h>

#define STA_POWER_DOWN  0
#define STA_POWER_ON    1

/**************************************
 * for CPU MTCMOS
 **************************************/
/*
 * regiser bit difinition
 */
/* SPM_FC1_PWR_CON */
/* SPM_FC2_PWR_CON */
/* SPM_FC3_PWR_CON */
#define SRAM_ISOINT_B   (1U << 6)
#define SRAM_CKISO      (1U << 5)
#define PWR_CLK_DIS     (1U << 4)
#define PWR_ON_S        (1U << 3)
#define PWR_ON          (1U << 2)
#define PWR_ISO         (1U << 1)
#define PWR_RST_B       (1U << 0)

/* SPM_CPU_FC1_L1_PDN */
/* SPM_CPU_FC2_L1_PDN */
/* SPM_CPU_FC3_L1_PDN */
#define L1_PDN_ACK      (1U << 8)
#define L1_PDN          (1U << 0)

/* SPM_PWR_STATUS */
/* SPM_PWR_STATUS_S */
#define FC1             (1U << 11)
#define FC2             (1U << 10)
#define FC3             (1U <<  9)

/* SPM_SLEEP_TIMER_STA */
#define APMCU3_SLEEP    (1U << 18)
#define APMCU2_SLEEP    (1U << 17)
#define APMCU1_SLEEP    (1U << 16)

static DEFINE_SPINLOCK(spm_cpu_lock);

#define spm_mtcmos_cpu_lock(flags)   \
	do {    \
		spin_lock_irqsave(&spm_cpu_lock, flags);  \
	} while (0)

#define spm_mtcmos_cpu_unlock(flags) \
	do {    \
		spin_unlock_irqrestore(&spm_cpu_lock, flags);    \
	} while (0)


/**************************************
 * for non-CPU MTCMOS
 **************************************/
static DEFINE_SPINLOCK(spm_noncpu_lock);

#define spm_mtcmos_noncpu_lock(flags)   \
	do {    \
		spin_lock_irqsave(&spm_noncpu_lock, flags);  \
	} while (0)

#define spm_mtcmos_noncpu_unlock(flags) \
	do {    \
		spin_unlock_irqrestore(&spm_noncpu_lock, flags);    \
	} while (0)


#define IFR_MISC_PWR_STA_MASK    (0x1 << 17)
#define HIF_PWR_STA_MASK    (0x1 << 16)
#define ETH_PWR_STA_MASK    (0x1 << 15)
#define BDP_PWR_STA_MASK    (0x1 << 14)

#define VDE_PWR_STA_MASK    (0x1 << 7)
//#define VEN_PWR_STA_MASK    (0x1 << 7)
#define IFR_PWR_STA_MASK    (0x1 << 6)
#define ISP_PWR_STA_MASK    (0x1 << 5)
#define DIS_PWR_STA_MASK    (0x1 << 3)
#define MFG_PWR_STA_MASK    (0x1 << 4)
#define DPY_PWR_STA_MASK    (0x1 << 2)
#define CONN_PWR_STA_MASK    (0x1 << 1)
//#define MD2_PWR_STA_MASK    (0x1 << 1)
#define MD1_PWR_STA_MASK    (0x1 << 0)

#define SRAM_PDN            (0xf << 8)
//#define SRAM_ACK            (0xf << 12)

//#define SROM_PDN            (0xf << 16)
//#define SROM_ACK            (0xf << 20)

#define MD_SRAM_PDN         (0x1 << 8)

#define VDE_SRAM_ACK        (0x1 << 12)
//#define VEN_SRAM_ACK        (0xf << 12)
#define IFR_SRAM_ACK        (0xf << 12)
#define ISP_SRAM_ACK        (0x3 << 12)
#define DIS_SRAM_ACK        (0xf << 12)
#define MFG_SRAM_ACK        (0x1 << 12)

//#define TOPAXI_SI0_CTL      (INFRACFG_BASE + 0x0200)
//#define TOPAXI_PROT_EN      (INFRACFG_AO_BASE + 0x0220)
//#define TOPAXI_PROT_STA1    (INFRACFG_AO_BASE + 0x0228)

//#define MFG_PROT_MASK   0x0020
//#define MD1_PROT_MASK   0x5300
//#define MD2_PROT_MASK   0xAC00
#define MD1_PROT_MASK     0x00B8
#define CONN_PROT_MASK    0x0104
#define DISP_PROT_MASK    0x0002//0x0042

//#define MFG_SI0_MASK    0x0400

/*
 * 1. for CPU MTCMOS: CPU0, CPU1, CPU2, CPU3, DBG, CPUSYS
 * 2. call spm_mtcmos_cpu_lock/unlock() before/after any operations
 */
extern void spm_mtcmos_cpu_lock(unsigned long *flags);
extern void spm_mtcmos_cpu_unlock(unsigned long *flags);

extern int spm_mtcmos_ctrl_cpu0(int state, int chkWfiBeforePdn);
extern int spm_mtcmos_ctrl_cpu1(int state, int chkWfiBeforePdn);
extern int spm_mtcmos_ctrl_cpu2(int state, int chkWfiBeforePdn);
extern int spm_mtcmos_ctrl_cpu3(int state, int chkWfiBeforePdn);

extern int spm_mtcmos_ctrl_dbg(int state);
extern int spm_mtcmos_ctrl_cpusys(int state);

extern bool spm_cpusys_can_power_down(void);


/*
 * 1. for non-CPU MTCMOS: VDEC, VENC, ISP, DISP, MFG, INFRA, DDRPHY, MDSYS1, MDSYS2
 * 2. call spm_mtcmos_noncpu_lock/unlock() before/after any operations
 */
//extern void spm_mtcmos_noncpu_lock(unsigned long *flags);
//extern void spm_mtcmos_noncpu_unlock(unsigned long *flags);

extern int spm_mtcmos_ctrl_vdec(int state);
//extern int spm_mtcmos_ctrl_venc(int state);
extern int spm_mtcmos_ctrl_isp(int state);
extern int spm_mtcmos_ctrl_disp(int state);
extern int spm_mtcmos_ctrl_mfg(int state);

extern int spm_mtcmos_ctrl_infra(int state);
extern int spm_mtcmos_ctrl_ddrphy(int state);

extern int spm_mtcmos_ctrl_mdsys1(int state);
//extern int spm_mtcmos_ctrl_mdsys2(int state);
extern int spm_mtcmos_ctrl_connsys(int state);

extern int spm_mtcmos_ctrl_bdpsys(int state);
extern int spm_mtcmos_ctrl_ethsys(int state);
extern int spm_mtcmos_ctrl_hifsys(int state);

extern int spm_mtcmos_ctrl_ifrmiscsys(int state);


extern int test_spm_gpu_power_on(void);

#endif
