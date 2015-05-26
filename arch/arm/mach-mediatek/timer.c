#include <asm/mach/time.h>
#include <mach/mt_timer.h>

extern struct mt_clock mt6582_gpt;
extern int generic_timer_register(void);


struct mt_clock *mt6582_clocks[] = {
	&mt6582_gpt,
};

void __init mt_timer_init(void)
{
	int i;
	struct mt_clock *clock;
	int err;

	for (i = 0; i < ARRAY_SIZE(mt6582_clocks); i++) {
		clock = mt6582_clocks[i];

		clock->init_func();

		if (clock->clocksource.name) {
			if (clocksource_register(&(clock->clocksource)))
				pr_err("timer_init: clocksource_register"
						"failed for %s\n",
						clock->clocksource.name);
		}

		if (setup_irq(clock->irq.irq, &(clock->irq)))
			pr_err("timer_init: setup_irq failed for %s\n",
					clock->irq.name);

		if (clock->clockevent.name)
			clockevents_register_device(&(clock->clockevent));
	}

#ifndef CONFIG_MT7623_FPGA
	if (generic_timer_register())
		pr_err("generic_timer_register failed, err=%d\n", err);
#endif
}
