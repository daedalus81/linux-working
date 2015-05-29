#ifndef _MT_IDLE_H
#define _MT_IDLE_H

extern void enable_dpidle_by_bit(int id);
extern void disable_dpidle_by_bit(int id);


#ifdef __MT_IDLE_C__

enum {
	IDLE_TYPE_SO = 0,
	IDLE_TYPE_DP = 1,
	IDLE_TYPE_SL = 2,
	IDLE_TYPE_RG = 3,
	NR_TYPES = 4,
};

enum {
	BY_CPU = 0,
	BY_CLK = 1,
	BY_TMR = 2,
	BY_OTH = 3,
	BY_VTG = 4,
	NR_REASONS = 5
};

/* idle task driver internal use only */

#if EN_PTP_OD
extern u32 ptp_data[3];
#endif

#ifdef SPM_SODI_ENABLED
extern u32 gSPM_SODI_EN;
extern bool gSpm_IsLcmVideoMode;
#endif

extern unsigned long localtimer_get_counter(void);
extern int localtimer_set_next_event(unsigned long evt);

#endif /* __MT_IDLE_C__ */


#endif
