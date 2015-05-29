#ifndef _MT_CPUFREQ_H
#define _MT_CPUFREQ_H

#include <linux/module.h>

/*********************
* Clock Mux Register
**********************/
#define TOP_CKMUXSEL    (0xF0001000)
#define TOP_CKDIV1_CPU  (0xF0001008)

/****************************
* PMIC Wrapper DVFS Register
*****************************/
#define PWRAP_BASE              (0xF000D000)
#define PMIC_WRAP_DVFS_ADR0     (PWRAP_BASE + 0xE4)
#define PMIC_WRAP_DVFS_WDATA0   (PWRAP_BASE + 0xE8)
#define PMIC_WRAP_DVFS_ADR1     (PWRAP_BASE + 0xEC)
#define PMIC_WRAP_DVFS_WDATA1   (PWRAP_BASE + 0xF0)
#define PMIC_WRAP_DVFS_ADR2     (PWRAP_BASE + 0xF4)
#define PMIC_WRAP_DVFS_WDATA2   (PWRAP_BASE + 0xF8)
#define PMIC_WRAP_DVFS_ADR3     (PWRAP_BASE + 0xFC)
#define PMIC_WRAP_DVFS_WDATA3   (PWRAP_BASE + 0x100)
#define PMIC_WRAP_DVFS_ADR4     (PWRAP_BASE + 0x104)
#define PMIC_WRAP_DVFS_WDATA4   (PWRAP_BASE + 0x108)
#define PMIC_WRAP_DVFS_ADR5     (PWRAP_BASE + 0x10C)
#define PMIC_WRAP_DVFS_WDATA5   (PWRAP_BASE + 0x110)
#define PMIC_WRAP_DVFS_ADR6     (PWRAP_BASE + 0x114)
#define PMIC_WRAP_DVFS_WDATA6   (PWRAP_BASE + 0x118)
#define PMIC_WRAP_DVFS_ADR7     (PWRAP_BASE + 0x11C)
#define PMIC_WRAP_DVFS_WDATA7   (PWRAP_BASE + 0x120)

#define DVFS_F0_1   (1690000)   // KHz
#define DVFS_F0_2   (1599000)   // KHz
#define DVFS_F0_3   (1508000)   // KHz
#define DVFS_F0_4   (1391000)   // KHz
#define DVFS_F0     (1300000)   // KHz
#define DVFS_F1_0   (1222000)   // KHz
#define DVFS_F1     (1196000)   // KHz
#define DVFS_F1_1   (1118000)   // KHz
#define DVFS_F2     (1040000)   // KHz
#define DVFS_F2_1   (1001000)   // KHz
#define DVFS_F3     ( 747500)   // KHz
#define DVFS_F4     ( 598000)   // KHz

#if defined(HQA_LV_1_09V)
    #define DVFS_V0     (1200)  // mV
    #define DVFS_V1     (1150)  // mV
    #define DVFS_V2     (1090)  // mV
    #define DVFS_V3     (1090)  // mV
#elif defined(HQA_NV_1_15V)
    #define DVFS_V0     (1260)  // mV
    #define DVFS_V1     (1200)  // mV
    #define DVFS_V2     (1150)  // mV
    #define DVFS_V3     (1050)  // mV /*Not used */
#elif defined(HQA_HV_1_21V)
    #define DVFS_V0     (1320)  // mV
    #define DVFS_V1     (1210)  // mV
    #define DVFS_V2     (1150)  // mV /*Not used */
    #define DVFS_V3     (1050)  // mV /*Not used */
#else /* Normal case */
    #define DVFS_V0     (1300)  // mV
    #define DVFS_V1     (1200)  // mV
    #ifdef CPUFREQ_SDIO_TRANSFER
    #define DVFS_V2_0   (1185)  // mV
    #endif
    #define DVFS_V2     (1150)  // mV
    #ifdef MT_DVFS_LOW_VOLTAGE_SUPPORT
    #define DVFS_V3     (1050)  // mV
    #endif
#endif

/*****************************************
* PMIC settle time, should not be changed
******************************************/
#define PMIC_SETTLE_TIME (40) // us

/*****************************************
* PLL settle time, should not be changed
******************************************/
#define PLL_SETTLE_TIME (30) // us

/***********************************************
* RMAP DOWN TIMES to postpone frequency degrade
************************************************/
#define RAMP_DOWN_TIMES (2)

/**********************************
* Available Clock Source for CPU
***********************************/
#define TOP_CKMUXSEL_CLKSQ   0x0
#define TOP_CKMUXSEL_ARMPLL  0x1
#define TOP_CKMUXSEL_MAINPLL 0x2
#define TOP_CKMUXSEL_UNIVPLL 0x3

/*****************
* extern function
******************/
extern int mt_cpufreq_state_set(int enabled);
extern void mt_cpufreq_thermal_protect(unsigned int limited_power);
void mt_cpufreq_enable_by_ptpod(void);
unsigned int mt_cpufreq_disable_by_ptpod(void);
extern unsigned int mt_cpufreq_max_frequency_by_DVS(unsigned int num);
void mt_cpufreq_return_default_DVS_by_ptpod(void);
extern bool mt_cpufreq_earlysuspend_status_get(void);


/*****************
* dummy type define for kernel 3.10
******************/
enum mt_cpu_dvfs_id {

#ifdef MTK_FORCE_CLUSTER1
	MT_CPU_DVFS_BIG,
	MT_CPU_DVFS_LITTLE,
#else
	MT_CPU_DVFS_LITTLE,
	MT_CPU_DVFS_BIG,
#endif

	NR_MT_CPU_DVFS,
};

#endif
