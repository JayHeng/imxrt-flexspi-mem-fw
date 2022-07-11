/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>
#include "fsl_netc_timer.h"

#define NETC_TIMER_EXT_TRIG_FIFO_THRESHOLD_MAX (15U)

void NETC_TimerInit(netc_timer_handle_t *handle, const netc_timer_config_t *config)
{
    uint32_t period = NETC_NANOSECOND_ONE_SECOND / config->refClkHz;

    /* Reset this function */
    NETC_F0_PCI_HDR_TYPE0->PCI_CFC_PCIE_DEV_CTL |= ENETC_PCI_TYPE0_PCI_CFC_PCIE_DEV_CTL_INIT_FLR_MASK;
    while ((NETC_F0_PCI_HDR_TYPE0->PCI_CFC_PCIE_DEV_CTL & ENETC_PCI_TYPE0_PCI_CFC_PCIE_DEV_CTL_INIT_FLR_MASK) != 0U)
    {
    }

    /* Enable master bus and memory access for PCIe and MSI-X. */
    NETC_F0_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK;

    /* Initialize the handle. */
    handle->base      = TMR0_BASE;
    handle->timerFreq = config->refClkHz;

    handle->base->TMR_CTRL =
        ENETC_PF_TMR_TMR_CTRL_TCLK_PERIOD(period) | ENETC_PF_TMR_TMR_CTRL_COPH(config->clkOutputPhase) |
        ENETC_PF_TMR_TMR_CTRL_CIPH(config->clkInputPhase) | ENETC_PF_TMR_TMR_CTRL_TE(config->enableTimer) |
        ENETC_PF_TMR_TMR_CTRL_CK_SEL(config->clockSelect);

    NETC_TimerAdjustFreq(handle, config->defaultPpb);

    handle->base->TMR_CNT_L = 0;
    handle->base->TMR_CNT_H = 0;
}

void NETC_TimerDeinit(netc_timer_handle_t *handle)
{
    /* Disable the 1588 timer. */
    NETC_TimerEnable(handle, false);

    /* Disable master bus and memory access for PCIe and MSI-X. */
    NETC_F0_PCI_HDR_TYPE0->PCI_CFH_CMD &=
        ~(ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);

    memset(handle, 0, sizeof(netc_timer_handle_t));
}

void NETC_TimerEnable(netc_timer_handle_t *handle, bool enable)
{
    if (enable)
    {
        handle->base->TMR_CTRL |= ENETC_PF_TMR_TMR_CTRL_TE_MASK;
    }
    else
    {
        handle->base->TMR_CTRL &= ~ENETC_PF_TMR_TMR_CTRL_TE_MASK;
    }
}

void NETC_TimerConfigureAlarm(netc_timer_handle_t *handle,
                              netc_timer_alarm_index_t alarmId,
                              const netc_timer_alarm_t *alarm)
{
    uint32_t control;

    control = handle->base->TMR_CTRL;
    control &=
        (alarmId == kNETC_TimerAlarm1) ? (~ENETC_PF_TMR_TMR_CTRL_ALM1P_MASK) : (~ENETC_PF_TMR_TMR_CTRL_ALM2P_MASK);
    control |= (alarmId == kNETC_TimerAlarm1) ? (ENETC_PF_TMR_TMR_CTRL_ALM1P(alarm->polarity)) :
                                                ENETC_PF_TMR_TMR_CTRL_ALM2P(alarm->polarity);
    handle->base->TMR_CTRL = control;

    control = handle->base->TMR_ALARM_CTRL;
    control &= (alarmId == kNETC_TimerAlarm1) ?
                   (~(ENETC_PF_TMR_TMR_ALARM_CTRL_PG1_MASK | ENETC_PF_TMR_TMR_ALARM_CTRL_ALARM1_PW_MASK)) :
                   (~(ENETC_PF_TMR_TMR_ALARM_CTRL_PG2_MASK | ENETC_PF_TMR_TMR_ALARM_CTRL_ALARM2_PW_MASK));
    control |= (alarmId == kNETC_TimerAlarm1) ? (ENETC_PF_TMR_TMR_ALARM_CTRL_PG1(alarm->pulseGenSync) |
                                                 ENETC_PF_TMR_TMR_ALARM_CTRL_ALARM1_PW(alarm->pulseWidth)) :
                                                (ENETC_PF_TMR_TMR_ALARM_CTRL_PG2(alarm->pulseGenSync) |
                                                 ENETC_PF_TMR_TMR_ALARM_CTRL_ALARM2_PW(alarm->pulseWidth));
    handle->base->TMR_ALARM_CTRL = control;

    /* Alarm interrupt mask configuration. */
    control = handle->base->TMR_TEMASK;
    control &= (alarmId == kNETC_TimerAlarm1) ? (~ENETC_PF_TMR_TMR_TEMASK_ALM1EN_MASK) :
                                                (~ENETC_PF_TMR_TMR_TEMASK_ALM2EN_MASK);
    control |= (alarmId == kNETC_TimerAlarm1) ? ENETC_PF_TMR_TMR_TEMASK_ALM1EN(alarm->enableInterrupt) :
                                                ENETC_PF_TMR_TMR_TEMASK_ALM2EN(alarm->enableInterrupt);
    handle->base->TMR_TEMASK = control;
}

void NETC_TimerStartAlarm(netc_timer_handle_t *handle, netc_timer_alarm_index_t alarmId, uint64_t nanosecond)
{
    if (alarmId == kNETC_TimerAlarm1)
    {
        handle->base->TMR_ALARM1_L = (uint32_t)nanosecond;
        handle->base->TMR_ALARM1_H = (uint32_t)(nanosecond >> 32U);
    }
    else
    {
        handle->base->TMR_ALARM2_L = (uint32_t)nanosecond;
        handle->base->TMR_ALARM2_H = (uint32_t)(nanosecond >> 32U);
    }
}

void NETC_TimerStopAlarm(netc_timer_handle_t *handle, netc_timer_alarm_index_t alarmId)
{
    if (alarmId == kNETC_TimerAlarm1)
    {
        handle->base->TMR_ALARM1_L = 0;
    }
    else
    {
        handle->base->TMR_ALARM2_L = 0;
    }
}

void NETC_TimerConfigureFIPER(netc_timer_handle_t *handle, const netc_timer_fiper_config_t *config)
{
    assert((config->prescale % 2U) == 0U);

    handle->base->TMR_CTRL &=
        ~(ENETC_PF_TMR_TMR_CTRL_FS_MASK | ENETC_PF_TMR_TMR_CTRL_PP1L_MASK | ENETC_PF_TMR_TMR_CTRL_PP2L_MASK);
    handle->base->TMR_CTRL |= ENETC_PF_TMR_TMR_CTRL_FS(config->startCondition) |
                              ENETC_PF_TMR_TMR_CTRL_PP1L(config->fiper1Loopback) |
                              ENETC_PF_TMR_TMR_CTRL_PP2L(config->fiper1Loopback);
    handle->base->TMR_PRSC = config->prescale;
}

void NETC_TimerStartFIPER(netc_timer_handle_t *handle,
                          netc_timer_fiper_index_t fiperId,
                          const netc_timer_fiper_t *fiper)
{
    uint32_t control;
    uint32_t clear;

    if (fiperId == kNETC_TimerFiper1)
    {
        /* Disbale the FIPER first */
        handle->base->TMR_FIPER_CTRL |= ENETC_PF_TMR_TMR_FIPER_CTRL_FIPER1_DIS_MASK;

        handle->base->TMR_TEMASK &= ~ENETC_PF_TMR_TMR_TEMASK_PP1EN_MASK;
        handle->base->TMR_TEMASK |= ENETC_PF_TMR_TMR_TEMASK_PP1EN(fiper->enableInterrupt);
        clear   = ENETC_PF_TMR_TMR_FIPER_CTRL_FIPER1_PW_MASK | ENETC_PF_TMR_TMR_FIPER_CTRL_PG1_MASK;
        control = ENETC_PF_TMR_TMR_FIPER_CTRL_FIPER1_PW(fiper->pulseWidth) |
                  ENETC_PF_TMR_TMR_FIPER_CTRL_PG1(fiper->pulseGenSync);
    }
    else if (fiperId == kNETC_TimerFiper2)
    {
        /* Disbale the FIPER first */
        handle->base->TMR_FIPER_CTRL |= ENETC_PF_TMR_TMR_FIPER_CTRL_FIPER2_DIS_MASK;

        handle->base->TMR_TEMASK &= ~ENETC_PF_TMR_TMR_TEMASK_PP2EN_MASK;
        handle->base->TMR_TEMASK |= ENETC_PF_TMR_TMR_TEMASK_PP2EN(fiper->enableInterrupt);
        clear   = ENETC_PF_TMR_TMR_FIPER_CTRL_FIPER2_PW_MASK | ENETC_PF_TMR_TMR_FIPER_CTRL_PG2_MASK;
        control = ENETC_PF_TMR_TMR_FIPER_CTRL_FIPER2_PW(fiper->pulseWidth) |
                  ENETC_PF_TMR_TMR_FIPER_CTRL_PG2(fiper->pulseGenSync);
    }
    else
    {
        /* Disbale the FIPER first */
        handle->base->TMR_FIPER_CTRL |= ENETC_PF_TMR_TMR_FIPER_CTRL_FIPER3_DIS_MASK;

        handle->base->TMR_TEMASK &= ~ENETC_PF_TMR_TMR_TEMASK_PP3EN_MASK;
        handle->base->TMR_TEMASK |= ENETC_PF_TMR_TMR_TEMASK_PP3EN(fiper->enableInterrupt);
        clear   = ENETC_PF_TMR_TMR_FIPER_CTRL_FIPER3_PW_MASK | ENETC_PF_TMR_TMR_FIPER_CTRL_PG3_MASK;
        control = ENETC_PF_TMR_TMR_FIPER_CTRL_FIPER3_PW(fiper->pulseWidth) |
                  ENETC_PF_TMR_TMR_FIPER_CTRL_PG3(fiper->pulseGenSync);
    }

    handle->base->TMR_FIPER[fiperId] = fiper->pulsePeriod;
    handle->base->TMR_FIPER_CTRL &= ~clear;
    handle->base->TMR_FIPER_CTRL |= control;

    handle->base->TMR_FIPER_CTRL &= ~(ENETC_PF_TMR_TMR_FIPER_CTRL_FIPER1_DIS_MASK << ((uint32_t)fiperId * 8U));
}

void NETC_TimerStopFIPER(netc_timer_handle_t *handle, netc_timer_fiper_index_t fiperId)
{
    handle->base->TMR_FIPER_CTRL |= (ENETC_PF_TMR_TMR_FIPER_CTRL_FIPER1_DIS_MASK << ((uint32_t)fiperId * 8U));
}

status_t NETC_TimerSetTsFifoThreshold(netc_timer_handle_t *handle, uint8_t threshold)
{
    status_t result = kStatus_Fail;

    if (threshold <= NETC_TIMER_EXT_TRIG_FIFO_THRESHOLD_MAX)
    {
        handle->base->TMR_ECTRL = threshold;
        result                  = kStatus_Success;
    }

    return result;
}

void NETC_TimerConfigureExtPulseTrig(netc_timer_handle_t *handle,
                                     netc_timer_exttrig_index_t extTrigId,
                                     const netc_timer_ext_trig_t *extTrig)
{
    uint32_t control;
    uint32_t clear;

    clear   = (extTrigId == kNETC_TimerExtTrig1) ? ENETC_PF_TMR_TMR_CTRL_ETEP1_MASK : ENETC_PF_TMR_TMR_CTRL_ETEP2_MASK;
    control = (extTrigId == kNETC_TimerExtTrig1) ? ENETC_PF_TMR_TMR_CTRL_ETEP1(extTrig->polarity) :
                                                   ENETC_PF_TMR_TMR_CTRL_ETEP2(extTrig->polarity);
    handle->base->TMR_CTRL &= ~clear;
    handle->base->TMR_CTRL |= control;

    clear = (extTrigId == kNETC_TimerExtTrig1) ?
                (ENETC_PF_TMR_TMR_TEMASK_ETS1EN_MASK | ENETC_PF_TMR_TMR_TEMASK_ETS1_THREN_MASK |
                 ENETC_PF_TMR_TMR_TEMASK_ETS1_OVEN_MASK) :
                (ENETC_PF_TMR_TMR_TEMASK_ETS2EN_MASK | ENETC_PF_TMR_TMR_TEMASK_ETS2_THREN_MASK |
                 ENETC_PF_TMR_TMR_TEMASK_ETS2_OVEN_MASK);
    control = (extTrigId == kNETC_TimerExtTrig1) ?
                  (ENETC_PF_TMR_TMR_TEMASK_ETS1EN(extTrig->enableTsAvailInterrupt) |
                   ENETC_PF_TMR_TMR_TEMASK_ETS1_THREN(extTrig->enableFifoThresholdHitInterrupt) |
                   ENETC_PF_TMR_TMR_TEMASK_ETS1_OVEN(extTrig->enableFifoOverflowInterrupt)) :
                  (ENETC_PF_TMR_TMR_TEMASK_ETS2EN(extTrig->enableTsAvailInterrupt) |
                   ENETC_PF_TMR_TMR_TEMASK_ETS2_THREN(extTrig->enableFifoThresholdHitInterrupt) |
                   ENETC_PF_TMR_TMR_TEMASK_ETS2_OVEN(extTrig->enableFifoOverflowInterrupt));
    handle->base->TMR_TEMASK &= ~clear;
    handle->base->TMR_TEMASK |= control;
}

/*!
 * @brief Check the timestamp available status in FIFO
 *
 * @param base NETC timer peripheral base address.
 * @param tsFifo The FIFO number to read timestamp out.
 * @return True: FIFO is empty, False: FIFO is not empty
 */
static bool NETC_TimerCheckTsFifoEmpty(netc_timer_handle_t *handle, netc_timer_exttrig_index_t extTrigId)
{
    return ((handle->base->TMR_STAT & (ENETC_PF_TMR_TMR_STAT_ETS1_VLD_MASK << (uint32_t)extTrigId)) == 0U);
}

status_t NETC_TimerReadExtPulseCaptureTime(netc_timer_handle_t *handle,
                                           netc_timer_exttrig_index_t extTrigId,
                                           uint64_t *nanosecond)
{
    status_t result = kStatus_Fail;
    uint32_t timeLow, timeHigh;

    if (!NETC_TimerCheckTsFifoEmpty(handle, extTrigId))
    {
        if (extTrigId == kNETC_TimerExtTrig1)
        {
            timeLow  = handle->base->TMR_ETTS1_L;
            timeHigh = handle->base->TMR_ETTS1_H;
        }
        else
        {
            timeLow  = handle->base->TMR_ETTS2_L;
            timeHigh = handle->base->TMR_ETTS2_H;
        }
        *nanosecond = ((uint64_t)timeHigh << 32U) + timeLow;
        result      = kStatus_Success;
    }
    return result;
}

void NETC_TimerGetCurrentTime(netc_timer_handle_t *handle, uint64_t *nanosecond)
{
    uint32_t timeLow, timeHigh;

    /* Read low bytes first to make high bytes store in shadow register */
    timeLow     = handle->base->TMR_CUR_TIME_L;
    timeHigh    = handle->base->TMR_CUR_TIME_H;
    *nanosecond = ((uint64_t)timeHigh << 32U) + timeLow;
}

void NETC_TimerAddOffset(netc_timer_handle_t *handle, int64_t nanosecond)
{
    uint64_t offset;

    if (nanosecond >= 0U)
    {
        offset = nanosecond;
    }
    else
    {
        offset = UINT64_MAX - (uint64_t)(-nanosecond);
    }

    handle->base->TMROFF_L = (uint32_t)offset;
    handle->base->TMROFF_H = (uint32_t)(offset >> 32U);
}

void NETC_TimerAdjustFreq(netc_timer_handle_t *handle, int32_t ppb)
{
    double fractional, inter;
    uint32_t addend;

    fractional = modf((double)NETC_NANOSECOND_ONE_SECOND / (handle->timerFreq + ppb), &inter);
    addend     = (uint32_t)ceil(fractional * UINT32_MAX);

    handle->base->TMR_ADD = addend;
}

void NETC_TimerGetFrtSrtTime(netc_timer_handle_t *handle, uint64_t *frt, uint64_t *srt)
{
    uint32_t tsLow, tsHigh;

    /* Read TMR_FRT_L captures all 64b of FRT_H/L and 64b of SRT_H/L for an atomic read of all 4 registers. */
    tsLow  = handle->base->TMR_FRT_L;
    tsHigh = handle->base->TMR_FRT_H;
    *frt   = ((uint64_t)tsHigh << 32U) + tsLow;

    tsLow  = handle->base->TMR_SRT_L;
    tsHigh = handle->base->TMR_SRT_H;
    *srt   = ((uint64_t)tsHigh << 32U) + tsLow;
}
