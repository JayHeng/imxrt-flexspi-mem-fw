/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_netc_switch.h"

/*! @name NETC register map. */
/*@{*/
#define NETC_SWT_PORT_GROUP_BASE_OFFSET (0x4000U) /*!< The SWITCH port group register base address offset. */

#define NETC_SWT_PCIE_FUNC_OFFSET (2)
/*! @brief Pointers to netc bases for each instance. */
static ENETC_PCI_TYPE0_Type *const s_netcPciBases[] = ENETC_PCI_TYPE0_BASE_PTRS;

/*! @brief Pointers to netc bases for each instance. */
static NETC_SW_Type *const s_netcSWBases[] = NETC_SW_BASE_PTRS;

/*! @brief Common message data buffer for command ring 0 and ring 1, data buffer address need align with 16 bytes */
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tb_data_buffer_t dataBuffer0, 16);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tb_data_buffer_t dataBuffer1, 16);
static netc_tb_data_buffer_t *s_cmdDataBuffer[2] = {&dataBuffer0, &dataBuffer1};

static void SWT_GetBaseResource(swt_handle_t *handle, netc_hw_switch_idx_t sw)
{
    handle->hw.func   = s_netcPciBases[NETC_SWT_PCIE_FUNC_OFFSET + sw];
    handle->hw.base   = s_netcSWBases[sw];
    handle->hw.common = (NETC_SW_ENETC_Type *)((uint32_t)handle->hw.base);
    handle->hw.global = (ENETC_GLOBAL_Type *)((uint32_t)handle->hw.base + 0x80000U);
    for (uint32_t i = 0U; i < FSL_NETC_SWITCH_MAX_PORT_NUMBER; i++)
    {
        handle->hw.ports[i].port =
            (NETC_PORT_Type *)((uint32_t)handle->hw.base + (i + 1U) * NETC_SWT_PORT_GROUP_BASE_OFFSET);
        if (NETC_PortIsPseudo(handle->hw.ports[i].port))
        {
            handle->hw.ports[i].pseudo =
                (NETC_PSEUDO_LINK_Type *)((uint32_t)handle->hw.base + (i + 1U) * NETC_SWT_PORT_GROUP_BASE_OFFSET);
        }
        else
        {
            handle->hw.ports[i].eth =
                (NETC_ETH_LINK_Type *)((uint32_t)handle->hw.base + (i + 1U) * NETC_SWT_PORT_GROUP_BASE_OFFSET);
        }
    }
}

status_t SWT_GetDefaultConfig(swt_config_t *config)
{
    assert(config != NULL);

    (void)memset(config, 0, sizeof(swt_config_t));
    for (uint32_t i = 0U; i < 5U; i++)
    {
        config->ports[i].bridgeCfg.isRxVlanAware        = true;
        config->ports[i].bridgeCfg.acceptUntag          = true;
        config->ports[i].bridgeCfg.acceptPriorityTag    = true;
        config->ports[i].bridgeCfg.acceptSingleTag      = true;
        config->ports[i].bridgeCfg.acceptDoubleTag      = true;
        config->ports[i].bridgeCfg.enSrcPortPrun        = true;
        config->ports[i].bridgeCfg.stgMode[0]           = kNETC_ForwardFrame;
        config->ports[i].isiCfg.defaultISEID            = 0xFFFFU;
        config->ports[i].commonCfg.acceptTpid.innerMask = kNETC_OuterStanCvlan | kNETC_OuterStanSvlan;
        config->ports[i].commonCfg.acceptTpid.outerMask = kNETC_InnerStanCvlan | kNETC_InnerStanSvlan;
        config->ports[i].commonCfg.pSpeed               = 0x63U;
        config->ports[i].commonCfg.rxTsSelect           = kNETC_SyncTime;
        config->ports[i].commonCfg.stompFcs             = true;
        config->ports[i].commonCfg.sduCfg.rxPpduBco     = 20U;
        config->ports[i].commonCfg.sduCfg.txPpduBco     = 20U;
        config->ports[i].commonCfg.timeGate.holdSkew    = 64;
        config->ports[i].ethMac.preemptMode             = kNETC_PreemptDisable;
        config->ports[i].ethMac.enMergeVerify           = false;
        config->ports[i].ethMac.mergeVerifyTime         = 10U;
        for (uint32_t j = 0U; j < 8U; j++)
        {
            config->ports[i].txTcCfg[j].sduCfg.maxSduSized = 0x600U;
            config->ports[i].txTcCfg[j].sduCfg.sduType     = kNETC_MPDU;
            config->ports[i].ipvToTC[j]                    = j;
        }
    }
    for (uint32_t i = 0U; i < 2U; i++)
    {
        for (uint32_t j = 0U; j < 8U; j++)
        {
            config->rxqosCfg.profiles[i].ipv[j * 2U]      = j;
            config->rxqosCfg.profiles[i].ipv[j * 2U + 1U] = j;
            config->rxqosCfg.profiles[i].dr[j * 2U + 1U]  = 2U;
        }
    }
    config->bridgeCfg.dVFCfg.portMembership = 0x1FU;
    config->bridgeCfg.dVFCfg.mfo            = kNETC_FDBLookUpWithFlood;
    config->bridgeCfg.dVFCfg.mlo            = kNETC_HardwareMACLearn;
    config->imrCfg.enMirror                 = false;
    config->imrCfg.efmEntryID               = 0xFFFFU;

    return kStatus_Success;
}

status_t SWT_Init(swt_handle_t *handle, const swt_config_t *config, const swt_buffer_config_t *buffConfig)
{
    assert((handle != NULL) && (config != NULL));
    status_t result = kStatus_Success;
    NETC_PORT_Type *port;

    memset(handle, 0, sizeof(swt_handle_t));
    /* Get the register map resource for SWITCH */
    SWT_GetBaseResource(handle, config->switchIdx);
    /* Reset this function */
    handle->hw.func->PCI_CFC_PCIE_DEV_CTL |= ENETC_PCI_TYPE0_PCI_CFC_PCIE_DEV_CTL_INIT_FLR_MASK;
    while ((handle->hw.func->PCI_CFC_PCIE_DEV_CTL & ENETC_PCI_TYPE0_PCI_CFC_PCIE_DEV_CTL_INIT_FLR_MASK) != 0U)
    {
    }
    /* Enable master bus and memory access for PCIe and MSI-X */
    handle->hw.func->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);
    /* For Switch NTMP operation */
    NETC_F4_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);
    /* Store configuration in handle */
    handle->cfg = config;
    /* Configure switch ingress mirror destination */
    if (kStatus_Success != SWT_RxMirrorConfig(handle, &config->imrCfg))
    {
        return kStatus_Fail;
    }
    /* Configure switch FDB hash table maximum number of  dynamically entries */
    handle->hw.base->FDBHTMCR = NETC_SW_FDBHTMCR_DYN_LIMIT(config->dynFDBLimit);
    /* Configure switch bridge */
    (void)SWT_BridgeInit(handle, &config->bridgeCfg);
    /* Configure switch command BD rings */
    for (uint32_t i = 0U; i < config->cmdRingUse; i++)
    {
        result = SWT_CmdBDRInit(handle, (netc_hw_swt_cbdr_idx_t)i, &config->cmdBdrCfg[i]);
        if (kStatus_Success != result)
        {
            break;
        }
        handle->cmdBdRing[i].bdBase        = config->cmdBdrCfg[i].bdBase;
        handle->cmdBdRing[i].bdLength      = config->cmdBdrCfg[i].bdLength;
        handle->cmdBdRing[i].producerIndex = 0U;
        handle->cmdBdRing[i].cleanIndex    = 0U;
        handle->cmdBdRing[i].bdrEnable     = true;
    }
    /* Configure switch Ingress Port Filtering */
    (void)SWT_RxIPFInit(handle, &config->ipfCfg);
    /* Configure switch Ingress VLAN classification */
    (void)SWT_RxVlanCInit(handle, &config->vlanCfg);
    /* Configure switch Ingress QOS classification */
    (void)SWT_RxQosCInit(handle, &config->rxqosCfg);
    /* Configure switch egress QOS to VLAN map classification */
    (void)SWT_TxEPPQosToVlanConfig(handle, &config->txqosCfg);
    /* Configure switch PSFP */
    (void)SWT_RxPSFPInit(handle, &config->psfpCfg);
    /* Configure all ports and macs */
    for (uint32_t i = 0U; (i < 5U) && (kStatus_Success == result); i++)
    {
        port = handle->hw.ports[i].port;
        (void)SWT_BridgeConfigPort(handle, (netc_hw_port_idx_t)i, &config->ports[i].bridgeCfg);
        (void)NETC_PortSetIPF(port, &config->ports[i].ipfCfg);
        (void)NETC_PortSetISI(port, &config->ports[i].isiCfg);
        (void)NETC_PortConfig(port, &config->ports[i].commonCfg);
        port->PMCR      = NETC_PORT_PMCR_IMIRE(config->ports[i].enMirror);
        port->PFMCR     = NETC_PORT_PFMCR_FMMA(config->ports[i].ignoreFMMiscfg);
        port->PLANIDCR  = NETC_PORT_PLANIDCR_LANID(config->ports[i].lanID);
        port->PIPV2QMR0 = NETC_PORT_PIPV2QMR0_IPV7_Q(config->ports[i].ipvToTC[7]) |
                          NETC_PORT_PIPV2QMR0_IPV6_Q(config->ports[i].ipvToTC[6]) |
                          NETC_PORT_PIPV2QMR0_IPV5_Q(config->ports[i].ipvToTC[5]) |
                          NETC_PORT_PIPV2QMR0_IPV4_Q(config->ports[i].ipvToTC[4]) |
                          NETC_PORT_PIPV2QMR0_IPV3_Q(config->ports[i].ipvToTC[3]) |
                          NETC_PORT_PIPV2QMR0_IPV2_Q(config->ports[i].ipvToTC[2]) |
                          NETC_PORT_PIPV2QMR0_IPV1_Q(config->ports[i].ipvToTC[1]) |
                          NETC_PORT_PIPV2QMR0_IPV0_Q(config->ports[i].ipvToTC[0]);
        /* Configure Ethernet MAC */
        if (!NETC_PortIsPseudo(port))
        {
            (void)NETC_PortConfigEthMac(handle->hw.ports[i].eth, &config->ports[i].ethMac);
            NETC_PortSetMacAddr(port, &config->ports[i].macAddr[0]);
            port->PCTFCR = NETC_PORT_PCTFCR_BSQS(config->ports[i].outBufferSize) |
                           NETC_PORT_PCTFCR_ECTS(!config->ports[i].outCutThrough) |
                           NETC_PORT_PCTFCR_ICTS(!config->ports[i].inCutThrough);
        }
        /* Configure traffic class */
        for (uint32_t j = 8U; j > 0U; j--)
        {
            result = SWT_TxTrafficClassConfig(handle, (netc_hw_port_idx_t)i, (netc_hw_tc_idx_t)(j - 1U),
                                              &config->ports[i].txTcCfg[(j - 1U)]);
            if (result != kStatus_Success)
            {
                return result;
            }
        }
    }
    return result;
}

status_t SWT_RxMirrorConfig(swt_handle_t *handle, const netc_swt_imr_config_t *config)
{
    assert((handle != NULL) && (config != NULL));

    handle->hw.base->IMDCR0 = NETC_SW_IMDCR0_PORT((kNETC_SWTMPort != config->destPort) ? config->destPort : 0U) |
                              NETC_SW_IMDCR0_DR(config->dr) | NETC_SW_IMDCR0_IPV(config->ipv) |
                              NETC_SW_IMDCR0_MIRDEST((kNETC_SWTMPort == config->destPort) ? 1U : 0U) |
                              NETC_SW_IMDCR0_MIREN(config->enMirror);
    if ((kNETC_SWTMPort == config->destPort) && (0xFFFFU != config->efmEntryID))
    {
        return kStatus_Fail;
    }
    else
    {
        handle->hw.base->IMDCR1 =
            NETC_SW_IMDCR1_EFM_LEN_CHANGE(config->efmLengthChange) | NETC_SW_IMDCR1_EFMEID(config->efmEntryID);
        return kStatus_Success;
    }
}

status_t SWT_SetEpHandle(swt_handle_t *handle, ep_handle_t *epHandle)
{
    handle->epHandle = epHandle;

    return kStatus_Success;
}

status_t SWT_SendFrame(
    swt_handle_t *handle, netc_hw_port_idx_t portIdx, netc_frame_struct_t *frame, void *context, swt_tx_opt *opt)
{
    ep_tx_opt txOpt = {0};

    if (opt != NULL)
    {
        txOpt.flags = opt->flags;
        txOpt.vlan  = opt->vlan;
    }
    txOpt.swtPort = portIdx;
    return EP_SendFrame(handle->epHandle, 0, frame, context, &txOpt);
}

void SWT_ReclaimTxDescriptor(swt_handle_t *handle)
{
    EP_ReclaimTxDescriptor(handle->epHandle, 0);
}

status_t SWT_GetRxFrameSize(swt_handle_t *handle, uint8_t ring, uint32_t *length)
{
    return EP_GetRxFrameSize(handle->epHandle, ring, length);
}

status_t SWT_ReceiveFrameCopy(
    swt_handle_t *handle, uint8_t ring, void *buffer, uint32_t length, netc_frame_attr_t *attr)
{
    return EP_ReceiveFrameCopy(handle->epHandle, ring, buffer, length, attr);
}

static status_t SWT_GetIdleCmdBDRing(swt_handle_t *handle, netc_cbdr_handle_t *cdbrHandle)
{
    for (uint32_t i = 0U; i < handle->cfg->cmdRingUse; i++)
    {
        if (0U == (handle->hw.base->NUM_CBDR[i].CBDRSR & NETC_SW_CBDRSR_BUSY_MASK))
        {
            cdbrHandle->base   = (netc_cbdr_hw_t *)&handle->hw.base->NUM_CBDR[i].CBDRMR;
            cdbrHandle->cmdr   = &handle->cmdBdRing[i];
            cdbrHandle->buffer = s_cmdDataBuffer[i];
            return kStatus_Success;
        }
    }
    return kStatus_Fail;
}

status_t SWT_CmdBDRInit(swt_handle_t *handle, netc_hw_swt_cbdr_idx_t ring, const netc_cmd_bdr_config_t *config)
{
    assert(handle != NULL);
    return NETC_CmdBDRInit((netc_cbdr_hw_t *)&handle->hw.base->NUM_CBDR[ring].CBDRMR, config);
}

status_t SWT_CmdBDRDeinit(swt_handle_t *handle, netc_hw_swt_cbdr_idx_t ring)
{
    assert(handle != NULL);
    return NETC_CmdBDRDeinit((netc_cbdr_hw_t *)&handle->hw.base->NUM_CBDR[ring].CBDRMR);
}

status_t SWT_RxIPFAddTableEntry(swt_handle_t *handle, netc_tb_ipf_config_t *config, uint32_t *entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_AddIPFTableEntry(&cdbrHandle, config, entryID);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxIPFDelTableEntry(swt_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_DelIPFTableEntry(&cdbrHandle, entryID);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxIPFResetMatchCounter(swt_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_ResetIPFMatchCounter(&cdbrHandle, entryID);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxIPFGetMatchedCount(swt_handle_t *handle, uint32_t entryID, uint64_t *count)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_GetIPFMatchedCount(&cdbrHandle, entryID, count);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_BridgeInit(swt_handle_t *handle, const netc_swt_bridge_config_t *config)
{
    assert(handle != NULL);
    assert(config != NULL);

    /* Configure switch default VLAN filter entry */
    handle->hw.base->VFHTDECR0 =
        NETC_SW_VFHTDECR0_IPMFLE(config->dVFCfg.enIPMFlood) | NETC_SW_VFHTDECR0_IPMFE(config->dVFCfg.enIPMFilter) |
        NETC_SW_VFHTDECR0_STG_ID(config->dVFCfg.stgID) |
        ((config->dVFCfg.portMembership << NETC_SW_VFHTDECR0_PORT0_SHIFT) &
         (NETC_SW_VFHTDECR0_PORT4_MASK | NETC_SW_VFHTDECR0_PORT3_MASK | NETC_SW_VFHTDECR0_PORT2_MASK |
          NETC_SW_VFHTDECR0_PORT1_MASK | NETC_SW_VFHTDECR0_PORT0_MASK));
    handle->hw.base->VFHTDECR1 = NETC_SW_VFHTDECR1_BASE_ETEID(config->dVFCfg.baseETEID) |
                                 NETC_SW_VFHTDECR1_VL_MODE(config->dVFCfg.enUseFilterID) |
                                 NETC_SW_VFHTDECR1_FID(config->dVFCfg.filterID);
    handle->hw.base->VFHTDECR2 =
        NETC_SW_VFHTDECR2_MFO(config->dVFCfg.mfo) | NETC_SW_VFHTDECR2_MLO(config->dVFCfg.mlo) |
        ((config->dVFCfg.etaPortBitmap << NETC_SW_VFHTDECR2_ET_PORT0_SHIFT) &
         (NETC_SW_VFHTDECR2_ET_PORT4_MASK | NETC_SW_VFHTDECR2_ET_PORT3_MASK | NETC_SW_VFHTDECR2_ET_PORT2_MASK |
          NETC_SW_VFHTDECR2_ET_PORT1_MASK | NETC_SW_VFHTDECR2_ET_PORT0_MASK));
    return kStatus_Success;
}

status_t SWT_BridgeConfigPort(swt_handle_t *handle, netc_hw_port_idx_t portIdx, const netc_swt_port_config_t *config)
{
    assert((handle != NULL) && (config != NULL));

    NETC_PORT_Type *base  = handle->hw.ports[portIdx].port;
    uint32_t stgMode      = 0U;
    uint8_t tagAccepyMode = (uint8_t)config->acceptUntag | ((uint8_t)config->acceptPriorityTag << 1U) |
                            ((uint8_t)config->acceptSingleTag << 2U) | ((uint8_t)config->acceptDoubleTag << 3U);

    for (uint32_t i = 0U; i < 16U; i++)
    {
        stgMode |= (config->stgMode[i] & NETC_PORT_BPSTGSR_STG_STATE0_MASK) << (i * 2);
    }
    base->BPSTGSR = stgMode;
    base->BPDVR   = NETC_PORT_BPDVR_TXTAGA(config->txVlanAction) | NETC_PORT_BPDVR_RXVAM(!config->isRxVlanAware) |
                  NETC_PORT_BPDVR_RXTAGA(tagAccepyMode) | NETC_PORT_BPDVR_TPID(config->defaultVlan.tpid) |
                  NETC_PORT_BPDVR_PCP(config->defaultVlan.pcp) | NETC_PORT_BPDVR_DEI(config->defaultVlan.dei) |
                  NETC_PORT_BPDVR_VID(config->defaultVlan.vid);
    base->BPCR = NETC_PORT_BPCR_SRCPRND(!config->enSrcPortPrun) | NETC_PORT_BPCR_STAMVD(config->enMacStationMove) |
                 NETC_PORT_BPCR_BCASTE(config->enBcastStormCtrl) | NETC_PORT_BPCR_MCASTE(config->enMcastStormCtrl) |
                 NETC_PORT_BPCR_UMCASTE(config->enUnMcastStormCtrl) |
                 NETC_PORT_BPCR_UUCASTE(config->enUnUcastStormCtrl) | NETC_PORT_BPCR_DYN_LIMIT(config->maxDynaFDBEntry);
    base->BPSCR0 =
        NETC_PORT_BPSCR0_BCASTRPEID(config->bcastRpEntryID) | NETC_PORT_BPSCR0_UUCASTRPEID(config->unUcastRpEntryID);
    base->BPSCR1 =
        NETC_PORT_BPSCR1_UMCASTRPEID(config->unMcastRpEntryID) | NETC_PORT_BPSCR1_MCASTRPEID(config->mcastEntryID);
    return kStatus_Success;
}

status_t SWT_BridgeAddVFTableEntry(swt_handle_t *handle, netc_tb_vf_config_t *config, uint32_t *entryID)
{
    assert((handle != NULL) && (config != NULL));
    netc_cmd_bd_t cmdBd = {0};
    status_t status     = kStatus_Success;
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_vf_req_data_t));
        /* Add entry to VLAN Filter Table and query the ENTRY_ID  */
        cdbrHandle.buffer->vf.request.cfge                       = config->cfge;
        cdbrHandle.buffer->vf.request.keye                       = config->keye;
        cdbrHandle.buffer->vf.request.commonHeader.updateActions = 1U;
        cdbrHandle.buffer->vf.request.commonHeader.queryActions  = 1U;
        cmdBd.req.addr                                           = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                      = sizeof(netc_tb_vf_req_data_t);
        cmdBd.req.resLength                                      = sizeof(netc_tb_vf_rsp_data_t);
        cmdBd.req.tableId                                        = kNETC_VFTable;
        cmdBd.req.cmd                                            = kNETC_AddAndQueryEntry;
        /* Add operation only support Exact Key Element Match */
        cmdBd.req.accessType = kNETC_ExactKeyMatch;
        status               = NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
        if (kStatus_Success == status)
        {
            *entryID = cdbrHandle.buffer->vf.response.entryID;
        }
        return status;
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_BridgeDelVFTableEntry(swt_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cmd_bd_t cmdBd = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_vf_req_data_t));
        cdbrHandle.buffer->vf.request.entryID                    = entryID;
        cdbrHandle.buffer->vf.request.commonHeader.updateActions = 0;
        cdbrHandle.buffer->vf.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                           = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                      = 8U;
        cmdBd.req.resLength                                      = 4U;
        cmdBd.req.tableId                                        = kNETC_VFTable;
        cmdBd.req.cmd                                            = kNETC_DeleteEntry;
        cmdBd.req.accessType                                     = kNETC_EntryIDMatch;
        return NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_BridgeAddFDBTableEntry(swt_handle_t *handle, netc_tb_fdb_config_t *config, uint32_t *entryID)
{
    assert((handle != NULL) && (config != NULL));
    netc_cmd_bd_t cmdBd = {0};
    status_t status     = kStatus_Success;
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_fdb_req_data_t));
        /* Add entry to FDB Table and query the ENTRY_ID  */
        cdbrHandle.buffer->fdb.request.cfge                       = config->cfge;
        cdbrHandle.buffer->fdb.request.keye                       = config->keye;
        cdbrHandle.buffer->fdb.request.commonHeader.updateActions = kNETC_FDBCfgEUpdate;
        cdbrHandle.buffer->fdb.request.commonHeader.queryActions  = 1U;
        cmdBd.req.addr                                            = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                       = sizeof(netc_tb_fdb_req_data_t);
        cmdBd.req.resLength                                       = sizeof(netc_tb_fdb_rsp_data_t);
        cmdBd.req.tableId                                         = kNETC_FDBTable;
        cmdBd.req.cmd                                             = kNETC_AddAndQueryEntry;
        /* Add operation only support Exact Key Element Match */
        cmdBd.req.accessType = kNETC_ExactKeyMatch;
        status               = NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
        if (kStatus_Success == status)
        {
            *entryID = cdbrHandle.buffer->fdb.response.entryID;
        }
        return status;
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_BridgeDelFDBTableEntry(swt_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cmd_bd_t cmdBd = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_fdb_req_data_t));
        cdbrHandle.buffer->fdb.request.entryID                    = entryID;
        cdbrHandle.buffer->fdb.request.commonHeader.updateActions = 0;
        cdbrHandle.buffer->fdb.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                            = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                       = 36U;
        cmdBd.req.resLength                                       = 4U;
        cmdBd.req.tableId                                         = kNETC_FDBTable;
        cmdBd.req.cmd                                             = kNETC_DeleteEntry;
        cmdBd.req.accessType                                      = kNETC_EntryIDMatch;
        return NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_BridgeGetFDBActivityState(swt_handle_t *handle, uint32_t entryID, netc_tb_fdb_acte_t *state)
{
    assert(handle != NULL);
    netc_cmd_bd_t cmdBd = {0};
    status_t status     = kStatus_Success;
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_fdb_req_data_t));
        cdbrHandle.buffer->fdb.request.entryID                    = entryID;
        cdbrHandle.buffer->fdb.request.commonHeader.updateActions = 0;
        cdbrHandle.buffer->fdb.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                            = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                       = 36U;
        cmdBd.req.resLength                                       = sizeof(netc_tb_fdb_rsp_data_t);
        cmdBd.req.tableId                                         = kNETC_FDBTable;
        cmdBd.req.cmd                                             = kNETC_QueryEntry;
        cmdBd.req.accessType                                      = kNETC_EntryIDMatch;
        status = NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
        if (kStatus_Success == status)
        {
            *state = cdbrHandle.buffer->fdb.response.acte;
        }
        return status;
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_BridgeAddL2MCFTableEntry(swt_handle_t *handle, netc_tb_l2mcf_config_t *config, uint32_t *entryID)
{
    assert((handle != NULL) && (config != NULL));
    netc_cmd_bd_t cmdBd = {0};
    status_t status     = kStatus_Success;
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_l2mcf_req_data_t));
        /* Add entry to L2MCF Table and query the ENTRY_ID  */
        cdbrHandle.buffer->l2mcf.request.cfge                       = config->cfge;
        cdbrHandle.buffer->l2mcf.request.keye                       = config->keye;
        cdbrHandle.buffer->l2mcf.request.commonHeader.updateActions = kNETC_FDBCfgEUpdate;
        cdbrHandle.buffer->l2mcf.request.commonHeader.queryActions  = 1U;
        cmdBd.req.addr                                              = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                         = sizeof(netc_tb_l2mcf_req_data_t);
        cmdBd.req.resLength                                         = sizeof(netc_tb_l2mcf_rsp_data_t);
        cmdBd.req.tableId                                           = kNETC_L2MCFTable;
        cmdBd.req.cmd                                               = kNETC_AddAndQueryEntry;
        /* Add operation only support Exact Key Element Match */
        cmdBd.req.accessType = kNETC_ExactKeyMatch;
        status               = NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
        if (kStatus_Success == status)
        {
            *entryID = cdbrHandle.buffer->l2mcf.response.entryID;
        }
        return status;
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_BridgeDelL2MCFTableEntry(swt_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cmd_bd_t cmdBd = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_l2mcf_req_data_t));
        cdbrHandle.buffer->l2mcf.request.entryID                    = entryID;
        cdbrHandle.buffer->l2mcf.request.commonHeader.updateActions = 0;
        cdbrHandle.buffer->l2mcf.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                              = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                         = 36U;
        /* Add command response data buffer only contains 4 bytes status filed */
        cmdBd.req.resLength  = 4U;
        cmdBd.req.tableId    = kNETC_L2MCFTable;
        cmdBd.req.cmd        = kNETC_DeleteEntry;
        cmdBd.req.accessType = kNETC_EntryIDMatch;
        return NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_BridgeGetL2MCFActivityState(swt_handle_t *handle, uint32_t entryID, netc_tb_l2mcf_acte_t *state)
{
    assert(handle != NULL);
    netc_cmd_bd_t cmdBd = {0};
    status_t status     = kStatus_Success;
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_l2mcf_req_data_t));
        cdbrHandle.buffer->l2mcf.request.entryID                    = entryID;
        cdbrHandle.buffer->l2mcf.request.commonHeader.updateActions = 0;
        cdbrHandle.buffer->l2mcf.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                              = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                         = 36U;
        cmdBd.req.resLength                                         = sizeof(netc_tb_l2mcf_rsp_data_t);
        cmdBd.req.tableId                                           = kNETC_L2MCFTable;
        cmdBd.req.cmd                                               = kNETC_QueryEntry;
        cmdBd.req.accessType                                        = kNETC_EntryIDMatch;
        status = NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
        if (kStatus_Success == status)
        {
            *state = cdbrHandle.buffer->l2mcf.response.acte;
        }
        return status;
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPAddISITableEntry(swt_handle_t *handle, netc_tb_isi_config_t *config, uint32_t *entryID)
{
    assert((handle != NULL) && (config != NULL) && (entryID != NULL));
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_AddISITableEntry(&cdbrHandle, config, entryID);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPDelISITableEntry(swt_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_DelISITableEntry(&cdbrHandle, entryID);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPAddISTableEntry(swt_handle_t *handle, netc_tb_is_config_t *config)
{
    assert((handle != NULL) && (config != NULL));
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_AddISTableEntry(&cdbrHandle, config);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPDelISTableEntry(swt_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_DelISTableEntry(&cdbrHandle, entryID);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPAddISFTableEntry(swt_handle_t *handle, netc_tb_isf_config_t *config, uint32_t *entryID)
{
    assert((handle != NULL) && (config != NULL) && (entryID != NULL));
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_AddISFTableEntry(&cdbrHandle, config, entryID);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPDelISFTableEntry(swt_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_DelISFTableEntry(&cdbrHandle, entryID);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPAddISCTableEntry(swt_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_AddISCTableEntry(&cdbrHandle, entryID);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPGetISCStatistic(swt_handle_t *handle, uint32_t entryID, netc_tb_isc_stse_t *statistic)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_GetISCStatistic(&cdbrHandle, entryID, statistic);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPResetISCStatistic(swt_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_ResetISCStatistic(&cdbrHandle, entryID);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPAddSGITableEntry(swt_handle_t *handle, netc_tb_sgi_config_t *config)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_AddSGITableEntry(&cdbrHandle, config);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPDelSGITableEntry(swt_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_DelSGITableEntry(&cdbrHandle, entryID);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPGetSGIState(swt_handle_t *handle, uint32_t entryID, netc_tb_sgi_sgise_t *state)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_GetSGIState(&cdbrHandle, entryID, state);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPAddSGCLTableEntry(swt_handle_t *handle, netc_tb_sgcl_config_t *config, uint8_t *buffer)
{
    assert((handle != NULL) && (buffer != NULL) && (config != NULL));
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        cdbrHandle.buffer = (netc_tb_data_buffer_t *)buffer;
        return NETC_AddSGCLTableEntry(&cdbrHandle, config);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPDelSGCLTableEntry(swt_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_DelSGCLTableEntry(&cdbrHandle, entryID);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPGetSGCLState(swt_handle_t *handle, uint32_t entryID, netc_tb_sgcl_sgclse_t *state, uint8_t *buffer)
{
    assert((handle != NULL) && (buffer != NULL) && (state != NULL));
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        cdbrHandle.buffer = (netc_tb_data_buffer_t *)buffer;
        return NETC_GetSGCLState(&cdbrHandle, entryID, state);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPAddRPTableEntry(swt_handle_t *handle, netc_tb_rp_config_t *config)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_AddRPTableEntry(&cdbrHandle, config);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPDelRPTableEntry(swt_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_DelRPTableEntry(&cdbrHandle, entryID);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_RxPSFPGetRPStatistic(swt_handle_t *handle, uint32_t entryID, netc_tb_rp_stse_t *statis)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_GetRPStatistic(&cdbrHandle, entryID, statis);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_TxTrafficClassConfig(swt_handle_t *handle,
                                  netc_hw_port_idx_t portIdx,
                                  netc_hw_tc_idx_t tcIdx,
                                  const netc_tx_tc_config_t *config)
{
    assert((handle != NULL) && (config != NULL));
    status_t reseult;
    NETC_PORT_Type *base = handle->hw.ports[portIdx].port;

    if (!NETC_PortIsPseudo(base))
    {
        uint32_t temp = base->PFPCR & (~(1U << (uint8_t)tcIdx));
        base->PFPCR   = temp | ((uint32_t)config->enPreemption << (uint8_t)tcIdx);
    }

    reseult = ENETC_PortConfigTXMaxSDU(base, tcIdx, &config->sduCfg);
    if ((kStatus_Success == reseult) && (config->enableCbs))
    {
        reseult = ENETC_PortConfigCBS(base, tcIdx, &config->cbsCfg);
    }
    return reseult;
}

status_t SWT_TxTGSConfigAdminGcl(swt_handle_t *handle, netc_tb_tgs_gcl_t *config)
{
    assert((handle != NULL) && (config != NULL));
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_ConfigTGSAdminList(&cdbrHandle, config);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_TxtTGSGetOperGcl(swt_handle_t *handle, netc_tb_tgs_gcl_t *gcl, uint32_t length)
{
    assert((handle != NULL) && (gcl != NULL));
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        return NETC_GetTGSOperationList(&cdbrHandle, gcl, length);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_FMAddTableEntry(swt_handle_t *handle, netc_tb_fm_config_t *config)
{
    netc_cmd_bd_t cmdBd = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_fm_req_data_t));
        /* Add entry to FM Table */
        cdbrHandle.buffer->fm.request.cfge                       = config->cfge;
        cdbrHandle.buffer->fm.request.entryID                    = config->entryID;
        cdbrHandle.buffer->fm.request.commonHeader.updateActions = 1U;
        cdbrHandle.buffer->fm.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                           = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                      = sizeof(netc_tb_fm_req_data_t);
        cmdBd.req.resLength                                      = sizeof(netc_tb_fm_rsp_data_t);
        cmdBd.req.tableId                                        = kNETC_FMTable;
        cmdBd.req.cmd                                            = kNETC_AddEntry;
        /* Add operation only support Entry ID Match */
        cmdBd.req.accessType = kNETC_EntryIDMatch;
        return NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_FMDelTableEntry(swt_handle_t *handle, uint32_t entryID)
{
    netc_cmd_bd_t cmdBd = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_fm_req_data_t));
        cdbrHandle.buffer->fm.request.entryID                    = entryID;
        cdbrHandle.buffer->fm.request.commonHeader.updateActions = 0U;
        cdbrHandle.buffer->fm.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                           = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                      = sizeof(netc_tb_fm_req_data_t);
        cmdBd.req.resLength                                      = sizeof(netc_tb_fm_rsp_data_t);
        cmdBd.req.tableId                                        = kNETC_FMTable;
        cmdBd.req.cmd                                            = kNETC_DeleteEntry;
        cmdBd.req.accessType                                     = kNETC_EntryIDMatch;
        return NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_FMDUpdateTableEntry(swt_handle_t *handle, netc_tb_fmd_update_config_t *config, uint32_t length)
{
    netc_cmd_bd_t cmdBd = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (((uintptr_t)config % 16U) != 0U)
    {
        return kStatus_Fail;
    }

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        cdbrHandle.buffer                                         = (netc_tb_data_buffer_t *)config;
        cdbrHandle.buffer->fmd.request.commonHeader.updateActions = 1U;
        cdbrHandle.buffer->fmd.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                            = (uintptr_t)config;
        cmdBd.req.reqLength                                       = length + 8U;
        cmdBd.req.resLength                                       = length + 8U;
        cmdBd.req.tableId                                         = kNETC_FMDTable;
        cmdBd.req.cmd                                             = kNETC_UpdateEntry;
        /* Only support Entry ID Match */
        cmdBd.req.accessType = kNETC_EntryIDMatch;
        return NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_FMDQueryTableEntry(swt_handle_t *handle, netc_tb_fmd_query_buffer_t *query, uint32_t length)
{
    netc_cmd_bd_t cmdBd = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (((uintptr_t)query % 16U) != 0U)
    {
        return kStatus_Fail;
    }

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        cdbrHandle.buffer                                         = (netc_tb_data_buffer_t *)query;
        cdbrHandle.buffer->fmd.request.commonHeader.updateActions = 0U;
        cdbrHandle.buffer->fmd.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                            = (uintptr_t)query;
        cmdBd.req.reqLength                                       = 8U;
        cmdBd.req.resLength                                       = length + 4U;
        cmdBd.req.tableId                                         = kNETC_FMDTable;
        cmdBd.req.cmd                                             = kNETC_QueryEntry;
        /* Only support Entry ID Match */
        cmdBd.req.accessType = kNETC_EntryIDMatch;
        return NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_TxEPPAddETTableEntry(swt_handle_t *handle, netc_tb_et_config_t *config)
{
    netc_cmd_bd_t cmdBd = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_et_req_data_t));
        /* Add entry to ET Table and query the ENTRY_ID */
        cdbrHandle.buffer->et.request.cfge                       = config->cfge;
        cdbrHandle.buffer->et.request.entryID                    = config->entryID;
        cdbrHandle.buffer->et.request.commonHeader.updateActions = 1U;
        cdbrHandle.buffer->et.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                           = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                      = sizeof(netc_tb_et_req_data_t);
        cmdBd.req.resLength                                      = sizeof(netc_tb_et_rsp_data_t);
        cmdBd.req.tableId                                        = kNETC_ETTable;
        cmdBd.req.cmd                                            = kNETC_AddEntry;
        /* Add operation only support Entry ID Match */
        cmdBd.req.accessType = kNETC_EntryIDMatch;
        return NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_TxEPPDelETTableEntry(swt_handle_t *handle, uint32_t entryID)
{
    netc_cmd_bd_t cmdBd = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_et_req_data_t));
        cdbrHandle.buffer->et.request.entryID                    = entryID;
        cdbrHandle.buffer->et.request.commonHeader.updateActions = 0U;
        cdbrHandle.buffer->et.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                           = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                      = sizeof(netc_tb_et_req_data_t);
        cmdBd.req.resLength                                      = sizeof(netc_tb_et_rsp_data_t);
        cmdBd.req.tableId                                        = kNETC_ETTable;
        cmdBd.req.cmd                                            = kNETC_DeleteEntry;
        cmdBd.req.accessType                                     = kNETC_EntryIDMatch;
        return NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_TxEPPQosToVlanConfig(swt_handle_t *handle, const netc_swt_qos_to_vlan_config_t *config)
{
    for (uint32_t i = 0U; i < 2; i++)
    {
        for (uint32_t j = 0U; j < 4U; j++)
        {
            handle->hw.base->MAP_PCP[i].QOSVLANMPR[j] =
                NETC_SW_QOSVLANMPR_IPV0_DR0(config->profiles[i].qos[j * 8U]) |
                NETC_SW_QOSVLANMPR_IPV0_DR1(config->profiles[i].qos[j * 8U + 1U]) |
                NETC_SW_QOSVLANMPR_IPV0_DR2(config->profiles[i].qos[j * 8U + 2U]) |
                NETC_SW_QOSVLANMPR_IPV0_DR1(config->profiles[i].qos[j * 8U + 3U]) |
                NETC_SW_QOSVLANMPR_IPV1_DR0(config->profiles[i].qos[j * 8U + 4U]) |
                NETC_SW_QOSVLANMPR_IPV1_DR1(config->profiles[i].qos[j * 8U + 5U]) |
                NETC_SW_QOSVLANMPR_IPV1_DR2(config->profiles[i].qos[j * 8U + 6U]) |
                NETC_SW_QOSVLANMPR_IPV1_DR3(config->profiles[i].qos[j * 8U + 7U]);
        }
        handle->hw.base->PCP2PCPMPR[i] =
            NETC_SW_PCP2PCPMPR_PCP0(config->profiles[i].pcp[0]) | NETC_SW_PCP2PCPMPR_PCP1(config->profiles[i].pcp[1]) |
            NETC_SW_PCP2PCPMPR_PCP2(config->profiles[i].pcp[2]) | NETC_SW_PCP2PCPMPR_PCP3(config->profiles[i].pcp[3]) |
            NETC_SW_PCP2PCPMPR_PCP4(config->profiles[i].pcp[4]) | NETC_SW_PCP2PCPMPR_PCP5(config->profiles[i].pcp[5]) |
            NETC_SW_PCP2PCPMPR_PCP6(config->profiles[i].pcp[6]) | NETC_SW_PCP2PCPMPR_PCP7(config->profiles[i].pcp[7]);
    }
    return kStatus_Success;
}

status_t SWT_GetECStatistic(swt_handle_t *handle, uint32_t entryID, netc_tb_ec_stse_t *statistic)
{
    netc_cmd_bd_t cmdBd = {0};
    status_t status     = kStatus_Success;
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_ec_req_data_t));
        cdbrHandle.buffer->ec.request.entryID                    = entryID;
        cdbrHandle.buffer->ec.request.commonHeader.updateActions = 0U;
        cdbrHandle.buffer->ec.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                           = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                      = sizeof(netc_tb_ec_req_data_t);
        cmdBd.req.resLength                                      = sizeof(netc_tb_ec_rsp_data_t);
        cmdBd.req.tableId                                        = kNETC_ECTable;
        cmdBd.req.cmd                                            = kNETC_QueryEntry;
        cmdBd.req.accessType                                     = kNETC_EntryIDMatch;
        status = NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
        if (kStatus_Success == status)
        {
            *statistic = cdbrHandle.buffer->ec.response.stse;
        }
        return status;
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_ResetECStatistic(swt_handle_t *handle, uint32_t entryID)
{
    netc_cmd_bd_t cmdBd = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_ec_req_data_t));
        /* Reset EC Table according to the ENTRY_ID (EC_EID) */
        cdbrHandle.buffer->ec.request.entryID                    = entryID;
        cdbrHandle.buffer->ec.request.commonHeader.updateActions = 1U;
        cdbrHandle.buffer->ec.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                           = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                      = sizeof(netc_tb_ec_req_data_t);
        cmdBd.req.resLength                                      = sizeof(netc_tb_ec_rsp_data_t);
        cmdBd.req.tableId                                        = kNETC_ECTable;
        cmdBd.req.cmd                                            = kNETC_UpdateEntry;
        cmdBd.req.accessType                                     = kNETC_EntryIDMatch;
        return NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_TxETMConfigClassQueue(swt_handle_t *handle, netc_tb_etmcq_config_t *config)
{
    netc_cmd_bd_t cmdBd = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_etmcq_data_t));
        cdbrHandle.buffer->cq.request.entryID                    = config->entryID;
        cdbrHandle.buffer->cq.request.cfge                       = config->cfge;
        cdbrHandle.buffer->cq.request.commonHeader.updateActions = kNETC_CQCfgEUpdate | kNETC_CQStsEUpdate;
        cdbrHandle.buffer->cq.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                           = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                      = sizeof(netc_tb_etmcq_req_data_t);
        cmdBd.req.resLength                                      = sizeof(netc_tb_etmcq_rsp_data_t);
        cmdBd.req.tableId                                        = kNETC_ECQTable;
        cmdBd.req.cmd                                            = kNETC_UpdateEntry;
        cmdBd.req.accessType                                     = kNETC_EntryIDMatch;
        return NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_GetETMClassQueueStatistic(swt_handle_t *handle, uint32_t entryID, netc_tb_etmcq_stse_t *statistic)
{
    netc_cmd_bd_t cmdBd = {0};
    status_t status     = kStatus_Success;
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_etmcq_data_t));
        cdbrHandle.buffer->cq.request.entryID                    = entryID;
        cdbrHandle.buffer->cq.request.commonHeader.updateActions = 0U;
        cdbrHandle.buffer->cq.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                           = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                      = sizeof(netc_tb_etmcq_req_data_t);
        cmdBd.req.resLength                                      = sizeof(netc_tb_etmcq_rsp_data_t);
        cmdBd.req.tableId                                        = kNETC_ECQTable;
        cmdBd.req.cmd                                            = kNETC_QueryEntry;
        cmdBd.req.accessType                                     = kNETC_EntryIDMatch;
        status = NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
        if (kStatus_Success == status)
        {
            *statistic = cdbrHandle.buffer->cq.response.stse;
        }
        return status;
    }
    else
    {
        return kStatus_Fail;
    }
}
status_t SWT_TxETMConfigClassScheduler(swt_handle_t *handle, netc_tb_etmcs_config_t *config)
{
    netc_cmd_bd_t cmdBd = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_etmcs_data_t));
        cdbrHandle.buffer->cs.request.entryID                    = config->entryID;
        cdbrHandle.buffer->cs.request.cfge                       = config->cfge;
        cdbrHandle.buffer->cs.request.commonHeader.updateActions = 1U;
        cdbrHandle.buffer->cs.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                           = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                      = sizeof(netc_tb_etmcs_req_data_t);
        cmdBd.req.resLength                                      = sizeof(netc_tb_etmcs_rsp_data_t);
        cmdBd.req.tableId                                        = kNETC_ECSTable;
        cmdBd.req.cmd                                            = kNETC_UpdateEntry;
        cmdBd.req.accessType                                     = kNETC_EntryIDMatch;
        return NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_TxETMConfigCongestionGroup(swt_handle_t *handle, netc_tb_etmcg_config_t *config)
{
    netc_cmd_bd_t cmdBd = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_etmcg_data_t));
        cdbrHandle.buffer->cg.request.entryID                    = config->entryID;
        cdbrHandle.buffer->cg.request.cfge                       = config->cfge;
        cdbrHandle.buffer->cg.request.commonHeader.updateActions = 1U;
        cdbrHandle.buffer->cg.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                           = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                      = sizeof(netc_tb_etmcg_req_data_t);
        cmdBd.req.resLength                                      = sizeof(netc_tb_etmcg_rsp_data_t);
        cmdBd.req.tableId                                        = kNETC_ECGTable;
        cmdBd.req.cmd                                            = kNETC_UpdateEntry;
        cmdBd.req.accessType                                     = kNETC_EntryIDMatch;
        return NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_GetETMCongestionGroupStatistic(swt_handle_t *handle, uint32_t entryID, netc_tb_etmcg_stse_t *statistic)
{
    netc_cmd_bd_t cmdBd = {0};
    status_t status     = kStatus_Success;
    netc_cbdr_handle_t cdbrHandle;

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        memset(cdbrHandle.buffer, 0, sizeof(netc_tb_etmcg_data_t));
        cdbrHandle.buffer->cg.request.entryID                    = entryID;
        cdbrHandle.buffer->cg.request.commonHeader.updateActions = 0U;
        cdbrHandle.buffer->cg.request.commonHeader.queryActions  = 0U;
        cmdBd.req.addr                                           = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                      = sizeof(netc_tb_etmcg_req_data_t);
        cmdBd.req.resLength                                      = sizeof(netc_tb_etmcg_rsp_data_t);
        cmdBd.req.tableId                                        = kNETC_ECGTable;
        cmdBd.req.cmd                                            = kNETC_QueryEntry;
        cmdBd.req.accessType                                     = kNETC_EntryIDMatch;
        status = NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
        if (kStatus_Success == status)
        {
            *statistic = cdbrHandle.buffer->cg.response.stse;
        }
        return status;
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t SWT_FRERAddISEQGTableEntry(swt_handle_t *handle, netc_tb_iseqg_config_t *config)
{
    assert(handle != NULL);

    uint32_t entryNumMax = handle->hw.common->ISQGITCAPR & NETC_SW_ENETC_ISQGITCAPR_NUM_ENTRIES_MASK;
    status_t status      = kStatus_Success;
    netc_cmd_bd_t cmdBd  = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (config->entryID >= entryNumMax)
    {
        return kStatus_Fail;
    }

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        cdbrHandle.buffer->iseqg.request.entryID                    = config->entryID;
        cdbrHandle.buffer->iseqg.request.commonHeader.updateActions = 1;
        cdbrHandle.buffer->iseqg.request.commonHeader.queryActions  = 0;
        cdbrHandle.buffer->iseqg.request.cfge                       = config->cfge;
        cmdBd.req.addr                                              = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                         = 12U;
        cmdBd.req.resLength                                         = 8U;
        cmdBd.req.tableId                                           = kNETC_ISGTable;
        cmdBd.req.cmd                                               = kNETC_AddEntry;
        cmdBd.req.accessType                                        = kNETC_EntryIDMatch;
        status = NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        status = kStatus_Fail;
    }

    return status;
}

status_t SWT_FRERDelISEQGTableEntry(swt_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);

    uint32_t entryNumMax = handle->hw.common->ISQGITCAPR & NETC_SW_ENETC_ISQGITCAPR_NUM_ENTRIES_MASK;
    status_t status      = kStatus_Success;
    netc_cmd_bd_t cmdBd  = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (entryID >= entryNumMax)
    {
        return kStatus_Fail;
    }

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        cdbrHandle.buffer->iseqg.request.entryID                    = entryID;
        cdbrHandle.buffer->iseqg.request.commonHeader.updateActions = 0;
        cdbrHandle.buffer->iseqg.request.commonHeader.queryActions  = 0;
        cmdBd.req.addr                                              = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                         = 12U;
        cmdBd.req.resLength                                         = 8U;
        cmdBd.req.tableId                                           = kNETC_ISGTable;
        cmdBd.req.cmd                                               = kNETC_DeleteEntry;
        cmdBd.req.accessType                                        = kNETC_EntryIDMatch;
        status = NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        status = kStatus_Fail;
    }

    return status;
}

status_t SWT_FRERGetISEQGState(swt_handle_t *handle, uint32_t entryID, netc_tb_iseqg_sgse_t *state)
{
    assert(handle != NULL);

    uint32_t entryNumMax = handle->hw.common->ISQGITCAPR & NETC_SW_ENETC_ISQGITCAPR_NUM_ENTRIES_MASK;
    status_t status      = kStatus_Success;
    netc_cmd_bd_t cmdBd  = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (entryID >= entryNumMax)
    {
        return kStatus_Fail;
    }

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        cdbrHandle.buffer->iseqg.request.entryID                    = entryID;
        cdbrHandle.buffer->iseqg.request.commonHeader.updateActions = 0;
        cdbrHandle.buffer->iseqg.request.commonHeader.queryActions  = 0;
        cmdBd.req.addr                                              = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                         = 12U;
        cmdBd.req.resLength                                         = 8U;
        cmdBd.req.tableId                                           = kNETC_ISGTable;
        cmdBd.req.cmd                                               = kNETC_QueryEntry;
        cmdBd.req.accessType                                        = kNETC_EntryIDMatch;
        status = NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
        if (status == kStatus_Success)
        {
            *state = cdbrHandle.buffer->iseqg.response.sgse;
        }
    }
    else
    {
        status = kStatus_Fail;
    }

    return status;
}

status_t SWT_FRERConfigESEQRTableEntry(swt_handle_t *handle, netc_tb_eseqr_config_t *config)
{
    assert(handle != NULL);

    uint32_t entryNumMax = handle->hw.common->ESQRTCAPR & NETC_SW_ENETC_ESQRTCAPR_NUM_ENTRIES_MASK;
    status_t status      = kStatus_Success;
    netc_cmd_bd_t cmdBd  = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (config->entryID >= entryNumMax)
    {
        return kStatus_Fail;
    }

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        cdbrHandle.buffer->eseqr.request.entryID                    = config->entryID;
        cdbrHandle.buffer->eseqr.request.commonHeader.updateActions = 0x7;
        cdbrHandle.buffer->eseqr.request.commonHeader.queryActions  = 0;
        cdbrHandle.buffer->eseqr.request.cfge                       = config->cfge;
        cmdBd.req.addr                                              = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                         = 16U;
        cmdBd.req.resLength                                         = 84U;
        cmdBd.req.tableId                                           = kNETC_ESRTable;
        cmdBd.req.cmd                                               = kNETC_UpdateEntry;
        cmdBd.req.accessType                                        = kNETC_EntryIDMatch;
        status = NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
    }
    else
    {
        status = kStatus_Fail;
    }

    return status;
}

status_t SWT_FRERGetESEQRStatistic(swt_handle_t *handle,
                                   uint32_t entryID,
                                   netc_tb_eseqr_stse_t *statistic,
                                   netc_tb_eseqr_srse_t *state)
{
    assert(handle != NULL);

    uint32_t entryNumMax = handle->hw.common->ESQRTCAPR & NETC_SW_ENETC_ESQRTCAPR_NUM_ENTRIES_MASK;
    status_t status      = kStatus_Success;
    netc_cmd_bd_t cmdBd  = {0};
    netc_cbdr_handle_t cdbrHandle;

    if (entryID >= entryNumMax)
    {
        return kStatus_Fail;
    }

    if (SWT_GetIdleCmdBDRing(handle, &cdbrHandle) == kStatus_Success)
    {
        cdbrHandle.buffer->eseqr.request.entryID                    = entryID;
        cdbrHandle.buffer->eseqr.request.commonHeader.updateActions = 0;
        cdbrHandle.buffer->eseqr.request.commonHeader.queryActions  = 0;
        cmdBd.req.addr                                              = (uintptr_t)cdbrHandle.buffer;
        cmdBd.req.reqLength                                         = 16U;
        cmdBd.req.resLength                                         = 84U;
        cmdBd.req.tableId                                           = kNETC_ESRTable;
        cmdBd.req.cmd                                               = kNETC_QueryEntry;
        cmdBd.req.accessType                                        = kNETC_EntryIDMatch;
        status = NETC_CmdBDSendCommand(cdbrHandle.base, cdbrHandle.cmdr, &cmdBd, kNETC_NtmpV2_0);
        if (status == kStatus_Success)
        {
            *statistic = cdbrHandle.buffer->eseqr.response.stse;
            *state     = cdbrHandle.buffer->eseqr.response.srse;
        }
    }
    else
    {
        status = kStatus_Fail;
    }

    return status;
}
