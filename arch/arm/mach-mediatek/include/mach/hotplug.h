#ifndef _HOTPLUG
#define _HOTPLUG

#include <linux/kernel.h>   //printk
#include <asm/atomic.h>
#include <mach/mt_reg_base.h>

//#define HOTPLUG_DEBUG

#define HOTPLUG_LOG_PRINT                               HOTPLUG_LOG_WITH_PRINTK

#if !defined(HOTPLUG_DEBUG)
#define HOTPLUG_INFO(fmt, args...)
#else
#define HOTPLUG_INFO(fmt, args...)                      printk("[Power/hotplug] "fmt, ##args)
#endif


/* profilling */
//#define CONFIG_HOTPLUG_PROFILING
#define CONFIG_HOTPLUG_PROFILING_COUNT                  100


/* register address */
#define BOOT_ADDR                                       (INFRACFG_AO_BASE + 0x800)


/* register read/write */
#define REG_READ(addr)           (*(volatile u32 *)(addr))
#define REG_WRITE(addr, value)   (*(volatile u32 *)(addr) = (u32)(value))


/* power on/off cpu*/
#define CONFIG_HOTPLUG_WITH_POWER_CTRL


/* global variable */
extern volatile int pen_release;
extern atomic_t hotplug_cpu_count;

#ifdef CONFIG_MTK_SCHED_TRACERS
DECLARE_PER_CPU(u64, last_event_ts);
#endif

#endif //enf of #ifndef _HOTPLUG
