/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_SWITCH_H_
#define _FSL_SWITCH_H_

#include "fsl_netc.h"
#include "fsl_netc_endpoint.h"
#include "hw/fsl_netc_hw.h"
#include "hw/fsl_netc_hw_enetc.h"
#include "hw/fsl_netc_hw_port.h"
#include "hw/fsl_netc_hw_si.h"

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region api_swt
#endif
//////////////////////////////////////
// Group for the Switch Driver
//////////////////////////////////////
/*!
 * @defgroup netc_swt NETC SWITCH (SWT) Driver
 * @details This is the group of data structure/functions for the NETC switch
 * drivers.
 * This group handles switch devices covering the features to Initialize/De-Initialize the switch, transfer
 * packets, configure the port/links, and get the status and statistic. Note that it doesn't cover the phy configuration
 * which shall be configured in MDIO module for both external PHY and internal (virtual) PHY.
 *
 * The switch API mainly relates to the SWITCH IP which provides IEEE 802.1Q functionalities.
 *
 * APIs in this group take the swt_handle_t as first parameter to retrieve the current context.
 * User shall not manipulate the members of the handle without invoking the provided APIs or help macros as it may
 * break the driver state.
 *
 * All API in this groups start with `SWT_` and data structure specific to switch start with `swt_`.
 *
 * @ingroup netc_api
 */
/*!
 * @defgroup netc_swt_init Switch (SWT) Generic Configuration
 * @details Generic Ethernet switch configuration
 * MAC, host interface and driver configuration.
 * @ingroup netc_swt
 */
/*!
 * @defgroup netc_swt_ntmp Switch (SWT) Table Management Module
 * @details Generic Table Management related functionalities
 * @ingroup netc_swt
 */
/*!
 * @defgroup netc_swt_interrupt Switch (SWT) Interrupt Module
 * @details Interrupt related functionalities
 * @ingroup netc_swt
 */
/*!
 * @defgroup netc_swt_xfer Switch (SWT) Transmit/Receive
 * @details Data interface for transmit and receive
 * @ingroup netc_swt
 */
/*!
 * @defgroup netc_swt_datapath Switch (SWT) data path
 * @details QoS, classification and filtering functionalities
 * @ingroup netc_swt
 */
/*!
 * @defgroup netc_swt_stat Switch (SWT) Statistic Module
 * @details Statistics counters
 * @ingroup netc_swt
 */
/*!
 * @defgroup netc_swt_tx Switch (SWT) Egress data path configuration
 * @details API/Data structure for configuration of the endpoint ingress datapath
 * This module includes steps below in sequence and APIs are placed following this sequence for user to identify easily
 * - EPP : Egress Packet Processing
 * - CBS: Credit Based Shaper also referred as IEEE 802.1 Qav
 * - TGS: Time Gate Scheduling referred as IEEE 802.1 Qbv
 * - ETM : Egress Queuing and Traffic Management
 *
 * API in this group starts with `SWT_Tx`.
 *
 * @ingroup netc_swt_datapath
 */
/*!
 * @defgroup netc_swt_rx Switch (SWT) Ingress data path configuration
 * @details API/Data structure for configuration of the switch ingress datapath
 * This module includes steps below in sequence and APIs are placed following this sequence for user to identify easily
 * - Parser
 * - VLAN Classification
 * - Ingress QoS Classification
 * - IPF : Ingress Port Filtering
 * - PSFP : Per-Stream Filtering and Policing
 *   - Stream Identification and Filtering
 *   - Policying: Rate Policing and Stream Gating
 * - L2 Filtering: VLAN/MAC Address
 * - Enqueue (Ingress Congestion Management ICM)
 *
 * API in this group starts with `SWT_Rx`.
 *
 * @ingroup netc_swt_datapath
 */

/*!
 * @defgroup netc_swt_mgt Switch (SWT) management
 * @details Specific switch management and the Rx/Tx sharing features (e.g FDB)
 * - FM : Ingress/Egress Frame modification
 * - FRER : Frame Replication and Elimination for Reliability sequence generation
 * @ingroup netc_swt
 */

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion api_swt
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_swt_init
#endif
/*! @addtogroup netc_swt_init
 * @{
 */
typedef struct _swt_handle swt_handle_t;

typedef enum _netc_swt_event
{
    kNETC_EPEvent0,
} netc_swt_event_t;

/*! @brief Callback for endpoint generated async events */
typedef status_t (*swt_event_cb_t)(swt_handle_t *handle, netc_swt_event_t event, void *data);

/*! @brief Callback for reclaimed tx frames */
typedef status_t (*swt_reclaim_cb_t)(swt_handle_t *handle, netc_tx_frame_info_t *frameInfo);

/*! @brief Configuration for the buffer descriptors ring */
typedef struct _swt_buffer_config
{
    netc_rx_bdr_config_t rxBdConfig; /*!< Receive buffer ring config. */
    netc_tx_bdr_config_t txBdConfig; /*!< Transmit buffer ring config. */

    uint32_t *rxBufferAddr; /*!< RX buffers start adress (if zero-copy is not enabled) */
    uint32_t rxBufferSize;  /*!< RX buffers size (if zero-copy is not enabled) */
} swt_buffer_config_t;

/*! @brief Configuration structure for Switch */
typedef struct _swt_config
{
    netc_hw_switch_idx_t switchIdx; /*!< Switch index */
    netc_hw_si_idx_t siIdx;         /*!< Mananagement interface SI index */
    struct
    {
        netc_port_ethmac_t ethMac;        /*!< Ethernet MAC configuration, vaild only on port 0 ~ 3 */
        uint8_t macAddr[6];               /*!< Ethernet MAC address, vaild only on port 0 ~ 3 */
        netc_port_common_t commonCfg;     /*!< Port common configuration */
        netc_swt_port_config_t bridgeCfg; /*!< Port bridge configuration */
        netc_ipf_port_config_t ipfCfg;    /*!< Port ingress port filter configuration */
        netc_tx_tc_config_t txTcCfg[8];   /*!< Port Tx traffic class related configuration, vaild only on port 0 ~ 3 */
        uint8_t ipvToTC[8];               /*!< Port IPV to Tx traffic class queue index mapping, range in 0 ~ 7 */
        netc_psfp_isi_port_config_t isiCfg; /*!< Port Ingress stream identification */
        bool enMirror;                      /*!< Enable/Disable Ingress Mirroring on this port */
        bool ignoreFMMiscfg;                /*!< Enable/Disable ignore the Frame Modification Misconfiguration Action */
        uint8_t lanID;                      /*!< The HSR's LANID this port belong to */
        bool inCutThrough;     /*!< Enable/Disable Ingress Cut Through on this port, vaild only on port 0 ~ 3 */
        bool outCutThrough;    /*!< Enable/Disable Egress Cut Through on this port, vaild only on port 0 ~ 3 */
        uint8_t outBufferSize; /*!< Specifies the minimal frame data buffered in words (24B) before the frame can be Cut
                                  Through out an egress port, range in 1 ~ 3, vaild only on port 0 ~ 3 */
    } ports[5];                /*!< The common configuration required for all ports */
    uint16_t dynFDBLimit; /*!< Maximum number of entries which can be dynamically learned by FDB table, a value of 0
                             implies no limit. */
    netc_swt_psfp_config_t psfpCfg;          /*!< The switch PSFP configuration, cover the ISI key construction
                                                 profiles configuration */
    netc_ipf_config_t ipfCfg;                /*!< The switch ingress port filter configuration */
    netc_vlan_classify_config_t vlanCfg;     /*!< The switch ingress vlan classify configuration */
    netc_swt_qos_classify_config_t rxqosCfg; /*!< The switch ingress VLAN to QoS mapping configuration */
    netc_swt_qos_to_vlan_config_t txqosCfg;  /*!< The switch egress QoS to VLAN mapping configuration */
    netc_swt_imr_config_t imrCfg;            /*!< The switch Ingress mirror configuration */
    netc_swt_bridge_config_t bridgeCfg;      /*!< The switch bridge configuration */
    uint8_t cmdRingUse;                      /*!< Number of command BD rings to be used */
    netc_cmd_bdr_config_t cmdBdrCfg[2];      /*!< The switch command BD rings configuration */
    bool rxZeroCopy;                         /*!< Enable zero-copy receive mode */
    netc_rx_alloc_cb_t rxBuffAlloc; /*!< Callback function to alloc memory, must be provided for zero-copy Rx. */
    netc_rx_free_cb_t rxBuffFree;   /*!< Callback function to free memory, must be provided for zero-copy Rx. */
} swt_config_t;

/*! @brief Handle for SWITCH */
struct _swt_handle
{
    netc_switch_hw_t hw;         /*!< Hardware register map resource. */
    const swt_config_t *cfg;     /*!< User configuration. */
    netc_cmd_bdr_t cmdBdRing[2]; /*!< Command BD ring handle for switch. */
    ep_handle_t *epHandle;       /*!< The EP handle to send/receive management frame. */
};

/*! @} */ // end of netc_swt_init
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_swt_init
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_swt_interrupt
#endif
/*! @addtogroup netc_swt_interrupt
 * @{
 */

/*! @} */ // end of netc_swt_interrupt
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_swt_interrupt
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_swt_xfer
#endif
/*! @addtogroup netc_swt_xfer
 * @{
 */

typedef enum _swt_tx_opt_flags
{
    kSWT_TX_OPT_REQ_TSR     = 0x1,       /*!< Request timestamp reference, returned in RX ring */
    kSWT_TX_OPT_VLAN_INSERT = 0x1 << 1U, /*!< VLAN insert, used if kEP_TX_OPT_VLAN_INSERT is set */
} swt_tx_opt_flags;

typedef struct _swt_tx_opt
{
    uint32_t flags;   /*!< A bitmask of swt_tx_opt_flags */
    uint8_t ipv;      /*!< Internal Priority Value */
    uint8_t dr;       /*!< Discard resilience */
    netc_vlan_t vlan; /*!< VLAN insert, used if kSWT_TX_OPT_VLAN_INSERT is set */
} swt_tx_opt;

/*! @} */ // end of netc_swt_xfer
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_swt_xfer
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_swt_datapath
#endif
/*! @addtogroup netc_swt_datapath
 * @{
 */

/*! @} */ // end of netc_swt_datapath
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_swt_datapath
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_swt_stat
#endif
/*! @addtogroup netc_swt_stat
 * @{
 */

/*! @} */ // end of netc_swt_stat
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_swt_stat
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_swt_init
#endif
/*! @addtogroup netc_swt_init
 * @{
 */

/*!
 * @brief Init SWITCH
 *
 * @param handle
 * @param config The user configuration
 * @param buffConfig Array of buffer configurations (for each queue/ring)
 * @return status_t
 */
status_t SWT_Init(swt_handle_t *handle, const swt_config_t *config, const swt_buffer_config_t *buffConfig);

/*!
 * @brief Deinit switch
 *
 * @param handle
 * @return status_t
 */
status_t SWT_Deinit(swt_handle_t *handle);

/*!
 * @brief Get deafult configuration for switch
 *
 * @param config
 * @return status_t
 */
status_t SWT_GetDefaultConfig(swt_config_t *config);

/*! @} */ // end of netc_swt_init
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_swt_init
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_swt_ntmp
#endif
/*! @addtogroup netc_swt_ntmp
 * @{
 */

/*!
 * @brief Init SWITCH command BD ring.
 *
 * @param handle
 * @param ring Command BD ring index
 * @param config The user configuration
 * @return status_t
 */
status_t SWT_CmdBDRInit(swt_handle_t *handle, netc_hw_swt_cbdr_idx_t ring, const netc_cmd_bdr_config_t *config);

/*!
 * @brief Deinit switch command BD ring.
 *
 * @param handle
 * @param ring Command BD ring index
 * @return status_t
 */
status_t SWT_CmdBDRDeinit(swt_handle_t *handle, netc_hw_swt_cbdr_idx_t ring);

/*!
 * @brief Execution Switch command BD.
 *
 * @param handle
 * @param ring Command BD ring index
 * @param cbd The command BD pointer
 * @return status_t
 */
status_t SWT_ExecCmdBD(swt_handle_t *handle, netc_hw_swt_cbdr_idx_t ring, netc_cmd_bd_t *cbd);

/*! @} */ // end of netc_swt_ntmp
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_swt_ntmp
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_swt_interrupt
#endif
/*! @addtogroup netc_swt_interrupt
 * @{
 */

/*! @} */ // end of netc_swt_interrupt
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_swt_interrupt
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_swt_rx
#endif
/*! @addtogroup netc_swt_rx
 * @{
 */
/*!
 * @brief Configure switch ingress mirror
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_RxMirrorConfig(swt_handle_t *handle, const netc_swt_imr_config_t *config);
/*!
 * @brief Configure Parser in Receive Data Path
 *
 * @param handle
 * @param portIdx
 * @param config
 * @return status_t
 */
static inline status_t SWT_RxParserConfig(swt_handle_t *handle,
                                          netc_hw_port_idx_t portIdx,
                                          const netc_port_parser_config_t *config)
{
    NETC_PortSetParser(handle->hw.ports[portIdx].port, config);
    return kStatus_Success;
}

/*!
 * @brief Configure the customer vlan type
 *
 * @param handle
 * @param config
 * @return status_t
 */
static inline status_t SWT_RxVlanCInit(swt_handle_t *handle, const netc_vlan_classify_config_t *config)
{
    NETC_RxVlanCInit(handle->hw.common, config, true);
    return kStatus_Success;
}

/*!
 * @brief Configure the Accepted Vlan for PORT.
 *
 * @param handle
 * @param portIdx
 * @param config
 * @return status_t
 */
static inline status_t SWT_RxVlanCConfigPort(swt_handle_t *handle,
                                             netc_hw_port_idx_t portIdx,
                                             netc_port_vlan_classify_config_t *config)
{
    NETC_PortSetVlanClassify(handle->hw.ports[portIdx].port, config);
    return kStatus_Success;
}

// Qos Classification
/*!
 * @brief Init the ingress QoS classification for Switch
 *
 * @param handle
 * @param config
 * @return status_t
 */
static inline status_t SWT_RxQosCInit(swt_handle_t *handle, const netc_swt_qos_classify_config_t *config)
{
    NETC_RxQosCInit(handle->hw.common, &config->profiles[0], false);
    NETC_RxQosCInit(handle->hw.common, &config->profiles[1], true);
    return kStatus_Success;
}

/*!
 * @brief Configure the QoS Classification for Switch Port
 *
 * @param handle
 * @param portIdx
 * @param config
 * @return status_t
 */
static inline status_t SWT_RxQosCConfigPort(swt_handle_t *handle,
                                            netc_hw_port_idx_t portIdx,
                                            netc_port_qos_classify_config_t *config)
{
    NETC_PortSetQosC(handle->hw.ports[portIdx].port, config);
    return kStatus_Success;
}

// Ingress Port Filter
/*!
 * @brief Enable / Disable Ingress Port Filtering
 *
 * Applied for both Switch and ENETC
 *
 * @param handle
 * @param config
 * @return status_t
 */
static inline status_t SWT_RxIPFInit(swt_handle_t *handle, const netc_ipf_config_t *config)
{
    handle->hw.common->DOSL2CR = NETC_SW_ENETC_DOSL2CR_SAMEADDR(config->l2DiscardSmacEquDmac) |
                                 NETC_SW_ENETC_DOSL2CR_MSAMCC(config->l2DiscardMCSmac);
    //    handle->hw.common.DOSL3CR = NETC_SW_ENETC_DOSL3CR_SAMEADDR(config->l3DiscardSipEquDip);
    return kStatus_Success;
}
/*!
 * @brief Config the IPF for specified Port
 *
 * @param handle
 * @param portIdx
 * @param config
 * @return status_t
 */
static inline status_t SWT_RxIPFConfigPort(swt_handle_t *handle,
                                           netc_hw_port_idx_t portIdx,
                                           const netc_ipf_port_config_t *config)
{
    NETC_PortSetIPF(handle->hw.ports[portIdx].port, config);
    return kStatus_Success;
}
/*!
 * @brief Add an entry for the ingress Port Filter Table.
 *
 * This function do an add & query with return hardware id which can be used as future query / delete / update.
 *
 * @param handle
 * @param config
 * @param entryID
 * @return status_t
 */
status_t SWT_RxIPFAddTableEntry(swt_handle_t *handle, netc_tb_ipf_config_t *config, uint32_t *entryID);

/*!
 * @brief Delete an entry for the ingress Port Filter Table.
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_RxIPFDelTableEntry(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Reset the counter of an ingress port filter entry
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_RxIPFResetMatchCounter(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get the matched count for entry in IPF
 *
 * @param handle
 * @param entryID
 * @param count
 * @return status_t
 */
status_t SWT_RxIPFGetMatchedCount(swt_handle_t *handle, uint32_t entryID, uint64_t *count);

// PSPF
/*!
 * @brief Init the Switch PSFP
 * Cover configuration below
 * - ISI  key construction profiles (ISIDKC0CR0 - ISIDKC3CR0)
 *
 * Not Cover
 * - Port specific isi config
 *
 * @param handle
 * @param config
 * @return status_t
 */
static inline status_t SWT_RxPSFPInit(swt_handle_t *handle, const netc_swt_psfp_config_t *config)
{
    NETC_PSFPKcProfileInit(handle->hw.common, &config->kcRule[0], false);
    NETC_PSFPKcProfileInit(handle->hw.common, &config->kcRule[2], true);
    return kStatus_Success;
}

/*!
 * @brief Configure Ingress stream identification for specified port
 * @param handle
 * @param portIdx
 * @param config
 * @return status_t
 */
static inline status_t SWT_RxPSFPConfigPortISI(swt_handle_t *handle,
                                               netc_hw_port_idx_t portIdx,
                                               const netc_psfp_isi_port_config_t *config)
{
    NETC_PortSetISI(handle->hw.ports[portIdx].port, config);
    return kStatus_Success;
}

/*!
 * @brief Add an entry into the stream identification table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_RxPSFPAddISITableEntry(swt_handle_t *handle, netc_tb_isi_config_t *config, uint32_t *entryID);

/*!
 * @brief Delete an entry in the stream identification table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_RxPSFPDelISITableEntry(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Add an entry into the ingress stream table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_RxPSFPAddISTableEntry(swt_handle_t *handle, netc_tb_is_config_t *config);

/*!
 * @brief Delete an entry in the stream identification table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_RxPSFPDelISTableEntry(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Add an entry into the ingress stream filter table
 *
 * @param handle
 * @param config
 * @param entryID
 * @return status_t
 */
status_t SWT_RxPSFPAddISFTableEntry(swt_handle_t *handle, netc_tb_isf_config_t *config, uint32_t *entryID);

/*!
 * @brief Delete an entry in the ingress stream filter table
 *
 * @param handle
 * @param keye element
 * @return status_t
 */
status_t SWT_RxPSFPDelISFTableEntry(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Add entry to Rate Policying table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_RxPSFPAddRPTableEntry(swt_handle_t *handle, netc_tb_rp_config_t *config);

/*!
 * @brief Delete entry in the Rate policying table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_RxPSFPDelRPTableEntry(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get statistic of specified Rate Policer entry
 *
 * @param handle
 * @param entryID
 * @param statis
 * @return status_t
 */
status_t SWT_RxPSFPGetRPStatistic(swt_handle_t *handle, uint32_t entryID, netc_tb_rp_stse_t *statis);

/*!
 * @brief Add entry in ingress stream count table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_RxPSFPAddISCTableEntry(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get ingress stream count statistic
 *
 * @param handle
 * @param entryID
 * @param statistic
 * @return status_t
 */
status_t SWT_RxPSFPGetISCStatistic(swt_handle_t *handle, uint32_t entryID, netc_tb_isc_stse_t *statistic);

/*!
 * @brief Reset the count of the ingress stream count
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_RxPSFPResetISCStatistic(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Add entry in stream gate instance table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_RxPSFPAddSGITableEntry(swt_handle_t *handle, netc_tb_sgi_config_t *config);

/*!
 * @brief Delete entry in stream gate instance table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_RxPSFPDelSGITableEntry(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get state of the stream gate instance for specified entry
 *
 * @param handle
 * @param entryID
 * @param state
 * @return status_t
 */
status_t SWT_RxPSFPGetSGIState(swt_handle_t *handle, uint32_t entryID, netc_tb_sgi_sgise_t *state);

/*!
 * @brief Add entry into Stream Gate Control List Table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_RxPSFPAddSGCLTableEntry(swt_handle_t *handle, netc_tb_sgcl_config_t *config, uint8_t *buffer);

/*!
 * @brief Delete entry of Stream Gate Control List Table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_RxPSFPDelSGCLTableEntry(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get state (ref count) for Stream Gate Control List table entry
 *
 * @param handle
 * @param entryID
 * @param state
 * @return status_t
 */
status_t SWT_RxPSFPGetSGCLState(swt_handle_t *handle, uint32_t entryID, netc_tb_sgcl_sgclse_t *state, uint8_t *buffer);

/*! @} */ // end of netc_swt_rx
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_swt_rx
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_swt_mgt
#endif
/*! @addtogroup netc_swt_mgt
 * @{
 */

/*!
 * @brief Config the Bridge
 *
 * Cover configuration below
 * - Vlan filter table default VFHTDRCR0-2
 *
 * Not Cover
 * - Port specific Vlan config
 * - Port Storm Control BPSCR0/R1
 * - FDB entry
 * - Vlan Filter Table entry
 * - Multicast filter entry
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_BridgeInit(swt_handle_t *handle, const netc_swt_bridge_config_t *config);

/*!
 * @brief Config bridge port vlan Tx/Rx
 *
 * Cover configuration below
 * - Port specific Vlan config
 * - Port Storm Control BPSCR0/R1
 *
 * @param handle
 * @param portIdx
 * @param config
 * @return status_t
 */
status_t SWT_BridgeConfigPort(swt_handle_t *handle, netc_hw_port_idx_t portIdx, const netc_swt_port_config_t *config);

/*!
 * @brief Add entry into bridge vlan filter table
 *
 * @param handle
 * @param config
 * @param entryID
 * @return status_t
 */
status_t SWT_BridgeAddVFTableEntry(swt_handle_t *handle, netc_tb_vf_config_t *config, uint32_t *entryID);

/*!
 * @brief Delete entry in bridge vlan filter table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_BridgeDelVFTableEntry(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Add entry into bridge FDB table
 *
 * @param handle
 * @param config
 * @param entryID
 * @return status_t
 */
status_t SWT_BridgeAddFDBTableEntry(swt_handle_t *handle, netc_tb_fdb_config_t *config, uint32_t *entryID);

/*!
 * @brief Delete an entry in bridge FDB Table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_BridgeDelFDBTableEntry(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get the activity data for FDB table entry
 *
 * @param handle
 * @param entryID
 * @param state
 * @return status_t
 */
status_t SWT_BridgeGetFDBActivityState(swt_handle_t *handle, uint32_t entryID, netc_tb_fdb_acte_t *state);

/*!
 * @brief Add entry into Bridge multicast filter table
 *
 * @param handle
 * @param config
 * @param entryID
 * @return status_t
 */
status_t SWT_BridgeAddL2MCFTableEntry(swt_handle_t *handle, netc_tb_l2mcf_config_t *config, uint32_t *entryID);

/*!
 * @brief Get the activity data for L2MCF table entry
 *
 * @param handle
 * @param entryID
 * @param state
 * @return status_t
 */
status_t SWT_BridgeGetL2MCFActivityState(swt_handle_t *handle, uint32_t entryID, netc_tb_l2mcf_acte_t *state);

/*!
 * @brief Delete entry in bridge multicast filter table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_BridgeDelL2MCFTableEntry(swt_handle_t *handle, uint32_t entryID);

// Frame Modification
/*!
 * @brief Config Frame Modification
 *
 * @param handle
 * @param portIdx
 * @param ignoreFMMiscfg
 * @return status_t
 */
static inline status_t SWT_FMConfigPort(swt_handle_t *handle, netc_hw_port_idx_t portIdx, bool ignoreFMMiscfg)
{
    handle->hw.ports[portIdx].port->PFMCR = NETC_PORT_PFMCR_FMMA(ignoreFMMiscfg);
    return kStatus_Success;
}

/*!
 * @brief Add entry into the Frame Modify table
 *
 * Add entry of the frame modification table. Frame modification can be encoded into the table id directly or provided
 * as index of the table entry. refer to the frame modification entry definition.
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_FMAddTableEntry(swt_handle_t *handle, netc_tb_fm_config_t *config);

/*!
 * @brief Delete table entry in Frame Modify Table
 *
 * The provided ID must be table index. Error return if the id is action encoded id.
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_FMDelTableEntry(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Update Frame Modify data table contents
 *
 * @param handle
 * @param config
 * @param length
 * @return status_t
 */
status_t SWT_FMDUpdateTableEntry(swt_handle_t *handle, netc_tb_fmd_update_config_t *config, uint32_t length);
/*!
 * @brief Query Frame Modify data table contents
 *
 * @param handle
 * @param query
 * @param length
 * @return status_t
 */
status_t SWT_FMDQueryTableEntry(swt_handle_t *handle, netc_tb_fmd_query_buffer_t *query, uint32_t length);

/*!
 * @brief Add entry into FRER sequence number generation
 *
 * @param handle
 * @param config
 * @return uint32_t
 */
status_t SWT_FRERAddISEQGTableEntry(swt_handle_t *handle, netc_tb_iseqg_config_t *config);

/*!
 * @brief Delete entry into FRER sequence number generation
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_FRERDelISEQGTableEntry(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get the Sequence generation number form sequence generation table for specified entry
 *
 * @param handle
 * @param entryID
 * @param state
 * @return status_t
 */
status_t SWT_FRERGetISEQGState(swt_handle_t *handle, uint32_t entryID, netc_tb_iseqg_sgse_t *state);

/*!
 * @brief Config FRER sequence number recovery table entry
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_FRERConfigESEQRTableEntry(swt_handle_t *handle, netc_tb_eseqr_config_t *config);

/*!
 * @brief Get FRER sequence recorvery table state and statistic
 *
 * @param handle
 * @param entryID
 * @param statistic
 * @param state
 * @return status_t
 */
status_t SWT_FRERGetESEQRStatistic(swt_handle_t *handle,
                                   uint32_t entryID,
                                   netc_tb_eseqr_stse_t *statistic,
                                   netc_tb_eseqr_srse_t *state);

/*! @} */ // end of netc_swt_mgt
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_swt_mgt
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_swt_tx
#endif
/*! @addtogroup netc_swt_tx
 * @{
 */

/*!
 * @brief Configure the transmit maximum SDU for port Traffic Class
 *
 * @param handle
 * @param portIdx
 * @param tcIdx
 * @param config
 * @return status_t
 */
static inline status_t SWT_TxSDUConfigPort(swt_handle_t *handle,
                                           netc_hw_port_idx_t portIdx,
                                           netc_hw_tc_idx_t tcIdx,
                                           const port_sdu_config_t *config)
{
    assert((handle != NULL) && (config != NULL));

    return ENETC_PortConfigTXMaxSDU(handle->hw.ports[portIdx].port, tcIdx, config);
}

/*!
 * @brief Configure the Credit Based Shaper for port Traffic Class
 *
 * @param handle
 * @param portIdx
 * @param tcIdx
 * @param config
 * @return status_t
 */
static inline status_t SWT_TxCBSConfigPort(swt_handle_t *handle,
                                           netc_hw_port_idx_t portIdx,
                                           netc_hw_tc_idx_t tcIdx,
                                           const port_cbs_config_t *config)
{
    assert((handle != NULL) && (config != NULL));

    return ENETC_PortConfigCBS(handle->hw.ports[portIdx].port, tcIdx, config);
}

/*!
 * @brief Config the Time Gate Scheduling entry admin gate control list
 *
 * This function is used to program the Enhanced Scheduled Transmisson. (IEEE802.1Qbv)
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_TxTGSConfigAdminGcl(swt_handle_t *handle, netc_tb_tgs_gcl_t *config);

/*!
 * @brief Get Time Gate Scheduling entry operation gate control list
 *
 * This function is used to read the Enhanced Scheduled Transmisson. (IEEE802.1Qbv)
 *
 * @param handle
 * @param gcl
 * @return status_t
 */
status_t SWT_TxtTGSGetOperGcl(swt_handle_t *handle, netc_tb_tgs_gcl_t *gcl, uint32_t length);

/*!
 * @brief Config the TC (traffic class) property
 *
 * @param handle
 * @param portIdx
 * @param tcIdx
 * @param config
 * @return status_t
 */
status_t SWT_TxTrafficClassConfig(swt_handle_t *handle,
                                  netc_hw_port_idx_t portIdx,
                                  netc_hw_tc_idx_t tcIdx,
                                  const netc_tx_tc_config_t *config);

/*!
 * @brief Get status of the Traffic class
 *
 * @param handle
 * @param portIdx
 * @param curState
 * @param aheadState
 * @return status_t
 */
status_t SWT_TxTrafficClassGetState(swt_handle_t *handle, netc_hw_port_idx_t portIdx, bool *curState, bool *aheadState);

/*!
 * @brief Add entry in the egress treatment table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_TxEPPAddETTableEntry(swt_handle_t *handle, netc_tb_et_config_t *config);

/*!
 * @brief Delete entry for the egress treatment table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_TxEPPDelETTableEntry(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Add entry in the egress treatment table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_TxEPPQosToVlanConfig(swt_handle_t *handle, const netc_swt_qos_to_vlan_config_t *config);

/*!
 * @brief Configure the WBFS and Strict priority for each PORT
 *
 * Check the ETM class scheduler table.
 * @note there is limitation between TSD and class queue scheduler configuration
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_TxETMConfigClassScheduler(swt_handle_t *handle, netc_tb_etmcs_config_t *config);

/*!
 * @brief Configure the class queue to congestion group map using ETM class queue table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_TxETMConfigClassQueue(swt_handle_t *handle, netc_tb_etmcq_config_t *config);

/*!
 * @brief Config ETM congestion group
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t SWT_TxETMConfigCongestionGroup(swt_handle_t *handle, netc_tb_etmcg_config_t *config);

/*!
 * @brief
 *
 * @param handle
 * @param portIdx
 * @param queueIdx
 * @param map
 * @return status_t
 */
status_t SWT_TxETMConfigQosToQueueMap(swt_handle_t *handle,
                                      netc_hw_port_idx_t portIdx,
                                      netc_hw_etm_class_queue_idx_t queueIdx,
                                      uint32_t map);

/*! @} */ // end of netc_swt_tx
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_swt_tx
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_swt_xfer
#endif
/*! @addtogroup netc_swt_xfer
 * @{
 */

/*!
 * @brief Set the EP handle used by switch frame management.
 *
 * @param handle
 * @param epHandle The EP handle
 * @retval status_t
 */
status_t SWT_SetEpHandle(swt_handle_t *handle, ep_handle_t *epHandle);

/*!
 * @brief Transmits a management frame on a specific port.
 *
 * @param handle
 * @param portIdx The output port index
 * @param frame The frame descriptor pointer
 * @param context Private context provided back on reclaim
 * @param opt Tx options.
 * @retval status_t
 */
status_t SWT_SendFrame(
    swt_handle_t *handle, netc_hw_port_idx_t portIdx, netc_frame_struct_t *frame, void *context, swt_tx_opt *opt);

/*!
 * @brief Reclaim tx descriptors.
 * This function is used to update the tx descriptor status.
 * For each reclaimed transmit frame the swt_reclaim_cb_t is called.
 *
 * This is called after being notified of a transmit completion from ISR.
 * It runs until there are no more frames to be reclaimed in the BD ring.
 *
 * @param handle
 */
void SWT_ReclaimTxDescriptor(swt_handle_t *handle);

/*!
 * @brief Receives a  management frame
 *
 * @param handle
 * @param frame The frame descriptor pointer
 * @param attr Frame attribute pointer
 * @return status_t
 */
status_t SWT_ReceiveFrame(swt_handle_t *handle, uint8_t ring, netc_frame_struct_t *frame, netc_frame_attr_t *attr);

/*!
 * @brief Receives frame which will be copied in the provided buffer.
 *
 * The size MUST be queried using SWT_GetRxFrameSize() beforhand.
 *
 * @param handle
 * @param buffer Buffer address
 * @param length Buffer length
 * @param attr Frame attribute pointer
 * @return status_t
 */
status_t SWT_ReceiveFrameCopy(
    swt_handle_t *handle, uint8_t ring, void *buffer, uint32_t length, netc_frame_attr_t *attr);

/*!
 * brief Gets the size of the pending frame in the specified receive ring buffer.
 *
 * Frame size without FCS
 *
 * @param handle The ENET handler structure. This is the same handler pointer used in the ENET_Init.
 * @param length The length of the valid frame received.
 * @return status_t
 */
status_t SWT_GetRxFrameSize(swt_handle_t *handle, uint8_t ring, uint32_t *length);

/*! @} */ // end of netc_swt_xfer
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_swt_xfer
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_swt_stat
#endif
/*! @addtogroup netc_swt_stat
 * @{
 */

/*!
 * @brief Get Switch cut through forwarding count
 *
 * Get the Cut-through frames counter of frames forwarded to at least one egress port.
 *
 * @param handle
 * @param statistic pointer to the statistic count data
 * @return status_t
 */
static inline status_t SWT_GetCutThroughForwardStatistic(swt_handle_t *handle, uint32_t *statistic)
{
    *statistic = handle->hw.base->CTFCR;
    return kStatus_Success;
}

/*!
 * @brief Get the port discard statistic and reason
 *
 * Get the discarded count of frames and its reasons.
 *
 * @param handle
 * @param portIdx port index
 * @param portType true - Tx port.  false - Rx port.
 * @param statistic pointer to the statistic data
 * @param resetCount reset or not reset the count of the discared Frame
 * @return status_t
 */
status_t SWT_GetPortDiscardStatistic(swt_handle_t *handle,
                                     netc_hw_port_idx_t portIdx,
                                     bool portType,
                                     netc_port_discard_statistic_t *statistic,
                                     bool resetCount);

/*!
 * @brief Clean the Port Rx discard reason. Set the related bits to 1 to clear the specific reasons
 *
 * @param handle
 * @param portIdx
 * @param portType true - Tx port.  false - Rx port.
 * @param reason0
 * @param reason1
 * @return status_t
 */
status_t SWT_ClearPortDiscardReason(
    swt_handle_t *handle, netc_hw_port_idx_t portIdx, bool portType, uint32_t reason0, uint32_t reason1);

/*!
 * @brief Get statistic of the congestion group
 *
 * @param handle
 * @param entryID
 * @param statistic
 * @return status_t
 */
status_t SWT_GetETMCongestionGroupStatistic(swt_handle_t *handle, uint32_t entryID, netc_tb_etmcg_stse_t *statistic);

/*!
 * @brief Get the statistic for the class queue in specified switch port
 *
 * @param handle
 * @param entryID
 * @param statistic
 * @return status_t
 */
status_t SWT_GetETMClassQueueStatistic(swt_handle_t *handle, uint32_t entryID, netc_tb_etmcq_stse_t *statistic);

/*!
 * @brief Get statistic for specified egress count table entry
 *
 * @param handle
 * @param entryID
 * @param statistic
 * @return status_t
 */
status_t SWT_GetECStatistic(swt_handle_t *handle, uint32_t entryID, netc_tb_ec_stse_t *statistic);

/*!
 * @brief Reset statistic for specified egress count table entry
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t SWT_ResetECStatistic(swt_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get switch port discard statistic
 *
 * @param handle
 * @param portIdx
 * @param statistic
 * @return status_t
 */
status_t SWT_GetBridgePortDiscardStatistic(swt_handle_t *handle,
                                           netc_hw_port_idx_t portIdx,
                                           netc_switch_port_discard_statistic_t *statistic);

/*! @} */ // end of netc_swt_stat
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_swt_stat
#endif

#if defined(__cplusplus)
}
#endif
#endif /* _FSL_SWITCH_H_ */
