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
    kFlexspi1_DataA_GPIO_SD_B1_11_08   = 0x00,
    kFlexspi1_DataA_GPIO_AD_B1_13_10   = 0x01,

    kFlexspi1_DataB_GPIO_SD_B1_03_00   = 0x10,
    kFlexspi1_DataB_GPIO_AD_B1_07_04   = 0x11,
};

enum _flexspi1_ss_b_sel
{
    kFlexspi1_Ss0A_GPIO_SD_B1_06   = 0x00,
    kFlexspi1_Ss0A_GPIO_AD_B1_15   = 0x01,
    kFlexspi1_Ss1A_GPIO_SD_B0_00   = 0x02,
    kFlexspi1_Ss1A_GPIO_SD_B1_04   = 0x03,
    kFlexspi1_Ss1A_GPIO_AD_B1_08   = 0x04,

    kFlexspi1_Ss0B_GPIO_SD_B0_04   = 0x10,
    kFlexspi1_Ss0B_GPIO_SD_B1_05   = 0x11,
    kFlexspi1_Ss1B_GPIO_SD_B0_01   = 0x12,
};

enum _flexspi1_sclk_sel
{
    kFlexspi1_SclkA_GPIO_SD_B1_07  = 0x00,
    kFlexspi1_SclkA_GPIO_AD_B1_14  = 0x01,

    kFlexspi1_SclkB_GPIO_SD_B1_04  = 0x10,
};

enum _flexspi1_dqs_sel
{
    kFlexspi1_DqsA_GPIO_SD_B1_05   = 0x00,
    kFlexspi1_DqsA_GPIO_AD_B1_09   = 0x01,

    kFlexspi1_DqsB_GPIO_SD_B0_05   = 0x10,
};

enum _flexspi1_rst_b_sel
{
    kFlexspi1_Rst_GPIO_AD_B0_09    = 0x00,
};

/////////////////////////////////////////////////////////////

#define MTU_USE_FSPI1_PORTA1                      \
    {                                             \
        s_userConfig.mixspiBase = FLEXSPI;        \
        s_userConfig.mixspiPort = kFLEXSPI_PortA1;\
    }
#define MTU_USE_FSPI1_PORTA2                      \
    {                                             \
        s_userConfig.mixspiBase = FLEXSPI;        \
        s_userConfig.mixspiPort = kFLEXSPI_PortA2;\
    }
#define MTU_USE_FSPI1_PORTB1                      \
    {                                             \
        s_userConfig.mixspiBase = FLEXSPI;        \
        s_userConfig.mixspiPort = kFLEXSPI_PortB1;\
    }
#define MTU_USE_FSPI1_PORTB2                      \
    {                                             \
        s_userConfig.mixspiBase = FLEXSPI;        \
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
                    case kFlexspi1_DataA_GPIO_AD_B1_13_10:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_13_GPIO1_IO29, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_12_GPIO1_IO28, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_11_GPIO1_IO27, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_10_GPIO1_IO26, 0U);
                        GPIO_PinInit(GPIO1, 29, &do_config);
                        GPIO_PinInit(GPIO1, 28, &do_config);
                        GPIO_PinInit(GPIO1, 27, &do_config);
                        GPIO_PinInit(GPIO1, 26, &do_config);
                        s_pinInfo[0].gpioGroup = 1;
                        s_pinInfo[0].gpioPin = 29;
                        s_pinInfo[1].gpioGroup = 1;
                        s_pinInfo[1].gpioPin = 28;
                        s_pinInfo[2].gpioGroup = 1;
                        s_pinInfo[2].gpioPin = 27;
                        s_pinInfo[3].gpioGroup = 1;
                        s_pinInfo[3].gpioPin = 26;
                        break;
                    case kFlexspi1_DataB_GPIO_SD_B1_03_00:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_GPIO3_IO03, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_GPIO3_IO02, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_01_GPIO3_IO01, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_00_GPIO3_IO00, 0U);
                        GPIO_PinInit(GPIO3, 3, &do_config);
                        GPIO_PinInit(GPIO3, 2, &do_config);
                        GPIO_PinInit(GPIO3, 1, &do_config);
                        GPIO_PinInit(GPIO3, 0, &do_config);
                        s_pinInfo[0].gpioGroup = 3;
                        s_pinInfo[0].gpioPin = 3;
                        s_pinInfo[1].gpioGroup = 3;
                        s_pinInfo[1].gpioPin = 2;
                        s_pinInfo[2].gpioGroup = 3;
                        s_pinInfo[2].gpioPin = 1;
                        s_pinInfo[3].gpioGroup = 3;
                        s_pinInfo[3].gpioPin = 0;
                        break;
                    case kFlexspi1_DataB_GPIO_AD_B1_07_04:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_05_GPIO1_IO21, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20, 0U);
                        GPIO_PinInit(GPIO1, 23, &do_config);
                        GPIO_PinInit(GPIO1, 22, &do_config);
                        GPIO_PinInit(GPIO1, 21, &do_config);
                        GPIO_PinInit(GPIO1, 20, &do_config);
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
            if (packet->pintestEn.option.B.ss_b)
            {
                switch (packet->memConnection.ss_b)
                {
                    case kFlexspi1_Ss0A_GPIO_SD_B1_06:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_06_GPIO3_IO06, 0U);
                        GPIO_PinInit(GPIO3, 6, &do_config);
                        s_pinInfo[4].gpioGroup = 3;
                        s_pinInfo[4].gpioPin = 6;
                        break;
                    case kFlexspi1_Ss0A_GPIO_AD_B1_15:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_15_GPIO1_IO31, 0U);
                        GPIO_PinInit(GPIO1, 31, &do_config);
                        s_pinInfo[4].gpioGroup = 1;
                        s_pinInfo[4].gpioPin = 31;
                        break;
                    case kFlexspi1_Ss1A_GPIO_SD_B0_00:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_00_GPIO3_IO12, 0U);
                        GPIO_PinInit(GPIO3, 12, &do_config);
                        s_pinInfo[4].gpioGroup = 3;
                        s_pinInfo[4].gpioPin = 12;
                        break;
                    case kFlexspi1_Ss1A_GPIO_SD_B1_04:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04, 0U);
                        GPIO_PinInit(GPIO3, 4, &do_config);
                        s_pinInfo[4].gpioGroup = 3;
                        s_pinInfo[4].gpioPin = 4;
                        break;
                    case kFlexspi1_Ss1A_GPIO_AD_B1_08:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_GPIO1_IO24, 0U);
                        GPIO_PinInit(GPIO1, 24, &do_config);
                        s_pinInfo[4].gpioGroup = 1;
                        s_pinInfo[4].gpioPin = 24;
                        break;
                    case kFlexspi1_Ss0B_GPIO_SD_B0_04:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_04_GPIO3_IO16, 0U);
                        GPIO_PinInit(GPIO3, 16, &do_config);
                        s_pinInfo[4].gpioGroup = 3;
                        s_pinInfo[4].gpioPin = 16;
                        break;
                    case kFlexspi1_Ss0B_GPIO_SD_B1_05:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_GPIO3_IO05, 0U);
                        GPIO_PinInit(GPIO3, 5, &do_config);
                        s_pinInfo[4].gpioGroup = 3;
                        s_pinInfo[4].gpioPin = 5;
                        break;
                    case kFlexspi1_Ss1B_GPIO_SD_B0_01:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_01_GPIO3_IO13, 0U);
                        GPIO_PinInit(GPIO3, 13, &do_config);
                        s_pinInfo[4].gpioGroup = 3;
                        s_pinInfo[4].gpioPin = 13;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.sclk)
            {
                switch (packet->memConnection.sclk)
                {
                    case kFlexspi1_SclkA_GPIO_SD_B1_07:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_07_GPIO3_IO07, 0U);
                        GPIO_PinInit(GPIO3, 7, &do_config);
                        s_pinInfo[5].gpioGroup = 3;
                        s_pinInfo[5].gpioPin = 7;
                        break;
                    case kFlexspi1_SclkA_GPIO_AD_B1_14:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_GPIO1_IO30, 0U);
                        GPIO_PinInit(GPIO1, 30, &do_config);
                        s_pinInfo[5].gpioGroup = 1;
                        s_pinInfo[5].gpioPin = 30;
                        break;
                    case kFlexspi1_SclkB_GPIO_SD_B1_04:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04, 0U);
                        GPIO_PinInit(GPIO3, 4, &do_config);
                        s_pinInfo[5].gpioGroup = 3;
                        s_pinInfo[5].gpioPin = 4;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.dqs0)
            {
                switch (packet->memConnection.dqs0)
                {
                    case kFlexspi1_DqsA_GPIO_SD_B1_05:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_GPIO3_IO05, 0U);
                        GPIO_PinInit(GPIO3, 5, &do_config);
                        s_pinInfo[6].gpioGroup = 3;
                        s_pinInfo[6].gpioPin = 5;
                        break;
                    case kFlexspi1_DqsA_GPIO_AD_B1_09:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_GPIO1_IO25, 0U);
                        GPIO_PinInit(GPIO1, 25, &do_config);
                        s_pinInfo[6].gpioGroup = 1;
                        s_pinInfo[6].gpioPin = 25;
                        break;
                    case kFlexspi1_DqsB_GPIO_SD_B0_05:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_05_GPIO3_IO17, 0U);
                        GPIO_PinInit(GPIO3, 17, &do_config);
                        s_pinInfo[6].gpioGroup = 3;
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
                    case kFlexspi1_DataB_GPIO_SD_B1_03_00:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_GPIO3_IO03, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_GPIO3_IO02, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_01_GPIO3_IO01, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_00_GPIO3_IO00, 0U);
                        GPIO_PinInit(GPIO3, 3, &do_config);
                        GPIO_PinInit(GPIO3, 2, &do_config);
                        GPIO_PinInit(GPIO3, 1, &do_config);
                        GPIO_PinInit(GPIO3, 0, &do_config);
                        s_pinInfo[7].gpioGroup = 3;
                        s_pinInfo[7].gpioPin = 3;
                        s_pinInfo[8].gpioGroup = 3;
                        s_pinInfo[8].gpioPin = 2;
                        s_pinInfo[9].gpioGroup = 3;
                        s_pinInfo[9].gpioPin = 1;
                        s_pinInfo[10].gpioGroup = 3;
                        s_pinInfo[10].gpioPin = 0;
                        break;
                    case kFlexspi1_DataB_GPIO_AD_B1_07_04:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_05_GPIO1_IO21, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20, 0U);
                        GPIO_PinInit(GPIO1, 23, &do_config);
                        GPIO_PinInit(GPIO1, 22, &do_config);
                        GPIO_PinInit(GPIO1, 21, &do_config);
                        GPIO_PinInit(GPIO1, 20, &do_config);
                        s_pinInfo[7].gpioGroup = 1;
                        s_pinInfo[7].gpioPin = 23;
                        s_pinInfo[8].gpioGroup = 1;
                        s_pinInfo[8].gpioPin = 22;
                        s_pinInfo[9].gpioGroup = 1;
                        s_pinInfo[9].gpioPin = 21;
                        s_pinInfo[10].gpioGroup = 1;
                        s_pinInfo[10].gpioPin = 20;
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
                    case kFlexspi1_SclkB_GPIO_SD_B1_04:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04, 0U);
                        GPIO_PinInit(GPIO3, 4, &do_config);
                        s_pinInfo[19].gpioGroup = 3;
                        s_pinInfo[19].gpioPin = 4;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.dqs1)
            {
                switch (packet->memConnection.dqs1)
                {
                    case kFlexspi1_DqsB_GPIO_SD_B0_05:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_05_GPIO3_IO17, 0U);
                        GPIO_PinInit(GPIO3, 17, &do_config);
                        s_pinInfo[20].gpioGroup = 3;
                        s_pinInfo[20].gpioPin = 17;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.rst_b)
            {
                switch (packet->memConnection.rst_b)
                {
                    case kFlexspi1_Rst_GPIO_AD_B0_09:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 0U);
                        //IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 0x10B0U); 
                        GPIO_PinInit(GPIO1, 9, &do_config);
                        s_pinInfo[21].gpioGroup = 1;
                        s_pinInfo[21].gpioPin = 9;
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
                    if (packet->padCtrl.dataLow4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00, packet->padCtrl.dataLow4bit);
                    }
                    break;
                case kFlexspi1_DataA_GPIO_AD_B1_13_10:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_13_FLEXSPIA_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_12_FLEXSPIA_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_11_FLEXSPIA_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_10_FLEXSPIA_DATA03, 1U);
                    if (packet->padCtrl.dataLow4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_13_FLEXSPIA_DATA00, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_12_FLEXSPIA_DATA01, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_11_FLEXSPIA_DATA02, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_10_FLEXSPIA_DATA03, packet->padCtrl.dataLow4bit);
                    }
                    break;
                case kFlexspi1_DataB_GPIO_SD_B1_03_00:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_FLEXSPIB_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_FLEXSPIB_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_01_FLEXSPIB_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_00_FLEXSPIB_DATA03, 1U);
                    if (packet->padCtrl.dataLow4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_03_FLEXSPIB_DATA00, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_02_FLEXSPIB_DATA01, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_01_FLEXSPIB_DATA02, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_00_FLEXSPIB_DATA03, packet->padCtrl.dataLow4bit);
                    }
                    break;
                case kFlexspi1_DataB_GPIO_AD_B1_07_04:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_FLEXSPIB_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_FLEXSPIB_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_05_FLEXSPIB_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_FLEXSPIB_DATA03, 1U);
                    if (packet->padCtrl.dataLow4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_07_FLEXSPIB_DATA00, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_06_FLEXSPIB_DATA01, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_05_FLEXSPIB_DATA02, packet->padCtrl.dataLow4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_04_FLEXSPIB_DATA03, packet->padCtrl.dataLow4bit);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.ss_b)
            {
                case kFlexspi1_Ss0A_GPIO_SD_B1_06:
                    MTU_USE_FSPI1_PORTA1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi1_Ss0A_GPIO_AD_B1_15:
                    MTU_USE_FSPI1_PORTA1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_15_FLEXSPIA_SS0_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_15_FLEXSPIA_SS0_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi1_Ss1A_GPIO_SD_B0_00:
                    MTU_USE_FSPI1_PORTA2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_00_FLEXSPIA_SS1_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_FLEXSPIA_SS1_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi1_Ss1A_GPIO_SD_B1_04:
                    MTU_USE_FSPI1_PORTA2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_FLEXSPIA_SS1_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_FLEXSPIA_SS1_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi1_Ss1A_GPIO_AD_B1_08:
                    MTU_USE_FSPI1_PORTA2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_FLEXSPIA_SS1_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_08_FLEXSPIA_SS1_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi1_Ss0B_GPIO_SD_B0_04:
                    MTU_USE_FSPI1_PORTB1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_04_FLEXSPIB_SS0_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_04_FLEXSPIB_SS0_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi1_Ss0B_GPIO_SD_B1_05:
                    MTU_USE_FSPI1_PORTB1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_FLEXSPIB_SS0_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_05_FLEXSPIB_SS0_B, packet->padCtrl.ss_b);
                    }
                    break;
                case kFlexspi1_Ss1B_GPIO_SD_B0_01:
                    MTU_USE_FSPI1_PORTB2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_01_FLEXSPIB_SS1_B, 1U);
                    if (packet->padCtrl.ss_b != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_FLEXSPIB_SS1_B, packet->padCtrl.ss_b);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.sclk)
            {
                case kFlexspi1_SclkA_GPIO_SD_B1_07:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK, 1U);
                    if (packet->padCtrl.sclk != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK, packet->padCtrl.sclk);
                    }
                    break;
                case kFlexspi1_SclkA_GPIO_AD_B1_14:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_FLEXSPIA_SCLK, 1U);
                    if (packet->padCtrl.sclk != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_14_FLEXSPIA_SCLK, packet->padCtrl.sclk);
                    }
                    break;
                case kFlexspi1_SclkB_GPIO_SD_B1_04:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_FLEXSPIB_SCLK, 1U);
                    if (packet->padCtrl.sclk != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_FLEXSPIB_SCLK, packet->padCtrl.sclk);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs0)
            {
                case kFlexspi1_DqsA_GPIO_SD_B1_05:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS, 1U);
                    if (packet->padCtrl.dqs0 != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS, packet->padCtrl.dqs0);
                    }
                    break;
                case kFlexspi1_DqsA_GPIO_AD_B1_09:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_FLEXSPIA_DQS, 1U);
                    if (packet->padCtrl.dqs0 != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_09_FLEXSPIA_DQS, packet->padCtrl.dqs0);
                    }
                    break;
                case kFlexspi1_DqsB_GPIO_SD_B0_05:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_05_FLEXSPIB_DQS, 1U);
                    if (packet->padCtrl.dqs0 != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_05_FLEXSPIB_DQS, packet->padCtrl.dqs0);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dataHigh4bit)
            {
                case kFlexspi1_DataB_GPIO_SD_B1_03_00:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_FLEXSPIB_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_FLEXSPIB_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_01_FLEXSPIB_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_00_FLEXSPIB_DATA03, 1U);
                    if (packet->padCtrl.dataHigh4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_03_FLEXSPIB_DATA00, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_02_FLEXSPIB_DATA01, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_01_FLEXSPIB_DATA02, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_00_FLEXSPIB_DATA03, packet->padCtrl.dataHigh4bit);
                    }
                    break;
                case kFlexspi1_DataB_GPIO_AD_B1_07_04:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_FLEXSPIB_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_FLEXSPIB_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_05_FLEXSPIB_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_FLEXSPIB_DATA03, 1U);
                    if (packet->padCtrl.dataHigh4bit != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_07_FLEXSPIB_DATA00, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_06_FLEXSPIB_DATA01, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_05_FLEXSPIB_DATA02, packet->padCtrl.dataHigh4bit);
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_04_FLEXSPIB_DATA03, packet->padCtrl.dataHigh4bit);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.sclk_n)
            {
                case kFlexspi1_SclkB_GPIO_SD_B1_04:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_FLEXSPIB_SCLK, 1U);
                    if (packet->padCtrl.sclk_n != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_FLEXSPIB_SCLK, packet->padCtrl.sclk_n);
                    }
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs1)
            {
                case kFlexspi1_DqsB_GPIO_SD_B0_05:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_05_FLEXSPIB_DQS, 1U);
                    if (packet->padCtrl.dqs1 != DEFAULT_PAD_CTRL_MAGIC)
                    {
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_05_FLEXSPIB_DQS, packet->padCtrl.dqs1);
                    }
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
    mixspi_user_config_t *userConfig = (mixspi_user_config_t *)config;
    mixspi_root_clk_freq_t clkFreq = userConfig->mixspiRootClkFreq;
    if (userConfig->mixspiBase == FLEXSPI)
    {
        userConfig->instance = 1;
        const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};
        CLOCK_InitUsb1Pll(&g_ccmConfigUsbPll);
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 12-35.
        if (clkFreq == kMixspiRootClkFreq_30MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 30);   /* Set PLL3 PFD0 clock 288MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 7);   /* flexspi clock 36M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_50MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 22);   /* Set PLL3 PFD0 clock 392.7MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 7);   /* flexspi clock 49.1M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_60MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 18);   /* Set PLL3 PFD0 clock 480MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 7);   /* flexspi clock 60M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_80MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 18);   /* Set PLL3 PFD0 clock 480MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 5);   /* flexspi clock 80M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_100MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 13);   /* Set PLL3 PFD0 clock 664.6MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 6);   /* flexspi clock 94.9M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_120MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 18);   /* Set PLL3 PFD0 clock 480MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 3);   /* flexspi clock 120M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_133MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 33);   /* Set PLL3 PFD0 clock 261.8MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 1);   /* flexspi clock 130.9M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_166MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 26);   /* Set PLL3 PFD0 clock 332.3MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 1);   /* flexspi clock 166.1M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_200MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 22);   /* Set PLL3 PFD0 clock 392.7MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 1);   /* flexspi clock 196.3M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_240MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 18);   /* Set PLL3 PFD0 clock 480MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 1);   /* flexspi clock 240M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_266MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 33);   /* Set PLL3 PFD0 clock 261.8MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 0);   /* flexspi clock 261.8M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_332MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 26);   /* Set PLL3 PFD0 clock 332.3MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 0);   /* flexspi clock 332.3M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_400MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 22);   /* Set PLL3 PFD0 clock 392.7MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 0);   /* flexspi clock 392.7M. */
        }
        else
        {
            printf("FLEXSPI1 clock freq is not set.\r\n");
        }
    }
    else if (userConfig->mixspiBase == FLEXSPI2)
    {
        userConfig->instance = 2;
        const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};
        CLOCK_InitUsb1Pll(&g_ccmConfigUsbPll);
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 12-35.
        if (clkFreq == kMixspiRootClkFreq_30MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 30);   /* Set PLL3 PFD0 clock 288MHZ. */
            CLOCK_SetMux(kCLOCK_Flexspi2Mux, 0x1); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_Flexspi2Div, 7);   /* flexspi clock 36M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_50MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 22);   /* Set PLL3 PFD0 clock 392.7MHZ. */
            CLOCK_SetMux(kCLOCK_Flexspi2Mux, 0x1); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_Flexspi2Div, 7);   /* flexspi clock 49.1M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_60MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 18);   /* Set PLL3 PFD0 clock 480MHZ. */
            CLOCK_SetMux(kCLOCK_Flexspi2Mux, 0x1); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_Flexspi2Div, 7);   /* flexspi clock 60M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_80MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 18);   /* Set PLL3 PFD0 clock 480MHZ. */
            CLOCK_SetMux(kCLOCK_Flexspi2Mux, 0x1); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_Flexspi2Div, 5);   /* flexspi clock 80M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_100MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 13);   /* Set PLL3 PFD0 clock 664.6MHZ. */
            CLOCK_SetMux(kCLOCK_Flexspi2Mux, 0x1); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_Flexspi2Div, 6);   /* flexspi clock 94.9M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_120MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 18);   /* Set PLL3 PFD0 clock 480MHZ. */
            CLOCK_SetMux(kCLOCK_Flexspi2Mux, 0x1); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_Flexspi2Div, 3);   /* flexspi clock 120M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_133MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 33);   /* Set PLL3 PFD0 clock 261.8MHZ. */
            CLOCK_SetMux(kCLOCK_Flexspi2Mux, 0x1); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_Flexspi2Div, 1);   /* flexspi clock 130.9M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_166MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 26);   /* Set PLL3 PFD0 clock 332.3MHZ. */
            CLOCK_SetMux(kCLOCK_Flexspi2Mux, 0x1); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_Flexspi2Div, 1);   /* flexspi clock 166.1M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_200MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 22);   /* Set PLL3 PFD0 clock 392.7MHZ. */
            CLOCK_SetMux(kCLOCK_Flexspi2Mux, 0x1); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_Flexspi2Div, 1);   /* flexspi clock 196.3M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_240MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 18);   /* Set PLL3 PFD0 clock 480MHZ. */
            CLOCK_SetMux(kCLOCK_Flexspi2Mux, 0x1); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_Flexspi2Div, 1);   /* flexspi clock 240M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_266MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 33);   /* Set PLL3 PFD0 clock 261.8MHZ. */
            CLOCK_SetMux(kCLOCK_Flexspi2Mux, 0x1); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_Flexspi2Div, 0);   /* flexspi clock 261.8M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_332MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 26);   /* Set PLL3 PFD0 clock 332.3MHZ. */
            CLOCK_SetMux(kCLOCK_Flexspi2Mux, 0x1); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_Flexspi2Div, 0);   /* flexspi clock 332.3M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_400MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 22);   /* Set PLL3 PFD0 clock 392.7MHZ. */
            CLOCK_SetMux(kCLOCK_Flexspi2Mux, 0x1); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_Flexspi2Div, 0);   /* flexspi clock 392.7M. */
        }
        else
        {
            printf("FLEXSPI2 clock freq is not set.\r\n");
        }
    }
    else
    {
    }
}

uint32_t bsp_mixspi_get_clock(void *config)
{
    mixspi_user_config_t *userConfig = (mixspi_user_config_t *)config;
    if (userConfig->mixspiBase == FLEXSPI)
    {
        return CLOCK_GetClockRootFreq(kCLOCK_FlexspiClkRoot);
    }
    else if (userConfig->mixspiBase == FLEXSPI2)
    {
        return CLOCK_GetClockRootFreq(kCLOCK_Flexspi2ClkRoot);
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
    uint32_t clkSel;
    uint32_t clkDiv;
    if (userConfig->mixspiBase == FLEXSPI)
    {
        index = 1;
        clkSel = CCM->CSCMR1 & CCM_CSCMR1_FLEXSPI_CLK_SEL_MASK;
        clkDiv = (CCM->CSCMR1 & CCM_CSCMR1_FLEXSPI_PODF_MASK) >> CCM_CSCMR1_FLEXSPI_PODF_SHIFT;
        switch (clkSel)
        {
            case CCM_CSCMR1_FLEXSPI_CLK_SEL(0):
                printf("FLEXSPI1 Clk Source from 2'b00 - semc_clk_root_pre clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
                break;

            case CCM_CSCMR1_FLEXSPI_CLK_SEL(1):
                printf("FLEXSPI1 Clk Source from 2'b01 - PLL3(USB1 PLL) sw_clk %dHz.\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllClk));
                break;

            case CCM_CSCMR1_FLEXSPI_CLK_SEL(2):
                printf("FLEXSPI1 Clk Source from 2'b10 - PLL2(System PLL) PFD2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
                break;

            case CCM_CSCMR1_FLEXSPI_CLK_SEL(3):
                printf("FLEXSPI1 Clk Source from 2'b11 - PLL3(USB1 PLL) PFD0 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk));
                break;

            default:
                break;
        }
    }
    else if (userConfig->mixspiBase == FLEXSPI2)
    {
        index = 2;
        clkSel = CCM->CBCMR & CCM_CBCMR_FLEXSPI2_CLK_SEL_MASK;
        clkDiv = (CCM->CBCMR & CCM_CBCMR_FLEXSPI2_PODF_MASK) >> CCM_CBCMR_FLEXSPI2_PODF_SHIFT;
        switch (clkSel)
        {
            case CCM_CBCMR_FLEXSPI2_CLK_SEL(0):
                printf("FLEXSPI2 Clk Source from 2'b00 - PLL2(System PLL) PFD2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
                break;

            case CCM_CBCMR_FLEXSPI2_CLK_SEL(1):
                printf("FLEXSPI2 Clk Source from 2'b01 - PLL3(USB1 PLL) PFD0 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk));
                break;

            case CCM_CBCMR_FLEXSPI2_CLK_SEL(2):
                printf("FLEXSPI2 Clk Source from 2'b10 - PLL3(USB1 PLL) PFD1 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk));
                break;

            case CCM_CBCMR_FLEXSPI2_CLK_SEL(3):
                printf("FLEXSPI2 Clk Source from 2'b11 - PLL2(System PLL) main_clk %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
                break;

            default:
                break;
        }
    }
    else
    {
    }
    printf("FLEXSPI%d Clk Source Divider: %d.\r\n", index, clkDiv+1U);
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
    if (userConfig->mixspiBase == FLEXSPI)
    {
        return FlexSPI_AMBA_BASE;
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

