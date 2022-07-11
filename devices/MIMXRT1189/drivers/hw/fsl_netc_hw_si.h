/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_NETC_SI_H_
#define _FSL_NETC_SI_H_

#include "fsl_netc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_hw_si
#endif
/*!
 * @addtogroup netc_hw_si
 * @{
 */

#define NETC_SI_RING_NUM_MAX 4

/*! @name Control and status bit masks of the transmit buffer descriptor. */
/*@{*/
/*! @brief Defines for read format. */
#define NETC_SI_TXDESCRIP_RD_TXSTART(n) ((uint32_t)(n)&0x1fffffUL)
#define NETC_SI_TXDESCRIP_RD_DR(n)      (((uint32_t)(n)&0x1fU) << 10U)
#define NETC_SI_TXDESCRIP_RD_IPV(n)     (((uint32_t)(n)&0x7U) << 12U)
#define NETC_SI_TXDESCRIP_RD_PORT(n)    (((uint32_t)(n)&0x1fU) << 16U)
#define NETC_SI_TXDESCRIP_RD_TSR(n)     (((uint32_t)(n)&0x1U) << 22U)
#define NETC_SI_TXDESCRIP_RD_SMSO_MASK  (1UL << 23U)
#define NETC_SI_TXDESCRIP_RD_FLQ(n)     (((uint32_t)(n)&0x03U) << 24U)
#define NETC_SI_TXDESCRIP_RD_TSE_MASK   (1UL << 25U)
#define NETC_SI_TXDESCRIP_RD_FL(n)      (((uint32_t)(n)&0x03U) << 27U)
/*@}*/

/*!
 * @brief Status for the Transmit Buffer Descriptor Status
 *
 */
enum _netc_transmit_bd_status
{
    kNETC_TxBDStatusProgrammingError        = 0x001U,
    kNETC_TxBDStatusTimeSpecificDepartDrop  = 0x002U,
    kNETC_TxBDStatusFrameSizeError          = 0x008U,
    kNETC_TxBDStatusNullAddress             = 0x009U,
    kNETC_TxBDStatusInvalidFrameBufChainLen = 0x00aU,
    kNETC_TxBDStatusSrcMacSpoofingDetected  = 0x010U,
    kNETC_TxBDStatusFrameDropDuePortReset   = 0x020U,
    kNETC_TxBDStatusFrameDropDuePortDisable = 0x021U,
    kNETC_TxBDStatusVlanTPIDNotAllowed      = 0x040U,
    kNETC_TxBDStatusProgramErrorInBD        = 0x060U,
    kNETC_TxBDStatusFrameTooLarge           = 0x080U,
    kNETC_TxBDStatusAXIReadError            = 0x090U,
    kNETC_TxBDStatusAXIWriteError           = 0x091U,
    // TODO Complete the list
};

/*!
 * @brief Helper macro function to fill the flags field of standard Tx BD.
 *
 */
#define NETC_TX_BD_TXSTART(txstart) \
    {                               \
    } // TODO

/*!
 * @brief Helper macro function to fill the flags field of standard Tx BD as Switch masquerading.
 *
 */
#define NETC_TX_BD_SWITCH_MASQUERADING(inport) \
    {                                          \
    } // TODO

/*!
 * @brief Helper macro function to fill the flags field of standard Tx BD as Switch direct enqueue.
 *
 */
#define NETC_TX_BD_SWITCH_DIRECT_ENQUEUE(outport, ipv, dr) \
    {                                                      \
    } // TODO

/*!
 * @brief Status code for the receive buffer descriptor
 *
 */
enum _netc_receive_buffer_descriptor_status
{
    kNETC_RxBDStatusNoError = 0x0, /*!< No Error happen. */
    // TODO Complete the list
};

/*!
 * @brief Flags for the receive buffer descriptor
 *
 */
enum _netc_receive_buffer_descriptor_flags_mask
{
    kNETC_RxBDFlagMaskRSSValid = 0x1, /*!< Mask bit for the RSS Valid. */
    // TODO, Complete the list
};

/*!
 * @brief Configuration structure for the Transmit Buffer Descriptor Ring
 *
 */
typedef struct _netc_ep_txbdr_config
{
    uint8_t weight;
    uint8_t priority;
    // IPV to VLAN map
} netc_ep_txbdr_config_t;

/*!
 * @brief Configuration structure for the Receive Buffer Descriptor Ring
 *
 */
typedef struct _netc_ep_rxbdr_config
{
    void *reserved;
} netc_ep_rxbdr_config_t;

/*! @brief Configuration for the buffer descriptors ring */
typedef struct _netc_si_bdr_config
{
    netc_rx_bdr_config_t rxBdConfig[NETC_SI_RING_NUM_MAX]; /*!< Receive buffer ring config. */
    netc_tx_bdr_config_t txBdConfig[NETC_SI_RING_NUM_MAX]; /*!< Transmit buffer ring config. */
    uint8_t rxBdrGroupNum; /*!< Rx BD ring group number, 0 or 1 to use ring 0 as default. */
    uint8_t ringPerGroup;  /*!< The ring number every group. */
    uint8_t *rxBufferAddr; /*!< RX buffers start adress (if zero-copy is not enabled) */
    uint32_t rxBufferSize; /*!< RX buffers size */
} netc_si_bdr_config_t;

/*!
 * @brief Configuration structure for the Command Buffer Descriptor Ring
 *
 */
typedef struct _netc_cbd_config
{
    void *reserved;
} netc_cbd_config_t;

/*!
 * @brief ENETC Station Interface BD Ring priority enumeration
 *
 */
typedef enum _enetc_si_bdr_priority
{
    kNETC_SIBdrPriorityLowest = 0U,
    kNETC_SIBdrPriority0      = 0U,
    kNETC_SIBdrPriority1,
    kNETC_SIBdrPriority2,
    kNETC_SIBdrPriority3,
    kNETC_SIBdrPriority4,
    kNETC_SIBdrPriority5,
    kNETC_SIBdrPriority6,
    kNETC_SIBdrPriority7,
    kNETC_SIBdrPriorityHighest = kNETC_SIBdrPriority7,
} enetc_si_bdr_priority_t;

/*! @} */ // end of netc_hw_si
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_hw_si
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma region netc_hw_si
#endif
/*!
 * @addtogroup netc_hw_si
 * @{
 */

/*!
 * @brief Enable the Station Interface(SI)
 *
 * @param base
 */
static inline void NETC_SIEnable(ENETC_SI_Type *base, bool enable)
{
    if (enable)
    {
        base->SIMR |= ENETC_SI_SIMR_EN_MASK;
    }
    else
    {
        base->SIMR &= ~ENETC_SI_SIMR_EN_MASK;
    }
}

/*!
 * @brief Set the port primary address for specified SI
 *
 * @param base  ENETC base address
 * @param si  SI index
 * @param macAddr  MAC address
 * @return status_t
 */
status_t NETC_SISetPrimaryMACAddr(NETC_ENETC_Type *base, netc_hw_si_idx_t si, void *macAddr);

/*!
 * @brief Get the Primary MAC address of specified MAC
 *
 * @param base  ENETC base address
 * @param si  SI index
 * @param macAddr  Memory pointer stores MAC address
 * @return status_t
 */
status_t NETC_SIGetPrimaryMACAddr(NETC_ENETC_Type *base, netc_hw_si_idx_t si, uint8_t *macAddr);

/*!
 * @brief Enable/Disable unicast/multicast/boardcast promisc mode for specified SI
 *
 * @param base
 * @param type
 * @param enable
 */
static inline void NETC_SIEnablePromisc(ENETC_SI_Type *base, netc_packet_type_t type, bool enable)
{
    if (enable)
    {
        base->SIMR |= (1U << ((uint8_t)type + 1U));
    }
    else
    {
        base->SIMR &= ~(1U << ((uint8_t)type + 1U));
    }
}

/*!
 * @brief Enable/Disable VLAN promisc mode for specified SI
 *
 * @param base
 * @param si
 * @param enable
 * @return status_t
 */
status_t NETC_SIEnableVlanPromisc(NETC_ENETC_Type *base, netc_hw_si_idx_t si, bool enable);

/*!
 * @brief Configure SI based VLAN
 *
 * @param base
 * @param si
 * @param vlan
 * @return status_t
 */
status_t NETC_SIConfigVlan(NETC_ENETC_Type *base, netc_hw_si_idx_t si, netc_vlan_t vlan);

/*!
 * @brief Enable/Disable SI based VLAN feature
 *
 * PSIVLAN register. It is used for SI-based VLAN insertion(VLAN tag to be inserted) and SI-based VLAN removal (VLAN tag
 * to be matched for removal)
 *
 * @param base
 * @param si
 * @param enable
 * @return status_t
 */
status_t NETC_SIEnableVlan(NETC_ENETC_Type *base, netc_hw_si_idx_t si, bool enable);

/*!
 * @brief Generate the hash idx for the MAC Address Hash Table
 *
 * @param addr
 * @return status_t
 */
status_t NETC_SIGenerateMACAddrHashIdx(const uint8_t *addr);

/*!
 * @brief Add exact match MAC address table entry
 *
 * @param base
 * @param addr
 * @return status_t
 */
status_t NETC_SIAddMacAddrExactMatchEntry(ENETC_SI_Type *base, netc_packet_type_t type, uint8_t *addr);

/*!
 * @brief Delete the exact match mac address table entry
 *
 * @param base
 * @param addr
 * @return status_t
 */
status_t NETC_SIDelMacAddrExactMatchEntry(ENETC_SI_Type *base, netc_packet_type_t type, uint8_t *addr);

/*!
 * @brief Generate the VLAN filter hash idx with given vlan id
 *
 * @param base
 * @param vid
 * @return status_t
 */
status_t NETC_SIGenerateVlanHashIdx(ENETC_SI_Type *base, uint8_t vid);

/*!
 * @brief Add vlan id entry into VLAN Filter hash table
 *
 * @param base
 * @param hashIdx
 * @return status_t
 */
status_t NETC_SIAddVlanHashEntry(ENETC_SI_Type *base, uint8_t hashIdx);

/*!
 * @brief Remove entry of the VLAN filter hash table with given hash index
 *
 * @param base
 * @param hashIdx
 * @return status_t
 */
status_t NETC_SIDelVlanHashEntry(ENETC_SI_Type *base, uint8_t hashIdx);

/*!
 * @brief Config the command Buffer Descriptor Ring for the SI
 *
 * @param si
 * @param psConfig
 * @return status_t
 */
static inline status_t NETC_SIConfigCommandBDR(netc_hw_si_idx_t si, netc_cbd_config_t *psConfig)
{
    return kStatus_Success;
}

/*!
 * @brief Set producer index of specified Tx BD ring
 *
 * @param base  SI base address.
 * @param ring  BD ring index.
 * @param producer  The producer index of specified ring.
 */
static inline void NETC_SISetTxProducer(ENETC_SI_Type *base, uint8_t ring, uint16_t producer)
{
    base->BDR[ring].TBPIR = producer;
}

/*!
 * @brief Get consumer index of specified Tx BD ring
 *
 * @param base  SI base address.
 * @param ring  BD ring index.
 * @return consumer  The consumer index of specified ring.
 */
static inline uint16_t NETC_SIGetTxConsumer(ENETC_SI_Type *base, uint8_t ring)
{
    return (uint16_t)base->BDR[ring].TBCIR;
}

/*!
 * @brief Set consumer index of specified Rx BD ring
 *
 * @param base  SI base address.
 * @param ring  BD ring index.
 * @param consumer  The consumer index of specified ring.
 */
static inline void NETC_SISetRxConsumer(ENETC_SI_Type *base, uint8_t ring, uint16_t consumer)
{
    base->BDR[ring].RBCIR = consumer;
}

/*!
 * @brief Configure the Transmit Buffer Descriptor Ring for specified SI
 *
 * @param base  SI base address.
 * @param ring  BD ring index.
 * @param bdrConfig  The BD configuration.
 * @return status_t
 */
status_t NETC_SIConfigTxBDR(ENETC_SI_Type *base, uint8_t ring, const netc_si_bdr_config_t *bdrConfig);

/*!
 * @brief Configure the Rx Buffer Descriptor Ring for specified SI
 *
 * @param base  SI base address.
 * @param ring  BD ring index.
 * @param bdrConfig  The BD configuration.
 * @param rxZeroCopy  Whether Rx zero copy is enabled.
 * @return status_t
 */
status_t NETC_SIConfigRxBDR(ENETC_SI_Type *base, uint8_t ring, const netc_si_bdr_config_t *bdrConfig, bool rxZeroCopy);

/*!
 * @brief Configure the Tx Buffer Descript Ring priority
 *
 * @param base
 * @param bdr
 * @param vlan
 * @return status_t
 */
status_t NETC_SISetTxBDRPriority(ENETC_SI_Type *base, uint8_t ring, netc_vlan_t vlan);

/*!
 * @brief Configure the primary MAC Address for Virtual SI
 *
 * @param base
 * @param siIdx
 * @param macAddr
 * @return status_t
 */
status_t NETC_SISetMacAddrForVF(ENETC_SI_Type *base, netc_hw_si_idx_t siIdx, uint8_t *macAddr);

/*!
 * @brief Configure the Buffer Descriptor Ring priority for specified SI
 *
 * @param si
 * @param bdr
 * @param vlan
 * @return status_t
 */
status_t NETC_SISetVlanForVSI(ENETC_SI_Type *base, netc_hw_si_idx_t siIdx, uint8_t vlan);

/*!
 * @brief Enable/Disble Anti-Spoof for specifid virtual SI
 *
 * @param base
 * @param siIdx
 * @param enable
 * @return status_t
 */
status_t NETC_SIEnableAntiSpoofForVSI(ENETC_SI_Type *base, netc_hw_si_idx_t siIdx, bool enable);

/*!
 * @brief Enable the mapping of VLAN to IPV
 *
 * @param base  SI base address.
 * @param vlan  The VLAN tag.
 * @param ipv  The IPV value for this VLAN mapping.
 */
void NETC_SIMapVlanToIpv(ENETC_SI_Type *base, netc_vlan_t vlan, uint8_t ipv);

/*!
 * @brief Enable the mapping of VLAN to IPV
 *
 * @param base  SI base address.
 * @param enable  Whether enable mapping.
 * @return status_t
 */
static inline void NETC_SIEnableVlanToIpv(ENETC_SI_Type *base, bool enable)
{
    if (enable)
    {
        base->SIMR |= ENETC_SI_SIMR_V2IPVE_MASK;
    }
    else
    {
        base->SIMR &= ~ENETC_SI_SIMR_V2IPVE_MASK;
    }
}

/*!
 * @brief Set the IPV to ring mapping
 *
 * @param base  SI base address.
 * @param ipv  IPV value to be mapped.
 * @param ring  The Rx BD ring index to be mapped.
 * @return status_t
 */
static inline void NETC_SIMapIpvToRing(ENETC_SI_Type *base, uint8_t ipv, uint8_t ring)
{
    assert((ipv <= 7U) && (ring <= 7U));

    base->SIIPVBDRMR0 |= ENETC_SI_SIIPVBDRMR0_IPV0BDR(ring) << (ipv * 4U);
}

/*!
 * @brief Set the group of Rx BD ring
 * @note Set group number as 1, ring 0 receives all Rx frames.
 *
 * @param base  SI base address.
 * @param groupNum  Total group number.
 * @param ringPerGroup  Rings per group.
 * @return status_t
 */
static inline void NETC_SISetRxBDRGroup(ENETC_SI_Type *base, uint8_t groupNum, uint8_t ringPerGroup)
{
    assert(groupNum <= 3U);
    assert(ringPerGroup <= 8U);

    /* Treat value 0 or 1 as non-group. */
    if (groupNum == 0U)
    {
        groupNum     = 1;
        ringPerGroup = 1;
    }
    else
    {
        /* If there's mutiple groups, rings per group should be > 0. */
        assert(ringPerGroup != 0U);
    }

    base->SIRBGCR = ENETC_SI_SIRBGCR_NUM_GROUPS(groupNum - 1U) | ENETC_SI_SIRBGCR_RINGS_PER_GROUP(ringPerGroup - 1U);
}

/*!
 * @brief Clean the SI transmit interrupt flags
 *
 * @param base  SI base address.
 * @param txFrameIntrMask  IPV value to be mapped, bit x represents ring x.
 * @param txThresIntrMask  The Rx BD ring index to be mapped, bit x represents ring x.
 */
static inline void NETC_SICleanTxIntrFlags(ENETC_SI_Type *base, uint16_t txFrameIntrMask, uint16_t txThresIntrMask)
{
    base->SITXIDR0 = (txFrameIntrMask << 16U) + txThresIntrMask;
}

/*!
 * @brief Clean the SI receive interrupt flags
 *
 * @param base  SI base address.
 * @param rxIntrMask  Rx interrupt bit mask, bit x represents ring x.
 */
static inline void NETC_SICleanRxIntrFlags(ENETC_SI_Type *base, uint32_t rxIntrMask)
{
    base->SIRXIDR0 = rxIntrMask;
}

/*!
 * @brief Get the discard statistic from SI layer
 *
 * @param base  SI base address.
 * @param statistic  The statistic data.
 */
void NETC_SIGetDiscardStatistic(ENETC_SI_Type *base, netc_si_discard_statistic_t *statistic);

/*!
 * @brief Get the traffic statistic from SI layer
 *
 * @param base  SI base address.
 * @param statistic  The statistic data.
 */
void NETC_SIGetTrafficStatistic(ENETC_SI_Type *base, netc_si_traffic_statistic_t *statistic);

/*! @} */ // end of netc_hw_si
#if !(defined(__GNUC__) || defined(__ICCARM__))
#pragma endregion netc_hw_si
#endif

#if defined(__cplusplus)
}
#endif
#endif /* _FSL_NETC_SI_H_ */
