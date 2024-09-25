/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_iopctl.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "mtu.h"
#include "mtu_pin.h"
#include "mtu_uart.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define IOPCTRL_FUNC0      (/* Pin is configured as FUNC0 */             \
                           IOPCTL_PIO_FUNC0 |                            \
                           /* Disable pull-up / pull-down function */    \
                           IOPCTL_PIO_PUPD_DI |                          \
                           /* Enable pull-down function */               \
                           IOPCTL_PIO_PULLDOWN_EN |                      \
                           /* Disable input buffer function */           \
                           IOPCTL_PIO_INBUF_DI |                         \
                           /* Normal mode */                             \
                           IOPCTL_PIO_SLEW_RATE_NORMAL |                 \
                           /* Normal drive */                            \
                           IOPCTL_PIO_FULLDRIVE_DI |                     \
                           /* Analog mux is disabled */                  \
                           IOPCTL_PIO_ANAMUX_DI |                        \
                           /* Pseudo Output Drain is disabled */         \
                           IOPCTL_PIO_PSEDRAIN_DI |                      \
                           /* Input function is not inverted */          \
                           IOPCTL_PIO_INV_DI)

#define IOPCTRL_FUNC1      (/* Pin is configured as FUNC1 */             \
                           IOPCTL_PIO_FUNC1 |                            \
                           /* Enable pull-up / pull-down function */     \
                           IOPCTL_PIO_PUPD_EN |                          \
                           /* Enable pull-down function */               \
                           IOPCTL_PIO_PULLDOWN_EN |                      \
                           /* Enables input buffer function */           \
                           IOPCTL_PIO_INBUF_EN |                         \
                           /* Normal mode */                             \
                           IOPCTL_PIO_SLEW_RATE_NORMAL |                 \
                           /* Full drive enable */                       \
                           IOPCTL_PIO_FULLDRIVE_EN |                     \
                           /* Analog mux is disabled */                  \
                           IOPCTL_PIO_ANAMUX_DI |                        \
                           /* Pseudo Output Drain is disabled */         \
                           IOPCTL_PIO_PSEDRAIN_DI |                      \
                           /* Input function is not inverted */          \
                           IOPCTL_PIO_INV_DI)


/////////////////////////////////////////////////////////////
enum _flexspi1_data_line_sel
{
    kFlexspi1_DataA_PIO1_23_20   = 0x00,
    kFlexspi1_DataA_PIO1_27_24   = 0x01,
};

enum _flexspi1_ss_b_sel
{
    kFlexspi1_Ss0A_PIO1_19       = 0x00,
};

enum _flexspi1_sclk_sel
{
    kFlexspi1_SclkA_PIO1_18      = 0x00,
};

enum _flexspi1_dqs_sel
{
    kFlexspi1_DqsA_PIO1_28       = 0x00,
};

enum _flexspi1_rst_b_sel
{
    kFlexspi1_Rst_PIO0_14        = 0x00,
};


/////////////////////////////////////////////////////////////


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

   
/*******************************************************************************
 * Variables
 ******************************************************************************/

pin_info_t s_pinInfo[13];


/*******************************************************************************
 * Code
 ******************************************************************************/

void bsp_flexspi_pinmux_config(void *configPacket, bool isPintest)
{
    if (isPintest)
    {
        pin_unittest_packet_t *packet = (pin_unittest_packet_t *)configPacket;
        gpio_pin_config_t do_config = {kGPIO_DigitalOutput, 0};
        memset(s_pinInfo, 0xFF, sizeof(s_pinInfo));
        if (packet->memConnection.instance == 1)
        {
            if (packet->unittestEn.option.B.dataLow4bit)
            {
                switch (packet->memConnection.dataLow4bit)
                {
                    case kFlexspi1_DataA_PIO1_23_20:
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 23U, IOPCTRL_FUNC0);
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 22U, IOPCTRL_FUNC0);
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 21U, IOPCTRL_FUNC0);
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 20U, IOPCTRL_FUNC0);
                        GPIO_PortInit(GPIO, 1);
                        GPIO_PinInit(GPIO, 1, 23, &do_config);
                        GPIO_PinInit(GPIO, 1, 22, &do_config);
                        GPIO_PinInit(GPIO, 1, 21, &do_config);
                        GPIO_PinInit(GPIO, 1, 20, &do_config);
                        s_pinInfo[0].gpioGroup = 1;
                        s_pinInfo[0].gpioPin = 23;
                        s_pinInfo[1].gpioGroup = 1;
                        s_pinInfo[1].gpioPin = 22;
                        s_pinInfo[2].gpioGroup = 1;
                        s_pinInfo[2].gpioPin = 21;
                        s_pinInfo[3].gpioGroup = 1;
                        s_pinInfo[3].gpioPin = 20;
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.ss_b)
            {
                switch (packet->memConnection.ss_b)
                {
                    case kFlexspi1_Ss0A_PIO1_19:
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 19U, IOPCTRL_FUNC0);
                        GPIO_PortInit(GPIO, 1);
                        GPIO_PinInit(GPIO, 1, 19, &do_config);
                        s_pinInfo[4].gpioGroup = 1;
                        s_pinInfo[4].gpioPin = 19;
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.sclk)
            {
                switch (packet->memConnection.sclk)
                {
                    case kFlexspi1_SclkA_PIO1_18:
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 18U, IOPCTRL_FUNC0);
                        GPIO_PortInit(GPIO, 1);
                        GPIO_PinInit(GPIO, 1, 18, &do_config);
                        s_pinInfo[5].gpioGroup = 1;
                        s_pinInfo[5].gpioPin = 18;
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.dqs)
            {
                switch (packet->memConnection.dqs)
                {
                    case kFlexspi1_DqsA_PIO1_28:
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 17U, IOPCTRL_FUNC0);
                        GPIO_PortInit(GPIO, 1);
                        GPIO_PinInit(GPIO, 1, 17, &do_config);
                        s_pinInfo[6].gpioGroup = 1;
                        s_pinInfo[6].gpioPin = 17;
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.rst_b)
            {
                switch (packet->memConnection.rst_b)
                {
                    case kFlexspi1_Rst_PIO0_14:
                        IOPCTL_PinMuxSet(IOPCTL, 0U, 14U, IOPCTRL_FUNC0);
                        GPIO_PortInit(GPIO, 0);
                        GPIO_PinInit(GPIO, 0, 14, &do_config);
                        s_pinInfo[7].gpioGroup = 0;
                        s_pinInfo[7].gpioPin = 14;
                        break;
                    default:
                        break;
                }
            }
        }
        else if (packet->memConnection.instance == 2)
        {
        }
    }
    else
    {
        config_system_packet_t *packet = (config_system_packet_t *)configPacket;
        if (packet->memConnection.instance == 1)
        {
            switch (packet->memConnection.dataLow4bit)
            {
                case kFlexspi1_DataA_PIO1_23_20:
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 23U, IOPCTRL_FUNC1);
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 22U, IOPCTRL_FUNC1);
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 21U, IOPCTRL_FUNC1);
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 20U, IOPCTRL_FUNC1);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.ss_b)
            {
                case kFlexspi1_Ss0A_PIO1_19:
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 19U, IOPCTRL_FUNC1);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.sclk)
            {
                case kFlexspi1_SclkA_PIO1_18:
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 18U, IOPCTRL_FUNC1);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs)
            {
                case kFlexspi1_DqsA_PIO1_28:
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 28U, IOPCTRL_FUNC1);
                    break;
                default:
                    break;
            }
        }
        else if (packet->memConnection.instance == 2)
        {
        }
    }
}

void bsp_flexspi_gpios_toggle(void)
{
    if (s_pinUnittestPacket.unittestEn.enableAdcSample)
    {
        uint8_t convValue = bsp_adc_get_conv_value();
        mtu_uart_sendhex(&convValue, sizeof(convValue));
    }
  
    for (uint32_t i = 0; i < sizeof(s_pinInfo) / sizeof(pin_info_t); i++)
    {
        if (s_pinInfo[i].gpioGroup != 0xFFFFu)
        {
            GPIO_PortToggle(GPIO, s_pinInfo[i].gpioGroup, 1u << s_pinInfo[i].gpioPin);
        }
    }
}

