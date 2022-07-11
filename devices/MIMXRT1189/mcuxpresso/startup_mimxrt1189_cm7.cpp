//*****************************************************************************
// MIMXRT1189_cm7 startup code for use with MCUXpresso IDE
//
// Version : 211221
//*****************************************************************************
//
// Copyright 2016-2021 NXP
// All rights reserved.
//
// SPDX-License-Identifier: BSD-3-Clause
//*****************************************************************************

#if defined (DEBUG)
#pragma GCC push_options
#pragma GCC optimize ("Og")
#endif // (DEBUG)

#if defined (__cplusplus)
#ifdef __REDLIB__
#error Redlib does not support C++
#else
//*****************************************************************************
//
// The entry point for the C++ library startup
//
//*****************************************************************************
extern "C" {
    extern void __libc_init_array(void);
}
#endif
#endif

#define WEAK __attribute__ ((weak))
#define WEAK_AV __attribute__ ((weak, section(".after_vectors")))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

//*****************************************************************************
#if defined (__cplusplus)
extern "C" {
#endif

//*****************************************************************************
// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
//*****************************************************************************
//*****************************************************************************
// Declaration of external SystemInit function
//*****************************************************************************
#if defined (__USE_CMSIS)
extern void SystemInit(void);
#endif // (__USE_CMSIS)

//*****************************************************************************
// Forward declaration of the core exception handlers.
// When the application defines a handler (with the same name), this will
// automatically take precedence over these weak definitions.
// If your application is a C++ one, then any interrupt handlers defined
// in C++ files within in your main application will need to have C linkage
// rather than C++ linkage. To do this, make sure that you are using extern "C"
// { .... } around the interrupt handler within your main application code.
//*****************************************************************************
     void ResetISR(void);
WEAK void IntDefaultHandler(void);

//*****************************************************************************
// Forward declaration of the application IRQ handlers. When the application
// defines a handler (with the same name), this will automatically take
// precedence over weak definitions below
//*****************************************************************************
WEAK void TMR1_IRQHandler(void);
WEAK void DAP_IRQHandler(void);
WEAK void M7_CTI_TRIGGER_OUTPUT_IRQHandler(void);
WEAK void M33_CTI_TRIGGER_OUTPUT_IRQHandler(void);
WEAK void TMR5_IRQHandler(void);
WEAK void TMR6_IRQHandler(void);
WEAK void TMR7_IRQHandler(void);
WEAK void TMR8_IRQHandler(void);
WEAK void CAN1_IRQHandler(void);
WEAK void CAN1_ERROR_IRQHandler(void);
WEAK void GPIO1_0_IRQHandler(void);
WEAK void GPIO1_1_IRQHandler(void);
WEAK void I3C1_IRQHandler(void);
WEAK void LPI2C1_IRQHandler(void);
WEAK void LPI2C2_IRQHandler(void);
WEAK void LPIT1_IRQHandler(void);
WEAK void LPSPI1_IRQHandler(void);
WEAK void LPSPI2_IRQHandler(void);
WEAK void LPTMR1_IRQHandler(void);
WEAK void LPUART1_IRQHandler(void);
WEAK void LPUART2_IRQHandler(void);
WEAK void MU1_IRQHandler(void);
WEAK void MU2_IRQHandler(void);
WEAK void PWM1_FAULT_IRQHandler(void);
WEAK void PWM1_0_IRQHandler(void);
WEAK void PWM1_1_IRQHandler(void);
WEAK void PWM1_2_IRQHandler(void);
WEAK void PWM1_3_IRQHandler(void);
WEAK void SENTINEL_TRUST_MUA_RX_FULL_IRQHandler(void);
WEAK void SENTINEL_TRUST_MUA_TX_EMPTY_IRQHandler(void);
WEAK void SENTINEL_APPS_CORE_MUA_RX_FULL_IRQHandler(void);
WEAK void SENTINEL_APPS_CORE_MUA_TX_EMPTY_IRQHandler(void);
WEAK void SENTINEL_REALTIME_CORE_MUA_RX_FULL_IRQHandler(void);
WEAK void SENTINEL_REALTIME_CORE_MUA_TX_EMPTY_IRQHandler(void);
WEAK void SENTINEL_SECURE_IRQHandler(void);
WEAK void SENTINEL_NONSECURE_IRQHandler(void);
WEAK void TPM1_IRQHandler(void);
WEAK void TPM2_IRQHandler(void);
WEAK void RTWDOG1_IRQHandler(void);
WEAK void RTWDOG2_IRQHandler(void);
WEAK void Reserved56_IRQHandler(void);
WEAK void PDM_HWVAD_EVENT_IRQHandler(void);
WEAK void PDM_HWVAD_ERROR_IRQHandler(void);
WEAK void PDM_EVENT_IRQHandler(void);
WEAK void PDM_ERROR_IRQHandler(void);
WEAK void SAI1_IRQHandler(void);
WEAK void M33_PS_IRQHandler(void);
WEAK void M33_TCM_ECC_IRQHandler(void);
WEAK void M33_TCM_ERROR_IRQHandler(void);
WEAK void M7_TCM_ECC_IRQHandler(void);
WEAK void M7_TCM_ERROR_IRQHandler(void);
WEAK void CAN2_IRQHandler(void);
WEAK void CAN2_ERROR_IRQHandler(void);
WEAK void FLEXIO1_IRQHandler(void);
WEAK void FLEXIO2_IRQHandler(void);
WEAK void FLEXSPI1_IRQHandler(void);
WEAK void FLEXSPI2_IRQHandler(void);
WEAK void GPIO2_0_IRQHandler(void);
WEAK void GPIO2_1_IRQHandler(void);
WEAK void GPIO3_0_IRQHandler(void);
WEAK void GPIO3_1_IRQHandler(void);
WEAK void I3C2_IRQHandler(void);
WEAK void LPI2C3_IRQHandler(void);
WEAK void LPI2C4_IRQHandler(void);
WEAK void LPIT2_IRQHandler(void);
WEAK void LPSPI3_IRQHandler(void);
WEAK void LPSPI4_IRQHandler(void);
WEAK void LPTMR2_IRQHandler(void);
WEAK void LPUART3_IRQHandler(void);
WEAK void LPUART4_IRQHandler(void);
WEAK void LPUART5_IRQHandler(void);
WEAK void LPUART6_IRQHandler(void);
WEAK void ASRC_IRQHandler(void);
WEAK void BBNSM_IRQHandler(void);
WEAK void SYS_CTRL1_IRQHandler(void);
WEAK void TPM3_IRQHandler(void);
WEAK void TPM4_IRQHandler(void);
WEAK void TPM5_IRQHandler(void);
WEAK void TPM6_IRQHandler(void);
WEAK void RTWDOG3_IRQHandler(void);
WEAK void RTWDOG4_IRQHandler(void);
WEAK void RTWDOG5_IRQHandler(void);
WEAK void Reserved98_IRQHandler(void);
WEAK void TEMPSENSOR_IRQHandler(void);
WEAK void Reserved100_IRQHandler(void);
WEAK void LDO_1P8_IRQHandler(void);
WEAK void USDHC1_IRQHandler(void);
WEAK void USDHC2_IRQHandler(void);
WEAK void Reserved104_IRQHandler(void);
WEAK void TMR2_IRQHandler(void);
WEAK void LDO_1P0_IRQHandler(void);
WEAK void MECC1_IRQHandler(void);
WEAK void MECC2_IRQHandler(void);
WEAK void ADC1_IRQHandler(void);
WEAK void DMA_ERROR_IRQHandler(void);
WEAK void DMA_0_IRQHandler(void);
WEAK void DMA_1_IRQHandler(void);
WEAK void DMA_2_IRQHandler(void);
WEAK void DMA_3_IRQHandler(void);
WEAK void DMA_4_IRQHandler(void);
WEAK void DMA_5_IRQHandler(void);
WEAK void DMA_6_IRQHandler(void);
WEAK void DMA_7_IRQHandler(void);
WEAK void DMA_8_IRQHandler(void);
WEAK void DMA_9_IRQHandler(void);
WEAK void DMA_10_IRQHandler(void);
WEAK void DMA_11_IRQHandler(void);
WEAK void DMA_12_IRQHandler(void);
WEAK void DMA_13_IRQHandler(void);
WEAK void DMA_14_IRQHandler(void);
WEAK void DMA_15_IRQHandler(void);
WEAK void DMA_16_IRQHandler(void);
WEAK void DMA_17_IRQHandler(void);
WEAK void DMA_18_IRQHandler(void);
WEAK void DMA_19_IRQHandler(void);
WEAK void DMA_20_IRQHandler(void);
WEAK void DMA_21_IRQHandler(void);
WEAK void DMA_22_IRQHandler(void);
WEAK void DMA_23_IRQHandler(void);
WEAK void DMA_24_IRQHandler(void);
WEAK void DMA_25_IRQHandler(void);
WEAK void DMA_26_IRQHandler(void);
WEAK void DMA_27_IRQHandler(void);
WEAK void DMA_28_IRQHandler(void);
WEAK void DMA_29_IRQHandler(void);
WEAK void DMA_30_IRQHandler(void);
WEAK void DMA_31_IRQHandler(void);
WEAK void DMA4_ERROR_IRQHandler(void);
WEAK void DMA4_0_1_IRQHandler(void);
WEAK void DMA4_2_3_IRQHandler(void);
WEAK void DMA4_4_5_IRQHandler(void);
WEAK void DMA4_6_7_IRQHandler(void);
WEAK void DMA4_8_9_IRQHandler(void);
WEAK void DMA4_10_11_IRQHandler(void);
WEAK void DMA4_12_13_IRQHandler(void);
WEAK void DMA4_14_15_IRQHandler(void);
WEAK void DMA4_16_17_IRQHandler(void);
WEAK void DMA4_18_19_IRQHandler(void);
WEAK void DMA4_20_21_IRQHandler(void);
WEAK void DMA4_22_23_IRQHandler(void);
WEAK void DMA4_24_25_IRQHandler(void);
WEAK void DMA4_26_27_IRQHandler(void);
WEAK void DMA4_28_29_IRQHandler(void);
WEAK void DMA4_30_31_IRQHandler(void);
WEAK void Reserved160_IRQHandler(void);
WEAK void Reserved161_IRQHandler(void);
WEAK void Reserved162_IRQHandler(void);
WEAK void Reserved163_IRQHandler(void);
WEAK void Reserved164_IRQHandler(void);
WEAK void Reserved165_IRQHandler(void);
WEAK void Reserved166_IRQHandler(void);
WEAK void Reserved167_IRQHandler(void);
WEAK void LPI2C5_IRQHandler(void);
WEAK void LPI2C6_IRQHandler(void);
WEAK void SAI4_IRQHandler(void);
WEAK void SPDIF_IRQHandler(void);
WEAK void LPUART9_IRQHandler(void);
WEAK void LPUART10_IRQHandler(void);
WEAK void LPUART11_IRQHandler(void);
WEAK void LPUART12_IRQHandler(void);
WEAK void Reserved176_IRQHandler(void);
WEAK void Reserved177_IRQHandler(void);
WEAK void Reserved178_IRQHandler(void);
WEAK void Reserved179_IRQHandler(void);
WEAK void TMR3_IRQHandler(void);
WEAK void TMR4_IRQHandler(void);
WEAK void M33_SYSRESET_REQ_IRQHandler(void);
WEAK void M33_LOCKUP_IRQHandler(void);
WEAK void M7_SYSRESET_REQ_IRQHandler(void);
WEAK void M7_LOCKUP_IRQHandler(void);
WEAK void PWM2_FAULT_IRQHandler(void);
WEAK void PWM2_0_IRQHandler(void);
WEAK void PWM2_1_IRQHandler(void);
WEAK void PWM2_2_IRQHandler(void);
WEAK void PWM2_3_IRQHandler(void);
WEAK void PWM3_FAULT_IRQHandler(void);
WEAK void PWM3_0_IRQHandler(void);
WEAK void PWM3_1_IRQHandler(void);
WEAK void PWM3_2_IRQHandler(void);
WEAK void PWM3_3_IRQHandler(void);
WEAK void PWM4_FAULT_IRQHandler(void);
WEAK void PWM4_0_IRQHandler(void);
WEAK void PWM4_1_IRQHandler(void);
WEAK void PWM4_2_IRQHandler(void);
WEAK void PWM4_3_IRQHandler(void);
WEAK void EQDC1_IRQHandler(void);
WEAK void EQDC2_IRQHandler(void);
WEAK void EQDC3_IRQHandler(void);
WEAK void EQDC4_IRQHandler(void);
WEAK void ADC2_IRQHandler(void);
WEAK void DCDC_IRQHandler(void);
WEAK void CAN3_IRQHandler(void);
WEAK void CAN3_ERROR_IRQHandler(void);
WEAK void DAC_IRQHandler(void);
WEAK void LPSPI5_IRQHandler(void);
WEAK void LPSPI6_IRQHandler(void);
WEAK void LPUART7_IRQHandler(void);
WEAK void LPUART8_IRQHandler(void);
WEAK void SAI2_IRQHandler(void);
WEAK void SAI3_IRQHandler(void);
WEAK void ACMP1_IRQHandler(void);
WEAK void ACMP2_IRQHandler(void);
WEAK void ACMP3_IRQHandler(void);
WEAK void ACMP4_IRQHandler(void);
WEAK void Reserved220_IRQHandler(void);
WEAK void Reserved221_IRQHandler(void);
WEAK void Reserved222_IRQHandler(void);
WEAK void Reserved223_IRQHandler(void);
WEAK void Reserved224_IRQHandler(void);
WEAK void GPT1_IRQHandler(void);
WEAK void GPT2_IRQHandler(void);
WEAK void KPP_IRQHandler(void);
WEAK void USBPHY1_IRQHandler(void);
WEAK void USBPHY2_IRQHandler(void);
WEAK void USB_OTG2_IRQHandler(void);
WEAK void USB_OTG1_IRQHandler(void);
WEAK void XSPI_SLV_IRQHandler(void);
WEAK void NETC_IRQHandler(void);
WEAK void MSGINTR1_IRQHandler(void);
WEAK void MSGINTR2_IRQHandler(void);
WEAK void MSGINTR3_IRQHandler(void);
WEAK void MSGINTR4_IRQHandler(void);
WEAK void MSGINTR5_IRQHandler(void);
WEAK void MSGINTR6_IRQHandler(void);
WEAK void SINC1_0_IRQHandler(void);
WEAK void SINC1_1_IRQHandler(void);
WEAK void SINC1_2_IRQHandler(void);
WEAK void SINC1_3_IRQHandler(void);
WEAK void SINC2_0_IRQHandler(void);
WEAK void SINC2_1_IRQHandler(void);
WEAK void SINC2_2_IRQHandler(void);
WEAK void SINC2_3_IRQHandler(void);
WEAK void GPIO4_0_IRQHandler(void);
WEAK void GPIO4_1_IRQHandler(void);
WEAK void GPIO5_0_IRQHandler(void);
WEAK void GPIO5_1_IRQHandler(void);
WEAK void GPIO6_0_IRQHandler(void);
WEAK void GPIO6_1_IRQHandler(void);
WEAK void ECAT_IRQHandler(void);

//*****************************************************************************
// Forward declaration of the driver IRQ handlers. These are aliased
// to the IntDefaultHandler, which is a 'forever' loop. When the driver
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions
//*****************************************************************************
void TMR1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DAP_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void M7_CTI_TRIGGER_OUTPUT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void M33_CTI_TRIGGER_OUTPUT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TMR5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TMR6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TMR7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TMR8_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CAN1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CAN1_ERROR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO1_0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO1_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void I3C1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPI2C1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPI2C2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPIT1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPSPI1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPSPI2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPTMR1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MU1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MU2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM1_FAULT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM1_0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM1_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM1_2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM1_3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SENTINEL_TRUST_MUA_RX_FULL_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SENTINEL_TRUST_MUA_TX_EMPTY_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SENTINEL_APPS_CORE_MUA_RX_FULL_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SENTINEL_APPS_CORE_MUA_TX_EMPTY_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SENTINEL_REALTIME_CORE_MUA_RX_FULL_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SENTINEL_REALTIME_CORE_MUA_TX_EMPTY_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SENTINEL_SECURE_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SENTINEL_NONSECURE_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TPM1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TPM2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void RTWDOG1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void RTWDOG2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved56_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PDM_HWVAD_EVENT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PDM_HWVAD_ERROR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PDM_EVENT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PDM_ERROR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SAI1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void M33_PS_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void M33_TCM_ECC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void M33_TCM_ERROR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void M7_TCM_ECC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void M7_TCM_ERROR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CAN2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CAN2_ERROR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXIO1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXIO2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXSPI1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXSPI2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO2_0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO2_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO3_0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO3_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void I3C2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPI2C3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPI2C4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPIT2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPSPI3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPSPI4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPTMR2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ASRC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void BBNSM_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SYS_CTRL1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TPM3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TPM4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TPM5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TPM6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void RTWDOG3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void RTWDOG4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void RTWDOG5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved98_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TEMPSENSOR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved100_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LDO_1P8_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USDHC1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USDHC2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved104_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TMR2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LDO_1P0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MECC1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MECC2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ADC1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_ERROR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_8_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_9_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_10_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_11_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_12_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_13_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_14_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_15_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_16_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_17_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_18_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_19_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_20_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_21_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_22_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_23_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_24_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_25_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_26_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_27_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_28_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_29_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_30_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_31_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_ERROR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_0_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_2_3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_4_5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_6_7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_8_9_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_10_11_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_12_13_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_14_15_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_16_17_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_18_19_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_20_21_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_22_23_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_24_25_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_26_27_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_28_29_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_30_31_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved160_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved161_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved162_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved163_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved164_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved165_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved166_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved167_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPI2C5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPI2C6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SAI4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SPDIF_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART9_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART10_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART11_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART12_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved176_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved177_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved178_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved179_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TMR3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TMR4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void M33_SYSRESET_REQ_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void M33_LOCKUP_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void M7_SYSRESET_REQ_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void M7_LOCKUP_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM2_FAULT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM2_0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM2_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM2_2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM2_3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM3_FAULT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM3_0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM3_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM3_2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM3_3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM4_FAULT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM4_0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM4_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM4_2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PWM4_3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EQDC1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EQDC2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EQDC3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EQDC4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ADC2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DCDC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CAN3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CAN3_ERROR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DAC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPSPI5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPSPI6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART8_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SAI2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SAI3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ACMP1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ACMP2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ACMP3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ACMP4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved220_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved221_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved222_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved223_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved224_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPT1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPT2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void KPP_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USBPHY1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USBPHY2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USB_OTG2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USB_OTG1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void XSPI_SLV_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void NETC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MSGINTR1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MSGINTR2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MSGINTR3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MSGINTR4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MSGINTR5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MSGINTR6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SINC1_0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SINC1_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SINC1_2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SINC1_3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SINC2_0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SINC2_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SINC2_2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SINC2_3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO4_0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO4_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO5_0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO5_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO6_0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO6_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ECAT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);

//*****************************************************************************
// The entry point for the application.
// __main() is the entry point for Redlib based applications
// main() is the entry point for Newlib based applications
//*****************************************************************************
#if defined (__REDLIB__)
extern void __main(void);
#endif
extern int main(void);

//*****************************************************************************
// External declaration for the pointer to the stack top from the Linker Script
//*****************************************************************************
extern void _vStackTop(void);
//*****************************************************************************
#if defined (__cplusplus)
} // extern "C"
#endif
//*****************************************************************************
// The vector table.
// This relies on the linker script to place at correct location in memory.
//*****************************************************************************

extern void (* const g_pfnVectors[])(void);
extern void * __Vectors __attribute__ ((alias ("g_pfnVectors")));

__attribute__ ((used, section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    // Core Level - CM7
    &_vStackTop,                       // The initial stack pointer
    ResetISR,                          // The reset handler
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved

    // Chip Level - MIMXRT1189_cm7
    TMR1_IRQHandler,                          // 16 : TMR1 interrupt
    DAP_IRQHandler,                          // 17 : DAP interrupt
    M7_CTI_TRIGGER_OUTPUT_IRQHandler,                // 18 : CTI trigger outputs from CM7
    M33_CTI_TRIGGER_OUTPUT_IRQHandler,               // 19 : CTI trigger outputs from CM33
    TMR5_IRQHandler,                          // 20 : TMR5 interrupt
    TMR6_IRQHandler,                          // 21 : TMR6 interrupt
    TMR7_IRQHandler,                          // 22 : TMR7 interrupt
    TMR8_IRQHandler,                          // 23 : TMR8 interrupt
    CAN1_IRQHandler,                          // 24 : CAN1 interrupt
    CAN1_ERROR_IRQHandler,                          // 25 : CAN1 error interrupt
    GPIO1_0_IRQHandler,                          // 26 : GPIO1 interrupt 0
    GPIO1_1_IRQHandler,                          // 27 : GPIO1 interrupt 1
    I3C1_IRQHandler,                          // 28 : I3C1 interrupt
    LPI2C1_IRQHandler,                          // 29 : LPI2C1 interrupt
    LPI2C2_IRQHandler,                          // 30 : LPI2C2 interrupt
    LPIT1_IRQHandler,                          // 31 : LPIT1 interrupt
    LPSPI1_IRQHandler,                          // 32 : LPSPI1 interrupt
    LPSPI2_IRQHandler,                          // 33 : LPSPI2 interrupt
    LPTMR1_IRQHandler,                          // 34 : LPTMR1 interrupt
    LPUART1_IRQHandler,                          // 35 : LPUART1 interrupt
    LPUART2_IRQHandler,                          // 36 : LPUART2 interrupt
    MU1_IRQHandler,                          // 37 : MU1 interrupt
    MU2_IRQHandler,                          // 38 : MU2 interrupt
    PWM1_FAULT_IRQHandler,                          // 39 : PWM1 fault or reload error interrupt
    PWM1_0_IRQHandler,                          // 40 : PWM1 capture 0, compare 0, or reload 0 interrupt
    PWM1_1_IRQHandler,                          // 41 : PWM1 capture 1, compare 1, or reload 1 interrupt
    PWM1_2_IRQHandler,                          // 42 : PWM1 capture 2, compare 2, or reload 2 interrupt
    PWM1_3_IRQHandler,                          // 43 : PWM1 capture 3, compare 3, or reload 3 interrupt
    SENTINEL_TRUST_MUA_RX_FULL_IRQHandler,           // 44 : Sentinel Trust MUA RX full interrupt
    SENTINEL_TRUST_MUA_TX_EMPTY_IRQHandler,          // 45 : Sentinel Trust MUA TX empty interrupt
    SENTINEL_APPS_CORE_MUA_RX_FULL_IRQHandler,       // 46 : Sentinel Apps Core MUA RX full interrupt
    SENTINEL_APPS_CORE_MUA_TX_EMPTY_IRQHandler,      // 47 : Sentinel Apps Core MUA TX empty interrupt
    SENTINEL_REALTIME_CORE_MUA_RX_FULL_IRQHandler,   // 48 : Sentinel Realtime Core MUA RX full interrupt
    SENTINEL_REALTIME_CORE_MUA_TX_EMPTY_IRQHandler,  // 49 : Sentinel Realtime Core MUA TX empty interrupt
    SENTINEL_SECURE_IRQHandler,                      // 50 : Sentinel secure interrupt
    SENTINEL_NONSECURE_IRQHandler,                   // 51 : Sentinel non-secure interrupt
    TPM1_IRQHandler,                          // 52 : TPM1 interrupt
    TPM2_IRQHandler,                          // 53 : TPM2 interrupt
    RTWDOG1_IRQHandler,                          // 54 : RTWDOG1 interrupt
    RTWDOG2_IRQHandler,                          // 55 : RTWDOG2 interrupt
    Reserved56_IRQHandler,                          // 56 : Reserved interrupt
    PDM_HWVAD_EVENT_IRQHandler,                      // 57 : HWVAD event interrupt
    PDM_HWVAD_ERROR_IRQHandler,                      // 58 : HWVAD error interrupt
    PDM_EVENT_IRQHandler,                          // 59 : PDM event interrupt
    PDM_ERROR_IRQHandler,                          // 60 : PDM error interrupt
    SAI1_IRQHandler,                          // 61 : SAI interrupt
    M33_PS_IRQHandler,                          // 62 : M33 PS Tag/Data Parity Error
    M33_TCM_ECC_IRQHandler,                          // 63 : M33 TCM ECC interrupt
    M33_TCM_ERROR_IRQHandler,                        // 64 : M33 TCM Error interrupt
    M7_TCM_ECC_IRQHandler,                          // 65 : M7 TCM ECC interrupt
    M7_TCM_ERROR_IRQHandler,                         // 66 : M7 TCM Error interrupt
    CAN2_IRQHandler,                          // 67 : CAN2 interrupt
    CAN2_ERROR_IRQHandler,                          // 68 : CAN2 error interrupt
    FLEXIO1_IRQHandler,                          // 69 : FLEXIO1 interrupt
    FLEXIO2_IRQHandler,                          // 70 : FLEXIO2 interrupt
    FLEXSPI1_IRQHandler,                          // 71 : FLEXSPI1 interrupt
    FLEXSPI2_IRQHandler,                          // 72 : FLEXSPI2 interrupt
    GPIO2_0_IRQHandler,                          // 73 : GPIO2 interrupt 0
    GPIO2_1_IRQHandler,                          // 74 : GPIO2 interrupt 1
    GPIO3_0_IRQHandler,                          // 75 : GPIO3 interrupt 0
    GPIO3_1_IRQHandler,                          // 76 : GPIO3 interrupt 1
    I3C2_IRQHandler,                          // 77 : I3C2 interrupt
    LPI2C3_IRQHandler,                          // 78 : LPI2C3 interrupt
    LPI2C4_IRQHandler,                          // 79 : LPI2C4 interrput
    LPIT2_IRQHandler,                          // 80 : LPIT2 interrupt
    LPSPI3_IRQHandler,                          // 81 : LPSPI3 interrupt
    LPSPI4_IRQHandler,                          // 82 : LPSPI4 interrupt
    LPTMR2_IRQHandler,                          // 83 : LPTMR2 interrupt
    LPUART3_IRQHandler,                          // 84 : LPUART3 interrupt
    LPUART4_IRQHandler,                          // 85 : LPUART4 interrupt
    LPUART5_IRQHandler,                          // 86 : LPUART5 interrupt
    LPUART6_IRQHandler,                          // 87 : LPUART6 interrupt
    ASRC_IRQHandler,                          // 88 : ASRC interrupt
    BBNSM_IRQHandler,                          // 89 : BBNSM iterrupt
    SYS_CTRL1_IRQHandler,                          // 90 : System Counter compare interrupt 0 and 1
    TPM3_IRQHandler,                          // 91 : TPM3 interrupt
    TPM4_IRQHandler,                          // 92 : TPM4 interrupt
    TPM5_IRQHandler,                          // 93 : TPM5 interrupt
    TPM6_IRQHandler,                          // 94 : TPM6 interrupt
    RTWDOG3_IRQHandler,                          // 95 : RTWDOG3 interrupt
    RTWDOG4_IRQHandler,                          // 96 : RTWDOG4 interrupt
    RTWDOG5_IRQHandler,                          // 97 : RTWDOG5 interrupt
    Reserved98_IRQHandler,                          // 98 : Reserved interrupt
    TEMPSENSOR_IRQHandler,                          // 99 : TempSensor alarm
    Reserved100_IRQHandler,                          // 100: Reserved interrupt
    LDO_1P8_IRQHandler,                          // 101: LDO 1P8 interrupt
    USDHC1_IRQHandler,                          // 102: USDHC1
    USDHC2_IRQHandler,                          // 103: USDHC2
    Reserved104_IRQHandler,                          // 104: Reserved interrupt
    TMR2_IRQHandler,                          // 105: TMR2 interrupt
    LDO_1P0_IRQHandler,                          // 106: LDO 1P0 interrupt
    MECC1_IRQHandler,                          // 107: MECC1 interrupt
    MECC2_IRQHandler,                          // 108: MECC2 interrupt
    ADC1_IRQHandler,                          // 109: ADC1 interrupt
    DMA_ERROR_IRQHandler,                          // 110: DMA error interrupt
    DMA_0_IRQHandler,                          // 111: DMA channel 0 interrupt
    DMA_1_IRQHandler,                          // 112: DMA channel 1 interrupt
    DMA_2_IRQHandler,                          // 113: DMA channel 2 interrupt
    DMA_3_IRQHandler,                          // 114: DMA channel 3 interrupt
    DMA_4_IRQHandler,                          // 115: DMA channel 4 interrupt
    DMA_5_IRQHandler,                          // 116: DMA channel 5 interrupt
    DMA_6_IRQHandler,                          // 117: DMA channel 6 interrupt
    DMA_7_IRQHandler,                          // 118: DMA channel 7 interrupt
    DMA_8_IRQHandler,                          // 119: DMA channel 8 interrupt
    DMA_9_IRQHandler,                          // 120: DMA channel 9 interrupt
    DMA_10_IRQHandler,                          // 121: DMA channel 10 interrupt
    DMA_11_IRQHandler,                          // 122: DMA channel 11 interrupt
    DMA_12_IRQHandler,                          // 123: DMA channel 12 interrupt
    DMA_13_IRQHandler,                          // 124: DMA channel 13 interrupt
    DMA_14_IRQHandler,                          // 125: DMA channel 14 interrupt
    DMA_15_IRQHandler,                          // 126: DMA channel 15 interrupt
    DMA_16_IRQHandler,                          // 127: DMA channel 16 interrupt
    DMA_17_IRQHandler,                          // 128: DMA channel 17 interrupt
    DMA_18_IRQHandler,                          // 129: DMA channel 18 interrupt
    DMA_19_IRQHandler,                          // 130: DMA channel 19 interrupt
    DMA_20_IRQHandler,                          // 131: DMA channel 20 interrupt
    DMA_21_IRQHandler,                          // 132: DMA channel 21 interrupt
    DMA_22_IRQHandler,                          // 133: DMA channel 22 interrupt
    DMA_23_IRQHandler,                          // 134: DMA channel 23 interrupt
    DMA_24_IRQHandler,                          // 135: DMA channel 24 interrupt
    DMA_25_IRQHandler,                          // 136: DMA channel 25 interrupt
    DMA_26_IRQHandler,                          // 137: DMA channel 26 interrupt
    DMA_27_IRQHandler,                          // 138: DMA channel 27 interrupt
    DMA_28_IRQHandler,                          // 139: DMA channel 28 interrupt
    DMA_29_IRQHandler,                          // 140: DMA channel 29 interrupt
    DMA_30_IRQHandler,                          // 141: DMA channel 30 interrupt
    DMA_31_IRQHandler,                          // 142: DMA channel 31 interrupt
    DMA4_ERROR_IRQHandler,                          // 143: DMA error interrupt
    DMA4_0_1_IRQHandler,                          // 144: DMA channel 0/1 interrupt
    DMA4_2_3_IRQHandler,                          // 145: DMA channel 2/3 interrupt
    DMA4_4_5_IRQHandler,                          // 146: DMA channel 4/5 interrupt
    DMA4_6_7_IRQHandler,                          // 147: DMA channel 6/7 interrupt
    DMA4_8_9_IRQHandler,                          // 148: DMA channel 8/9 interrupt
    DMA4_10_11_IRQHandler,                          // 149: DMA channel 10/11 interrupt
    DMA4_12_13_IRQHandler,                          // 150: DMA channel 12/13 interrupt
    DMA4_14_15_IRQHandler,                          // 151: DMA channel 14/15 interrupt
    DMA4_16_17_IRQHandler,                          // 152: DMA channel 16/17 interrupt
    DMA4_18_19_IRQHandler,                          // 153: DMA channel 18/19 interrupt
    DMA4_20_21_IRQHandler,                          // 154: DMA channel 20/21 interrupt
    DMA4_22_23_IRQHandler,                          // 155: DMA channel 22/23 interrupt
    DMA4_24_25_IRQHandler,                          // 156: DMA channel 24/25 interrupt
    DMA4_26_27_IRQHandler,                          // 157: DMA channel 26/27 interrupt
    DMA4_28_29_IRQHandler,                          // 158: DMA channel 28/29 interrupt
    DMA4_30_31_IRQHandler,                          // 159: DMA channel 30/31 interrupt
    Reserved160_IRQHandler,                          // 160: Reserved interrupt
    Reserved161_IRQHandler,                          // 161: Reserved interrupt
    Reserved162_IRQHandler,                          // 162: Reserved interrupt
    Reserved163_IRQHandler,                          // 163: Reserved interrupt
    Reserved164_IRQHandler,                          // 164: Reserved interrupt
    Reserved165_IRQHandler,                          // 165: Reserved interrupt
    Reserved166_IRQHandler,                          // 166: Reserved interrupt
    Reserved167_IRQHandler,                          // 167: Reserved interrupt
    LPI2C5_IRQHandler,                          // 168: LPI2C5 interrupt
    LPI2C6_IRQHandler,                          // 169: LPI2C6 interrupt
    SAI4_IRQHandler,                          // 170: SAI4 interrupt
    SPDIF_IRQHandler,                          // 171: SPDIF interrupt
    LPUART9_IRQHandler,                          // 172: LPUART9 interrupt
    LPUART10_IRQHandler,                          // 173: LPUART10 interrupt
    LPUART11_IRQHandler,                          // 174: LPUART11 interrupt
    LPUART12_IRQHandler,                          // 175: LPUART12 interrupt
    Reserved176_IRQHandler,                          // 176: Reserved interrupt
    Reserved177_IRQHandler,                          // 177: Reserved interrupt
    Reserved178_IRQHandler,                          // 178: Reserved interrupt
    Reserved179_IRQHandler,                          // 179: Reserved interrupt
    TMR3_IRQHandler,                          // 180: TMR3 interrupt
    TMR4_IRQHandler,                          // 181: TMR4 interrupt
    M33_SYSRESET_REQ_IRQHandler,                     // 182: CM33 SYSREQRST SRC reset source
    M33_LOCKUP_IRQHandler,                          // 183: CM33 LOCKUP SRC reset source
    M7_SYSRESET_REQ_IRQHandler,                      // 184: CM33 SYSREQRST SRC reset source
    M7_LOCKUP_IRQHandler,                          // 185: CM33 LOCKUP SRC reset source
    PWM2_FAULT_IRQHandler,                          // 186: PWM2 fault or reload error interrupt
    PWM2_0_IRQHandler,                          // 187: PWM2 capture 0, compare 0, or reload 0 interrupt
    PWM2_1_IRQHandler,                          // 188: PWM2 capture 1, compare 1, or reload 1 interrupt
    PWM2_2_IRQHandler,                          // 189: PWM2 capture 2, compare 2, or reload 2 interrupt
    PWM2_3_IRQHandler,                          // 190: PWM2 capture 3, compare 3, or reload 3 interrupt
    PWM3_FAULT_IRQHandler,                          // 191: PWM3 fault or reload error interrupt
    PWM3_0_IRQHandler,                          // 192: PWM3 capture 0, compare 0, or reload 0 interrupt
    PWM3_1_IRQHandler,                          // 193: PWM3 capture 1, compare 1, or reload 1 interrupt
    PWM3_2_IRQHandler,                          // 194: PWM3 capture 2, compare 2, or reload 2 interrupt
    PWM3_3_IRQHandler,                          // 195: PWM3 capture 3, compare 3, or reload 3 interrupt
    PWM4_FAULT_IRQHandler,                          // 196: PWM4 fault or reload error interrupt
    PWM4_0_IRQHandler,                          // 197: PWM4 capture 0, compare 0, or reload 0 interrupt
    PWM4_1_IRQHandler,                          // 198: PWM4 capture 1, compare 1, or reload 1 interrupt
    PWM4_2_IRQHandler,                          // 199: PWM4 capture 2, compare 2, or reload 2 interrupt
    PWM4_3_IRQHandler,                          // 200: PWM4 capture 3, compare 3, or reload 3 interrupt
    EQDC1_IRQHandler,                          // 201: EQDC1 interrupt
    EQDC2_IRQHandler,                          // 202: EQDC2 interrupt
    EQDC3_IRQHandler,                          // 203: EQDC3 interrupt
    EQDC4_IRQHandler,                          // 204: EQDC4 interrupt
    ADC2_IRQHandler,                          // 205: ADC2 interrupt
    DCDC_IRQHandler,                          // 206: DCDC interrupt
    CAN3_IRQHandler,                          // 207: CAN3 interrupt
    CAN3_ERROR_IRQHandler,                          // 208: CAN3 error interrupt
    DAC_IRQHandler,                          // 209: DAC interrupt
    LPSPI5_IRQHandler,                          // 210: LPSPI5 interrupt
    LPSPI6_IRQHandler,                          // 211: LPSPI6 interrupt
    LPUART7_IRQHandler,                          // 212: LPUART7 interrupt
    LPUART8_IRQHandler,                          // 213: LPUART8 interrupt
    SAI2_IRQHandler,                          // 214: SAI2 interrupt
    SAI3_IRQHandler,                          // 215: SAI3 interrupt
    ACMP1_IRQHandler,                          // 216: CMP1 interrupt
    ACMP2_IRQHandler,                          // 217: CMP2 interrupt
    ACMP3_IRQHandler,                          // 218: CMP3 interrupt
    ACMP4_IRQHandler,                          // 219: CMP4 interrupt
    Reserved220_IRQHandler,                          // 220: Reserved interrupt
    Reserved221_IRQHandler,                          // 221: Reserved interrupt
    Reserved222_IRQHandler,                          // 222: Reserved interrupt
    Reserved223_IRQHandler,                          // 223: Reserved interrupt
    Reserved224_IRQHandler,                          // 224: Reserved interrupt
    GPT1_IRQHandler,                          // 225: GPT1 interrupt
    GPT2_IRQHandler,                          // 226: GPT2 interrupt
    KPP_IRQHandler,                          // 227: KPP interrupt
    USBPHY1_IRQHandler,                          // 228: USBPHY1 interrupt
    USBPHY2_IRQHandler,                          // 229: USBPHY2 interrupt
    USB_OTG2_IRQHandler,                          // 230: USBOTG2 interrupt
    USB_OTG1_IRQHandler,                          // 231: USBOTG1 interrupt
    XSPI_SLV_IRQHandler,                          // 232: XSPI_SLV interrupt
    NETC_IRQHandler,                          // 233: NETC interrupt
    MSGINTR1_IRQHandler,                          // 234: MSGINTR1 interrupt
    MSGINTR2_IRQHandler,                          // 235: MSGINTR2 interrupt
    MSGINTR3_IRQHandler,                          // 236: MSGINTR3 interrupt
    MSGINTR4_IRQHandler,                          // 237: MSGINTR4 interrupt
    MSGINTR5_IRQHandler,                          // 238: MSGINTR5 interrupt
    MSGINTR6_IRQHandler,                          // 239: MSGINTR6 interrupt
    SINC1_0_IRQHandler,                          // 240: SINC Filter Glue 1 channel 0
    SINC1_1_IRQHandler,                          // 241: SINC Filter Glue 1 channel 1
    SINC1_2_IRQHandler,                          // 242: SINC Filter Glue 1 channel 2
    SINC1_3_IRQHandler,                          // 243: SINC Filter Glue 1 channel 3
    SINC2_0_IRQHandler,                          // 244: SINC Filter Glue 2 channel 0
    SINC2_1_IRQHandler,                          // 245: SINC Filter Glue 2 channel 1
    SINC2_2_IRQHandler,                          // 246: SINC Filter Glue 2 channel 2
    SINC2_3_IRQHandler,                          // 247: SINC Filter Glue 2 channel 3
    GPIO4_0_IRQHandler,                          // 248: GPIO4 interrupt 0
    GPIO4_1_IRQHandler,                          // 249: GPIO4 interrupt 1
    GPIO5_0_IRQHandler,                          // 250: GPIO5 interrupt 0
    GPIO5_1_IRQHandler,                          // 251: GPIO5 interrupt 1
    GPIO6_0_IRQHandler,                          // 252: GPIO6 interrupt 0
    GPIO6_1_IRQHandler,                          // 253: GPIO6 interrupt 1
    ECAT_IRQHandler,                          // 254: ECAT interrupt
}; /* End of g_pfnVectors */

//*****************************************************************************
// Functions to carry out the initialization of RW and BSS data sections. These
// are written as separate functions rather than being inlined within the
// ResetISR() function in order to cope with MCUs with multiple banks of
// memory.
//*****************************************************************************
__attribute__ ((section(".after_vectors.init_data")))
void data_init(unsigned int romstart, unsigned int start, unsigned int len) {
    unsigned int *pulDest = (unsigned int*) start;
    unsigned int *pulSrc = (unsigned int*) romstart;
    unsigned int loop;
    for (loop = 0; loop < len; loop = loop + 4)
        *pulDest++ = *pulSrc++;
}

__attribute__ ((section(".after_vectors.init_bss")))
void bss_init(unsigned int start, unsigned int len) {
    unsigned int *pulDest = (unsigned int*) start;
    unsigned int loop;
    for (loop = 0; loop < len; loop = loop + 4)
        *pulDest++ = 0;
}

//*****************************************************************************
// The following symbols are constructs generated by the linker, indicating
// the location of various points in the "Global Section Table". This table is
// created by the linker via the Code Red managed linker script mechanism. It
// contains the load address, execution address and length of each RW data
// section and the execution and length of each BSS (zero initialized) section.
//*****************************************************************************
extern unsigned int __data_section_table;
extern unsigned int __data_section_table_end;
extern unsigned int __bss_section_table;
extern unsigned int __bss_section_table_end;

//*****************************************************************************
// Reset entry point for your code.
// Sets up a simple runtime environment and initializes the C/C++
// library.
//*****************************************************************************
__attribute__ ((naked, section(".after_vectors.reset")))
void ResetISR(void) {
    // Disable interrupts
    __asm volatile ("cpsid i");
    __asm volatile ("MSR MSP, %0" : : "r" (&_vStackTop) : );

#if defined (__USE_CMSIS)
// If __USE_CMSIS defined, then call CMSIS SystemInit code
    SystemInit();
#else
    // Disable Watchdog
    volatile unsigned int *WDOG1_WCR = (unsigned int *) 0x400B8000;
    *WDOG1_WCR = *WDOG1_WCR & ~(1 << 2);
    volatile unsigned int *WDOG2_WCR = (unsigned int *) 0x400D0000;
    *WDOG2_WCR = *WDOG2_WCR & ~(1 << 2);
    // Write watchdog update key to unlock
    *((volatile unsigned int *)0x424B0004) = 0xD928C520;
    // Set timeout value
    *((volatile unsigned int *)0x424B0008) = 0xFFFF;
    // Now disable watchdog via control register
    volatile unsigned int *RTWDOG_CS = (unsigned int *) 0x424B0000;
    *RTWDOG_CS = (*RTWDOG_CS & ~(1 << 7)) | (1 << 5);

#endif // (__USE_CMSIS)

    //
    // Copy the data sections from flash to SRAM.
    //
    unsigned int LoadAddr, ExeAddr, SectionLen;
    unsigned int *SectionTableAddr;

    // Load base address of Global Section Table
    SectionTableAddr = &__data_section_table;

    // Copy the data sections from flash to SRAM.
    while (SectionTableAddr < &__data_section_table_end) {
        LoadAddr = *SectionTableAddr++;
        ExeAddr = *SectionTableAddr++;
        SectionLen = *SectionTableAddr++;
        data_init(LoadAddr, ExeAddr, SectionLen);
    }

    // At this point, SectionTableAddr = &__bss_section_table;
    // Zero fill the bss segment
    while (SectionTableAddr < &__bss_section_table_end) {
        ExeAddr = *SectionTableAddr++;
        SectionLen = *SectionTableAddr++;
        bss_init(ExeAddr, SectionLen);
    }

#if !defined (__USE_CMSIS)
// Assume that if __USE_CMSIS defined, then CMSIS SystemInit code
// will setup the VTOR register

    // Check to see if we are running the code from a non-zero
    // address (eg RAM, external flash), in which case we need
    // to modify the VTOR register to tell the CPU that the
    // vector table is located at a non-0x0 address.
    unsigned int * pSCB_VTOR = (unsigned int *) 0xE000ED08;
    if ((unsigned int *)g_pfnVectors!=(unsigned int *) 0x00000000) {
        *pSCB_VTOR = (unsigned int)g_pfnVectors;
    }
#endif // (__USE_CMSIS)
#if defined (__cplusplus)
    //
    // Call C++ library initialisation
    //
    __libc_init_array();
#endif

    // Reenable interrupts
    __asm volatile ("cpsie i");

#if defined (__REDLIB__)
    // Call the Redlib library, which in turn calls main()
    __main();
#else
    main();
#endif

    //
    // main() shouldn't return, but if it does, we'll just enter an infinite loop
    //
    while (1) {
        ;
    }
}

//*****************************************************************************
// Default core exception handlers. Override the ones here by defining your own
// handler routines in your application code.
//*****************************************************************************
//*****************************************************************************
// Processor ends up here if an unexpected interrupt occurs or a specific
// handler is not present in the application code.
//*****************************************************************************
WEAK_AV void IntDefaultHandler(void)
{ while(1) {}
}

//*****************************************************************************
// Default application exception handlers. Override the ones here by defining
// your own handler routines in your application code. These routines call
// driver exception handlers or IntDefaultHandler() if no driver exception
// handler is included.
//*****************************************************************************
WEAK void TMR1_IRQHandler(void)
{   TMR1_DriverIRQHandler();
}

WEAK void DAP_IRQHandler(void)
{   DAP_DriverIRQHandler();
}

WEAK void M7_CTI_TRIGGER_OUTPUT_IRQHandler(void)
{   M7_CTI_TRIGGER_OUTPUT_DriverIRQHandler();
}

WEAK void M33_CTI_TRIGGER_OUTPUT_IRQHandler(void)
{   M33_CTI_TRIGGER_OUTPUT_DriverIRQHandler();
}

WEAK void TMR5_IRQHandler(void)
{   TMR5_DriverIRQHandler();
}

WEAK void TMR6_IRQHandler(void)
{   TMR6_DriverIRQHandler();
}

WEAK void TMR7_IRQHandler(void)
{   TMR7_DriverIRQHandler();
}

WEAK void TMR8_IRQHandler(void)
{   TMR8_DriverIRQHandler();
}

WEAK void CAN1_IRQHandler(void)
{   CAN1_DriverIRQHandler();
}

WEAK void CAN1_ERROR_IRQHandler(void)
{   CAN1_ERROR_DriverIRQHandler();
}

WEAK void GPIO1_0_IRQHandler(void)
{   GPIO1_0_DriverIRQHandler();
}

WEAK void GPIO1_1_IRQHandler(void)
{   GPIO1_1_DriverIRQHandler();
}

WEAK void I3C1_IRQHandler(void)
{   I3C1_DriverIRQHandler();
}

WEAK void LPI2C1_IRQHandler(void)
{   LPI2C1_DriverIRQHandler();
}

WEAK void LPI2C2_IRQHandler(void)
{   LPI2C2_DriverIRQHandler();
}

WEAK void LPIT1_IRQHandler(void)
{   LPIT1_DriverIRQHandler();
}

WEAK void LPSPI1_IRQHandler(void)
{   LPSPI1_DriverIRQHandler();
}

WEAK void LPSPI2_IRQHandler(void)
{   LPSPI2_DriverIRQHandler();
}

WEAK void LPTMR1_IRQHandler(void)
{   LPTMR1_DriverIRQHandler();
}

WEAK void LPUART1_IRQHandler(void)
{   LPUART1_DriverIRQHandler();
}

WEAK void LPUART2_IRQHandler(void)
{   LPUART2_DriverIRQHandler();
}

WEAK void MU1_IRQHandler(void)
{   MU1_DriverIRQHandler();
}

WEAK void MU2_IRQHandler(void)
{   MU2_DriverIRQHandler();
}

WEAK void PWM1_FAULT_IRQHandler(void)
{   PWM1_FAULT_DriverIRQHandler();
}

WEAK void PWM1_0_IRQHandler(void)
{   PWM1_0_DriverIRQHandler();
}

WEAK void PWM1_1_IRQHandler(void)
{   PWM1_1_DriverIRQHandler();
}

WEAK void PWM1_2_IRQHandler(void)
{   PWM1_2_DriverIRQHandler();
}

WEAK void PWM1_3_IRQHandler(void)
{   PWM1_3_DriverIRQHandler();
}

WEAK void SENTINEL_TRUST_MUA_RX_FULL_IRQHandler(void)
{   SENTINEL_TRUST_MUA_RX_FULL_DriverIRQHandler();
}

WEAK void SENTINEL_TRUST_MUA_TX_EMPTY_IRQHandler(void)
{   SENTINEL_TRUST_MUA_TX_EMPTY_DriverIRQHandler();
}

WEAK void SENTINEL_APPS_CORE_MUA_RX_FULL_IRQHandler(void)
{   SENTINEL_APPS_CORE_MUA_RX_FULL_DriverIRQHandler();
}

WEAK void SENTINEL_APPS_CORE_MUA_TX_EMPTY_IRQHandler(void)
{   SENTINEL_APPS_CORE_MUA_TX_EMPTY_DriverIRQHandler();
}

WEAK void SENTINEL_REALTIME_CORE_MUA_RX_FULL_IRQHandler(void)
{   SENTINEL_REALTIME_CORE_MUA_RX_FULL_DriverIRQHandler();
}

WEAK void SENTINEL_REALTIME_CORE_MUA_TX_EMPTY_IRQHandler(void)
{   SENTINEL_REALTIME_CORE_MUA_TX_EMPTY_DriverIRQHandler();
}

WEAK void SENTINEL_SECURE_IRQHandler(void)
{   SENTINEL_SECURE_DriverIRQHandler();
}

WEAK void SENTINEL_NONSECURE_IRQHandler(void)
{   SENTINEL_NONSECURE_DriverIRQHandler();
}

WEAK void TPM1_IRQHandler(void)
{   TPM1_DriverIRQHandler();
}

WEAK void TPM2_IRQHandler(void)
{   TPM2_DriverIRQHandler();
}

WEAK void RTWDOG1_IRQHandler(void)
{   RTWDOG1_DriverIRQHandler();
}

WEAK void RTWDOG2_IRQHandler(void)
{   RTWDOG2_DriverIRQHandler();
}

WEAK void Reserved56_IRQHandler(void)
{   Reserved56_DriverIRQHandler();
}

WEAK void PDM_HWVAD_EVENT_IRQHandler(void)
{   PDM_HWVAD_EVENT_DriverIRQHandler();
}

WEAK void PDM_HWVAD_ERROR_IRQHandler(void)
{   PDM_HWVAD_ERROR_DriverIRQHandler();
}

WEAK void PDM_EVENT_IRQHandler(void)
{   PDM_EVENT_DriverIRQHandler();
}

WEAK void PDM_ERROR_IRQHandler(void)
{   PDM_ERROR_DriverIRQHandler();
}

WEAK void SAI1_IRQHandler(void)
{   SAI1_DriverIRQHandler();
}

WEAK void M33_PS_IRQHandler(void)
{   M33_PS_DriverIRQHandler();
}

WEAK void M33_TCM_ECC_IRQHandler(void)
{   M33_TCM_ECC_DriverIRQHandler();
}

WEAK void M33_TCM_ERROR_IRQHandler(void)
{   M33_TCM_ERROR_DriverIRQHandler();
}

WEAK void M7_TCM_ECC_IRQHandler(void)
{   M7_TCM_ECC_DriverIRQHandler();
}

WEAK void M7_TCM_ERROR_IRQHandler(void)
{   M7_TCM_ERROR_DriverIRQHandler();
}

WEAK void CAN2_IRQHandler(void)
{   CAN2_DriverIRQHandler();
}

WEAK void CAN2_ERROR_IRQHandler(void)
{   CAN2_ERROR_DriverIRQHandler();
}

WEAK void FLEXIO1_IRQHandler(void)
{   FLEXIO1_DriverIRQHandler();
}

WEAK void FLEXIO2_IRQHandler(void)
{   FLEXIO2_DriverIRQHandler();
}

WEAK void FLEXSPI1_IRQHandler(void)
{   FLEXSPI1_DriverIRQHandler();
}

WEAK void FLEXSPI2_IRQHandler(void)
{   FLEXSPI2_DriverIRQHandler();
}

WEAK void GPIO2_0_IRQHandler(void)
{   GPIO2_0_DriverIRQHandler();
}

WEAK void GPIO2_1_IRQHandler(void)
{   GPIO2_1_DriverIRQHandler();
}

WEAK void GPIO3_0_IRQHandler(void)
{   GPIO3_0_DriverIRQHandler();
}

WEAK void GPIO3_1_IRQHandler(void)
{   GPIO3_1_DriverIRQHandler();
}

WEAK void I3C2_IRQHandler(void)
{   I3C2_DriverIRQHandler();
}

WEAK void LPI2C3_IRQHandler(void)
{   LPI2C3_DriverIRQHandler();
}

WEAK void LPI2C4_IRQHandler(void)
{   LPI2C4_DriverIRQHandler();
}

WEAK void LPIT2_IRQHandler(void)
{   LPIT2_DriverIRQHandler();
}

WEAK void LPSPI3_IRQHandler(void)
{   LPSPI3_DriverIRQHandler();
}

WEAK void LPSPI4_IRQHandler(void)
{   LPSPI4_DriverIRQHandler();
}

WEAK void LPTMR2_IRQHandler(void)
{   LPTMR2_DriverIRQHandler();
}

WEAK void LPUART3_IRQHandler(void)
{   LPUART3_DriverIRQHandler();
}

WEAK void LPUART4_IRQHandler(void)
{   LPUART4_DriverIRQHandler();
}

WEAK void LPUART5_IRQHandler(void)
{   LPUART5_DriverIRQHandler();
}

WEAK void LPUART6_IRQHandler(void)
{   LPUART6_DriverIRQHandler();
}

WEAK void ASRC_IRQHandler(void)
{   ASRC_DriverIRQHandler();
}

WEAK void BBNSM_IRQHandler(void)
{   BBNSM_DriverIRQHandler();
}

WEAK void SYS_CTRL1_IRQHandler(void)
{   SYS_CTRL1_DriverIRQHandler();
}

WEAK void TPM3_IRQHandler(void)
{   TPM3_DriverIRQHandler();
}

WEAK void TPM4_IRQHandler(void)
{   TPM4_DriverIRQHandler();
}

WEAK void TPM5_IRQHandler(void)
{   TPM5_DriverIRQHandler();
}

WEAK void TPM6_IRQHandler(void)
{   TPM6_DriverIRQHandler();
}

WEAK void RTWDOG3_IRQHandler(void)
{   RTWDOG3_DriverIRQHandler();
}

WEAK void RTWDOG4_IRQHandler(void)
{   RTWDOG4_DriverIRQHandler();
}

WEAK void RTWDOG5_IRQHandler(void)
{   RTWDOG5_DriverIRQHandler();
}

WEAK void Reserved98_IRQHandler(void)
{   Reserved98_DriverIRQHandler();
}

WEAK void TEMPSENSOR_IRQHandler(void)
{   TEMPSENSOR_DriverIRQHandler();
}

WEAK void Reserved100_IRQHandler(void)
{   Reserved100_DriverIRQHandler();
}

WEAK void LDO_1P8_IRQHandler(void)
{   LDO_1P8_DriverIRQHandler();
}

WEAK void USDHC1_IRQHandler(void)
{   USDHC1_DriverIRQHandler();
}

WEAK void USDHC2_IRQHandler(void)
{   USDHC2_DriverIRQHandler();
}

WEAK void Reserved104_IRQHandler(void)
{   Reserved104_DriverIRQHandler();
}

WEAK void TMR2_IRQHandler(void)
{   TMR2_DriverIRQHandler();
}

WEAK void LDO_1P0_IRQHandler(void)
{   LDO_1P0_DriverIRQHandler();
}

WEAK void MECC1_IRQHandler(void)
{   MECC1_DriverIRQHandler();
}

WEAK void MECC2_IRQHandler(void)
{   MECC2_DriverIRQHandler();
}

WEAK void ADC1_IRQHandler(void)
{   ADC1_DriverIRQHandler();
}

WEAK void DMA_ERROR_IRQHandler(void)
{   DMA_ERROR_DriverIRQHandler();
}

WEAK void DMA_0_IRQHandler(void)
{   DMA_0_DriverIRQHandler();
}

WEAK void DMA_1_IRQHandler(void)
{   DMA_1_DriverIRQHandler();
}

WEAK void DMA_2_IRQHandler(void)
{   DMA_2_DriverIRQHandler();
}

WEAK void DMA_3_IRQHandler(void)
{   DMA_3_DriverIRQHandler();
}

WEAK void DMA_4_IRQHandler(void)
{   DMA_4_DriverIRQHandler();
}

WEAK void DMA_5_IRQHandler(void)
{   DMA_5_DriverIRQHandler();
}

WEAK void DMA_6_IRQHandler(void)
{   DMA_6_DriverIRQHandler();
}

WEAK void DMA_7_IRQHandler(void)
{   DMA_7_DriverIRQHandler();
}

WEAK void DMA_8_IRQHandler(void)
{   DMA_8_DriverIRQHandler();
}

WEAK void DMA_9_IRQHandler(void)
{   DMA_9_DriverIRQHandler();
}

WEAK void DMA_10_IRQHandler(void)
{   DMA_10_DriverIRQHandler();
}

WEAK void DMA_11_IRQHandler(void)
{   DMA_11_DriverIRQHandler();
}

WEAK void DMA_12_IRQHandler(void)
{   DMA_12_DriverIRQHandler();
}

WEAK void DMA_13_IRQHandler(void)
{   DMA_13_DriverIRQHandler();
}

WEAK void DMA_14_IRQHandler(void)
{   DMA_14_DriverIRQHandler();
}

WEAK void DMA_15_IRQHandler(void)
{   DMA_15_DriverIRQHandler();
}

WEAK void DMA_16_IRQHandler(void)
{   DMA_16_DriverIRQHandler();
}

WEAK void DMA_17_IRQHandler(void)
{   DMA_17_DriverIRQHandler();
}

WEAK void DMA_18_IRQHandler(void)
{   DMA_18_DriverIRQHandler();
}

WEAK void DMA_19_IRQHandler(void)
{   DMA_19_DriverIRQHandler();
}

WEAK void DMA_20_IRQHandler(void)
{   DMA_20_DriverIRQHandler();
}

WEAK void DMA_21_IRQHandler(void)
{   DMA_21_DriverIRQHandler();
}

WEAK void DMA_22_IRQHandler(void)
{   DMA_22_DriverIRQHandler();
}

WEAK void DMA_23_IRQHandler(void)
{   DMA_23_DriverIRQHandler();
}

WEAK void DMA_24_IRQHandler(void)
{   DMA_24_DriverIRQHandler();
}

WEAK void DMA_25_IRQHandler(void)
{   DMA_25_DriverIRQHandler();
}

WEAK void DMA_26_IRQHandler(void)
{   DMA_26_DriverIRQHandler();
}

WEAK void DMA_27_IRQHandler(void)
{   DMA_27_DriverIRQHandler();
}

WEAK void DMA_28_IRQHandler(void)
{   DMA_28_DriverIRQHandler();
}

WEAK void DMA_29_IRQHandler(void)
{   DMA_29_DriverIRQHandler();
}

WEAK void DMA_30_IRQHandler(void)
{   DMA_30_DriverIRQHandler();
}

WEAK void DMA_31_IRQHandler(void)
{   DMA_31_DriverIRQHandler();
}

WEAK void DMA4_ERROR_IRQHandler(void)
{   DMA4_ERROR_DriverIRQHandler();
}

WEAK void DMA4_0_1_IRQHandler(void)
{   DMA4_0_1_DriverIRQHandler();
}

WEAK void DMA4_2_3_IRQHandler(void)
{   DMA4_2_3_DriverIRQHandler();
}

WEAK void DMA4_4_5_IRQHandler(void)
{   DMA4_4_5_DriverIRQHandler();
}

WEAK void DMA4_6_7_IRQHandler(void)
{   DMA4_6_7_DriverIRQHandler();
}

WEAK void DMA4_8_9_IRQHandler(void)
{   DMA4_8_9_DriverIRQHandler();
}

WEAK void DMA4_10_11_IRQHandler(void)
{   DMA4_10_11_DriverIRQHandler();
}

WEAK void DMA4_12_13_IRQHandler(void)
{   DMA4_12_13_DriverIRQHandler();
}

WEAK void DMA4_14_15_IRQHandler(void)
{   DMA4_14_15_DriverIRQHandler();
}

WEAK void DMA4_16_17_IRQHandler(void)
{   DMA4_16_17_DriverIRQHandler();
}

WEAK void DMA4_18_19_IRQHandler(void)
{   DMA4_18_19_DriverIRQHandler();
}

WEAK void DMA4_20_21_IRQHandler(void)
{   DMA4_20_21_DriverIRQHandler();
}

WEAK void DMA4_22_23_IRQHandler(void)
{   DMA4_22_23_DriverIRQHandler();
}

WEAK void DMA4_24_25_IRQHandler(void)
{   DMA4_24_25_DriverIRQHandler();
}

WEAK void DMA4_26_27_IRQHandler(void)
{   DMA4_26_27_DriverIRQHandler();
}

WEAK void DMA4_28_29_IRQHandler(void)
{   DMA4_28_29_DriverIRQHandler();
}

WEAK void DMA4_30_31_IRQHandler(void)
{   DMA4_30_31_DriverIRQHandler();
}

WEAK void Reserved160_IRQHandler(void)
{   Reserved160_DriverIRQHandler();
}

WEAK void Reserved161_IRQHandler(void)
{   Reserved161_DriverIRQHandler();
}

WEAK void Reserved162_IRQHandler(void)
{   Reserved162_DriverIRQHandler();
}

WEAK void Reserved163_IRQHandler(void)
{   Reserved163_DriverIRQHandler();
}

WEAK void Reserved164_IRQHandler(void)
{   Reserved164_DriverIRQHandler();
}

WEAK void Reserved165_IRQHandler(void)
{   Reserved165_DriverIRQHandler();
}

WEAK void Reserved166_IRQHandler(void)
{   Reserved166_DriverIRQHandler();
}

WEAK void Reserved167_IRQHandler(void)
{   Reserved167_DriverIRQHandler();
}

WEAK void LPI2C5_IRQHandler(void)
{   LPI2C5_DriverIRQHandler();
}

WEAK void LPI2C6_IRQHandler(void)
{   LPI2C6_DriverIRQHandler();
}

WEAK void SAI4_IRQHandler(void)
{   SAI4_DriverIRQHandler();
}

WEAK void SPDIF_IRQHandler(void)
{   SPDIF_DriverIRQHandler();
}

WEAK void LPUART9_IRQHandler(void)
{   LPUART9_DriverIRQHandler();
}

WEAK void LPUART10_IRQHandler(void)
{   LPUART10_DriverIRQHandler();
}

WEAK void LPUART11_IRQHandler(void)
{   LPUART11_DriverIRQHandler();
}

WEAK void LPUART12_IRQHandler(void)
{   LPUART12_DriverIRQHandler();
}

WEAK void Reserved176_IRQHandler(void)
{   Reserved176_DriverIRQHandler();
}

WEAK void Reserved177_IRQHandler(void)
{   Reserved177_DriverIRQHandler();
}

WEAK void Reserved178_IRQHandler(void)
{   Reserved178_DriverIRQHandler();
}

WEAK void Reserved179_IRQHandler(void)
{   Reserved179_DriverIRQHandler();
}

WEAK void TMR3_IRQHandler(void)
{   TMR3_DriverIRQHandler();
}

WEAK void TMR4_IRQHandler(void)
{   TMR4_DriverIRQHandler();
}

WEAK void M33_SYSRESET_REQ_IRQHandler(void)
{   M33_SYSRESET_REQ_DriverIRQHandler();
}

WEAK void M33_LOCKUP_IRQHandler(void)
{   M33_LOCKUP_DriverIRQHandler();
}

WEAK void M7_SYSRESET_REQ_IRQHandler(void)
{   M7_SYSRESET_REQ_DriverIRQHandler();
}

WEAK void M7_LOCKUP_IRQHandler(void)
{   M7_LOCKUP_DriverIRQHandler();
}

WEAK void PWM2_FAULT_IRQHandler(void)
{   PWM2_FAULT_DriverIRQHandler();
}

WEAK void PWM2_0_IRQHandler(void)
{   PWM2_0_DriverIRQHandler();
}

WEAK void PWM2_1_IRQHandler(void)
{   PWM2_1_DriverIRQHandler();
}

WEAK void PWM2_2_IRQHandler(void)
{   PWM2_2_DriverIRQHandler();
}

WEAK void PWM2_3_IRQHandler(void)
{   PWM2_3_DriverIRQHandler();
}

WEAK void PWM3_FAULT_IRQHandler(void)
{   PWM3_FAULT_DriverIRQHandler();
}

WEAK void PWM3_0_IRQHandler(void)
{   PWM3_0_DriverIRQHandler();
}

WEAK void PWM3_1_IRQHandler(void)
{   PWM3_1_DriverIRQHandler();
}

WEAK void PWM3_2_IRQHandler(void)
{   PWM3_2_DriverIRQHandler();
}

WEAK void PWM3_3_IRQHandler(void)
{   PWM3_3_DriverIRQHandler();
}

WEAK void PWM4_FAULT_IRQHandler(void)
{   PWM4_FAULT_DriverIRQHandler();
}

WEAK void PWM4_0_IRQHandler(void)
{   PWM4_0_DriverIRQHandler();
}

WEAK void PWM4_1_IRQHandler(void)
{   PWM4_1_DriverIRQHandler();
}

WEAK void PWM4_2_IRQHandler(void)
{   PWM4_2_DriverIRQHandler();
}

WEAK void PWM4_3_IRQHandler(void)
{   PWM4_3_DriverIRQHandler();
}

WEAK void EQDC1_IRQHandler(void)
{   EQDC1_DriverIRQHandler();
}

WEAK void EQDC2_IRQHandler(void)
{   EQDC2_DriverIRQHandler();
}

WEAK void EQDC3_IRQHandler(void)
{   EQDC3_DriverIRQHandler();
}

WEAK void EQDC4_IRQHandler(void)
{   EQDC4_DriverIRQHandler();
}

WEAK void ADC2_IRQHandler(void)
{   ADC2_DriverIRQHandler();
}

WEAK void DCDC_IRQHandler(void)
{   DCDC_DriverIRQHandler();
}

WEAK void CAN3_IRQHandler(void)
{   CAN3_DriverIRQHandler();
}

WEAK void CAN3_ERROR_IRQHandler(void)
{   CAN3_ERROR_DriverIRQHandler();
}

WEAK void DAC_IRQHandler(void)
{   DAC_DriverIRQHandler();
}

WEAK void LPSPI5_IRQHandler(void)
{   LPSPI5_DriverIRQHandler();
}

WEAK void LPSPI6_IRQHandler(void)
{   LPSPI6_DriverIRQHandler();
}

WEAK void LPUART7_IRQHandler(void)
{   LPUART7_DriverIRQHandler();
}

WEAK void LPUART8_IRQHandler(void)
{   LPUART8_DriverIRQHandler();
}

WEAK void SAI2_IRQHandler(void)
{   SAI2_DriverIRQHandler();
}

WEAK void SAI3_IRQHandler(void)
{   SAI3_DriverIRQHandler();
}

WEAK void ACMP1_IRQHandler(void)
{   ACMP1_DriverIRQHandler();
}

WEAK void ACMP2_IRQHandler(void)
{   ACMP2_DriverIRQHandler();
}

WEAK void ACMP3_IRQHandler(void)
{   ACMP3_DriverIRQHandler();
}

WEAK void ACMP4_IRQHandler(void)
{   ACMP4_DriverIRQHandler();
}

WEAK void Reserved220_IRQHandler(void)
{   Reserved220_DriverIRQHandler();
}

WEAK void Reserved221_IRQHandler(void)
{   Reserved221_DriverIRQHandler();
}

WEAK void Reserved222_IRQHandler(void)
{   Reserved222_DriverIRQHandler();
}

WEAK void Reserved223_IRQHandler(void)
{   Reserved223_DriverIRQHandler();
}

WEAK void Reserved224_IRQHandler(void)
{   Reserved224_DriverIRQHandler();
}

WEAK void GPT1_IRQHandler(void)
{   GPT1_DriverIRQHandler();
}

WEAK void GPT2_IRQHandler(void)
{   GPT2_DriverIRQHandler();
}

WEAK void KPP_IRQHandler(void)
{   KPP_DriverIRQHandler();
}

WEAK void USBPHY1_IRQHandler(void)
{   USBPHY1_DriverIRQHandler();
}

WEAK void USBPHY2_IRQHandler(void)
{   USBPHY2_DriverIRQHandler();
}

WEAK void USB_OTG2_IRQHandler(void)
{   USB_OTG2_DriverIRQHandler();
}

WEAK void USB_OTG1_IRQHandler(void)
{   USB_OTG1_DriverIRQHandler();
}

WEAK void XSPI_SLV_IRQHandler(void)
{   XSPI_SLV_DriverIRQHandler();
}

WEAK void NETC_IRQHandler(void)
{   NETC_DriverIRQHandler();
}

WEAK void MSGINTR1_IRQHandler(void)
{   MSGINTR1_DriverIRQHandler();
}

WEAK void MSGINTR2_IRQHandler(void)
{   MSGINTR2_DriverIRQHandler();
}

WEAK void MSGINTR3_IRQHandler(void)
{   MSGINTR3_DriverIRQHandler();
}

WEAK void MSGINTR4_IRQHandler(void)
{   MSGINTR4_DriverIRQHandler();
}

WEAK void MSGINTR5_IRQHandler(void)
{   MSGINTR5_DriverIRQHandler();
}

WEAK void MSGINTR6_IRQHandler(void)
{   MSGINTR6_DriverIRQHandler();
}

WEAK void SINC1_0_IRQHandler(void)
{   SINC1_0_DriverIRQHandler();
}

WEAK void SINC1_1_IRQHandler(void)
{   SINC1_1_DriverIRQHandler();
}

WEAK void SINC1_2_IRQHandler(void)
{   SINC1_2_DriverIRQHandler();
}

WEAK void SINC1_3_IRQHandler(void)
{   SINC1_3_DriverIRQHandler();
}

WEAK void SINC2_0_IRQHandler(void)
{   SINC2_0_DriverIRQHandler();
}

WEAK void SINC2_1_IRQHandler(void)
{   SINC2_1_DriverIRQHandler();
}

WEAK void SINC2_2_IRQHandler(void)
{   SINC2_2_DriverIRQHandler();
}

WEAK void SINC2_3_IRQHandler(void)
{   SINC2_3_DriverIRQHandler();
}

WEAK void GPIO4_0_IRQHandler(void)
{   GPIO4_0_DriverIRQHandler();
}

WEAK void GPIO4_1_IRQHandler(void)
{   GPIO4_1_DriverIRQHandler();
}

WEAK void GPIO5_0_IRQHandler(void)
{   GPIO5_0_DriverIRQHandler();
}

WEAK void GPIO5_1_IRQHandler(void)
{   GPIO5_1_DriverIRQHandler();
}

WEAK void GPIO6_0_IRQHandler(void)
{   GPIO6_0_DriverIRQHandler();
}

WEAK void GPIO6_1_IRQHandler(void)
{   GPIO6_1_DriverIRQHandler();
}

WEAK void ECAT_IRQHandler(void)
{   ECAT_DriverIRQHandler();
}

//*****************************************************************************

#if defined (DEBUG)
#pragma GCC pop_options
#endif // (DEBUG)
