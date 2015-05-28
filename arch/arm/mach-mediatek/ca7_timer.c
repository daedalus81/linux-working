#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/smp.h>
#include <linux/jiffies.h>
#include <linux/clockchips.h>
#include <linux/interrupt.h>

#include <asm/localtimer.h>


#define read_cntfrq(cntfrq) \
	do {    \
		__asm__ __volatile__(   \
				"MRC p15, 0, %0, c14, c0, 0\n"  \
				:"=r"(cntfrq)   \
				:   \
				:"memory"); \
	} while (0)

#define change_cntfrq(cntfrq) \
	do {    \
		__asm__ __volatile__(   \
				"MCR p15, 0, %0, c14, c0, 0\n"  \
				:   \
				:"r"(cntfrq));  \
	} while (0)

#define read_cntkctl(cntkctl)   \
	do {    \
		__asm__ __volatile__(   \
				"MRC p15, 0, %0, c14, c1, 0\n"  \
				:"=r"(cntkctl)  \
				:   \
				:"memory"); \
	} while (0)

#define read_cntpct(cntpct_lo, cntpct_hi)   \
	do {    \
		__asm__ __volatile__(   \
				"MRRC p15, 0, %0, %1, c14\n"    \
				:"=r"(cntpct_lo), "=r"(cntpct_hi)   \
				:   \
				:"memory"); \
	} while (0)

#define read_cntvct(cntvct_lo, cntvct_hi)   \
	do {    \
		__asm__ __volatile__(   \
				"MRRC p15, 1, %0, %1, c14\n"    \
				:"=r"(cntvct_lo), "=r"(cntvct_hi)   \
				:   \
				:"memory"); \
	} while (0)

#define read_cntp_ctl(cntp_ctl)   \
	do {    \
		__asm__ __volatile__(   \
				"MRC p15, 0, %0, c14, c2, 1\n"  \
				:"=r"(cntp_ctl) \
				:   \
				:"memory"); \
	} while (0)

#define write_cntp_ctl(cntp_ctl)  \
	do {    \
		__asm__ __volatile__(   \
				"MCR p15, 0, %0, c14, c2, 1\n"  \
				:   \
				:"r"(cntp_ctl)); \
	} while (0)


#define read_cntp_cval(cntp_cval_lo, cntp_cval_hi) \
	do {    \
		__asm__ __volatile__(   \
				"MRRC p15, 2, %0, %1, c14\n"    \
				:"=r"(cntp_cval_lo), "=r"(cntp_cval_hi) \
				:   \
				:"memory"); \
	} while (0)

#define write_cntp_cval(cntp_cval_lo, cntp_cval_hi) \
	do {    \
		__asm__ __volatile__(   \
				"MCRR p15, 2, %0, %1, c14\n"    \
				:   \
				:"r"(cntp_cval_lo), "r"(cntp_cval_hi));    \
	} while (0)

#define read_cntp_tval(cntp_tval) \
	do {    \
		__asm__ __volatile__(   \
				"MRC p15, 0, %0, c14, c2, 0"    \
				:"=r"(cntp_tval)    \
				:   \
				:"memory"); \
	} while (0)

#define write_cntp_tval(cntp_tval) \
	do {    \
		__asm__ __volatile__(   \
				"MCR p15, 0, %0, c14, c2, 0\n"    \
				:   \
				:"r"(cntp_tval));    \
	} while (0)


#define read_cntv_ctl(cntv_ctl)   \
	do {    \
		__asm__ __volatile__(   \
				"MRC p15, 0, %0, c14, c3, 1\n"  \
				:"=r"(cntv_ctl) \
				:   \
				:"memory"); \
	} while (0)

#define read_cntv_cval(cntv_cval_lo, cntv_cval_hi) \
	do {    \
		__asm__ __volatile__(   \
				"MRRC p15, 3, %0, %1, c14\n"    \
				:"=r"(cntv_cval_lo), "=r"(cntv_cval_hi) \
				:   \
				:"memory"); \
	} while (0)

#define read_cntv_tval(cntv_tval) \
	do {    \
		__asm__ __volatile__(   \
				"MRC p15, 0, %0, c14, c3, 0"    \
				:"=r"(cntv_tval)    \
				:   \
				:"memory"); \
	} while (0)


#define CNTP_CTL_ENABLE     (1 << 0)
#define CNTP_CTL_IMASK      (1 << 1)
#define CNTP_CTL_ISTATUS    (1 << 2)

#define MT_LOCAL_TIMER_DEBUG
static void save_localtimer_info(unsigned long evt, int ext);

static unsigned long generic_timer_rate;

static struct clock_event_device __percpu **timer_evt;
static int timer_ppi;

static void generic_timer_set_mode(enum clock_event_mode mode,
		struct clock_event_device *clk)
{
	unsigned int ctrl;

	switch (mode) {
		case CLOCK_EVT_MODE_ONESHOT:
			ctrl = CNTP_CTL_ENABLE;
			break;
		case CLOCK_EVT_MODE_PERIODIC:
		case CLOCK_EVT_MODE_UNUSED:
		case CLOCK_EVT_MODE_SHUTDOWN:
		default:
			ctrl = CNTP_CTL_IMASK;
	}

	write_cntp_ctl(ctrl);
}

static int generic_timer_set_next_event(unsigned long evt,
		struct clock_event_device *unused)
{
	write_cntp_tval(evt);
	write_cntp_ctl(CNTP_CTL_ENABLE);

	save_localtimer_info(evt, 0);

	return 0;
}

int localtimer_set_next_event(unsigned long evt)
{
	generic_timer_set_next_event(evt, NULL);

	save_localtimer_info(evt, 1);

	return 0;
}

unsigned long localtimer_get_counter(void)
{
	unsigned long evt;
	read_cntp_tval(evt);

	return evt;
}


/*
 * generic_timer_ack: checks for a local timer interrupt.
 *
 * If a local timer interrupt has occurred, acknowledge and return 1.
 * Otherwise, return 0.
 */
static int generic_timer_ack(void)
{
	unsigned int cntp_ctl;
	read_cntp_ctl(cntp_ctl);

	if (cntp_ctl & CNTP_CTL_ISTATUS) {
		write_cntp_ctl(CNTP_CTL_IMASK);
		return 1;
	}

	printk("WARNING: Generic Timer CNTP_CTL = 0x%x\n", cntp_ctl);
	return 0;
}

static void generic_timer_stop(struct clock_event_device *clk)
{
	generic_timer_set_mode(CLOCK_EVT_MODE_UNUSED, clk);
	disable_percpu_irq(clk->irq);
}

static void __cpuinit generic_timer_calibrate_rate(void)
{
	unsigned long count;
	u64 waitjiffies;

	/*
	 * If this is the first time round, we need to work out how fast
	 * the timer ticks
	 */
	if (generic_timer_rate == 0) {
		printk("Calibrating local timer... ");

		/* Wait for a tick to start */
		waitjiffies = get_jiffies_64() + 1;

		while (get_jiffies_64() < waitjiffies)
			udelay(10);

		/* OK, now the tick has started, let's get the timer going */
		waitjiffies += 5;

		/* enable, no interrupt or reload */
		write_cntp_ctl(CNTP_CTL_ENABLE | CNTP_CTL_IMASK);

		/* maximum value */
		write_cntp_tval(0xFFFFFFFFU);

		while (get_jiffies_64() < waitjiffies)
			udelay(10);

		read_cntp_tval(count);
		generic_timer_rate = (0xFFFFFFFFU - count) * (HZ / 5);

		printk("%lu.%02luMHz.\n", generic_timer_rate / 1000000,
				(generic_timer_rate / 10000) % 100);
	}
}

static irqreturn_t timer_handler(int irq, void *dev_id)
{
	struct clock_event_device *evt = *(struct clock_event_device **)dev_id;
	//#ifdef CONFIG_MT_SCHED_MONITOR
#if 0
	// add timer event tracer for wdt debug
	__raw_get_cpu_var(local_timer_ts) = sched_clock();
	if (generic_timer_ack()) {
		evt->event_handler(evt);
		__raw_get_cpu_var(local_timer_te) = sched_clock();
		return IRQ_HANDLED;
	}
	__raw_get_cpu_var(local_timer_te) = sched_clock();
	return IRQ_NONE;
#else
	if (generic_timer_ack()) {
		evt->event_handler(evt);
		return IRQ_HANDLED;
	}
	return IRQ_NONE;
#endif
}


/*
 * Setup the local clock events for a CPU.
 */
static int __cpuinit generic_timer_setup(struct clock_event_device *clk)
{
	struct clock_event_device **this_cpu_clk;

	pr_info("[ca7_timer]%s entry\n", __func__);
	generic_timer_calibrate_rate();

	write_cntp_ctl(0x0);

	clk->name = "generic_timer";
	clk->features = CLOCK_EVT_FEAT_ONESHOT;
	clk->rating = 350;
	clk->set_mode = generic_timer_set_mode;
	clk->set_next_event = generic_timer_set_next_event;
	clk->irq = timer_ppi;

	this_cpu_clk = __this_cpu_ptr(timer_evt);
	*this_cpu_clk = clk;

	clockevents_config_and_register(clk, generic_timer_rate,
			0xf, 0x7fffffff);

	enable_percpu_irq(clk->irq, 0);

	return 0;
}

static struct local_timer_ops generic_timer_ops __cpuinitdata = {
	.setup  = generic_timer_setup,
	.stop   = generic_timer_stop,
};

int __init generic_timer_register(void)
{
	int err;

	if (timer_evt)
		return -EBUSY;

	/* When TEE is enabled, change to use non-secure local timer */
#if defined(CONFIG_TRUSTONIC_TEE_SUPPORT) || defined(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT)
	timer_ppi = GIC_PPI_NS_PRIVATE_TIMER;
#else
	timer_ppi = GIC_PPI_PRIVATE_TIMER;
#endif

	timer_evt = alloc_percpu(struct clock_event_device *);

	if (!timer_evt) {
		err = -ENOMEM;
		goto out_exit;
	}

	err = request_percpu_irq(timer_ppi, timer_handler, "timer", timer_evt);
	if (err) {
		pr_err("generic timer: can't register interrupt %d (%d)\n",
				timer_ppi, err);
		goto out_free;
	}

	err = local_timer_register(&generic_timer_ops);
	if (err)
		goto out_irq;

	return 0;

out_irq:
	free_percpu_irq(timer_ppi, timer_evt);
out_free:
	free_percpu(timer_evt);
	timer_evt = NULL;
out_exit:
	return err;
}

#ifdef MT_LOCAL_TIMER_DEBUG
#include <linux/sched.h>

struct localtimer_info {
	unsigned long evt;
	unsigned int ctrl;
	int ext;
	unsigned long long timestamp;
};

static struct localtimer_info save_data[NR_CPUS];

static void save_localtimer_info(unsigned long evt, int ext)
{
	int cpu;
	unsigned int ctrl;

	cpu = smp_processor_id();
	read_cntp_ctl(ctrl);

	save_data[cpu].evt = evt;
	save_data[cpu].ctrl = ctrl;
	save_data[cpu].ext = ext;
	save_data[cpu].timestamp = sched_clock();
}

int dump_localtimer_info(char* buffer, int size)
{
	int i;
	int len = 0;
#define LOCAL_LEN   256
	char fmt[LOCAL_LEN];

	unsigned int cntp_ctl;
	unsigned int cntp_tval;
	unsigned int cntp_cval_lo, cntp_cval_hi;
	unsigned int cntpct_lo, cntpct_hi;

	if (!buffer || size <= 1) {
		return 0;
	}

	len += snprintf(fmt + len, LOCAL_LEN - len,
			"[localtimer]cpu evt ctl ext time\n");

	for (i = 0; i < nr_cpu_ids; i++) {
		len += snprintf(fmt + len, LOCAL_LEN - len, "%d %lx %x %d %llx\n",
				i, save_data[i].evt, save_data[i].ctrl,
				save_data[i].ext, save_data[i].timestamp);
	}

	read_cntp_ctl(cntp_ctl);
	read_cntp_cval(cntp_cval_lo, cntp_cval_hi);
	read_cntp_tval(cntp_tval);
	read_cntpct(cntpct_lo, cntpct_hi);

	len += snprintf(fmt + len, LOCAL_LEN - len, "cpu ctl tval cval pct\n");
	len += snprintf(fmt + len, LOCAL_LEN - len,
			"%d %x %x (%x,%x) (%x,%x)\n",
			smp_processor_id(), cntp_ctl, cntp_tval,
			cntp_cval_lo, cntp_cval_hi, cntpct_lo, cntpct_hi);

	len = min(len, size - 1);
	memcpy(buffer, fmt, len);
	*(buffer + len) = '\0';

	return len;
}
#else

static inline void save_localtimer_info(unsigned long evt, int ext)
{
	return ;
}

int dump_localtimer_info(char* buffer, int size)
{
	return 0;
}

#endif

