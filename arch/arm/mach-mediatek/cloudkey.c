/*
 * Device Tree support for UBNT Cloudkey based on
 * Mediatek MT7626 SoC
 *
 * Copyright (c) 2015 Ubiquiti Networks
 * Author: Shrirang Bagul <shrirang.bagul@ubnt.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/init.h>
#include <asm/mach/arch.h>

static const char * const cloudkey_board_dt_compat[] = {
	"mediatek,mt7623",
	NULL,
};

DT_MACHINE_START(MEDIATEK_DT, "Mediatek Cortex-A7 (Device Tree)")
	.init_machine	= mt_board_init,
	.dt_compat	= cloudkey_board_dt_compat,
MACHINE_END
