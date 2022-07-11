/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_netc_hw_port.h"

bool NETC_PortIsPseudo(NETC_PORT_Type *base)
{
    return ((base->PCAPR & NETC_PORT_PCAPR_LINK_TYPE_MASK) != 0U);
}

void NETC_PortSetMacAddr(NETC_PORT_Type *base, const uint8_t *macAddr)
{
    uint32_t address;

    /* Set physical address lower register. */
    address     = ((uint32_t *)macAddr)[0];
    base->PMAR0 = address;

    /* Set physical address high register. */
    address     = ((uint32_t)macAddr[5] << 8U) | (uint32_t)macAddr[4];
    base->PMAR1 = NETC_PORT_PMAR1_PRIM_MAC_ADDR(address);
}

status_t NETC_PortConfig(NETC_PORT_Type *base, const netc_port_common_t *config)
{
    NETC_PortSetParser(base, &config->parser);
    NETC_PortSetVlanClassify(base, &config->acceptTpid);
    NETC_PortSetQosC(base, &config->qosMode);
    ENETC_PortConfigTGS(base, &config->timeGate);
    base->PCR = NETC_PORT_PCR_PSPEED(config->pSpeed) | NETC_PORT_PCR_FCSEA(!config->stompFcs) |
                NETC_PORT_PCR_TIMER_CS(config->rxTsSelect);
    base->PSGCR = NETC_PORT_PSGCR_OGC(config->rxSgCfg.ogcMode) | NETC_PORT_PSGCR_PDELAY(config->rxSgCfg.pDelay);

    /* Configure Port SDU overhead */
    base->PRXSDUOR = NETC_PORT_PRXSDUOR_MACSEC_BCO(config->sduCfg.rxMacsecBco) |
                     NETC_PORT_PRXSDUOR_PPDU_BCO(config->sduCfg.rxPpduBco);
    base->PRXSDUOR = NETC_PORT_PTXSDUOR_MACSEC_BCO(config->sduCfg.txMacsecBco) |
                     NETC_PORT_PTXSDUOR_PPDU_BCO(config->sduCfg.txPpduBco);
    return kStatus_Success;
}

status_t NETC_PortConfigEthMac(NETC_ETH_LINK_Type *base, const netc_port_ethmac_t *config)
{
    uint32_t reg = 0;

    /* Set interface mode */
    reg = NETC_ETH_LINK_PM0_IF_MODE_IFMODE(config->miiMode);

    /* Enable reverse mode */
    if (config->enableRevMii)
    {
        reg |= NETC_ETH_LINK_PM0_IF_MODE_REVMII_MASK;
    }

    if ((config->miiMode == kNETC_MiiMode) || (config->miiMode == kNETC_RmiiMode))
    {
        if (config->miiSpeed == kNETC_MiiSpeed10M)
        {
            reg |= NETC_ETH_LINK_PM0_IF_MODE_M10_MASK;
        }
        else if (config->miiSpeed == kNETC_MiiSpeed100M)
        {
            reg &= ~NETC_ETH_LINK_PM0_IF_MODE_M10_MASK;
        }
        else
        {
            return kStatus_Fail;
        }
        reg |= NETC_ETH_LINK_PM0_IF_MODE_HD(!config->miiDuplex);
    }
    else if (config->miiMode == kNETC_RgmiiMode)
    {
        if (config->miiSpeed == kNETC_MiiSpeed1000M)
        {
            reg |= NETC_ETH_LINK_PM0_IF_MODE_SSP(config->miiSpeed);
            reg |= NETC_ETH_LINK_PM0_IF_MODE_SFD(1);
        }
        else
        {
            reg |= NETC_ETH_LINK_PM0_IF_MODE_SSP(!config->miiSpeed);
            reg |= NETC_ETH_LINK_PM0_IF_MODE_SFD(config->miiDuplex);
        }
    }
    else /* kNETC_GmiiMode, force 1Gbps and full speed */
    {
        /* Intentional empty */
    }
    base->PM0_IF_MODE = reg;
    base->PM1_IF_MODE = reg;

    base->MAC_MERGE_MMCSR = NETC_ETH_LINK_MAC_MERGE_MMCSR_VT(config->mergeVerifyTime) |
                            NETC_ETH_LINK_MAC_MERGE_MMCSR_VDIS(!config->enMergeVerify) |
                            NETC_ETH_LINK_MAC_MERGE_MMCSR_ME(config->preemptMode);

    reg = NETC_ETH_LINK_PM0_COMMAND_CONFIG_TX_EN_MASK | NETC_ETH_LINK_PM0_COMMAND_CONFIG_RX_EN_MASK |
          NETC_ETH_LINK_PM0_COMMAND_CONFIG_TS_MODE(config->txTsSelect);
    /* Enable Tx/Rx */
    base->PM0_COMMAND_CONFIG = reg;
    base->PM1_COMMAND_CONFIG = reg;

    return kStatus_Success;
}

status_t NETC_PortEnableLoopback(NETC_ETH_LINK_Type *base, bool enable)
{
    if (enable)
    {
        base->PM0_COMMAND_CONFIG |= NETC_ETH_LINK_PM0_COMMAND_CONFIG_LOOP_ENA_MASK | 0x800;
        base->PM1_COMMAND_CONFIG |= NETC_ETH_LINK_PM1_COMMAND_CONFIG_LOOP_ENA_MASK;
    }
    else
    {
        base->PM0_COMMAND_CONFIG &= ~(NETC_ETH_LINK_PM0_COMMAND_CONFIG_LOOP_ENA_MASK | 0x800);
        base->PM1_COMMAND_CONFIG &= ~NETC_ETH_LINK_PM1_COMMAND_CONFIG_LOOP_ENA_MASK;
    }

    return kStatus_Success;
}

void NETC_PortGetDiscardStatistic(NETC_PORT_Type *base,
                                  netc_port_discard_tpye_t discardType,
                                  netc_port_discard_statistic_t *statistic)
{
    switch (discardType)
    {
        case kNETC_RxDiscard:
            statistic->count   = base->PRXDCR;
            statistic->reason0 = base->PRXDCRR0;
            statistic->reason1 = base->PRXDCRR1;
            break;
        case kNETC_TxDiscard:
            statistic->count   = base->PTXDCR;
            statistic->reason0 = base->PTXDCRR0;
            statistic->reason1 = base->PTXDCRR1;
            break;
        case kNETC_BridgeDiscard:
            statistic->count   = base->BPDCR;
            statistic->reason0 = base->BPDCRR0;
            statistic->reason1 = base->BPDCRR1;
            break;
        default:
            break;
    }
}
void NETC_PortGetPhyMacTxStatistic(NETC_ETH_LINK_Type *base,
                                   netc_port_phy_mac_tpye_t macType,
                                   netc_port_phy_mac_traffic_statistic_t *statistic)
{
    volatile const uint64_t *tempAddr;
    statistic->rxMinPacket = 0;

    if (macType == kNETC_ExpressMAC)
    {
        tempAddr = &base->PM0_TEOCTN;
    }
    else if (macType == kNETC_PreemptableMAC)
    {
        tempAddr = &base->PM1_TEOCTN;
    }
    else
    {
        return;
    }

    statistic->totalOctet             = tempAddr[0];
    statistic->validOctet             = tempAddr[1];
    statistic->pauseFrame             = tempAddr[3];
    statistic->validFrame             = tempAddr[4];
    statistic->vlanFrame              = tempAddr[6];
    statistic->unicastFrame           = tempAddr[8];
    statistic->multicastFrame         = tempAddr[9];
    statistic->boradcastFrame         = tempAddr[10];
    statistic->totalPacket            = tempAddr[12];
    statistic->total64BPacket         = tempAddr[14];
    statistic->total65To127BPacket    = tempAddr[15];
    statistic->total128To255BPacket   = tempAddr[16];
    statistic->total256To511BPacket   = tempAddr[17];
    statistic->total511To1023BPacket  = tempAddr[18];
    statistic->total1024To1522BPacket = tempAddr[19];
    statistic->total1523ToMaxBPacket  = tempAddr[20];
    statistic->controlPacket          = tempAddr[24];
}

void NETC_PortGetPhyMacRxStatistic(NETC_ETH_LINK_Type *base,
                                   netc_port_phy_mac_tpye_t macType,
                                   netc_port_phy_mac_traffic_statistic_t *statistic)
{
    volatile const uint64_t *tempAddr;

    if (macType == kNETC_ExpressMAC)
    {
        tempAddr = &base->PM0_REOCTN;
    }
    else if (macType == kNETC_PreemptableMAC)
    {
        tempAddr = &base->PM1_REOCTN;
    }
    else
    {
        return;
    }

    statistic->totalOctet             = tempAddr[0];
    statistic->validOctet             = tempAddr[1];
    statistic->pauseFrame             = tempAddr[3];
    statistic->validFrame             = tempAddr[4];
    statistic->vlanFrame              = tempAddr[6];
    statistic->unicastFrame           = tempAddr[8];
    statistic->multicastFrame         = tempAddr[9];
    statistic->boradcastFrame         = tempAddr[10];
    statistic->totalPacket            = tempAddr[12];
    statistic->total64BPacket         = tempAddr[14];
    statistic->total65To127BPacket    = tempAddr[15];
    statistic->total128To255BPacket   = tempAddr[16];
    statistic->total256To511BPacket   = tempAddr[17];
    statistic->total511To1023BPacket  = tempAddr[18];
    statistic->total1024To1522BPacket = tempAddr[19];
    statistic->total1523ToMaxBPacket  = tempAddr[20];
    statistic->controlPacket          = tempAddr[24];
    statistic->rxMinPacket            = tempAddr[26];
}

void NETC_PortGetPhyMacDiscardStatistic(NETC_ETH_LINK_Type *base,
                                        netc_port_phy_mac_tpye_t macType,
                                        netc_port_phy_mac_discard_statistic_t *statistic)
{
    volatile const uint64_t *tempAddr;

    if (macType == kNETC_ExpressMAC)
    {
        tempAddr = &base->PM0_RXPFN;
    }
    else if (macType == kNETC_PreemptableMAC)
    {
        tempAddr = &base->PM1_RXPFN;
    }
    else
    {
        return;
    }

    statistic->rxError              = tempAddr[4];
    statistic->rxUndersized         = tempAddr[10];
    statistic->rxOversized          = tempAddr[18];
    statistic->rxErrorFCS           = tempAddr[2];
    statistic->rxFragment           = tempAddr[20];
    statistic->rxJabber             = tempAddr[19];
    statistic->rxDiscard            = tempAddr[8];
    statistic->rxDiscardNoTruncated = tempAddr[22];
    statistic->txErrorFCS           = tempAddr[34];
    statistic->txUndersized         = tempAddr[42];
}
void NETC_PortGetPhyMacPreemptionStatistic(NETC_ETH_LINK_Type *base,
                                           netc_port_phy_mac_preemption_statistic_t *statistic)
{
    statistic->rxReassembledFrame = base->MAC_MERGE_MMFAOCR;
    statistic->rxReassembledError = base->MAC_MERGE_MMFAECR;
    statistic->rxMPacket          = base->MAC_MERGE_MMFCRXR;
    statistic->rxSMDError         = base->MAC_MERGE_MMFSECR;
    statistic->txPreemptionReq    = base->MAC_MERGE_MMHCR;
    statistic->txMPacket          = base->MAC_MERGE_MMFCTXR;
}

void NETC_PortGetPseudoMacTrafficStatistic(NETC_PSEUDO_LINK_Type *base,
                                           bool getTx,
                                           netc_port_pseudo_mac_traffic_statistic_t *statistic)
{
    if (getTx)
    {
        statistic->totalOctet     = ((uint64_t)base->PPMTOCR[1] << 32U) | base->PPMTOCR[0];
        statistic->unicastFrame   = ((uint64_t)base->PPMTUFCR[1] << 32U) | base->PPMTUFCR[0];
        statistic->multicastFrame = ((uint64_t)base->PPMTMFCR[1] << 32U) | base->PPMTMFCR[0];
        statistic->boradcastFrame = ((uint64_t)base->PPMTBFCR[1] << 32U) | base->PPMTBFCR[0];
    }
    else
    {
        statistic->totalOctet     = ((uint64_t)base->PPMROCR[1] << 32U) | base->PPMROCR[0];
        statistic->unicastFrame   = ((uint64_t)base->PPMRUFCR[1] << 32U) | base->PPMRUFCR[0];
        statistic->multicastFrame = ((uint64_t)base->PPMRMFCR[1] << 32U) | base->PPMRMFCR[0];
        statistic->boradcastFrame = ((uint64_t)base->PPMRBFCR[1] << 32U) | base->PPMRBFCR[0];
    }
}

status_t ENETC_PortConfigCBS(NETC_PORT_Type *base, netc_hw_tc_idx_t tcIdx, const port_cbs_config_t *config)
{
    uint8_t usedBw = 0U;
    uint8_t index  = (uint8_t)kNETC_TxTC7;
    for (; index > (uint8_t)tcIdx; index--)
    {
        if (0U != (base->TCT_NUM[index].PTCCBSR0 & NETC_PORT_PTCCBSR0_CBSE_MASK))
        {
            usedBw += base->TCT_NUM[index].PTCCBSR0 & NETC_PORT_PTCCBSR0_BW_MASK;
        }
        else
        {
            break;
        }
    }
    if ((index == tcIdx) && ((usedBw + config->bwWeight) <= 100U))
    {
        base->TCT_NUM[tcIdx].PTCCBSR0 = NETC_PORT_PTCCBSR0_BW(config->bwWeight) | NETC_PORT_PTCCBSR0_CBSE_MASK;
        base->TCT_NUM[tcIdx].PTCCBSR1 = NETC_PORT_PTCCBSR1_HI_CREDIT(config->hiCredit);
        return kStatus_Success;
    }
    else
    {
        /* Traffic classes that support CBS need to be had numerically higher priority, and the sum of all traffic class
         * credit-based shaper's bandwidth cannot execeed 100 */
        return kStatus_Fail;
    }
}
