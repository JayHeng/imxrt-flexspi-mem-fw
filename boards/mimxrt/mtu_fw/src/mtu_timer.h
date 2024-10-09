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

void     mtu_task_timer_init(uint32_t taskCycleInMs, void *callback);

void     mtu_task_timer_deinit(void);

void     mtu_life_timer_init(void);

void     mtu_life_timer_deinit(void);

uint64_t mtu_life_timer_clock(void);

#endif /* __MTU_TIMER__ */
