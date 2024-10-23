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

#define PAD_CONFIG                     (0x0A)

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

void bsp_mixspi_pinmux_config(void *configPacket, bool isPintest, uint32_t pad_config)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);

    if (pad_config == 0)
    {
        pad_config = PAD_CONFIG;
    }
    else
    {
        pad_config = 0x0AU;
    }

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
                        break;
                    case kFlexspi1_DataB_GPIO_SD_B2_03_00:
                        break;
                    case kFlexspi1_DataB_GPIO_AD_15_12:
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
                        break;
                    case kFlexspi1_SclkB_GPIO_SD_B2_04:
                        break;
                    case kFlexspi1_SclkB_GPIO_AD_16:
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
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_08_FLEXSPI1_A_DATA00, pad_config);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_09_FLEXSPI1_A_DATA01, pad_config);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_10_FLEXSPI1_A_DATA02, pad_config);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_11_FLEXSPI1_A_DATA03, pad_config);
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
                    MTU_USE_FSPI1_PORTA1
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_06_FLEXSPI1_A_SS0_B, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_06_FLEXSPI1_A_SS0_B, pad_config);
                    break;
                case kFlexspi1_Ss0A_GPIO_AD_18:
                    MTU_USE_FSPI1_PORTA1
                    break;
                case kFlexspi1_Ss1A_GPIO_SD_B1_02:
                    MTU_USE_FSPI1_PORTA2
                    break;
                case kFlexspi1_Ss0B_GPIO_SD_B1_04:
                    MTU_USE_FSPI1_PORTB1
                    break;
                case kFlexspi1_Ss1B_GPIO_AD_35:
                    MTU_USE_FSPI1_PORTB2
                    break;
                case kFlexspi1_Ss1B_GPIO_SD_B1_03:
                    MTU_USE_FSPI1_PORTB2
                    break;
                default:
                    break;
            }
            switch (packet->memConnection.sclk)
            {
                case kFlexspi1_SclkA_GPIO_SD_B2_07:
                    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_07_FLEXSPI1_A_SCLK, 1U);
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_07_FLEXSPI1_A_SCLK, pad_config);
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
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_05_FLEXSPI1_A_DQS, pad_config);
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
    if (userConfig->mixspiBase == FLEXSPI1)
    {
        userConfig->instance = 1;
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 12-35.
        if (clkFreq == kMixspiRootClkFreq_30MHz)
        {
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc48MDiv2;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_50MHz)
        {
            /* Init System Pll2 (528MHz) pfd2. */
            // 528*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            // CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd2, 24);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd2;
            rootCfg.div = 8;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_60MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 18);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 8;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_80MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 18);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 6;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_100MHz)
        {
            /* Init System Pll2 (528MHz) pfd2. */
            // 528*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            // CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd2, 24);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd2;
            rootCfg.div = 4;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_120MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 18);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 4;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_133MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            //CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 13);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 5;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_166MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            //CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 13);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 4;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_200MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 22);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_240MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 18);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_266MHz)
        {
            /* Init System Pll2 (528MHz) pfd2. */
            // 528*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd2, 18);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd2;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_332MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            //CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 13);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_400MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 22);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else
        {
            printf("FLEXSPI1 clock freq is not set.\r\n");
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
    }
    else
    {
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
