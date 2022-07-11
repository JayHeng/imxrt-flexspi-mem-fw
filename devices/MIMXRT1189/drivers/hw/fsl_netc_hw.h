/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_NETC_HW_H_
#define _FSL_NETC_HW_H_

#include "fsl_netc.h"

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_hw
#endif
/*! @addtogroup netc_hw
 * @{
 */

// TODO, shall be in the feature file
#define FSL_NETC_PSEUDO_MAC_MAX_SI_NUM  (2)
#define FSL_NETC_SWITCH_MAX_PORT_NUMBER (5)

/*! @brief Nanosecond in one second. */
#define NETC_NANOSECOND_ONE_SECOND (1000000000UL)

// TODO Shall be in the feature file or in the header file
/*!
 * @brief Register groups for the Port/Link hardware
 *
 */
typedef struct _netc_port_hw
{
    NETC_PORT_Type *port; /*!< Port Address */
    union
    {
        NETC_ETH_LINK_Type *eth;       /*!< MAC Port Address */
        NETC_PSEUDO_LINK_Type *pseudo; /*!< Pseudo link port address */
    };
} netc_port_hw_t;

/*!
 * @brief Register group for the ENETC peripheral hardware
 *
 */
typedef struct _netc_enetc_hw
{
    ENETC_PCI_TYPE0_Type *func;   /*!< PCIE function number */
    NETC_ENETC_Type *base;        /*!< Base Address of ENETC Module */
    NETC_SW_ENETC_Type *common;   /*!< Common Address of ENETC module */
    netc_port_hw_t portGroup;     /*!< Port Register group */
    ENETC_GLOBAL_Type *global;    /*!< Global NETC address */
    ENETC_SI_Type *si;            /*! Station Interfce for the P/V SI */
    netc_msix_entry_t *msixTable; /*! MSIX Table address */
} netc_enetc_hw_t;

/*!
 * @brief Register group for the Switch peripheral hardware
 *
 */
typedef struct _netc_switch_hw
{
    ENETC_PCI_TYPE0_Type *func;                            /*!< Base address for PCIE function */
    NETC_SW_Type *base;                                    /*!< Base address for Switch */
    NETC_SW_ENETC_Type *common;                            /*!< Common address of ENETC Module */
    netc_port_hw_t ports[FSL_NETC_SWITCH_MAX_PORT_NUMBER]; /*!< Port Register Group */
    ENETC_GLOBAL_Type *global;                             /*!< GLobal NETC Register Base Address */
} netc_switch_hw_t;

/*!
 * @brief Register group for the Timer peripheral hardware
 *
 */
typedef struct _netc_timer_hw
{
    ENETC_PF_TMR_Type *base;   /*!< Base register address for timer module */
    ENETC_GLOBAL_Type *global; /*!< Global NETC register address */
} netc_timer_hw_t;

/*!
 * @brief Register group for the EMDIO peripheral hardware
 *
 */
typedef struct _netc_emdio_hw
{
    ENETC_PF_EMDIO_Type *base; /*!< Base register address for EMDIO module */
    ENETC_GLOBAL_Type *global; /*!< Global NETC register base address */
} netc_emdio_hw_t;

/*! @} */ // end of netc_hw
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_hw
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_hw_table
#endif
/*! @addtogroup netc_hw_table
 * @{
 */

/*! @brief Register group for SI/Switch command bd ring */
typedef struct _netc_cbdr_hw
{
    __IO uint32_t CBDRMR; /**< Command BDR mode register. */
    __I uint32_t CBDRSR;  /**< Command BDR status register. */
    uint8_t RESERVED_0[8];
    __IO uint32_t CBDRBAR0; /*!<  Command BDR base address register 0 */
    __IO uint32_t CBDRBAR1; /*!<  Command BDR base address register 1 */
    __IO uint32_t CBDRPIR;  /*!<  Command BDR producer index register */
    __IO uint32_t CBDRCIR;  /*!<  Command BDR consumer index register */
    __IO uint32_t CBDRLENR; /*!<  Command BDR length register */
} netc_cbdr_hw_t;

/*! @brief Handle for common part of EP/Switch NTMP */
typedef struct _netc_cbdr_handle
{
    netc_cbdr_hw_t *base;
    netc_cmd_bdr_t *cmdr;
    netc_tb_data_buffer_t *buffer;
} netc_cbdr_handle_t;

/*! @} */ // end of netc_hw_table
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_hw_table
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_hw_common
#endif
/*! @addtogroup netc_hw_common
 * @{
 */

static inline uint16_t EP_IncreaseIndex(uint16_t index, uint16_t max)
{
    if (++index >= max)
    {
        index = 0;
    }
    return index;
}

/*!
 * @brief Initialize the Ingress Stream Identification Key construction rule profiles
 *
 * @param base
 * @param rule
 * @param enKcPair1
 * @return void
 */
void NETC_PSFPKcProfileInit(NETC_SW_ENETC_Type *base, const netc_isi_kc_rule_t *rule, bool enKcPair1);

/*!
 * @brief Initialize the customer vlan type
 *
 * @param base
 * @param config
 * @param enRtag
 * @return void
 */
void NETC_RxVlanCInit(NETC_SW_ENETC_Type *base, const netc_vlan_classify_config_t *config, bool enRtag);
/*!
 * @brief Initialize the ingress QoS classification
 *
 * @param handle
 * @param config
 * @return status_t
 */
void NETC_RxQosCInit(NETC_SW_ENETC_Type *base, const netc_qos_classify_profile_t *profile, bool enProfile1);

/*! @} */ // end of netc_hw_common
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_hw_common
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_hw_table
#endif
/*! @addtogroup netc_hw_table
 * @{
 */

/*!
 * @brief Initialize the command BD ring
 *
 * @param base
 * @param config
 * @return status_t
 */
status_t NETC_CmdBDRInit(netc_cbdr_hw_t *base, const netc_cmd_bdr_config_t *config);

/*!
 * @brief Deinitialize the command BD ring
 *
 * @param base
 * @return status_t
 */
status_t NETC_CmdBDRDeinit(netc_cbdr_hw_t *base);

/*!
 * @brief Send the Command Buffer Descriptor to operate on a NTMP table
 *
 * @param base
 * @param cmdBdRing
 * @param cbd
 * @param version
 * @return status_t
 */
status_t NETC_CmdBDSendCommand(netc_cbdr_hw_t *base,
                               netc_cmd_bdr_t *cmdBdRing,
                               netc_cmd_bd_t *cbd,
                               netc_cbd_version_t version);

/*!
 * @brief Add entry into the ingress Port Filter Table.
 *
 * @param handle
 * @param config
 * @param entryID
 * @return status_t
 */
status_t NETC_AddIPFTableEntry(netc_cbdr_handle_t *handle, netc_tb_ipf_config_t *config, uint32_t *entryID);

/*!
 * @brief Delete an entry in the ingress Port Filter Table.
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t NETC_DelIPFTableEntry(netc_cbdr_handle_t *handle, uint32_t entryID);

/*!
 * @brief Reset the counter of an ingress port filter Table entry
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t NETC_ResetIPFMatchCounter(netc_cbdr_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get the matched count of an ingress port filter Table entry
 *
 * @param handle
 * @param entryID
 * @param count
 * @return status_t
 */
status_t NETC_GetIPFMatchedCount(netc_cbdr_handle_t *handle, uint32_t entryID, uint64_t *count);
/*!
 * @brief Add entry into Ingress Stream Identification table
 *
 * @param handle
 * @param config
 * @param entryID
 * @return status_t
 */
status_t NETC_AddISITableEntry(netc_cbdr_handle_t *handle, netc_tb_isi_config_t *config, uint32_t *entryID);

/*!
 * @brief Delete an entry in Ingress stream identification table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t NETC_DelISITableEntry(netc_cbdr_handle_t *handle, uint32_t entryID);

/*!
 * @brief Add entry into Ingress Stream table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t NETC_AddISTableEntry(netc_cbdr_handle_t *handle, netc_tb_is_config_t *config);

/*!
 * @brief Delete an entry in Ingress stream table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t NETC_DelISTableEntry(netc_cbdr_handle_t *handle, uint32_t entryID);

/*!
 * @brief Add entry into Ingress Stream Identification table
 *
 * @param handle
 * @param config
 * @param entryID
 * @return status_t
 */
status_t NETC_AddISFTableEntry(netc_cbdr_handle_t *handle, netc_tb_isf_config_t *config, uint32_t *entryID);

/*!
 * @brief Delete an entry in Ingress stream identification table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t NETC_DelISFTableEntry(netc_cbdr_handle_t *handle, uint32_t entryID);

/*!
 * @brief Add entry in ingress stream count table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t NETC_AddISCTableEntry(netc_cbdr_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get ingress stream count statistic
 *
 * @param handle
 * @param entryID
 * @param statistic
 * @return status_t
 */
status_t NETC_GetISCStatistic(netc_cbdr_handle_t *handle, uint32_t entryID, netc_tb_isc_stse_t *statistic);

/*!
 * @brief Reset the count of the ingress stream count
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t NETC_ResetISCStatistic(netc_cbdr_handle_t *handle, uint32_t entryID);

/*!
 * @brief Add entry into stream gate instance table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t NETC_AddSGITableEntry(netc_cbdr_handle_t *handle, netc_tb_sgi_config_t *config);

/*!
 * @brief Delete entry in the stream gate instance table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t NETC_DelSGITableEntry(netc_cbdr_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get statistic of specified stream gate instance table entry
 *
 * @param handle
 * @param entryID
 * @param statis
 * @return status_t
 */
status_t NETC_GetSGIState(netc_cbdr_handle_t *handle, uint32_t entryID, netc_tb_sgi_sgise_t *statis);

/*!
 * @brief Add entry into Stream Gate Control List Table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t NETC_AddSGCLTableEntry(netc_cbdr_handle_t *handle, netc_tb_sgcl_config_t *config);

/*!
 * @brief Delete entry of Stream Gate Control List Table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t NETC_DelSGCLTableEntry(netc_cbdr_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get state (ref count) for Stream Gate Control List table entry
 *
 * @param handle
 * @param entryID
 * @param state
 * @return status_t
 */
status_t NETC_GetSGCLState(netc_cbdr_handle_t *handle, uint32_t entryID, netc_tb_sgcl_sgclse_t *state);

/*!
 * @brief Add entry into Rate Policying table
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t NETC_AddRPTableEntry(netc_cbdr_handle_t *handle, netc_tb_rp_config_t *config);

/*!
 * @brief Delete entry in the Rate policying table
 *
 * @param handle
 * @param entryID
 * @return status_t
 */
status_t NETC_DelRPTableEntry(netc_cbdr_handle_t *handle, uint32_t entryID);

/*!
 * @brief Get statistic of specified Rate Policer table entry
 *
 * @param handle
 * @param entryID
 * @param statis
 * @return status_t
 */
status_t NETC_GetRPStatistic(netc_cbdr_handle_t *handle, uint32_t entryID, netc_tb_rp_stse_t *statis);
/*!
 * @brief Config the QBV (Time Gate Scheduling)
 *
 * @param handle
 * @param config
 * @return status_t
 */
status_t NETC_ConfigTGSAdminList(netc_cbdr_handle_t *handle, netc_tb_tgs_gcl_t *config);
/*!
 * @brief Get time gate table operation list
 *
 * @param handle
 * @param gcl
 * @param length
 * @return status_t
 */
status_t NETC_GetTGSOperationList(netc_cbdr_handle_t *handle, netc_tb_tgs_gcl_t *gcl, uint32_t length);
/*! @} */ // end of netc_hw_table
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_hw_table
#endif

#if defined(__cplusplus)
}
#endif
#endif /* _FSL_NETC_HW_H_ */
