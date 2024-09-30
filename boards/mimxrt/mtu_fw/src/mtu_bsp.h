/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MTU_BSP__
#define __MTU_BSP__

#include <stdint.h>
#include <stdbool.h>
#include "mtu.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

// Supported Mixspi clock defn
typedef enum _mixspi_root_clk_freq
{
    kMixspiRootClkFreq_30MHz  = 1,
    kMixspiRootClkFreq_50MHz  = 2,
    kMixspiRootClkFreq_60MHz  = 3,
    kMixspiRootClkFreq_80MHz  = 4,
    kMixspiRootClkFreq_100MHz = 5,
    kMixspiRootClkFreq_120MHz = 6,
    kMixspiRootClkFreq_133MHz = 7,
    kMixspiRootClkFreq_166MHz = 8,
    kMixspiRootClkFreq_200MHz = 9,
    kMixspiRootClkFreq_240MHz = 10,
    kMixspiRootClkFreq_266MHz = 11,
    kMixspiRootClkFreq_332MHz = 12,
    kMixspiRootClkFreq_400MHz = 13,
} mixspi_root_clk_freq_t;

typedef struct PinInfo
{
    uint16_t gpioGroup;
    uint16_t gpioPin;
} pin_info_t;

#define MTU_MAX_PINS (22)

/*******************************************************************************
 * API
 ******************************************************************************/

void     bsp_mixspi_pinmux_config(void *configPacket, bool isPintest);

void     bsp_mixspi_gpios_toggle(void);

void     bsp_mixspi_clock_init(void *config);

void     bsp_mixspi_clock_source(void *config);

void     bsp_mixspi_sw_delay_us(uint64_t us);

void     bsp_adc_echo_info(void);

void     bsp_adc_init(void);

uint8_t  bsp_adc_get_conv_value(void);

void     bsp_adc_deinit(void);

#endif /* __MTU_BSP__ */
