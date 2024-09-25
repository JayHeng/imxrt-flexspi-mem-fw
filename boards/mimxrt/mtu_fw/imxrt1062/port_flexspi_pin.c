/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "mtu.h"
#include "mtu_pin.h"
#include "mtu_uart.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/////////////////////////////////////////////////////////////
enum _flexspi1_data_line_sel
{
    kFlexspi1_DataA_GPIO_SD_B1_11_08   = 0x00,

    kFlexspi1_DataB_GPIO_SD_B1_03_00   = 0x10,
};

enum _flexspi1_ss_b_sel
{
    kFlexspi1_Ss0A_GPIO_SD_B1_06   = 0x00,
    kFlexspi1_Ss1A_GPIO_SD_B0_00   = 0x01,

    kFlexspi1_Ss0B_GPIO_SD_B0_04   = 0x10,
    kFlexspi1_Ss1B_GPIO_SD_B0_01   = 0x11,
};

enum _flexspi1_sclk_sel
{
    kFlexspi1_SclkA_GPIO_SD_B1_07  = 0x00,

    kFlexspi1_SclkB_GPIO_SD_B1_04  = 0x10,
};

enum _flexspi1_dqs_sel
{
    kFlexspi1_DqsA_GPIO_SD_B1_05   = 0x00,

    kFlexspi1_DqsB_GPIO_SD_B0_05   = 0x10,
};

enum _flexspi1_rst_b_sel
{
    kFlexspi1_Rst_GPIO_AD_B0_09    = 0x00,
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
    CLOCK_EnableClock(kCLOCK_Iomuxc);
    if (isPintest)
    {
        pin_unittest_packet_t *packet = (pin_unittest_packet_t *)configPacket;
        gpio_pin_config_t do_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
        memset(s_pinInfo, 0xFF, sizeof(s_pinInfo));
        if (packet->memConnection.instance == 1)
        {
            if (packet->unittestEn.option.B.dataLow4bit)
            {
                switch (packet->memConnection.dataLow4bit)
                {
                    case kFlexspi1_DataA_GPIO_SD_B1_11_08:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_11_GPIO3_IO11, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_10_GPIO3_IO10, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_09_GPIO3_IO09, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_08_GPIO3_IO08, 0U);
                        GPIO_PinInit(GPIO3, 11, &do_config);
                        GPIO_PinInit(GPIO3, 10, &do_config);
                        GPIO_PinInit(GPIO3, 9, &do_config);
                        GPIO_PinInit(GPIO3, 8, &do_config);
                        s_pinInfo[0].gpioGroup = 3;
                        s_pinInfo[0].gpioPin = 11;
                        s_pinInfo[1].gpioGroup = 3;
                        s_pinInfo[1].gpioPin = 10;
                        s_pinInfo[2].gpioGroup = 3;
                        s_pinInfo[2].gpioPin = 9;
                        s_pinInfo[3].gpioGroup = 3;
                        s_pinInfo[3].gpioPin = 8;
                        break;
                    case kFlexspi1_DataB_GPIO_SD_B1_03_00:
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.ss_b)
            {
                switch (packet->memConnection.ss_b)
                {
                    case kFlexspi1_Ss0A_GPIO_SD_B1_06:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_06_GPIO3_IO06, 0U);
                        GPIO_PinInit(GPIO3, 6, &do_config);
                        s_pinInfo[4].gpioGroup = 3;
                        s_pinInfo[4].gpioPin = 6;
                        break;
                    case kFlexspi1_Ss1A_GPIO_SD_B0_00:
                        break;
                    case kFlexspi1_Ss0B_GPIO_SD_B0_04:
                        break;
                    case kFlexspi1_Ss1B_GPIO_SD_B0_01:
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.sclk)
            {
                switch (packet->memConnection.sclk)
                {
                    case kFlexspi1_SclkA_GPIO_SD_B1_07:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_07_GPIO3_IO07, 0U);
                        GPIO_PinInit(GPIO3, 7, &do_config);
                        s_pinInfo[5].gpioGroup = 3;
                        s_pinInfo[5].gpioPin = 7;
                        break;
                    case kFlexspi1_SclkB_GPIO_SD_B1_04:
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.dqs)
            {
                switch (packet->memConnection.dqs)
                {
                    case kFlexspi1_DqsA_GPIO_SD_B1_05:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_GPIO3_IO05, 0U);
                        GPIO_PinInit(GPIO3, 5, &do_config);
                        s_pinInfo[6].gpioGroup = 3;
                        s_pinInfo[6].gpioPin = 5;
                        break;
                    case kFlexspi1_DqsB_GPIO_SD_B0_05:
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.rst_b)
            {
                switch (packet->memConnection.rst_b)
                {
                    case kFlexspi1_Rst_GPIO_AD_B0_09:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 0U);
                        //IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 0x10B0U); 
                        GPIO_PinInit(GPIO1, 9, &do_config);
                        s_pinInfo[7].gpioGroup = 1;
                        s_pinInfo[7].gpioPin = 9;
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
                case kFlexspi1_DataA_GPIO_SD_B1_11_08:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00, 0x0AU);
                    break;
                case kFlexspi1_DataB_GPIO_SD_B1_03_00:
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.ss_b)
            {
                case kFlexspi1_Ss0A_GPIO_SD_B1_06:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B, 0x0AU);
                    break;
                case kFlexspi1_Ss1A_GPIO_SD_B0_00:
                    break;
                case kFlexspi1_Ss0B_GPIO_SD_B0_04:
                    break;
                case kFlexspi1_Ss1B_GPIO_SD_B0_01:
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.sclk)
            {
                case kFlexspi1_SclkA_GPIO_SD_B1_07:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK, 0x0AU);
                    break;
                case kFlexspi1_SclkB_GPIO_SD_B1_04:
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs)
            {
                case kFlexspi1_DqsA_GPIO_SD_B1_05:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS, 0x0AU);
                    break;
                case kFlexspi1_DqsB_GPIO_SD_B0_05:
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
  
    GPIO_Type *gpioBase[] = GPIO_BASE_PTRS;
    for (uint32_t i = 0; i < sizeof(s_pinInfo) / sizeof(pin_info_t); i++)
    {
        if (s_pinInfo[i].gpioGroup != 0xFFFFu)
        {
            GPIO_PortToggle(gpioBase[s_pinInfo[i].gpioGroup], 1u << s_pinInfo[i].gpioPin);
        }
    }
}

