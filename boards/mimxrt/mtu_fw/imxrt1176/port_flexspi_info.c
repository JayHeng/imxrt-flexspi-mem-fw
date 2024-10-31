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

#define MTU_USE_FSPI1_PORTA1                      \
    {                                             \
        s_userConfig.mixspiBase = FLEXSPI1;       \
        s_userConfig.mixspiPort = kFLEXSPI_PortA1;\
    }
#define MTU_USE_FSPI1_PORTA2                      \
    {                                             \
        s_userConfig.mixspiBase = FLEXSPI1;       \
        s_userConfig.mixspiPort = kFLEXSPI_PortA2;\
    }
#define MTU_USE_FSPI1_PORTB1                      \
    {                                             \
        s_userConfig.mixspiBase = FLEXSPI1;       \
        s_userConfig.mixspiPort = kFLEXSPI_PortB1;\
    }
#define MTU_USE_FSPI1_PORTB2                      \
    {                                             \
        s_userConfig.mixspiBase = FLEXSPI1;       \
        s_userConfig.mixspiPort = kFLEXSPI_PortB2;\
    }
#define MTU_USE_FSPI2_PORTA1                      \
    {                                             \
        s_userConfig.mixspiBase = FLEXSPI2;       \
        s_userConfig.mixspiPort = kFLEXSPI_PortA1;\
    }
#define MTU_USE_FSPI2_PORTA2                      \
    {                                             \
        s_userConfig.mixspiBase = FLEXSPI2;       \
        s_userConfig.mixspiPort = kFLEXSPI_PortA2;\
    }
#define MTU_USE_FSPI2_PORTB1                      \
    {                                             \
        s_userConfig.mixspiBase = FLEXSPI2;       \
        s_userConfig.mixspiPort = kFLEXSPI_PortB1;\
    }
#define MTU_USE_FSPI2_PORTB2                      \
    {                                             \
        s_userConfig.mixspiBase = FLEXSPI2;       \
        s_userConfig.mixspiPort = kFLEXSPI_PortB2;\
    }

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

   
/*******************************************************************************
 * Variables
 ******************************************************************************/

pin_info_t s_pinInfo[MTU_MAX_PINS];

/*******************************************************************************
 * Code
 ******************************************************************************/

void bsp_mixspi_pinmux_config(void *configPacket, bool isPintest)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);

    if (isPintest)
    {
        pin_unittest_packet_t *packet = (pin_unittest_packet_t *)configPacket;
        gpio_pin_config_t do_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
        memset(s_pinInfo, 0xFF, sizeof(s_pinInfo));
        if (packet->memConnection.instance == 1)
        {
            if (packet->pintestEn.option.B.dataLow4bit)
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
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_20_GPIO9_IO19, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_21_GPIO9_IO20, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_22_GPIO9_IO21, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_23_GPIO9_IO22, 0U);
                        GPIO_PinInit(GPIO9, 19, &do_config);
                        GPIO_PinInit(GPIO9, 20, &do_config);
                        GPIO_PinInit(GPIO9, 21, &do_config);
                        GPIO_PinInit(GPIO9, 22, &do_config);
                        s_pinInfo[0].gpioGroup = 9;
                        s_pinInfo[0].gpioPin = 19;
                        s_pinInfo[1].gpioGroup = 9;
                        s_pinInfo[1].gpioPin = 20;
                        s_pinInfo[2].gpioGroup = 9;
                        s_pinInfo[2].gpioPin = 21;
                        s_pinInfo[3].gpioGroup = 9;
                        s_pinInfo[3].gpioPin = 22;
                        break;
                    case kFlexspi1_DataB_GPIO_SD_B2_03_00:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_00_GPIO10_IO09, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_01_GPIO10_IO10, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_02_GPIO10_IO11, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_03_GPIO10_IO12, 0U);
                        GPIO_PinInit(GPIO10, 9, &do_config);
                        GPIO_PinInit(GPIO10, 10, &do_config);
                        GPIO_PinInit(GPIO10, 11, &do_config);
                        GPIO_PinInit(GPIO10, 12, &do_config);
                        s_pinInfo[0].gpioGroup = 10;
                        s_pinInfo[0].gpioPin = 9;
                        s_pinInfo[1].gpioGroup = 10;
                        s_pinInfo[1].gpioPin = 10;
                        s_pinInfo[2].gpioGroup = 10;
                        s_pinInfo[2].gpioPin = 11;
                        s_pinInfo[3].gpioGroup = 10;
                        s_pinInfo[3].gpioPin = 12;
                        break;
                    case kFlexspi1_DataB_GPIO_AD_15_12:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_12_GPIO9_IO11, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_13_GPIO9_IO12, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_14_GPIO9_IO13, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_15_GPIO9_IO14, 0U);
                        GPIO_PinInit(GPIO9, 11, &do_config);
                        GPIO_PinInit(GPIO9, 12, &do_config);
                        GPIO_PinInit(GPIO9, 13, &do_config);
                        GPIO_PinInit(GPIO9, 14, &do_config);
                        s_pinInfo[0].gpioGroup = 9;
                        s_pinInfo[0].gpioPin = 11;
                        s_pinInfo[1].gpioGroup = 9;
                        s_pinInfo[1].gpioPin = 12;
                        s_pinInfo[2].gpioGroup = 9;
                        s_pinInfo[2].gpioPin = 13;
                        s_pinInfo[3].gpioGroup = 9;
                        s_pinInfo[3].gpioPin = 14;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.ss_b)
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
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_18_GPIO9_IO17, 0U);
                        GPIO_PinInit(GPIO9, 17, &do_config);
                        s_pinInfo[4].gpioGroup = 9;
                        s_pinInfo[4].gpioPin = 17;
                        break;
                    case kFlexspi1_Ss1A_GPIO_SD_B1_02:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_GPIO10_IO05, 0U);
                        GPIO_PinInit(GPIO10, 5, &do_config);
                        s_pinInfo[4].gpioGroup = 10;
                        s_pinInfo[4].gpioPin = 5;
                        break;
                    case kFlexspi1_Ss0B_GPIO_SD_B1_04:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO10_IO07, 0U);
                        GPIO_PinInit(GPIO10, 7, &do_config);
                        s_pinInfo[4].gpioGroup = 10;
                        s_pinInfo[4].gpioPin = 7;
                        break;
                    case kFlexspi1_Ss1B_GPIO_AD_35:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_35_GPIO10_IO02, 0U);
                        GPIO_PinInit(GPIO10, 2, &do_config);
                        s_pinInfo[4].gpioGroup = 10;
                        s_pinInfo[4].gpioPin = 2;
                        break;
                    case kFlexspi1_Ss1B_GPIO_SD_B1_03:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_GPIO10_IO06, 0U);
                        GPIO_PinInit(GPIO10, 6, &do_config);
                        s_pinInfo[4].gpioGroup = 10;
                        s_pinInfo[4].gpioPin = 6;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.sclk)
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
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_19_GPIO9_IO18, 0U);
                        GPIO_PinInit(GPIO9, 18, &do_config);
                        s_pinInfo[5].gpioGroup = 9;
                        s_pinInfo[5].gpioPin = 18;
                        break;
                    case kFlexspi1_SclkB_GPIO_SD_B2_04:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_04_GPIO10_IO13, 0U);
                        GPIO_PinInit(GPIO10, 13, &do_config);
                        s_pinInfo[5].gpioGroup = 10;
                        s_pinInfo[5].gpioPin = 13;
                        break;
                    case kFlexspi1_SclkB_GPIO_AD_16:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_16_GPIO9_IO15, 0U);
                        GPIO_PinInit(GPIO9, 15, &do_config);
                        s_pinInfo[5].gpioGroup = 9;
                        s_pinInfo[5].gpioPin = 15;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.dqs0)
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
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_17_GPIO9_IO16, 0U);
                        GPIO_PinInit(GPIO9, 16, &do_config);
                        s_pinInfo[6].gpioGroup = 9;
                        s_pinInfo[6].gpioPin = 16;
                        break;
                    case kFlexspi1_DqsA_GPIO_EMC_B2_18:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_18_GPIO8_IO28, 0U);
                        GPIO_PinInit(GPIO8, 28, &do_config);
                        s_pinInfo[6].gpioGroup = 8;
                        s_pinInfo[6].gpioPin = 28;
                        break;
                    case kFlexspi1_DqsB_GPIO_SD_B1_05:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_GPIO10_IO08, 0U);
                        GPIO_PinInit(GPIO10, 8, &do_config);
                        s_pinInfo[6].gpioGroup = 10;
                        s_pinInfo[6].gpioPin = 8;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.dataHigh4bit)
            {
                switch (packet->memConnection.dataHigh4bit)
                {
                    case kFlexspi1_DataB_GPIO_SD_B2_03_00:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_00_GPIO10_IO09, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_01_GPIO10_IO10, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_02_GPIO10_IO11, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_03_GPIO10_IO12, 0U);
                        GPIO_PinInit(GPIO10, 9, &do_config);
                        GPIO_PinInit(GPIO10, 10, &do_config);
                        GPIO_PinInit(GPIO10, 11, &do_config);
                        GPIO_PinInit(GPIO10, 12, &do_config);
                        s_pinInfo[7].gpioGroup = 10;
                        s_pinInfo[7].gpioPin = 9;
                        s_pinInfo[8].gpioGroup = 10;
                        s_pinInfo[8].gpioPin = 10;
                        s_pinInfo[9].gpioGroup = 10;
                        s_pinInfo[9].gpioPin = 11;
                        s_pinInfo[10].gpioGroup = 10;
                        s_pinInfo[10].gpioPin = 12;
                        break;
                    case kFlexspi1_DataB_GPIO_AD_15_12:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_12_GPIO9_IO11, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_13_GPIO9_IO12, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_14_GPIO9_IO13, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_15_GPIO9_IO14, 0U);
                        GPIO_PinInit(GPIO9, 11, &do_config);
                        GPIO_PinInit(GPIO9, 12, &do_config);
                        GPIO_PinInit(GPIO9, 13, &do_config);
                        GPIO_PinInit(GPIO9, 14, &do_config);
                        s_pinInfo[7].gpioGroup = 9;
                        s_pinInfo[7].gpioPin = 11;
                        s_pinInfo[8].gpioGroup = 9;
                        s_pinInfo[8].gpioPin = 12;
                        s_pinInfo[9].gpioGroup = 9;
                        s_pinInfo[9].gpioPin = 13;
                        s_pinInfo[10].gpioGroup = 9;
                        s_pinInfo[10].gpioPin = 14;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.dataTop8bit)
            {
            }
            if (packet->pintestEn.option.B.sclk_n)
            {
                switch (packet->memConnection.sclk_n)
                {
                    case kFlexspi1_SclkB_GPIO_SD_B2_04:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_04_GPIO10_IO13, 0U);
                        GPIO_PinInit(GPIO10, 13, &do_config);
                        s_pinInfo[19].gpioGroup = 10;
                        s_pinInfo[19].gpioPin = 13;
                        break;
                    case kFlexspi1_SclkB_GPIO_AD_16:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_16_GPIO9_IO15, 0U);
                        GPIO_PinInit(GPIO9, 15, &do_config);
                        s_pinInfo[19].gpioGroup = 9;
                        s_pinInfo[19].gpioPin = 15;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.dqs1)
            {
                switch (packet->memConnection.dqs1)
                {
                    case kFlexspi1_DqsB_GPIO_SD_B1_05:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_GPIO10_IO08, 0U);
                        GPIO_PinInit(GPIO10, 8, &do_config);
                        s_pinInfo[20].gpioGroup = 10;
                        s_pinInfo[20].gpioPin = 8;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.rst_b)
            {
            }
        }
        else if (packet->memConnection.instance == 2)
        {
            if (packet->pintestEn.option.B.dataLow4bit)
            {
                switch (packet->memConnection.dataLow4bit)
                {
                    case kFlexspi2_DataA_GPIO_EMC_B2_16_13:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_16_GPIO8_IO26, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_15_GPIO8_IO25, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_14_GPIO8_IO24, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_13_GPIO8_IO23, 0U);
                        GPIO_PinInit(GPIO8, 26, &do_config);
                        GPIO_PinInit(GPIO8, 25, &do_config);
                        GPIO_PinInit(GPIO8, 24, &do_config);
                        GPIO_PinInit(GPIO8, 23, &do_config);
                        s_pinInfo[0].gpioGroup = 8;
                        s_pinInfo[0].gpioPin = 26;
                        s_pinInfo[1].gpioGroup = 8;
                        s_pinInfo[1].gpioPin = 25;
                        s_pinInfo[2].gpioGroup = 8;
                        s_pinInfo[2].gpioPin = 24;
                        s_pinInfo[3].gpioGroup = 8;
                        s_pinInfo[3].gpioPin = 23;
                        break;
                    case kFlexspi2_DataA_GPIO_SD_B1_05_02:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_GPIO10_IO08, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO10_IO07, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_GPIO10_IO06, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_GPIO10_IO05, 0U);
                        GPIO_PinInit(GPIO10, 8, &do_config);
                        GPIO_PinInit(GPIO10, 7, &do_config);
                        GPIO_PinInit(GPIO10, 6, &do_config);
                        GPIO_PinInit(GPIO10, 5, &do_config);
                        s_pinInfo[0].gpioGroup = 10;
                        s_pinInfo[0].gpioPin = 8;
                        s_pinInfo[1].gpioGroup = 10;
                        s_pinInfo[1].gpioPin = 7;
                        s_pinInfo[2].gpioGroup = 10;
                        s_pinInfo[2].gpioPin = 6;
                        s_pinInfo[3].gpioGroup = 10;
                        s_pinInfo[3].gpioPin = 5;
                        break;
                    case kFlexspi2_DataB_GPIO_EMC_B2_06_03:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_06_GPIO8_IO16, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_05_GPIO8_IO15, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_04_GPIO8_IO14, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_03_GPIO8_IO13, 0U);
                        GPIO_PinInit(GPIO8, 16, &do_config);
                        GPIO_PinInit(GPIO8, 15, &do_config);
                        GPIO_PinInit(GPIO8, 14, &do_config);
                        GPIO_PinInit(GPIO8, 13, &do_config);
                        s_pinInfo[0].gpioGroup = 8;
                        s_pinInfo[0].gpioPin = 16;
                        s_pinInfo[1].gpioGroup = 8;
                        s_pinInfo[1].gpioPin = 15;
                        s_pinInfo[2].gpioGroup = 8;
                        s_pinInfo[2].gpioPin = 14;
                        s_pinInfo[3].gpioGroup = 8;
                        s_pinInfo[3].gpioPin = 13;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.ss_b)
            {
                switch (packet->memConnection.ss_b)
                {
                    case kFlexspi2_Ss0A_GPIO_EMC_B2_11:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_11_GPIO8_IO21, 0U);
                        GPIO_PinInit(GPIO8, 21, &do_config);
                        s_pinInfo[4].gpioGroup = 8;
                        s_pinInfo[4].gpioPin = 21;
                        break;
                    case kFlexspi2_Ss0A_GPIO_SD_B1_00:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_00_GPIO10_IO03, 0U);
                        GPIO_PinInit(GPIO10, 3, &do_config);
                        s_pinInfo[4].gpioGroup = 10;
                        s_pinInfo[4].gpioPin = 3;
                        break;
                    case kFlexspi2_Ss1A_GPIO_AD_01:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_01_GPIO9_IO00, 0U);
                        GPIO_PinInit(GPIO9, 0, &do_config);
                        s_pinInfo[4].gpioGroup = 9;
                        s_pinInfo[4].gpioPin = 0;
                        break;
                    case kFlexspi2_Ss0B_GPIO_EMC_B2_08:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_08_GPIO8_IO18, 0U);
                        GPIO_PinInit(GPIO8, 18, &do_config);
                        s_pinInfo[4].gpioGroup = 8;
                        s_pinInfo[4].gpioPin = 18;
                        break;
                    case kFlexspi2_Ss1B_GPIO_AD_00:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_00_GPIO8_IO31, 0U);
                        GPIO_PinInit(GPIO8, 31, &do_config);
                        s_pinInfo[4].gpioGroup = 8;
                        s_pinInfo[4].gpioPin = 31;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.sclk)
            {
                switch (packet->memConnection.sclk)
                {
                    case kFlexspi2_SclkA_GPIO_EMC_B2_10:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_10_GPIO8_IO20, 0U);
                        GPIO_PinInit(GPIO8, 20, &do_config);
                        s_pinInfo[5].gpioGroup = 8;
                        s_pinInfo[5].gpioPin = 20;
                        break;
                    case kFlexspi2_SclkA_GPIO_SD_B1_01:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_01_GPIO10_IO04, 0U);
                        GPIO_PinInit(GPIO10, 4, &do_config);
                        s_pinInfo[5].gpioGroup = 10;
                        s_pinInfo[5].gpioPin = 4;
                        break;
                    case kFlexspi2_SclkB_GPIO_EMC_B2_09:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_09_GPIO8_IO19, 0U);
                        GPIO_PinInit(GPIO8, 19, &do_config);
                        s_pinInfo[5].gpioGroup = 8;
                        s_pinInfo[5].gpioPin = 19;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.dqs0)
            {
                switch (packet->memConnection.dqs0)
                {
                    case kFlexspi2_DqsA_GPIO_EMC_B2_12:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_12_GPIO8_IO22, 0U);
                        GPIO_PinInit(GPIO8, 22, &do_config);
                        s_pinInfo[6].gpioGroup = 8;
                        s_pinInfo[6].gpioPin = 22;
                        break;
                    case kFlexspi2_DqsB_GPIO_EMC_B2_07:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_07_GPIO8_IO17, 0U);
                        GPIO_PinInit(GPIO8, 17, &do_config);
                        s_pinInfo[6].gpioGroup = 8;
                        s_pinInfo[6].gpioPin = 17;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.dataHigh4bit)
            {
                switch (packet->memConnection.dataHigh4bit)
                {
                    case kFlexspi2_DataA_GPIO_EMC_B2_20_17:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_20_GPIO8_IO30, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_19_GPIO8_IO29, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_18_GPIO8_IO28, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_17_GPIO8_IO27, 0U);
                        GPIO_PinInit(GPIO8, 30, &do_config);
                        GPIO_PinInit(GPIO8, 29, &do_config);
                        GPIO_PinInit(GPIO8, 28, &do_config);
                        GPIO_PinInit(GPIO8, 27, &do_config);
                        s_pinInfo[7].gpioGroup = 8;
                        s_pinInfo[7].gpioPin = 30;
                        s_pinInfo[8].gpioGroup = 8;
                        s_pinInfo[8].gpioPin = 29;
                        s_pinInfo[9].gpioGroup = 8;
                        s_pinInfo[9].gpioPin = 28;
                        s_pinInfo[10].gpioGroup = 8;
                        s_pinInfo[10].gpioPin = 27;
                        break;
                    case kFlexspi2_DataB_GPIO_EMC_B1_41_B2_02_00:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_41_GPIO8_IO09, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_00_GPIO8_IO10, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_01_GPIO8_IO11, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_02_GPIO8_IO12, 0U);
                        GPIO_PinInit(GPIO8, 9, &do_config);
                        GPIO_PinInit(GPIO8, 10, &do_config);
                        GPIO_PinInit(GPIO8, 11, &do_config);
                        GPIO_PinInit(GPIO8, 12, &do_config);
                        s_pinInfo[7].gpioGroup = 8;
                        s_pinInfo[7].gpioPin = 9;
                        s_pinInfo[8].gpioGroup = 8;
                        s_pinInfo[8].gpioPin = 10;
                        s_pinInfo[9].gpioGroup = 8;
                        s_pinInfo[9].gpioPin = 11;
                        s_pinInfo[10].gpioGroup = 8;
                        s_pinInfo[10].gpioPin = 12;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.dataTop8bit)
            {
            }
            if (packet->pintestEn.option.B.sclk_n)
            {
                switch (packet->memConnection.sclk_n)
                {
                    case kFlexspi2_SclkB_GPIO_EMC_B2_09:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_09_GPIO8_IO19, 0U);
                        GPIO_PinInit(GPIO8, 19, &do_config);
                        s_pinInfo[19].gpioGroup = 8;
                        s_pinInfo[19].gpioPin = 19;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.dqs1)
            {
                switch (packet->memConnection.dqs1)
                {
                    case kFlexspi2_DqsB_GPIO_EMC_B2_07:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_07_GPIO8_IO17, 0U);
                        GPIO_PinInit(GPIO8, 17, &do_config);
                        s_pinInfo[20].gpioGroup = 8;
                        s_pinInfo[20].gpioPin = 17;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.rst_b)
            {
            }
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
                    if (packet->padCtrl.dataLow4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_08_FLEXSPI1_A_DATA00, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_09_FLEXSPI1_A_DATA01, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_10_FLEXSPI1_A_DATA02, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_11_FLEXSPI1_A_DATA03, packet->padCtrl.dataLow4bit);
                    }
                    break;
                case kFlexspi1_DataA_GPIO_AD_23_20:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_20_FLEXSPI1_A_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_21_FLEXSPI1_A_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_22_FLEXSPI1_A_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_23_FLEXSPI1_A_DATA03, 1U);
                    if (packet->padCtrl.dataLow4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_20_FLEXSPI1_A_DATA00, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_21_FLEXSPI1_A_DATA01, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_22_FLEXSPI1_A_DATA02, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_23_FLEXSPI1_A_DATA03, packet->padCtrl.dataLow4bit);
                    }
                    break;
                case kFlexspi1_DataB_GPIO_SD_B2_03_00:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_00_FLEXSPI1_B_DATA03, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_01_FLEXSPI1_B_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_02_FLEXSPI1_B_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_03_FLEXSPI1_B_DATA00, 1U);
                    if (packet->padCtrl.dataLow4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_00_FLEXSPI1_B_DATA03, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_01_FLEXSPI1_B_DATA02, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_02_FLEXSPI1_B_DATA01, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_03_FLEXSPI1_B_DATA00, packet->padCtrl.dataLow4bit);
                    }
                    break;
                case kFlexspi1_DataB_GPIO_AD_15_12:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_12_FLEXSPI1_B_DATA03, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_13_FLEXSPI1_B_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_14_FLEXSPI1_B_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_15_FLEXSPI1_B_DATA00, 1U);
                    if (packet->padCtrl.dataLow4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_12_FLEXSPI1_B_DATA03, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_13_FLEXSPI1_B_DATA02, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_14_FLEXSPI1_B_DATA01, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_15_FLEXSPI1_B_DATA00, packet->padCtrl.dataLow4bit);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.ss_b)
            {
                case kFlexspi1_Ss0A_GPIO_SD_B2_06:
                    MTU_USE_FSPI1_PORTA1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_06_FLEXSPI1_A_SS0_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_06_FLEXSPI1_A_SS0_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi1_Ss0A_GPIO_AD_18:
                    MTU_USE_FSPI1_PORTA1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_18_FLEXSPI1_A_SS0_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_18_FLEXSPI1_A_SS0_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi1_Ss1A_GPIO_SD_B1_02:
                    MTU_USE_FSPI1_PORTA2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_FLEXSPI1_A_SS1_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_02_FLEXSPI1_A_SS1_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi1_Ss0B_GPIO_SD_B1_04:
                    MTU_USE_FSPI1_PORTB1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_FLEXSPI1_B_SS0_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_FLEXSPI1_B_SS0_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi1_Ss1B_GPIO_AD_35:
                    MTU_USE_FSPI1_PORTB2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_35_FLEXSPI1_B_SS1_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_35_FLEXSPI1_B_SS1_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi1_Ss1B_GPIO_SD_B1_03:
                    MTU_USE_FSPI1_PORTB2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_FLEXSPI1_B_SS1_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_03_FLEXSPI1_B_SS1_B, packet->padCtrl.ss_b);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.sclk)
            {
                case kFlexspi1_SclkA_GPIO_SD_B2_07:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_07_FLEXSPI1_A_SCLK, 1U);
                    if (packet->padCtrl.sclk != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_07_FLEXSPI1_A_SCLK, packet->padCtrl.sclk);
                    }
                    break;
                case kFlexspi1_SclkA_GPIO_AD_19:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_19_FLEXSPI1_A_SCLK, 1U);
                    if (packet->padCtrl.sclk != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_19_FLEXSPI1_A_SCLK, packet->padCtrl.sclk);
                    }
                    break;
                case kFlexspi1_SclkB_GPIO_SD_B2_04:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_04_FLEXSPI1_B_SCLK, 1U);
                    if (packet->padCtrl.sclk != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_04_FLEXSPI1_B_SCLK, packet->padCtrl.sclk);
                    }
                    break;
                case kFlexspi1_SclkB_GPIO_AD_16:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_16_FLEXSPI1_B_SCLK, 1U);
                    if (packet->padCtrl.sclk != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_16_FLEXSPI1_B_SCLK, packet->padCtrl.sclk);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs0)
            {
                case kFlexspi1_DqsA_GPIO_SD_B2_05:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_05_FLEXSPI1_A_DQS, 1U);
                    if (packet->padCtrl.dqs0 != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_05_FLEXSPI1_A_DQS, packet->padCtrl.dqs0);
                    }
                    break;
                case kFlexspi1_DqsA_GPIO_AD_17:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_17_FLEXSPI1_A_DQS, 1U);
                    if (packet->padCtrl.dqs0 != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_17_FLEXSPI1_A_DQS, packet->padCtrl.dqs0);
                    }
                    break;
                case kFlexspi1_DqsA_GPIO_EMC_B2_18:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_18_FLEXSPI1_A_DQS, 1U);
                    if (packet->padCtrl.dqs0 != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_18_FLEXSPI1_A_DQS, packet->padCtrl.dqs0);
                    }
                    break;
                case kFlexspi1_DqsB_GPIO_SD_B1_05:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_FLEXSPI1_B_DQS, 1U);
                    if (packet->padCtrl.dqs0 != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_05_FLEXSPI1_B_DQS, packet->padCtrl.dqs0);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dataHigh4bit)
            {
                case kFlexspi1_DataB_GPIO_SD_B2_03_00:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_00_FLEXSPI1_B_DATA03, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_01_FLEXSPI1_B_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_02_FLEXSPI1_B_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_03_FLEXSPI1_B_DATA00, 1U);
                    if (packet->padCtrl.dataHigh4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_00_FLEXSPI1_B_DATA03, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_01_FLEXSPI1_B_DATA02, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_02_FLEXSPI1_B_DATA01, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_03_FLEXSPI1_B_DATA00, packet->padCtrl.dataHigh4bit);
                    }
                    break;
                case kFlexspi1_DataB_GPIO_AD_15_12:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_12_FLEXSPI1_B_DATA03, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_13_FLEXSPI1_B_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_14_FLEXSPI1_B_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_15_FLEXSPI1_B_DATA00, 1U);
                    if (packet->padCtrl.dataHigh4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_12_FLEXSPI1_B_DATA03, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_13_FLEXSPI1_B_DATA02, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_14_FLEXSPI1_B_DATA01, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_15_FLEXSPI1_B_DATA00, packet->padCtrl.dataHigh4bit);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.sclk_n)
            {
                case kFlexspi1_SclkB_GPIO_SD_B2_04:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_04_FLEXSPI1_B_SCLK, 1U);
                    if (packet->padCtrl.sclk_n != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_04_FLEXSPI1_B_SCLK, packet->padCtrl.sclk_n);
                    }
                    break;
                case kFlexspi1_SclkB_GPIO_AD_16:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_16_FLEXSPI1_B_SCLK, 1U);
                    if (packet->padCtrl.sclk_n != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_16_FLEXSPI1_B_SCLK, packet->padCtrl.sclk_n);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs1)
            {
                case kFlexspi1_DqsB_GPIO_SD_B1_05:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_FLEXSPI1_B_DQS, 1U);
                    if (packet->padCtrl.dqs1 != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_05_FLEXSPI1_B_DQS, packet->padCtrl.dqs1);
                    }
                    break;
                default:
                    break;
            }
        }
        else if (packet->memConnection.instance == 2)
        {
            switch (packet->memConnection.dataLow4bit)
            {
                case kFlexspi2_DataA_GPIO_EMC_B2_16_13:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_16_FLEXSPI2_A_DATA03, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_15_FLEXSPI2_A_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_14_FLEXSPI2_A_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_13_FLEXSPI2_A_DATA00, 1U);
                    if (packet->padCtrl.dataLow4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_16_FLEXSPI2_A_DATA03, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_15_FLEXSPI2_A_DATA02, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_14_FLEXSPI2_A_DATA01, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_13_FLEXSPI2_A_DATA00, packet->padCtrl.dataLow4bit);
                    }
                    break;
                case kFlexspi2_DataA_GPIO_SD_B1_05_02:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_FLEXSPI2_A_DATA03, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_FLEXSPI2_A_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_FLEXSPI2_A_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_FLEXSPI2_A_DATA00, 1U);
                    if (packet->padCtrl.dataLow4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_05_FLEXSPI2_A_DATA03, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_FLEXSPI2_A_DATA02, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_03_FLEXSPI2_A_DATA01, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_02_FLEXSPI2_A_DATA00, packet->padCtrl.dataLow4bit);
                    }
                    break;
                case kFlexspi2_DataB_GPIO_EMC_B2_06_03:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_06_FLEXSPI2_B_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_05_FLEXSPI2_B_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_04_FLEXSPI2_B_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_03_FLEXSPI2_B_DATA03, 1U);
                    if (packet->padCtrl.dataLow4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_06_FLEXSPI2_B_DATA00, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_05_FLEXSPI2_B_DATA01, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_04_FLEXSPI2_B_DATA02, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_03_FLEXSPI2_B_DATA03, packet->padCtrl.dataLow4bit);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.ss_b)
            {
                case kFlexspi2_Ss0A_GPIO_EMC_B2_11:
                    MTU_USE_FSPI2_PORTA1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_11_FLEXSPI2_A_SS0_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_11_FLEXSPI2_A_SS0_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi2_Ss0A_GPIO_SD_B1_00:
                    MTU_USE_FSPI2_PORTA1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_00_FLEXSPI2_A_SS0_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_00_FLEXSPI2_A_SS0_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi2_Ss1A_GPIO_AD_01:
                    MTU_USE_FSPI2_PORTA2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_01_FLEXSPI2_A_SS1_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_01_FLEXSPI2_A_SS1_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi2_Ss0B_GPIO_EMC_B2_08:
                    MTU_USE_FSPI2_PORTB1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_08_FLEXSPI2_B_SS0_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_08_FLEXSPI2_B_SS0_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi2_Ss1B_GPIO_AD_00:
                    MTU_USE_FSPI2_PORTB2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_00_FLEXSPI2_B_SS1_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_00_FLEXSPI2_B_SS1_B, packet->padCtrl.ss_b);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.sclk)
            {
                case kFlexspi2_SclkA_GPIO_EMC_B2_10:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_10_FLEXSPI2_A_SCLK, 1U);
                    if (packet->padCtrl.sclk != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_10_FLEXSPI2_A_SCLK, packet->padCtrl.sclk);
                    }
                    break;
                case kFlexspi2_SclkA_GPIO_SD_B1_01:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_01_FLEXSPI2_A_SCLK, 1U);
                    if (packet->padCtrl.sclk != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_01_FLEXSPI2_A_SCLK, packet->padCtrl.sclk);
                    }
                    break;
                case kFlexspi2_SclkB_GPIO_EMC_B2_09:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_09_FLEXSPI2_B_SCLK, 1U);
                    if (packet->padCtrl.sclk != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_09_FLEXSPI2_B_SCLK, packet->padCtrl.sclk);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs0)
            {
                case kFlexspi2_DqsA_GPIO_EMC_B2_12:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_12_FLEXSPI2_A_DQS, 1U);
                    if (packet->padCtrl.dqs0 != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_12_FLEXSPI2_A_DQS, packet->padCtrl.dqs0);
                    }
                    break;
                case kFlexspi2_DqsB_GPIO_EMC_B2_07:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_07_FLEXSPI2_B_DQS, 1U);
                    if (packet->padCtrl.dqs0 != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_07_FLEXSPI2_B_DQS, packet->padCtrl.dqs0);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dataHigh4bit)
            {
                case kFlexspi2_DataA_GPIO_EMC_B2_20_17:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_20_FLEXSPI2_A_DATA07, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_19_FLEXSPI2_A_DATA06, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_18_FLEXSPI2_A_DATA05, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_17_FLEXSPI2_A_DATA04, 1U);
                    if (packet->padCtrl.dataHigh4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_20_FLEXSPI2_A_DATA07, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_19_FLEXSPI2_A_DATA06, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_18_FLEXSPI2_A_DATA05, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_17_FLEXSPI2_A_DATA04, packet->padCtrl.dataHigh4bit);
                    }
                    break;
                case kFlexspi2_DataB_GPIO_EMC_B1_41_B2_02_00:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_41_FLEXSPI2_B_DATA07, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_00_FLEXSPI2_B_DATA06, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_01_FLEXSPI2_B_DATA05, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_02_FLEXSPI2_B_DATA04, 1U);
                    if (packet->padCtrl.dataHigh4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_41_FLEXSPI2_B_DATA07, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_00_FLEXSPI2_B_DATA06, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_01_FLEXSPI2_B_DATA05, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_02_FLEXSPI2_B_DATA04, packet->padCtrl.dataHigh4bit);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.sclk_n)
            {
                case kFlexspi2_SclkB_GPIO_EMC_B2_09:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_09_FLEXSPI2_B_SCLK, 1U);
                    if (packet->padCtrl.sclk_n != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_09_FLEXSPI2_B_SCLK, packet->padCtrl.sclk_n);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs1)
            {
                case kFlexspi2_DqsB_GPIO_EMC_B2_07:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_07_FLEXSPI2_B_DQS, 1U);
                    if (packet->padCtrl.dqs1 != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B2_07_FLEXSPI2_B_DQS, packet->padCtrl.sclk_n);
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void bsp_mixspi_gpios_toggle(void)
{
    if (s_pinUnittestPacket.pintestEn.enableAdcSample)
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

void bsp_mixspi_clock_init(void *config)
{
    clock_root_config_t rootCfg = {0};
    mixspi_user_config_t *userConfig = (mixspi_user_config_t *)config;
    mixspi_root_clk_freq_t clkFreq = userConfig->mixspiRootClkFreq;
    uint8_t kCLOCK_FLEXSPIx_ClockRoot_MuxOscRc48MDiv2;
    uint8_t kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll3Pfd0;
    uint8_t kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll2Pfd2;
    clock_root_t clockRoot;
    if (userConfig->mixspiBase == FLEXSPI1)
    {
        userConfig->instance = 1;
        clockRoot = kCLOCK_Root_Flexspi1;
        kCLOCK_FLEXSPIx_ClockRoot_MuxOscRc48MDiv2 = kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc48MDiv2;
        kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll3Pfd0  = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
        kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll2Pfd2  = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd2;
    }
    else if (userConfig->mixspiBase == FLEXSPI2)
    {
        userConfig->instance = 2;
        clockRoot = kCLOCK_Root_Flexspi2;
        kCLOCK_FLEXSPIx_ClockRoot_MuxOscRc48MDiv2 = kCLOCK_FLEXSPI2_ClockRoot_MuxOscRc48MDiv2;
        kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll3Pfd0  = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd0;
        kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll2Pfd2  = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll2Pfd2;
    }
    else
    {
    }
    // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 12-35.
    if (clkFreq == kMixspiRootClkFreq_30MHz)
    {
        rootCfg.mux = kCLOCK_FLEXSPIx_ClockRoot_MuxOscRc48MDiv2;
        rootCfg.div = 1;
        CLOCK_SetRootClock(clockRoot, &rootCfg);
    }
    else if (clkFreq == kMixspiRootClkFreq_50MHz)
    {
        /* Init System Pll2 (528MHz) pfd2. */
        // 528*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        // CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd2, 24);
        rootCfg.mux = kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll2Pfd2;
        rootCfg.div = 8;
        CLOCK_SetRootClock(clockRoot, &rootCfg);
    }
    else if (clkFreq == kMixspiRootClkFreq_60MHz)
    {
        /* Init System Pll3 (480MHz) pfd0. */
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 18);
        rootCfg.mux = kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll3Pfd0;
        rootCfg.div = 8;
        CLOCK_SetRootClock(clockRoot, &rootCfg);
    }
    else if (clkFreq == kMixspiRootClkFreq_80MHz)
    {
        /* Init System Pll3 (480MHz) pfd0. */
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 18);
        rootCfg.mux = kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll3Pfd0;
        rootCfg.div = 6;
        CLOCK_SetRootClock(clockRoot, &rootCfg);
    }
    else if (clkFreq == kMixspiRootClkFreq_100MHz)
    {
        /* Init System Pll2 (528MHz) pfd2. */
        // 528*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        // CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd2, 24);
        rootCfg.mux = kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll2Pfd2;
        rootCfg.div = 4;
        CLOCK_SetRootClock(clockRoot, &rootCfg);
    }
    else if (clkFreq == kMixspiRootClkFreq_120MHz)
    {
        /* Init System Pll3 (480MHz) pfd0. */
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 18);
        rootCfg.mux = kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll3Pfd0;
        rootCfg.div = 4;
        CLOCK_SetRootClock(clockRoot, &rootCfg);
    }
    else if (clkFreq == kMixspiRootClkFreq_133MHz)
    {
        /* Init System Pll3 (480MHz) pfd0. */
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        //CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 13);
        rootCfg.mux = kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll3Pfd0;
        rootCfg.div = 5;
        CLOCK_SetRootClock(clockRoot, &rootCfg);
    }
    else if (clkFreq == kMixspiRootClkFreq_166MHz)
    {
        /* Init System Pll3 (480MHz) pfd0. */
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        //CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 13);
        rootCfg.mux = kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll3Pfd0;
        rootCfg.div = 4;
        CLOCK_SetRootClock(clockRoot, &rootCfg);
    }
    else if (clkFreq == kMixspiRootClkFreq_200MHz)
    {
        /* Init System Pll3 (480MHz) pfd0. */
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 22);
        rootCfg.mux = kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll3Pfd0;
        rootCfg.div = 2;
        CLOCK_SetRootClock(clockRoot, &rootCfg);
    }
    else if (clkFreq == kMixspiRootClkFreq_240MHz)
    {
        /* Init System Pll3 (480MHz) pfd0. */
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 18);
        rootCfg.mux = kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll3Pfd0;
        rootCfg.div = 2;
        CLOCK_SetRootClock(clockRoot, &rootCfg);
    }
    else if (clkFreq == kMixspiRootClkFreq_266MHz)
    {
        /* Init System Pll2 (528MHz) pfd2. */
        // 528*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd2, 18);
        rootCfg.mux = kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll2Pfd2;
        rootCfg.div = 2;
        CLOCK_SetRootClock(clockRoot, &rootCfg);
    }
    else if (clkFreq == kMixspiRootClkFreq_332MHz)
    {
        /* Init System Pll3 (480MHz) pfd0. */
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        //CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 13);
        rootCfg.mux = kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll3Pfd0;
        rootCfg.div = 2;
        CLOCK_SetRootClock(clockRoot, &rootCfg);
    }
    else if (clkFreq == kMixspiRootClkFreq_400MHz)
    {
        /* Init System Pll3 (480MHz) pfd0. */
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 22);
        rootCfg.mux = kCLOCK_FLEXSPIx_ClockRoot_MuxSysPll3Pfd0;
        rootCfg.div = 1;
        CLOCK_SetRootClock(clockRoot, &rootCfg);
    }
    else
    {
        printf("FLEXSPI1 clock freq is not set.\r\n");
    }

    uint32_t flexspiClk = CLOCK_GetRootClockFreq(clockRoot);
    if (flexspiClk > 166000000U)
    {
        __NOP();
    }
}

uint32_t bsp_mixspi_get_clock(void *config)
{
    mixspi_user_config_t *userConfig = (mixspi_user_config_t *)config;
    if (userConfig->mixspiBase == FLEXSPI1)
    {
        return CLOCK_GetRootClockFreq(kCLOCK_Root_Flexspi1);
    }
    else if (userConfig->mixspiBase == FLEXSPI2)
    {
        return CLOCK_GetRootClockFreq(kCLOCK_Root_Flexspi2);
    }
    else
    {
        return 0;
    }
}

void bsp_mixspi_clock_source(void *config)
{
    mixspi_user_config_t *userConfig = (mixspi_user_config_t *)config;
    uint32_t index = 0;
    clock_root_t root;
    uint32_t clkSel;
    if (userConfig->mixspiBase == FLEXSPI1)
    {
        root = kCLOCK_Root_Flexspi1;
        clkSel = CLOCK_GetRootClockMux(root);
        switch (clkSel)
        {
            case kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc48MDiv2:
                printf("FLEXSPI1 Clk Source from 3'b000 - OSC RC48M Div2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc48MDiv2));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxOsc24MOut:
                printf("FLEXSPI1 Clk Source from 3'b001 - OSC RC24M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_Osc24MOut));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc400M:
                printf("FLEXSPI1 Clk Source from 3'b010 - OSC RC400M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc400M));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc16M:
                printf("FLEXSPI1 Clk Source from 3'b011 - OSC RC16M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc16M));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0:
                printf("FLEXSPI1 Clk Source from 3'b100 - Sys PLL3 PFD0 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll3Pfd0));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Out:
                printf("FLEXSPI1 Clk Source from 3'b101 - Sys PLL2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll2));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd2:
                printf("FLEXSPI1 Clk Source from 3'b110 - Sys PLL2 PFD2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll2Pfd2));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Out:
                printf("FLEXSPI1 Clk Source from 3'b111 - Sys PLL3 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll3));
                break;

            default:
                break;
        }
    }
    else if (userConfig->mixspiBase == FLEXSPI2)
    {
        root = kCLOCK_Root_Flexspi2;
        clkSel = CLOCK_GetRootClockMux(root);
        switch (clkSel)
        {
            case kCLOCK_FLEXSPI2_ClockRoot_MuxOscRc48MDiv2:
                printf("FLEXSPI2 Clk Source from 3'b000 - OSC RC48M Div2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc48MDiv2));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxOsc24MOut:
                printf("FLEXSPI2 Clk Source from 3'b001 - OSC RC24M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_Osc24MOut));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxOscRc400M:
                printf("FLEXSPI2 Clk Source from 3'b010 - OSC RC400M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc400M));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxOscRc16M:
                printf("FLEXSPI2 Clk Source from 3'b011 - OSC RC16M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc16M));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd0:
                printf("FLEXSPI2 Clk Source from 3'b100 - Sys PLL3 PFD0 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll3Pfd0));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll2Out:
                printf("FLEXSPI2 Clk Source from 3'b101 - Sys PLL2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll2));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll2Pfd2:
                printf("FLEXSPI2 Clk Source from 3'b110 - Sys PLL2 PFD2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll2Pfd2));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Out:
                printf("FLEXSPI2 Clk Source from 3'b111 - Sys PLL3 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll3));
                break;

            default:
                break;
        }
    }
    else
    {
    }
    index = (root - kCLOCK_Root_Flexspi1) + 1;
    uint32_t clkDiv = CLOCK_GetRootClockDiv(root);
    printf("FLEXSPI%d Clk Source Divider: %d.\r\n", index, clkDiv);
    printf("FLEXSPI%d Clk Frequency: %dHz.\r\n", index, bsp_mixspi_get_clock(userConfig));
}

void bsp_mixspi_sw_delay_us(uint64_t us)
{
    uint32_t ticks_per_us = CLOCK_GetFreq(kCLOCK_CpuClk) / 1000000;
    while (us--)
    {
        register uint32_t ticks = 1 + ticks_per_us / 4;
        while (ticks--)
        {
            __NOP();
        }
    }
}

uint32_t bsp_mixspi_get_amba_base(void *config)
{
    mixspi_user_config_t *userConfig = (mixspi_user_config_t *)config;
    if (userConfig->mixspiBase == FLEXSPI1)
    {
        return FlexSPI1_AMBA_BASE;
    }
    else if (userConfig->mixspiBase == FLEXSPI2)
    {
        return FlexSPI2_AMBA_BASE;
    }
    else
    {
        return 0;
    }
}
