/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * How to setup clock using clock driver functions:
 *
 * 1. Call CLOCK_InitXXXPLL() to configure corresponding PLL clock.
 *
 * 2. Call CLOCK_InitXXXpfd() to configure corresponding PLL pfd clock.
 *
 * 3. Call CLOCK_SetRootClock() to configure corresponding module clock source and divider.
 *
 */

#include "clock_config.h"
#include "fsl_iomuxc.h"
#include "fsl_dcdc.h"
#include "fsl_pmu.h"
#include "fsl_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/
void BOARD_InitBootClocks(void)
{
    BOARD_BootClockRUN();
}

/*******************************************************************************
 ********************** Configuration BOARD_BootClockRUN ***********************
 ******************************************************************************/

/*******************************************************************************
 * Variables for BOARD_BootClockRUN configuration
 ******************************************************************************/
const clock_arm_pll_config_t armPllConfig_BOARD_BootClockRUN =
    {
        .postDivider = kCLOCK_PllPostDiv2,        /* Post divider, 0 - DIV by 2, 1 - DIV by 4, 2 - DIV by 8, 3 - DIV by 1 */
        .loopDivider = 132,                       /* PLL Loop divider, Fout = Fin * ( loopDivider / ( 2 * postDivider ) ) */
    };

const clock_sys_pll1_config_t sysPll1Config_BOARD_BootClockRUN =
    {
        .pllDiv2En = 1,                           /* Enable Sys Pll1 divide-by-2 clock or not */
        .pllDiv5En = 1,                           /* Enable Sys Pll1 divide-by-5 clock or not */
        .ss = NULL,                               /* Spread spectrum parameter */
        .ssEnable = false,                        /* Enable spread spectrum or not */
    };

const clock_sys_pll2_config_t sysPll2Config_BOARD_BootClockRUN =
    {
        .mfd = 268435455,                         /* Denominator of spread spectrum */
        .ss = NULL,                               /* Spread spectrum parameter */
        .ssEnable = false,                        /* Enable spread spectrum or not */
    };

/*******************************************************************************
 * Code for BOARD_BootClockRUN configuration
 ******************************************************************************/
void BOARD_BootClockRUN(void)
{
    clock_root_config_t rootCfg = {0};

#if __CORTEX_M == 7
    DCDC_SetVDD1P0BuckModeTargetVoltage(DCDC, kDCDC_1P0Target1P1V);
#endif

    /* Config CLK_1M */
    CLOCK_OSC_Set1MHzOutputBehavior(kCLOCK_1MHzOutEnableFreeRunning1Mhz);

    /* Init OSC RC 400M */
    CLOCK_OSC_EnableOscRc400M();
    CLOCK_OSC_GateOscRc400M(true);

    /* Init OSC RC 24M */
    CLOCK_OSC_EnableOscRc24M(true);

    /* Config OSC 24M */
    ANADIG_OSC->OSC_24M_CTRL |= ANADIG_OSC_OSC_24M_CTRL_OSC_EN(1) | ANADIG_OSC_OSC_24M_CTRL_BYPASS_EN(0) | ANADIG_OSC_OSC_24M_CTRL_BYPASS_CLK(0) | ANADIG_OSC_OSC_24M_CTRL_LP_EN(1) | ANADIG_OSC_OSC_24M_CTRL_OSC_24M_GATE(0);
    /* Wait for 24M OSC to be stable. */
    while (ANADIG_OSC_OSC_24M_CTRL_OSC_24M_STABLE_MASK !=
            (ANADIG_OSC->OSC_24M_CTRL & ANADIG_OSC_OSC_24M_CTRL_OSC_24M_STABLE_MASK))
    {
    }

    /* Swicth both core to OscRC24M first */
#if __CORTEX_M == 7
    rootCfg.mux = kCLOCK_M7_ClockRoot_MuxOscRc24M;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_M7, &rootCfg);
#endif

#if __CORTEX_M == 33
    rootCfg.mux = kCLOCK_M33_ClockRoot_MuxOscRc24M;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_M33, &rootCfg);
#endif

    /*
    * if DCD is used, please make sure the clock source of SEMC is not changed in the following PLL/PFD configuration code.
    */
    /* Init Arm Pll. */
    CLOCK_InitArmPll(&armPllConfig_BOARD_BootClockRUN);

    /* Init Sys Pll1. */
    CLOCK_InitSysPll1(&sysPll1Config_BOARD_BootClockRUN);

    /* Init Sys Pll2. */
    CLOCK_InitSysPll2(&sysPll2Config_BOARD_BootClockRUN);

    /* Init System Pll2 pfd0. */
    CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd0, 27);

    /* Init System Pll2 pfd1. */
    CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd1, 16);

    /* Init System Pll2 pfd2. */
    CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd2, 24);

    /* Init System Pll2 pfd3. */
    CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd3, 32);

    /* Init Sys Pll3. */
    CLOCK_InitSysPll3();

#if defined(XIP_EXTERNAL_FLASH)
    /* Init System Pll3 pfd0. */
    CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 13);

    /* Init System Pll3 pfd1. */
    CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd1, 17);

    /* Init System Pll3 pfd2. */
    CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd2, 32);

    /* Init System Pll3 pfd3. */
    CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd3, 22);
#else
    /* Init System Pll3 pfd0. */
    CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 22);

    /* Init System Pll3 pfd1. */
    CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd1, 33);

    /* Init System Pll3 pfd2. */
    CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd2, 26);

    /* Init System Pll3 pfd3. */
    CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd3, 18);
#endif

    /* Bypass Audio Pll. */
    CLOCK_SetPllBypass(kCLOCK_PllAudio, true);

    /* DeInit Audio Pll. */
    CLOCK_DeinitAudioPll();

    /* Module clock root configurations. */
#if __CORTEX_M == 7
    rootCfg.mux = kCLOCK_M7_ClockRoot_MuxArmPllOut;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_M7, &rootCfg);
#endif

#if __CORTEX_M == 33
    rootCfg.mux = kCLOCK_M33_ClockRoot_MuxSysPll3Out;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_M33, &rootCfg);

    rootCfg.mux = kCLOCK_SENTINEL_ClockRoot_MuxSysPll1Out;
    rootCfg.div = 5;
    CLOCK_SetRootClock(kCLOCK_Root_Sentinel, &rootCfg);
	
    rootCfg.mux = kCLOCK_BUS_AON_ClockRoot_MuxSysPll2Out;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Bus_Aon, &rootCfg);
#endif

#if __CORTEX_M == 7
    rootCfg.mux = kCLOCK_BUS_WAKEUP_ClockRoot_MuxSysPll2Out;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Bus_Wakeup, &rootCfg);

    rootCfg.mux = kCLOCK_WAKEUP_AXI_ClockRoot_MuxSysPll3Out;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Wakeup_Axi, &rootCfg);
#endif

    rootCfg.mux = kCLOCK_SWO_TRACE_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 3;
    CLOCK_SetRootClock(kCLOCK_Root_Swo_Trace, &rootCfg);

#if __CORTEX_M == 33
    rootCfg.mux = kCLOCK_M33_SYSTICK_ClockRoot_MuxOsc24MOut;
    rootCfg.div = 240;
    CLOCK_SetRootClock(kCLOCK_Root_M33_Systick, &rootCfg);
#endif

#if __CORTEX_M == 7
    rootCfg.mux = kCLOCK_M7_SYSTICK_ClockRoot_MuxOsc24MOut;
    rootCfg.div = 240;
    CLOCK_SetRootClock(kCLOCK_Root_M7_Systick, &rootCfg);
#endif

    rootCfg.mux = kCLOCK_FLEXIO1_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Flexio1, &rootCfg);

    rootCfg.mux = kCLOCK_FLEXIO2_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Flexio2, &rootCfg);

    rootCfg.mux = kCLOCK_LPIT3_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 3;
    CLOCK_SetRootClock(kCLOCK_Root_Lpit3, &rootCfg);

    rootCfg.mux = kCLOCK_LPTIMER1_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 3;
    CLOCK_SetRootClock(kCLOCK_Root_Lptimer1, &rootCfg);

    rootCfg.mux = kCLOCK_LPTIMER2_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 3;
    CLOCK_SetRootClock(kCLOCK_Root_Lptimer2, &rootCfg);

    rootCfg.mux = kCLOCK_LPTIMER3_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 3;
    CLOCK_SetRootClock(kCLOCK_Root_Lptimer3, &rootCfg);

    rootCfg.mux = kCLOCK_TPM2_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 3;
    CLOCK_SetRootClock(kCLOCK_Root_Tpm2, &rootCfg);

    rootCfg.mux = kCLOCK_TPM4_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 3;
    CLOCK_SetRootClock(kCLOCK_Root_Tpm4, &rootCfg);

    rootCfg.mux = kCLOCK_TPM5_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 3;
    CLOCK_SetRootClock(kCLOCK_Root_Tpm5, &rootCfg);

    rootCfg.mux = kCLOCK_TPM6_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 3;
    CLOCK_SetRootClock(kCLOCK_Root_Tpm6, &rootCfg);

    rootCfg.mux = kCLOCK_GPT1_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Gpt1, &rootCfg);
	
    rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Gpt2, &rootCfg);
	
#if !(defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
    rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);

    rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
#endif

    rootCfg.mux = kCLOCK_FLEXSPI_SLV_ClockRoot_MuxSysPll2Out;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Flexspi_Slv, &rootCfg);
	
    rootCfg.mux = kCLOCK_CAN1_ClockRoot_MuxSysPll3Out;
    rootCfg.div = 6;
    CLOCK_SetRootClock(kCLOCK_Root_Can1, &rootCfg);

    rootCfg.mux = kCLOCK_CAN2_ClockRoot_MuxSysPll3Out;
    rootCfg.div = 6;
    CLOCK_SetRootClock(kCLOCK_Root_Can2, &rootCfg);

    rootCfg.mux = kCLOCK_CAN3_ClockRoot_MuxSysPll3Out;
    rootCfg.div = 6;
    CLOCK_SetRootClock(kCLOCK_Root_Can3, &rootCfg);

    rootCfg.mux = kCLOCK_LPUART0102_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 10;
    CLOCK_SetRootClock(kCLOCK_Root_Lpuart0102, &rootCfg);

    rootCfg.mux = kCLOCK_LPUART0304_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 10;
    CLOCK_SetRootClock(kCLOCK_Root_Lpuart0304, &rootCfg);

    rootCfg.mux = kCLOCK_LPUART0506_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 10;
    CLOCK_SetRootClock(kCLOCK_Root_Lpuart0506, &rootCfg);

    rootCfg.mux = kCLOCK_LPUART0708_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 10;
    CLOCK_SetRootClock(kCLOCK_Root_Lpuart0708, &rootCfg);

    rootCfg.mux = kCLOCK_LPUART0910_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 10;
    CLOCK_SetRootClock(kCLOCK_Root_Lpuart0910, &rootCfg);

    rootCfg.mux = kCLOCK_LPUART1112_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 10;
    CLOCK_SetRootClock(kCLOCK_Root_Lpuart1112, &rootCfg);

    rootCfg.mux = kCLOCK_LPI2C0102_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Lpi2c0102, &rootCfg);

    rootCfg.mux = kCLOCK_LPI2C0304_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Lpi2c0304, &rootCfg);

    rootCfg.mux = kCLOCK_LPI2C0506_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Lpi2c0506, &rootCfg);

    rootCfg.mux = kCLOCK_LPSPI0102_ClockRoot_MuxSysPll3Pfd1;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Lpspi0102, &rootCfg);

    rootCfg.mux = kCLOCK_LPSPI0304_ClockRoot_MuxSysPll3Pfd1;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Lpspi0304, &rootCfg);

    rootCfg.mux = kCLOCK_LPSPI0506_ClockRoot_MuxSysPll3Pfd1;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Lpspi0506, &rootCfg);

    rootCfg.mux = kCLOCK_I3C1_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 10;
    CLOCK_SetRootClock(kCLOCK_Root_I3c1, &rootCfg);

    rootCfg.mux = kCLOCK_I3C2_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 10;
    CLOCK_SetRootClock(kCLOCK_Root_I3c2, &rootCfg);

    rootCfg.mux = kCLOCK_USDHC1_ClockRoot_MuxSysPll2Pfd2;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Usdhc1, &rootCfg);

    rootCfg.mux = kCLOCK_USDHC2_ClockRoot_MuxSysPll2Pfd2;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Usdhc2, &rootCfg);

#ifndef SKIP_SEMC_INIT
    rootCfg.mux = kCLOCK_SEMC_ClockRoot_MuxSysPll1Out;
    rootCfg.div = 5;
    CLOCK_SetRootClock(kCLOCK_Root_Semc, &rootCfg);
#endif

    rootCfg.mux = kCLOCK_ADC1_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Adc1, &rootCfg);

    rootCfg.mux = kCLOCK_ADC2_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Adc2, &rootCfg);

    rootCfg.mux = kCLOCK_ACMP_ClockRoot_MuxSysPll3Out;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Acmp, &rootCfg);
	
    rootCfg.mux = kCLOCK_ECAT_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 5;
    CLOCK_SetRootClock(kCLOCK_Root_Ecat, &rootCfg);

    rootCfg.mux = kCLOCK_ENET_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Enet, &rootCfg);

    rootCfg.mux = kCLOCK_TMR_1588_ClockRoot_MuxSysPll3Out;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Tmr_1588, &rootCfg);

    rootCfg.mux = kCLOCK_NETC_ClockRoot_MuxSysPll3Pfd3;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Netc, &rootCfg);

    rootCfg.mux = kCLOCK_MAC0_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 10;
    CLOCK_SetRootClock(kCLOCK_Root_Mac0, &rootCfg);

    rootCfg.mux = kCLOCK_MAC1_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Mac1, &rootCfg);

    rootCfg.mux = kCLOCK_MAC2_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Mac2, &rootCfg);

    rootCfg.mux = kCLOCK_MAC3_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Mac3, &rootCfg);

    rootCfg.mux = kCLOCK_MAC4_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 10;
    CLOCK_SetRootClock(kCLOCK_Root_Mac4, &rootCfg);

    rootCfg.mux = kCLOCK_SERDES0_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Serdes0, &rootCfg);

    rootCfg.mux = kCLOCK_SERDES1_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Serdes1, &rootCfg);

    rootCfg.mux = kCLOCK_SERDES2_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Serdes2, &rootCfg);

    rootCfg.mux = kCLOCK_SERDES0_1G_ClockRoot_MuxSysPll1Out;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Serdes0_1G, &rootCfg);

    rootCfg.mux = kCLOCK_SERDES1_1G_ClockRoot_MuxSysPll1Out;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Serdes1_1G, &rootCfg);

    rootCfg.mux = kCLOCK_SERDES2_1G_ClockRoot_MuxSysPll1Out;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Serdes2_1G, &rootCfg);

    rootCfg.mux = kCLOCK_XCELBUSX_ClockRoot_MuxSysPll3Out;
    rootCfg.div = 5;
    CLOCK_SetRootClock(kCLOCK_Root_Xcelbusx, &rootCfg);

    rootCfg.mux = kCLOCK_XRIOCU4_ClockRoot_MuxOsc24MOut;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Xriocu4, &rootCfg);

    rootCfg.mux = kCLOCK_MCTRL_ClockRoot_MuxSysPll1Div5;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Mctrl, &rootCfg);

    rootCfg.mux = kCLOCK_SAI1_ClockRoot_MuxAudioPllOut;
    rootCfg.div = 12;
    CLOCK_SetRootClock(kCLOCK_Root_Sai1, &rootCfg);

    rootCfg.mux = kCLOCK_SAI2_ClockRoot_MuxAudioPllOut;
    rootCfg.div = 12;
    CLOCK_SetRootClock(kCLOCK_Root_Sai2, &rootCfg);

    rootCfg.mux = kCLOCK_SAI3_ClockRoot_MuxAudioPllOut;
    rootCfg.div = 12;
    CLOCK_SetRootClock(kCLOCK_Root_Sai3, &rootCfg);

    rootCfg.mux = kCLOCK_SAI4_ClockRoot_MuxAudioPllOut;
    rootCfg.div = 12;
    CLOCK_SetRootClock(kCLOCK_Root_Sai4, &rootCfg);

    rootCfg.mux = kCLOCK_SPDIF_ClockRoot_MuxAudioPllOut;
    rootCfg.div = 12;
    CLOCK_SetRootClock(kCLOCK_Root_Spdif, &rootCfg);

    rootCfg.mux = kCLOCK_ASRC_ClockRoot_MuxSysPll3Out;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Asrc, &rootCfg);

    rootCfg.mux = kCLOCK_MIC_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 3;
    CLOCK_SetRootClock(kCLOCK_Root_Mic, &rootCfg);

    rootCfg.mux = kCLOCK_CKO1_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 3;
    CLOCK_SetRootClock(kCLOCK_Root_Cko1, &rootCfg);

    rootCfg.mux = kCLOCK_CKO2_ClockRoot_MuxSysPll1Div5;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Cko2, &rootCfg);

#if __CORTEX_M == 7
    SystemCoreClock = CLOCK_GetRootClockFreq(kCLOCK_Root_M7);
#else
    SystemCoreClock = CLOCK_GetRootClockFreq(kCLOCK_Root_M33);
#endif
}
