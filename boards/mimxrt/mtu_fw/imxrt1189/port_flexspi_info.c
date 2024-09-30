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
#include "fsl_rgpio.h"
#include "clock_config.h"
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
    kFlexspi1_Ss1A_GPIO_B2_01      = 0x01,
    kFlexspi1_Ss1A_GPIO_SD_B1_04   = 0x02,
    kFlexspi1_Ss1A_GPIO_SD_B1_02   = 0x03,

    kFlexspi1_Ss0B_GPIO_SD_B2_06   = 0x10,
    kFlexspi1_Ss0B_GPIO_SD_B1_05   = 0x11,
    kFlexspi1_Ss0B_GPIO_SD_B1_04   = 0x12,
    kFlexspi1_Ss0B_GPIO_B1_04      = 0x13,
    kFlexspi1_Ss1B_GPIO_SD_B2_04   = 0x14,
    kFlexspi1_Ss1B_GPIO_SD_B1_03   = 0x15,
    kFlexspi1_Ss1B_GPIO_B1_02      = 0x16,
};

enum _flexspi1_sclk_sel
{
    kFlexspi1_SclkA_GPIO_B2_08     = 0x00,

    kFlexspi1_SclkB_GPIO_SD_B2_07  = 0x10,
    kFlexspi1_SclkB_GPIO_B1_05     = 0x11,
    kFlexspi1_SclkB_GPIO_B2_02     = 0x12,
};

enum _flexspi1_dqs_sel
{
    kFlexspi1_DqsA_GPIO_B2_07      = 0x00,
    kFlexspi1_DqsA_GPIO_SD_B2_12   = 0x01,

    kFlexspi1_DqsB_GPIO_SD_B2_05   = 0x10,
    kFlexspi1_DqsB_GPIO_SD_B2_12   = 0x11,
    kFlexspi1_DqsB_GPIO_B1_03      = 0x12,
};

/////////////////////////////////////////////////////////////
enum _flexspi2_data_line_sel
{
    kFlexspi2_DataA_GPIO_EMC_B1_38_35       = 0x00,
    kFlexspi2_DataA_GPIO_AON_27_24          = 0x01,

    kFlexspi2_DataB_GPIO_EMC_B1_33_30       = 0x10,
    kFlexspi2_DataB_GPIO_AON_18_15          = 0x11,
    kFlexspi2_DataB_GPIO_EMC_B1_25_22       = 0x12,
};

enum _flexspi2_ss_b_sel
{
    kFlexspi2_Ss0A_GPIO_EMC_B1_39   = 0x00,
    kFlexspi2_Ss0A_GPIO_AON_22      = 0x01,
    kFlexspi2_Ss1A_GPIO_EMC_B1_26   = 0x02,
    kFlexspi2_Ss1A_GPIO_AON_20      = 0x03,
    kFlexspi2_Ss1A_GPIO_AON_19      = 0x04,

    kFlexspi2_Ss0B_GPIO_EMC_B1_28   = 0x10,
    kFlexspi2_Ss0B_GPIO_AON_21      = 0x11,
    kFlexspi2_Ss1B_GPIO_EMC_B1_27   = 0x12,
};

enum _flexspi2_sclk_sel
{
    kFlexspi2_SclkA_GPIO_EMC_B1_41  = 0x00,
    kFlexspi2_SclkA_GPIO_AON_23     = 0x01,

    kFlexspi2_SclkB_GPIO_EMC_B1_34  = 0x10,
    kFlexspi2_SclkB_GPIO_AON_19     = 0x11,
};

enum _flexspi2_dqs_sel
{
    kFlexspi2_DqsA_GPIO_EMC_B1_40   = 0x00,
    kFlexspi2_DqsA_GPIO_AON_21      = 0x01,
    kFlexspi2_DqsA_GPIO_AON_28      = 0x02,

    kFlexspi2_DqsB_GPIO_EMC_B1_29   = 0x10,
    kFlexspi2_DqsB_GPIO_AON_20      = 0x11,
    kFlexspi2_DqsB_GPIO_AON_28      = 0x12,
    kFlexspi2_DqsB_GPIO_EMC_B1_21   = 0x13,
};

#if MTU_USE_MEM_DC_MUX
#define MTU_USE_MEM_DC_MUX1_FSPI1_PORTA           \
    {                                             \
        RGPIO_PinWrite(RGPIO4, 23, 0);            \
    }

#define MTU_USE_MEM_DC_MUX2_FSPI1_PORTA           \
    {                                             \
        RGPIO_PinWrite(RGPIO4, 23, 0);            \
    }

#define MTU_USE_MEM_DC_MUX1_FSPI1_PORTB           \
    {                                             \
        RGPIO_PinWrite(RGPIO4, 23, 0);            \
    }

#define MTU_USE_MEM_DC_MUX2_FSPI1_PORTB           \
    {                                             \
        RGPIO_PinWrite(RGPIO4, 23, 1);            \
    }

#define MTU_USE_MEM_DC_MUX1_FSPI2_PORTA           \
    {                                             \
        RGPIO_PinWrite(RGPIO4, 22, 0);            \
    }

#define MTU_USE_MEM_DC_MUX2_FSPI2_PORTA           \
    {                                             \
        RGPIO_PinWrite(RGPIO4, 22, 1);            \
    }

#define MTU_USE_MEM_DC_MUX1_FSPI2_PORTB           \
    {                                             \
        RGPIO_PinWrite(RGPIO4, 22, 0);            \
    }

#define MTU_USE_MEM_DC_MUX2_FSPI2_PORTB           \
    {                                             \
        RGPIO_PinWrite(RGPIO4, 22, 1);            \
    }

#define MTU_USE_MEM_DC_MUX3_FSPI2_PORTB           \
    {                                             \
        RGPIO_PinWrite(RGPIO4, 22, 1);            \
    }
#else
#define MTU_USE_MEM_DC_MUX1_FSPI1_PORTA  {}
#define MTU_USE_MEM_DC_MUX2_FSPI1_PORTA  {}
#define MTU_USE_MEM_DC_MUX1_FSPI1_PORTB  {}
#define MTU_USE_MEM_DC_MUX2_FSPI1_PORTB  {}
#define MTU_USE_MEM_DC_MUX1_FSPI2_PORTA  {}
#define MTU_USE_MEM_DC_MUX2_FSPI2_PORTA  {}
#define MTU_USE_MEM_DC_MUX1_FSPI2_PORTB  {}
#define MTU_USE_MEM_DC_MUX2_FSPI2_PORTB  {}
#define MTU_USE_MEM_DC_MUX3_FSPI2_PORTB  {}
#endif

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
    CLOCK_EnableClock(kCLOCK_Iomuxc1);
    CLOCK_EnableClock(kCLOCK_Iomuxc2);

#if MTU_USE_EVK_SOCKET
    rgpio_pin_config_t do1_config = {kRGPIO_DigitalOutput, 0};
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_04_GPIO4_IO04, 0U);
    RGPIO_PinInit(RGPIO4, 4, &do1_config);
    RGPIO_PinWrite(RGPIO4, 4, 1);
#elif MTU_USE_MEM_DC_MUX
    rgpio_pin_config_t do1_config = {kRGPIO_DigitalOutput, 0};
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_23_GPIO4_IO23, 0U);
    RGPIO_PinInit(RGPIO4, 23, &do1_config);
    RGPIO_PinWrite(RGPIO4, 23, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_22_GPIO4_IO22, 0U);
    RGPIO_PinInit(RGPIO4, 22, &do1_config);
    RGPIO_PinWrite(RGPIO4, 22, 0);
#endif

    if (isPintest)
    {
        pin_unittest_packet_t *packet = (pin_unittest_packet_t *)configPacket;
        rgpio_pin_config_t do_config = {kRGPIO_DigitalOutput, 0};
        memset(s_pinInfo, 0xFF, sizeof(s_pinInfo));
        if (packet->memConnection.instance == 1)
        {
            if (packet->pintestEn.option.B.dataLow4bit)
            {
                switch (packet->memConnection.dataLow4bit)
                {
                    case kFlexspi1_DataA_GPIO_B2_13_10:
                        MTU_USE_MEM_DC_MUX1_FSPI1_PORTA
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
                        MTU_USE_MEM_DC_MUX1_FSPI1_PORTB
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
                        MTU_USE_MEM_DC_MUX2_FSPI1_PORTB
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
            if (packet->pintestEn.option.B.ss_b)
            {
                switch (packet->memConnection.ss_b)
                {
                    case kFlexspi1_Ss0A_GPIO_B2_09:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_09_GPIO6_IO23, 0U);
                        RGPIO_PinInit(RGPIO6, 23, &do_config);
                        s_pinInfo[4].gpioGroup = 6;
                        s_pinInfo[4].gpioPin = 23;
                        break;
                    case kFlexspi1_Ss1A_GPIO_B2_01:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_01_GPIO6_IO15, 0U);
                        RGPIO_PinInit(RGPIO6, 15, &do_config);
                        s_pinInfo[4].gpioGroup = 6;
                        s_pinInfo[4].gpioPin = 15;
                        break;
                    case kFlexspi1_Ss1A_GPIO_SD_B1_04:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO5_IO08, 0U);
                        RGPIO_PinInit(RGPIO5, 8, &do_config);
                        s_pinInfo[4].gpioGroup = 5;
                        s_pinInfo[4].gpioPin = 8;
                        break;
                    case kFlexspi1_Ss1A_GPIO_SD_B1_02:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_GPIO5_IO06, 0U);
                        RGPIO_PinInit(RGPIO5, 6, &do_config);
                        s_pinInfo[4].gpioGroup = 5;
                        s_pinInfo[4].gpioPin = 6;
                        break;
                    case kFlexspi1_Ss0B_GPIO_SD_B2_06:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_06_GPIO5_IO16, 0U);
                        RGPIO_PinInit(RGPIO5, 16, &do_config);
                        s_pinInfo[4].gpioGroup = 5;
                        s_pinInfo[4].gpioPin = 16;
                        break;
                    case kFlexspi1_Ss0B_GPIO_SD_B1_05:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_GPIO5_IO09, 0U);
                        RGPIO_PinInit(RGPIO5, 9, &do_config);
                        s_pinInfo[4].gpioGroup = 5;
                        s_pinInfo[4].gpioPin = 9;
                        break;
                    case kFlexspi1_Ss0B_GPIO_SD_B1_04:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO5_IO08, 0U);
                        RGPIO_PinInit(RGPIO5, 8, &do_config);
                        s_pinInfo[4].gpioGroup = 5;
                        s_pinInfo[4].gpioPin = 8;
                        break;
                    case kFlexspi1_Ss0B_GPIO_B1_04:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B1_04_GPIO6_IO04, 0U);
                        RGPIO_PinInit(RGPIO6, 4, &do_config);
                        s_pinInfo[4].gpioGroup = 6;
                        s_pinInfo[4].gpioPin = 4;
                        break;
                    case kFlexspi1_Ss1B_GPIO_SD_B2_04:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_04_GPIO5_IO14, 0U);
                        RGPIO_PinInit(RGPIO5, 14, &do_config);
                        s_pinInfo[4].gpioGroup = 5;
                        s_pinInfo[4].gpioPin = 14;
                        break;
                    case kFlexspi1_Ss1B_GPIO_SD_B1_03:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_GPIO5_IO07, 0U);
                        RGPIO_PinInit(RGPIO5, 7, &do_config);
                        s_pinInfo[4].gpioGroup = 5;
                        s_pinInfo[4].gpioPin = 7;
                        break;
                    case kFlexspi1_Ss1B_GPIO_B1_02:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B1_02_GPIO6_IO02, 0U);
                        RGPIO_PinInit(RGPIO6, 2, &do_config);
                        s_pinInfo[4].gpioGroup = 6;
                        s_pinInfo[4].gpioPin = 2;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.sclk)
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
                    case kFlexspi1_SclkB_GPIO_B2_02:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_02_GPIO6_IO16, 0U);
                        RGPIO_PinInit(RGPIO6, 16, &do_config);
                        s_pinInfo[5].gpioGroup = 6;
                        s_pinInfo[5].gpioPin = 16;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.dqs0)
            {
                switch (packet->memConnection.dqs0)
                {
                    case kFlexspi1_DqsA_GPIO_B2_07:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_07_GPIO6_IO21, 0U);
                        RGPIO_PinInit(RGPIO6, 21, &do_config);
                        s_pinInfo[6].gpioGroup = 6;
                        s_pinInfo[6].gpioPin = 21;
                        break;
                    case kFlexspi1_DqsA_GPIO_SD_B2_12:
                    case kFlexspi1_DqsB_GPIO_SD_B2_12:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_12_DUMMY_GPIO5_IO22, 0U);
                        RGPIO_PinInit(RGPIO5, 22, &do_config);
                        s_pinInfo[6].gpioGroup = 5;
                        s_pinInfo[6].gpioPin = 22;
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
            if (packet->pintestEn.option.B.dataHigh4bit)
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
            if (packet->pintestEn.option.B.dataTop8bit)
            {
            }
            if (packet->pintestEn.option.B.sclk_n)
            {
                switch (packet->memConnection.sclk_n)
                {
                    case kFlexspi1_SclkB_GPIO_B2_02:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B2_02_GPIO6_IO16, 0U);
                        RGPIO_PinInit(RGPIO6, 16, &do_config);
                        s_pinInfo[19].gpioGroup = 6;
                        s_pinInfo[19].gpioPin = 16;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.dqs1)
            {
                switch (packet->memConnection.dqs1)
                {
                    case kFlexspi1_DqsA_GPIO_SD_B2_12:
                    case kFlexspi1_DqsB_GPIO_SD_B2_12:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_12_DUMMY_GPIO5_IO22, 0U);
                        RGPIO_PinInit(RGPIO5, 22, &do_config);
                        s_pinInfo[20].gpioGroup = 5;
                        s_pinInfo[20].gpioPin = 22;
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
                    case kFlexspi2_DataA_GPIO_EMC_B1_38_35:
                        MTU_USE_MEM_DC_MUX1_FSPI2_PORTA
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_38_GPIO3_IO06, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_37_GPIO3_IO05, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_36_GPIO3_IO04, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_35_GPIO3_IO03, 0U);
                        RGPIO_PinInit(RGPIO3, 6, &do_config);
                        RGPIO_PinInit(RGPIO3, 5, &do_config);
                        RGPIO_PinInit(RGPIO3, 4, &do_config);
                        RGPIO_PinInit(RGPIO3, 3, &do_config);
                        s_pinInfo[0].gpioGroup = 3;
                        s_pinInfo[0].gpioPin = 6;
                        s_pinInfo[1].gpioGroup = 3;
                        s_pinInfo[1].gpioPin = 5;
                        s_pinInfo[2].gpioGroup = 3;
                        s_pinInfo[2].gpioPin = 4;
                        s_pinInfo[3].gpioGroup = 3;
                        s_pinInfo[3].gpioPin = 3;
                        break;
                    case kFlexspi2_DataA_GPIO_AON_27_24:
                        MTU_USE_MEM_DC_MUX2_FSPI2_PORTA
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_27_GPIO1_IO27, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_26_GPIO1_IO26, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_25_GPIO1_IO25, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_24_GPIO1_IO24, 0U);
                        RGPIO_PinInit(RGPIO1, 27, &do_config);
                        RGPIO_PinInit(RGPIO1, 26, &do_config);
                        RGPIO_PinInit(RGPIO1, 25, &do_config);
                        RGPIO_PinInit(RGPIO1, 24, &do_config);
                        s_pinInfo[0].gpioGroup = 1;
                        s_pinInfo[0].gpioPin = 27;
                        s_pinInfo[1].gpioGroup = 1;
                        s_pinInfo[1].gpioPin = 26;
                        s_pinInfo[2].gpioGroup = 1;
                        s_pinInfo[2].gpioPin = 25;
                        s_pinInfo[3].gpioGroup = 1;
                        s_pinInfo[3].gpioPin = 24;
                        break;
                    case kFlexspi2_DataB_GPIO_EMC_B1_33_30:
                        MTU_USE_MEM_DC_MUX1_FSPI2_PORTA
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_33_GPIO3_IO01, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_32_GPIO3_IO00, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_31_GPIO2_IO31, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_30_GPIO2_IO30, 0U);
                        RGPIO_PinInit(RGPIO3, 1, &do_config);
                        RGPIO_PinInit(RGPIO3, 0, &do_config);
                        RGPIO_PinInit(RGPIO2, 31, &do_config);
                        RGPIO_PinInit(RGPIO2, 30, &do_config);
                        s_pinInfo[0].gpioGroup = 3;
                        s_pinInfo[0].gpioPin = 1;
                        s_pinInfo[1].gpioGroup = 3;
                        s_pinInfo[1].gpioPin = 0;
                        s_pinInfo[2].gpioGroup = 2;
                        s_pinInfo[2].gpioPin = 31;
                        s_pinInfo[3].gpioGroup = 2;
                        s_pinInfo[3].gpioPin = 30;
                        break;
                    case kFlexspi2_DataB_GPIO_AON_18_15:
                        MTU_USE_MEM_DC_MUX2_FSPI2_PORTA
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_18_GPIO1_IO18, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_17_GPIO1_IO17, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_16_GPIO1_IO16, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_15_GPIO1_IO15, 0U);
                        RGPIO_PinInit(RGPIO1, 18, &do_config);
                        RGPIO_PinInit(RGPIO1, 17, &do_config);
                        RGPIO_PinInit(RGPIO1, 16, &do_config);
                        RGPIO_PinInit(RGPIO1, 15, &do_config);
                        s_pinInfo[0].gpioGroup = 1;
                        s_pinInfo[0].gpioPin = 18;
                        s_pinInfo[1].gpioGroup = 1;
                        s_pinInfo[1].gpioPin = 17;
                        s_pinInfo[2].gpioGroup = 1;
                        s_pinInfo[2].gpioPin = 16;
                        s_pinInfo[3].gpioGroup = 1;
                        s_pinInfo[3].gpioPin = 15;
                        break;
                    case kFlexspi2_DataB_GPIO_EMC_B1_25_22:
                        MTU_USE_MEM_DC_MUX3_FSPI2_PORTB
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_25_GPIO2_IO25, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_24_GPIO2_IO24, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_23_GPIO2_IO23, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_22_GPIO2_IO22, 0U);
                        RGPIO_PinInit(RGPIO2, 25, &do_config);
                        RGPIO_PinInit(RGPIO2, 24, &do_config);
                        RGPIO_PinInit(RGPIO2, 23, &do_config);
                        RGPIO_PinInit(RGPIO2, 22, &do_config);
                        s_pinInfo[0].gpioGroup = 2;
                        s_pinInfo[0].gpioPin = 25;
                        s_pinInfo[1].gpioGroup = 2;
                        s_pinInfo[1].gpioPin = 24;
                        s_pinInfo[2].gpioGroup = 2;
                        s_pinInfo[2].gpioPin = 23;
                        s_pinInfo[3].gpioGroup = 2;
                        s_pinInfo[3].gpioPin = 22;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.ss_b)
            {
                switch (packet->memConnection.ss_b)
                {
                    case kFlexspi2_Ss0A_GPIO_EMC_B1_39:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_39_GPIO3_IO07, 0U);
                        RGPIO_PinInit(RGPIO3, 7, &do_config);
                        s_pinInfo[4].gpioGroup = 3;
                        s_pinInfo[4].gpioPin = 7;
                        break;
                    case kFlexspi2_Ss0A_GPIO_AON_22:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_22_GPIO1_IO22, 0U);
                        RGPIO_PinInit(RGPIO1, 22, &do_config);
                        s_pinInfo[4].gpioGroup = 1;
                        s_pinInfo[4].gpioPin = 22;
                        break;
                    case kFlexspi2_Ss1A_GPIO_EMC_B1_26:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_26_GPIO2_IO26, 0U);
                        RGPIO_PinInit(RGPIO2, 26, &do_config);
                        s_pinInfo[4].gpioGroup = 2;
                        s_pinInfo[4].gpioPin = 26;
                        break;
                    case kFlexspi2_Ss1A_GPIO_AON_20:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_20_GPIO1_IO20, 0U);
                        RGPIO_PinInit(RGPIO1, 20, &do_config);
                        s_pinInfo[4].gpioGroup = 1;
                        s_pinInfo[4].gpioPin = 20;
                        break;
                    case kFlexspi2_Ss1A_GPIO_AON_19:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_19_GPIO1_IO19, 0U);
                        RGPIO_PinInit(RGPIO1, 19, &do_config);
                        s_pinInfo[4].gpioGroup = 1;
                        s_pinInfo[4].gpioPin = 19;
                        break;
                    case kFlexspi2_Ss0B_GPIO_EMC_B1_28:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_28_GPIO2_IO28, 0U);
                        RGPIO_PinInit(RGPIO2, 28, &do_config);
                        s_pinInfo[4].gpioGroup = 2;
                        s_pinInfo[4].gpioPin = 28;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.sclk)
            {
                switch (packet->memConnection.sclk)
                {
                    case kFlexspi2_SclkA_GPIO_EMC_B1_41:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_41_GPIO3_IO09, 0U);
                        RGPIO_PinInit(RGPIO3, 9, &do_config);
                        s_pinInfo[5].gpioGroup = 3;
                        s_pinInfo[5].gpioPin = 9;
                        break;
                    case kFlexspi2_SclkA_GPIO_AON_23:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_23_GPIO1_IO23, 0U);
                        RGPIO_PinInit(RGPIO1, 23, &do_config);
                        s_pinInfo[5].gpioGroup = 1;
                        s_pinInfo[5].gpioPin = 23;
                        break;
                    case kFlexspi2_SclkB_GPIO_EMC_B1_34:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_34_GPIO3_IO02, 0U);
                        RGPIO_PinInit(RGPIO3, 2, &do_config);
                        s_pinInfo[5].gpioGroup = 3;
                        s_pinInfo[5].gpioPin = 2;
                        break;
                    case kFlexspi2_SclkB_GPIO_AON_19:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_19_GPIO1_IO19, 0U);
                        RGPIO_PinInit(RGPIO1, 19, &do_config);
                        s_pinInfo[5].gpioGroup = 1;
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
                    case kFlexspi2_DqsA_GPIO_EMC_B1_40:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_40_GPIO3_IO08, 0U);
                        RGPIO_PinInit(RGPIO3, 8, &do_config);
                        s_pinInfo[6].gpioGroup = 3;
                        s_pinInfo[6].gpioPin = 8;
                        break;
                    case kFlexspi2_DqsA_GPIO_AON_21:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_21_GPIO1_IO21, 0U);
                        RGPIO_PinInit(RGPIO1, 21, &do_config);
                        s_pinInfo[6].gpioGroup = 1;
                        s_pinInfo[6].gpioPin = 21;
                        break;
                    case kFlexspi2_DqsA_GPIO_AON_28:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_28_DUMMY_GPIO1_IO28, 0U);
                        RGPIO_PinInit(RGPIO1, 28, &do_config);
                        s_pinInfo[6].gpioGroup = 1;
                        s_pinInfo[6].gpioPin = 28;
                        break;
                    case kFlexspi2_DqsB_GPIO_EMC_B1_29:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_29_GPIO2_IO29, 0U);
                        RGPIO_PinInit(RGPIO2, 29, &do_config);
                        s_pinInfo[6].gpioGroup = 2;
                        s_pinInfo[6].gpioPin = 29;
                        break;
                    case kFlexspi2_DqsB_GPIO_AON_20:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_20_GPIO1_IO20, 0U);
                        RGPIO_PinInit(RGPIO1, 20, &do_config);
                        s_pinInfo[6].gpioGroup = 1;
                        s_pinInfo[6].gpioPin = 20;
                        break;
                    case kFlexspi2_DqsB_GPIO_AON_28:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_28_DUMMY_GPIO1_IO28, 0U);
                        RGPIO_PinInit(RGPIO1, 28, &do_config);
                        s_pinInfo[6].gpioGroup = 1;
                        s_pinInfo[6].gpioPin = 28;
                        break;
                    case kFlexspi2_DqsB_GPIO_EMC_B1_21:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_21_GPIO2_IO21, 0U);
                        RGPIO_PinInit(RGPIO2, 21, &do_config);
                        s_pinInfo[6].gpioGroup = 2;
                        s_pinInfo[6].gpioPin = 21;
                        break;
                    default:
                        break;
                }
            }
            if (packet->pintestEn.option.B.dataHigh4bit)
            {
                switch (packet->memConnection.dataHigh4bit)
                {
                    case kFlexspi2_DataB_GPIO_EMC_B1_33_30:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_33_GPIO3_IO01, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_32_GPIO3_IO00, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_31_GPIO2_IO31, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_30_GPIO2_IO30, 0U);
                        RGPIO_PinInit(RGPIO3, 1, &do_config);
                        RGPIO_PinInit(RGPIO3, 0, &do_config);
                        RGPIO_PinInit(RGPIO2, 31, &do_config);
                        RGPIO_PinInit(RGPIO2, 30, &do_config);
                        s_pinInfo[7].gpioGroup = 3;
                        s_pinInfo[7].gpioPin = 1;
                        s_pinInfo[8].gpioGroup = 3;
                        s_pinInfo[8].gpioPin = 0;
                        s_pinInfo[9].gpioGroup = 2;
                        s_pinInfo[9].gpioPin = 31;
                        s_pinInfo[10].gpioGroup = 2;
                        s_pinInfo[10].gpioPin = 30;
                        break;
                    case kFlexspi2_DataB_GPIO_AON_18_15:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_18_GPIO1_IO18, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_17_GPIO1_IO17, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_16_GPIO1_IO16, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_15_GPIO1_IO15, 0U);
                        RGPIO_PinInit(RGPIO1, 18, &do_config);
                        RGPIO_PinInit(RGPIO1, 17, &do_config);
                        RGPIO_PinInit(RGPIO1, 16, &do_config);
                        RGPIO_PinInit(RGPIO1, 15, &do_config);
                        s_pinInfo[7].gpioGroup = 1;
                        s_pinInfo[7].gpioPin = 18;
                        s_pinInfo[8].gpioGroup = 1;
                        s_pinInfo[8].gpioPin = 17;
                        s_pinInfo[9].gpioGroup = 1;
                        s_pinInfo[9].gpioPin = 16;
                        s_pinInfo[10].gpioGroup = 1;
                        s_pinInfo[10].gpioPin = 15;
                        break;
                    case kFlexspi2_DataB_GPIO_EMC_B1_25_22:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_25_GPIO2_IO25, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_24_GPIO2_IO24, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_23_GPIO2_IO23, 0U);
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_22_GPIO2_IO22, 0U);
                        RGPIO_PinInit(RGPIO2, 25, &do_config);
                        RGPIO_PinInit(RGPIO2, 24, &do_config);
                        RGPIO_PinInit(RGPIO2, 23, &do_config);
                        RGPIO_PinInit(RGPIO2, 22, &do_config);
                        s_pinInfo[7].gpioGroup = 2;
                        s_pinInfo[7].gpioPin = 25;
                        s_pinInfo[8].gpioGroup = 2;
                        s_pinInfo[8].gpioPin = 24;
                        s_pinInfo[9].gpioGroup = 2;
                        s_pinInfo[9].gpioPin = 23;
                        s_pinInfo[10].gpioGroup = 2;
                        s_pinInfo[10].gpioPin = 22;
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
                    case kFlexspi2_SclkB_GPIO_EMC_B1_34:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_34_GPIO3_IO02, 0U);
                        RGPIO_PinInit(RGPIO3, 2, &do_config);
                        s_pinInfo[19].gpioGroup = 3;
                        s_pinInfo[19].gpioPin = 2;
                        break;
                    case kFlexspi2_SclkB_GPIO_AON_19:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_19_GPIO1_IO19, 0U);
                        RGPIO_PinInit(RGPIO1, 19, &do_config);
                        s_pinInfo[19].gpioGroup = 1;
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
                    case kFlexspi2_DqsB_GPIO_EMC_B1_29:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_29_GPIO2_IO29, 0U);
                        RGPIO_PinInit(RGPIO2, 29, &do_config);
                        s_pinInfo[20].gpioGroup = 2;
                        s_pinInfo[20].gpioPin = 29;
                        break;
                    case kFlexspi2_DqsB_GPIO_AON_20:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_20_GPIO1_IO20, 0U);
                        RGPIO_PinInit(RGPIO1, 20, &do_config);
                        s_pinInfo[20].gpioGroup = 1;
                        s_pinInfo[20].gpioPin = 20;
                        break;
                    case kFlexspi2_DqsB_GPIO_AON_28:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_AON_28_DUMMY_GPIO1_IO28, 0U);
                        RGPIO_PinInit(RGPIO1, 28, &do_config);
                        s_pinInfo[20].gpioGroup = 1;
                        s_pinInfo[20].gpioPin = 28;
                        break;
                    case kFlexspi2_DqsB_GPIO_EMC_B1_21:
                        IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_21_GPIO2_IO21, 0U);
                        RGPIO_PinInit(RGPIO2, 21, &do_config);
                        s_pinInfo[20].gpioGroup = 2;
                        s_pinInfo[20].gpioPin = 21;
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
                case kFlexspi1_DataA_GPIO_B2_13_10:
                    MTU_USE_MEM_DC_MUX1_FSPI1_PORTA
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
                    MTU_USE_MEM_DC_MUX1_FSPI1_PORTB
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
                    MTU_USE_MEM_DC_MUX2_FSPI1_PORTB
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
                    MTU_USE_FSPI1_PORTA1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_09_FLEXSPI1_BUS2BIT_A_SS0_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_09_FLEXSPI1_BUS2BIT_A_SS0_B, 0x0AU);
                    break;
                case kFlexspi1_Ss1A_GPIO_B2_01:
                    MTU_USE_FSPI1_PORTA2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_01_FLEXSPI1_BUS2BIT_A_SS1_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_01_FLEXSPI1_BUS2BIT_A_SS1_B, 0x0AU);
                    break;
                case kFlexspi1_Ss1A_GPIO_SD_B1_04:
                    MTU_USE_FSPI1_PORTA2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_FLEXSPI1_BUS2BIT_A_SS1_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_FLEXSPI1_BUS2BIT_A_SS1_B, 0x0AU);
                    break;
                case kFlexspi1_Ss1A_GPIO_SD_B1_02:
                    MTU_USE_FSPI1_PORTA2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_FLEXSPI1_BUS2BIT_A_SS1_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_02_FLEXSPI1_BUS2BIT_A_SS1_B, 0x0AU);
                    break;
                case kFlexspi1_Ss0B_GPIO_SD_B2_06:
                    MTU_USE_FSPI1_PORTB1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_06_FLEXSPI1_BUS2BIT_B_SS0_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_06_FLEXSPI1_BUS2BIT_B_SS0_B, 0x0AU);
                    break;
                case kFlexspi1_Ss0B_GPIO_SD_B1_05:
                    MTU_USE_FSPI1_PORTB1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_FLEXSPI1_BUS2BIT_B_SS0_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_05_FLEXSPI1_BUS2BIT_B_SS0_B, 0x0AU);
                    break;
                case kFlexspi1_Ss0B_GPIO_SD_B1_04:
                    MTU_USE_FSPI1_PORTB1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_FLEXSPI1_BUS2BIT_B_SS0_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_FLEXSPI1_BUS2BIT_B_SS0_B, 0x0AU);
                    break;
                case kFlexspi1_Ss0B_GPIO_B1_04:
                    MTU_USE_FSPI1_PORTB1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_04_FLEXSPI1_BUS2BIT_B_SS0_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_04_FLEXSPI1_BUS2BIT_B_SS0_B, 0x0AU);
                    break;
                case kFlexspi1_Ss1B_GPIO_SD_B2_04:
                    MTU_USE_FSPI1_PORTB2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_04_FLEXSPI1_BUS2BIT_B_SS1_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_04_FLEXSPI1_BUS2BIT_B_SS1_B, 0x0AU);
                    break;
                case kFlexspi1_Ss1B_GPIO_SD_B1_03:
                    MTU_USE_FSPI1_PORTB2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_FLEXSPI1_BUS2BIT_B_SS1_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_03_FLEXSPI1_BUS2BIT_B_SS1_B, 0x0AU);
                    break;
                case kFlexspi1_Ss1B_GPIO_B1_02:
                    MTU_USE_FSPI1_PORTB2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_02_FLEXSPI1_BUS2BIT_B_SS1_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_02_FLEXSPI1_BUS2BIT_B_SS1_B, 0x0AU);
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
                case kFlexspi1_SclkB_GPIO_B2_02:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_02_FLEXSPI1_BUS2BIT_B_SCLK, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_02_FLEXSPI1_BUS2BIT_B_SCLK, 0x0AU);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs0)
            {
                case kFlexspi1_DqsA_GPIO_B2_07:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_07_FLEXSPI1_BUS2BIT_A_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_07_FLEXSPI1_BUS2BIT_A_DQS, 0x0AU);
                    break;
                case kFlexspi1_DqsA_GPIO_SD_B2_12:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_12_DUMMY_FLEXSPI1_BUS2BIT_A_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_12_DUMMY_FLEXSPI1_BUS2BIT_A_DQS, 0x0AU);
                    break;
                case kFlexspi1_DqsB_GPIO_SD_B2_05:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_05_FLEXSPI1_BUS2BIT_B_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_05_FLEXSPI1_BUS2BIT_B_DQS, 0x0AU);
                    break;
                case kFlexspi1_DqsB_GPIO_SD_B2_12:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_12_DUMMY_FLEXSPI1_BUS2BIT_B_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_12_DUMMY_FLEXSPI1_BUS2BIT_B_DQS, 0x0AU);
                    break;
                case kFlexspi1_DqsB_GPIO_B1_03:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_03_FLEXSPI1_BUS2BIT_B_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_03_FLEXSPI1_BUS2BIT_B_DQS, 0x0AU);
                    break;
                default:
                    s_userConfig.mixspiReadSampleClock = kFLEXSPI_ReadSampleClkLoopbackInternally;
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
            switch (packet->memConnection.sclk_n)
            {
                case kFlexspi1_SclkB_GPIO_B2_02:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B2_02_FLEXSPI1_BUS2BIT_B_SCLK, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B2_02_FLEXSPI1_BUS2BIT_B_SCLK, 0x0AU);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs1)
            {
                case kFlexspi1_DqsA_GPIO_SD_B2_12:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_12_DUMMY_FLEXSPI1_BUS2BIT_A_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_12_DUMMY_FLEXSPI1_BUS2BIT_A_DQS, 0x0AU);
                    break;
                case kFlexspi1_DqsB_GPIO_SD_B2_12:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_12_DUMMY_FLEXSPI1_BUS2BIT_B_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_12_DUMMY_FLEXSPI1_BUS2BIT_B_DQS, 0x0AU);
                    break;
                default:
                    break;
            }
        }
        else if (packet->memConnection.instance == 2)
        {
            switch (packet->memConnection.dataLow4bit)
            {
                case kFlexspi2_DataA_GPIO_EMC_B1_38_35:
                    MTU_USE_MEM_DC_MUX1_FSPI2_PORTA
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_38_FLEXSPI2_BUS2BIT_A_DATA03, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_37_FLEXSPI2_BUS2BIT_A_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_36_FLEXSPI2_BUS2BIT_A_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_35_FLEXSPI2_BUS2BIT_A_DATA00, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_38_FLEXSPI2_BUS2BIT_A_DATA03, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_37_FLEXSPI2_BUS2BIT_A_DATA02, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_36_FLEXSPI2_BUS2BIT_A_DATA01, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_35_FLEXSPI2_BUS2BIT_A_DATA00, 0x0AU);
                    break;
                case kFlexspi2_DataA_GPIO_AON_27_24:
                    MTU_USE_MEM_DC_MUX2_FSPI2_PORTA
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_27_FLEXSPI2_BUS2BIT_A_DATA03, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_26_FLEXSPI2_BUS2BIT_A_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_25_FLEXSPI2_BUS2BIT_A_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_24_FLEXSPI2_BUS2BIT_A_DATA00, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_27_FLEXSPI2_BUS2BIT_A_DATA03, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_26_FLEXSPI2_BUS2BIT_A_DATA02, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_25_FLEXSPI2_BUS2BIT_A_DATA01, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_24_FLEXSPI2_BUS2BIT_A_DATA00, 0x0AU);
                    break;
                case kFlexspi2_DataB_GPIO_EMC_B1_33_30:
                    MTU_USE_MEM_DC_MUX1_FSPI2_PORTA
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_33_FLEXSPI2_BUS2BIT_B_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_32_FLEXSPI2_BUS2BIT_B_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_31_FLEXSPI2_BUS2BIT_B_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_30_FLEXSPI2_BUS2BIT_B_DATA03, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_33_FLEXSPI2_BUS2BIT_B_DATA00, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_32_FLEXSPI2_BUS2BIT_B_DATA01, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_31_FLEXSPI2_BUS2BIT_B_DATA02, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_30_FLEXSPI2_BUS2BIT_B_DATA03, 0x0AU);
                    break;
                case kFlexspi2_DataB_GPIO_AON_18_15:
                    MTU_USE_MEM_DC_MUX2_FSPI2_PORTA
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_18_FLEXSPI2_BUS2BIT_B_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_17_FLEXSPI2_BUS2BIT_B_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_16_FLEXSPI2_BUS2BIT_B_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_15_FLEXSPI2_BUS2BIT_B_DATA03, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_18_FLEXSPI2_BUS2BIT_B_DATA00, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_17_FLEXSPI2_BUS2BIT_B_DATA01, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_16_FLEXSPI2_BUS2BIT_B_DATA02, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_15_FLEXSPI2_BUS2BIT_B_DATA03, 0x0AU);
                    break;
                case kFlexspi2_DataB_GPIO_EMC_B1_25_22:
                    MTU_USE_MEM_DC_MUX3_FSPI2_PORTB
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_25_FLEXSPI2_BUS2BIT_B_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_24_FLEXSPI2_BUS2BIT_B_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_23_FLEXSPI2_BUS2BIT_B_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_22_FLEXSPI2_BUS2BIT_B_DATA03, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_25_FLEXSPI2_BUS2BIT_B_DATA00, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_24_FLEXSPI2_BUS2BIT_B_DATA01, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_23_FLEXSPI2_BUS2BIT_B_DATA02, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_22_FLEXSPI2_BUS2BIT_B_DATA03, 0x0AU);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.ss_b)
            {
                case kFlexspi2_Ss0A_GPIO_EMC_B1_39:
                    MTU_USE_FSPI2_PORTA1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_39_FLEXSPI2_BUS2BIT_A_SS0_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_39_FLEXSPI2_BUS2BIT_A_SS0_B, 0x0AU);
                    break;
                case kFlexspi2_Ss0A_GPIO_AON_22:
                    MTU_USE_FSPI2_PORTA1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_22_FLEXSPI2_BUS2BIT_A_SS0_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_39_FLEXSPI2_BUS2BIT_A_SS0_B, 0x0AU);
                    break;
                case kFlexspi2_Ss1A_GPIO_EMC_B1_26:
                    MTU_USE_FSPI2_PORTA2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_26_FLEXSPI2_BUS2BIT_A_SS1_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_26_FLEXSPI2_BUS2BIT_A_SS1_B, 0x0AU);
                    break;
                case kFlexspi2_Ss1A_GPIO_AON_20:
                    MTU_USE_FSPI2_PORTA2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_20_FLEXSPI2_BUS2BIT_A_SS1_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_20_FLEXSPI2_BUS2BIT_A_SS1_B, 0x0AU);
                    break;
                case kFlexspi2_Ss1A_GPIO_AON_19:
                    MTU_USE_FSPI2_PORTA2
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_19_FLEXSPI2_BUS2BIT_A_SS1_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_19_FLEXSPI2_BUS2BIT_A_SS1_B, 0x0AU);
                    break;
                case kFlexspi2_Ss0B_GPIO_EMC_B1_28:
                    MTU_USE_FSPI2_PORTB1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_28_FLEXSPI2_BUS2BIT_B_SS0_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_28_FLEXSPI2_BUS2BIT_B_SS0_B, 0x0AU);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.sclk)
            {
                case kFlexspi2_SclkA_GPIO_EMC_B1_41:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_41_FLEXSPI2_BUS2BIT_A_SCLK, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_41_FLEXSPI2_BUS2BIT_A_SCLK, 0x0AU);
                    break;
                case kFlexspi2_SclkA_GPIO_AON_23:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_23_FLEXSPI2_BUS2BIT_A_SCLK, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_23_FLEXSPI2_BUS2BIT_A_SCLK, 0x0AU);
                    break;
                case kFlexspi2_SclkB_GPIO_EMC_B1_34:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_34_FLEXSPI2_BUS2BIT_B_SCLK, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_34_FLEXSPI2_BUS2BIT_B_SCLK, 0x0AU);
                    break;
                case kFlexspi2_SclkB_GPIO_AON_19:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_19_FLEXSPI2_BUS2BIT_B_SCLK, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_19_FLEXSPI2_BUS2BIT_B_SCLK, 0x0AU);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs0)
            {
                case kFlexspi2_DqsA_GPIO_EMC_B1_40:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_40_FLEXSPI2_BUS2BIT_A_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_40_FLEXSPI2_BUS2BIT_A_DQS, 0x0AU);
                    break;
                case kFlexspi2_DqsA_GPIO_AON_21:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_21_FLEXSPI2_BUS2BIT_A_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_21_FLEXSPI2_BUS2BIT_A_DQS, 0x0AU);
                    break;
                case kFlexspi2_DqsA_GPIO_AON_28:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_28_DUMMY_FLEXSPI2_BUS2BIT_A_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_28_DUMMY_FLEXSPI2_BUS2BIT_A_DQS, 0x0AU);
                    break;
                case kFlexspi2_DqsB_GPIO_EMC_B1_29:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_29_FLEXSPI2_BUS2BIT_B_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_29_FLEXSPI2_BUS2BIT_B_DQS, 0x0AU);
                    break;
                case kFlexspi2_DqsB_GPIO_AON_20:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_20_FLEXSPI2_BUS2BIT_B_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_20_FLEXSPI2_BUS2BIT_B_DQS, 0x0AU);
                    break;
                case kFlexspi2_DqsB_GPIO_AON_28:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_28_DUMMY_FLEXSPI2_BUS2BIT_B_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_28_DUMMY_FLEXSPI2_BUS2BIT_B_DQS, 0x0AU);
                    break;
                case kFlexspi2_DqsB_GPIO_EMC_B1_21:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_21_FLEXSPI2_BUS2BIT_B_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_21_FLEXSPI2_BUS2BIT_B_DQS, 0x0AU);
                    break;
                default:
                    s_userConfig.mixspiReadSampleClock = kFLEXSPI_ReadSampleClkLoopbackInternally;
                    break;
            }
            switch (packet->memConnection.dataHigh4bit)
            {
                case kFlexspi2_DataB_GPIO_EMC_B1_33_30:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_33_FLEXSPI2_BUS2BIT_B_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_32_FLEXSPI2_BUS2BIT_B_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_31_FLEXSPI2_BUS2BIT_B_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_30_FLEXSPI2_BUS2BIT_B_DATA03, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_33_FLEXSPI2_BUS2BIT_B_DATA00, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_32_FLEXSPI2_BUS2BIT_B_DATA01, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_31_FLEXSPI2_BUS2BIT_B_DATA02, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_30_FLEXSPI2_BUS2BIT_B_DATA03, 0x0AU);
                    break;
                case kFlexspi2_DataB_GPIO_AON_18_15:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_18_FLEXSPI2_BUS2BIT_B_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_17_FLEXSPI2_BUS2BIT_B_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_16_FLEXSPI2_BUS2BIT_B_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_15_FLEXSPI2_BUS2BIT_B_DATA03, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_18_FLEXSPI2_BUS2BIT_B_DATA00, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_17_FLEXSPI2_BUS2BIT_B_DATA01, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_16_FLEXSPI2_BUS2BIT_B_DATA02, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_15_FLEXSPI2_BUS2BIT_B_DATA03, 0x0AU);
                    break;
                case kFlexspi2_DataB_GPIO_EMC_B1_25_22:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_25_FLEXSPI2_BUS2BIT_B_DATA00, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_24_FLEXSPI2_BUS2BIT_B_DATA01, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_23_FLEXSPI2_BUS2BIT_B_DATA02, 1U);
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_22_FLEXSPI2_BUS2BIT_B_DATA03, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_25_FLEXSPI2_BUS2BIT_B_DATA00, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_24_FLEXSPI2_BUS2BIT_B_DATA01, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_23_FLEXSPI2_BUS2BIT_B_DATA02, 0x0AU);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_22_FLEXSPI2_BUS2BIT_B_DATA03, 0x0AU);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.sclk_n)
            {
                case kFlexspi2_SclkB_GPIO_EMC_B1_34:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_34_FLEXSPI2_BUS2BIT_B_SCLK, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_34_FLEXSPI2_BUS2BIT_B_SCLK, 0x0AU);
                    break;
                case kFlexspi2_SclkB_GPIO_AON_19:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_19_FLEXSPI2_BUS2BIT_B_SCLK, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_19_FLEXSPI2_BUS2BIT_B_SCLK, 0x0AU);
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.dqs1)
            {
                case kFlexspi2_DqsB_GPIO_EMC_B1_29:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_29_FLEXSPI2_BUS2BIT_B_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_29_FLEXSPI2_BUS2BIT_B_DQS, 0x0AU);
                    break;
                case kFlexspi2_DqsB_GPIO_AON_20:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_20_FLEXSPI2_BUS2BIT_B_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_20_FLEXSPI2_BUS2BIT_B_DQS, 0x0AU);
                    break;
                case kFlexspi2_DqsB_GPIO_AON_28:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AON_28_DUMMY_FLEXSPI2_BUS2BIT_B_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AON_28_DUMMY_FLEXSPI2_BUS2BIT_B_DQS, 0x0AU);
                    break;
                case kFlexspi2_DqsB_GPIO_EMC_B1_21:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_21_FLEXSPI2_BUS2BIT_B_DQS, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_B1_21_FLEXSPI2_BUS2BIT_B_DQS, 0x0AU);
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
  
    RGPIO_Type *gpioBase[] = RGPIO_BASE_PTRS;
    for (uint32_t i = 0; i < sizeof(s_pinInfo) / sizeof(pin_info_t); i++)
    {
        if (s_pinInfo[i].gpioGroup != 0xFFFFu)
        {
            RGPIO_PortToggle(gpioBase[s_pinInfo[i].gpioGroup], 1u << s_pinInfo[i].gpioPin);
        }
    }
}

void bsp_mixspi_clock_init(void *config)
{
    clock_root_config_t rootCfg = {0};
    mixspi_user_config_t *userConfig = (mixspi_user_config_t *)config;
    mixspi_root_clk_freq_t clkFreq = userConfig->mixspiRootClkFreq;
    if (userConfig->mixspiBase == FLEXSPI1)
    {
        userConfig->instance = 1;
        ///////////////////////////////////////////////////////////////////
        // default settings in clock_config.c

        /* Init System Pll2 pfd0. */
        // 528*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        //CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd0, 29);  -- 327MHz

        /* Init System Pll3 pfd0. */
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        //CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 22);  -- 392MHz
        ///////////////////////////////////////////////////////////////////

        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 12-35.
        if (clkFreq == kMixspiRootClkFreq_30MHz)
        {
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc24M;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_50MHz)
        {
            // System Pll3 pfd0 (392MHz) / 8
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 8;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_60MHz)
        {
            // System Pll3 pfd0 (392MHz) / 7
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 7;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_80MHz)
        {
            // System Pll3 pfd0 (392MHz) / 5
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 5;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_100MHz)
        {
            // System Pll3 pfd0 (392MHz) / 4
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 4;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_120MHz)
        {
            // System Pll2 pfd0 (327MHz) / 3
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd0;
            rootCfg.div = 3;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_133MHz)
        {
            // System Pll3 pfd0 (392MHz) / 3
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 3;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_166MHz)
        {
            // System Pll2 pfd0 (327MHz) / 2
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd0;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_200MHz)
        {
            // System Pll3 pfd0 (392MHz) / 2
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_332MHz)
        {
            // System Pll2 pfd0 (327MHz) / 1
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd0;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_400MHz)
        {
            // System Pll3 pfd0 (392MHz) / 1
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else
        {
            printf("--FLEXSPI1 clock freq is not set.\r\n");
        }
        uint32_t flexspiClk = CLOCK_GetRootClockFreq(kCLOCK_Root_Flexspi1);
        if (flexspiClk > 166000000U)
        {
            __NOP();
        }
    }
    else if (userConfig->mixspiBase == FLEXSPI2)
    {
        userConfig->instance = 2;
        ///////////////////////////////////////////////////////////////////
        // default settings in clock_config.c

        /* Init System Pll2 pfd1. */
        // 528*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        //CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd1, 29);  -- 327MHz

        /* Init System Pll3 pfd2. */
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        //CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd2, 22);  -- 392MHz
        ///////////////////////////////////////////////////////////////////

        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 12-35.
        if (clkFreq == kMixspiRootClkFreq_30MHz)
        {
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxOscRc24M;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_50MHz)
        {
            // System Pll3 pfd2 (392MHz) / 8
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
            rootCfg.div = 8;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_60MHz)
        {
            // System Pll3 pfd2 (392MHz) / 7
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
            rootCfg.div = 7;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_80MHz)
        {
            // System Pll3 pfd2 (392MHz) / 5
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
            rootCfg.div = 5;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_100MHz)
        {
            // System Pll3 pfd2 (392MHz) / 4
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
            rootCfg.div = 4;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_120MHz)
        {
            // System Pll2 pfd1 (327MHz) / 3
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll2Pfd1;
            rootCfg.div = 3;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_133MHz)
        {
            // System Pll3 pfd2 (392MHz) / 3
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
            rootCfg.div = 3;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_166MHz)
        {
            // System Pll2 pfd1 (327MHz) / 2
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll2Pfd1;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_200MHz)
        {
            // System Pll3 pfd2 (392MHz) / 2
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_332MHz)
        {
            // System Pll2 pfd1 (327MHz) / 1
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll2Pfd1;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_400MHz)
        {
            // System Pll3 pfd2 (392MHz) / 1
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else
        {
            printf("--FlexSPI2 clock freq is not set.\r\n");
        }
        uint32_t flexspiClk = CLOCK_GetRootClockFreq(kCLOCK_Root_Flexspi2);
        if (flexspiClk > 166000000U)
        {
            __NOP();
        }
    }
    else
    {
    }
}

static uint32_t bsp_mixspi_get_clock(void *config)
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
            case kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc24M:
                printf("--FLEXSPI1 Clk Source from 2'b00 - OSC RC24M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_Osc24M));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc400M:
                printf("--FLEXSPI1 Clk Source from 2'b01 - OSC RC400M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc400M));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0:
                printf("--FLEXSPI1 Clk Source from 2'b10 - Sys PLL3 PFD0 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll3Pfd0));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd0:
                printf("--FLEXSPI1 Clk Source from 2'b11 - Sys PLL2 PFD0 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll2Pfd0));
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
            case kCLOCK_FLEXSPI2_ClockRoot_MuxOscRc24M:
                printf("--FLEXSPI2 Clk Source from 2'b00 - OSC RC24M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_Osc24M));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxOscRc400M:
                printf("--FLEXSPI2 Clk Source from 2'b01 - OSC RC400M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc400M));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2:
                printf("--FLEXSPI2 Clk Source from 2'b10 - Sys PLL3 PFD2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll3Pfd2));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll2Pfd1:
                printf("--FLEXSPI2 Clk Source from 2'b11 - Sys PLL2 PFD1 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll2Pfd1));
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
    printf("--FLEXSPI%d Clk Source Divider: %d.\r\n", index, clkDiv);
    printf("--FLEXSPI%d Clk Frequency: %dHz.\r\n", index, bsp_mixspi_get_clock(userConfig));
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

