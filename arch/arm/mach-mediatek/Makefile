obj-$(CONFIG_ARCH_MEDIATEK) += mediatek.o

ifeq ($(CONFIG_MT7623),y)

ccflags-$(CONFIG_ARCH_MULTIPLATFORM) += -I$(srctree)/$(src)/include -I$(srctree)/arch/arm/mach-mediatek/include

obj-$(CONFIG_ARCH_MEDIATEK) := common.o irq.o timer.o mt_dbg.o
obj-$(CONFIG_ARCH_MEDIATEK) += mt_devs.o mt_gpt.o
#obj-$(CONFIG_ARCH_MEDIATEK) += mt_devs.o mt_gpt.o mt_boot.o board.o patchinfo.o
obj-$(CONFIG_ARCH_MEDIATEK) += ca7_timer.o
obj-$(CONFIG_ARCH_MEDIATEK) += mt_dma.o
obj-$(CONFIG_ARCH_MEDIATEK) += mt_reg_dump.o
obj-$(CONFIG_ARCH_MEDIATEK) += mt_pm_init.o

ifeq ($(CONFIG_SMP),y)
#ccflags-y += -I$(srctree)
obj-$(CONFIG_ARCH_MEDIATEK) += mt-smp.o hotplug.o mt-headsmp.o mt_hotplug.o mt_hotplug_mechanism.o
endif

obj-$(CONFIG_ARCH_MEDIATEK) += eint.o mt_freqhopping.o
obj-$(CONFIG_MT65XX_TRACER) += pmu_v7.o mon_interface.o mt_mon.o mt_emi_bm.o
obj-$(CONFIG_ARCH_MEDIATEK) += emi_bwl.o emi_mpu.o

obj-$(CONFIG_ARCH_MEDIATEK) += mt_pm_ldo.o
#endif
obj-$(CONFIG_ARCH_MEDIATEK) += innercache.o mt_l2c.o
obj-$(CONFIG_MTK_CIRQ) += mt_cirq.o

obj-$(CONFIG_ARCH_MEDIATEK) += mt_spm.o mt_spm_sleep.o mt_spm_mtcmos.o
ccflags-$(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT) += -I$(srctree)/include/trustzone/
obj-$(CONFIG_ARCH_MEDIATEK) += mt_sleep.o
#obj-y += mt_cpufreq.o mt_gpufreq.o mt_cpu_ss.o
obj-$(CONFIG_ARCH_MEDIATEK) += mt_cpufreq.o mt_cpu_ss.o
obj-$(CONFIG_ARCH_MEDIATEK) += mt_emifreq.o
obj-$(CONFIG_ARCH_MEDIATEK) += mt_dormant.o cpu_dormant.o
obj-$(CONFIG_ARCH_MEDIATEK) += mt_dcm.o mt_idle.o
obj-$(CONFIG_ARCH_MEDIATEK) += mt_clkmgr_common.o
obj-$(CONFIG_ARCH_MEDIATEK) += mt_clkmgr.o
obj-$(CONFIG_ARCH_MEDIATEK) += sbchk_base.o
obj-$(CONFIG_ARCH_MEDIATEK) += mtk_ccci_helper.o
obj-$(CONFIG_ARCH_MEDIATEK) += mtk_eemcs_helper.o

ifneq ($(CONFIG_MTK_CCCI_DEVICES),y)
ccflags-y  += -D__USING_DUMMY_CCCI_API__
endif

obj-$(CONFIG_ARCH_MEDIATEK) += mt_ptp.o
obj-$(CONFIG_ARCH_MEDIATEK) += mssv.o

ifeq ($(CONFIG_PCI), y)
obj-$(CONFIG_ARCH_MEDIATEK) += mt_pcie.o
endif
ifeq ($(CONFIG_KERNEL_NVRAM), y)
obj-$(CONFIG_ARCH_MEDIATEK) += nvram.o
endif

# Hibernation
AFLAGS_swsusp.o	:= -DTEXT_OFFSET=$(TEXT_OFFSET)
obj-$(CONFIG_HIBERNATION)	+= cpu_hibernate.o swsusp.o

KBUILD_AFLAGS	:=$(KBUILD_AFLAGS:-msoft-float=-Wa,-mfpu=neon)

endif # CONFIG_MT7623
