#ifndef _MT_GPT_H_
#define _MT_GPT_H_

#include <mach/mt_typedefs.h>

#define GPT1            0x0
#define GPT2            0x1
#define GPT3            0x2
#define GPT4            0x3
#define GPT5            0x4
#define GPT6            0x5
#define GPT7            0x6
#define NR_GPTS         0x7


#define GPT_ONE_SHOT    0x0
#define GPT_REPEAT      0x1
#define GPT_KEEP_GO     0x2
#define GPT_FREE_RUN    0x3


#define GPT_CLK_DIV_1   0x0000
#define GPT_CLK_DIV_2   0x0001
#define GPT_CLK_DIV_3   0x0002
#define GPT_CLK_DIV_4   0x0003
#define GPT_CLK_DIV_5   0x0004
#define GPT_CLK_DIV_6   0x0005
#define GPT_CLK_DIV_7   0x0006
#define GPT_CLK_DIV_8   0x0007
#define GPT_CLK_DIV_9   0x0008
#define GPT_CLK_DIV_10  0x0009
#define GPT_CLK_DIV_11  0x000a
#define GPT_CLK_DIV_12  0x000b
#define GPT_CLK_DIV_13  0x000c
#define GPT_CLK_DIV_16  0x000d
#define GPT_CLK_DIV_32  0x000e
#define GPT_CLK_DIV_64  0x000f

#define GPT_CLK_SRC_SYS 0x0
#define GPT_CLK_SRC_RTC 0x1

#define GPT_NOAUTOEN    0x0001
#define GPT_NOIRQEN     0x0002

#define GPT4_1MS_TICK       ((U32)13000)        // 1000000 / 76.92ns = 13000.520
#define GPT_IRQEN           (APMCU_GPTIMER_BASE + 0x00)
#define GPT_IRQSTA          (APMCU_GPTIMER_BASE + 0x04)
#define GPT_IRQACK          (APMCU_GPTIMER_BASE + 0x08)
#define GPT1_BASE           (APMCU_GPTIMER_BASE + 0x10)
#define GPT4_BASE           (APMCU_GPTIMER_BASE + 0x40)
#define GPT7_BASE           (APMCU_GPTIMER_BASE + 0x8C)

#define GPT_CON             (0x00)
#define GPT_CLK             (0x04)
#define GPT_CNT             (0x08)
#define GPT_CMP             (0x0C)
#define GPT_CNTH            (0x18)
#define GPT_CMPH            (0x1C)

#define GPT_CON_ENABLE      (0x1 << 0)
#define GPT_CON_CLRCNT      (0x1 << 1)
#define GPT_CON_OPMODE      (0x3 << 4)

#define GPT_CLK_CLKDIV      (0xf << 0)
#define GPT_CLK_CLKSRC      (0x1 << 4)

#define GPT_OPMODE_MASK     (0x3)
#define GPT_CLKDIV_MASK     (0xf)
#define GPT_CLKSRC_MASK     (0x1)

#define GPT_OPMODE_OFFSET   (4)
#define GPT_CLKSRC_OFFSET   (4)

#define GPT_ISR             (0x0010)
#define GPT_IN_USE          (0x0100)

#define GPT_FEAT_64_BIT     (0x0001)

struct gpt_device {
	unsigned int id;
	unsigned int mode;
	unsigned int clksrc;
	unsigned int clkdiv;
	unsigned int cmp[2];
	void (*func)(unsigned long);
	int flags;
	int features;
	unsigned int base_addr;
};

extern int request_gpt(unsigned int id, unsigned int mode, unsigned int clksrc,
                unsigned int clkdiv, unsigned int cmp,
                void (*func)(unsigned long), unsigned int flags);
extern int free_gpt(unsigned int id);

extern int start_gpt(unsigned int id);
extern int stop_gpt(unsigned int id);
extern int restart_gpt(unsigned int id);

extern int gpt_is_counting(unsigned int id);

extern int gpt_set_cmp(unsigned int id, unsigned int val);
extern int gpt_get_cmp(unsigned int id, unsigned int *ptr);
extern int gpt_get_cnt(unsigned int id, unsigned int *ptr);

extern int gpt_check_irq(unsigned int id);
extern int gpt_check_and_ack_irq(unsigned int id);

#endif
