/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MTU_TIMER__
#define __MTU_TIMER__

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * API
 ******************************************************************************/

void mtu_init_timer(uint32_t pulseInMs, void *callback);

void mtu_deinit_timer(void);

#endif /* __MTU_TIMER__ */
