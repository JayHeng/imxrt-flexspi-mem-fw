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
#include "mtu.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct PinInfo
{
    uint16_t gpioGroup;
    uint16_t gpioPin;
} pin_info_t;

#define MTU_MAX_PINS (22)

/*******************************************************************************
 * API
 ******************************************************************************/

void bsp_flexspi_pinmux_config(void *configPacket, bool isPintest);

void bsp_flexspi_gpios_toggle(void);

void bsp_adc_echo_info(void);

void bsp_adc_init(void);

uint8_t bsp_adc_get_conv_value(void);

void bsp_adc_deinit(void);

#endif /* __MTU_PIN__ */
