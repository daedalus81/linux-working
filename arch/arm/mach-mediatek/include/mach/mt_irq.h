#ifndef __MT_IRQ_H
#define __MT_IRQ_H

#define GIC_PRIVATE_SIGNALS     (32)
#define NR_GIC_SGI              (16)
#define NR_GIC_PPI              (16)
#define GIC_PPI_OFFSET          (27)
#define MT_NR_PPI               (5)
#define MT_NR_SPI               (224)
#define NR_MT_IRQ_LINE          (GIC_PPI_OFFSET + MT_NR_PPI + MT_NR_SPI)

#define MT65xx_EDGE_SENSITIVE 0
#define MT65xx_LEVEL_SENSITIVE 1

#define MT65xx_POLARITY_LOW   0
#define MT65xx_POLARITY_HIGH  1

#define GIC_PPI_GLOBAL_TIMER      (GIC_PPI_OFFSET + 0)
#define GIC_PPI_LEGACY_FIQ        (GIC_PPI_OFFSET + 1)
#define GIC_PPI_PRIVATE_TIMER     (GIC_PPI_OFFSET + 2)
#define GIC_PPI_NS_PRIVATE_TIMER  (GIC_PPI_OFFSET + 3)
#define GIC_PPI_LEGACY_IRQ        (GIC_PPI_OFFSET + 4)

#define MT_BTIF_IRQ_ID                  (GIC_PRIVATE_SIGNALS + 50)
#define MT_DMA_BTIF_TX_IRQ_ID              (GIC_PRIVATE_SIGNALS + 71)
#define MT_DMA_BTIF_RX_IRQ_ID              (GIC_PRIVATE_SIGNALS + 72)

#define MT6582_USB0_IRQ_ID             64
#define MT6582_USB1_IRQ_ID             65
#define TS_IRQ_ID                      66
#define TS_BATCH_IRQ_ID                67
#define LOWBATTERY_IRQ_ID              68
#define PWM_IRQ_ID                     69
#define THERM_CTRL_IRQ_ID              70
#define MT_MSDC0_IRQ_ID                71
#define MT_MSDC1_IRQ_ID                72
#define MT_MSDC2_IRQ_ID                73
#define MT_MSDC3_IRQ_ID                74
#define MT_I2C0_IRQ_ID                 76
#define MT_I2C1_IRQ_ID                 77
#define MT_I2C2_IRQ_ID                 78
#define BITF_IRQ_ID                    82
#define MT_UART1_IRQ_ID                83
#define MT_UART2_IRQ_ID                84
#define MT_UART3_IRQ_ID                85
#define MT_UART4_IRQ_ID                86
#define MT_NFIECC_IRQ_ID               87
#define MT_NFI_IRQ_ID                  88
#define MT_GDMA1_IRQ_ID                89
#define MT_GDMA2_IRQ_ID                90
#define MT6582_HIF_PDMA_IRQ_ID         91
#define AP_DMA_I2C0_IRQ_ID             92
#define AP_DMA_I2C1_IRQ_ID             93
#define AP_DMA_I2C2_IRQ_ID             94
#define MT_DMA_UART0_TX_IRQ_ID         95
#define MT_DMA_UART0_RX_IRQ_ID         96
#define MT_DMA_UART1_TX_IRQ_ID         97
#define MT_DMA_UART1_RX_IRQ_ID         98
#define MT_DMA_UART2_TX_IRQ_ID         99
#define MT_DMA_UART2_RX_IRQ_ID         100
#define MT_DMA_UART3_TX_IRQ_ID         101
#define MT_DMA_UART3_RX_IRQ_ID         102
#define AP_DMA_BTIF_TX_IRQ_ID          103
#define AP_DMA_BTIF_RX_IRQ_ID          104
#define MT_GCPU_IRQ_ID                 105
#define MT_GCPU_DMX_IRQ_ID             106
#define MT_GCPU_MMU_IRQ_ID             107
#define MT_GCPU_MMU_SEC_IRQ_ID         108
#define MT_ETHER_NIC_WRAP_IRQ_ID       109
#define MT6582_SPI1_IRQ_ID             110
#define MSDC0_WAKEUP_PS_IRQ_ID         111
#define MSDC1_WAKEUP_PS_IRQ_ID         112
#define MSDC2_WAKEUP_PS_IRQ_ID         113
#define MT_CRYPTO_RING0_IRQ_ID         114
#define MT_CRYPTO_RING1_IRQ_ID         115
#define MT_CRYPTO_RING2_IRQ_ID         116
#define MT_PTP_FSM_IRQ_ID              117
#define BTIF_WAKEUP_IRQ_ID             118
#define MT_IRRX_IRQ_ID                 119
#define MT_WDT_IRQ_ID                  120
#define MT_CRYPTO_RING3_IRQ_ID         123
#define DCC_APARM_IRQ_ID               124
#define APARM_CTI_IRQ_ID               125
#define MT_APARM_DOMAIN_IRQ_ID         126
#define MT_APARM_DECERR_IRQ_ID         127
#define DOMAIN_ABORT_IRQ_ID0           128
#define MT_CRYPTO_IRQ_ID               129
#define MT_HSDMA_IRQ_ID                130
#define MT_GDMA_IRQ_ID                 131
#define CCIF0_AP_IRQ_ID                132
#define MT_I2S_IRQ_ID                  134
#define MT_PCM_IRQ_ID                  135
#define AFE_MCU_IRQ_ID                 136
#define M4U1_IRQ_ID                    138
#define M4UL2_IRQ_ID                   139
#define M4UL2_SEC_IRQ_ID               140
#define REFRESH_RATE_IRQ_ID            141
#define MT6582_APARM_GPTTIMER_IRQ_LINE 144
#define MT_EINT_IRQ_ID                 145
#define EINT_EVENT_IRQ_ID              146
#define MT6582_PMIC_WRAP_IRQ_ID        147
#define MT_KP_IRQ_ID                   148
#define MT_SPM_IRQ_ID                  149
#define MT_SPM1_IRQ_ID                 150
#define MT_SPM2_IRQ_ID                 151
#define MT_SPM3_IRQ_ID                 152
#define MT_EINT_DIRECT0_IRQ_ID         153
#define MT_EINT_DIRECT1_IRQ_ID         154
#define MT_EINT_DIRECT2_IRQ_ID         155
#define MT_EINT_DIRECT3_IRQ_ID         156
#define MT_EINT_DIRECT4_IRQ_ID         157
#define MT_EINT_DIRECT5_IRQ_ID         158
#define MT_EINT_DIRECT6_IRQ_ID         159
#define MT_EINT_DIRECT7_IRQ_ID         160
#define MT_EINT_DIRECT8_IRQ_ID         161
#define MT_EINT_DIRECT9_IRQ_ID         162
#define MT_EINT_DIRECT10_IRQ_ID        163
#define MT_EINT_DIRECT11_IRQ_ID        164
#define MT_EINT_DIRECT12_IRQ_ID        165
#define MT_EINT_DIRECT13_IRQ_ID        166
#define MT_EINT_DIRECT14_IRQ_ID        167
#define SMI_LARB0_IRQ_ID               168
#define SMI_LARB1_IRQ_ID               169
#define SMI_LARB2_IRQ_ID               170
#define MT_VDEC_IRQ_ID                 171
#define MT_VENC_IRQ_ID                 172
#define MT6582_JPEG_ENC_IRQ_ID         173
#define SENINF_IRQ_ID                  174
#define CAMERA_ISP_IRQ0_ID             175
#define CAMERA_ISP_IRQ1_ID             176
#define CAMERA_ISP_IRQ2_ID             177
#define MT6582_DISP_MDP_RDMA_IRQ_ID    178
#define MT6582_DISP_MDP_RSZ0_IRQ_ID    179
#define MT6582_DISP_MDP_RSZ1_IRQ_ID    180
#define MT6582_DISP_MDP_TDSHP_IRQ_ID   181
#define MT6582_DISP_MDP_WDMA_IRQ_ID    182
#define MT6582_DISP_MDP_WROT_IRQ_ID    183
#define MT6582_DISP_RDMA_IRQ_ID        184
#define MT6582_DISP_OVL_IRQ_ID         185
#define MT6582_DISP_WDMA_IRQ_ID        186
#define MT6582_DISP_BLS_IRQ_ID         187
#define MT6582_DISP_COLOR_IRQ_ID       188
#define MT6582_DISP_DSI_IRQ_ID         189
#define MT6582_DISP_DPI0_IRQ_ID        190
#define MT6582_DISP_CMDQ_IRQ_ID        191
#define MT6582_DISP_CMDQ_SECURE_IRQ_ID 192
#define MT6582_DISP_MUTEX_IRQ_ID       193
#define MM_DUMMY0_IRQ_ID               194
#define MM_DUMMY1_IRQ_ID               195
#define MT6582_DISP_RDMA1_IRQ_ID       196
#define MM_DUMMY3_IRQ_ID               197
#define MM_DUMMY4_IRQ_ID               198
#define MM_DUMMY5_IRQ_ID               199
#define MM_DUMMY6_IRQ_ID               200
#define MM_DUMMY7_IRQ_ID               201
#define MT_MFG_IRQ0_ID                 202
#define MT_MFG_IRQ1_ID                 203
#define MT_MFG_IRQ2_ID                 204
#define MT_MFG_IRQ3_ID                 205
#define MT_MFG_IRQ4_ID                 206
#define MT_MFG_IRQ5_ID                 207
#define MT_MFG_IRQ6_ID                 208
#define MT_MFG_IRQ7_ID                 209
#define MT_MFG_IRQ8_ID                 210
#define MT_MFG_IRQ9_ID                 211
#define MT_MFG_IRQ10_ID                212
#define MT_APXGPT_SECURE_IRQ_ID        213
#define MT_CEC_IRQ_ID                  214
#define CONN_WDT_IRQ_ID                215
#define WF_HIF_IRQ_ID                  216
#define MT_CONN2AP_BTIF_WAKEUP_IRQ_ID  217
#define BT_CVSD_IRQ_ID                 218
#define MT_CIRQ_IRQ_ID                 219
#define MT_PCIE_LINK_DOWN_RST_IRQ_ID   224
#define MT_PCIE0_IRQ_ID                225
#define MT_PCIE1_IRQ_ID                226
#define MT_PCIE2_IRQ_ID                227
#define MT_SSUSB_XHCI0_IRQ_ID          228
#define MT_SSUSB_XHCI1_IRQ_ID          229
#define MT_FE_PDMA_IRQ_ID              230
#define MT_FE_QDMA_IRQ_ID              231
#define MT_FE_ORIG_IRQ_ID              232

#endif
