/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mtu.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/////////////////////////////////////////////////////////////
enum _flexspi1_data_line_sel
{
    kFlexspi1_DataA_GPIO_SD_B2_11_08   = 0x00,
    kFlexspi1_DataA_GPIO_AD_23_20      = 0x01,

    kFlexspi1_DataB_GPIO_SD_B2_03_00   = 0x10,
    kFlexspi1_DataB_GPIO_AD_15_12      = 0x11,
};

enum _flexspi1_ss_b_sel
{
    kFlexspi1_Ss0A_GPIO_SD_B2_06   = 0x00,
    kFlexspi1_Ss0A_GPIO_AD_18      = 0x01,
    kFlexspi1_Ss1A_GPIO_SD_B1_02   = 0x02,

    kFlexspi1_Ss0B_GPIO_SD_B1_04   = 0x10,
    kFlexspi1_Ss1B_GPIO_AD_35      = 0x11,
    kFlexspi1_Ss1B_GPIO_SD_B1_03   = 0x12,
};

enum _flexspi1_sclk_sel
{
    kFlexspi1_SclkA_GPIO_SD_B2_07  = 0x00,
    kFlexspi1_SclkA_GPIO_AD_19     = 0x01,

    kFlexspi1_SclkB_GPIO_SD_B2_04  = 0x10,
    kFlexspi1_SclkB_GPIO_AD_16     = 0x11,
};

enum _flexspi1_dqs_sel
{
    kFlexspi1_DqsA_GPIO_SD_B2_05   = 0x00,
    kFlexspi1_DqsA_GPIO_AD_17      = 0x01,
    kFlexspi1_DqsA_GPIO_EMC_B2_18  = 0x02,

    kFlexspi1_DqsB_GPIO_SD_B1_05   = 0x10,
};

enum _flexspi1_rst_b_sel
{
    kFlexspi1_Rst_GPIO_SD_B1_00    = 0x00,
    kFlexspi1_Rst_GPIO_EMC_B1_40   = 0x01,
};

/////////////////////////////////////////////////////////////
enum _flexspi2_data_line_sel
{
    kFlexspi2_DataA_GPIO_EMC_B2_16_13       = 0x00,
    kFlexspi2_DataA_GPIO_SD_B1_05_02        = 0x01,
    kFlexspi2_DataA_GPIO_EMC_B2_20_17       = 0x02,

    kFlexspi2_DataB_GPIO_EMC_B2_06_03       = 0x10,
    kFlexspi2_DataB_GPIO_EMC_B1_41_B2_02_00 = 0x11,
};

enum _flexspi2_ss_b_sel
{
    kFlexspi2_Ss0A_GPIO_EMC_B2_11   = 0x00,
    kFlexspi2_Ss0A_GPIO_SD_B1_00    = 0x01,
    kFlexspi2_Ss1A_GPIO_AD_01       = 0x02,

    kFlexspi2_Ss0B_GPIO_EMC_B2_08   = 0x10,
    kFlexspi2_Ss1B_GPIO_AD_00       = 0x11,
};

enum _flexspi2_sclk_sel
{
    kFlexspi2_SclkA_GPIO_EMC_B2_10  = 0x00,
    kFlexspi2_SclkA_GPIO_SD_B1_01   = 0x01,

    kFlexspi2_SclkB_GPIO_EMC_B2_09  = 0x10,
};

enum _flexspi2_dqs_sel
{
    kFlexspi2_DqsA_GPIO_EMC_B2_12   = 0x00,

    kFlexspi2_DqsB_GPIO_EMC_B2_07   = 0x10,
};

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
                    case kFlexspi1_DataA_GPIO_SD_B2_11_08:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_08_GPIO10_IO17, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_09_GPIO10_IO18, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_10_GPIO10_IO19, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_11_GPIO10_IO20, 0U);
                        GPIO_PinInit(GPIO10, 17, &do_config);
                        GPIO_PinInit(GPIO10, 18, &do_config);
                        GPIO_PinInit(GPIO10, 19, &do_config);
                        GPIO_PinInit(GPIO10, 20, &do_config);
                        s_pinInfo[0].gpioGroup = 10;
                        s_pinInfo[0].gpioPin = 17;
                        s_pinInfo[1].gpioGroup = 10;
                        s_pinInfo[1].gpioPin = 18;
                        s_pinInfo[2].gpioGroup = 10;
                        s_pinInfo[2].gpioPin = 19;
                        s_pinInfo[3].gpioGroup = 10;
                        s_pinInfo[3].gpioPin = 20;
                        break;
                    case kFlexspi1_DataA_GPIO_AD_23_20:
                        break;
                    case kFlexspi1_DataB_GPIO_SD_B2_03_00:
                        break;
                    case kFlexspi1_DataB_GPIO_AD_15_12:
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.ss_b)
            {
                switch (packet->memConnection.ss_b)
                {
                    case kFlexspi1_Ss0A_GPIO_SD_B2_06:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_06_GPIO10_IO15, 0U);
                        GPIO_PinInit(GPIO10, 15, &do_config);
                        s_pinInfo[4].gpioGroup = 10;
                        s_pinInfo[4].gpioPin = 15;
                        break;
                    case kFlexspi1_Ss0A_GPIO_AD_18:
                        break;
                    case kFlexspi1_Ss1A_GPIO_SD_B1_02:
                        break;
                    case kFlexspi1_Ss0B_GPIO_SD_B1_04:
                        break;
                    case kFlexspi1_Ss1B_GPIO_AD_35:
                        break;
                    case kFlexspi1_Ss1B_GPIO_SD_B1_03:
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.sclk)
            {
                switch (packet->memConnection.sclk)
                {
                    case kFlexspi1_SclkA_GPIO_SD_B2_07:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_07_GPIO10_IO16, 0U);
                        GPIO_PinInit(GPIO10, 16, &do_config);
                        s_pinInfo[5].gpioGroup = 10;
                        s_pinInfo[5].gpioPin = 16;
                        break;
                    case kFlexspi1_SclkA_GPIO_AD_19:
                        break;
                    case kFlexspi1_SclkB_GPIO_SD_B2_04:
                        break;
                    case kFlexspi1_SclkB_GPIO_AD_16:
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.dqs0)
            {
                switch (packet->memConnection.dqs0)
                {
                    case kFlexspi1_DqsA_GPIO_SD_B2_05:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_05_GPIO10_IO14, 0U);
                        GPIO_PinInit(GPIO10, 14, &do_config);
                        s_pinInfo[6].gpioGroup = 10;
                        s_pinInfo[6].gpioPin = 14;
                        break;
                    case kFlexspi1_DqsA_GPIO_AD_17:
                        break;
                    case kFlexspi1_DqsA_GPIO_EMC_B2_18:
                        break;
                    case kFlexspi1_DqsB_GPIO_SD_B1_05:
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
                case kFlexspi1_DataA_GPIO_SD_B2_11_08:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_08_FLEXSPI1_A_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_09_FLEXSPI1_A_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_10_FLEXSPI1_A_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_11_FLEXSPI1_A_DATA03, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_08_FLEXSPI1_A_DATA00, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_09_FLEXSPI1_A_DATA01, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_10_FLEXSPI1_A_DATA02, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_11_FLEXSPI1_A_DATA03, 0x0AU);
                    break;
                case kFlexspi1_DataA_GPIO_AD_23_20:
                    break;
                case kFlexspi1_DataB_GPIO_SD_B2_03_00:
                    break;
                case kFlexspi1_DataB_GPIO_AD_15_12:
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.ss_b)
            {
                case kFlexspi1_Ss0A_GPIO_SD_B2_06:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_06_FLEXSPI1_A_SS0_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_06_FLEXSPI1_A_SS0_B, 0x0AU);
                    break;
                case kFlexspi1_Ss0A_GPIO_AD_18:
                    break;
                case kFlexspi1_Ss1A_GPIO_SD_B1_02:
                    break;
                case kFlexspi1_Ss0B_GPIO_SD_B1_04:
                    break;
                case kFlexspi1_Ss1B_GPIO_AD_35:
                    break;
                case kFlexspi1_Ss1B_GPIO_SD_B1_03:
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.sclk)
            {
                case kFlexspi1_SclkA_GPIO_SD_B2_07:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_07_FLEXSPI1_A_SCLK, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_07_FLEXSPI1_A_SCLK, 0x0AU);
                    break;
                case kFlexspi1_SclkA_GPIO_AD_19:
                    break;
                case kFlexspi1_SclkB_GPIO_SD_B2_04:
                    break;
                case kFlexspi1_SclkB_GPIO_AD_16:
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs0)
            {
                case kFlexspi1_DqsA_GPIO_SD_B2_05:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_05_FLEXSPI1_A_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_05_FLEXSPI1_A_DQS, 0x0AU);
                    break;
                case kFlexspi1_DqsA_GPIO_AD_17:
                    break;
                case kFlexspi1_DqsA_GPIO_EMC_B2_18:
                    break;
                case kFlexspi1_DqsB_GPIO_SD_B1_05:
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

void bsp_flexspi_clock_init(void)
{
    /*Clock setting for flexspi1*/
    CLOCK_SetRootClockDiv(kCLOCK_Root_Flexspi1, 2);
    CLOCK_SetRootClockMux(kCLOCK_Root_Flexspi1, 0);
}

