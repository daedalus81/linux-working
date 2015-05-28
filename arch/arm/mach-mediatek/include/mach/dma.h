#ifndef __ASM_ARCH_DMA_H
#define __ASM_ARCH_DMA_H

#define MAX_DMA_ADDRESS     (0xFFFFFFFF)
#define MAX_DMA_CHANNELS    (0)

#endif  /* !__ASM_ARCH_DMA_H */

#ifndef __MT_DMA_H__
#define __MT_DMA_H__

/* define DMA channels */
enum {
    G_DMA_1 = 0, G_DMA_2,
    P_DMA_AP_HIF, P_DMA_MD_HIF,
    P_DMA_SIM1, P_DMA_SIM2,
    P_DMA_IRDA,
    P_DMA_UART1_TX, P_DMA_UART1_RX,
    P_DMA_UART2_TX, P_DMA_UART2_RX,
    P_DMA_UART3_TX, P_DMA_UART3_RX,
};

/* define DMA error code */
enum {
    DMA_ERR_CH_BUSY = 1,
    DMA_ERR_INVALID_CH = 2,
    DMA_ERR_CH_FREE = 3,
    DMA_ERR_NO_FREE_CH = 4,
    DMA_ERR_INV_CONFIG = 5,
};

/* define DMA ISR callback function's prototype */
typedef void (*DMA_ISR_CALLBACK)(void *);

typedef enum
{
    DMA_FALSE = 0,
    DMA_TRUE
} DMA_BOOL;

typedef enum
{
    DMA_OK = 0,
    DMA_FAIL
} DMA_STATUS;

typedef enum
{
    REMAINING_LENGTH = 0, /* not valid for virtual FIFO */
    VF_READPTR,           /* only valid for virtual FIFO */
    VF_WRITEPTR,          /* only valid for virtual FIFO */
    VF_FFCNT,             /* only valid for virtual FIFO */
    VF_ALERT,             /* only valid for virtual FIFO */
    VF_EMPTY,             /* only valid for virtual FIFO */
    VF_FULL,              /* only valid for virtual FIFO */
    VF_PORT
} INFO_TYPE;

typedef enum
{
    GDMA_1 = 0,
    GDMA_2,
    GDMA_ANY
} DMA_CHAN;

typedef enum
{
    ALL = 0,
    SRC,
    DST,
    SRC_AND_DST
} DMA_CONF_FLAG;

/* define GDMA configurations */
struct mt_gdma_conf
{
    unsigned int count;
    int iten;
    unsigned int burst;
    int dfix;
    int sfix;
    unsigned int limiter;
    unsigned int src;
    unsigned int dst;
    int wpen;
    int wpsd;
    unsigned int wplen;
    unsigned int wpto;
    //unsigned int cohen;
    unsigned int sec;
    void (*isr_cb)(void *);
    void *data;
};

/* burst */
#define DMA_CON_BURST_SINGLE    (0x00000000)
#define DMA_CON_BURST_2BEAT     (0x00010000)
#define DMA_CON_BURST_3BEAT     (0x00020000)
#define DMA_CON_BURST_4BEAT     (0x00030000)
#define DMA_CON_BURST_5BEAT     (0x00040000)
#define DMA_CON_BURST_6BEAT     (0x00050000)
#define DMA_CON_BURST_7BEAT     (0x00060000)
#define DMA_CON_BURST_8BEAT     (0x00070000)

/* size */
/* keep for backward compatibility only */
#define DMA_CON_SIZE_BYTE   (0x00000000)
#define DMA_CON_SIZE_SHORT  (0x00000001)
#define DMA_CON_SIZE_LONG   (0x00000002)

/*
 * DMA information
 */

#define NR_GDMA_CHANNEL     (2)
#define NR_PDMA_CHANNEL     (5)
#define NR_VFFDMA_CHANNEL   (6)
#define GDMA_START          (1)
#define NR_DMA              (NR_GDMA_CHANNEL + NR_PDMA_CHANNEL + NR_VFFDMA_CHANNEL)

/*
 * Register Definition
 */

#define DMA_BASE_CH(n)     IOMEM((AP_DMA_BASE + 0x0080 * (n + 1)))
#define DMA_GLOBAL_INT_FLAG IOMEM((AP_DMA_BASE + 0x0000))
#define DMA_GLOBAL_RUNNING_STATUS IOMEM((AP_DMA_BASE + 0x0008))
#define DMA_GLOBAL_GSEC_EN IOMEM((AP_DMA_BASE + 0x0014))
#define DMA_GDMA_SEC_EN(n) IOMEM((AP_DMA_BASE + 0x0020 + 4 * (n)))

/*
 * General DMA channel register mapping
 */
#define DMA_INT_FLAG(base)      IOMEM((base + 0x0000))
#define DMA_INT_EN(base)        IOMEM((base + 0x0004))
#define DMA_START(base)         IOMEM((base + 0x0008))
#define DMA_RESET(base)         IOMEM((base + 0x000C))
#define DMA_STOP(base)          IOMEM((base + 0x0010))
#define DMA_FLUSH(base)         IOMEM((base + 0x0014))
#define DMA_CON(base)           IOMEM((base + 0x0018))
#define DMA_SRC(base)           IOMEM((base + 0x001C))
#define DMA_DST(base)           IOMEM((base + 0x0020))
#define DMA_LEN1(base)          IOMEM((base + 0x0024))
#define DMA_LEN2(base)          IOMEM((base + 0x0028))
#define DMA_JUMP_ADDR(base)     IOMEM((base + 0x002C))
#define DMA_IBUFF_SIZE(base)    IOMEM((base + 0x0030))
#define DMA_CONNECT(base)       IOMEM((base + 0x0034))
#define DMA_AXIATTR(base)       IOMEM((base + 0x0038))
#define DMA_DBG_STAT(base)      IOMEM((base + 0x0050))

/*
 * Register Setting
 */

#define DMA_GLBSTA_RUN(ch)      (0x00000001 << ((ch)))
#define DMA_GLBSTA_IT(ch)       (0x00000001 << ((ch)))
#define DMA_GDMA_LEN_MAX_MASK   (0x000FFFFF)

#define DMA_CON_DIR             (0x00000001)
#define DMA_CON_FPEN            (0x00000002)    /* Use fix pattern. */
#define DMA_CON_SLOW_EN         (0x00000004)
#define DMA_CON_DFIX            (0x00000008)
#define DMA_CON_SFIX            (0x00000010)
#define DMA_CON_WPEN            (0x00008000)
#define DMA_CON_WPSD            (0x00100000)
#define DMA_CON_WSIZE_1BYTE     (0x00000000)
#define DMA_CON_WSIZE_2BYTE     (0x01000000)
#define DMA_CON_WSIZE_4BYTE     (0x02000000)
#define DMA_CON_RSIZE_1BYTE     (0x00000000)
#define DMA_CON_RSIZE_2BYTE     (0x10000000)
#define DMA_CON_RSIZE_4BYTE     (0x20000000)
#define DMA_CON_BURST_MASK      (0x00070000)
#define DMA_CON_SLOW_OFFSET     (5)
#define DMA_CON_SLOW_MAX_MASK   (0x000003FF)

#define DMA_START_BIT           (0x00000001)
#define DMA_STOP_BIT            (0x00000000)
#define DMA_INT_FLAG_BIT        (0x00000001)
#define DMA_INT_FLAG_CLR_BIT    (0x00000000)
#define DMA_INT_EN_BIT          (0x00000001)
#define DMA_FLUSH_BIT           (0x00000001)
#define DMA_FLUSH_CLR_BIT       (0x00000000)
#define DMA_UART_RX_INT_EN_BIT  (0x00000003)
#define DMA_INT_EN_CLR_BIT      (0x00000000)
#define DMA_WARM_RST_BIT        (0x00000001)
#define DMA_HARD_RST_BIT        (0x00000002)
#define DMA_HARD_RST_CLR_BIT    (0x00000000)
#define DMA_READ_COHER_BIT      (0x00000010)
#define DMA_WRITE_COHER_BIT     (0x00100000)
#define DMA_GSEC_EN_BIT         (0x00000001)
#define DMA_SEC_EN_BIT          (0x00000001)



/*
 * Register Limitation
 */

#define MAX_TRANSFER_LEN1   (0xFFFFF)
#define MAX_TRANSFER_LEN2   (0xFFFFF)
#define MAX_SLOW_DOWN_CNTER (0x3FF)

/*
 * channel information structures
 */

struct dma_ctrl
{
    int in_use;
    void (*isr_cb)(void *);
    void *data;
};

extern void mt_reset_gdma_conf(const unsigned int iChannel);

extern int mt_config_gdma(int channel, struct mt_gdma_conf *config, DMA_CONF_FLAG flag);
extern int mt_free_gdma(int channel);
extern int mt_req_gdma(DMA_CHAN chan);
extern int mt_start_gdma(int channel);
extern int mt_polling_gdma(int channel, unsigned long timeout);
extern int mt_stop_gdma(int channel);
extern int mt_dump_gdma(int channel);
extern int mt_warm_reset_gdma(int channel);
extern int mt_hard_reset_gdma(int channel);
extern int mt_reset_gdma(int channel);
extern void mt_dma_running_status(void);
/* This channel is used for APDMA Dummy READ.
   in MT6592 this channel will be used by Frequency hopping all the time
   .Owner: Chieh-Jay Liu
 */
#define DFS_APDMA_CHANNEL 0
#endif  /* !__MT_DMA_H__ */
