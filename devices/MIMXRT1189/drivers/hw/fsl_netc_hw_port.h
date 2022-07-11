/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_NETC_PORT_H_
#define _FSL_NETC_PORT_H_

#include "fsl_netc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_hw_port_tx
#endif
/*! @addtogroup netc_hw_port_tx
 * @{
 */

/*!
 * @brief Port steam gate configuration structure
 *
 */
typedef struct _port_stream_gate
{
    void *reserved;
} port_stream_gate_t;

/*! @} */ // end of netc_hw_port_tx
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_hw_port_tx
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_hw_port_rx
#endif
/*! @addtogroup netc_hw_port_rx
 * @{
 */

/*!
 * @brief PORT TSN PSFP configuration
 *
 */
typedef enum _port_psfp_config
{
    kNETC_Reserve,
} port_psfp_config_t;

/*! @} */ // end of netc_hw_port_rx
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_hw_port_rx
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_hw_port_tx
#endif
/*! @addtogroup netc_hw_port_tx
 * @{
 */

/*!
 * @brief Configure the port time gating Scheduling
 *
 * @param base
 * @param config
 * @return status_t
 */
static inline status_t ENETC_PortConfigTGS(NETC_PORT_Type *base, const netc_port_tg_config_t *config)
{
    base->PTGSCR   = NETC_PORT_PTGSCR_TGE(config->enableTg);
    base->PTGSATOR = NETC_PORT_PTGSATOR_ADV_TIME_OFFSET(config->advOffset);
    if ((base->PCAPR & NETC_PORT_PCAPR_LINK_TYPE_MASK) == 0U)
    {
        base->PTGSHCR = NETC_PORT_PTGSHCR_HOLD_SKEW(config->holdSkew);
    }
    return kStatus_Success;
}

/*!
 * @brief Config the Credit-Based Shaper (CBS) for specified Port
 *
 * @param base
 * @param tcIdx
 * @param config
 * @return status_t
 */
status_t ENETC_PortConfigCBS(NETC_PORT_Type *base, netc_hw_tc_idx_t tcIdx, const port_cbs_config_t *config);

/*!
 * @brief Config the max Transmit max SDKU
 *
 * @param base
 * @param tcIdx
 * @param config
 * @return status_t
 */
static inline status_t ENETC_PortConfigTXMaxSDU(NETC_PORT_Type *base,
                                                netc_hw_tc_idx_t tcIdx,
                                                const port_sdu_config_t *config)
{
    if (config->maxSduSized > 0x07D0U)
    {
        /* This limit must not exceed 0x07D0 (2000 bytes) */
        return kStatus_OutOfRange;
    }
    else
    {
        base->TCT_NUM[tcIdx].PTCTMSDUR = NETC_PORT_PTCTMSDUR_SF_MAXSDU_DIS(config->enTxMaxSduCheck) |
                                         NETC_PORT_PTCTMSDUR_SDU_TYPE(config->sduType) |
                                         NETC_PORT_PTCTMSDUR_MAXSDU(config->maxSduSized);
        return kStatus_Success;
    }
}

/*!
 * @brief Enable / Disable Time Specific Departure (TSD) feature
 *
 * It can't work with QBV CBS
 *
 * @param port
 * @param isEnable
 * @return status_t
 */
status_t NETC_PortEnableTSD(netc_hw_port_idx_t port, bool isEnable);

/*!
 * @brief Config the ETM Class table. Only applicable for the SWITCH port
 *
 * @return status_t
 */
status_t NETC_PortConfigETMTable();

/*!
 * @brief Configure the ETM class scheduler table. Only applicable for the SWITCH Port.
 *
 * @return status_t
 */
status_t NETC_PortConfigETMSchedulerTable();

/*!
 * @brief Configure the ETM conggestion group table. Only applicable for the switch port.
 *
 * @return status_t
 */
status_t NETC_PortConfigETMCongestionGroupTable();

/*!
 * @brief Configure the Frame Modification table
 *
 * TODO It shall be a seperate group
 *
 * @return status_t
 */
status_t NETC_SwitchConfigFrameModificationTable();

/*!
 * @brief Configure the Frame Modification Data table
 *
 * TODO It shall be a seperate group
 *
 * @return status_t
 */
status_t NETC_SwitchConfigFrameModificationDataTable();

/*!
 * @brief Configure the Egreess Treatment Table for specified switch port.
 *
 * @return status_t
 */
status_t NETC_PortConfigEgressTreatmentTable();

/*! @} */ // end of netc_hw_port_tx
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_hw_port_tx
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_hw_port_rx
#endif
/*! @addtogroup netc_hw_port_rx
 * @{
 */
/*!
 * @brief Configure the PSFP For the port
 *
 * @param port
 * @param psConfig
 * @return status_t
 */
status_t NETC_PortConfigPSFP(netc_hw_port_idx_t port, port_psfp_config_t *psConfig);

/*!
 * @brief Config stream gate table
 *
 * @param port
 * @param psConfig
 * @return status_t
 */
status_t NETC_PortConfigStreamGateInstanceTable(netc_hw_port_idx_t port, port_stream_gate_t *psConfig);

/*!
 * @brief Config the stream gate control list table
 *
 * @return status_t
 */
status_t NETC_PortConfigStreamGateControlListTable(netc_hw_port_idx_t port);

/*!
 * @brief Configure Stream table
 *
 * @return status_t
 */
status_t NETC_PortConfigStreamTable(netc_hw_port_idx_t port);

/*!
 * @brief Config Port Filter table
 *
 * @return status_t
 */
status_t NETC_PortConfigPortFilterTable(netc_hw_port_idx_t port);

/*!
 * @brief Configure the Ingress stream filter table
 *
 * @return status_t
 */
status_t NETC_PortConfigStreamFilterTable(netc_hw_port_idx_t port);

/*!
 * @brief Configure the stream identification table
 *
 * @return status_t
 */
status_t NETC_PortConfigStreamIdentificationTable(netc_hw_port_idx_t port);

/*!
 * @brief Get the stream counter information for specified stream
 *
 * @return status_t
 */
status_t NETC_PortGetStreamCountStatistics(netc_hw_port_idx_t port);

/*!
 * @brief Configure MAC filter table
 *
 * @return status_t
 */
status_t NETC_PortConfigMacFilterTable(netc_hw_port_idx_t port);

/*!
 * @brief Configure the VLAN Address Filter Table
 *
 * @return status_t
 */
status_t NETC_PortConfigVlanAddressFilterTable(netc_hw_port_idx_t port);

/*! @} */ // end of netc_ep_tc
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_hw_port_rx
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_hw_port_mac
#endif
/*! @addtogroup netc_hw_port_mac
 * @{
 */

/*!
 * @brief Enable/Disable Loopback for specified MAC
 *
 * @param base
 * @param enable
 * @return status_t
 */
status_t NETC_PortEnableLoopback(NETC_ETH_LINK_Type *base, bool enable);

/*!
 * @brief Enable/Disable MAC for specified Port
 *
 * @param base
 * @param enable
 * @return status_t
 */
status_t NETC_PortEnableMac(NETC_PORT_Type *base, bool enable);

/*!
 * @brief Configure specified PORT
 *
 * @param base   NETC port module base address.
 * @param config Port configuration structure.
 * @return status_t
 */
status_t NETC_PortConfig(NETC_PORT_Type *base, const netc_port_common_t *config);

/*!
 * @brief Configure ethernet MAC for specified PORT.
 * Set the MII mode and MAC address.
 *
 * @param base Ethernet MAC port peripheral base address.
 * @param config The port configuration.
 * @return status_t
 */
status_t NETC_PortConfigEthMac(NETC_ETH_LINK_Type *base, const netc_port_ethmac_t *config);

/*!
 * @brief Set the MAC address.
 *
 * @param handle
 * @param macAddr
 */
void NETC_PortSetMacAddr(NETC_PORT_Type *base, const uint8_t *macAddr);

/*!
 * @brief Check whether this port a pseudo MAC port.
 *
 * @param base PORT peripheral base address.
 */
bool NETC_PortIsPseudo(NETC_PORT_Type *base);

/*!
 * @brief Set port Parser.
 *
 * @param base  PORT peripheral base address.
 * @param config  The port ingress filter configuration.
 */
static inline void NETC_PortSetParser(NETC_PORT_Type *base, const netc_port_parser_config_t *config)
{
    base->PPCR = NETC_PORT_PPCR_L4PFS(config->l4PayloadCount) | NETC_PORT_PPCR_L4HFP(config->enableL4Parser) |
                 NETC_PORT_PPCR_L3PFS(config->l3PayloadCount) | NETC_PORT_PPCR_L3HFP(config->enableL3Parser) |
                 NETC_PORT_PPCR_L2PFS(config->l2PloadCount);
}

/*!
 * @brief Set port acceptable VLAN.
 *
 * @param base  PORT peripheral base address.
 * @param config  The port acceptable vlan classification configuration.
 */
static inline void NETC_PortSetVlanClassify(NETC_PORT_Type *base, const netc_port_vlan_classify_config_t *config)
{
    base->PTAR = NETC_PORT_PTAR_OVTPIDL(config->outerMask) | NETC_PORT_PTAR_IVTPIDL(config->innerMask);
}

/*!
 * @brief Set port Qos Classification.
 *
 * @param base  PORT peripheral base address.
 * @param config  The port ingress filter configuration.
 */
static inline void NETC_PortSetQosC(NETC_PORT_Type *base, const netc_port_qos_classify_config_t *config)
{
    base->PQOSMR = NETC_PORT_PQOSMR_QVMP(config->qosVlanMap) | NETC_PORT_PQOSMR_VQMP(config->vlanQosMap) |
                   NETC_PORT_PQOSMR_DIPV(config->defaultIpv) | NETC_PORT_PQOSMR_DDR(config->defaultDr) |
                   NETC_PORT_PQOSMR_VE(config->enVlanInfo) | NETC_PORT_PQOSMR_VS(config->vlanTagSelect);
}

/*!
 * @brief Set port ingress filter.
 *
 * @param base  PORT peripheral base address.
 * @param config  The port ingress filter configuration.
 */
static inline void NETC_PortSetIPF(NETC_PORT_Type *base, const netc_ipf_port_config_t *config)
{
    base->PCR    = NETC_PORT_PCR_L2DOSE(config->enL2Dos);
    base->PIPFCR = NETC_PORT_PIPFCR_EN(config->enIPFTable);
}

/*!
 * @brief Set port Ingress stream identification.
 *
 * @param base  PORT peripheral base address.
 * @param config  The port Ingress stream identification configuration.
 */
static inline void NETC_PortSetISI(NETC_PORT_Type *base, const netc_psfp_isi_port_config_t *config)
{
    /* Configure ingress stream identification */
    base->PISIDCR = NETC_PORT_PISIDCR_ISEID(config->defaultISEID) | NETC_PORT_PISIDCR_KC1EN(config->enKC1) |
                    NETC_PORT_PISIDCR_KC0EN(config->enKC0) | NETC_PORT_PISIDCR_KCPAIR(config->kcPair);
}

/*!
 * @brief Get phy MAC preemption status.
 *
 * @param base  PORT MAC peripheral base address.
 * @param status Point to the buffer which store status.
 */
static inline void NETC_PortGetPhyMacPreemptionStatus(NETC_ETH_LINK_Type *base,
                                                      netc_port_phy_mac_preemption_status_t *status)
{
    status->verifyStatus = (netc_port_preemption_verify_status_t)(
        (base->MAC_MERGE_MMCSR & NETC_ETH_LINK_MAC_MERGE_MMCSR_VSTS_MASK) >> NETC_ETH_LINK_MAC_MERGE_MMCSR_VSTS_SHIFT);
    status->mergeActive = (base->MAC_MERGE_MMCSR & NETC_ETH_LINK_MAC_MERGE_MMCSR_TXSTS_MASK) ? true : false;
}

void NETC_PortGetDiscardStatistic(NETC_PORT_Type *base,
                                  netc_port_discard_tpye_t discardType,
                                  netc_port_discard_statistic_t *statistic);

void NETC_PortGetPhyMacTxStatistic(NETC_ETH_LINK_Type *base,
                                   netc_port_phy_mac_tpye_t macType,
                                   netc_port_phy_mac_traffic_statistic_t *statistic);

void NETC_PortGetPhyMacRxStatistic(NETC_ETH_LINK_Type *base,
                                   netc_port_phy_mac_tpye_t macType,
                                   netc_port_phy_mac_traffic_statistic_t *statistic);

void NETC_PortGetPhyMacDiscardStatistic(NETC_ETH_LINK_Type *base,
                                        netc_port_phy_mac_tpye_t macType,
                                        netc_port_phy_mac_discard_statistic_t *statistic);

void NETC_PortGetPhyMacPreemptionStatistic(NETC_ETH_LINK_Type *base,
                                           netc_port_phy_mac_preemption_statistic_t *statistic);

void NETC_PortGetPseudoMacTrafficStatistic(NETC_PSEUDO_LINK_Type *base,
                                           bool getTx,
                                           netc_port_pseudo_mac_traffic_statistic_t *statistic);

/*! @} */ // end of netc_hw_port_mac
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_hw_port_mac
#endif

#if defined(__cplusplus)
}
#endif
#endif /* _FSL_NETC_PORT_H_ */
