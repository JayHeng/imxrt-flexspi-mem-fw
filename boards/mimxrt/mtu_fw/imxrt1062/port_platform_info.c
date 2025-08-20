/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "mtu.h"
#include "clock_config.h"

/*******************************************************************************
 * Declarations
 ******************************************************************************/

#if (defined(__ICCARM__)) // IAR
#pragma section = ".intvec"
#pragma section = "ApplicationFlash"
#pragma section = "ApplicationRam"
#define __RAM_START ((uint32_t)__section_begin("ApplicationRam"))
#define __RAM_END ((uint32_t)__section_end("ApplicationRam") - 1)
#ifndef __ROM_START
#define __ROM_START ((uint32_t)__section_begin(".intvec"))
#endif
#define __ROM_END ((uint32_t)__section_end("ApplicationFlash"))

#elif (defined(__CC_ARM)) || (__ARMCC_VERSION) // MDK

#elif (defined(__GNUC__)) // GCC

#else
#error Unknown toolchain!
#endif // __ICCARM__

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

void bsp_rt_system_clocks_print(void)
{
    printf("Platform clock roots frequency (MHz):\n");
    printf("CPU Clock: %6.2f\n", CLOCK_GetFreq(kCLOCK_CpuClk) / 1000000.0);
    printf("AHB Clock: %6.2f\n", CLOCK_GetFreq(kCLOCK_AhbClk) / 1000000.0);
    printf("IPG Clock: %6.2f\n", CLOCK_GetFreq(kCLOCK_IpgClk) / 1000000.0);
    printf("PER Clock: %6.2f\n", CLOCK_GetFreq(kCLOCK_PerClk) / 1000000.0);
}

void bsp_rt_system_srams_print(void)
{
    printf("Platform Default SRAM regions(NS):\n");
    printf("   [0x00000000 - 0x0001FFFF], 128KB, M7 ITCM\n");
    printf("   [0x20000000 - 0x2001FFFF], 128KB, M7 DTCM\n");
    printf("   [0x20200000 - 0x2027FFFF], 512KB, OCRAM2\n");
    printf("   [0x20280000 - 0x202BFFFF], 256KB, M7 OCRAM\n");
    printf("SRAM regions reserved by FW:\n");
    uint32_t codeStart = __ROM_START;
    uint32_t codeEnd   = __ROM_END;
    uint32_t dataStart = __RAM_START;
    uint32_t dataEnd   = __RAM_END;
    printf("   [0x%08X - 0x%08X], ro - usage rate %d%\n", codeStart, codeEnd, (codeEnd - codeStart)*100/(128*1024));
    printf("   [0x%08X - 0x%08X], rw - usage rate %d%\n", dataStart, dataEnd, (dataEnd - dataStart)*100/(128*1024));
}


uint32_t bsp_life_timer_clocks_per_sec(void)
{
    return CLOCK_GetFreq(kCLOCK_PerClk);
}

