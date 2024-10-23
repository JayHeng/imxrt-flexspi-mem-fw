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
    for(uint32_t i=0; i<4; i++)
    {
        uint32_t freq = CLOCK_GetRootClockFreq((clock_root_t)i);
        freq /= 1000000;
        char name[5] = {0};
        *(uint32_t *)name = *(uint32_t *)(CCM->CLOCK_ROOT[i].RESERVED_1);
        printf("%4s: %d\n", name, freq);
    }
}

void bsp_rt_system_srams_print(void)
{
    printf("Platform Default SRAM regions(NS):\n");
    printf("   [0x0FFE0000 - 0x0FFFFFFF], 128KB, M33 Code TCM\n");
    printf("   [0x20000000 - 0x2001FFFF], 128KB, M33 System TCM\n");
    printf("   [0x20480000 - 0x204FFFFF], 512KB, OCRAM1\n");
    printf("   [0x20500000 - 0x2053FFFF], 256KB, OCRAM2\n");
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
    return CLOCK_GetRootClockFreq(kCLOCK_Root_Bus_Aon);
}

