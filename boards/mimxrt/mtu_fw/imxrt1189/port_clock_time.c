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
    printf("Clock roots frequency (MHz):\n");
    for(uint32_t i=0; i<4; i++)
    {
        uint32_t freq = CLOCK_GetRootClockFreq((clock_root_t)i);
        freq /= 1000000;
        char name[5] = {0};
        *(uint32_t *)name = *(uint32_t *)(CCM->CLOCK_ROOT[i].RESERVED_1);
        printf("%4s: %d\n", name, freq);
    }
}

uint32_t bsp_life_timer_clocks_per_sec(void)
{
    return CLOCK_GetRootClockFreq(kCLOCK_Root_Bus_Aon);
}

