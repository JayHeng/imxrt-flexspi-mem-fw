/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "fsl_rgpio.h"
#include "mtu.h"
#include "mtu_pin.h"
#include "mtu_uart.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/////////////////////////////////////////////////////////////
enum _flexspi1_data_line_sel
{
    kFlexspi1_DataA_GPIO_B2_13_10      = 0x00,
    kFlexspi1_DataA_GPIO_B2_06_03      = 0x01,

    kFlexspi1_DataB_GPIO_SD_B2_11_08   = 0x10,
    kFlexspi1_DataB_GPIO_SD_B2_03_00   = 0x11,
    kFlexspi1_DataB_GPIO_B1_13_10      = 0x12,
    kFlexspi1_DataB_GPIO_B1_09_06      = 0x13,
};

enum _flexspi1_ss_b_sel
{
    kFlexspi1_Ss0A_GPIO_B2_09      = 0x00,

    kFlexspi1_Ss0B_GPIO_SD_B2_06   = 0x10,
    kFlexspi1_Ss0B_GPIO_B1_04      = 0x11,
};

enum _flexspi1_sclk_sel
{
    kFlexspi1_SclkA_GPIO_B2_08     = 0x00,

    kFlexspi1_SclkB_GPIO_SD_B2_07  = 0x10,
    kFlexspi1_SclkB_GPIO_B1_05     = 0x11,
};

enum _flexspi1_dqs_sel
{
    kFlexspi1_DqsA_GPIO_B2_07      = 0x00,

    kFlexspi1_DqsB_GPIO_SD_B2_05   = 0x10,
    kFlexspi1_DqsB_GPIO_B1_03      = 0x11,
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
    CLOCK_EnableClock(kCLOCK_Iomuxc1);
    CLOCK_EnableClock(kCLOCK_Iomuxc2);

#if MTU_USE_EVK_SOCKET
    rgpio_pin_config_t do1_config = {kRGPIO_DigitalOutput, 0};
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_04_GPIO4_IO04, 0U);
    RGPIO_PinInit(RGPIO4, 4, &do1_config);
    RGPIO_PinWrite(RGPIO4, 4, 1);
#endif

    if (isPintest)
    {
        pin_unittest_packet_t *packet = (pin_unittest_packet_t *)configPacket;
        rgpio_pin_config_t do_config = {kRGPIO_DigitalOutput, 0};
        memset(s_pinInfo, 0xFF, sizeof(s_pinInfo));
        if (packet->memConnection.instance == 1)
        {
            if (packet->unittestEn.option.B.dataLow4bit)
            {
                switch (packet->memConnection.dataLow4bit)
                {
                    case kFlexspi1_DataA_GPIO_B2_13_10:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_13_GPIO6_IO27, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_12_GPIO6_IO26, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_11_GPIO6_IO25, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_10_GPIO6_IO24, 0U);
                        RGPIO_PinInit(RGPIO6, 27, &do_config);
                        RGPIO_PinInit(RGPIO6, 26, &do_config);
                        RGPIO_PinInit(RGPIO6, 25, &do_config);
                        RGPIO_PinInit(RGPIO6, 24, &do_config);
                        s_pinInfo[0].gpioGroup = 6;
                        s_pinInfo[0].gpioPin = 27;
                        s_pinInfo[1].gpioGroup = 6;
                        s_pinInfo[1].gpioPin = 26;
                        s_pinInfo[2].gpioGroup = 6;
                        s_pinInfo[2].gpioPin = 25;
                        s_pinInfo[3].gpioGroup = 6;
                        s_pinInfo[3].gpioPin = 24;
                        break;
                    case kFlexspi1_DataB_GPIO_SD_B2_11_08:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_11_GPIO5_IO21, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_10_GPIO5_IO20, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_09_GPIO5_IO19, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_08_GPIO5_IO18, 0U);
                        RGPIO_PinInit(RGPIO5, 21, &do_config);
                        RGPIO_PinInit(RGPIO5, 20, &do_config);
                        RGPIO_PinInit(RGPIO5, 19, &do_config);
                        RGPIO_PinInit(RGPIO5, 18, &do_config);
                        s_pinInfo[0].gpioGroup = 5;
                        s_pinInfo[0].gpioPin = 21;
                        s_pinInfo[1].gpioGroup = 5;
                        s_pinInfo[1].gpioPin = 20;
                        s_pinInfo[2].gpioGroup = 5;
                        s_pinInfo[2].gpioPin = 19;
                        s_pinInfo[3].gpioGroup = 5;
                        s_pinInfo[3].gpioPin = 18;
                        break;
                    case kFlexspi1_DataB_GPIO_B1_13_10:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B1_13_GPIO6_IO13, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_GPIO6_IO12, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_GPIO6_IO11, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B1_10_GPIO6_IO10, 0U);
                        RGPIO_PinInit(RGPIO6, 13, &do_config);
                        RGPIO_PinInit(RGPIO6, 12, &do_config);
                        RGPIO_PinInit(RGPIO6, 11, &do_config);
                        RGPIO_PinInit(RGPIO6, 10, &do_config);
                        s_pinInfo[0].gpioGroup = 6;
                        s_pinInfo[0].gpioPin = 13;
                        s_pinInfo[1].gpioGroup = 6;
                        s_pinInfo[1].gpioPin = 12;
                        s_pinInfo[2].gpioGroup = 6;
                        s_pinInfo[2].gpioPin = 11;
                        s_pinInfo[3].gpioGroup = 6;
                        s_pinInfo[3].gpioPin = 10;
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.ss_b)
            {
                switch (packet->memConnection.ss_b)
                {
                    case kFlexspi1_Ss0A_GPIO_B2_09:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_09_GPIO6_IO23, 0U);
                        RGPIO_PinInit(RGPIO6, 23, &do_config);
                        s_pinInfo[4].gpioGroup = 6;
                        s_pinInfo[4].gpioPin = 23;
                        break;
                    case kFlexspi1_Ss0B_GPIO_SD_B2_06:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_06_GPIO5_IO16, 0U);
                        RGPIO_PinInit(RGPIO5, 16, &do_config);
                        s_pinInfo[4].gpioGroup = 5;
                        s_pinInfo[4].gpioPin = 16;
                        break;
                    case kFlexspi1_Ss0B_GPIO_B1_04:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B1_04_GPIO6_IO04, 0U);
                        RGPIO_PinInit(RGPIO6, 4, &do_config);
                        s_pinInfo[4].gpioGroup = 6;
                        s_pinInfo[4].gpioPin = 4;
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.sclk)
            {
                switch (packet->memConnection.sclk)
                {
                    case kFlexspi1_SclkA_GPIO_B2_08:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_08_GPIO6_IO22, 0U);
                        RGPIO_PinInit(RGPIO6, 22, &do_config);
                        s_pinInfo[5].gpioGroup = 6;
                        s_pinInfo[5].gpioPin = 22;
                        break;
                    case kFlexspi1_SclkB_GPIO_SD_B2_07:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_07_GPIO5_IO17, 0U);
                        RGPIO_PinInit(RGPIO5, 17, &do_config);
                        s_pinInfo[5].gpioGroup = 5;
                        s_pinInfo[5].gpioPin = 17;
                        break;
                    case kFlexspi1_SclkB_GPIO_B1_05:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B1_05_GPIO6_IO05, 0U);
                        RGPIO_PinInit(RGPIO6, 5, &do_config);
                        s_pinInfo[5].gpioGroup = 6;
                        s_pinInfo[5].gpioPin = 5;
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.dqs)
            {
                switch (packet->memConnection.dqs)
                {
                    case kFlexspi1_DqsA_GPIO_B2_07:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_07_GPIO6_IO21, 0U);
                        RGPIO_PinInit(RGPIO6, 21, &do_config);
                        s_pinInfo[6].gpioGroup = 6;
                        s_pinInfo[6].gpioPin = 21;
                        break;
                    case kFlexspi1_DqsB_GPIO_SD_B2_05:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_05_GPIO5_IO15, 0U);
                        RGPIO_PinInit(RGPIO5, 15, &do_config);
                        s_pinInfo[6].gpioGroup = 5;
                        s_pinInfo[6].gpioPin = 15;
                        break;
                    case kFlexspi1_DqsB_GPIO_B1_03:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B1_03_GPIO6_IO03, 0U);
                        RGPIO_PinInit(RGPIO6, 3, &do_config);
                        s_pinInfo[6].gpioGroup = 6;
                        s_pinInfo[6].gpioPin = 3;
                        break;
                    default:
                        break;
                }
            }
            if (packet->unittestEn.option.B.dataHigh4bit)
            {
                switch (packet->memConnection.dataHigh4bit)
                {
                    case kFlexspi1_DataA_GPIO_B2_06_03:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_06_GPIO6_IO20, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_05_GPIO6_IO19, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_04_GPIO6_IO18, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_03_GPIO6_IO17, 0U);
                        RGPIO_PinInit(RGPIO6, 20, &do_config);
                        RGPIO_PinInit(RGPIO6, 19, &do_config);
                        RGPIO_PinInit(RGPIO6, 18, &do_config);
                        RGPIO_PinInit(RGPIO6, 17, &do_config);
                        s_pinInfo[7].gpioGroup = 6;
                        s_pinInfo[7].gpioPin = 20;
                        s_pinInfo[8].gpioGroup = 6;
                        s_pinInfo[8].gpioPin = 19;
                        s_pinInfo[9].gpioGroup = 6;
                        s_pinInfo[9].gpioPin = 18;
                        s_pinInfo[10].gpioGroup = 6;
                        s_pinInfo[10].gpioPin = 17;
                        break;
                    case kFlexspi1_DataB_GPIO_SD_B2_03_00:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_03_GPIO5_IO13, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_02_GPIO5_IO12, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_01_GPIO5_IO11, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_00_GPIO5_IO10, 0U);
                        RGPIO_PinInit(RGPIO5, 13, &do_config);
                        RGPIO_PinInit(RGPIO5, 12, &do_config);
                        RGPIO_PinInit(RGPIO5, 11, &do_config);
                        RGPIO_PinInit(RGPIO5, 10, &do_config);
                        s_pinInfo[7].gpioGroup = 5;
                        s_pinInfo[7].gpioPin = 13;
                        s_pinInfo[8].gpioGroup = 5;
                        s_pinInfo[8].gpioPin = 12;
                        s_pinInfo[9].gpioGroup = 5;
                        s_pinInfo[9].gpioPin = 11;
                        s_pinInfo[10].gpioGroup = 5;
                        s_pinInfo[10].gpioPin = 10;
                        break;
                    case kFlexspi1_DataB_GPIO_B1_09_06:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_GPIO6_IO09, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_GPIO6_IO08, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B1_07_GPIO6_IO07, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B1_06_GPIO6_IO06, 0U);
                        RGPIO_PinInit(RGPIO6, 9, &do_config);
                        RGPIO_PinInit(RGPIO6, 8, &do_config);
                        RGPIO_PinInit(RGPIO6, 7, &do_config);
                        RGPIO_PinInit(RGPIO6, 6, &do_config);
                        s_pinInfo[7].gpioGroup = 6;
                        s_pinInfo[7].gpioPin = 9;
                        s_pinInfo[8].gpioGroup = 6;
                        s_pinInfo[8].gpioPin = 8;
                        s_pinInfo[9].gpioGroup = 6;
                        s_pinInfo[9].gpioPin = 7;
                        s_pinInfo[10].gpioGroup = 6;
                        s_pinInfo[10].gpioPin = 6;
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
                case kFlexspi1_DataA_GPIO_B2_13_10:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_10_FLEXSPI1_BUS2BIT_A_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_11_FLEXSPI1_BUS2BIT_A_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_12_FLEXSPI1_BUS2BIT_A_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_13_FLEXSPI1_BUS2BIT_A_DATA03, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_10_FLEXSPI1_BUS2BIT_A_DATA00, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_11_FLEXSPI1_BUS2BIT_A_DATA01, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_12_FLEXSPI1_BUS2BIT_A_DATA02, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_13_FLEXSPI1_BUS2BIT_A_DATA03, 0x0AU);
                    break;
                case kFlexspi1_DataB_GPIO_SD_B2_11_08:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_08_FLEXSPI1_BUS2BIT_B_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_09_FLEXSPI1_BUS2BIT_B_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_10_FLEXSPI1_BUS2BIT_B_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_11_FLEXSPI1_BUS2BIT_B_DATA03, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_08_FLEXSPI1_BUS2BIT_B_DATA00, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_09_FLEXSPI1_BUS2BIT_B_DATA01, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_10_FLEXSPI1_BUS2BIT_B_DATA02, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_11_FLEXSPI1_BUS2BIT_B_DATA03, 0x0AU);
                    break;
                case kFlexspi1_DataB_GPIO_B1_13_10:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_13_FLEXSPI1_BUS2BIT_B_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_FLEXSPI1_BUS2BIT_B_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_FLEXSPI1_BUS2BIT_B_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_10_FLEXSPI1_BUS2BIT_B_DATA03, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_13_FLEXSPI1_BUS2BIT_B_DATA00, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_12_FLEXSPI1_BUS2BIT_B_DATA01, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_FLEXSPI1_BUS2BIT_B_DATA02, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_10_FLEXSPI1_BUS2BIT_B_DATA03, 0x0AU);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.ss_b)
            {
                case kFlexspi1_Ss0A_GPIO_B2_09:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_09_FLEXSPI1_BUS2BIT_A_SS0_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_09_FLEXSPI1_BUS2BIT_A_SS0_B, 0x0AU);
                    break;
                case kFlexspi1_Ss0B_GPIO_SD_B2_06:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_06_FLEXSPI1_BUS2BIT_B_SS0_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_06_FLEXSPI1_BUS2BIT_B_SS0_B, 0x0AU);
                    break;
                case kFlexspi1_Ss0B_GPIO_B1_04:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_04_FLEXSPI1_BUS2BIT_B_SS0_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_04_FLEXSPI1_BUS2BIT_B_SS0_B, 0x0AU);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.sclk)
            {
                case kFlexspi1_SclkA_GPIO_B2_08:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_08_FLEXSPI1_BUS2BIT_A_SCLK, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_08_FLEXSPI1_BUS2BIT_A_SCLK, 0x0AU);
                    break;
                case kFlexspi1_SclkB_GPIO_SD_B2_07:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_07_FLEXSPI1_BUS2BIT_B_SCLK, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_07_FLEXSPI1_BUS2BIT_B_SCLK, 0x0AU);
                    break;
                case kFlexspi1_SclkB_GPIO_B1_05:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_05_FLEXSPI1_BUS2BIT_B_SCLK, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_05_FLEXSPI1_BUS2BIT_B_SCLK, 0x0AU);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs)
            {
                case kFlexspi1_DqsA_GPIO_B2_07:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_07_FLEXSPI1_BUS2BIT_A_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_07_FLEXSPI1_BUS2BIT_A_DQS, 0x0AU);
                    break;
                case kFlexspi1_DqsB_GPIO_SD_B2_05:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_05_FLEXSPI1_BUS2BIT_B_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_05_FLEXSPI1_BUS2BIT_B_DQS, 0x0AU);
                    break;
                case kFlexspi1_DqsB_GPIO_B1_03:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_03_FLEXSPI1_BUS2BIT_B_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_03_FLEXSPI1_BUS2BIT_B_DQS, 0x0AU);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dataHigh4bit)
            {
                case kFlexspi1_DataA_GPIO_B2_06_03:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_06_FLEXSPI1_BUS2BIT_A_DATA07, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_05_FLEXSPI1_BUS2BIT_A_DATA06, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_04_FLEXSPI1_BUS2BIT_A_DATA05, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_03_FLEXSPI1_BUS2BIT_A_DATA04, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_06_FLEXSPI1_BUS2BIT_A_DATA07, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_05_FLEXSPI1_BUS2BIT_A_DATA06, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_04_FLEXSPI1_BUS2BIT_A_DATA05, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_03_FLEXSPI1_BUS2BIT_A_DATA04, 0x0AU);
                    break;
                case kFlexspi1_DataB_GPIO_SD_B2_03_00:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_03_FLEXSPI1_BUS2BIT_B_DATA07, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_02_FLEXSPI1_BUS2BIT_B_DATA06, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_01_FLEXSPI1_BUS2BIT_B_DATA05, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_00_FLEXSPI1_BUS2BIT_B_DATA04, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_03_FLEXSPI1_BUS2BIT_B_DATA07, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_02_FLEXSPI1_BUS2BIT_B_DATA06, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_01_FLEXSPI1_BUS2BIT_B_DATA05, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_00_FLEXSPI1_BUS2BIT_B_DATA04, 0x0AU);
                    break;
                case kFlexspi1_DataB_GPIO_B1_09_06:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_06_FLEXSPI1_BUS2BIT_B_DATA07, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_07_FLEXSPI1_BUS2BIT_B_DATA06, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_FLEXSPI1_BUS2BIT_B_DATA05, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_FLEXSPI1_BUS2BIT_B_DATA04, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_06_FLEXSPI1_BUS2BIT_B_DATA07, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_07_FLEXSPI1_BUS2BIT_B_DATA06, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_FLEXSPI1_BUS2BIT_B_DATA05, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_09_FLEXSPI1_BUS2BIT_B_DATA04, 0x0AU);
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
  
    RGPIO_Type *gpioBase[] = RGPIO_BASE_PTRS;
    for (uint32_t i = 0; i < sizeof(s_pinInfo) / sizeof(pin_info_t); i++)
    {
        if (s_pinInfo[i].gpioGroup != 0xFFFFu)
        {
            RGPIO_PortToggle(gpioBase[s_pinInfo[i].gpioGroup], 1u << s_pinInfo[i].gpioPin);
        }
    }
}

