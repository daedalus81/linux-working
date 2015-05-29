#ifndef _MT_DORMANT_
#define _MT_DORMANT_

#define MAX_CPUS 1

/*
 * Power status
 */
#define STATUS_RUN      0
#define STATUS_STANDBY  1
#define STATUS_DORMANT  2
#define STATUS_SHUTDOWN 3
#define DORMANT_MODE  STATUS_DORMANT
#define SHUTDOWN_MODE STATUS_SHUTDOWN

#define BOOTROM_PWR_CTRL        (INFRACFG_AO_BASE + 0x804)
#define BOOTROM_BOOT_ADDR       (INFRACFG_AO_BASE + 0x800)
#define NS_SLAVE_BOOT_ADDR      (BOOT_SHARE_BASE + 1012)

#define CA7_CACHE_CONFIG        (MCUSYS_CFGREG_BASE + 0x0000)

#define reg_read(addr)          (*(volatile u32 *)(addr))
#define reg_write(addr, val)    mt65xx_reg_sync_writel(val, addr)

/* Timer Bits */
#define HYP_TIMER_MULT  0xa	/* 12Mhz * 10 i.e. interrupt every 10ms. Linux uses 12MHz * 10 */
#define LCL_TIMER_FREQ  0x7f	/* Every 128th timer acts as a trigger */
#define HYP_TIMER_IRQ   0x1a
#define LCL_TIMER_IRQ   0x1e
#define TIMER_ENABLE    0x1
#define TIMER_DISABLE   0x0
#define TIMER_MASK_IRQ  0x2
#define TIMER_IRQ_STAT  0x4

/* PMU States.  */
#define	PMU_STATE0 0
#define	PMU_STATE1 1
#define	PMU_STATE2 2

#define MAX_CLUSTERS    2
#define MAX_CORES       8
#define MAX_CPUIFS      8
#define STACK_SIZE      96

#define REGS 32
#define PMCR_IDX       0
#define PMSELR_IDX     1
#define PMCNTENSET_IDX 2
#define PMCNTENCLR_IDX 3
#define PMCCNTR_IDX    4
#define PMOVSR_IDX     5
#define PMINTENSET_IDX 6
#define PMINTENCLR_IDX 7
#define PMXEVTYPE0_IDX 8
#define PMXEVCNT0_IDX  9
#define PMXEVTYPE1_IDX 10
#define PMXEVCNT1_IDX  11
#define PMXEVTYPE2_IDX 12
#define PMXEVCNT2_IDX  13
#define PMXEVTYPE3_IDX 14
#define PMXEVCNT3_IDX  15
/*
 * Defines for PMU states
 */
static int pmu_mode = PMU_STATE0;

#define DIDR_VERSION_SHIFT 16
#define DIDR_VERSION_MASK  0xF
#define DIDR_VERSION_7_1   5
#define DIDR_BP_SHIFT      24
#define DIDR_BP_MASK       0xF
#define DIDR_WP_SHIFT      28
#define DIDR_WP_MASK       0xF
#define CLAIMCLR_CLEAR_ALL 0xff

#define DRAR_VALID_MASK   0x00000003
#define DSAR_VALID_MASK   0x00000003
#define DRAR_ADDRESS_MASK 0xFFFFF000
#define DSAR_ADDRESS_MASK 0xFFFFF000
#define OSLSR_OSLM_MASK   0x00000009
#define OSLAR_UNLOCKED    0x00000000
#define OSLAR_LOCKED      0xC5ACCE55
#define LAR_UNLOCKED      0xC5ACCE55
#define LAR_LOCKED        0x00000000
#define OSDLR_UNLOCKED    0x00000000
#define OSDLR_LOCKED      0x00000001

#define DBGREG_BP_VAL     0x0
#define DBGREG_WP_VAL     0x1
#define DBGREG_BP_CTRL    0x2
#define DBGREG_WP_CTRL    0x3
#define DBGREG_BP_XVAL    0x4

/* MCU_BIU Register */
#define MCU_BIU_CONTROL        (MCU_BIU_BASE)

/*
 * switch to amp / smp
 */
#define switch_to_amp()                         \
	do {                                        \
		isb();                                  \
		dsb();                                  \
		asm volatile(                           \
				"MRC p15,0,r0,c1,c0,1\n"        \
				"BIC r0,r0,#0x00000040\n"       \
				"MCR p15,0,r0,c1,c0,1\n"        \
				:                               \
				:                               \
				:"r0"                           \
				);                              \
		isb();                                  \
		dsb();                                  \
	} while (0)

#define switch_to_smp()                         \
	do {                                        \
		isb();                                  \
		dsb();                                  \
		asm volatile(                           \
				"MRC p15,0,r0,c1,c0,1\n"        \
				"ORR r0,r0,#0x00000040\n"       \
				"MCR p15,0,r0,c1,c0,1\n"        \
				:                               \
				:                               \
				:"r0"                           \
				);                              \
		isb();                                  \
		dsb();                                  \
	} while (0)

typedef struct {
	unsigned vcr;
	unsigned eacr;
	unsigned claim;
	unsigned claimclr;
	unsigned dtrrx_e;
	unsigned dtrtx_e;
	unsigned dscr_e;
	unsigned wfar;
	unsigned bvr[16];
	unsigned bcr[16];
	unsigned wvr[16];
	unsigned wcr[16];
	unsigned bxvr[16];
} debug_context_t;              /* total size 88 * 4 = 352 bytes */

typedef struct ns_gic_cpu_context {
	unsigned int gic_cpu_if_regs[32];   /* GIC context local to the CPU */
	unsigned int gic_dist_if_pvt_regs[32];  /* GIC SGI/PPI context local to the CPU */
} gic_cpu_context;

typedef struct fault_regs {
	unsigned dfar;
	unsigned ifar;
	unsigned ifsr;
	unsigned dfsr;
	unsigned adfsr;
	unsigned aifsr;
} cp15_fault_regs;

typedef struct ns_banked_cp15_context {
	unsigned int cp15_misc_regs[2]; /* cp15 miscellaneous registers */
	unsigned int cp15_ctrl_regs[20];    /* cp15 control registers */
	unsigned int cp15_mmu_regs[16]; /* cp15 mmu registers */
	cp15_fault_regs ns_cp15_fault_regs; /* cp15 fault status registers */
} banked_cp15_context;

typedef struct gen_tmr_ctx {
	unsigned cntfrq;
	unsigned long long cntvoff;
	unsigned cnthctl;
	unsigned cntkctl;
	unsigned long long cntp_cval;
	unsigned cntp_tval;
	unsigned cntp_ctl;
	unsigned long long cntv_cval;
	unsigned cntv_tval;
	unsigned cntv_ctl;
	unsigned long long cnthp_cval;
	unsigned cnthp_tval;
	unsigned cnthp_ctl;
} generic_timer_context;

typedef struct ns_cpu_context {
	unsigned int banked_cpu_regs[32];   /* Banked cpu registers */
	banked_cp15_context banked_cp15_regs;   /* Per cpu banked cp15 context */
	generic_timer_context cp15_timer_ctx;   /* Global counter registers if accessible in NS world */
	gic_cpu_context gic_cpu_ctx;    /* Per cpu GIC distributor and interface context */
	unsigned int endianess; /* Per cpu endianess */
	unsigned int vfp_regs[34];  /* Dummy entry for VFP context. */
	//debug_context_t debug_ctx;  /* Entry for Debug context. */
	unsigned int  dbg_data[32]; /* Entry for Debug context. */
} cpu_context;

typedef struct ns_global_context {
	unsigned int gic_dist_if_regs[512]; /* GIC distributor context to be saved by the last cpu. */
	unsigned int generic_timer_regs[8]; /* Global timers if the NS world has access to them */
} global_context;

/*
 * Structure to preserve the OS mmu and stack state for swtich from OS to Switcher
 * context handler.
 */
typedef struct os_state {
	unsigned sctlr;
	unsigned dacr;
	unsigned ttbr0;
	unsigned nmrr;
	unsigned prrr;
} os_state;

/*
 * Top level structure to hold the complete context of a core in a cluster in
 * a multi-cluster system
 */
typedef struct core_context {
	/*
	 * Non-secure context save area
	 */
	cpu_context ns_cpu_ctx;

} core_context;

/*
 * Top level structure to hold the complete context of a cluster in a multi-
 * cluster system
 */
typedef struct cluster_context {
	core_context core[MAX_CORES];
	unsigned num_cores;
	global_context ns_cluster_ctx;
} cluster_context;

/*
 * Top level structure to hold the complete context of a multi cluster system
 */
typedef struct system_context {
	cluster_context cluster;
	unsigned num_clusters;
	unsigned warm_reset;
} system_context;

typedef volatile struct {	/* Registers  Save?                                          */
	unsigned const didr;	/*         0  Read only                                    */
	unsigned dscr_i;	/*         1  ignore - use dscr_e instead                  */
	unsigned const dummy1[3];	/*       2-4  ignore                                       */
	unsigned dtrrx_dtrtx_i;	/*         5  ignore                                       */
	unsigned wfar;		/*         6  ignore - transient information               */
	unsigned vcr;		/*         7  Save                                         */
	unsigned const dummy2;	/*         8  ignore                                       */
	unsigned ecr;		/*         9  ignore                                       */
	unsigned dsccr;		/*        10  ignore                                       */
	unsigned dsmcr;		/*        11  ignore                                       */
	unsigned const dummy3[20];	/*     12-31  ignore                                       */
	unsigned dtrrx_e;	/*        32  ignore                                       */
	unsigned itr_pcsr;	/*        33  ignore                                       */
	unsigned dscr_e;	/*        34  Save                                         */
	unsigned dtrtx_e;	/*        35  ignore                                       */
	unsigned drcr;		/*        36  ignore                                       */
	unsigned eacr;		/*        37  Save - V7.1 only                             */
	unsigned const dummy4[2];	/*     38-39  ignore                                       */
	unsigned pcsr;		/*        40  ignore                                       */
	unsigned cidsr;		/*        41  ignore                                       */
	unsigned vidsr;		/*        42  ignore                                       */
	unsigned const dummy5[21];	/*     43-63  ignore                                       */
	unsigned bvr[16];	/*     64-79  Save                                         */
	unsigned bcr[16];	/*     80-95  Save                                         */
	unsigned wvr[16];	/*    96-111  Save                                         */
	unsigned wcr[16];	/*   112-127  Save                                         */
	unsigned const dummy6[16];	/*   128-143  ignore                                       */
	unsigned bxvr[16];	/*   144-159  Save if have Virtualization extensions       */
	unsigned const dummy7[32];	/*   160-191  ignore                                       */
	unsigned oslar;		/*       192  If oslsr[0] is 1, unlock before save/restore */
	unsigned const oslsr;	/*       193  ignore                                       */
	unsigned ossrr;		/*       194  ignore                                       */
	unsigned const dummy8;	/*       195  ignore                                       */
	unsigned prcr;		/*       196  ignore                                       */
	unsigned prsr;		/*       197  clear SPD on restore                         */
	unsigned const dummy9[762];	/*   198-959  ignore                                       */
	unsigned itctrl;	/*       960  ignore                                       */
	unsigned const dummy10[39];	/*   961-999  ignore                                       */
	unsigned claimset;	/*      1000  Restore claim bits to here                   */
	unsigned claimclr;	/*      1001  Save claim bits from here                    */
	unsigned const dummy11[2];	/* 1002-1003  ignore                                       */
	unsigned lar;		/*      1004  Unlock before restore                        */
	unsigned const lsr;	/*      1005  ignore                                       */
	unsigned const authstatus;	/*      1006  Read only                                    */
	unsigned const dummy12;	/*      1007  ignore                                       */
	unsigned const devid2;	/*      1008  Read only                                    */
	unsigned const devid1;	/*      1009  Read only                                    */
	unsigned const devid;	/*      1010  Read only                                    */
	unsigned const devtype;	/*      1011  Read only                                    */
	unsigned const pid[8];	/* 1012-1019  Read only                                    */
	unsigned const cid[4];	/* 1020-1023  Read only                                    */
} debug_registers_t;

typedef struct {
	unsigned (*read) (void);
	void (*write) (unsigned);
} rw_ops;

typedef struct {
	rw_ops bvr;
	rw_ops bcr;
	rw_ops wvr;
	rw_ops wcr;
	rw_ops bxvr;
} dbgreg_rw_ops;

typedef struct
{
	volatile unsigned int control;                      /* 0x000 */
	const unsigned int controller_type;
	const unsigned int implementer;
	const char padding1[116];
	volatile unsigned int security[32];                 /* 0x080 */
	struct set_and_clear_regs enable;                   /* 0x100 */
	struct set_and_clear_regs pending;                  /* 0x200 */
	struct set_and_clear_regs active;                   /* 0x300 */
	volatile unsigned int priority[256];                /* 0x400 */
	volatile unsigned int target[256];                  /* 0x800 */
	volatile unsigned int configuration[64];            /* 0xC00 */
	const char padding3[512];                           /* 0xD00 */
	volatile unsigned int software_interrupt;           /* 0xF00 */
	volatile unsigned int sgi_clr_pending[4];           /* 0xF10 */
	volatile unsigned int sgi_set_pending[4];           /* 0xF20 */
	const char padding4[176];
	unsigned const int peripheral_id[4];                /* 0xFE0 */
	unsigned const int primecell_id[4];                 /* 0xFF0 */
} interrupt_distributor;



typedef struct
{
	volatile unsigned int control;                      /* 0x00 */
	volatile unsigned int priority_mask;                /* 0x04 */
	volatile unsigned int binary_point;                 /* 0x08 */
	volatile unsigned const int interrupt_ack;          /* 0x0c */
	volatile unsigned int end_of_interrupt;             /* 0x10 */
	volatile unsigned const int running_priority;       /* 0x14 */
	volatile unsigned const int highest_pending;        /* 0x18 */
	volatile unsigned int aliased_binary_point;         /* 0x1c */
	volatile unsigned const int aliased_interrupt_ack;  /* 0x20 */
	volatile unsigned int alias_end_of_interrupt;       /* 0x24 */
	volatile unsigned const int alias_highest_pending;  /* 0x28 */
} cpu_interface;

typedef struct
{
	unsigned int mcu_biu_control; /* MCU_BIU control register */
} mcu_biu_reg;

struct set_and_clear_regs
{
	volatile u32 set[32], clear[32];
};

/* input value:
 * DORMANT_MODE / SHUTDOWN_MODE
 *
 * return value:
 * 0 : execute wfi then power down
 * 1 : wake up and resume
 */
extern int cpu_power_down(int mode);
extern void cpu_dormant_init(void);
extern void cpu_check_dormant_abort(void);

extern unsigned *copy_words(volatile unsigned *destination,
		volatile unsigned *source, unsigned num_words);

extern void cpu_wake_up(void);

extern void save_control_registers(unsigned *pointer, int is_secure);
extern void save_mmu(unsigned *pointer);
extern void save_mpu(unsigned *pointer);
extern void save_performance_monitors(unsigned *pointer);
extern void save_banked_registers(unsigned *pointer);
extern void save_cp15(unsigned *pointer);
extern void save_vfp(unsigned *pointer);
extern void save_generic_timer(unsigned *pointer, int is_hyp);
//extern void save_v7_debug(unsigned *pointer);
extern void save_fault_status(unsigned *pointer);

extern void restore_control_registers(unsigned *pointer, int is_secure);
extern void mt_restore_control_registers(unsigned *pointer, int is_secure);
extern void restore_mmu(unsigned *pointer);
extern void restore_mpu(unsigned *pointer);
extern void restore_performance_monitors(unsigned *pointer);
extern void restore_banked_registers(unsigned *pointer);
extern void restore_cp15(unsigned *pointer);
extern void restore_vfp(unsigned *pointer);
extern void restore_generic_timer(unsigned *pointer, int is_hyp);
//extern void restore_v7_debug(unsigned *pointer);
extern void restore_fault_status(unsigned *pointer);

extern void write_cntp_ctl(unsigned);
extern void write_cntv_ctl(unsigned);
extern unsigned read_cpuid(void);
extern unsigned read_clusterid(void);
extern unsigned read_nsacr(void);
extern unsigned read_id_pfr1(void);

extern unsigned read_dbg_osdlr(void);
extern unsigned read_dbg_drar(void);
extern unsigned read_dbg_dsar(void);
extern unsigned read_dbg_devid(void);
extern unsigned read_dbg_didr(void);
extern unsigned read_dbg_dtrrxext(void);
extern unsigned read_dbg_dtrtxext(void);
extern unsigned read_dbg_dscrext(void);
extern unsigned read_dbg_wfar(void);
extern unsigned read_dbg_vcr(void);
extern unsigned read_dbg_claimclr(void);
extern unsigned read_dbg_bvr0(void);
extern unsigned read_dbg_bvr1(void);
extern unsigned read_dbg_bvr2(void);
extern unsigned read_dbg_bvr3(void);
extern unsigned read_dbg_bvr4(void);
extern unsigned read_dbg_bvr5(void);
extern unsigned read_dbg_bvr6(void);
extern unsigned read_dbg_bvr7(void);
extern unsigned read_dbg_bvr8(void);
extern unsigned read_dbg_bvr9(void);
extern unsigned read_dbg_bvr10(void);
extern unsigned read_dbg_bvr11(void);
extern unsigned read_dbg_bvr12(void);
extern unsigned read_dbg_bvr13(void);
extern unsigned read_dbg_bvr14(void);
extern unsigned read_dbg_bvr15(void);
extern unsigned read_dbg_bcr0(void);
extern unsigned read_dbg_bcr1(void);
extern unsigned read_dbg_bcr2(void);
extern unsigned read_dbg_bcr3(void);
extern unsigned read_dbg_bcr4(void);
extern unsigned read_dbg_bcr5(void);
extern unsigned read_dbg_bcr6(void);
extern unsigned read_dbg_bcr7(void);
extern unsigned read_dbg_bcr8(void);
extern unsigned read_dbg_bcr9(void);
extern unsigned read_dbg_bcr10(void);
extern unsigned read_dbg_bcr11(void);
extern unsigned read_dbg_bcr12(void);
extern unsigned read_dbg_bcr13(void);
extern unsigned read_dbg_bcr14(void);
extern unsigned read_dbg_bcr15(void);
extern unsigned read_dbg_wvr0(void);
extern unsigned read_dbg_wvr1(void);
extern unsigned read_dbg_wvr2(void);
extern unsigned read_dbg_wvr3(void);
extern unsigned read_dbg_wvr4(void);
extern unsigned read_dbg_wvr5(void);
extern unsigned read_dbg_wvr6(void);
extern unsigned read_dbg_wvr7(void);
extern unsigned read_dbg_wvr8(void);
extern unsigned read_dbg_wvr9(void);
extern unsigned read_dbg_wvr10(void);
extern unsigned read_dbg_wvr11(void);
extern unsigned read_dbg_wvr12(void);
extern unsigned read_dbg_wvr13(void);
extern unsigned read_dbg_wvr14(void);
extern unsigned read_dbg_wvr15(void);
extern unsigned read_dbg_wcr0(void);
extern unsigned read_dbg_wcr1(void);
extern unsigned read_dbg_wcr2(void);
extern unsigned read_dbg_wcr3(void);
extern unsigned read_dbg_wcr4(void);
extern unsigned read_dbg_wcr5(void);
extern unsigned read_dbg_wcr6(void);
extern unsigned read_dbg_wcr7(void);
extern unsigned read_dbg_wcr8(void);
extern unsigned read_dbg_wcr9(void);
extern unsigned read_dbg_wcr10(void);
extern unsigned read_dbg_wcr11(void);
extern unsigned read_dbg_wcr12(void);
extern unsigned read_dbg_wcr13(void);
extern unsigned read_dbg_wcr14(void);
extern unsigned read_dbg_wcr15(void);
extern unsigned read_dbg_bxvr0(void);
extern unsigned read_dbg_bxvr1(void);
extern unsigned read_dbg_bxvr2(void);
extern unsigned read_dbg_bxvr3(void);
extern unsigned read_dbg_bxvr4(void);
extern unsigned read_dbg_bxvr5(void);
extern unsigned read_dbg_bxvr6(void);
extern unsigned read_dbg_bxvr7(void);
extern unsigned read_dbg_bxvr8(void);
extern unsigned read_dbg_bxvr9(void);
extern unsigned read_dbg_bxvr10(void);
extern unsigned read_dbg_bxvr11(void);
extern unsigned read_dbg_bxvr12(void);
extern unsigned read_dbg_bxvr13(void);
extern unsigned read_dbg_bxvr14(void);
extern unsigned read_dbg_bxvr15(void);

extern void write_dbg_osdlr(unsigned);
extern void write_dbg_oslar(unsigned);
extern void write_dbg_dtrrxext(unsigned);
extern void write_dbg_dtrtxext(unsigned);
extern void write_dbg_dscrext(unsigned);
extern void write_dbg_wfar(unsigned);
extern void write_dbg_vcr(unsigned);
extern void write_dbg_claimset(unsigned);
extern void write_dbg_bvr0(unsigned);
extern void write_dbg_bvr1(unsigned);
extern void write_dbg_bvr2(unsigned);
extern void write_dbg_bvr3(unsigned);
extern void write_dbg_bvr4(unsigned);
extern void write_dbg_bvr5(unsigned);
extern void write_dbg_bvr6(unsigned);
extern void write_dbg_bvr7(unsigned);
extern void write_dbg_bvr8(unsigned);
extern void write_dbg_bvr9(unsigned);
extern void write_dbg_bvr10(unsigned);
extern void write_dbg_bvr11(unsigned);
extern void write_dbg_bvr12(unsigned);
extern void write_dbg_bvr13(unsigned);
extern void write_dbg_bvr14(unsigned);
extern void write_dbg_bvr15(unsigned);
extern void write_dbg_bcr0(unsigned);
extern void write_dbg_bcr1(unsigned);
extern void write_dbg_bcr2(unsigned);
extern void write_dbg_bcr3(unsigned);
extern void write_dbg_bcr4(unsigned);
extern void write_dbg_bcr5(unsigned);
extern void write_dbg_bcr6(unsigned);
extern void write_dbg_bcr7(unsigned);
extern void write_dbg_bcr8(unsigned);
extern void write_dbg_bcr9(unsigned);
extern void write_dbg_bcr10(unsigned);
extern void write_dbg_bcr11(unsigned);
extern void write_dbg_bcr12(unsigned);
extern void write_dbg_bcr13(unsigned);
extern void write_dbg_bcr14(unsigned);
extern void write_dbg_bcr15(unsigned);
extern void write_dbg_wvr0(unsigned);
extern void write_dbg_wvr1(unsigned);
extern void write_dbg_wvr2(unsigned);
extern void write_dbg_wvr3(unsigned);
extern void write_dbg_wvr4(unsigned);
extern void write_dbg_wvr5(unsigned);
extern void write_dbg_wvr6(unsigned);
extern void write_dbg_wvr7(unsigned);
extern void write_dbg_wvr8(unsigned);
extern void write_dbg_wvr9(unsigned);
extern void write_dbg_wvr10(unsigned);
extern void write_dbg_wvr11(unsigned);
extern void write_dbg_wvr12(unsigned);
extern void write_dbg_wvr13(unsigned);
extern void write_dbg_wvr14(unsigned);
extern void write_dbg_wvr15(unsigned);
extern void write_dbg_wcr0(unsigned);
extern void write_dbg_wcr1(unsigned);
extern void write_dbg_wcr2(unsigned);
extern void write_dbg_wcr3(unsigned);
extern void write_dbg_wcr4(unsigned);
extern void write_dbg_wcr5(unsigned);
extern void write_dbg_wcr6(unsigned);
extern void write_dbg_wcr7(unsigned);
extern void write_dbg_wcr8(unsigned);
extern void write_dbg_wcr9(unsigned);
extern void write_dbg_wcr10(unsigned);
extern void write_dbg_wcr11(unsigned);
extern void write_dbg_wcr12(unsigned);
extern void write_dbg_wcr13(unsigned);
extern void write_dbg_wcr14(unsigned);
extern void write_dbg_wcr15(unsigned);
extern void write_dbg_bxvr0(unsigned);
extern void write_dbg_bxvr1(unsigned);
extern void write_dbg_bxvr2(unsigned);
extern void write_dbg_bxvr3(unsigned);
extern void write_dbg_bxvr4(unsigned);
extern void write_dbg_bxvr5(unsigned);
extern void write_dbg_bxvr6(unsigned);
extern void write_dbg_bxvr7(unsigned);
extern void write_dbg_bxvr8(unsigned);
extern void write_dbg_bxvr9(unsigned);
extern void write_dbg_bxvr10(unsigned);
extern void write_dbg_bxvr11(unsigned);
extern void write_dbg_bxvr12(unsigned);
extern void write_dbg_bxvr13(unsigned);
extern void write_dbg_bxvr14(unsigned);
extern void write_dbg_bxvr15(unsigned);

extern void __enable_cache(void);
extern void __disable_cache(void);
extern void __disable_dcache(void);

extern void inner_dcache_flush_L1(void);
extern void inner_dcache_flush_all(void);

extern void invalidate_unified_TLB_inner_shareable(void);

extern void __inner_inv_dcache_L1(void);
extern void __inner_inv_dcache_L2(void);

extern void __inner_clean_dcache_L1(void);
extern void __inner_clean_dcache_L2(void);
extern void __inner_clean_dcache_all(void);

extern void trace_stop_dormant(void);
extern void trace_start_dormant(void);

//for save/restore breakpoint and watchpoint
extern void save_dbg_regs(unsigned int data[]);
extern void restore_dbg_regs(unsigned int data[]);
#endif

