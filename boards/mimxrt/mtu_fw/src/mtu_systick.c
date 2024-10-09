/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <ctype.h>
#include <stdio.h>
#include "clock_config.h"
#include "mtu.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

   
/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile uint32_t s_taskCurCounter;
uint32_t s_taskMaxCounter;

void (*timer_task)(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

void SysTick_Handler(void)
{
    s_taskCurCounter--;
    if (!s_taskCurCounter)
    {
        s_taskCurCounter = s_taskMaxCounter;
        if (timer_task)
        {
            timer_task();
        }
    }
}

void mtu_task_timer_init(uint32_t taskCycleInMs, void *callback)
{
    s_taskMaxCounter = taskCycleInMs;
    s_taskCurCounter = taskCycleInMs;
    timer_task = (void (*)(void))callback;
    /* Set systick reload value to generate 1ms interrupt */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1)
        {
        }
    }
}

void mtu_task_timer_deinit(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    s_taskMaxCounter = 0;
    timer_task = NULL;
}
