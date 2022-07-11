/*
 * Copyright 2021-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_EP_H_
#define _FSL_EP_H_

#include "fsl_netc.h"
#include "hw/fsl_netc_hw.h"
#include "hw/fsl_netc_hw_enetc.h"
#include "hw/fsl_netc_hw_port.h"
#include "hw/fsl_netc_hw_si.h"

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region api_ep
#endif
//////////////////////////////////////
// Group for the End Point Driver
//////////////////////////////////////
/*!
 * @defgroup netc_ep NETC Endpoint (EP) Driver
 * @details API / Data structure for using NETC driver and endpoint.
 * This group handles endpoint devices covering the features to Initialize/De-Initialize the endpoint, transfer
 * packets, configure the port/links, and get the status and statistic. Note that it doesn't cover the phy configuration
 * which shall be configured in MDIO module for both external real PHY and internal virtual PHY.
 *
 * The endpoint API mainly relates to the ENETC IP which interfaces the host to the MAC or Pseudo-MAC (Switch).
 *
 * APIs in this group take the ep_handle_t as first parameter to retrieve the current context.
 * User shall not manipulate the members of the handle without invoking the provided APIs or help macros as it may
 * break the driver state.
 *
 * All API in this groups start with `EP_` and data structure specific to endpoint start with `ep_`.
 *
 * @ingroup netc_api
 */

/*!
 * @defgroup netc_ep_config Endpoint (EP) Generic Configuration
 * @details Generic Ethernet endpoint configuration
 * MAC, host interface and driver configuration.
 * @ingroup netc_ep
 */
/*!
 * @defgroup netc_ep_stat Endpoint (EP) Statistic Module
 * @details Statistics counters
 * @ingroup netc_ep
 */
/*!
 * @defgroup netc_ep_interrupt Endpoint (EP) Interrupt Module
 * @details Interrupt related functionalities
 *
 * @ingroup netc_ep
 */
/*!
 * @defgroup netc_ep_xfer Endpoint (EP) Transmit/Receive
 * @details Data interface for transmit and receive
 *
 * @ingroup netc_ep
 */
/*!
 * @defgroup netc_ep_datapath Endpoint (EP) data path
 * @details QoS, classification and filtering functionalities
 * @ingroup netc_ep
 */
/*!
 * @defgroup netc_ep_tx Endpoint (EP) Egress data path configuration
 * @details API/Data structure for configuration of the endpoint ingress datapath
 * This module includes steps below in sequence and APIs are placed following this sequence for user to identify easily
 * - Schedule: Credit Based Shaper also referred as IEEE 802.1 Qav
 * - Schedule: Time Specific Departure
 * - Schedule: Time Gate Scheduling referred as IEEE 802.1 Qbv
 * - VLAN Insert
 *
 * API in this group starts with `EP_Tx`.
 *
 * @ingroup netc_ep_datapath
 */
/*!
 * @defgroup netc_ep_rx Endpoint (EP) Ingress data path configuration
 * @details API/Data structure for configuration of the endpoint ingress datapath.
 * This module includes steps below in sequence and APIs are placed following this sequence for user to identify easily
 * - Parser
 * - VLAN Classification
 * - Ingress QoS Classification
 * - Ingress Port Filtering
 * - Stream Identification and Filtering
 * - L2 Filtering: VLAN/MAC Address
 * - Policying: Rate Policing and Stream Gating
 * - Enqueue (Ingress Congestion Management ICM)
 * - HTA (Host Transfer Agent Rx)
 *
 * API in this group starts with `EP_Rx`.
 *
 * @ingroup netc_ep_datapath
 */

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion api_ep
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_ep_stat
#endif
/*! @addtogroup netc_ep_stat
 * @{
 */

/*! @brief Status/interrupt detect flags merged to same set of enum. TODO SITMRIDR */
typedef enum _ep_flags
{
    kNETC_EPTimerSyncedFlag     = 0x1,
    kNETC_EPICMBlockedFlag      = 0x1 << 1,
    kNETC_EPWakeOnLANActiveFlag = 0x1 << 2,

} ep_flags_t;

/*! @} */ // end of netc_ep_stat
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_ep_stat
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_ep_interrupt
#endif
/*! @addtogroup netc_ep_interrupt
 * @{
 */

/*!
 * @brief MSIX EP vector structure
 */
typedef struct _ep_msix_vector
{
    uint8_t *txRingVectorArray; /*!< Tx ring interrupt specified entry index. */
    uint8_t *rxRingVectorArray; /*!< Rx ring interrupt specified entry index. */
    uint8_t txRingNum;          /*!< Tx ring number. */
    uint8_t rxRingNum;          /*!< Rx ring number. */
    uint8_t cmdVector;          /*!< Command ring interrupt specified entry index. */
    uint8_t siComVector;        /*!< PSI-VSI communication interrupt specified entry index. */
} ep_msix_vector_t;

/*!
 * @brief Interrupt enable/disable flags.
 *
 * The value of the enumerator is not necessary match the bit in register. All interrupts in Endpoint are
 * merged into this enum except the BDR specific interrupt.
 * TODO SITMRIER
 */
typedef enum _ep_interrupt_flag
{
    kNETC_EPPSIResetInterruptEnable = 0x1,
    kNETC_EPPSIMsgRxInterruptEnable = 0x1 << 1U,
} ep_interrupt_flag_t;

/*! @} */ // end of netc_ep_interrupt
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_ep_interrupt
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_ep_xfer
#endif
/*! @addtogroup netc_ep_xfer
 * @{
 */

typedef enum _ep_rx_flags
{
    kEP_RX_RSS_VALID       = 0x1,       /*!< Request timestamp */
    kEP_RX_VLAN_VALID      = 0x1 << 1U, /*!< Specifiy frame departure time */
    kEP_RX_TIMESTAMP_VALID = 0x1 << 2U, /*!< Enable port masquerading */
} ep_rx_flags_t;

typedef enum _ep_tx_ext_flags
{
    kEP_TX_EXT_VLAN_INSERT = 0x1,       /*!< Enable VLAN insert */
    kEP_TX_EXT_TWO_STEP_TS = 0x1 << 2U, /*!< Enable two-step timestamp offload */
} ep_tx_ext_flags_t;

typedef enum _ep_tx_opt_flags
{
    kEP_TX_OPT_REQ_TS      = 0x1,       /*!< Request timestamp */
    kEP_TX_OPT_VLAN_INSERT = 0x1 << 1U, /*!< Enable VLAN insert */
    kEP_TX_OPT_START_TIME  = 0x1 << 2U, /*!< Specifiy frame departure time */
    kEP_TX_OPT_MASQUERADE  = 0x1 << 3U, /*!< Enable port masquerading, only for pseudo MAC */
} ep_tx_opt_flags;

typedef struct _ep_tx_opt
{
    uint32_t flags;             /*!< A bitmask of ep_tx_opt_flags */
    uint32_t timestamp;         /*!< Departure timestamp, used if kEP_TX_OPT_START_TIME is set */
    netc_hw_port_idx_t swtPort; /*!< Switch port index, used if kEP_TX_OPT_MASQUERADE is set */
    netc_vlan_t vlan;           /*!< VLAN insert, used if kEP_TX_OPT_VLAN_INSERT is set */
} ep_tx_opt;

/*! @} */ // end of netc_ep_xfer
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_ep_xfer
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_ep_datapath
#endif
/*! @addtogroup netc_ep_datapath
 * @{
 */
/*! @brief Port Ingress Filter config */
typedef struct _netc_ep_ipf_config
{
    bool l2DiscardMCSmac;              /*!< DOSL2CR */
    bool l2DiscardSmacEquDmac;         /*!< DOSL2CR */
    netc_ipf_port_config_t portConfig; /*!< Configuration for port connected to enetc peripheral. */
} netc_ep_ipf_config_t;

/*! @brief PSFP config */
typedef struct _netc_ep_psfp_config
{
    netc_isi_kc_rule_t kcRule[2];
    netc_psfp_isi_port_config_t isiPortConfig;
} netc_ep_psfp_config_t;

/*! @} */ // end of netc_ep_datapath
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_ep_datapath
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_ep_config
#endif
/*! @addtogroup netc_ep_config
 * @{
 */

typedef struct _ep_handle ep_handle_t;

/*! @brief Callback for endpoint generated async events */
typedef status_t (*ep_event_cb_t)(ep_handle_t *handle, netc_ep_event_t event, void *data);

/*! @brief Callback for reclaimed tx frames */
typedef status_t (*ep_reclaim_cb_t)(ep_handle_t *handle, netc_tx_frame_info_t *frameInfo);

/*! @brief Configuration for the endpoint handle */
typedef struct _ep_config
{
    netc_hw_si_idx_t si;                /*!< Station interface index. */
    netc_hw_enetc_si_config_t siConfig; /*!< Station interface configuration. */
    uint8_t txPrioToTC[8]; /*!< Tx BD ring priority to Tx traffic class queue index mapping, range in TC0 ~ TC7 */
    netc_tx_tc_config_t txTcCfg[8]; /*!< Tx traffic class related configuration, vaild only on ENETC 0 */
    netc_ep_psfp_config_t psfpCfg; /*!< PSFP configuration,cover the ISI key construction profile and port ngress stream
                                      identification configuration */
    netc_port_t port;              /*!< Port configuration. */
    netc_msix_entry_t *msixEntry;  /*!< MSIX table entry array. */
    uint8_t entryNum;              /*!< MSIX entry number. */
    ep_event_cb_t event_cb;        /*!< Callback for the endpoint generated events. */
    ep_reclaim_cb_t reclaim_cb;    /*!< Callback for reclaimed Tx frames. */
    bool rxCacheMaintain;          /*!< Tx buffer cache maintain. */
    bool txCacheMaintain;          /*!< Rx buffer cache maintain. */
    bool rxZeroCopy;               /*!< Enable zero-copy receive mode */
    netc_rx_alloc_cb_t rxBuffAlloc;     /*!< Callback function to alloc memory, must be provided for zero-copy Rx. */
    netc_rx_free_cb_t rxBuffFree;       /*!< Callback function to free memory, must be provided for zero-copy Rx. */
    netc_cmd_bdr_config_t cmdBdrConfig; /*!< Command BD ring configuration. */
} ep_config_t;

/*! @brief handle for the endpoint
 * Private internal data.
 */
struct _ep_handle
{
    netc_enetc_hw_t hw;                           /*!< Hardware register map resource. */
    const ep_config_t *cfg;                       /*!< User configuration. */
    netc_rx_bdr_t rxBdRing[NETC_EP_RING_NUM_MAX]; /*!< Receive buffer ring. */
    netc_tx_bdr_t txBdRing[NETC_EP_RING_NUM_MAX]; /*!< Transmit buffer ring. */
    uint8_t *rxBufferArray;                       /*!< RX buffers array adress */
    uint32_t rxBufferSize;                        /*!< RX buffers size (if zero-copy is not enabled) */
    netc_cmd_bdr_t cmdBdRing;                     /*!< Command BD ring handle for endpoint */
    uint8_t unicastHashCount[64];                 /*!< Unicast hash index collisions counter. */
    uint8_t multicastHashCount[64];               /*!< Multicast hash index collisions counter. */
    uint8_t vlanHashCount[64];                    /*!< VLAN hash index collisions counter. */
};

/*! @} */ // end of netc_ep_config
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_ep_config
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_ep_config
#endif
/*! @addtogroup netc_ep_config
 * @{
 */

/*!
 * @brief Initialize the endpoint with specified station interface
 *
 * Each station interface needs to call this API. In the case of a virtual station interface
 * it's necessary that the physical station interface has been initialized beforehand.
 *
 * @param handle
 * @param macAddr Primary MAC address
 * @param config The user configuration
 * @param bdrConfig Array of buffer configurations (for each queue/ring)
 * @return status_t
 */
status_t EP_Init(ep_handle_t *handle,
                 uint8_t *macAddr,
                 const ep_config_t *config,
                 const netc_si_bdr_config_t *bdrConfig);

/*!
 * @brief De-initialize the endpoint.
 *
 * @param handle
 * @return status_t
 */
status_t EP_Deinit(ep_handle_t *handle);

/*!
 * @brief Get the default configuration.
 *
 * @param config
 * @return status_t
 */
status_t EP_GetDefaultConfig(ep_config_t *config);

/*!
 * @brief Enable MAC transmission/reception
 *
 * To be called when the PHY link is up.
 *
 * @param handle
 * @param speed
 * @param duplex
 * @return status_t
 */
status_t EP_Up(ep_handle_t handle, netc_hw_mii_speed_t speed, netc_hw_mii_duplex_t duplex);

/*!
 * @brief Disable MAC transmission/reception
 *
 * To be called when the PHY link is down.
 *
 * @param handle
 * @return status_t
 */
status_t EP_Down(ep_handle_t handle);

/*!
 * @brief Set the Primary MAC address.
 *
 * @param handle
 * @param macAddr
 * @return status_t
 */
status_t EP_SetPrimaryMacAddr(ep_handle_t *handle, uint8_t *macAddr);

/*!
 * @brief Configure SI based Vlan Insert/Removal
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t EP_ConfigVlanOffload(ep_handle_t *handle, netc_si_vlan_offload_config_t *config);

/*! @} */ // end of netc_ep_config
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_ep_config
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_ep_ntmp
#endif
/*! @addtogroup netc_ep_ntmp
 * @{
 */

/*!
 * @brief Initialize endpoint command BD ring
 *
 * @param handle
 * @param config The command BD ring configuration
 * @return status_t
 */
status_t EP_CmdBDRInit(ep_handle_t *handle, const netc_cmd_bdr_config_t *config);

/*!
 * @brief Deinit endpoint command BD ring
 *
 * @param handle
 * @return status_t
 */
status_t EP_CmdBDRDeinit(ep_handle_t *handle);

/*!
 * @brief Send command BD and wait execution finished.
 *
 * @param handle
 * @param cbd The command BD pointer
 * @param version The command BD version
 * @return status_t
 */
status_t EP_SendCmdBD(ep_handle_t *handle, netc_cmd_bd_t *cbd, netc_cbd_version_t version);

/*! @} */ // end of netc_ep_ntmp
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_ep_ntmp
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_ep_rx
#endif
/*! @addtogroup netc_ep_rx
 * @{
 */

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region Rx Parser
#endif
/*!
 * @brief Configure Parser in Receive Data Path
 *
 * @param handle
 * @param config
 * @return status_t
 */
static inline status_t EP_RxParserConfig(ep_handle_t *handle, netc_port_parser_config_t *config)
{
    NETC_PortSetParser(handle->hw.portGroup.port, config);
    return kStatus_Success;
}
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion Rx Parser
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region Vlan classification
#endif
/*!
 * @brief Configure the customer vlan type
 *
 * @param handle
 * @param config
 * @return status_t
 */
static inline status_t EP_RxVlanCInit(ep_handle_t *handle, const netc_vlan_classify_config_t *config)
{
    NETC_RxVlanCInit(handle->hw.common, config, false);
    return kStatus_Success;
}

/*!
 * @brief Configure the Accepted Vlan.
 * @param handle
 * @param mask
 * @return status_t
 */
static inline status_t EP_RxVlanCConfigPort(ep_handle_t *handle, netc_port_vlan_classify_config_t *config)
{
    NETC_PortSetVlanClassify(handle->hw.portGroup.port, config);
    return kStatus_Success;
}

/*!
 * @brief Configure the Native outer Vlan
 *
 * @param handle
 * @param useInZeroVlan
 * @param useInNoVlan
 * @param vlan
 * @return status_t
 */
status_t EP_RxVlanClsConfigNativeOuterVlan(ep_handle_t *handle, bool useInZeroVlan, bool useInNoVlan, netc_vlan_t vlan);

/*!
 * @brief Enable VLAN to IPV map with provided data or Disable Vlan to IPV map
 *
 * @param handle
 * @param enable
 * @param map See SICVLANR0/1
 * @return status_t
 */
status_t EP_RxVlanClsMapVlanToIPV(ep_handle_t *handle, bool enable, uint64_t map);

/*!
 * @brief Configure the Native inner Vlan
 *
 * @param handle
 * @param useInZeroVlan
 * @param useInNoVlan
 * @param vlan
 * @return status_t
 */
status_t EP_RxVlanCConfigNativeInnerVlan(ep_handle_t *handle, bool useInZeroVlan, bool useInNoVlan, netc_vlan_t vlan);

/*!
 * @brief Enable/Disable Outter as Inner Vlan if only one Vlan tag is found
 *
 * @param handle
 * @param enable
 * @return status_t
 */
status_t EP_RxVlanCEnableVlanOuterAsInner(ep_handle_t *handle, bool enable);
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion Vlan classification
#endif

static inline void EP_RxMapVlanToIpv(ep_handle_t *handle, netc_vlan_t vlan, uint8_t ipv)
{
    NETC_SIMapVlanToIpv(handle->hw.si, vlan, ipv);
}

static inline void EP_RxMapIpvToRing(ep_handle_t *handle, uint8_t ipv, uint8_t ring)
{
    NETC_SIMapIpvToRing(handle->hw.si, ipv, ring);
}

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region PSFP
#endif
/*!
 * @brief Enable / Disable Ingress Port Filtering
 *
 * Applied for both Switch and ENETC
 *
 * @param handle
 * @param config IPF general features
 * @return status_t
 */
status_t EP_RxIPFInit(ep_handle_t *handle, netc_ep_ipf_config_t *config);

/*!
 * @brief Add an entry for the ingress Port Filter Table.
 *
 * This function do an add & query with return hardware id which can be used as future query / delete / update.
 *
 * @param handle
 * @param config  IPF instance configuaration
 * @param entryID  The table entry ID read out
 * @return status_t
 */
status_t EP_RxIPFAddTableEntry(ep_handle_t *handle, netc_tb_ipf_config_t *config, uint32_t *entryID);

/*!
 * @brief Delete an entry for the ingress Port Filter Table.
 *
 * @param handle
 * @param entryID  The table entry ID
 * @return status_t
 */
status_t EP_RxIPFDelTableEntry(ep_handle_t *handle, uint32_t entryID);

/*!
 * @brief Reset the counter of an ingress port filter entry
 *
 * @param handle
 * @param entryID  The table entry ID
 * @return status_t
 */
status_t EP_RxIPFResetMatchCounter(ep_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get the matched count for entry in IPF
 *
 * @param handle
 * @param entryID  The table entry ID
 * @param count  A count of how many times this entry has been matched.
 * @return status_t
 */
status_t EP_RxIPFGetMatchedCount(ep_handle_t *handle, uint32_t entryID, uint64_t *count);

/*!
 * @brief Init the ENETC PSFP, inlcude
 *
 * @param handle
 * @param config
 * @return status_t
 */
static inline status_t EP_RxPSFPInit(ep_handle_t *handle, const netc_ep_psfp_config_t *config)
{
    NETC_PSFPKcProfileInit(handle->hw.common, &config->kcRule[0], false);
    NETC_PortSetISI(handle->hw.portGroup.port, &config->isiPortConfig);
    return kStatus_Success;
}

/*!
 * @brief Add an entry into the stream identification table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t EP_RxPSFPAddISITableEntry(ep_handle_t *handle, netc_tb_isi_config_t *config, uint32_t *entryID);

/*!
 * @brief Delete an entry in the stream identification table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t EP_RxPSFPDelISITableEntry(ep_handle_t *handle, uint32_t entryID);

/*!
 * @brief Add an entry into the ingress stream table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t EP_RxPSFPAddISTableEntry(ep_handle_t *handle, netc_tb_is_config_t *config);

/*!
 * @brief Delete an entry in the stream identification table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t EP_RxPSFPDelISTableEntry(ep_handle_t *handle, uint32_t entryID);

/*!
 * @brief Add an entry into the ingress stream filter table
 *
 * @param handle
 * @param config
 * @param entryID
 * @return uint32_t
 */
status_t EP_RxPSFPAddISFTableEntry(ep_handle_t *handle, netc_tb_isf_config_t *config, uint32_t *entryID);

/*!
 * @brief Del an entry into the stream filter table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t EP_RxPSFPDelISFTableEntry(ep_handle_t *handle, uint32_t entryID);

/*!
 * @brief Add entry to Rate Policying table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t EP_RxPSFPAddRPTableEntry(ep_handle_t *handle, netc_tb_rp_config_t *config);

/*!
 * @brief Delete entry in the Rate policying table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t EP_RxPSFPDelRPTableEntry(ep_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get statistic of specified Rate Policer entry
 *
 * @param handle
 * @param entryID
 * @param statis
 * @return status_t
 */
status_t EP_RxPSFPGetRPStatistic(ep_handle_t *handle, uint32_t entryID, netc_tb_rp_stse_t *statis);

/*!
 * @brief Add entry in ingress stream count table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t EP_RxPSFPAddISCTableEntry(ep_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get ingress stream count statistic
 *
 * @param handle
 * @param entryID
 * @param statistic
 * @return status_t
 */
status_t EP_RxPSFPGetISCStatistic(ep_handle_t *handle, uint32_t entryID, netc_tb_isc_stse_t *statistic);

/*!
 * @brief Reset the count of the ingress stream count
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t EP_RxPSFPResetISCStatistic(ep_handle_t *handle, uint32_t entryID);

/*!
 * @brief Add entry in stream gate instance table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t EP_RxPSFPAddSGITableEntry(ep_handle_t *handle, netc_tb_sgi_config_t *config);

/*!
 * @brief Delete entry in stream gate instance table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t EP_RxPSFPDelSGITableEntry(ep_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get state of the stream gate instance for specified entry
 *
 * @param handle
 * @param entryID
 * @param state
 * @return status_t
 */
status_t EP_RxPSFPGetSGIState(ep_handle_t *handle, uint32_t entryID, netc_tb_sgi_sgise_t *state);

/*!
 * @brief Add entry into Stream Gate Control List Table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t EP_RxPSFPAddSGCLTableEntry(ep_handle_t *handle, netc_tb_sgcl_config_t *config, uint8_t *buffer);

/*!
 * @brief Delete entry of Stream Gate Control List Table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t EP_RxPSFPDelSGCLTableEntry(ep_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get state (ref count) for Stream Gate Control List table entry
 *
 * @param handle
 * @param entryID
 * @param state
 * @return status_t
 */
status_t EP_RxPSFPGetSGCLState(ep_handle_t *handle, uint32_t entryID, netc_tb_sgcl_sgclse_t *state, uint8_t *buffer);

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion PSFP
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region L2 Filtering
#endif

/*!
 * @brief Init the L2 MAC Filter for a specified SI
 *
 * @param handle
 * @param config  The L2 MAC Filter configuration
 * @return status_t
 */
status_t EP_RxL2MFInit(ep_handle_t *handle, netc_si_l2mf_config_t *config);

/*!
 * @brief Add entry into the MAC address hash filter with given MAC address
 * Hardware layer will not maitain the counter of the hash filter. API layer shall cover this requirement.
 *
 * @param handle
 * @param packtype  Unicast or multicast MAC address
 * @param macAddr   MAC address
 * @return status_t
 */
status_t EP_RxL2MFAddHashEntry(ep_handle_t *handle, netc_packet_type_t packtype, uint8_t *macAddr);

/*!
 * @brief Delete entry into the MAC address hash filter with given MAC address
 * Hardware layer will not maitain the counter of the hash filter. API layer shall cover this requirement.
 *
 * @param handle
 * @param packtype  Unicast or multicast MAC address
 * @param macAddr   MAC address for filter
 * @return status_t
 */
status_t EP_RxL2MFDelHashEntry(ep_handle_t *handle, netc_packet_type_t packtype, uint8_t *macAddr);

/*!
 * @brief Add entry into the MAC filter exact match table
 *
 * The entry is associated to the current Station Interface
 *
 * @param handle
 * @param idx      Index in the entry table
 * @param macAddr  MAC address for filter
 * @return status_t
 */
status_t EP_RxL2MFAddEMTableEntry(ep_handle_t *handle, uint32_t idx, uint8_t *macAddr);

/*!
 * @brief Delete entry into the MAC filter exact match table
 *
 * @param handle
 * @param idx  Index in the entry table
 * @return status_t
 */
status_t EP_RxL2MFDelEMTableEntry(ep_handle_t *handle, uint32_t idx);

// L2 Vlan Filter Table
/*!
 * @brief For VLAN filter, use inner vlan tag or outer vlan tag
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t EP_RxL2VFInit(ep_handle_t *handle, netc_si_l2vf_config_t *config);

/*!
 * @brief Add entry into the VLAN hash filter with given MAC address
 * Hardware layer will not maitain the counter of the hash filter. API layer shall cover this requirement.
 *
 * @param handle
 * @param vlanId  VLAN identifier for filter
 * @return status_t
 */
status_t EP_RxL2VFAddHashEntry(ep_handle_t *handle, uint16_t vlanId);

/*!
 * @brief Delete entry into the VLAN hash filter with given MAC address
 * Hardware layer will not maitain the counter of the hash filter. API layer shall cover this requirement.
 *
 * @param handle
 * @param vlanId  VLAN identifier for filter
 * @return status_t
 */
status_t EP_RxL2VFDelHashEntry(ep_handle_t *handle, uint16_t vlanId);

/*!
 * @brief Add entry into the MAC filter exact match table
 *
 * The entry is associated to the current Station Interface
 *
 * @param handle
 * @param idx     Index in the entry table
 * @param vlanId  VLAN identifier
 * @param tpid    VLAN TPID
 * @return status_t
 */
status_t EP_RxL2VFAddEMTableEntry(ep_handle_t *handle, uint32_t idx, uint16_t vlanId, netc_vlan_tpid_select_t tpid);

/*!
 * @brief Delete entry into the VLAN filter exact match table
 *
 * @param handle
 * @param idx  Index in the entry table
 * @return status_t
 */
status_t EP_RxL2VFDelEMTableEntry(ep_handle_t *handle, uint32_t idx);

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion L2 Filtering
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region ICM
#endif
/*!
 * @brief Configure the Ingress Congestion Management for specified port
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t EP_RxICMConfig(ep_handle_t *handle, netc_icm_config_t_t *config);

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion ICM
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region Rx HTA
#endif
/*!
 * @brief Config Rx Host Transfer Agent
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t EP_RxHTAConfig(ep_handle_t *handle, netc_si_hta_config_t *config);
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion Rx HTA
#endif

/*! @} */ // end of netc_ep_rx
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_ep_rx
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_ep_tx
#endif
/*! @addtogroup netc_ep_tx
 * @{
 */
/*!
 * @brief Configure the Credit Based Shaper for Traffic Class
 *
 * @param handle
 * @param tcIdx
 * @param config
 * @return status_t
 */
status_t EP_TxCBSConfig(ep_handle_t *handle, netc_hw_tc_idx_t tcIdx, port_cbs_config_t *config);

/*!
 * @brief Configure time specific departure
 *
 * @param handle
 * @param tcIdx
 * @param config
 * @return status_t
 */
status_t EP_TxTSDConfig(ep_handle_t *handle, netc_hw_tc_idx_t tcIdx, netc_tsd_config_t *config);

/*!
 * @brief Config the Time Gate Scheduling entry admin gate control list
 *
 * This function is used to program the Enhanced Scheduled Transmisson. (IEEE802.1Qbv)
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t EP_TxTGSConfigAdminGcl(ep_handle_t *handle, netc_tb_tgs_gcl_t *config);

/*!
 * @brief Get Time Gate Scheduling entry operation gate control list
 *
 * This function is used to read the Enhanced Scheduled Transmisson. (IEEE802.1Qbv)
 *
 * @param handle
 * @param gcl
 * @return status_t
 */
status_t EP_TxtTGSGetOperGcl(ep_handle_t *handle, netc_tb_tgs_gcl_t *gcl, uint32_t length);

/*!
 * @brief Config the TC (traffic class) property
 *
 * @param handle
 * @param portIdx
 * @param config
 * @return status_t
 */
status_t EP_TxTrafficClassConfig(ep_handle_t *handle, netc_hw_tc_idx_t tcIdx, const netc_tx_tc_config_t *config);

/*!
 * @brief Get status of the Traffic class
 *
 * @param handle
 * @param curState
 * @param aheadState
 * @return status_t
 */
status_t EP_TxTrafficClassGetState(ep_handle_t *handle, bool *curState, bool *aheadState);

/*! @} */ // end of netc_ep_tx
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_ep_tx
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_ep_stat
#endif
/*! @addtogroup netc_ep_stat
 * @{
 */
/*!
 * @brief Get status flags which is ORed from multiple status register.
 *
 * @param handle
 * @param mask 0 stands for get all status flags or provide the mask to get specified to improve performance
 * @return uint32_t
 */
uint32_t EP_GetStatusFlags(ep_handle_t *handle, uint32_t mask);

/*! @} */ // end of netc_ep_stat
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_ep_stat
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_ep_interrupt
#endif
/*! @addtogroup netc_ep_interrupt
 * @{
 */
/*!
 * @brief Enable interrupts
 *
 * @param handle
 * @param mask see @ref _ep_interrupt_flag
 * @return status_t
 */
status_t EP_EnableInterrupt(ep_handle_t *handle, uint32_t mask);

/*!
 * @brief Disable interrupts
 *
 * @param handle
 * @param mask
 * @return status_t
 */
status_t EP_DisableInterrupt(ep_handle_t *handle, uint32_t mask);

/*!
 * @brief Clean the SI transmit interrupt flags
 *
 * @param handle    The EP handle.
 * @param txFrameIntrMask  IPV value to be mapped, bit x represents ring x.
 * @param txThresIntrMask  The Rx BD ring index to be mapped, bit x represents ring x.
 */
static inline void EP_CleanTxIntrFlags(ep_handle_t *handle, uint16_t txFrameIntrMask, uint16_t txThresIntrMask)
{
    NETC_SICleanTxIntrFlags(handle->hw.si, txFrameIntrMask, txThresIntrMask);
}

/*!
 * @brief Clean the SI receive interrupt flags
 *
 * @param handle    The EP handle.
 * @param rxIntrMask  Rx interrupt bit mask, bit x represents ring x.
 */
static inline void EP_CleanRxIntrFlags(ep_handle_t *handle, uint32_t rxIntrMask)
{
    NETC_SICleanRxIntrFlags(handle->hw.si, rxIntrMask);
}

/*!
 * @brief Set the interrupt vector to specified entry index for EP
 *
 * This function is to bind interrupts of EP to specified MSIX entry index. If interrupt occurs and it's not masked in
 * both submodule and MSIX table, the interrupt message in corresponding entry will be sent.
 *
 * @param instance The submodule instance number.
 * @param siIdx The SI number for EP instance.
 * @param vector Bind the interrupt to specified entry.
 */
status_t EP_MSIXSetVector(ep_handle_t *handle, ep_msix_vector_t *vector);

/*!
 * @brief Set the MSIX entry mask status for specified entry
 *
 * This function is to set the entry enable/disable the message interrupt transmission.
 *
 * @param handle    The EP handle
 * @param entryIdx  The entry index in the table.
 * @param mask      The mask state. True: Mask, False: Unmask.
 * @return status_t
 */
status_t NETC_MsixSetEntryMask(ep_handle_t *handle, uint8_t entryIdx, bool mask);

/*!
 * @brief Get the status in MSIX PBA table
 *
 * This function is to get the entry pending status from MSIX PBA table. If interrupt occurs but masked by vector
 * control of entry, pending bit in PBA will be set.
 *
 * @param handle  The EP handle
 * @param status  Pending status, bit n for entry n.
 * @return status_t
 */
status_t NETC_MsixGetPendingStatus(ep_handle_t *handle, uint32_t *status);

/*!
 * @brief Clean the status in MSIX PBA table
 *
 * This function is to clean the entry pending status from MSIX PBA table.
 *
 * @param handle  The EP handle
 * @param status  Pending status, bit n for entry n.
 * @return status_t
 */
status_t NETC_MsixCleanPendingStatus(ep_handle_t *handle, uint32_t status);

/*! @} */ // end of netc_ep_interrupt
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_ep_interrupt
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_ep_xfer
#endif
/*! @addtogroup netc_ep_xfer
 * @{
 */
/*!
 * @brief Transmits a frame for specified ring.
 * This API is zero-copy and requires the ep_reclaim_cb_t to be called to free the transmitted frame.
 *
 * @param handle
 * @param ring The ring index
 * @param frame The frame descriptor pointer
 * @param context Private context provided back by ep_reclaim_cb_t
 * @param opt Additional tx options. If NULL, default is tx timestamping enabled, no start time and no masquerading.
 * @retval status_t
 */
status_t EP_SendFrame(ep_handle_t *handle, uint8_t ring, netc_frame_struct_t *frame, void *context, ep_tx_opt *opt);

/*!
 * @brief Reclaim tx descriptors.
 * This function is used to update the tx descriptor status and
 * get the tx timestamp. For each reclaimed transmit frame the
 * ep_reclaim_cb_t is called.
 *
 * This is called after being notified of a transmit completion from ISR.
 * It runs until there are no more frames to be reclaimed in the BD ring.
 *
 * @param handle
 * @param ring The ring index
 */
void EP_ReclaimTxDescriptor(ep_handle_t *handle, uint8_t ring);

/*!
 * @brief Receives a frame
 *
 * @param handle
 * @param ring The ring index
 * @param frame The frame descriptor pointer
 * @param attr Frame attribute pointer
 * @return status_t
 */
status_t EP_ReceiveFrame(ep_handle_t *handle, uint8_t ring, netc_frame_struct_t *frame, netc_frame_attr_t *attr);

/*!
 * @brief Receives frame which will be copied in the provided buffer.
 *
 * The size MUST be queried using EP_GetRxFrameSize() beforhand.
 *
 * @param handle
 * @param ring Ring index
 * @param buffer Buffer address
 * @param length Buffer length
 * @param attr Frame attribute pointer
 * @return status_t
 */
status_t EP_ReceiveFrameCopy(ep_handle_t *handle, uint8_t ring, void *buffer, uint32_t length, netc_frame_attr_t *attr);

/*!
 * brief Gets the size of the pending frame in the specified receive ring buffer.
 *
 * Frame size without FCS
 *
 * @param handle The ENET handler structure. This is the same handler pointer used in the ENET_Init.
 * @param length The length of the valid frame received.
 * @param ring The ring index
 * @return status_t
 */
status_t EP_GetRxFrameSize(ep_handle_t *handle, uint8_t ring, uint32_t *length);

/*! @} */ // end of netc_ep_xfer
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_ep_xfer
#endif

#if defined(__cplusplus)
}
#endif
#endif /* _FSL_EP_H_ */
