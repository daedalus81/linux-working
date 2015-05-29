#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <asm/system.h>
#include <mach/mt_reg_base.h>
#include <mach/mt_dormant.h>
#include <mach/mt_spm.h>
#include <mach/mt_irq.h>
#include <mach/sync_write.h>
#include <mach/mt_spm_mtcmos.h>

#if defined(CONFIG_TRUSTONIC_TEE)
#include <mach/mt_secure_api.h>
#endif

#if defined(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT)
#include <mach/mtk_boot_share_page.h>
#include <trustzone/kree/tz_pm.h>
#endif

unsigned int clusters_ctx[MAX_CLUSTERS][MAX_CORES][REGS];
unsigned int migration_ctx[MAX_CORES][REGS];

//#define CPU_DORMANT_DEBUG


#ifndef  CPU_DORMANT_DEBUG
#define CPU_DORMANT_INFO(fmt, args...)
#else
#define CPU_DORMANT_INFO(fmt, args...)    printk("[Power/cpu_dormant] "fmt, ##args)
#endif


/*********************************
 * glabal variable
 **********************************/
volatile static int dormant_ret_flag[4] = {0,0,0,0};
volatile int power_state[4] = {STATUS_RUN,STATUS_RUN,STATUS_RUN,STATUS_RUN};
mcu_biu_reg mcu_biu;

/*
 * Top level structure which encapsulates the context of the entire
 * Kingfisher system
 */
system_context switcher_context;

dbgreg_rw_ops dbgreg_rw_handlers[] = {
	{
		{read_dbg_bvr0, write_dbg_bvr0,},
		{read_dbg_bcr0, write_dbg_bcr0,},
		{read_dbg_wvr0, write_dbg_wvr0,},
		{read_dbg_wcr0, write_dbg_wcr0,},
		{read_dbg_bxvr0, write_dbg_bxvr0,},
	},
	{
		{read_dbg_bvr1, write_dbg_bvr1,},
		{read_dbg_bcr1, write_dbg_bcr1,},
		{read_dbg_wvr1, write_dbg_wvr1,},
		{read_dbg_wcr1, write_dbg_wcr1,},
		{read_dbg_bxvr1, write_dbg_bxvr1,},
	},
	{
		{read_dbg_bvr2, write_dbg_bvr2,},
		{read_dbg_bcr2, write_dbg_bcr2,},
		{read_dbg_wvr2, write_dbg_wvr2,},
		{read_dbg_wcr2, write_dbg_wcr2,},
		{read_dbg_bxvr2, write_dbg_bxvr2,},
	},
	{
		{read_dbg_bvr3, write_dbg_bvr3,},
		{read_dbg_bcr3, write_dbg_bcr3,},
		{read_dbg_wvr3, write_dbg_wvr3,},
		{read_dbg_wcr3, write_dbg_wcr3,},
		{read_dbg_bxvr3, write_dbg_bxvr3,},
	},
	{
		{read_dbg_bvr4, write_dbg_bvr4,},
		{read_dbg_bcr4, write_dbg_bcr4,},
		{read_dbg_wvr4, write_dbg_wvr4,},
		{read_dbg_wcr4, write_dbg_wcr4,},
		{read_dbg_bxvr4, write_dbg_bxvr4,},
	},
	{
		{read_dbg_bvr5, write_dbg_bvr5,},
		{read_dbg_bcr5, write_dbg_bcr5,},
		{read_dbg_wvr5, write_dbg_wvr5,},
		{read_dbg_wcr5, write_dbg_wcr5,},
		{read_dbg_bxvr5, write_dbg_bxvr5,},
	},
	{
		{read_dbg_bvr6, write_dbg_bvr6,},
		{read_dbg_bcr6, write_dbg_bcr6,},
		{read_dbg_wvr6, write_dbg_wvr6,},
		{read_dbg_wcr6, write_dbg_wcr6,},
		{read_dbg_bxvr6, write_dbg_bxvr6,},
	},
	{
		{read_dbg_bvr7, write_dbg_bvr7,},
		{read_dbg_bcr7, write_dbg_bcr7,},
		{read_dbg_wvr7, write_dbg_wvr7,},
		{read_dbg_wcr7, write_dbg_wcr7,},
		{read_dbg_bxvr7, write_dbg_bxvr7,},
	},
	{
		{read_dbg_bvr8, write_dbg_bvr8,},
		{read_dbg_bcr8, write_dbg_bcr8,},
		{read_dbg_wvr8, write_dbg_wvr8,},
		{read_dbg_wcr8, write_dbg_wcr8,},
		{read_dbg_bxvr8, write_dbg_bxvr8,},
	},
	{
		{read_dbg_bvr9, write_dbg_bvr9,},
		{read_dbg_bcr9, write_dbg_bcr9,},
		{read_dbg_wvr9, write_dbg_wvr9,},
		{read_dbg_wcr9, write_dbg_wcr9,},
		{read_dbg_bxvr9, write_dbg_bxvr9,},
	},
	{
		{read_dbg_bvr10, write_dbg_bvr10,},
		{read_dbg_bcr10, write_dbg_bcr10,},
		{read_dbg_wvr10, write_dbg_wvr10,},
		{read_dbg_wcr10, write_dbg_wcr10,},
		{read_dbg_bxvr10, write_dbg_bxvr10,},
	},
	{
		{read_dbg_bvr11, write_dbg_bvr11,},
		{read_dbg_bcr11, write_dbg_bcr11,},
		{read_dbg_wvr11, write_dbg_wvr11,},
		{read_dbg_wcr11, write_dbg_wcr11,},
		{read_dbg_bxvr11, write_dbg_bxvr11,},
	},
	{
		{read_dbg_bvr12, write_dbg_bvr12,},
		{read_dbg_bcr12, write_dbg_bcr12,},
		{read_dbg_wvr12, write_dbg_wvr12,},
		{read_dbg_wcr12, write_dbg_wcr12,},
		{read_dbg_bxvr12, write_dbg_bxvr12,},
	},
	{
		{read_dbg_bvr13, write_dbg_bvr13,},
		{read_dbg_bcr13, write_dbg_bcr13,},
		{read_dbg_wvr13, write_dbg_wvr13,},
		{read_dbg_wcr13, write_dbg_wcr13,},
		{read_dbg_bxvr13, write_dbg_bxvr13,},
	},
	{
		{read_dbg_bvr14, write_dbg_bvr14,},
		{read_dbg_bcr14, write_dbg_bcr14,},
		{read_dbg_wvr14, write_dbg_wvr14,},
		{read_dbg_wcr14, write_dbg_wcr14,},
		{read_dbg_bxvr14, write_dbg_bxvr14,},
	},
	{
		{read_dbg_bvr15, write_dbg_bvr15,},
		{read_dbg_bcr15, write_dbg_bcr15,},
		{read_dbg_wvr15, write_dbg_wvr15,},
		{read_dbg_wcr15, write_dbg_wcr15,},
		{read_dbg_bxvr15, write_dbg_bxvr15,},
	},
};


/*********************************
 * function
 **********************************/
/*
 * Saves the MCU_BIU register context
 * Requires 1 words of memory
 */
static void save_mcu_biu_register(void)
{
	mcu_biu.mcu_biu_control = reg_read(MCU_BIU_CONTROL);
}

static void restore_mcu_biu_register(void)
{
	reg_write(MCU_BIU_CONTROL, mcu_biu.mcu_biu_control);
}

/*
 * Saves the GIC CPU interface context
 * Requires 3 words of memory
 */
static void save_gic_interface(u32 *pointer, unsigned gic_interface_address)
{
	cpu_interface *ci = (cpu_interface *)gic_interface_address;

	pointer[0] = ci->control;
	pointer[1] = ci->priority_mask;
	pointer[2] = ci->binary_point;
	pointer[3] = ci->aliased_binary_point;

	/* TODO: add nonsecure stuff */
}

/*
 * Saves this CPU's banked parts of the distributor
 * Returns non-zero if an SGI/PPI interrupt is pending (after saving all required context)
 * Requires 19 words of memory
 */
static void save_gic_distributor_private(u32 *pointer, unsigned gic_distributor_address)
{
	interrupt_distributor *id = (interrupt_distributor *)gic_distributor_address;
	unsigned int *ptr = 0x0;

	/*  Save SGI,PPI enable status*/
	*pointer = id->enable.set[0];
	++pointer;
	/*  Save SGI,PPI priority status*/
	pointer = copy_words(pointer, id->priority, 8);
	/*  Save SGI,PPI target status*/
	pointer = copy_words(pointer, id->target, 8);
	/*  Save just the PPI configurations (SGIs are not configurable) */
	*pointer = id->configuration[1];
	++pointer;
	/*  Save SGI,PPI security status*/
	*pointer = id->security[0];
	++pointer;
#if 0
	/*
	 * Private peripheral interrupts need to be replayed on
	 * the destination cpu interface for consistency. This
	 * is the responsibility of the peripheral driver. When
	 * it sees a pending interrupt while saving its context
	 * it should record enough information to recreate the
	 * interrupt while restoring.
	 * We don't save the Pending/Active status and clear it
	 * so that it does not interfere when we are back.
	 */
	/*  Clear PPI pending status*/
	id->pending.clear[0] = 0xffffffff;
	id->active.clear[0] = 0xffffffff;
#endif
#if 1
	/*  Save SGI,PPI pending status*/
	*pointer = id->pending.set[0];
	++pointer;
#endif
	/*
	 * IPIs are different and can be replayed just by saving
	 * and restoring the set/clear pending registers
	 */
	ptr = pointer;
	copy_words(pointer, id->sgi_set_pending, 4);
	pointer += 8;

	/*
	 * Clear the pending SGIs on this cpuif so that they don't
	 * interfere with the wfi later on.
	 */
	copy_words(id->sgi_clr_pending, ptr, 4);



}

/*
 * Saves the shared parts of the distributor
 * Requires 1 word of memory, plus 20 words for each block of 32 SPIs (max 641 words)
 * Returns non-zero if an SPI interrupt is pending (after saving all required context)
 */
static void save_gic_distributor_shared(u32 *pointer, unsigned gic_distributor_address)
{
	interrupt_distributor *id = (interrupt_distributor *)gic_distributor_address;
	unsigned num_spis, *saved_pending;

	/* Calculate how many SPIs the GIC supports */
	num_spis = 32 * (id->controller_type & 0x1f);

	/* TODO: add nonsecure stuff */

	/* Save rest of GIC configuration */
	if (num_spis) {
		pointer = copy_words(pointer, id->enable.set + 1, num_spis / 32);
		pointer = copy_words(pointer, id->priority + 8, num_spis / 4);
		pointer = copy_words(pointer, id->target + 8, num_spis / 4);
		pointer = copy_words(pointer, id->configuration + 2, num_spis / 16);
		pointer = copy_words(pointer, id->security + 1, num_spis / 32);
		saved_pending = pointer;
		pointer = copy_words(pointer, id->pending.set + 1, num_spis / 32);
	}

	/* Save control register */
	*pointer = id->control;
}
static void restore_gic_interface(u32 *pointer, unsigned gic_interface_address)
{
	cpu_interface *ci = (cpu_interface *)gic_interface_address;

	/* TODO: add nonsecure stuff */

	ci->priority_mask = pointer[1];
	ci->binary_point = pointer[2];
	ci->aliased_binary_point = pointer[3];

	/* Restore control register last */
	ci->control = pointer[0];
}
static void restore_gic_distributor_private(u32 *pointer, unsigned gic_distributor_address)
{
	interrupt_distributor *id = (interrupt_distributor *)gic_distributor_address;
	unsigned tmp;
	//unsigned ctr, prev_val = 0, prev_ctr = 0;

	/* First disable the distributor so we can write to its config registers */
	tmp = id->control;
	id->control = 0;
	/* Restore SGI,PPI enable status*/
	id->enable.set[0] = *pointer;
	++pointer;
	/* Restore SGI,PPI priority  status*/
	copy_words(id->priority, pointer, 8);
	pointer += 8;
	/* Restore SGI,PPI target status*/
	copy_words(id->target, pointer, 8);
	pointer += 8;
	/* Restore just the PPI configurations (SGIs are not configurable) */
	id->configuration[1] = *pointer;
	++pointer;
	/* Restore SGI,PPI security status*/
	id->security[0] = *pointer;
	++pointer;
#if 0
	/*
	 * Clear active and  pending PPIs as they will be recreated by the
	 * peripiherals
	 */
	id->active.clear[0] = 0xffffffff;
	id->pending.clear[0] = 0xffffffff;
#endif
#if 1
	/*  Restore SGI,PPI pending status*/
	id->pending.set[0] = *pointer;
	++pointer;
#endif
	/*
	 * Restore pending SGIs
	 */
	copy_words(id->sgi_set_pending, pointer, 4);
	pointer += 4;

	id->control = tmp;
}
static void restore_gic_distributor_shared(u32 *pointer, unsigned gic_distributor_address)
{
	interrupt_distributor *id = (interrupt_distributor *)gic_distributor_address;
	unsigned num_spis;
	int i, j;

	/* First disable the distributor so we can write to its config registers */
	id->control = 0;

	/* Calculate how many SPIs the GIC supports */
	num_spis = 32 * ((id->controller_type) & 0x1f);

	/* TODO: add nonsecure stuff */

	/* Restore rest of GIC configuration */
	if (num_spis) {
		copy_words(id->enable.set + 1, pointer, num_spis / 32);
		pointer += num_spis / 32;
		copy_words(id->priority + 8, pointer, num_spis / 4);
		pointer += num_spis / 4;
		copy_words(id->target + 8, pointer, num_spis / 4);
		pointer += num_spis / 4;
		copy_words(id->configuration + 2, pointer, num_spis / 16);
		pointer += num_spis / 16;
		copy_words(id->security + 1, pointer, num_spis / 32);
		pointer += num_spis / 32;
		copy_words(id->pending.set + 1, pointer, num_spis / 32);

		if (reg_read( IO_VIRT_TO_PHYS(SPM_SLEEP_ISR_RAW_STA) ) & WAKE_SRC_KP) {
			i = MT_KP_IRQ_ID / GIC_PRIVATE_SIGNALS;
			j = MT_KP_IRQ_ID % GIC_PRIVATE_SIGNALS;
			id->pending.set[i] |= (1 << j);
		}

		/*
		   if (reg_read( IO_VIRT_TO_PHYS(SPM_SLEEP_ISR_RAW_STA) ) & WAKE_SRC_MD_WDT) {
		   i = MD_WDT_IRQ_ID / GIC_PRIVATE_SIGNALS;
		   j = MD_WDT_IRQ_ID % GIC_PRIVATE_SIGNALS;
		   id->pending.set[i] |= (1 << j);
		   }
		   */
		if (reg_read( IO_VIRT_TO_PHYS(SPM_SLEEP_ISR_RAW_STA) ) & WAKE_SRC_CONN_WDT) {
			i = CONN_WDT_IRQ_ID / GIC_PRIVATE_SIGNALS;
			j = CONN_WDT_IRQ_ID % GIC_PRIVATE_SIGNALS;
			id->pending.set[i] |= (1 << j);
		}

		if (reg_read( IO_VIRT_TO_PHYS(SPM_SLEEP_ISR_RAW_STA) ) & WAKE_SRC_TS) {
			i = TS_IRQ_ID / GIC_PRIVATE_SIGNALS;
			j = TS_IRQ_ID % GIC_PRIVATE_SIGNALS;
			id->pending.set[i] |= (1 << j);
		}

		if (reg_read( IO_VIRT_TO_PHYS(SPM_SLEEP_ISR_RAW_STA) ) & WAKE_SRC_LOW_BAT) {
			i = LOWBATTERY_IRQ_ID / GIC_PRIVATE_SIGNALS;
			j = LOWBATTERY_IRQ_ID % GIC_PRIVATE_SIGNALS;
			id->pending.set[i] |= (1 << j);
		}

		if (reg_read( IO_VIRT_TO_PHYS(SPM_SLEEP_ISR_RAW_STA) ) & WAKE_SRC_WDT) {
			i = MT_WDT_IRQ_ID / GIC_PRIVATE_SIGNALS;
			j = MT_WDT_IRQ_ID % GIC_PRIVATE_SIGNALS;
			id->pending.set[i] |= (1 << j);
		}

		pointer += num_spis / 32;
	}

	/* We assume the I and F bits are set in the CPSR so that we will not respond to interrupts! */
	/* Restore control register */
	id->control = *pointer;
}

static void restore_bp_reg(debug_context_t *dbg, unsigned index, unsigned type)
{
	switch (type) {
		case DBGREG_WP_VAL:
			dbgreg_rw_handlers[index].wvr.write(dbg->wvr[index]);
			break;
		case DBGREG_WP_CTRL:
			dbgreg_rw_handlers[index].wcr.write(dbg->wcr[index]);
			break;
		case DBGREG_BP_XVAL:
			dbgreg_rw_handlers[index].bxvr.write(dbg->bxvr[index]);
			break;
		case DBGREG_BP_VAL:
			dbgreg_rw_handlers[index].bvr.write(dbg->bvr[index]);
			break;
		case DBGREG_BP_CTRL:
			dbgreg_rw_handlers[index].bcr.write(dbg->bcr[index]);
			break;
		default:
			break;
	}

	return;
}

static void save_bp_reg(debug_context_t *dbg, unsigned index, unsigned type)
{
	switch (type) {
		case DBGREG_WP_VAL:
			dbg->wvr[index] = dbgreg_rw_handlers[index].wvr.read();
			break;
		case DBGREG_WP_CTRL:
			dbg->wcr[index] = dbgreg_rw_handlers[index].wcr.read();
			break;
		case DBGREG_BP_XVAL:
			dbg->bxvr[index] = dbgreg_rw_handlers[index].bxvr.read();
			break;
		case DBGREG_BP_VAL:
			dbg->bvr[index] = dbgreg_rw_handlers[index].bvr.read();
			break;
		case DBGREG_BP_CTRL:
			dbg->bcr[index] = dbgreg_rw_handlers[index].bcr.read();
			break;
		default:
			break;
	}

	return;
}

static void sr_bp_context(debug_context_t *dbg, unsigned bp_type, unsigned op)
{
	unsigned num_bps, num_ctx_cmps, num_wps, didr;
	unsigned index = 0, max_index = 0;

	didr = read_dbg_didr();
	num_bps = (didr >> 24) & 0xf;
	num_ctx_cmps = (didr >> 20) & 0xf;
	num_wps = (didr >> 28) & 0xf;

	switch (bp_type) {
		case DBGREG_WP_VAL:
		case DBGREG_WP_CTRL:
			max_index = num_wps;
			break;
		case DBGREG_BP_XVAL:
			index = num_bps - num_ctx_cmps;
		case DBGREG_BP_VAL:
		case DBGREG_BP_CTRL:
			max_index = num_bps;
			break;
		default:
			break;
	}

	for (; index <= max_index; index++)
		if (op)
			save_bp_reg(dbg, index, bp_type);
		else
			restore_bp_reg(dbg, index, bp_type);
	return;
}

static void save_v71_debug_cp14(unsigned *context)
{
	debug_context_t *dbg = (void *) context;
	unsigned virtext_present;

	/*
	 * Unlock the Double lock.
	 */
	if (read_dbg_osdlr() == 0x1)
		write_dbg_osdlr(OSDLR_UNLOCKED);

	virtext_present = (read_dbg_devid() >> 16) & 0xf;

	/*
	 * Prevent updates to the debug registers during a S&R operation
	 */
	write_dbg_oslar(OSLAR_LOCKED);

	dbg->dtrrx_e = read_dbg_dtrrxext();
	dbg->dtrtx_e = read_dbg_dtrtxext();
	dbg->dscr_e = read_dbg_dscrext();
	dbg->wfar = read_dbg_wfar();
	dbg->vcr = read_dbg_vcr();
	dbg->claimclr = read_dbg_claimclr();

	if (virtext_present)
		sr_bp_context(dbg, DBGREG_BP_XVAL, 1);

	sr_bp_context(dbg, DBGREG_BP_VAL, 1);
	sr_bp_context(dbg, DBGREG_BP_CTRL, 1);
	sr_bp_context(dbg, DBGREG_WP_VAL, 1);
	sr_bp_context(dbg, DBGREG_WP_CTRL, 1);

	write_dbg_osdlr(OSDLR_LOCKED);

	return;
}

static void restore_v71_debug_cp14(unsigned *context)
{
	debug_context_t *dbg = (void *) context;
	unsigned virtext_present;

	/*
	 * Unlock the Double lock.
	 */
	if (read_dbg_osdlr() == 0x1)
		write_dbg_osdlr(OSDLR_UNLOCKED);

	virtext_present = (read_dbg_devid() >> 16) & 0xf;

	/*
	 * Prevent updates to the debug registers during a S&R operation
	 */
	write_dbg_oslar(OSLAR_LOCKED);

	write_dbg_dtrrxext(dbg->dtrrx_e);
	write_dbg_dtrtxext(dbg->dtrtx_e);
	write_dbg_dscrext(dbg->dscr_e);
	write_dbg_wfar(dbg->wfar);
	write_dbg_vcr(dbg->vcr);
	write_dbg_claimset(dbg->claimclr);

	if (virtext_present)
		sr_bp_context(dbg, DBGREG_BP_XVAL, 0);

	sr_bp_context(dbg, DBGREG_BP_VAL, 0);
	sr_bp_context(dbg, DBGREG_BP_CTRL, 0);
	sr_bp_context(dbg, DBGREG_WP_VAL, 0);
	sr_bp_context(dbg, DBGREG_WP_CTRL, 0);
	isb();

	/*
	 * Unlock access to the debug registers
	 */
	write_dbg_oslar(OSLAR_UNLOCKED);

	return;
}

debug_registers_t *read_debug_address(void)
{
	unsigned drar, dsar;

	drar = read_dbg_drar();
	dsar = read_dbg_dsar();

	if (!(drar & DRAR_VALID_MASK)
			|| !(dsar & DSAR_VALID_MASK)) {
		return 0;   /* No memory-mapped debug on this processor */
	}

	return (debug_registers_t *) ((drar & DRAR_ADDRESS_MASK)
			+ (dsar & DSAR_ADDRESS_MASK));
}

/*
 * We assume that before save (and after restore):
 *   - OSLAR is NOT locked, or the debugger would not work properly
 *   - LAR is locked, because the ARM ARM says it must be
 *   - OSDLR is NOT locked, or the debugger would not work properly
 */

static void save_v7_debug_mmapped(unsigned *context)
{
	debug_registers_t *dbg = (void *)read_debug_address();
	debug_context_t *ctx = (void *)context;
	unsigned v71, num_bps, num_wps, i;
	unsigned didr, virtext_present = (read_id_pfr1() >> 12) & 0xf;

	if (!dbg) {
		return;
	}

	didr = dbg->didr;
	/*
	 * Work out what version of debug we have
	 */
	v71 =
		(((didr >> DIDR_VERSION_SHIFT) & DIDR_VERSION_MASK) ==
		 DIDR_VERSION_7_1);

	/*
	 * Save all context to memory
	 */
	ctx->vcr = dbg->vcr;
	ctx->dscr_e = dbg->dscr_e;
	ctx->claim = dbg->claimclr;

	if (v71) {
		ctx->eacr = dbg->eacr;
	}

	num_bps = 1 + ((didr >> DIDR_BP_SHIFT) & DIDR_BP_MASK);
	for (i = 0; i < num_bps; ++i) {
		ctx->bvr[i] = dbg->bvr[i];
		ctx->bcr[i] = dbg->bcr[i];
		if (virtext_present)
			ctx->bxvr[i] = dbg->bxvr[i];
	}

	num_wps = 1 + ((didr >> DIDR_WP_SHIFT) & DIDR_WP_MASK);
	for (i = 0; i < num_wps; ++i) {
		ctx->wvr[i] = dbg->wvr[i];
		ctx->wcr[i] = dbg->wcr[i];
	}

	/*
	 * If Debug V7.1, we must set osdlr (by cp14 interface) before power down.
	 * Once we have done this, debug becomes inaccessible.
	 */
	if (v71) {
		write_dbg_osdlr(OSDLR_LOCKED);
	}
}

static void restore_v7_debug_mmapped(unsigned *context)
{
	debug_registers_t *dbg = (void *)read_debug_address();
	debug_context_t *ctx = (void *)context;
	unsigned v71, num_bps, num_wps, i;
	unsigned didr, virtext_present = (read_id_pfr1() >> 12) & 0xf;;

	if (!dbg) {
		return;
	}

	didr = dbg->didr;
	/*
	 * Work out what version of debug we have
	 */
	v71 =
		(((didr >> DIDR_VERSION_SHIFT) & DIDR_VERSION_MASK) ==
		 DIDR_VERSION_7_1);

	/* Enable write access to registers */
	dbg->lar = LAR_UNLOCKED;
	/*
	 * If Debug V7.1, we must unset osdlr (by cp14 interface) before restoring.
	 * (If the CPU has not actually power-cycled, osdlr may not be reset).
	 */
	if (v71) {
		write_dbg_osdlr(OSDLR_UNLOCKED);
	}

	/*
	 * Restore all context from memory
	 */
	dbg->vcr = ctx->vcr;
	dbg->claimclr = CLAIMCLR_CLEAR_ALL;
	dbg->claimset = ctx->claim;

	if (v71) {
		dbg->eacr = ctx->eacr;
	}

	num_bps = 1 + ((didr >> DIDR_BP_SHIFT) & DIDR_BP_MASK);
	for (i = 0; i < num_bps; ++i) {
		dbg->bvr[i] = ctx->bvr[i];
		dbg->bcr[i] = ctx->bcr[i];
		if (virtext_present)
			dbg->bxvr[i] = ctx->bxvr[i];
	}

	num_wps = 1 + ((didr >> DIDR_WP_SHIFT) & DIDR_WP_MASK);
	for (i = 0; i < num_wps; ++i) {
		dbg->wvr[i] = ctx->wvr[i];
		dbg->wcr[i] = ctx->wcr[i];
	}

	/* Clear PRSR.SPD by reading PRSR */
	if (!v71) {
		(dbg->prsr);
	}

	/* Re-enable debug */
	dbg->dscr_e = ctx->dscr_e;

	/* Disable write access to registers */
	dbg->lar = LAR_LOCKED;
}

void save_v7_debug(unsigned *context)
{
	unsigned v71 = 0, didr = read_dbg_didr();

	v71 = (((didr >> DIDR_VERSION_SHIFT) & DIDR_VERSION_MASK) ==
			DIDR_VERSION_7_1);

	/*
	 * TODO: Code for saving the v7.0 Debug context through the
	 * cp14 interface has not been implemented as yet.
	 */
	if (v71)
		save_v71_debug_cp14(context);
	else
		save_v7_debug_mmapped(context);

	return;
}

void restore_v7_debug(unsigned *context)
{
	unsigned v71 = 0, didr = read_dbg_didr();

	v71 = (((didr >> DIDR_VERSION_SHIFT) & DIDR_VERSION_MASK) ==
			DIDR_VERSION_7_1);

	/*
	 * TODO: Code for restoring the v7.0 Debug context through the
	 * cp14 interface has not been implemented as yet.
	 */
	if (v71)
		restore_v71_debug_cp14(context);
	else
		restore_v7_debug_mmapped(context);

	return;
}

void stop_generic_timer(generic_timer_context * ctr_ctx)
{
	/*
	 * Disable the timer and mask the irq to prevent
	 * suprious interrupts on this cpu interface. It
	 * will bite us when we come back if we don't. It
	 * will be replayed on the inbound cluster.
	 */
	write_cntp_ctl(TIMER_MASK_IRQ);

#if 0 // wait GIC APIs by Sten
	/*
	 * If the local timer interrupt was being used as
	 * the asynchronous trigger, then it was disabled
	 * in handle_interrupt() to prevent this level-
	 * triggerred interrupt from firing. Now that its
	 * been acked at the peripheral. We can renable it
	 */
	if (!hyp_timer_trigger) {
		if (ctr_ctx->cntp_ctl & TIMER_IRQ_STAT)
			gic_enable_int(LCL_TIMER_IRQ);
	}
#endif

	return;
}

void save_pmu_context(unsigned cluster_id, unsigned cpu_id)
{
	switch (pmu_mode) {
		case PMU_STATE1:
			save_performance_monitors(migration_ctx[cpu_id]);
			break;
		case PMU_STATE2:
			save_performance_monitors(clusters_ctx[cluster_id][cpu_id]);
			break;
		case PMU_STATE0:
		default:
			break;
	};
}

void restore_pmu_context(unsigned cluster_id, unsigned cpu_id)
{
	switch (pmu_mode) {
		case PMU_STATE1:
			restore_performance_monitors(migration_ctx[cpu_id]);
			break;
		case PMU_STATE2:
			restore_performance_monitors(clusters_ctx[cluster_id][cpu_id]);
			break;
		case PMU_STATE0:
		default:
			break;
	};
}

static void platform_save_context(void)
{
	unsigned cpu_id = read_cpuid(), cluster_id = read_clusterid();
	cpu_context *ns_cpu_ctx = &switcher_context.cluster.core[cpu_id].ns_cpu_ctx;
	unsigned *gp_context = ns_cpu_ctx->banked_cpu_regs;
	unsigned *vfp_context = ns_cpu_ctx->vfp_regs;
	//debug_context_t *debug_context = &ns_cpu_ctx->debug_ctx;
	unsigned int *dbg_ctx = ns_cpu_ctx->dbg_data;
	banked_cp15_context *cp15_context = &ns_cpu_ctx->banked_cp15_regs;
	generic_timer_context *cp15_timer_ctx = &ns_cpu_ctx->cp15_timer_ctx;
	cp15_fault_regs *fault_ctx = &cp15_context->ns_cp15_fault_regs;
	gic_cpu_context *gic_pvt_context = &ns_cpu_ctx->gic_cpu_ctx;
	global_context *gbl_context = &switcher_context.cluster.ns_cluster_ctx;

	//printk("[platform_save_context] cpu_id=%d", cpu_id);

	/*
	 * Save the 32-bit Generic timer context & stop them
	 */
	save_generic_timer((unsigned *)cp15_timer_ctx, 0x0);
	stop_generic_timer(cp15_timer_ctx);

	/*
	 * Save v7 generic performance monitors
	 * Save cpu general purpose banked registers
	 * Save cp15 context
	 */
	save_pmu_context(cluster_id, cpu_id);
	//save_banked_registers(gp_context); // move to tail
	save_cp15(cp15_context->cp15_misc_regs);
	save_control_registers(cp15_context->cp15_ctrl_regs, 0x0);
	save_mmu(cp15_context->cp15_mmu_regs);
	save_fault_status((unsigned *)fault_ctx);

	save_vfp(vfp_context);

#if 1 // GIC APIs by Sten
	if(cpu_id==0)
	{
		/* Save cpu interface (cpu view) context */
		save_gic_interface(gic_pvt_context->gic_cpu_if_regs,GIC_CPU_BASE);
		/*
		 * TODO:
		 * Is it safe for the secondary cpu to save its context
		 * while the GIC distributor is on. Should be as its
		 * banked context and the cpu itself is the only one
		 * who can change it. Still have to consider cases e.g
		 * SGIs/Localtimers becoming pending.
		 */
		/* Save distributoer interface private context */
		save_gic_distributor_private(gic_pvt_context->gic_dist_if_pvt_regs,GIC_DIST_BASE);
		/* Save distributoer interface global context */
		save_gic_distributor_shared(gbl_context->gic_dist_if_regs,GIC_DIST_BASE);
	}
#endif

	//save_v7_debug((unsigned *)debug_context);
	//for save/restore breakpoint and watchpoint
	save_dbg_regs(dbg_ctx);

	save_mcu_biu_register();

	if (power_state[cpu_id] == STATUS_DORMANT) {
		/* disable L2 invalidate when reset */
		reg_write(CA7_CACHE_CONFIG, reg_read(CA7_CACHE_CONFIG) | (1U << 4));
	}

	dormant_ret_flag[cpu_id] = 0;

	save_banked_registers(gp_context);
}

static void platform_restore_context(void)
{
	unsigned cpu_id = read_cpuid();
	unsigned cluster_id = read_clusterid();

	cpu_context *ns_cpu_ctx = (cpu_context *)
		__pa(&switcher_context.cluster.core[cpu_id].ns_cpu_ctx);
	global_context *gbl_context = (global_context *)
		__pa(&switcher_context.cluster.ns_cluster_ctx);

	unsigned *vfp_context = ns_cpu_ctx->vfp_regs;
	//debug_context_t *debug_context = &ns_cpu_ctx->debug_ctx;
	//generic_timer_context *cp15_timer_ctx = &ns_cpu_ctx->cp15_timer_ctx;
	banked_cp15_context *cp15_context = &ns_cpu_ctx->banked_cp15_regs;
	cp15_fault_regs *fault_ctx = &cp15_context->ns_cp15_fault_regs;
	gic_cpu_context *gic_pvt_context = &ns_cpu_ctx->gic_cpu_ctx;

	cpu_context *ns_cpu_ctx_v = &switcher_context.cluster.core[cpu_id].ns_cpu_ctx;
	unsigned *gp_context_v = ns_cpu_ctx_v->banked_cpu_regs;
	banked_cp15_context *cp15_context_v = &ns_cpu_ctx_v->banked_cp15_regs;
	generic_timer_context *cp15_timer_ctx_v = &ns_cpu_ctx_v->cp15_timer_ctx;
	unsigned int *dbg_ctx = ns_cpu_ctx_v->dbg_data;

#if 1 // GIC APIs by Sten
	if(cpu_id==0) {
		/*restores the global context  */
		restore_gic_distributor_shared(gbl_context->gic_dist_if_regs,
				IO_VIRT_TO_PHYS(GIC_DIST_BASE));
		/*restores the private context  */
		restore_gic_distributor_private(gic_pvt_context->gic_dist_if_pvt_regs,
				IO_VIRT_TO_PHYS(GIC_DIST_BASE));
		/* Restore GIC context */
		restore_gic_interface(gic_pvt_context->gic_cpu_if_regs,
				IO_VIRT_TO_PHYS(GIC_CPU_BASE));
	}
#endif

	isb();
	dsb();
	restore_vfp(vfp_context);
	/*
	 * Restore cp15 context
	 * Restore cpu general purpose banked registers
	 * Restore v7 generic performance monitors
	 * Restore the 32-bit Generic timer context
	 */
	restore_fault_status((unsigned *)fault_ctx);

	restore_mmu(cp15_context->cp15_mmu_regs);

	invalidate_unified_TLB_inner_shareable();

	mt_restore_control_registers(cp15_context->cp15_ctrl_regs, 0x0);
	isb();
	dsb();

	//now MMU is restored, welcome to virtual world

	restore_cp15(cp15_context_v->cp15_misc_regs);

	restore_pmu_context(cluster_id, cpu_id);

	restore_generic_timer((unsigned *)cp15_timer_ctx_v, 0x0);

	//restore_v7_debug((unsigned *)debug_context);
	restore_dbg_regs(dbg_ctx);

	restore_mcu_biu_register();

	if (power_state[cpu_id] == STATUS_DORMANT) {
		/* enable L2 invalidate when reset */
		reg_write(CA7_CACHE_CONFIG,
				reg_read(CA7_CACHE_CONFIG) & ~(1U << 4));
	}
#if 0 /* FIXME early porting */
	trace_stop_dormant();
	trace_start_dormant();
#endif
	if (power_state[cpu_id] == STATUS_DORMANT)
		dormant_ret_flag[cpu_id] = 1;
	else if (power_state[cpu_id] == STATUS_SHUTDOWN)
		dormant_ret_flag[cpu_id] = 2;

	power_state[cpu_id] = STATUS_RUN;

	restore_banked_registers(gp_context_v);
}

int cpu_power_down(int mode)
{
	unsigned cpu_id;

	void (*restore_ptr)(void);
	restore_ptr = cpu_wake_up;

	cpu_id = read_cpuid();

#if !defined(CONFIG_TRUSTONIC_TEE)
	reg_write(BOOTROM_BOOT_ADDR, __pa(restore_ptr));
#else
	mt_secure_call(MC_FC_SLEEP, __pa(restore_ptr), cpu_id, 0);
#endif

#if defined(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT)
	*((unsigned int*)NS_SLAVE_BOOT_ADDR) = __pa(restore_ptr);
#endif

	power_state[cpu_id] = mode;

#if 1
	__disable_dcache();

	dsb();
	__asm__ __volatile__("MCR p15,0,r0,c8,c7,1");
	dsb();

	if (power_state[cpu_id] == STATUS_DORMANT) {
		/* Flush all data from the L1 data cache */
		inner_dcache_flush_L1();

		/* Clean all data from the L2 data cache */
		__inner_clean_dcache_L2();
	} else {
		/* Clean and invalidate all data from the L1 +L2 data cache */
		inner_dcache_flush_all();
	}

	/* Execute a CLREX instruction */
	__asm__ __volatile__("clrex");
#endif

	platform_save_context();

#if defined(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT)
	// After we returned from dormant, platform_restore_context
	// will bring us back to here.
	// Check dormant_ret_flag and only enter TrustZone dormant
	// if we are not on restore path
	if (!dormant_ret_flag[cpu_id] && !kree_pm_cpu_dormant(mode))
	{
		if (power_state[cpu_id] == STATUS_DORMANT) {
			dormant_ret_flag[cpu_id] = 1;
		} else if (power_state[cpu_id] == STATUS_SHUTDOWN) {
			dormant_ret_flag[cpu_id] = 2;
		}
	}
#endif

	return dormant_ret_flag[cpu_id];
}

void cpu_start_restore(void)
{
	platform_restore_context();
}

void cpu_dormant_init(void)
{
	//set Boot ROM power-down control to power down
	reg_write(BOOTROM_PWR_CTRL, reg_read(BOOTROM_PWR_CTRL) | 0x80000000);
}

void cpu_check_dormant_abort(void)
{
	unsigned cpu_id;
	cpu_id = read_cpuid();

	if (power_state[cpu_id] != STATUS_RUN) {
		//unsigned cpu_id;
		unsigned cluster_id;
		cpu_context *ns_cpu_ctx;
		generic_timer_context *cp15_timer_ctx;

		__enable_cache();

		//cpu_id = read_cpuid();
		cluster_id = read_clusterid();
		ns_cpu_ctx = &switcher_context.cluster.core[cpu_id].ns_cpu_ctx;
		cp15_timer_ctx = &ns_cpu_ctx->cp15_timer_ctx;

		// restore timer and performance monitor
		restore_generic_timer((unsigned *)cp15_timer_ctx, 0x0);
		restore_pmu_context(cluster_id, cpu_id);

		if (power_state[cpu_id] == STATUS_DORMANT) {
			/* enable L2 invalidate when reset */
			reg_write(CA7_CACHE_CONFIG,
					reg_read(CA7_CACHE_CONFIG) & ~(1U << 4));
		}

		power_state[cpu_id] = STATUS_RUN;
	} else {
		if ((cpu_id == 0) && (dormant_ret_flag[cpu_id] == 2)) {
			/* workaround for ARM CA7 Errata 802022 */
			extern void cpu_wake_up_errata_802022(void);
			reg_write(BOOTROM_BOOT_ADDR, virt_to_phys(cpu_wake_up_errata_802022));
#if defined(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT)
			kree_pm_cpu_dormant_workaround_wake(1);
#endif

			if (num_possible_cpus() == 4) {
				spm_mtcmos_ctrl_cpu1(STA_POWER_ON, 1);
				spm_mtcmos_ctrl_cpu2(STA_POWER_ON, 1);
				spm_mtcmos_ctrl_cpu3(STA_POWER_ON, 1);

				spm_mtcmos_ctrl_cpu3(STA_POWER_DOWN, 1);
				spm_mtcmos_ctrl_cpu2(STA_POWER_DOWN, 1);
				spm_mtcmos_ctrl_cpu1(STA_POWER_DOWN, 1);
			} else if (num_possible_cpus() == 3) {
				spm_mtcmos_ctrl_cpu1(STA_POWER_ON, 1);
				spm_mtcmos_ctrl_cpu2(STA_POWER_ON, 1);

				spm_mtcmos_ctrl_cpu2(STA_POWER_DOWN, 1);
				spm_mtcmos_ctrl_cpu1(STA_POWER_DOWN, 1);
			} else if (num_possible_cpus() == 2) {
				spm_mtcmos_ctrl_cpu1(STA_POWER_ON, 1);

				spm_mtcmos_ctrl_cpu1(STA_POWER_DOWN, 1);
			}

			/* set to normal boot address after workaround finish */
#if defined(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT)
			kree_pm_cpu_dormant_workaround_wake(0);
#endif
		}

		__enable_cache();
	}
}

MODULE_AUTHOR("Wan-Ching Huang <marc.huang@mediatek.com>");
MODULE_DESCRIPTION("MT658x Dormant/Shutdown Mode Driver $Revision: #1 $");

