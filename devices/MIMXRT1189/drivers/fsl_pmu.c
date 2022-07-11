/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_pmu.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.pmu_1"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PMU_LDO_AON_DIG_TRG_SPX_REG_SETPOINT_COUNTS         4U
#define PMU_LDO_AON_DIG_TRG_SPX_VOLTAGE_SETPOINTX_BIT_WIDTH 8UL

#define PMU_POWER_DETECT_CTRL_REGISTER (ANADIG_PMU->PMU_POWER_DETECT_CTRL)

#define PMU_BIAS_CTRL_WB_CFG_1P8_WELL_SELECT_MASK (0x1U)

#define PMU_BIAS_CTRL_WB_CFG_1P8_VOLTAGE_THRESHOLD_MASK  (0x2U)
#define PMU_BIAS_CTRL_WB_CFG_1P8_VOLTAGE_THRESHOLD_SHIFT 1U
#define PMU_BIAS_CTRL_WB_CFG_1P8_VOLTAGE_THRESHOLD(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << PMU_BIAS_CTRL_WB_CFG_1P8_VOLTAGE_THRESHOLD_SHIFT)) & \
     PMU_BIAS_CTRL_WB_CFG_1P8_VOLTAGE_THRESHOLD_MASK)

#define PMU_BIAS_CTRL_WB_CFG_1P8_DRIVE_STRENGTH_MASK  (0x1CU)
#define PMU_BIAS_CTRL_WB_CFG_1P8_DRIVE_STRENGTH_SHIFT 2U
#define PMU_BIAS_CTRL_WB_CFG_1P8_DRIVE_STRENGTH(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << PMU_BIAS_CTRL_WB_CFG_1P8_DRIVE_STRENGTH_SHIFT)) & \
     PMU_BIAS_CTRL_WB_CFG_1P8_DRIVE_STRENGTH_MASK)

#define PMU_BIAS_CTRL_WB_CFG_1P8_OSCILLATOR_FREQ_MASK  (0x1E0U)
#define PMU_BIAS_CTRL_WB_CFG_1P8_OSCILLATOR_FREQ_SHIFT 5U
#define PMU_BIAS_CTRL_WB_CFG_1P8_OSCILLATOR_FREQ(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << PMU_BIAS_CTRL_WB_CFG_1P8_OSCILLATOR_FREQ_SHIFT)) & \
     PMU_BIAS_CTRL_WB_CFG_1P8_OSCILLATOR_FREQ_MASK)

#define PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(member) \
    ((uint32_t)((ANADIG_PMU_BASE) + (uint32_t)offsetof(ANADIG_PMU_Type, member)))

#define PMU_LDO_ENABLE_SETPOINT_REGISTERS                                 \
    {                                                                     \
        PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_PLL_ENABLE_SP),             \
            PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_AON_ANA_ENABLE_SP),     \
            PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_AON_DIG_ENABLE_SP), 0UL \
    }

#define PMU_LDO_LP_MODE_EN_SETPOINT_REGISTERS                              \
    {                                                                      \
        0UL, PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_AON_ANA_LP_MODE_SP),    \
            PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_AON_DIG_LP_MODE_SP), 0UL \
    }

#define PMU_LDO_TRACKING_EN_SETPOINT_REGISTERS                                 \
    {                                                                          \
        0UL, PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_AON_ANA_TRACKING_EN_SP),    \
            PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_AON_DIG_TRACKING_EN_SP), 0UL \
    }

#define PMU_LDO_BYPASS_EN_SETPOINT_REGISTERS                                 \
    {                                                                        \
        0UL, PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_AON_ANA_BYPASS_EN_SP),    \
            PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_AON_DIG_BYPASS_EN_SP), 0UL \
    }

#define PMU_LDO_STBY_EN_REGISTERS                                          \
    {                                                                      \
        PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(PLL_LDO_STBY_EN_SP),             \
            PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_AON_ANA_STBY_EN_SP),     \
            PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_AON_DIG_STBY_EN_SP), 0UL \
    }

#define PMU_AON_DIG_TRG_REGISTERS                                   \
    {                                                               \
        PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_AON_DIG_TRG_SP0),     \
            PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_AON_DIG_TRG_SP1), \
            PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_AON_DIG_TRG_SP2), \
            PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(LDO_AON_DIG_TRG_SP3)  \
    }

#if (defined(PMU_HAS_FBB) && PMU_HAS_FBB)
#define PMU_BODY_BIAS_ENABLE_REGISTERS                                                                             \
    {                                                                                                              \
        PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(FBB_M7_ENABLE_SP), PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(RBB_SOC_ENABLE_SP), \
            PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(RBB_AON_ENABLE_SP)                                                   \
    }
#else
#define PMU_BODY_BIAS_ENABLE_REGISTERS                                                                             \
    {                                                                                                              \
        PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(RBB_SOC_ENABLE_SP), PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(RBB_AON_ENABLE_SP) \
    }
#endif /* PMU_HAS_FBB */

#if (defined(PMU_HAS_FBB) && PMU_HAS_FBB)
#define PMU_BODY_BIAS_STBY_EN_REGISTERS                                                                              \
    {                                                                                                                \
        PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(FBB_M7_STBY_EN_SP), PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(RBB_SOC_STBY_EN_SP), \
            PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(RBB_AON_STBY_EN_SP)                                                    \
    }
#else
#define PMU_BODY_BIAS_STBY_EN_REGISTERS                                                                              \
    {                                                                                                                \
        PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(RBB_SOC_STBY_EN_SP), PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(RBB_AON_STBY_EN_SP) \
    }
#endif /* PMU_HAS_FBB */

#if (defined(PMU_HAS_FBB) && PMU_HAS_FBB)
#define PMU_BODY_BIAS_CONFIGURE_REGISTERS                                                                          \
    {                                                                                                              \
        PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(FBB_M7_CONFIGURE), PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(RBB_SOC_CONFIGURE), \
            PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(RBB_AON_CONFIGURE)                                                   \
    }
#else
#define PMU_BODY_BIAS_CONFIGURE_REGISTERS                                                                          \
    {                                                                                                              \
        PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(RBB_SOC_CONFIGURE), PMU_GET_ANADIG_PMU_MEMBER_ADDRESS(RBB_AON_CONFIGURE) \
    }
#endif /* PMU_HAS_FBB */
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * brief Selects the control mode of the PLL LDO.
 *
 * param base PMU peripheral base address.
 * param mode The control mode of the PLL LDO. Please refer to pmu_control_mode_t.
 */
void PMU_SetPllLdoControlMode(ANADIG_PMU_Type *base, pmu_control_mode_t mode)
{
    if (mode == kPMU_StaticMode)
    {
        base->PMU_LDO_PLL &= ~ANADIG_PMU_PMU_LDO_PLL_LDO_PLL_CONTROL_MODE_MASK;
    }
    else
    {
        base->PMU_LDO_PLL |= ANADIG_PMU_PMU_LDO_PLL_LDO_PLL_CONTROL_MODE_MASK;
    }
}

/*!
 * brief Enables PLL LDO via AI interface in Static/Software mode.
 *
 * param base PMU peripheral base address.
 */
void PMU_StaticEnablePllLdo(ANADIG_PMU_Type *base)
{
    uint32_t temp32;

    temp32 = PHY_LDO->CTRL0.RW;

    if (temp32 !=
        (PHY_LDO_CTRL0_LINREG_OUTPUT_TRG(0x10) | PHY_LDO_CTRL0_LINREG_EN_MASK | PHY_LDO_CTRL0_LINREG_ILIMIT_EN_MASK))
    {
        PHY_LDO->CTRL0.RW =
            PHY_LDO_CTRL0_LINREG_OUTPUT_TRG(0x10) | PHY_LDO_CTRL0_LINREG_EN_MASK | PHY_LDO_CTRL0_LINREG_ILIMIT_EN_MASK;
        SDK_DelayAtLeastUs(1, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }
}

/*!
 * brief Disables PLL LDO via AI interface in Static/Software mode.
 */
void PMU_StaticDisablePllLdo(void)
{
    PHY_LDO->CTRL0.RW = 0UL;
}

/*!
 * brief Sets the Bypass mode of the AON ANA LDO.
 *
 * param base ANADIG_LDO_BBSM peripheral base address.
 * param enable Enable/Disable bypass mode.
 *          - \b true Enable AON ANA Bypass mode.
 *          - \b false Disable AON ANA Bypass mode.
 */
void PMU_StaticEnableAonAnaLdoBypassMode(ANADIG_LDO_BBSM_Type *base, bool enable)
{
    if (enable == false)
    {
        /* Enable AON ANA LDO and HP mode. */
        base->PMU_LDO_AON_ANA &=
            ~(ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_REG_LP_EN_MASK | ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_REG_DISABLE_MASK);
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Clear Bypass. */
        base->PMU_LDO_AON_ANA &= ~(ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_BYPASS_MODE_EN_MASK);
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Disable Tracking mode. */
        base->PMU_LDO_AON_ANA &= ~ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_TRACK_MODE_EN_MASK;
    }
    else
    {
        /* Enable HP mode. */
        base->PMU_LDO_AON_ANA &= ~ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_REG_LP_EN_MASK;
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Enable Tracking mode. */
        base->PMU_LDO_AON_ANA |= ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_TRACK_MODE_EN_MASK;
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Enabled Bypass. */
        base->PMU_LDO_AON_ANA |= ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_BYPASS_MODE_EN_MASK;
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Disable AON ANA LDO. */
        base->PMU_LDO_AON_ANA |= ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_REG_DISABLE_MASK;
    }
}

/*!
 * brief Fill the AON ANA LDO configuration structure with default settings.
 *
 * The default values are:
 * code
 *      config->mode                   = kPMU_HighPowerMode;
        config->enable2mALoad          = true;
        config->enable20uALoad         = false;
        config->enable4mALoad          = true;
        config->enableStandbyMode      = false;
        config->driverStrength         = kPMU_AonAnaLdoDriverStrength0;
        config->brownOutDetectorConfig = kPMU_AonAnaLdoBrownOutDetectorDisable;
        config->chargePumpCurrent      = kPMU_AonAnaChargePump300nA;
        config->outputRange            = kPMU_AonAnaLdoOutputFrom1P77To1P83;
 * endcode
 *
 * param config Pointer to the structure pmu_static_aon_ana_ldo_config_t. Please refer to @ref
 * pmu_static_aon_ana_ldo_config_t.
 */
void PMU_StaticGetAonAnaLdoDefaultConfig(pmu_static_aon_ana_ldo_config_t *config)
{
    assert(config != NULL);

    (void)memset(config, 0, sizeof(*config));

    config->mode              = kPMU_HighPowerMode;
    config->enable2mALoad     = true;
    config->enable20uALoad    = false;
    config->enable4mALoad     = true;
    config->enableStandbyMode = false;
}

/*!
 * brief Initialize the AON ANA LDO in Static/Sofware Mode.
 *
 * param base ANADIG_LDO_BBSM peripheral base address.
 * param config Pointer to the structure pmu_static_aon_ana_ldo_config_t. Please refer to @ref
 * pmu_static_aon_ana_ldo_config_t.
 */
void PMU_StaticAonAnaLdoInit(ANADIG_LDO_BBSM_Type *base, const pmu_static_aon_ana_ldo_config_t *config)
{
    assert(config != NULL);

    uint32_t regValue = base->PMU_LDO_AON_ANA;

    regValue &=
        ~(ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_REG_LP_EN_MASK | ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_PULL_DOWN_2MA_EN_MASK |
          ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_ALWAYS_4MA_PULLDOWN_EN_MASK |
          ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_PULL_DOWN_20UA_EN_MASK | ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_STANDBY_EN_MASK);

    if ((config->mode) == kPMU_LowPowerMode)
    {
        regValue |= ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_REG_LP_EN_MASK;
    }
    regValue |= ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_PULL_DOWN_2MA_EN(config->enable2mALoad);
    regValue |= ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_ALWAYS_4MA_PULLDOWN_EN(config->enable4mALoad);
    regValue |= ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_PULL_DOWN_20UA_EN(config->enable20uALoad);
    regValue |= ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_STANDBY_EN(config->enableStandbyMode);

    base->PMU_LDO_AON_ANA = regValue;

    /* Enable AON ANA DIG. */
    base->PMU_LDO_AON_ANA &= ~ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_REG_DISABLE_MASK;
}

/*!
 * brief Disable the output of AON ANA LDO.
 *
 * param base ANADIG_LDO_BBSM peripheral base address.
 */
void PMU_StaticAonAnaLdoDeinit(ANADIG_LDO_BBSM_Type *base)
{
    /* Disable AON ANA LDO. */
    base->PMU_LDO_AON_ANA |= ANADIG_LDO_BBSM_PMU_LDO_AON_ANA_REG_DISABLE_MASK;
}

/*!
 * brief Turn on/off Bypass mode of the AON DIG LDO in Static/Software mode.
 *
 * param base ANADIG_LDO_BBSM peripheral base address.
 * param enable
 *              true    -   Turn on Bypass mode of the AON DIG LDO.
 *              false   -   Turn off Bypass mode of the AON DIG LDO.
 */
void PMU_StaticEnableAonDigLdoBypassMode(ANADIG_LDO_BBSM_Type *base, bool enable)
{
    if (enable)
    {
        /* tracking */
        base->PMU_LDO_AON_DIG |= ANADIG_LDO_BBSM_PMU_LDO_AON_DIG_TRACKING_MODE_MASK;
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* set BYPASS */
        base->PMU_LDO_AON_DIG |= ANADIG_LDO_BBSM_PMU_LDO_AON_DIG_BYPASS_MODE_MASK;
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Disable AON DIG LDO */
        base->PMU_LDO_AON_DIG &= ~ANADIG_LDO_BBSM_PMU_LDO_AON_DIG_REG_EN_MASK;
    }
    else
    {
        /* Enable AON DIG LDO and HP mode */
        base->PMU_LDO_AON_DIG |= (ANADIG_LDO_BBSM_PMU_LDO_AON_DIG_REG_EN_MASK);
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Clear BYPASS */
        base->PMU_LDO_AON_DIG &= ~ANADIG_LDO_BBSM_PMU_LDO_AON_DIG_BYPASS_MODE_MASK;
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Disable tracking */
        base->PMU_LDO_AON_DIG &= ~ANADIG_LDO_BBSM_PMU_LDO_AON_DIG_TRACKING_MODE_MASK;
    }
}

/*!
 * @brief Gets the default configuration of AON DIG LDO.
 *
 * @param config Pointer to the structure pmu_static_aon_dig_config_t. Please refer to @ref
 * pmu_static_aon_dig_config_t.
 */
void PMU_StaticGetAonDigLdoDefaultConfig(pmu_static_aon_dig_config_t *config)
{
    assert(config != NULL);

    (void)memset(config, 0, sizeof(*config));

    config->voltageStepTime = kPMU_AonDigVoltageStepInc50us;
    config->targetVoltage   = kPMU_AonDigTargetStableVoltage1P0V;
}

/*!
 * @brief Initialize the AON DIG LDO in static mode.
 *
 * @param base ANADIG_LDO_BBSM peripheral base address.
 * @param config Pointer to the structure pmu_static_aon_dig_config_t. Please refer to @ref
 * pmu_static_aon_dig_config_t.
 */
void PMU_StaticAonDigLdoInit(ANADIG_LDO_BBSM_Type *base, const pmu_static_aon_dig_config_t *config)
{
    assert(config != NULL);

    uint32_t temp32 = base->PMU_LDO_AON_DIG;

    temp32 &= ~ANADIG_LDO_BBSM_PMU_LDO_AON_DIG_VOLTAGE_SELECT_MASK;
    temp32 |= ANADIG_LDO_BBSM_PMU_LDO_AON_DIG_VOLTAGE_SELECT(config->targetVoltage);
    base->PMU_LDO_AON_DIG = temp32;

    /* Enable AON DIG LDO. */
    base->PMU_LDO_AON_DIG |= ANADIG_LDO_BBSM_PMU_LDO_AON_DIG_REG_EN_MASK;
    SDK_DelayAtLeastUs(125U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    PMU_POWER_DETECT_CTRL_REGISTER |= ANADIG_PMU_PMU_POWER_DETECT_CTRL_CKGB_AON1P0_MASK;
}

/*!
 * @brief Disable the AON DIG LDO.
 *
 * @param base ANADIG_LDO_BBSM peripheral base address.
 */
void PMU_StaticAonDigLdoDeinit(ANADIG_LDO_BBSM_Type *base)
{
    PMU_POWER_DETECT_CTRL_REGISTER &= ~ANADIG_PMU_PMU_POWER_DETECT_CTRL_CKGB_AON1P0_MASK;
    base->PMU_LDO_AON_DIG &= ~ANADIG_LDO_BBSM_PMU_LDO_AON_DIG_REG_EN_MASK;
}

/*!
 * brief Gets the default config of the BBSM DIG LDO.
 *
 * The default values are:
 *  code
 *      config->mode                   = kPMU_LowPowerMode;
 *      config->chargePumpCurrent      = kPMU_BbsmDigChargePump12P5nA;
 *      config->dischargeResistorValue = kPMU_BbsmDigDischargeResistor15K;
 *      config->trimValue              = 0U;
 *      config->enablePullDown         = true;
 *      config->enableLdoStable        = false;
 *  endcode
 *
 * param config Pointer to the structure pmu_bbsm_dig_config_t. Please refer to pmu_bbsm_dig_config_t.
 */
void PMU_GetBbsmDigLdoDefaultConfig(pmu_bbsm_dig_config_t *config)
{
    assert(config != NULL);

    (void)memset(config, 0, sizeof(*config));

    config->mode                   = kPMU_LowPowerMode;
    config->chargePumpCurrent      = kPMU_BbsmDigChargePump12P5nA;
    config->dischargeResistorValue = kPMU_BbsmDigDischargeResistor15K;
    config->trimValue              = 0U;
    config->enablePullDown         = true;
    config->enableLdoStable        = false;
}

/*!
 * brief Disables Bandgap self bias for best noise performance.
 *
 * This function waits for the bandgap to be stable and disables the bandgap self bias.
 * After being powered up, it needs to wait for the bandgap stable to be stable and then disable Bandgap
 * Self bias for best noise performance.
 */
void PMU_DisableBandgapSelfBiasAfterPowerUp(void)
{
    uint32_t regValue;

    /* Wait Bandgap stable. */
    do
    {
        regValue = VMBANDGAP->STAT0.RW;
    } while ((regValue & VMBANDGAP_STAT0_REFTOP_VBGUP_MASK) == 0UL);

    /* Disable Bandgap self bias for best noise performance. */
    VMBANDGAP->CTRL0.SET = VMBANDGAP_CTRL0_REFTOP_SELFBIASOFF_MASK;
}

/*!
 * brief Enables Bandgap self bias before power down.
 *
 * This function will enable Bandgap self bias feature before powering down or there
 * will be risk of Bandgap not starting properly.
 */
void PMU_EnableBandgapSelfBiasBeforePowerDown(void)
{
    VMBANDGAP->CTRL0.CLR = VMBANDGAP_CTRL0_REFTOP_SELFBIASOFF_MASK;
}

/*!
 * brief Init Bandgap.
 *
 * param config. Pointer to the structure pmu_static_bandgap_config_t. Please refer to pmu_static_bandgap_config_t.
 */
void PMU_StaticBandgapInit(const pmu_static_bandgap_config_t *config)
{
    assert(config != NULL);

    uint32_t temp32;

    temp32 = VMBANDGAP->CTRL0.RW;
    temp32 &= ~(VMBANDGAP_CTRL0_REFTOP_PWD_MASK | VMBANDGAP_CTRL0_REFTOP_LINREGREF_PWD_MASK |
                VMBANDGAP_CTRL0_REFTOP_PWDVBGUP_MASK | VMBANDGAP_CTRL0_REFTOP_LOWPOWER_MASK);
    temp32 |= ((uint32_t)(config->powerDownOption) &
               (VMBANDGAP_CTRL0_REFTOP_PWD_MASK | VMBANDGAP_CTRL0_REFTOP_LINREGREF_PWD_MASK |
                VMBANDGAP_CTRL0_REFTOP_PWDVBGUP_MASK));
    temp32 |= VMBANDGAP_CTRL0_REFTOP_LOWPOWER(config->enableLowPowerMode);

    VMBANDGAP->CTRL0.RW = temp32;
}

/*!
 * brief Configures Well bias, such as power source, clock source and so on.
 *
 * param base PMU peripheral base address.
 * param config Pointer to the pmu_well_bias_config_t structure.
 */
void PMU_WellBiasInit(ANADIG_PMU_Type *base, const pmu_well_bias_config_t *config)
{
    assert(config != NULL);

    uint32_t tmp32;

    tmp32 = base->PMU_BIAS_CTRL;
    tmp32 &= ~(ANADIG_PMU_PMU_BIAS_CTRL_WB_CFG_1P8_MASK | ANADIG_PMU_PMU_BIAS_CTRL_WB_VDD_SEL_1P8_MASK);
    tmp32 |= ((uint32_t)config->wellBiasOption.wellBiasData &
              (ANADIG_PMU_PMU_BIAS_CTRL_WB_CFG_1P8_MASK | ANADIG_PMU_PMU_BIAS_CTRL_WB_VDD_SEL_1P8_MASK));
    base->PMU_BIAS_CTRL = tmp32;

    tmp32 = base->PMU_BIAS_CTRL2;
    tmp32 &= ~ANADIG_PMU_PMU_BIAS_CTRL2_WB_ADJ_1P8_MASK;
    tmp32 |= ANADIG_PMU_PMU_BIAS_CTRL2_WB_ADJ_1P8(config->adjustment);
    base->PMU_BIAS_CTRL2 = tmp32;
}

/*!
 * brief Enables/disables the selected body bias.
 *
 * param base PMU peripheral base address.
 * param name The name of the body bias to be turned on/off, please refer to pmu_body_bias_name_t.
 * param enable Used to turn on/off the specific body bias.
 *              - \b true Enable the selected body bias.
 *              - \b false Disable the selected body bias.
 */
void PMU_GetWellBiasDefaultConfig(pmu_well_bias_config_t *config)
{
    assert(config != NULL);

    (void)memset(config, 0, sizeof(*config));

    config->wellBiasOption.wellBiasData = 0U;
    config->adjustment                  = kPMU_Cref0fFCspl0fFDeltaC0fF;
}

/*!
 * brief Enables/disables the selected body bias.
 *
 * param base PMU peripheral base address.
 * param name The name of the body bias to be turned on/off, please refer to pmu_body_bias_name_t.
 * param enable Used to turn on/off the specific body bias.
 *              - \b true Enable the selected body bias.
 *              - \b false Disable the selected body bias.
 */
void PMU_EnableBodyBias(ANADIG_PMU_Type *base, pmu_body_bias_name_t name, bool enable)
{
    uint32_t tmp32;

    if (enable)
    {
        switch (name)
        {
#if (defined(PMU_HAS_FBB) && PMU_HAS_FBB)
            case kPMU_FBB_CM7:
            {
                tmp32 = base->PMU_BIAS_CTRL;
                tmp32 &= ~PMU_BIAS_CTRL_WB_CFG_1P8_WELL_SELECT_MASK;
                tmp32 |= PMU_BIAS_CTRL_WB_CFG_1P8_VOLTAGE_THRESHOLD_MASK;
                base->PMU_BIAS_CTRL = tmp32;

                tmp32 = base->PMU_BIAS_CTRL2;
                tmp32 &= ~(ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8_MASK);
                tmp32 |= ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8(1U) | ANADIG_PMU_PMU_BIAS_CTRL2_WB_EN_MASK;
                base->PMU_BIAS_CTRL2 = tmp32;

                while ((base->PMU_BIAS_CTRL2 & ANADIG_PMU_PMU_BIAS_CTRL2_WB_OK_MASK) !=
                       ANADIG_PMU_PMU_BIAS_CTRL2_WB_OK_MASK)
                {
                }
                break;
            }
#endif /* PMU_HAS_FBB */
            case kPMU_RBB_SOC:
            {
                tmp32 = base->PMU_BIAS_CTRL;
                tmp32 &= ~(PMU_BIAS_CTRL_WB_CFG_1P8_WELL_SELECT_MASK | PMU_BIAS_CTRL_WB_CFG_1P8_VOLTAGE_THRESHOLD_MASK);
                base->PMU_BIAS_CTRL = tmp32;

                tmp32 = base->PMU_BIAS_CTRL2;
                tmp32 &= ~(ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8_MASK);
                tmp32 |= ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8(2U) | ANADIG_PMU_PMU_BIAS_CTRL2_WB_EN_MASK;
                base->PMU_BIAS_CTRL2 = tmp32;
                while ((base->PMU_BIAS_CTRL2 & ANADIG_PMU_PMU_BIAS_CTRL2_WB_OK_MASK) !=
                       ANADIG_PMU_PMU_BIAS_CTRL2_WB_OK_MASK)
                {
                }
                break;
            }
            case kPMU_RBB_AON:
            {
                tmp32 = base->PMU_BIAS_CTRL;
                tmp32 &= ~(PMU_BIAS_CTRL_WB_CFG_1P8_WELL_SELECT_MASK | PMU_BIAS_CTRL_WB_CFG_1P8_VOLTAGE_THRESHOLD_MASK);
                base->PMU_BIAS_CTRL = tmp32;

                tmp32 = base->PMU_BIAS_CTRL2;
                tmp32 &= ~(ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8_MASK);
                tmp32 |= ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8(4U) | ANADIG_PMU_PMU_BIAS_CTRL2_WB_EN_MASK;
                base->PMU_BIAS_CTRL2 = tmp32;
                while ((base->PMU_BIAS_CTRL2 & ANADIG_PMU_PMU_BIAS_CTRL2_WB_OK_MASK) !=
                       ANADIG_PMU_PMU_BIAS_CTRL2_WB_OK_MASK)
                {
                }
                break;
            }
            default:
                /* This branch should never be hit. */
                break;
        }
    }
    else
    {
        base->PMU_BIAS_CTRL2 &=
            ~(ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8_MASK | ANADIG_PMU_PMU_BIAS_CTRL2_WB_EN_MASK);
    }
}

/*!
 * brief Gets the default config of body bias in GPC mode.
 *
 * param config Pointer to the structure pmu_gpc_body_bias_config_t.
 */
void PMU_GPCGetBodyBiasDefaultConfig(pmu_gpc_body_bias_config_t *config)
{
    assert(config != NULL);

    config->PWELLRegulatorSize = 1U;
    config->NWELLRegulatorSize = 1U;
    config->oscillatorSize     = 7U;
    config->regulatorStrength  = 5U;
}
