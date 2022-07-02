/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MTU_PIN__
#define __MTU_PIN__

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct PinInfo
{
    uint16_t gpioGroup;
    uint16_t gpioPin;
} pin_info_t;

/*******************************************************************************
 * API
 ******************************************************************************/

void bsp_flexspi_pinmux_config(void *configPacket, bool isPintest);

void bsp_flexspi_gpios_toggle(void);

#endif /* __MTU_PIN__ */
