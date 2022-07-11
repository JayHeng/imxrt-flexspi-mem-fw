/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_NETC_ENETC_H_
#define _FSL_NETC_ENETC_H_

#include "fsl_netc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define NETC_EP_RING_NUM_MAX 4

#if defined(__cplusplus)
extern "C" {
#endif

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_hw_enetc
#endif
/*! @addtogroup netc_hw_enetc
 * @{
 */

/*!
 * @brief ENETC configuration structure.
 *
 */
typedef struct _netc_enetc_config_t
{
    void *reserved;
} netc_enetc_config_t;

/*! @} */ // end of netc_hw_enetc
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_hw_enetc
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_hw_enetc
#endif
/*! @addtogroup netc_hw_enetc
 * @{
 */

/*!
 * @brief Initializes the NETC Module
 *
 * This function is to make the initialization for using CADC module.
 * The operations are:
 *  - Enable the clock for CADC.
 *  - Set power up delay and Idle work mode.
 *
 * @note The high limit value, low limit value, offset value and zerocrossing mode of each sample slot
 * will not be configured in this function, to set those options, the APIs in "Sample Slot Control Interfaces"
 * function group can be used.
 *
 * @param base   NETC peripheral base address.
 * @param psConfig Pointer to configuration structure.
 */
static inline void NETC_EnetcInit(NETC_ENETC_Type *base, const netc_enetc_config_t *psConfig)
{
    // Setup IRQ

    // Setup Phy Link

    // Setup the BD Ring for TX/RX

    // Start the ENETC
}

/*!
 * @brief De-Initializes the NETC Module
 *
 * @param base   NETC peripheral base address.
 */
void NETC_EnetcDeInit(int *base);

/*!
 * @brief Start the ENETC
 *
 * TODO Start the module
 * TODO Start the Phy Link
 *
 * @param base
 * @return status_t
 */
status_t NETC_EnetcStart(NETC_ENETC_Type *base);

/*!
 * @brief Stop the ENETC and its relevant phy link
 *
 * @param base
 * @return status_t
 */
status_t NETC_EnetcStop(NETC_ENETC_Type *base);

/*!
 * @brief Set MAC address for specified VSI of ENETC
 *
 * @param base
 * @param macAddr
 */
void NETC_EnetcSetSIMacAddr(NETC_ENETC_Type *base, uint8_t si, uint8_t *macAddr);

/*!
 * @brief Configure SI
 *
 * @param base  ENETC peripheral base address.
 * @param si  The SI number
 * @param psConfig  The SI configuration
 * @return status_t
 */
status_t NETC_EnetcConfigureSI(NETC_ENETC_Type *base, uint8_t si, const netc_hw_enetc_si_config_t *psConfig);

/*!
 * @brief Set SI MSIX table entry number
 *
 * @param base  ENETC peripheral base address.
 * @param si  The SI number.
 * @param msixNum  The MSIX table entry number.
 * @return status_t
 */
status_t NETC_EnetcSetMsixEntryNum(NETC_ENETC_Type *base, uint8_t si, uint32_t msixNum);

/*!
 * @brief Enable SI
 *
 * @param base
 * @param si
 */
static inline void NETC_EnetcEnableSI(NETC_ENETC_Type *base, uint8_t si, bool enable)
{
    /* Enable SI from ENETC block, need to set SI block together to enable SI fully. */
    if (enable)
    {
        base->PMR |= (NETC_ENETC_PMR_SI0EN_MASK << si);
    }
    else
    {
        base->PMR &= ~(NETC_ENETC_PMR_SI0EN_MASK << si);
    }
}

void NETC_GetPortDiscardStatistic(NETC_ENETC_Type *base, netc_enetc_port_discard_statistic_t *statistic);

static inline void NETC_EnetcSetIPF(NETC_SW_ENETC_Type *base, bool l2DiscardMCSmac, bool l2DiscardSmacEquDmac)
{
    base->DOSL2CR =
        NETC_SW_ENETC_DOSL2CR_SAMEADDR(l2DiscardSmacEquDmac) | NETC_SW_ENETC_DOSL2CR_MSAMCC(l2DiscardMCSmac);
}

/*!
 * @brief Enable MAC promiscuous mode
 *
 * @param base  ENETC peripheral base address.
 * @param si    SI index.
 * @param enableUCPromis  Enable unicast frame promiscuous.
 * @param enableMCPromis  Enable multicast frame promiscuous.
 */
void NETC_EnetcEnablePromiscuous(NETC_ENETC_Type *base, uint8_t si, bool enableUCPromis, bool enableMCPromis);

/*!
 * @brief Configure VLAN filter
 *
 * @param base  ENETC peripheral base address.
 * @param si    SI index.
 * @param config  Enable untagged VLAN frame promiscuous.
 */
void NETC_EnetcConfigureVlanFilter(NETC_ENETC_Type *base, uint8_t si, netc_si_l2vf_config_t *config);

/*!
 * @brief Add the hash filter for the MAC address
 *
 * Hardware layer will not maitain the counter of the hash filter. API layer shall cover this requirement.
 *
 * @param base  ENETC peripheral base address.
 * @param si    SI index.
 * @param type  Unicast or multicast frame type.
 * @param hashIndex  The calculated hash index of MAC address.
 */
void NETC_EnetcAddMacAddrHash(NETC_ENETC_Type *base, uint8_t si, netc_packet_type_t type, uint8_t hashIndex);

/*!
 * @brief Remove the hash filter for the MAC address
 *
 * @param base  ENETC peripheral base address.
 * @param si    SI index.
 * @param type  Unicast or multicast frame type.
 * @param hashIndex  The calculated hash index of MAC address.
 */
void NETC_EnetcDelMacAddrHash(NETC_ENETC_Type *base, uint8_t si, netc_packet_type_t type, uint8_t hashIndex);

/*!
 * @brief Add the hash filter for the VLAN
 *
 * @param base  ENETC peripheral base address.
 * @param si    SI index.
 * @param hashIndex  The calculated hash index of MAC address.
 */
void NETC_EnetcAddVlanHash(NETC_ENETC_Type *base, uint8_t si, uint8_t hashIndex);

/*!
 * @brief Remove the hash filter for the VLAN
 *
 * @param base  ENETC peripheral base address.
 * @param si    SI index.
 * @param hashIndex  The calculated hash index of MAC address.
 */
void NETC_EnetcDelVlanHash(NETC_ENETC_Type *base, uint8_t si, uint8_t hashIndex);

/*! @} */ // end of netc_hw_enetc
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_hw_enetc
#endif

#if defined(__cplusplus)
}
#endif
#endif /* _FSL_NETC_ENETC_H_ */
