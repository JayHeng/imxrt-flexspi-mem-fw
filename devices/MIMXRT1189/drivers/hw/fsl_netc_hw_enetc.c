/*
 * Copyright 2021-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_netc_hw_enetc.h"

void NETC_EnetcSetSIMacAddr(NETC_ENETC_Type *base, uint8_t si, uint8_t *macAddr)
{
    /* This API is for VSI. */
    assert(si != 0U);

    uint32_t address;

    /* Set physical address lower register. */
    address                   = ((uint32_t *)macAddr)[0];
    base->NUM_SI[si].PSIPMAR0 = address;

    /* Set physical address high register. */
    address                   = (((uint32_t)macAddr[5] << 8U) | (uint32_t)macAddr[4]);
    base->NUM_SI[si].PSIPMAR1 = address;
}

status_t NETC_EnetcConfigureSI(NETC_ENETC_Type *base, uint8_t si, const netc_hw_enetc_si_config_t *psConfig)
{
    uint16_t txBdrMax  = (base->ECAPR2 & NETC_ENETC_ECAPR2_NUM_TX_BDR_MASK) >> NETC_ENETC_ECAPR2_NUM_TX_BDR_SHIFT;
    uint16_t rxBdrMax  = (base->ECAPR2 & NETC_ENETC_ECAPR2_NUM_RX_BDR_MASK) >> NETC_ENETC_ECAPR2_NUM_RX_BDR_SHIFT;
    uint16_t siMax     = 1U + ((base->ECAPR1 & NETC_ENETC_ECAPR1_NUM_VSI_MASK) >> NETC_ENETC_ECAPR1_NUM_VSI_SHIFT);
    uint16_t txLeftBdr = txBdrMax;
    uint16_t rxLeftBdr = rxBdrMax;

    /* SI index ranges from 0 ~ max-1. */
    if (si >= siMax)
    {
        return kStatus_InvalidArgument;
    }

    /* Minus the ring number used by SIs in front. */
    for (uint8_t i = 0; i < si; i++)
    {
        txLeftBdr -=
            ((base->NUM_SI[i].PSICFGR0 & NETC_ENETC_PSICFGR0_NUM_TX_BDR_MASK) >> NETC_ENETC_PSICFGR0_NUM_TX_BDR_SHIFT);
        rxLeftBdr -=
            ((base->NUM_SI[i].PSICFGR0 & NETC_ENETC_PSICFGR0_NUM_RX_BDR_MASK) >> NETC_ENETC_PSICFGR0_NUM_RX_BDR_SHIFT);
    }

    if ((psConfig->txRingUse > txLeftBdr) || (psConfig->rxRingUse > rxLeftBdr))
    {
        return kStatus_NETC_LackOfResource;
    }

    /* Configure the station interface. */
    base->NUM_SI[si].PSICFGR0 =
        NETC_ENETC_PSICFGR0_SIBW(psConfig->bandWeight) | NETC_ENETC_PSICFGR0_SIVC(psConfig->vlanCtrl) |
        NETC_ENETC_PSICFGR0_ASE(psConfig->antiSpoofEnable) | NETC_ENETC_PSICFGR0_SIVIE(psConfig->vlanInsertEnable) |
        NETC_ENETC_PSICFGR0_VTE(psConfig->vlanExtractEnable) | NETC_ENETC_PSICFGR0_SPE(psConfig->sourcePruneEnable) |
        NETC_ENETC_PSICFGR0_NUM_TX_BDR(psConfig->txRingUse) | NETC_ENETC_PSICFGR0_NUM_RX_BDR(psConfig->rxRingUse);

    return kStatus_Success;
}

status_t NETC_EnetcSetMsixEntryNum(NETC_ENETC_Type *base, uint8_t si, uint32_t msixNum)
{
    uint16_t msixEntryMax = ((base->ECAPR1 & NETC_ENETC_ECAPR1_NUM_MSIX_MASK) >> NETC_ENETC_ECAPR1_NUM_MSIX_SHIFT) + 1U;
    uint16_t leftEntry    = msixEntryMax;
    uint32_t entryNum;

    /* Minus the MSIX entry used by SIs in front. */
    for (uint8_t i = 0; i < si; i++)
    {
        leftEntry -= (1U + ((base->NUM_SI[i].PSICFGR2 & NETC_ENETC_PSICFGR2_NUM_MSIX_MASK) >>
                            NETC_ENETC_PSICFGR2_NUM_MSIX_SHIFT));
    }

    if (msixNum > leftEntry)
    {
        return kStatus_NETC_LackOfResource;
    }

    entryNum                  = (msixNum != 0U) ? msixNum : 1;
    base->NUM_SI[si].PSICFGR2 = NETC_ENETC_PSICFGR2_NUM_MSIX(entryNum - 1U);

    return kStatus_Success;
}

void NETC_GetPortDiscardStatistic(NETC_ENETC_Type *base, netc_enetc_port_discard_statistic_t *statistic)
{
    for (uint32_t i = 0U; i < 4U; i++)
    {
        statistic->ingressDR[i] = base->PICDRADCR[i].PICDRDCR;
    }
    statistic->broadcastReject   = base->PBFDSIR;
    statistic->smacPruning       = base->PFDMSAPR;
    statistic->unicastMacFilt    = base->PUFDMFR;
    statistic->multicastMacFilt  = base->PMFDMFR;
    statistic->unicastVlanFilt   = base->PUFDVFR;
    statistic->multicastVlanFilt = base->PMFDVFR;
    statistic->boradcastVlanFilt = base->PBFDVFR;
}

void NETC_EnetcEnablePromiscuous(NETC_ENETC_Type *base, uint8_t si, bool enableUCPromis, bool enableMCPromis)
{
    if (enableUCPromis)
    {
        base->PSIPMMR |= (NETC_ENETC_PSIPMMR_SI0_MAC_UP_MASK << si);
    }
    else
    {
        base->PSIPMMR &= ~(NETC_ENETC_PSIPMMR_SI0_MAC_UP_MASK << si);
    }

    if (enableMCPromis)
    {
        base->PSIPMMR |= (NETC_ENETC_PSIPMMR_SI0_MAC_MP_MASK << si);
    }
    else
    {
        base->PSIPMMR &= ~(NETC_ENETC_PSIPMMR_SI0_MAC_MP_MASK << si);
    }
}

void NETC_EnetcConfigureVlanFilter(NETC_ENETC_Type *base, uint8_t si, netc_si_l2vf_config_t *config)
{
    if (config->acceptUntagged)
    {
        base->PSIPVMR |= (NETC_ENETC_PSIPVMR_SI0_VLAN_UTA_MASK << si);
    }
    else
    {
        base->PSIPVMR &= ~(NETC_ENETC_PSIPVMR_SI0_VLAN_UTA_MASK << si);
    }

    if (config->enPromis)
    {
        base->PSIPVMR |= (NETC_ENETC_PSIPVMR_SI0_VLAN_P_MASK << si);
    }
    else
    {
        base->PSIPVMR &= ~(NETC_ENETC_PSIPVMR_SI0_VLAN_P_MASK << si);
    }

    base->PSIVLANFMR = NETC_ENETC_PSIVLANFMR_VS(config->useOuterVlanTag);
}

void NETC_EnetcAddMacAddrHash(NETC_ENETC_Type *base, uint8_t si, netc_packet_type_t type, uint8_t hashIndex)
{
    assert(type != kNETC_PacketBroadcast);

    if (type == kNETC_PacketUnicast)
    {
        if (hashIndex < 32U)
        {
            base->NUM_SI[si].PSIUMHFR0 |= (1U << hashIndex);
        }
        else
        {
            base->NUM_SI[si].PSIUMHFR1 |= (1U << (hashIndex - 32U));
        }
    }
    else
    {
        if (hashIndex < 32U)
        {
            base->NUM_SI[si].PSIMMHFR0 |= (1U << hashIndex);
        }
        else
        {
            base->NUM_SI[si].PSIMMHFR1 |= (1U << (hashIndex - 32U));
        }
    }
}

void NETC_EnetcDelMacAddrHash(NETC_ENETC_Type *base, uint8_t si, netc_packet_type_t type, uint8_t hashIndex)
{
    assert(type != kNETC_PacketBroadcast);

    if (type == kNETC_PacketUnicast)
    {
        if (hashIndex < 32U)
        {
            base->NUM_SI[si].PSIUMHFR0 &= ~(1U << hashIndex);
        }
        else
        {
            base->NUM_SI[si].PSIUMHFR1 &= ~(1U << (hashIndex - 32U));
        }
    }
    else
    {
        if (hashIndex < 32U)
        {
            base->NUM_SI[si].PSIMMHFR0 &= ~(1U << hashIndex);
        }
        else
        {
            base->NUM_SI[si].PSIMMHFR1 &= ~(1U << (hashIndex - 32U));
        }
    }
}

void NETC_EnetcAddVlanHash(NETC_ENETC_Type *base, uint8_t si, uint8_t hashIndex)
{
    if (hashIndex < 32U)
    {
        base->NUM_SI[si].PSIVHFR0 |= (1U << hashIndex);
    }
    else
    {
        base->NUM_SI[si].PSIVHFR1 |= (1U << (hashIndex - 32U));
    }
}

void NETC_EnetcDelVlanHash(NETC_ENETC_Type *base, uint8_t si, uint8_t hashIndex)
{
    if (hashIndex < 32U)
    {
        base->NUM_SI[si].PSIVHFR0 &= ~(1U << hashIndex);
    }
    else
    {
        base->NUM_SI[si].PSIVHFR1 &= ~(1U << (hashIndex - 32U));
    }
}
