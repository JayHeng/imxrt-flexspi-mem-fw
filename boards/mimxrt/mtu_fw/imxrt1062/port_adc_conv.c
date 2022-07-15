/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "mtu.h"
#include "pin_mux.h"
#include "fsl_iomuxc.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_adc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC_BASE          ADC1
#define DEMO_ADC_USER_CHANNEL  0U
#define DEMO_ADC_CHANNEL_GROUP 0U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
const uint32_t g_Adc_12bitFullRange = 4096U;

adc_channel_config_t g_adcChannelConfigStruct;

/*******************************************************************************
 * Code
 ******************************************************************************/
void bsp_adc_echo_info(void)
{
    printf("--adc1_in0(GPIO_AD_B1_11) can be used to sample pin wave. \r\n");
}

/*!
 * @brief Main function
 */
void bsp_adc_init(void)
{
    adc_config_t adcConfigStruct;

    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_11_GPIO1_IO27, 0xB0U); 

    /*
     *  config->enableAsynchronousClockOutput = true;
     *  config->enableOverWrite =               false;
     *  config->enableContinuousConversion =    false;
     *  config->enableHighSpeed =               false;
     *  config->enableLowPower =                false;
     *  config->enableLongSample =              false;
     *  config->referenceVoltageSource =        kADC_ReferenceVoltageSourceVref;
     *  config->samplePeriodMode =              kADC_SamplePeriod2or12Clocks;
     *  config->clockSource =                   kADC_ClockSourceAD;
     *  config->clockDriver =                   kADC_ClockDriver1;
     *  config->resolution =                    kADC_Resolution12Bit;
     */
    ADC_GetDefaultConfig(&adcConfigStruct);
    ADC_Init(DEMO_ADC_BASE, &adcConfigStruct);
#if !(defined(FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE) && FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE)
    ADC_EnableHardwareTrigger(DEMO_ADC_BASE, false);
#endif

    /* Do auto hardware calibration. */
    if (kStatus_Success == ADC_DoAutoCalibration(DEMO_ADC_BASE))
    {
        //PRINTF("ADC_DoAutoCalibration() Done.\r\n");
    }
    else
    {
        //PRINTF("ADC_DoAutoCalibration() Failed.\r\n");
    }

    /* Configure the user channel and interrupt. */
    g_adcChannelConfigStruct.channelNumber                        = DEMO_ADC_USER_CHANNEL;
    g_adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;

    //PRINTF("ADC Full Range: %d\r\n", g_Adc_12bitFullRange);
}

uint8_t bsp_adc_get_conv_value(void)
{
    /*
     When in software trigger mode, each conversion would be launched once calling the "ADC_ChannelConfigure()"
     function, which works like writing a conversion command and executing it. For another channel's conversion,
     just to change the "channelNumber" field in channel's configuration structure, and call the
     "ADC_ChannelConfigure() again.
    */
    ADC_SetChannelConfig(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP, &g_adcChannelConfigStruct);
    while (0U == ADC_GetChannelStatusFlags(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP))
    {
    }

    uint32_t convValue = ADC_GetChannelConversionValue(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP);
    if (convValue > g_Adc_12bitFullRange / 2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void bsp_adc_deinit(void)
{
    ADC_Deinit(DEMO_ADC_BASE);
}
