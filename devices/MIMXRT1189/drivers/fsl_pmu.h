/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_PMU_H_
#define _FSL_PMU_H_

#include "fsl_common.h"

/*!
 * @addtogroup pmu
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version
 * @{
 */

/*! @brief PMU driver version */
#define FSL_PMU_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Version 2.0.0. */

/*!
 * @}
 */

#if defined(ANADIG_PMU_PMU_BIAS_CTRL2_FBB_M7_CONTROL_MODE_MASK)
#define PMU_HAS_FBB (1U)
#else
#define PMU_HAS_FBB (0U)
#endif /* ANADIG_PMU_PMU_BIAS_CTRL2_FBB_M7_CONTROL_MODE_MASK */

/*!
 * @brief System setpoints enumeration.
 */
enum _pmu_setpoint_map
{
    kPMU_SetPoint0  = 1UL << 0UL,  /*!< Set point 0. */
    kPMU_SetPoint1  = 1UL << 1UL,  /*!< Set point 1. */
    kPMU_SetPoint2  = 1UL << 2UL,  /*!< Set point 2. */
    kPMU_SetPoint3  = 1UL << 3UL,  /*!< Set point 3. */
    kPMU_SetPoint4  = 1UL << 4UL,  /*!< Set point 4. */
    kPMU_SetPoint5  = 1UL << 5UL,  /*!< Set point 5. */
    kPMU_SetPoint6  = 1UL << 6UL,  /*!< Set point 6. */
    kPMU_SetPoint7  = 1UL << 7UL,  /*!< Set point 7. */
    kPMU_SetPoint8  = 1UL << 8UL,  /*!< Set point 8. */
    kPMU_SetPoint9  = 1UL << 9UL,  /*!< Set point 9. */
    kPMU_SetPoint10 = 1UL << 10UL, /*!< Set point 10. */
    kPMU_SetPoint11 = 1UL << 11UL, /*!< Set point 11. */
    kPMU_SetPoint12 = 1UL << 12UL, /*!< Set point 12. */
    kPMU_SetPoint13 = 1UL << 13UL, /*!< Set point 13. */
    kPMU_SetPoint14 = 1UL << 14UL, /*!< Set point 14. */
    kPMU_SetPoint15 = 1UL << 15UL  /*!< Set point 15. */
};

/*!
 * @brief The name of LDOs
 */
typedef enum _pmu_ldo_name
{
    kPMU_PllLdo     = 0U, /*!< The PLL LDO in SOC domain. */
    kPMU_AonAnaLdo  = 1U, /*!< The AON ANA LDO in AON domain. */
    kPMU_AonDigLdo  = 2U, /*!< The AON DIG LDO in AON domain. */
    kPMU_BbsmDigLdo = 3U  /*!< The BBSM DIG LDO in BBSM domain. */
} pmu_ldo_name_t;

/*!
 * @brief The name of body bias.
 */
typedef enum _pmu_body_bias_name
{
#if (defined(PMU_HAS_FBB) && PMU_HAS_FBB)
    kPMU_FBB_CM7 = 0x0U, /*!< The FBB implemented in Cortex-M7 Platform. */
    kPMU_RBB_SOC = 0x1U, /*!< The RBB implemented in SOC. */
    kPMU_RBB_AON = 0x2U, /*!< The RBB implemented in AONMIX. */
#else
    kPMU_RBB_SOC = 0x0U, /*!< The RBB implemented in SOC. */
    kPMU_RBB_AON = 0x1U, /*!< The RBB implemented in AONMIX. */
#endif /* PMU_HAS_FBB */
} pmu_body_bias_name_t;

/*!
 * @brief The control mode of LDOs/Bandgaps/Body Bias.
 */
typedef enum _pmu_control_mode
{
    kPMU_StaticMode = 0U, /*!< Static/Software Control mode. */
    kPMU_GPCMode    = 1U, /*!< GPC/Hardware Control mode. */
} pmu_control_mode_t;

/*!
 * @brief The operation mode for the LDOs.
 */
typedef enum _pmu_ldo_operate_mode
{
    kPMU_LowPowerMode  = 0x0U, /*!< LDOs operate in Low power mode. */
    kPMU_HighPowerMode = 0x1U, /*!< LDOs operate in High power mode. */
} pmu_ldo_operate_mode_t;

/*!
 * @brief The enumeration of AON ANA LDO's charge pump current.
 */
typedef enum _pmu_aon_ana_ldo_charge_pump_current
{
    kPMU_AonAnaChargePump300nA = 0U, /*!< The current of the charge pump is selected as 300nA. */
    kPMU_AonAnaChargePump400nA = 1U, /*!< The current of the charge pump is selected as 400nA. */
    kPMU_AonAnaChargePump500nA = 2U, /*!< The current of the charge pump is selected as 500nA. */
    kPMU_AonAnaChargePump600nA = 3U, /*!< The current of the charge pump is selected as 600nA. */
} pmu_aon_ana_ldo_charge_pump_current_t;

/*!
 * @brief The enumeration of AON ANA LDO's output range.
 */
typedef enum _pmu_aon_ana_ldo_output_range
{
    kPMU_AonAnaLdoOutputFrom1P77To1P83 = 0U, /*!< The output voltage varies from 1.77V to 1.83V. */
    kPMU_AonAnaLdoOutputFrom1P72To1P77 = 1U, /*!< The output voltage varies from 1.72V to 1.77V. */
    kPMU_AonAnaLdoOutputFrom1P82To1P88 = 2U, /*!< The output voltage varies from 1.82V to 1.88V. */
} pmu_aon_ana_ldo_output_range_t;

/*!
 * @brief The enumeration of voltage step time for AON DIG LDO.
 */
typedef enum _pmu_aon_dig_voltage_step_time
{
    kPMU_AonDigVoltageStepInc15us  = 0x0U, /*!< AON DIG LDO voltage step time selected as 15us. */
    kPMU_AonDigVoltageStepInc25us  = 0x1U, /*!< AON DIG LDO voltage step time selected as 25us. */
    kPMU_AonDigVoltageStepInc50us  = 0x2U, /*!< AON DIG LDO voltage step time selected as 50us. */
    kPMU_AonDigVoltageStepInc100us = 0x3U, /*!< AON DIG LDO voltage step time selected as 100us. */
} pmu_aon_dig_voltage_step_time_t;

/*!
 * @brief The target output voltage of AON DIG LDO.
 */
typedef enum _pmu_aon_dig_target_output_voltage
{
    kPMU_AonDigTargetStableVoltage0P631V = 0x0U,  /*!< The target voltage selected as 0.631V */
    kPMU_AonDigTargetStableVoltage0P65V  = 0x1U,  /*!< The target voltage selected as 0.65V */
    kPMU_AonDigTargetStableVoltage0P67V  = 0x2U,  /*!< The target voltage selected as 0.67V */
    kPMU_AonDigTargetStableVoltage0P689V = 0x3U,  /*!< The target voltage selected as 0.689V */
    kPMU_AonDigTargetStableVoltage0P709V = 0x4U,  /*!< The target voltage selected as 0.709V */
    kPMU_AonDigTargetStableVoltage0P728V = 0x5U,  /*!< The target voltage selected as 0.728V */
    kPMU_AonDigTargetStableVoltage0P748V = 0x6U,  /*!< The target voltage selected as 0.748V */
    kPMU_AonDigTargetStableVoltage0P767V = 0x7U,  /*!< The target voltage selected as 0.767V */
    kPMU_AonDigTargetStableVoltage0P786V = 0x8U,  /*!< The target voltage selected as 0.786V */
    kPMU_AonDigTargetStableVoltage0P806V = 0x9U,  /*!< The target voltage selected as 0.806V */
    kPMU_AonDigTargetStableVoltage0P825V = 0xAU,  /*!< The target voltage selected as 0.825V */
    kPMU_AonDigTargetStableVoltage0P845V = 0xBU,  /*!< The target voltage selected as 0.845V */
    kPMU_AonDigTargetStableVoltage0P864V = 0xCU,  /*!< The target voltage selected as 0.864V */
    kPMU_AonDigTargetStableVoltage0P883V = 0xDU,  /*!< The target voltage selected as 0.883V */
    kPMU_AonDigTargetStableVoltage0P903V = 0xEU,  /*!< The target voltage selected as 0.903V */
    kPMU_AonDigTargetStableVoltage0P922V = 0xFU,  /*!< The target voltage selected as 0.922V */
    kPMU_AonDigTargetStableVoltage0P942V = 0x10U, /*!< The target voltage selected as 0.942V */
    kPMU_AonDigTargetStableVoltage0P961V = 0x11U, /*!< The target voltage selected as 0.961V */
    kPMU_AonDigTargetStableVoltage0P981V = 0x12U, /*!< The target voltage selected as 0.981V */
    kPMU_AonDigTargetStableVoltage1P0V   = 0x13U, /*!< The target voltage selected as 1.0V */
    kPMU_AonDigTargetStableVoltage1P019V = 0x14U, /*!< The target voltage selected as 1.019V */
    kPMU_AonDigTargetStableVoltage1P039V = 0x15U, /*!< The target voltage selected as 1.039V */
    kPMU_AonDigTargetStableVoltage1P058V = 0x16U, /*!< The target voltage selected as 1.058V */
    kPMU_AonDigTargetStableVoltage1P078V = 0x17U, /*!< The target voltage selected as 1.078V */
    kPMU_AonDigTargetStableVoltage1P097V = 0x18U, /*!< The target voltage selected as 1.097V */
    kPMU_AonDigTargetStableVoltage1P117V = 0x19U, /*!< The target voltage selected as 1.117V */
    kPMU_AonDigTargetStableVoltage1P136V = 0x1AU, /*!< The target voltage selected as 1.136V */
    kPMU_AonDigTargetStableVoltage1P155V = 0x1BU, /*!< The target voltage selected as 1.155V */
    kPMU_AonDigTargetStableVoltage1P175V = 0x1CU, /*!< The target voltage selected as 1.175V */
    kPMU_AonDigTargetStableVoltage1P194V = 0x1DU, /*!< The target voltage selected as 1.194V */
    kPMU_AonDigTargetStableVoltage1P214V = 0x1EU, /*!< The target voltage selected as 1.214V */
    kPMU_AonDigTargetStableVoltage1P233V = 0x1FU, /*!< The target voltage selected as 1.233V */
} pmu_aon_dig_target_output_voltage_t;

/*!
 * @brief The enumeration of the BBSM DIG LDO's charge pump current.
 */
typedef enum _pmu_bbsm_dig_charge_pump_current
{
    kPMU_BbsmDigChargePump12P5nA  = 0U, /*!< The current of BBSM DIG LDO's charge pump is selected as 12.5nA. */
    kPMU_BbsmDigChargePump6P25nA  = 1U, /*!< The current of BBSM DIG LDO's charge pump is selected as 6.25nA. */
    kPMU_BbsmDigChargePump18P75nA = 2U, /*!< The current of BBSM DIG LDO's charge pump is selected as 18.75nA. */
} pmu_bbsm_dig_charge_pump_current_t;

/*!
 * @brief The enumeration of the BBSM DIG LDO's discharge resistor.
 */
typedef enum _pmu_bbsm_dig_discharge_resistor_value
{
    kPMU_BbsmDigDischargeResistor15K = 0U, /*!< The Discharge Resistor is selected as 15K ohm */
    kPMU_BbsmDigDischargeResistor30K = 1U, /*!< The Discharge Resistor is selected as 30K ohm */
    kPMU_BbsmDigDischargeResistor9K  = 2U, /*!< The Discharge Resistor is selected as 9K ohm */
} pmu_bbsm_dig_discharge_resistor_value_t;

/*!
 * @brief The enumeration of bandgap power down option.
 */
enum _pmu_static_bandgap_power_down_option
{
    kPMU_PowerDownBandgapFully               = 1U << 0U, /*!< Fully power down the bandgap module. */
    kPMU_PowerDownVoltageReferenceOutputOnly = 1U << 1U, /*!< Power down only the reference output
                                                                section of the bandgap */
    kPMU_PowerDownBandgapVBGUPDetector = 1U << 2U,       /*!< Power down the VBGUP detector of the bandgap without
                                                              affecting any additional functionality. */
};

/*!
 * @brief The enumerator of well bias power source.
 */
typedef enum _pmu_well_bias_power_source
{
    kPMU_WellBiasPowerFromAonDigLdo = 0U, /*!< AON Dig LDO supplies the power stage and NWELL sampler. */
    kPMU_WellBiasPowerFromDCDC,           /*!< DCDC supplies the power stage and NWELL sampler. */
} pmu_well_bias_power_source_t;

/*!
 * @brief The enumerator of bias area size.
 */
typedef enum _pmu_bias_area_size
{
    kPMU_180uA_6mm2At125C = 0U, /*!< Imax = 180uA; Areamax-RVT = 6.00mm2 at 125C */
    kPMU_150uA_5mm2At125C,      /*!< Imax = 150uA; Areamax-RVT = 5.00mm2 at 125C */
    kPMU_120uA_4mm2At125C,      /*!< Imax = 120uA; Areamax-RVT = 4.00mm2 at 125C */
    kPMU_90uA_3mm2At125C,       /*!< Imax = 90uA; Areamax-RVT = 3.00mm2 at 125C */
    kPMU_60uA_2mm2At125C,       /*!< Imax = 60uA; Areamax-RVT = 2.00mm2 at 125C */
    kPMU_45uA_1P5mm2At125C,     /*!< Imax = 45uA; Areamax-RVT = 1P5mm2 at 125C */
    kPMU_30uA_1mm2At125C,       /*!< Imax = 30uA; Areamax-RVT = 1.00mm2 at 125C */
    kPMU_15uA_0P5mm2At125C,     /*!< Imax = 15uA; Areamax-RVT = 0.50mm2 at 125C */
} pmu_bias_area_size_t;

/*!
 * @brief The enumerator of well bias typical frequency.
 */
typedef enum _pmu_well_bias_typical_freq
{
    kPMU_OscFreqDiv128 = 0U, /*!< Typical frequency = osc_freq / 128. */
    kPMU_OscFreqDiv64  = 1U, /*!< Typical frequency = osc_freq / 64. */
    kPMU_OscFreqDiv32  = 2U, /*!< Typical frequency = osc_freq / 32. */
    kPMU_OscFreqDiv16  = 3U, /*!< Typical frequency = osc_freq / 16. */
    kPMU_OscFreqDiv8   = 4U, /*!< Typical frequency = osc_freq / 8. */
    kPMU_OscFreqDiv2   = 6U, /*!< Typical frequency = osc_freq / 2. */
    kPMU_OscFreq       = 7U, /*!< Typical frequency = oscillator frequency. */
} pmu_well_bias_typical_freq_t;

/*!
 * @brief The enumerator of well bias adaptive clock source.
 */
typedef enum _pmu_adaptive_clock_source
{
    kPMU_AdaptiveClkSourceOscClk = 0U,   /*!< The adaptive clock source is oscillator clock. */
    kPMU_AdaptiveClkSourceChargePumpClk, /*!< The adaptive clock source is charge pump clock. */
} pmu_adaptive_clock_source_t;

/*!
 * @brief The enumerator of frequency reduction due to cap increment.
 */
typedef enum _pmu_freq_reduction
{
    kPMU_FreqReductionNone = 0U, /*!< No frequency reduction. */
    kPMU_FreqReduction30PCT,     /*!< 30% frequency reduction due to cap increment. */
    kPMU_FreqReduction40PCT,     /*!< 40% frequency reduction due to cap increment. */
    kPMU_FreqReduction50PCT,     /*!< 50% frequency reduction due to cap increment. */
} pmu_freq_reduction_t;

/*!
 * @brief The enumerator of well bias 1P8 adjustment.
 */
typedef enum _pmu_well_bias_1P8_adjustment
{
    kPMU_Cref0fFCspl0fFDeltaC0fF = 0U, /*!< Cref = 0fF, Cspl = 0fF, DeltaC = 0fF. */
    kPMU_Cref0fFCspl30fFDeltaCN30fF,   /*!< Cref = 0fF, Cspl = 30fF, DeltaC = -30fF. */
    kPMU_Cref0fFCspl43fFDeltaCN43fF,   /*!< Cref = 0fF, Cspl = 43fF, DeltaC = -43fF. */
    kPMU_Cref0fFCspl62fFDeltaCN62fF,   /*!< Cref = 0fF, Cspl = 62fF, DeltaC = -62fF. */
    kPMU_Cref0fFCspl105fFDeltaCN105fF, /*!< Cref = 0fF, Cspl = 105fF, DeltaC = -105fF. */
    kPMU_Cref30fFCspl0fFDeltaC30fF,    /*!< Cref = 30fF, Cspl = 0fF, DeltaC = 30fF. */
    kPMU_Cref30fFCspl43fFDeltaCN12fF,  /*!< Cref = 30fF, Cspl = 43fF, DeltaC = -12fF. */
    kPMU_Cref30fFCspl105fFDeltaCN75fF, /*!< Cref = 30fF, Cspl = 105fF, DeltaC = -75fF. */
    kPMU_Cref43fFCspl0fFDeltaC43fF,    /*!< Cref = 43fF, Cspl = 0fF, DeltaC = 43fF. */
    kPMU_Cref43fFCspl30fFDeltaC13fF,   /*!< Cref = 43fF, Cspl = 30fF, DeltaC = 13fF. */
    kPMU_Cref43fFCspl62fFDeltaCN19fF,  /*!< Cref = 43fF, Cspl = 62fF, DeltaC = -19fF. */
    kPMU_Cref62fFCspl0fFDeltaC62fF,    /*!< Cref = 62fF, Cspl = 0fF, DeltaC = 62fF. */
    kPMU_Cref62fFCspl43fFDeltaC19fF,   /*!< Cref = 62fF, Cspl = 43fF, DeltaC = 19fF. */
    kPMU_Cref105fFCspl0fFDeltaC105fF,  /*!< Cref = 105fF, Cspl = 0fF, DeltaC = 105fF. */
    kPMU_Cref105fFCspl30fFDeltaC75fF,  /*!< Cref = 105fF, Cspl = 30fF, DeltaC = 75fF. */
} pmu_well_bias_1P8_adjustment_t;

/*!
 * @brief AON ANA LDO config.
 */
typedef struct _pmu_static_aon_ana_ldo_config
{
    pmu_ldo_operate_mode_t mode; /*!< The operate mode of AON ANA LDO. */
    bool enable2mALoad;          /*!< Enable/Disable 2mA load.
                                      - \b true Enables 2mA loading to prevent overshoot;
                                      - \b false Disables 2mA loading.*/
    bool enable4mALoad;          /*!< Enable/Disable 4mA load.
                                      - \b true Enables 4mA loading to prevent dramatic voltage drop;
                                      - \b false Disables 4mA load. */
    bool enable20uALoad;         /*!< Enable/Disable 20uA load.
                                      - \b true Enables 20uA loading to prevent overshoot;
                                      - \b false Disables 20uA load. */
    bool enableStandbyMode;      /*!< Enable/Disable Standby Mode.
                                      - \b true Enables Standby mode, if the STBY assert, the AON ANA LDO enter LP mode
                                      - \b false Disables Standby mode. */
} pmu_static_aon_ana_ldo_config_t;

/*!
 * @brief AON DIG LDO Config in Static/Software Mode.
 */
typedef struct _pmu_static_aon_dig_config
{
    bool enableStableDetect;                           /*!< Enable/Disable Stable Detect.
                                                               - \b true   Enables Stable Detect.
                                                               - \b false  Disables Stable Detect. */
    pmu_aon_dig_voltage_step_time_t voltageStepTime;   /*!< Step time. */
    pmu_aon_dig_target_output_voltage_t targetVoltage; /*!< The target output voltage. */
} pmu_static_aon_dig_config_t;

/*!
 * @brief BBSM DIG LDO config.
 */
typedef struct _pmu_bbsm_dig_config
{
    pmu_ldo_operate_mode_t mode;                          /*!< The operate mode the BBSM DIG LDO. */
    pmu_bbsm_dig_charge_pump_current_t chargePumpCurrent; /*!< The current of BBSM DIG LDO's charge pump current. */
    pmu_bbsm_dig_discharge_resistor_value_t dischargeResistorValue; /*!< The value of BBSM DIG LDO's
                                                                        Discharge Resistor. */
    uint8_t trimValue;                                              /*!< The trim value. */
    bool enablePullDown;                                            /*!< Enable/Disable Pull down.
                                                                      - \b true Enables the feature of using 1M ohm resistor to discharge the LDO output.
                                                                      - \b false Disables the feature of using 1M ohm resistor to discharge the LDO output. */
    bool enableLdoStable;                                           /*!< Enable/Disable BBSM DIG LDO Stable. */
} pmu_bbsm_dig_config_t;

/*!
 * @brief Bandgap config in static mode.
 */
typedef struct _pmu_static_bandgap_config
{
    uint8_t powerDownOption; /*!< The OR'ed value of @ref _pmu_static_bandgap_power_down_option. Please refer to @ref
                                _pmu_static_bandgap_power_down_option.  */
    bool enableLowPowerMode; /*!< Turn on/off the Low power mode.
                                  - \b true Turns on the low power operation of the bandgap.
                                  - \b false Turns off the low power operation of the bandgap. */
} pmu_static_bandgap_config_t;

/*!
 * @brief The union of well bias basic options, such as clock source, power source and so on.
 */
typedef union _pmu_well_bias_option
{
    uint16_t wellBiasData; /*!< well bias configuration data. */
    struct
    {
        uint16_t enablePWellOnly : 1U;     /*!< Turn on both PWELL and NWELL, or only trun on PWELL.
                                                - \b 1b0  PWELL and NEWLL are both turned on.
                                                - \b 1b1  PWELL is turned on only. */
        uint16_t reserved1 : 1U;           /*!< Reserved. */
        uint16_t biasAreaSize : 3U;        /*!< Select size of bias area, please refer to @ref pmu_bias_area_size_t */
        uint16_t disableAdaptiveFreq : 1U; /*!< Enable/Disable adaptive frequency.
                                                - \b 1b0 Frequency change after each half cycle minimum frequency
                                                        determined by typical frequency.
                                                - \b 1b1 Adaptive frequency disabled. Frequency determined by typical
                                                       frequency. */
        uint16_t wellBiasFreq : 3U;        /*!< Set well bias typical frequency, please refer to @ref
                                              pmu_well_bias_typical_freq_t. */
        uint16_t clkSource : 1U;     /*!< Config the adaptive clock source, please @ref pmu_adaptive_clock_source_t. */
        uint16_t freqReduction : 2U; /*!< Config the percent of frequency reduction due to cap increment,
                                         please refer to @ref pmu_freq_reduction_t.  */
        uint16_t enablePowerDownOption : 1U; /*!< Enable/Disable pull down option.
                                                 - \b false Pull down option is disabled.
                                                 - \b true  Pull down option is enabled. */
        uint16_t reserved2 : 1U;             /*!< Reserved. */
        uint16_t powerSource : 1U; /*!< Set power source, please refer to @ref pmu_well_bias_power_source_t. */
        uint16_t reserved3 : 1U;   /*!< Reserved. */
    } wellBiasStruct;
} pmu_well_bias_option_t;

/*!
 * @brief The structure of well bias configuration.
 */
typedef struct _pmu_well_bias_config
{
    pmu_well_bias_option_t wellBiasOption;     /*!< Well bias basic function, please
                                                    refer to @ref pmu_well_bias_option_t. */
    pmu_well_bias_1P8_adjustment_t adjustment; /*!< Well bias adjustment 1P8, please
                                                    refer to @ref pmu_well_bias_1P8_adjustment_t. */
} pmu_well_bias_config_t;

/*!
 * @brief The stucture of body bias config in GPC mode.
 */
typedef struct _pmu_gpc_body_bias_config
{
    uint8_t PWELLRegulatorSize; /*!< The size of the PWELL Regulator. */
    uint8_t NWELLRegulatorSize; /*!< The size of the NWELL Regulator. */
    uint8_t oscillatorSize;     /*!< The size of the oscillator bits. */
    uint8_t regulatorStrength;  /*!< The strength of the selected regulator. */
} pmu_gpc_body_bias_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name LDOs Control APIs
 * @{
 */

/*!
 * @brief Selects the control mode of the PLL LDO.
 *
 * @param base PMU peripheral base address.
 * @param mode The control mode of the PLL LDO. Please refer to @ref pmu_control_mode_t.
 */
void PMU_SetPllLdoControlMode(ANADIG_PMU_Type *base, pmu_control_mode_t mode);

/*!
 * @brief Switches the PLL LDO from Static/Software Mode to GPC/Hardware Mode.
 *
 * @param base PMU peripheral base address.
 */
void PMU_SwitchPllLdoToGPCMode(ANADIG_PMU_Type *base);

/*!
 * @brief Enables PLL LDO via AI interface in Static/Software mode.
 *
 * @param base PMU peripheral base address.
 */
void PMU_StaticEnablePllLdo(ANADIG_PMU_Type *base);

/*!
 * @brief Disables PLL LDO via AI interface in Static/Software mode.
 */
void PMU_StaticDisablePllLdo(void);

/*!
 * @brief Selects the control mode of the AON ANA LDO.
 *
 * @param base PMU peripheral base address.
 * @param mode The control mode of the AON ANA LDO. Please refer to @ref pmu_control_mode_t.
 */
void PMU_SetAonAnaLdoControlMode(ANADIG_LDO_BBSM_Type *base, pmu_control_mode_t mode);

/*!
 * @brief Sets the Bypass mode of the AON ANA LDO.
 *
 * @param base ANADIG_LDO_BBSM peripheral base address.
 * @param enable Enable/Disable bypass mode.
 *          - \b true Enable AON ANA Bypass mode.
 *          - \b false Disable AON ANA Bypass mode.
 */
void PMU_StaticEnableAonAnaLdoBypassMode(ANADIG_LDO_BBSM_Type *base, bool enable);

/*!
 * @brief Checks whether the AON ANA LDO is in bypass mode.
 *
 * @param base ANADIG_LDO_BBSM peripheral base address.
 * @return The result used to indicates whether the AON ANA LDO is in bypass mode.
 *              - \b true   The AON ANA LDO is in bypass mode.
 *              - \b false  The AON ANA LDO not in bypass mode.
 */
static inline bool PMU_StaticCheckAonAnaLdoBypassMode(ANADIG_LDO_BBSM_Type *base)
{
    return ((ANADIG_LDO_BBSM->PMU_LDO_AON_ANA & ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_BYPASS_MODE_EN_MASK) != 0UL);
}

/*!
 * @brief Fill the AON ANA LDO configuration structure with default settings.
 *
 * The default values are:
 * @code
 *      config->mode                   = kPMU_HighPowerMode;
        config->enable2mALoad          = true;
        config->enable20uALoad         = false;
        config->enable4mALoad          = true;
        config->enableStandbyMode      = false;
        config->driverStrength         = kPMU_AonAnaLdoDriverStrength0;
        config->brownOutDetectorConfig = kPMU_AonAnaLdoBrownOutDetectorDisable;
        config->chargePumpCurrent      = kPMU_AonAnaChargePump300nA;
        config->outputRange            = kPMU_AonAnaLdoOutputFrom1P77To1P83;
 * @endcode
 *
 * @param config Pointer to the structure @ref pmu_static_aon_ana_ldo_config_t.
 */
void PMU_StaticGetAonAnaLdoDefaultConfig(pmu_static_aon_ana_ldo_config_t *config);

/*!
 * @brief Initialize the AON ANA LDO in Static/Sofware Mode.
 *
 * @param base ANADIG_LDO_BBSM peripheral base address.
 * @param config Pointer to the structure @ref pmu_static_aon_ana_ldo_config_t.
 */
void PMU_StaticAonAnaLdoInit(ANADIG_LDO_BBSM_Type *base, const pmu_static_aon_ana_ldo_config_t *config);

/*!
 * @brief Disable the output of AON ANA LDO.
 *
 * @param base ANADIG_LDO_BBSM peripheral base address.
 */
void PMU_StaticAonAnaLdoDeinit(ANADIG_LDO_BBSM_Type *base);

/*!
 * @brief Selects the control mode of the AON DIG LDO.
 *
 * @param base PMU peripheral base address.
 * @param mode The control mode of the AON DIG LDO. Please refer to @ref pmu_control_mode_t.
 */
void PMU_SetAonDigLdoControlMode(ANADIG_LDO_BBSM_Type *base, pmu_control_mode_t mode);

/*!
 * @brief Turn on/off Bypass mode of the AON DIG LDO in Static/Software mode.
 *
 * @param base ANADIG_LDO_BBSM peripheral base address.
 * @param enable
 *               - \b true Turns on Bypass mode of the AON DIG LDO.
 *               - \b false Turns off Bypass mode of the AON DIG LDO.
 */
void PMU_StaticEnableAonDigLdoBypassMode(ANADIG_LDO_BBSM_Type *base, bool enable);

/*!
 * @brief Checks whether the AON DIG LDO is in bypass mode.
 *
 * @param base PMU peripheral base address.
 * @return The result used to indicates whether the AON DIG LDO is in bypass mode.
 *              - \b true   The AON DIG LDO is in bypass mode.
 *              - \b false  The AON DIG LDO not in bypass mode.
 */
static inline bool PMU_StaticCheckAonDigLdoBypassMode(ANADIG_LDO_BBSM_Type *base)
{
    return ((ANADIG_LDO_BBSM->PMU_LDO_AON_DIG & ANADIG_LDO_BBSM_PMU_LDO_AON_DIG_BYPASS_MODE_MASK) != 0UL);
}

/*!
 * @brief Gets the default configuration of AON DIG LDO.
 *
 * The default values are:
 *  @code
 *          config->enableStableDetect = false;
 *          config->voltageStepTime    = kPMU_AonDigVoltageStepInc50us;
 *          config->brownOutConfig     = kPMU_AonDigBrownOutDisable;
 *          config->targetVoltage      = kPMU_AonDigTargetStableVoltage1P0V;
 *          config->mode               = kPMU_HighPowerMode;
 *  @endcode
 * @param config Pointer to the structure @ref pmu_static_aon_dig_config_t.
 */
void PMU_StaticGetAonDigLdoDefaultConfig(pmu_static_aon_dig_config_t *config);

/*!
 * @brief Initialize the AON DIG LDO in static mode.
 *
 * @param base ANADIG_LDO_BBSM peripheral base address.
 * @param config Pointer to the structure @ref pmu_static_aon_dig_config_t.
 */
void PMU_StaticAonDigLdoInit(ANADIG_LDO_BBSM_Type *base, const pmu_static_aon_dig_config_t *config);

/*!
 * @brief Disable the AON DIG LDO.
 *
 * @param base ANADIG_LDO_BBSM peripheral base address.
 */
void PMU_StaticAonDigLdoDeinit(ANADIG_LDO_BBSM_Type *base);

/*!
 * @brief Sets the voltage step of AON DIG LDO in certain setpoint during GPC mode.
 *
 * @note The function provides the feature to set the voltage step to different setpoints.
 *
 * @param setpointMap The map of setpoints should be the OR'ed Value of @ref _pmu_setpoint_map.
 * @param voltageValue The voltage step to be set. See enumeration @ref pmu_aon_dig_target_output_voltage_t.
 */
void PMU_GPCSetAonDigLdoTargetVoltage(uint32_t setpointMap, pmu_aon_dig_target_output_voltage_t voltageValue);

/*!
 * @brief Gets the default config of the BBSM DIG LDO.
 *
 * The default values are:
 *  @code
 *      config->mode                   = kPMU_LowPowerMode;
 *      config->chargePumpCurrent      = kPMU_BbsmDigChargePump12P5nA;
 *      config->dischargeResistorValue = kPMU_BbsmDigDischargeResistor15K;
 *      config->trimValue              = 0U;
 *      config->enablePullDown         = true;
 *      config->enableLdoStable        = false;
 *  @endcode
 *
 * @param config Pointer to @ref pmu_bbsm_dig_config_t.
 */
void PMU_GetBbsmDigLdoDefaultConfig(pmu_bbsm_dig_config_t *config);

/*!
 * @brief  Controls the ON/OFF of the selected LDO in certain setpoints with GPC mode.
 *
 * @param name The name of the selected ldo. Please see enumeration @ref pmu_ldo_name_t for details.
 * @param setpointMap The map of setpoints should be the OR'ed Value of @ref _pmu_setpoint_map, 1b'1
 * means enable specific ldo in that setpoint.
 * For example, the code PMU_GPCEnableLdo(kPMU_PllLdo, 0x1U) means to enable PLL LDO in setpoint 0 and disable
 * PLL LDO in other setpoint.
 */
void PMU_GPCEnableLdo(pmu_ldo_name_t name, uint32_t setpointMap);

/*!
 * @brief Sets the operating mode of the selected LDO in certain setpoints with GPC mode.
 *
 * @param name The name of the selected ldo. Please see enumeration @ref pmu_ldo_name_t for details.
 * @param setpointMap The map of setpoints should be the OR'ed Value of @ref _pmu_setpoint_map.
 * @param mode The operating mode of the selected ldo. Please refer to enumeration @ref pmu_ldo_operate_mode_t for
 * details.
 */
void PMU_GPCSetLdoOperateMode(pmu_ldo_name_t name, uint32_t setpointMap, pmu_ldo_operate_mode_t mode);

/*!
 * @brief Controls the ON/OFF of the selected LDOs' Tracking mode in certain setpoints with GPC mode.
 *
 * @param name The name of the selected ldo. Please see enumeration @ref pmu_ldo_name_t for details.
 * @param setpointMap The map of setpoints that the LDO tracking mode will be enabled in those setpoints, this value
 * should be the OR'ed Value of @ref _pmu_setpoint_map.
 */
void PMU_GPCEnableLdoTrackingMode(pmu_ldo_name_t name, uint32_t setpointMap);

/*!
 * @brief Controls the ON/OFF of the selected LDOs' Bypass mode in certain setpoints with GPC mode.
 *
 * @param name The name of the selected ldo. Please see enumeration @ref pmu_ldo_name_t for details.
 * @param setpointMap The map of setpoints that the LDO bypass mode will be enabled in those setpoints, this value
 * should be the OR'ed Value of @ref _pmu_setpoint_map.
 */
void PMU_GPCEnableLdoBypassMode(pmu_ldo_name_t name, uint32_t setpointMap);

/*!
 * @brief When STBY assert, enable/disable the selected LDO enter it's Low power mode.
 *
 * @param name The name of the selected ldo. Please see enumeration @ref pmu_ldo_name_t for details.
 * @param setpointMap The map of setpoints that the LDO low power mode will be enabled in those setpoints if STBY
 * assert, this value should be the OR'ed Value of @ref _pmu_setpoint_map.
 */
void PMU_GPCEnableLdoStandbyMode(pmu_ldo_name_t name, uint32_t setpointMap);

/*!
 * @}
 */

/*!
 * @name Bandgap Control APIs
 * @{
 */

/*!
 * @brief Disables Bandgap self bias for best noise performance.
 *
 * This function should be invoked after powering up. This function will wait for the bandgap stable and disable the
 * bandgap self bias. After powering up, it need to wait for the bandgap to get stable and then disable Bandgap Self
 * bias for best noise performance.
 */
void PMU_DisableBandgapSelfBiasAfterPowerUp(void);

/*!
 * @brief Enables Bandgap self bias before power down.
 *
 * This function will enable Bandgap self bias feature before powering down or there
 * will be risk of Bandgap not starting properly.
 */
void PMU_EnableBandgapSelfBiasBeforePowerDown(void);

/*!
 * @brief Initialize Bandgap.
 *
 * @param config Pointer to the structure @ref pmu_static_bandgap_config_t.
 */
void PMU_StaticBandgapInit(const pmu_static_bandgap_config_t *config);

/*!
 * @}
 */

/*!
 * @name Body Bias Control APIs
 * @{
 */

/*!
 * @brief Configures Well bias, such as power source, clock source and so on.
 *
 * @param base PMU peripheral base address.
 * @param config Pointer to the @ref pmu_well_bias_config_t structure.
 */
void PMU_WellBiasInit(ANADIG_PMU_Type *base, const pmu_well_bias_config_t *config);

/*!
 * @brief Gets the default configuration of well bias.
 *
 * @param config The pointer to the @ref pmu_well_bias_config_t structure.
 */
void PMU_GetWellBiasDefaultConfig(pmu_well_bias_config_t *config);

/*!
 * @brief Selects the control mode of the Body Bias.
 *
 * @param base PMU peripheral base address.
 * @param name The name of the body bias. Please refer to @ref pmu_body_bias_name_t.
 * @param mode The control mode of the Body Bias. Please refer to @ref pmu_control_mode_t.
 */
void PMU_SetBodyBiasControlMode(ANADIG_PMU_Type *base, pmu_body_bias_name_t name, pmu_control_mode_t mode);

/*!
 * @brief Enables/disables the selected body bias.
 *
 * @param base PMU peripheral base address.
 * @param name The name of the body bias to be turned on/off, please refer to @ref pmu_body_bias_name_t.
 * @param enable Used to turn on/off the specific body bias.
 *              - \b true Enable the selected body bias.
 *              - \b false Disable the selected body bias.
 */
void PMU_EnableBodyBias(ANADIG_PMU_Type *base, pmu_body_bias_name_t name, bool enable);

/*!
 * @brief  Controls the ON/OFF of the selected body bias in certain setpoints with GPC mode.
 *
 * @param name The name of the selected body bias. Please see enumeration @ref pmu_body_bias_name_t for details.
 * @param setpointMap The map of setpoints that the specific body bias will be enabled in those setpoints, this value
 * should be the OR'ed Value of _pmu_setpoint_map.
 */
void PMU_GPCEnableBodyBias(pmu_body_bias_name_t name, uint32_t setpointMap);

/*!
 * @brief Controls the ON/OFF of the selected Body Bias' Wbias power switch in certain setpoints with GPC mode.
 *
 * @param name The name of the selected body bias. Please see the enumeration @ref pmu_body_bias_name_t for details.
 * @param setpointMap The map of setpoints that the specific body bias's wbias power switch will be turn on in those
 * setpoints, this value should be the OR'ed Value of @ref _pmu_setpoint_map.
 */
void PMU_GPCEnableBodyBiasStandbyMode(pmu_body_bias_name_t name, uint32_t setpointMap);

/*!
 * @brief Gets the default config of body bias in GPC mode.
 *
 * @param config Pointer to structure @ref pmu_gpc_body_bias_config_t.
 */
void PMU_GPCGetBodyBiasDefaultConfig(pmu_gpc_body_bias_config_t *config);

/*!
 * @brief Sets the config of the selected Body Bias in GPC mode.
 *
 * @param name The name of the selected body bias. Please see enumeration @ref pmu_body_bias_name_t for details.
 * @param config Pointer to structure @ref pmu_gpc_body_bias_config_t.
 */
void PMU_GPCSetBodyBiasConfig(pmu_body_bias_name_t name, const pmu_gpc_body_bias_config_t *config);

/*!
 * @}
 */

#if defined(__cplusplus)
}
#endif
/*!
 * @}
 */

#endif /* _FSL_PMU_H_ */
