/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mtu.h"
#include "fsl_common.h"
#include "fsl_iopctl.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
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

#define IOPCTRL_FUNC6      (/* Pin is configured as FUNC6 */             \
                           IOPCTL_PIO_FUNC6 |                            \
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

    kFlexspi1_DataB_PIO1_14_11   = 0x10,
    kFlexspi1_DataB_PIO2_23_17   = 0x11,
};

enum _flexspi1_ss_b_sel
{
    kFlexspi1_Ss0A_PIO1_19       = 0x00,

    kFlexspi1_Ss0B_PIO2_19       = 0x10,
};

enum _flexspi1_sclk_sel
{
    kFlexspi1_SclkA_PIO1_18      = 0x00,

    kFlexspi1_SclkB_PIO1_29      = 0x10,
};

enum _flexspi1_dqs_sel
{
    kFlexspi1_DqsA_PIO1_28       = 0x00,
};

enum _flexspi1_rst_b_sel
{
    kFlexspi1_Rst_PIO0_26        = 0x00,
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
                    case kFlexspi1_DataB_PIO1_14_11:
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 14U, IOPCTRL_FUNC0);
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 13U, IOPCTRL_FUNC0);
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 12U, IOPCTRL_FUNC0);
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 11U, IOPCTRL_FUNC0);
                        GPIO_PortInit(GPIO, 1);
                        GPIO_PinInit(GPIO, 1, 14, &do_config);
                        GPIO_PinInit(GPIO, 1, 13, &do_config);
                        GPIO_PinInit(GPIO, 1, 12, &do_config);
                        GPIO_PinInit(GPIO, 1, 11, &do_config);
                        s_pinInfo[0].gpioGroup = 1;
                        s_pinInfo[0].gpioPin = 14;
                        s_pinInfo[1].gpioGroup = 1;
                        s_pinInfo[1].gpioPin = 13;
                        s_pinInfo[2].gpioGroup = 1;
                        s_pinInfo[2].gpioPin = 12;
                        s_pinInfo[3].gpioGroup = 1;
                        s_pinInfo[3].gpioPin = 11;
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
                    case kFlexspi1_Ss0B_PIO2_19:
                        IOPCTL_PinMuxSet(IOPCTL, 2U, 19U, IOPCTRL_FUNC0);
                        GPIO_PortInit(GPIO, 2);
                        GPIO_PinInit(GPIO, 2, 19, &do_config);
                        s_pinInfo[4].gpioGroup = 2;
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
                    case kFlexspi1_SclkB_PIO1_29:
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 29U, IOPCTRL_FUNC0);
                        GPIO_PortInit(GPIO, 1);
                        GPIO_PinInit(GPIO, 1, 29, &do_config);
                        s_pinInfo[5].gpioGroup = 1;
                        s_pinInfo[5].gpioPin = 29;
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.dqs0)
            {
                switch (packet->memConnection.dqs0)
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
            if (packet->unittestEn.option.B.dataHigh4bit)
            {
                switch (packet->memConnection.dataHigh4bit)
                {
                    case kFlexspi1_DataA_PIO1_27_24:
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 27U, IOPCTRL_FUNC0);
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 26U, IOPCTRL_FUNC0);
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 25U, IOPCTRL_FUNC0);
                        IOPCTL_PinMuxSet(IOPCTL, 1U, 24U, IOPCTRL_FUNC0);
                        GPIO_PortInit(GPIO, 1);
                        GPIO_PinInit(GPIO, 1, 27, &do_config);
                        GPIO_PinInit(GPIO, 1, 26, &do_config);
                        GPIO_PinInit(GPIO, 1, 25, &do_config);
                        GPIO_PinInit(GPIO, 1, 24, &do_config);
                        s_pinInfo[7].gpioGroup = 1;
                        s_pinInfo[7].gpioPin = 27;
                        s_pinInfo[8].gpioGroup = 1;
                        s_pinInfo[8].gpioPin = 26;
                        s_pinInfo[9].gpioGroup = 1;
                        s_pinInfo[9].gpioPin = 25;
                        s_pinInfo[10].gpioGroup = 1;
                        s_pinInfo[10].gpioPin = 24;
                        break;
                    case kFlexspi1_DataB_PIO2_23_17:
                        IOPCTL_PinMuxSet(IOPCTL, 2U, 23U, IOPCTRL_FUNC0);
                        IOPCTL_PinMuxSet(IOPCTL, 2U, 22U, IOPCTRL_FUNC0);
                        IOPCTL_PinMuxSet(IOPCTL, 2U, 18U, IOPCTRL_FUNC0);
                        IOPCTL_PinMuxSet(IOPCTL, 2U, 17U, IOPCTRL_FUNC0);
                        GPIO_PortInit(GPIO, 2);
                        GPIO_PinInit(GPIO, 2, 23, &do_config);
                        GPIO_PinInit(GPIO, 2, 22, &do_config);
                        GPIO_PinInit(GPIO, 2, 18, &do_config);
                        GPIO_PinInit(GPIO, 2, 17, &do_config);
                        s_pinInfo[7].gpioGroup = 2;
                        s_pinInfo[7].gpioPin = 23;
                        s_pinInfo[8].gpioGroup = 2;
                        s_pinInfo[8].gpioPin = 22;
                        s_pinInfo[9].gpioGroup = 2;
                        s_pinInfo[9].gpioPin = 18;
                        s_pinInfo[10].gpioGroup = 2;
                        s_pinInfo[10].gpioPin = 17;
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.rst_b)
            {
                switch (packet->memConnection.rst_b)
                {
                    case kFlexspi1_Rst_PIO0_26:
                        IOPCTL_PinMuxSet(IOPCTL, 0U, 26U, IOPCTRL_FUNC0);
                        GPIO_PortInit(GPIO, 0);
                        GPIO_PinInit(GPIO, 0, 26, &do_config);
                        s_pinInfo[11].gpioGroup = 0;
                        s_pinInfo[11].gpioPin = 26;
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
                case kFlexspi1_DataB_PIO1_14_11:
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 14U, IOPCTRL_FUNC6);
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 13U, IOPCTRL_FUNC6);
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 12U, IOPCTRL_FUNC6);
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 11U, IOPCTRL_FUNC6);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.ss_b)
            {
                case kFlexspi1_Ss0A_PIO1_19:
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 19U, IOPCTRL_FUNC1);
                    break;
                case kFlexspi1_Ss0B_PIO2_19:
                    IOPCTL_PinMuxSet(IOPCTL, 2U, 19U, IOPCTRL_FUNC6);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.sclk)
            {
                case kFlexspi1_SclkA_PIO1_18:
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 18U, IOPCTRL_FUNC1);
                    break;
                case kFlexspi1_SclkB_PIO1_29:
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 29U, IOPCTRL_FUNC6);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs0)
            {
                case kFlexspi1_DqsA_PIO1_28:
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 28U, IOPCTRL_FUNC1);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dataHigh4bit)
            {
                case kFlexspi1_DataA_PIO1_27_24:
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 27U, IOPCTRL_FUNC1);
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 26U, IOPCTRL_FUNC1);
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 25U, IOPCTRL_FUNC1);
                    IOPCTL_PinMuxSet(IOPCTL, 1U, 24U, IOPCTRL_FUNC1);
                    break;
                case kFlexspi1_DataB_PIO2_23_17:
                    IOPCTL_PinMuxSet(IOPCTL, 2U, 23U, IOPCTRL_FUNC6);
                    IOPCTL_PinMuxSet(IOPCTL, 2U, 22U, IOPCTRL_FUNC6);
                    IOPCTL_PinMuxSet(IOPCTL, 2U, 18U, IOPCTRL_FUNC6);
                    IOPCTL_PinMuxSet(IOPCTL, 2U, 17U, IOPCTRL_FUNC6);
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

void bsp_flexspi_clock_init(void)
{

}

