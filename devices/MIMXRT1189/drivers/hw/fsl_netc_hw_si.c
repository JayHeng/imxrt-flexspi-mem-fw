/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_netc_hw_si.h"

status_t NETC_SIConfigTxBDR(ENETC_SI_Type *base, uint8_t ring, const netc_si_bdr_config_t *bdrConfig)
{
    const netc_tx_bdr_config_t *txRingConfig = &bdrConfig->txBdConfig[ring];
    uint64_t address;

    /* Set the MSIX entry index triggered when interrupt occurs. */
    base->SIMSITRVR[ring] = txRingConfig->msixEntryIdx;

    /* Map function only supports 32-bit now, so here limit 32-bit first */
#if defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
    address = MEMORY_ConvertMemoryMapAddress((uintptr_t)txRingConfig->bdArray, kMEMORY_Local2DMA);
#else
    address = (uintptr_t)txRingConfig->bdArray;
#endif

    /* Set the tx descriptor start/tail pointer, shall be word aligned. */
    base->BDR[ring].TBBAR0 = NETC_ADDR_LOW_32BIT(address);
    base->BDR[ring].TBBAR1 = NETC_ADDR_HIGH_32BIT(address);
    base->BDR[ring].TBPIR  = 0;
    base->BDR[ring].TBCIR  = 0;
    base->BDR[ring].TBLENR = ENETC_SI_TBLENR_LENGTH(txRingConfig->len / 8U);
    base->BDR[ring].TBIER =
        ENETC_SI_TBIER_TXFIE(txRingConfig->enIntr) | ENETC_SI_TBIER_TXTIE(txRingConfig->enThresIntr);
    base->BDR[ring].TBMR = ENETC_SI_TBMR_EN_MASK | ENETC_SI_TBMR_WRR(txRingConfig->wrrWeight) |
                           ENETC_SI_TBMR_PRIO(txRingConfig->priority) | ENETC_SI_TBMR_VIH(txRingConfig->isVlanInsert);

    return kStatus_Success;
}

status_t NETC_SIConfigRxBDR(ENETC_SI_Type *base, uint8_t ring, const netc_si_bdr_config_t *bdrConfig, bool rxZeroCopy)
{
    const netc_rx_bdr_config_t *rxRingConfig = &bdrConfig->rxBdConfig[ring];
    netc_rx_bd_t *rxDesc;
    uint64_t buffAddr;
    uint64_t address;
    uint8_t *rxBuff;
    uint16_t index;

    if ((rxRingConfig->extendDescEn && ((rxRingConfig->len % 16U) != 0U)) ||
        ((!rxRingConfig->extendDescEn) && (rxRingConfig->len % 8U) != 0U))
    {
        return kStatus_InvalidArgument;
    }

    /* Set the MSIX entry index triggered when interrupt occurs. */
    base->SIMSIRRVR[ring] = rxRingConfig->msixEntryIdx;

    /* Map function only supports 32-bit now, so here limit 32-bit first */
#if defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
    address = MEMORY_ConvertMemoryMapAddress((uintptr_t)rxRingConfig->bdArray, kMEMORY_Local2DMA);
#else
    address = (uintptr_t)rxRingConfig->bdArray;
#endif

    /* Set the Rx descriptor address, shall be word aligned. */
    base->BDR[ring].RBBSR  = bdrConfig->rxBufferSize;
    base->BDR[ring].RBBAR0 = NETC_ADDR_LOW_32BIT(address);
    base->BDR[ring].RBBAR1 = NETC_ADDR_HIGH_32BIT(address);
    base->BDR[ring].RBPIR  = 0;
    base->BDR[ring].RBCIR  = 0;
    base->BDR[ring].RBLENR = rxRingConfig->extendDescEn ? ENETC_SI_RBLENR_LENGTH(rxRingConfig->len / 16U) :
                                                          ENETC_SI_RBLENR_LENGTH(rxRingConfig->len / 8U);
    base->BDR[ring].RBIER = ENETC_SI_RBIER_RXTIE(rxRingConfig->enThresIntr);
    base->BDR[ring].RBICR0 =
        ENETC_SI_RBICR0_ICEN(rxRingConfig->enCoalIntr) | ENETC_SI_RBICR0_ICPT(rxRingConfig->intrThreshold);
    base->BDR[ring].RBICR1 = ENETC_SI_RBICR1_ICTT(rxRingConfig->intrTimerThres);
    base->BDR[ring].RBMR =
        ENETC_SI_RBMR_CRC(rxRingConfig->isKeepCRC) | ENETC_SI_RBMR_BDS(rxRingConfig->extendDescEn) |
        ENETC_SI_RBMR_CM(rxRingConfig->congestionMode) | ENETC_SI_RBMR_AL(rxRingConfig->enHeaderAlign) |
        ENETC_SI_RBMR_VTPD(rxRingConfig->disVlanPresent) | ENETC_SI_RBMR_VTE(rxRingConfig->enVlanExtract);

    /* Initialize the buffers in Rx BD ring. */
    if (!rxZeroCopy)
    {
        rxDesc = rxRingConfig->bdArray;

#if defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
        rxBuff = (uint8_t *)MEMORY_ConvertMemoryMapAddress((uintptr_t)&bdrConfig->rxBufferAddr[0], kMEMORY_Local2DMA);
#else
        rxBuff = &bdrConfig->rxBufferAddr[0];
#endif

        /* When extended BD is enabled, two standard BDs represet one extended BD. */
        if (rxRingConfig->extendDescEn)
        {
            for (index = 0U; index < rxRingConfig->len * 2U; index++)
            {
                if (index % 2U == 0U)
                {
                    buffAddr = (uintptr_t)&rxBuff[(ring + 1U) * (index / 2U) * bdrConfig->rxBufferSize];
                }
                else
                {
                    buffAddr = 0;
                }
                rxDesc->standard.addr = buffAddr;
                rxDesc++;
            }
        }
        else
        {
            for (index = 0U; index < rxRingConfig->len; index++)
            {
                buffAddr              = (uintptr_t)&rxBuff[(ring + 1U) * index * bdrConfig->rxBufferSize];
                rxDesc->standard.addr = buffAddr;
                rxDesc++;
            }
        }
    }

    base->BDR[ring].RBMR |= ENETC_SI_RBMR_EN_MASK;

    return kStatus_Success;
}

void NETC_SIMapVlanToIpv(ENETC_SI_Type *base, netc_vlan_t vlan, uint8_t ipv)
{
    assert(ipv <= 15U);

    uint8_t pcpDei = (vlan.pcp << 1U | vlan.dei);

    if (pcpDei <= 7U)
    {
        base->SIVLANIPVMR0 |= ENETC_SI_SIVLANIPVMR0_PCP_DEI_0(ipv) << (pcpDei * 4U);
    }
    else
    {
        pcpDei -= 8U;
        base->SIVLANIPVMR1 |= ENETC_SI_SIVLANIPVMR1_PCP_DEI_8(ipv) << (pcpDei * 4U);
    }
}

void NETC_SIGetDiscardStatistic(ENETC_SI_Type *base, netc_si_discard_statistic_t *statistic)
{
    statistic->programError = base->SIUPECTR;
    statistic->busError     = base->SIUNSBECTR;
    for (uint32_t i = 0; i < 14U; i++)
    {
        statistic->lackBD[i] = base->BDR[i].RBDCR;
    }
}

void NETC_SIGetTrafficStatistic(ENETC_SI_Type *base, netc_si_traffic_statistic_t *statistic)
{
    statistic->rxOctet          = ((uint64_t)base->SIROCT1 << 32U) | base->SIROCT0;
    statistic->rxFrame          = ((uint64_t)base->SIRFRM1 << 32U) | base->SIRFRM0;
    statistic->rxFrameUnicast   = ((uint64_t)base->SIRUCA1 << 32U) | base->SIRUCA0;
    statistic->rxFrameMulticast = ((uint64_t)base->SIRMCA1 << 32U) | base->SIRMCA0;
    statistic->txOctet          = ((uint64_t)base->SITOCT1 << 32U) | base->SITOCT0;
    statistic->txFrame          = ((uint64_t)base->SITFRM1 << 32U) | base->SITFRM0;
    statistic->txFrameUnicast   = ((uint64_t)base->SITUCA1 << 32U) | base->SITUCA0;
    statistic->txFrameMulticast = ((uint64_t)base->SITMCA1 << 32U) | base->SITMCA0;
}
