/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _PORT_FLEXSPI_MEM_H_
#define _PORT_FLEXSPI_MEM_H_

#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static inline void flexspi_clock_init(void)
{
    clock_root_config_t rootCfg = {0};
    rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc24M;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
}

#endif /* _PORT_FLEXSPI_MEM_H_ */
