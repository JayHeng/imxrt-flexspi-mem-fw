/*
 * Copyright 2021-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_netc_endpoint.h"

/*! @name NETC register map. */
/*@{*/
#define NETC_ENETC_PCIE_FUNC_OFFSET       3U       /*!< The ENETC PCIE function index. */
#define NETC_ENETC_PORT_GROUP_BASE_OFFSET 0x4000U  /*!< The ENETC port group register base address offset. */
#define NETC_ENETC_GLOBAL_BASE_OFFSET     0x10000U /*!< The ENETC global register base address offset. */
/*@}*/

/*! @name MSIX table memory address. */
/*@{*/
#define NETC_MSIX_TABLE_BASE       0x60BC0000U
#define NETC_MSIX_TABLE_OFFSET     0x10000U
#define NETC_MSIX_TABLE_PBA_OFFSET 0x800U
/*@}*/

/*! @name Defines some Ethernet parameters. */
/*@{*/
#define NETC_ENETC_TXFRAME_LEN_MAX (9600U) /*!< The Maximum length of frame length. */
#define NETC_ENETC_TXFRAME_LEN_MIN (16U)   /*!< The Minimum length of frame length. */
/*@}*/

/*! @name Get SI information from netc_hw_si_idx_t. */
/*@{*/
#define getSiInstance(si) ((uint16_t)si >> 8U)          /*!< The ENETC instance of this SI. */
#define getSiNum(si)      (((uint16_t)si >> 4U) & 0xFU) /*!< The SI number in the ENETC. */
#define getSiIdx(si)      ((uint16_t)si & 0xFU)         /*!< The actaul index in the netc_hw_si_idx_t. */
/*@}*/

/*! @brief Pointers to netc bases for each instance. */
static ENETC_PCI_TYPE0_Type *const s_netcBases[] = ENETC_PCI_TYPE0_BASE_PTRS;

/*! @brief Pointers to enetc bases for each instance. */
static NETC_ENETC_Type *const s_netcEnetcBases[] = NETC_ENETC_BASE_PTRS;

/*! @brief Command BD common buffer. */
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tb_data_buffer_t s_cmdData, 16);

/*!
 * @brief Get register map resource for handle
 *
 * @param handle  The EP handle.
 * @param si      The SI object.
 */
static void EP_GetBaseResource(ep_handle_t *handle, netc_hw_si_idx_t si)
{
    uint8_t instance = getSiInstance(si);
    uint8_t siNum    = getSiNum(si);
    uint8_t siIdx    = getSiIdx(si);

    handle->hw.func           = s_netcBases[NETC_ENETC_PCIE_FUNC_OFFSET + instance];
    handle->hw.base           = s_netcEnetcBases[instance];
    handle->hw.common         = (NETC_SW_ENETC_Type *)((uint32_t)handle->hw.base);
    handle->hw.portGroup.port = (NETC_PORT_Type *)((uint32_t)handle->hw.base + NETC_ENETC_PORT_GROUP_BASE_OFFSET);
    handle->hw.portGroup.eth  = (NETC_ETH_LINK_Type *)((uint32_t)handle->hw.base + NETC_ENETC_PORT_GROUP_BASE_OFFSET);
    handle->hw.global         = (ENETC_GLOBAL_Type *)((uint32_t)handle->hw.base + 0x10000U);
    if (siNum == 0U)
    {
        handle->hw.si = (ENETC_SI_Type *)((uint32_t)handle->hw.base - 0x10000U);
    }
    else
    {
        handle->hw.si = (ENETC_SI_Type *)((uint32_t)handle->hw.base + 0xC0000U);
    }
    handle->hw.msixTable = (netc_msix_entry_t *)(NETC_MSIX_TABLE_BASE + NETC_MSIX_TABLE_OFFSET * (3U + siIdx));
}

/*!
 * @brief Initialize the Tx/Rx BD ring
 *
 * @param handle     The EP handle.
 * @param config     The EP configuration.
 * @param bdrConfig  The BD ring configuration.
 */
static status_t EP_DescriptorInit(ep_handle_t *handle, const ep_config_t *config, const netc_si_bdr_config_t *bdrConfig)
{
    assert(config != NULL);

    uint8_t ringUse;
    uint8_t ring;

    assert(bdrConfig != NULL);

    ringUse = MAX(config->siConfig.txRingUse, config->siConfig.rxRingUse);

    for (ring = 0; ring < ringUse; ring++)
    {
        if (ring < config->siConfig.txRingUse)
        {
            if ((NETC_SIConfigTxBDR(handle->hw.si, ring, bdrConfig) != kStatus_Success))
            {
                return kStatus_Fail;
            }
            handle->txBdRing[ring].bdBase    = bdrConfig->txBdConfig[ring].bdArray;
            handle->txBdRing[ring].dirtyBase = bdrConfig->txBdConfig[ring].dirtyArray;
            handle->txBdRing[ring].len       = bdrConfig->txBdConfig[ring].len;
        }

        if (ring < config->siConfig.rxRingUse)
        {
            if ((NETC_SIConfigRxBDR(handle->hw.si, ring, bdrConfig, config->rxZeroCopy) != kStatus_Success))
            {
                return kStatus_Fail;
            }
            handle->rxBdRing[ring].bdBase     = bdrConfig->rxBdConfig[ring].bdArray;
            handle->rxBdRing[ring].extendDesc = bdrConfig->rxBdConfig[ring].extendDescEn;
            /* Double 16-bytes BD memory space should be provided by application. */
            handle->rxBdRing[ring].len = bdrConfig->rxBdConfig[ring].len;
        }
    }

    NETC_SISetRxBDRGroup(handle->hw.si, bdrConfig->rxBdrGroupNum, bdrConfig->ringPerGroup);

    handle->rxBufferArray = bdrConfig->rxBufferAddr;
    handle->rxBufferSize  = bdrConfig->rxBufferSize;

    return kStatus_Success;
}

/*!
 * @brief Setup Station Interface
 *
 * @param handle     The EP handle.
 * @param si         The SI number.
 * @param epConfig   The EP configuration.
 * @param bdrConfig  The BD ring configuration.
 */
static status_t EP_SISetup(ep_handle_t *handle,
                           uint8_t si,
                           const ep_config_t *epConfig,
                           const netc_si_bdr_config_t *bdrConfig)
{
    status_t result;

    /* Configure station interface. */
    result = NETC_EnetcConfigureSI(handle->hw.base, si, &epConfig->siConfig);
    if (result != kStatus_Success)
    {
        return result;
    }
    NETC_SIEnableVlanToIpv(handle->hw.si, true);

    /* Initialize the Tx/Rx buffer descriptor. */
    result = EP_DescriptorInit(handle, epConfig, bdrConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Enable station interface from both ENETC and SI sides. */
    NETC_EnetcEnableSI(handle->hw.base, si, true);
    NETC_SIEnable(handle->hw.si, true);

    return result;
}

/*!
 * @brief Set the MSIX entry table
 *
 * This function is to set entry table in specified MSIX Table memory. After entry table setup, need to set interrupts
 * vector to certain entry.
 *
 * @param handle    The EP handle
 * @param entry     The entry table structure array.
 * @param entryNum  The entry number.
 * @return status_t
 */
static status_t EP_MSIXSetEntryTable(ep_handle_t *handle, netc_msix_entry_t *entry, uint8_t entryNum)
{
    uint8_t msixNum =
        ((handle->hw.si->SIPCAPR1 & ENETC_SI_SIPCAPR1_NUM_MSIX_MASK) >> ENETC_SI_SIPCAPR1_NUM_MSIX_SHIFT) + 1U;
    uint8_t siNum        = getSiNum(handle->cfg->si);
    uint32_t *msixTable  = (uint32_t *)handle->hw.msixTable;
    uint32_t *entryTable = (uint32_t *)entry;
    status_t result;

    result = NETC_EnetcSetMsixEntryNum(handle->hw.base, siNum, entryNum);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Entry > 0, enable MSIX and disable mask. */
    if (entryNum != 0U)
    {
        if (entryNum > msixNum)
        {
            return kStatus_Fail;
        }

        /* Use 32-bit access to set MSIX table. */
        for (uint32_t i = 0; i < entryNum; i++)
        {
            msixTable[i * 4]     = entryTable[i * 4];
            msixTable[i * 4 + 1] = entryTable[i * 4 + 1];
            msixTable[i * 4 + 2] = entryTable[i * 4 + 2];
            msixTable[i * 4 + 3] = entryTable[i * 4 + 3];
        }
        handle->hw.func->PCI_CFC_MSIX_MSG_CTL = ENETC_PCI_TYPE0_PCI_CFC_MSIX_MSG_CTL_MSIX_EN_MASK;
    }
    else
    {
        handle->hw.func->PCI_CFC_MSIX_MSG_CTL = ENETC_PCI_TYPE0_PCI_CFC_MSIX_MSG_CTL_FUNC_MASK_MASK;
    }

    return kStatus_Success;
}

/*!
 * @brief Get the idle Tx BD number in the ring
 * @note The last BD before producer index should be treated as an used BD according to hardware design.
 *
 * @param txBdRing  The EP handle.
 * @return The idle BD number.
 */
static uint16_t EP_GetIdleTxBDNum(netc_tx_bdr_t *txBdRing)
{
    if (txBdRing->producerIndex >= txBdRing->cleanIndex)
    {
        return ((txBdRing->len - txBdRing->producerIndex) + txBdRing->cleanIndex - 1U);
    }
    else
    {
        return (txBdRing->cleanIndex - txBdRing->producerIndex - 1U);
    }
}

/*!
 * @brief Get the dirty Tx BD number ready for reclaiming in the ring
 *
 * @param handle  The EP handle.
 * @param ring  The ring index.
 * @return The dirty BD number.
 */
static uint16_t EP_GetDirtyTxBDNum(ep_handle_t *handle, uint8_t ring)
{
    netc_tx_bdr_t *txBdRing = &handle->txBdRing[ring];
    uint16_t consumer       = NETC_SIGetTxConsumer(handle->hw.si, ring);

    if (consumer >= txBdRing->cleanIndex)
    {
        return (consumer - txBdRing->cleanIndex);
    }
    else
    {
        return (txBdRing->len - txBdRing->cleanIndex + consumer);
    }
}

status_t EP_GetDefaultConfig(ep_config_t *config)
{
    assert(config != NULL);

    (void)memset(config, 0, sizeof(ep_config_t));

    config->port.common.acceptTpid.innerMask   = kNETC_OuterStanCvlan | kNETC_OuterStanSvlan;
    config->port.common.acceptTpid.outerMask   = kNETC_InnerStanCvlan | kNETC_InnerStanSvlan;
    config->port.common.pSpeed                 = 0x63U;
    config->port.common.rxTsSelect             = kNETC_SyncTime;
    config->port.common.stompFcs               = true;
    config->port.common.sduCfg.rxPpduBco       = 20U;
    config->port.common.sduCfg.txPpduBco       = 20U;
    config->port.common.timeGate.holdSkew      = 64;
    config->port.ethMac.enableRevMii           = false;
    config->port.ethMac.preemptMode            = kNETC_PreemptDisable;
    config->port.ethMac.enMergeVerify          = false;
    config->port.ethMac.mergeVerifyTime        = 10U;
    config->port.ethMac.txTsSelect             = kNETC_SyncTime;
    config->psfpCfg.isiPortConfig.defaultISEID = 0xFFFFU;
    for (uint32_t i = 0U; i < 8U; i++)
    {
        config->txTcCfg[i].sduCfg.maxSduSized = 0x600U;
        config->txTcCfg[i].sduCfg.sduType     = kNETC_MPDU;
        config->txPrioToTC[i]                 = i;
    }

    return kStatus_Success;
}

status_t EP_Init(ep_handle_t *handle,
                 uint8_t *macAddr,
                 const ep_config_t *config,
                 const netc_si_bdr_config_t *bdrConfig)
{
    uint8_t siNum = getSiNum(config->si);
    status_t result;

    memset(handle, 0, sizeof(ep_handle_t));

    /* Get the register map resource for ENETC */
    EP_GetBaseResource(handle, config->si);

    /* Store configuration in handle */
    handle->cfg = config;

    /* Reset this function */
    handle->hw.func->PCI_CFC_PCIE_DEV_CTL |= ENETC_PCI_TYPE0_PCI_CFC_PCIE_DEV_CTL_INIT_FLR_MASK;
    while ((handle->hw.func->PCI_CFC_PCIE_DEV_CTL & ENETC_PCI_TYPE0_PCI_CFC_PCIE_DEV_CTL_INIT_FLR_MASK) != 0U)
    {
    }

    /* Enable master bus and memory access for PCIe and MSI-X */
    handle->hw.func->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);

    if (siNum != 0U)
    {
        /* Enable virtual function */
        handle->hw.func->PCIE_CFC_SRIOV_CTL |=
            (ENETC_PCI_TYPE0_PCIE_CFC_SRIOV_CTL_VF_MSE_MASK | ENETC_PCI_TYPE0_PCIE_CFC_SRIOV_CTL_VF_ENABLE_MASK);
    }
    else
    {
        /* Only PSI needs to configure port */
        result = NETC_PortConfig(handle->hw.portGroup.port, &config->port.common);
        if (result != kStatus_Success)
        {
            return result;
        }

        if (!NETC_PortIsPseudo(handle->hw.portGroup.port))
        {
            result = NETC_PortConfigEthMac(handle->hw.portGroup.eth, &config->port.ethMac);
            if (result != kStatus_Success)
            {
                return result;
            }
        }
    }

    /* Set primary MAC address */
    result = EP_SetPrimaryMacAddr(handle, macAddr);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Configure ENETC PSFP */
    (void)EP_RxPSFPInit(handle, &config->psfpCfg);

    /* Transmit priority to traffic class mapping */
    handle->hw.base->PRIO2TCMR0 =
        NETC_ENETC_PRIO2TCMR0_PRIO7TC(config->txPrioToTC[7]) | NETC_ENETC_PRIO2TCMR0_PRIO6TC(config->txPrioToTC[6]) |
        NETC_ENETC_PRIO2TCMR0_PRIO5TC(config->txPrioToTC[5]) | NETC_ENETC_PRIO2TCMR0_PRIO4TC(config->txPrioToTC[4]) |
        NETC_ENETC_PRIO2TCMR0_PRIO3TC(config->txPrioToTC[3]) | NETC_ENETC_PRIO2TCMR0_PRIO2TC(config->txPrioToTC[2]) |
        NETC_ENETC_PRIO2TCMR0_PRIO1TC(config->txPrioToTC[1]) | NETC_ENETC_PRIO2TCMR0_PRIO0TC(config->txPrioToTC[0]);

    /* Configure traffic class */
    for (uint32_t i = 8U; i > 0U; i--)
    {
        result = EP_TxTrafficClassConfig(handle, (netc_hw_tc_idx_t)(i - 1U), &config->txTcCfg[(i - 1U)]);
        if (result != kStatus_Success)
        {
            return result;
        }
    }

    /* Set the MSIX table to handle interrupt */
    result = EP_MSIXSetEntryTable(handle, config->msixEntry, config->entryNum);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Initialize station interface */
    result = EP_SISetup(handle, siNum, config, bdrConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Initialize command BD ring for NTMP access */
    result = EP_CmdBDRInit(handle, &config->cmdBdrConfig);
    if (result != kStatus_Success)
    {
        return result;
    }
    handle->cmdBdRing.bdBase        = config->cmdBdrConfig.bdBase;
    handle->cmdBdRing.bdLength      = config->cmdBdrConfig.bdLength;
    handle->cmdBdRing.producerIndex = 0U;
    handle->cmdBdRing.cleanIndex    = 0U;
    handle->cmdBdRing.bdrEnable     = true;

    return result;
}

status_t EP_Deinit(ep_handle_t *handle)
{
    uint8_t siNum = getSiNum(handle->cfg->si);

    EP_CmdBDRDeinit(handle);
    NETC_EnetcEnableSI(handle->hw.base, siNum, false);
    NETC_SIEnable(handle->hw.si, false);

    return kStatus_Success;
}

status_t EP_SetPrimaryMacAddr(ep_handle_t *handle, uint8_t *macAddr)
{
    uint8_t siNum = getSiNum(handle->cfg->si);

    if (siNum == 0U)
    {
        NETC_PortSetMacAddr(handle->hw.portGroup.port, macAddr);
    }
    else
    {
        NETC_EnetcSetSIMacAddr(handle->hw.base, siNum, macAddr);
    }

    return kStatus_Success;
}

static void EP_SetupTxDescriptor(netc_tx_bd_t *txDesc,
                                 void *buffer,
                                 uint32_t buffLen,
                                 uint32_t frameLen,
                                 uint32_t bdIndex,
                                 uint32_t totBdNum,
                                 bool isExtEnable,
                                 bool isPseudoMac,
                                 bool isIntrEn,
                                 ep_tx_opt *opt)
{
    uint64_t address;
    bool tsReq;

    memset(txDesc, 0, sizeof(netc_tx_bd_t));

    /* Preare the descriptor for transmit. */
    if (bdIndex == 0U)
    {
#if defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
        address = (uintptr_t)MEMORY_ConvertMemoryMapAddress((uint32_t)(uint32_t *)buffer, kMEMORY_Local2DMA);
#else
        address = (uintptr_t)(uint32_t *)buffer;
#endif
        txDesc->standard.addr     = address;
        txDesc->standard.bufLen   = buffLen;
        txDesc->standard.frameLen = frameLen;

        /* Set the extension flag in first BD if related features are enabled */
        if (isExtEnable)
        {
            txDesc->standard.isExtended = 1;
        }

        if (isIntrEn)
        {
            txDesc->standard.enableInterrupt = 1;
        }

        if (opt != NULL)
        {
            if (isPseudoMac)
            {
                tsReq                  = ((opt->flags & kEP_TX_OPT_REQ_TS) != 0U);
                txDesc->standard.flags = NETC_SI_TXDESCRIP_RD_FL(0) | NETC_SI_TXDESCRIP_RD_FLQ(2) |
                                         NETC_SI_TXDESCRIP_RD_PORT(opt->swtPort) | NETC_SI_TXDESCRIP_RD_TSR(tsReq);
                if ((opt->flags & kEP_TX_OPT_MASQUERADE) == 0U)
                {
                    txDesc->standard.flags |= NETC_SI_TXDESCRIP_RD_SMSO_MASK;
                }
            }
            else if ((opt->flags & kEP_TX_OPT_START_TIME) != 0U)
            {
                txDesc->standard.flags = NETC_SI_TXDESCRIP_RD_FL(2) | NETC_SI_TXDESCRIP_RD_TSE_MASK |
                                         NETC_SI_TXDESCRIP_RD_TXSTART(opt->timestamp);
            }
            else
            {
                /* Intentional empty */
            }
        }
    }
    else if ((bdIndex == 1U) && isExtEnable)
    {
        if ((opt->flags & kEP_TX_OPT_VLAN_INSERT) != 0U)
        {
            txDesc->ext.pcp  = opt->vlan.pcp;
            txDesc->ext.dei  = opt->vlan.dei;
            txDesc->ext.vid  = opt->vlan.vid;
            txDesc->ext.tpid = opt->vlan.tpid;
            txDesc->ext.eFlags |= kEP_TX_EXT_VLAN_INSERT;
        }

        if (((opt->flags & kEP_TX_OPT_REQ_TS) != 0U) && (!isPseudoMac))
        {
            txDesc->ext.eFlags |= kEP_TX_EXT_TWO_STEP_TS;
        }
    }
    else
    {
#if defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
        address = (uintptr_t)MEMORY_ConvertMemoryMapAddress((uint32_t)(uint32_t *)buffer, kMEMORY_Local2DMA);
#else
        address = (uintptr_t)(uint32_t *)buffer;
#endif
        txDesc->standard.addr   = address;
        txDesc->standard.bufLen = buffLen;
    }

    /* Set final flag in last BD, same bit for stand or extension BD */
    if (bdIndex == (totBdNum - 1U))
    {
        txDesc->standard.isFinal = 1;
    }
}

status_t EP_SendFrame(ep_handle_t *handle, uint8_t ring, netc_frame_struct_t *frame, void *context, ep_tx_opt *opt)
{
    assert(handle != NULL);
    assert(ring < handle->cfg->siConfig.txRingUse);
    assert(frame->buffArray != NULL);
    /* First descriptor in a chain must not have a BUFF_LEN that is less than 16 bytes. */
    assert(frame->buffArray[0].length >= NETC_ENETC_TXFRAME_LEN_MIN);
    assert(frame->length != 0U);

    status_t result              = kStatus_Success;
    netc_tx_bdr_t *txBdRing      = &handle->txBdRing[ring];
    netc_buffer_struct_t *txBuff = frame->buffArray;
    uint32_t totBdNum            = frame->length;
    uint32_t bdIndex             = 0;
    uint32_t frameLen            = 0;
    bool isExtEnable             = false;
    bool isPseudoMac             = false;
    bool isIntrEn                = false;
    netc_tx_frame_info_t *txDirty;
    netc_tx_bd_t *txDesc;

    /* Calculate frame length and Tx data buffer number. */
    do
    {
        frameLen += txBuff->length;
        txBuff++;
    } while (--totBdNum != 0U);
    totBdNum = frame->length;

    if (NETC_PortIsPseudo(handle->hw.portGroup.port))
    {
        isPseudoMac = true;
    }

    /* Check option features. */
    if (opt != NULL)
    {
        /* Only standalone endpoint port supports TSD. */
        if (isPseudoMac && ((opt->flags & kEP_TX_OPT_START_TIME) != 0U))
        {
            return kStatus_InvalidArgument;
        }

        /* Only ring0 supports direct switch enqueue. */
        if (isPseudoMac && ((opt->flags & kEP_TX_OPT_MASQUERADE) == 0U) && (ring != 0U))
        {
            return kStatus_InvalidArgument;
        }

        /* Some features needs extended BD. */
        if ((opt->flags & kEP_TX_OPT_VLAN_INSERT) != 0U)
        {
            totBdNum++;
            isExtEnable = true;
        }
        else if (((opt->flags & kEP_TX_OPT_REQ_TS) != 0U) && (!isPseudoMac))
        {
            totBdNum++;
            isExtEnable = true;
        }
    }

    /* Check the frame length. */
    if ((frameLen > NETC_ENETC_TXFRAME_LEN_MAX) || (frameLen < NETC_ENETC_TXFRAME_LEN_MIN))
    {
        result = kStatus_NETC_TxFrameOverLen;
    }
    /* Check whether the available BD number is enough for Tx data buffer. */
    else if (totBdNum > EP_GetIdleTxBDNum(txBdRing))
    {
        result = kStatus_NETC_Busy;
    }
    else
    {
        txBuff = frame->buffArray;

        isIntrEn = ((handle->hw.si->BDR[ring].TBIER & ENETC_SI_TBIER_TXFIE_MASK) != 0U) ? true : false;

        for (bdIndex = 0; bdIndex < totBdNum; bdIndex++)
        {
            /* Needn't check the buffer address when set extended BD. */
            if ((txBuff->buffer == NULL) && isExtEnable && (bdIndex != 1U))
            {
                return kStatus_InvalidArgument;
            }

            if (handle->cfg->txCacheMaintain)
            {
                // DCACHE_CleanByRange((uint32_t)txBuff->buffer, txBuff->length);
            }

            /* Set context frame info in the first BD. */
            if (bdIndex == 0U)
            {
                /* Add context to frame info ring. */
                txDirty          = &txBdRing->dirtyBase[txBdRing->producerIndex];
                txDirty->context = context;
            }

            /* Get latest Rx BD and set it. */
            txDesc = &txBdRing->bdBase[txBdRing->producerIndex];
            EP_SetupTxDescriptor(txDesc, txBuff->buffer, txBuff->length, frameLen, bdIndex, totBdNum, isExtEnable,
                                 isPseudoMac, isIntrEn, opt);

            /* When extended BD enables, jump second BD which needn't buffer address. */
            if (!((bdIndex == 1U) && isExtEnable))
            {
                txBuff++;
            }

            /* Increase producer index. */
            txBdRing->producerIndex = EP_IncreaseIndex(txBdRing->producerIndex, txBdRing->len);
        }

        /* Make sure all data in the Tx BD is ready. */
        __DSB();

        /* Active Tx. */
        NETC_SISetTxProducer(handle->hw.si, ring, txBdRing->producerIndex);
    }

    return result;
}

void EP_ReclaimTxDescriptor(ep_handle_t *handle, uint8_t ring)
{
    netc_tx_bdr_t *txBdRing = &handle->txBdRing[ring];
    netc_tx_frame_info_t *frameInfo;
    netc_tx_bd_t *txDesc;
    uint16_t cleanNum = EP_GetDirtyTxBDNum(handle, ring);

    while (cleanNum--)
    {
        /* When callback enable, get the reclaim information. */
        if (handle->cfg->reclaim_cb != NULL)
        {
            txDesc = &txBdRing->bdBase[txBdRing->cleanIndex];

            /* Valid Tx frame information is in the first BD of one BD chain. */
            if (txDesc->writeback.written != 0U)
            {
                frameInfo            = &txBdRing->dirtyBase[txBdRing->cleanIndex];
                frameInfo->isTsAvail = true;
                frameInfo->timestamp = txDesc->writeback.timestamp;
                frameInfo->status    = (netc_ep_tx_status_t)txDesc->writeback.status;

                handle->cfg->reclaim_cb(handle, frameInfo);
                memset(frameInfo, 0, sizeof(netc_tx_frame_info_t));
            }
            else if (txDesc->standard.frameLen != 0U)
            {
                frameInfo         = &txBdRing->dirtyBase[txBdRing->cleanIndex];
                frameInfo->status = kNETC_EPTxSuccess;

                handle->cfg->reclaim_cb(handle, frameInfo);
                memset(frameInfo, 0, sizeof(netc_tx_frame_info_t));
            }
            else
            {
                /* Intentional empty */
            }
        }

        /* Update the clean index to free BD resource. */
        txBdRing->cleanIndex = EP_IncreaseIndex(txBdRing->cleanIndex, txBdRing->len);

        if (cleanNum == 0U)
        {
            cleanNum = EP_GetDirtyTxBDNum(handle, ring);
        }
    }
}

status_t EP_GetRxFrameSize(ep_handle_t *handle, uint8_t ring, uint32_t *length)
{
    assert(handle != NULL);
    assert(length != NULL);
    assert(ring < handle->cfg->siConfig.rxRingUse);

    netc_rx_bdr_t *rxBdRing = &handle->rxBdRing[ring];
    netc_rx_bd_t *rxDesc    = &rxBdRing->bdBase[rxBdRing->index];
    uint16_t index          = rxBdRing->index;
    uint32_t totlen         = 0;
    status_t result         = kStatus_Success;

    /* Reset the length to zero. */
    *length = 0;

    /* Check the current buffer descriptor's ready flag. The flag in first BD means entire frame ready status. */
    if (!rxDesc->writeback.isReady)
    {
        result = kStatus_NETC_RxFrameEmpty;
    }
    else
    {
        /* Check the status of the received frame. */
        if (rxDesc->writeback.error != 0U)
        {
            result = kStatus_NETC_RxFrameError;
        }
        else
        {
            do
            {
                /* Add check for abnormal case. */
                if (rxDesc->writeback.bufLen == 0U)
                {
                    result = kStatus_NETC_RxFrameError;
                    break;
                }
                totlen += rxDesc->writeback.bufLen;

                /* Find the last buffer descriptor. */
                if (rxDesc->writeback.isFinal)
                {
                    *length = totlen;
                    break;
                }

                /* Increase to next buffer descriptor. */
                if (rxBdRing->extendDesc)
                {
                    /* Extended BD needs additional offset. */
                    index = EP_IncreaseIndex(index, rxBdRing->len);
                }
                index  = EP_IncreaseIndex(index, rxBdRing->len);
                rxDesc = &rxBdRing->bdBase[index];
            } while (index != rxBdRing->index);
        }
    }

    return result;
}

static void EP_GetRxFrameAttribute(netc_rx_bd_t *rxDesc, netc_frame_attr_t *attr)
{
    (void)memset(attr, 0, sizeof(netc_frame_attr_t));

    attr->parserSummary = rxDesc->writeback.parserSummary;
    attr->hostReason    = (netc_host_reason_t)rxDesc->writeback.hr;

    if ((rxDesc->writeback.flags & kEP_RX_RSS_VALID) != 0U)
    {
        attr->isRssHashValid = true;
        attr->rssHash        = rxDesc->writeback.rssHashSwt;
    }
    if ((rxDesc->writeback.flags & kEP_RX_VLAN_VALID) != 0U)
    {
        attr->isVlanExtracted = true;
        attr->vlan.vid        = rxDesc->writeback.vid;
        attr->vlan.dei        = rxDesc->writeback.dei;
        attr->vlan.pcp        = rxDesc->writeback.pcp;
        attr->vlan.tpid       = rxDesc->writeback.tpid;
    }
    if ((rxDesc->writeback.flags & kEP_RX_TIMESTAMP_VALID) != 0U)
    {
        attr->isTsAvail = true;
    }
}

static void EP_DropFrame(ep_handle_t *handle, uint8_t ring)
{
    netc_rx_bdr_t *rxBdRing = &handle->rxBdRing[ring];
    netc_rx_bd_t *rxDesc;

    /* Update the buffer address in the BD which is writed back by hardware */
    do
    {
        rxDesc = &rxBdRing->bdBase[rxBdRing->index];
        if (rxBdRing->extendDesc)
        {
            rxDesc->standard.addr =
                (uintptr_t)&handle->rxBufferArray[(ring + 1U) * (rxBdRing->index / 2U) * handle->rxBufferSize];

            /* Skip 16-bytes extend BD space */
            rxBdRing->index = EP_IncreaseIndex(rxBdRing->index, rxBdRing->len);
        }
        else
        {
            rxDesc->standard.addr =
                (uintptr_t)&handle->rxBufferArray[(ring + 1U) * rxBdRing->index * handle->rxBufferSize];
        }
        rxBdRing->index = EP_IncreaseIndex(rxBdRing->index, rxBdRing->len);
    } while (!rxDesc->writeback.isFinal);

    /* Update the Rx consumer index to free idle BD */
    if (rxBdRing->extendDesc)
    {
        NETC_SISetRxConsumer(handle->hw.si, ring, rxBdRing->index / 2U);
    }
    else
    {
        NETC_SISetRxConsumer(handle->hw.si, ring, rxBdRing->index);
    }
}

status_t EP_ReceiveFrameCopy(ep_handle_t *handle, uint8_t ring, void *buffer, uint32_t length, netc_frame_attr_t *attr)
{
    assert(handle != NULL);
    assert(ring < handle->cfg->siConfig.rxRingUse);

    netc_rx_bdr_t *rxBdRing = &handle->rxBdRing[ring];
    netc_rx_bd_t *rxDesc    = &rxBdRing->bdBase[rxBdRing->index];
    status_t result         = kStatus_Success;
    uint32_t buffLen        = 0;
    uint32_t offset         = 0;
    bool isLastBuff         = false;
    bool isTsBd             = true;
    uintptr_t rxDmaBuff;
    uint32_t data;

    /* For data-NULL input, drop the frame. */
    if (buffer == NULL)
    {
        EP_DropFrame(handle, ring);
    }
    else
    {
        /* Get the Rx frame information from first BD */
        if (attr != NULL)
        {
            EP_GetRxFrameAttribute(rxDesc, attr);
        }

        do
        {
            /* Get the destination buffer address which NETC DMA copy frames to. */
            if (rxBdRing->extendDesc)
            {
                rxDmaBuff =
                    (uintptr_t)&handle->rxBufferArray[(ring + 1U) * (rxBdRing->index / 2U) * handle->rxBufferSize];
            }
            else
            {
                rxDmaBuff = (uintptr_t)&handle->rxBufferArray[(ring + 1U) * rxBdRing->index * handle->rxBufferSize];
            }

            if (handle->cfg->rxCacheMaintain)
            {
                /* Add the cache invalidate maintain. */
                // DCACHE_InvalidateByRange(rxDmaBuff, handle->rxBuffSizeAlign);
            }

            data    = (uint32_t)buffer + offset;
            buffLen = rxDesc->writeback.bufLen;
            (void)memcpy((void *)(uintptr_t)data, (void *)rxDmaBuff, buffLen);
            offset += buffLen;

            /* The last buffer descriptor of a frame. */
            isLastBuff = ((rxDesc->writeback.isFinal) ? true : false);

            /* Updates the receive buffer descriptors. */
            rxDesc->writeback.isReady = false;
            if (rxBdRing->extendDesc)
            {
#if defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
                rxDmaBuff = (uintptr_t)MEMORY_ConvertMemoryMapAddress(
                    (uint32_t)&handle->rxBufferArray[(ring + 1U) * (rxBdRing->index / 2U) * handle->rxBufferSize],
                    kMEMORY_Local2DMA);
#else
                rxDmaBuff =
                    (uintptr_t)&handle->rxBufferArray[(ring + 1U) * (rxBdRing->index / 2U) * handle->rxBufferSize];
#endif
                rxDesc->standard.addr = rxDmaBuff;

                /* Extended BD occupies one more 16 bytes space. */
                rxBdRing->index = EP_IncreaseIndex(rxBdRing->index, rxBdRing->len);
                rxDesc          = &rxBdRing->bdBase[rxBdRing->index];

                /* Get the timestamp from extended BD if it's valid. */
                if (isTsBd && attr->isTsAvail)
                {
                    /* Only first extended BD takes timestamp. */
                    isTsBd          = false;
                    attr->timestamp = rxDesc->ext.timestamp;
                }
            }
            else
            {
#if defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
                rxDmaBuff = (uintptr_t)MEMORY_ConvertMemoryMapAddress(
                    (uint32_t)&handle->rxBufferArray[(ring + 1U) * rxBdRing->index * handle->rxBufferSize],
                    kMEMORY_Local2DMA);
#else
                rxDmaBuff = (uintptr_t)&handle->rxBufferArray[(ring + 1U) * rxBdRing->index * handle->rxBufferSize];
#endif
                rxDesc->standard.addr = rxDmaBuff;
            }

            /* Get the latest buffer descriptor. */
            rxBdRing->index = EP_IncreaseIndex(rxBdRing->index, rxBdRing->len);
            rxDesc          = &rxBdRing->bdBase[rxBdRing->index];
        } while (!isLastBuff);

        /* Update the Rx consumer index to free idle BD */
        if (rxBdRing->extendDesc)
        {
            NETC_SISetRxConsumer(handle->hw.si, ring, rxBdRing->index / 2U);
        }
        else
        {
            NETC_SISetRxConsumer(handle->hw.si, ring, rxBdRing->index);
        }
    }

    return result;
}

status_t EP_CmdBDRInit(ep_handle_t *handle, const netc_cmd_bdr_config_t *config)
{
    assert(handle != NULL);
    return NETC_CmdBDRInit((netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR, config);
}

status_t EP_CmdBDRDeinit(ep_handle_t *handle)
{
    assert(handle != NULL);
    return NETC_CmdBDRDeinit((netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR);
}

status_t EP_RxIPFInit(ep_handle_t *handle, netc_ep_ipf_config_t *config)
{
    NETC_EnetcSetIPF(handle->hw.common, config->l2DiscardMCSmac, config->l2DiscardSmacEquDmac);
    NETC_PortSetIPF(handle->hw.portGroup.port, &config->portConfig);

    return kStatus_Success;
}

status_t EP_RxIPFAddTableEntry(ep_handle_t *handle, netc_tb_ipf_config_t *config, uint32_t *entryID)
{
    assert((handle != NULL) && (config != NULL));
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_AddIPFTableEntry(&cdbrHandle, config, entryID);
}

status_t EP_RxIPFDelTableEntry(ep_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_DelIPFTableEntry(&cdbrHandle, entryID);
}

status_t EP_RxIPFResetMatchCounter(ep_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_ResetIPFMatchCounter(&cdbrHandle, entryID);
}

status_t EP_RxIPFGetMatchedCount(ep_handle_t *handle, uint32_t entryID, uint64_t *count)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_GetIPFMatchedCount(&cdbrHandle, entryID, count);
}

status_t EP_RxPSFPAddSGITableEntry(ep_handle_t *handle, netc_tb_sgi_config_t *config)
{
    assert((handle != NULL) && (config != NULL));
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_AddSGITableEntry(&cdbrHandle, config);
}

status_t EP_RxPSFPDelSGITableEntry(ep_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_DelSGITableEntry(&cdbrHandle, entryID);
}

status_t EP_RxPSFPGetSGIState(ep_handle_t *handle, uint32_t entryID, netc_tb_sgi_sgise_t *state)
{
    assert((handle != NULL) && (state != NULL));
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_GetSGIState(&cdbrHandle, entryID, state);
}

status_t EP_RxPSFPAddSGCLTableEntry(ep_handle_t *handle, netc_tb_sgcl_config_t *config, uint8_t *buffer)
{
    assert((handle != NULL) && (buffer != NULL) && (config != NULL));
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = (netc_tb_data_buffer_t *)buffer;
    return NETC_AddSGCLTableEntry(&cdbrHandle, config);
}

status_t EP_RxPSFPDelSGCLTableEntry(ep_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_DelSGCLTableEntry(&cdbrHandle, entryID);
}

status_t EP_RxPSFPGetSGCLState(ep_handle_t *handle, uint32_t entryID, netc_tb_sgcl_sgclse_t *state, uint8_t *buffer)
{
    assert((handle != NULL) && (buffer != NULL) && (state != NULL));
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = (netc_tb_data_buffer_t *)buffer;
    return NETC_GetSGCLState(&cdbrHandle, entryID, state);
}

status_t EP_RxPSFPAddRPTableEntry(ep_handle_t *handle, netc_tb_rp_config_t *config)
{
    assert((handle != NULL) && (config != NULL));
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_AddRPTableEntry(&cdbrHandle, config);
}

status_t EP_RxPSFPDelRPTableEntry(ep_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_DelRPTableEntry(&cdbrHandle, entryID);
}

status_t EP_RxPSFPGetRPStatistic(ep_handle_t *handle, uint32_t entryID, netc_tb_rp_stse_t *statis)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_GetRPStatistic(&cdbrHandle, entryID, statis);
}

status_t EP_MSIXSetVector(ep_handle_t *handle, ep_msix_vector_t *vector)
{
    uint8_t i;

    for (i = 0; i < vector->txRingNum; i++)
    {
        handle->hw.si->SIMSITRVR[i] = vector->txRingVectorArray[i];
    }

    for (i = 0; i < vector->rxRingNum; i++)
    {
        handle->hw.si->SIMSIRRVR[i] = vector->rxRingVectorArray[i];
    }

    handle->hw.si->SICMSIVR = vector->cmdVector & ENETC_SI_SICMSIVR_VECTOR_MASK;
    handle->hw.si->SIMSIVR  = vector->siComVector & ENETC_SI_SIMSIVR_VECTOR_MASK;

    return kStatus_Success;
}

status_t NETC_MsixSetEntryMask(ep_handle_t *handle, uint8_t entryIdx, bool mask)
{
    if (entryIdx >= handle->cfg->entryNum)
    {
        return kStatus_Fail;
    }
    handle->hw.msixTable[entryIdx].control = (uint32_t)(mask);

    return kStatus_Success;
}

status_t NETC_MsixGetPendingStatus(ep_handle_t *handle, uint32_t *status)
{
    *status = *(uint32_t *)((uint32_t)handle->hw.msixTable + NETC_MSIX_TABLE_PBA_OFFSET);

    return kStatus_Success;
}

status_t NETC_MsixCleanPendingStatus(ep_handle_t *handle, uint32_t status)
{
    *(uint32_t *)((uint32_t)handle->hw.msixTable + NETC_MSIX_TABLE_PBA_OFFSET) = status;

    return kStatus_Success;
}

status_t EP_RxL2MFInit(ep_handle_t *handle, netc_si_l2mf_config_t *config)
{
    uint8_t siNum = getSiNum(handle->cfg->si);

    NETC_EnetcEnablePromiscuous(handle->hw.base, siNum, config->macUCPromis, config->macUCPromis);
    NETC_SIEnablePromisc(handle->hw.si, kNETC_PacketUnicast, config->rejectUC);
    NETC_SIEnablePromisc(handle->hw.si, kNETC_PacketMulticast, config->rejectMC);
    NETC_SIEnablePromisc(handle->hw.si, kNETC_PacketBroadcast, config->rejectBC);

    return kStatus_Success;
}

status_t EP_RxL2MFAddHashEntry(ep_handle_t *handle, netc_packet_type_t type, uint8_t *addr)
{
    assert(type != kNETC_PacketBroadcast);

    uint8_t siNum     = getSiNum(handle->cfg->si);
    uint8_t hashIndex = 0;
    uint64_t macAddr  = 0;
    int i             = 0;

    /* Calculate the hash index. */
    for (i = 5; i >= 0; i--)
    {
        macAddr = (macAddr << 8U) | addr[i];
    }
    for (i = 0; i < 8; i++)
    {
        hashIndex ^= (macAddr & 0x3F);
        macAddr = macAddr >> 6U;
    }

    NETC_EnetcAddMacAddrHash(handle->hw.base, siNum, type, hashIndex);

    /* Added the used count of hash index. */
    if (type == kNETC_PacketUnicast)
    {
        handle->unicastHashCount[hashIndex]++;
    }
    else
    {
        handle->multicastHashCount[hashIndex]++;
    }

    return kStatus_Success;
}

status_t EP_RxL2MFDelHashEntry(ep_handle_t *handle, netc_packet_type_t type, uint8_t *addr)
{
    assert(type != kNETC_PacketBroadcast);

    uint8_t siNum     = getSiNum(handle->cfg->si);
    uint8_t hashIndex = 0;
    uint64_t macAddr  = 0;
    int i             = 0;

    /* Calculate the hash index. */
    for (i = 5; i >= 0; i--)
    {
        macAddr = (macAddr << 8U) | addr[i];
    }
    for (i = 0; i < 8; i++)
    {
        hashIndex ^= (macAddr & 0x3F);
        macAddr = macAddr >> 6U;
    }

    if (type == kNETC_PacketUnicast)
    {
        if (handle->unicastHashCount[hashIndex] == 0U)
        {
            return kStatus_Fail;
        }
        else
        {
            /* Delete the hash index when there's no address using this index. */
            if (--handle->unicastHashCount[hashIndex] == 0U)
            {
                NETC_EnetcDelMacAddrHash(handle->hw.base, siNum, type, hashIndex);
            }
        }
    }
    else
    {
        if (handle->multicastHashCount[hashIndex] == 0U)
        {
            return kStatus_Fail;
        }
        else
        {
            /* Delete the hash index when there's no other MAC address using this hash index. */
            if (--handle->multicastHashCount[hashIndex] == 0U)
            {
                NETC_EnetcDelMacAddrHash(handle->hw.base, siNum, type, hashIndex);
            }
        }
    }

    return kStatus_Success;
}

status_t EP_RxL2MFAddEMTableEntry(ep_handle_t *handle, uint32_t idx, uint8_t *macAddr)
{
    assert((handle != NULL) && ((macAddr != NULL)));
    uint64_t address    = ((uint64_t)macAddr[4] << 32U) + ((uint64_t)macAddr[5] << 40U) + *(uint32_t *)&macAddr[0];
    netc_cmd_bd_t cmdBd = {0};

    cmdBd.generic.addr     = address;
    cmdBd.generic.en       = true;
    cmdBd.generic.siBitMap = 1U << getSiNum(handle->cfg->si);
    cmdBd.generic.index    = idx;
    cmdBd.generic.cmd      = 0;
    cmdBd.generic.class    = 1;
    cmdBd.generic.ci       = 0;
    cmdBd.generic.sf       = 1;
    return NETC_CmdBDSendCommand((netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR, &handle->cmdBdRing, &cmdBd,
                                 kNETC_NtmpV1_0);
}

status_t EP_RxL2MFDelEMTableEntry(ep_handle_t *handle, uint32_t idx)
{
    assert(handle != NULL);
    netc_cmd_bd_t cmdBd = {0};

    cmdBd.generic.addr     = 0;
    cmdBd.generic.en       = true;
    cmdBd.generic.siBitMap = 0;
    cmdBd.generic.index    = idx;
    cmdBd.generic.cmd      = 0;
    cmdBd.generic.class    = 1;
    cmdBd.generic.ci       = 0;
    cmdBd.generic.sf       = 1;
    return NETC_CmdBDSendCommand((netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR, &handle->cmdBdRing, &cmdBd,
                                 kNETC_NtmpV1_0);
}

status_t EP_RxL2VFInit(ep_handle_t *handle, netc_si_l2vf_config_t *config)
{
    NETC_EnetcConfigureVlanFilter(handle->hw.base, getSiNum(handle->cfg->si), config);
    return kStatus_Success;
}

status_t EP_RxL2VFAddHashEntry(ep_handle_t *handle, uint16_t vlanId)
{
    assert(handle != NULL);
    assert(vlanId <= 0xFFF);

    uint8_t siNum     = getSiNum(handle->cfg->si);
    uint8_t hash0     = (vlanId >> 6U) & 0x3FU;
    uint8_t hash1     = vlanId & 0x3FU;
    uint8_t hashIndex = hash0 ^ hash1;

    NETC_EnetcAddVlanHash(handle->hw.base, siNum, hashIndex);

    /* Added the used count of hash index. */
    handle->vlanHashCount[hashIndex]++;

    return kStatus_Success;
}

status_t EP_RxL2VFDelHashEntry(ep_handle_t *handle, uint16_t vlanId)
{
    assert(handle != NULL);
    assert(vlanId <= 0xFFF);

    uint8_t siNum     = getSiNum(handle->cfg->si);
    uint8_t hash0     = (vlanId >> 6U) & 0x3FU;
    uint8_t hash1     = vlanId & 0x3FU;
    uint8_t hashIndex = hash0 ^ hash1;

    if (handle->vlanHashCount[hashIndex] == 0U)
    {
        return kStatus_Fail;
    }
    else
    {
        /* Delete the hash index when there's no other VLAN ID using this hash index. */
        if (--handle->vlanHashCount[hashIndex] == 0U)
        {
            NETC_EnetcDelVlanHash(handle->hw.base, siNum, hashIndex);
        }
    }

    return kStatus_Success;
}

status_t EP_RxL2VFAddEMTableEntry(ep_handle_t *handle, uint32_t idx, uint16_t vlanId, netc_vlan_tpid_select_t tpid)
{
    assert(handle != NULL);
    netc_cmd_bd_t cmdBd = {0};

    cmdBd.generic.addr     = vlanId | (tpid << 16U);
    cmdBd.generic.en       = true;
    cmdBd.generic.siBitMap = 1U << getSiNum(handle->cfg->si);
    cmdBd.generic.index    = idx;
    cmdBd.generic.cmd      = 0;
    cmdBd.generic.class    = 2;
    cmdBd.generic.ci       = 0;
    cmdBd.generic.sf       = 1;
    return NETC_CmdBDSendCommand((netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR, &handle->cmdBdRing, &cmdBd,
                                 kNETC_NtmpV1_0);
}

status_t EP_RxL2VFDelEMTableEntry(ep_handle_t *handle, uint32_t idx)
{
    assert(handle != NULL);
    netc_cmd_bd_t cmdBd = {0};

    cmdBd.generic.addr     = 0;
    cmdBd.generic.en       = true;
    cmdBd.generic.siBitMap = 0;
    cmdBd.generic.index    = idx;
    cmdBd.generic.cmd      = 0;
    cmdBd.generic.class    = 2;
    cmdBd.generic.ci       = 0;
    cmdBd.generic.sf       = 1;
    return NETC_CmdBDSendCommand((netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR, &handle->cmdBdRing, &cmdBd,
                                 kNETC_NtmpV1_0);
}

status_t EP_TxTGSConfigAdminGcl(ep_handle_t *handle, netc_tb_tgs_gcl_t *config)
{
    assert((handle != NULL) && (config != NULL));
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr = &handle->cmdBdRing;
    return NETC_ConfigTGSAdminList(&cdbrHandle, config);
}

status_t EP_TxtTGSGetOperGcl(ep_handle_t *handle, netc_tb_tgs_gcl_t *gcl, uint32_t length)
{
    assert((handle != NULL) && (gcl != NULL));
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr = &handle->cmdBdRing;
    return NETC_GetTGSOperationList(&cdbrHandle, gcl, length);
}

status_t EP_TxTrafficClassConfig(ep_handle_t *handle, netc_hw_tc_idx_t tcIdx, const netc_tx_tc_config_t *config)
{
    assert((handle != NULL) && (config != NULL));
    status_t reseult;

    if (!NETC_PortIsPseudo(handle->hw.portGroup.port))
    {
        uint32_t temp                    = handle->hw.portGroup.port->PFPCR & (~(1U << (uint8_t)tcIdx));
        handle->hw.portGroup.port->PFPCR = temp | ((uint32_t)config->enPreemption << (uint8_t)tcIdx);
    }

    reseult = ENETC_PortConfigTXMaxSDU(handle->hw.portGroup.port, tcIdx, &config->sduCfg);
    if ((kStatus_Success == reseult) && (config->enableCbs))
    {
        reseult = ENETC_PortConfigCBS(handle->hw.portGroup.port, tcIdx, &config->cbsCfg);
    }
    return reseult;
}

status_t EP_RxPSFPAddISITableEntry(ep_handle_t *handle, netc_tb_isi_config_t *config, uint32_t *entryID)
{
    assert((handle != NULL) && (config != NULL) && (entryID != NULL));
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_AddISITableEntry(&cdbrHandle, config, entryID);
}

status_t EP_RxPSFPDelISITableEntry(ep_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_DelISITableEntry(&cdbrHandle, entryID);
}

status_t EP_RxPSFPAddISTableEntry(ep_handle_t *handle, netc_tb_is_config_t *config)
{
    assert((handle != NULL) && (config != NULL));
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_AddISTableEntry(&cdbrHandle, config);
}

status_t EP_RxPSFPDelISTableEntry(ep_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_DelISTableEntry(&cdbrHandle, entryID);
}

status_t EP_RxPSFPAddISFTableEntry(ep_handle_t *handle, netc_tb_isf_config_t *config, uint32_t *entryID)
{
    assert((handle != NULL) && (config != NULL) && (entryID != NULL));
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_AddISFTableEntry(&cdbrHandle, config, entryID);
}

status_t EP_RxPSFPDelISFTableEntry(ep_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_DelISFTableEntry(&cdbrHandle, entryID);
}

status_t EP_RxPSFPAddISCTableEntry(ep_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_AddISCTableEntry(&cdbrHandle, entryID);
}

status_t EP_RxPSFPGetISCStatistic(ep_handle_t *handle, uint32_t entryID, netc_tb_isc_stse_t *statistic)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_GetISCStatistic(&cdbrHandle, entryID, statistic);
}

status_t EP_RxPSFPResetISCStatistic(ep_handle_t *handle, uint32_t entryID)
{
    assert(handle != NULL);
    netc_cbdr_handle_t cdbrHandle;

    cdbrHandle.base   = (netc_cbdr_hw_t *)&handle->hw.si->SICBDRMR;
    cdbrHandle.cmdr   = &handle->cmdBdRing;
    cdbrHandle.buffer = &s_cmdData;
    return NETC_ResetISCStatistic(&cdbrHandle, entryID);
}
