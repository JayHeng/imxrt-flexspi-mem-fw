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

volatile uint32_t s_currentCounter;
uint32_t s_maxCounter;

void (*timer_task)(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

void SysTick_Handler(void)
{
    s_currentCounter--;
    if (!s_currentCounter)
    {
        s_currentCounter = s_maxCounter;
        if (timer_task)
        {
            timer_task();
        }
    }
}

void mtu_init_timer(uint32_t taskCycleInMs, void *callback)
{
    s_maxCounter = taskCycleInMs;
    s_currentCounter = taskCycleInMs;
    timer_task = (void (*)(void))callback;
    /* Set systick reload value to generate 1ms interrupt */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1)
        {
        }
    }
}

void mtu_deinit_timer(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    s_maxCounter = 0;
    timer_task = NULL;
}
