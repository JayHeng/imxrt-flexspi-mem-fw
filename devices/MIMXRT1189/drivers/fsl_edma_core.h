/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_EDMA_CORE_H_
#define _FSL_EDMA_CORE_H_

#include "fsl_edma_soc.h"

/*!
 * @addtogroup edma_core
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!@brief edma core channel struture definition */
typedef struct _edma_core_mp
{
    __IO uint32_t MP_CSR; /**< Channel Control and Status, array offset: 0x10000, array step: 0x10000 */
    __IO uint32_t MP_ES;  /**< Channel Error Status, array offset: 0x10004, array step: 0x10000 */
} edma_core_mp_t;

/*!@brief edma core channel struture definition */
typedef struct _edma_core_channel
{
    __IO uint32_t CH_CSR;   /**< Channel Control and Status, array offset: 0x10000, array step: 0x10000 */
    __IO uint32_t CH_ES;    /**< Channel Error Status, array offset: 0x10004, array step: 0x10000 */
    __IO uint32_t CH_INT;   /**< Channel Interrupt Status, array offset: 0x10008, array step: 0x10000 */
    __IO uint32_t CH_SBR;   /**< Channel System Bus, array offset: 0x1000C, array step: 0x10000 */
    __IO uint32_t CH_PRI;   /**< Channel Priority, array offset: 0x10010, array step: 0x10000 */
    __IO uint32_t CH_MUX;   /**< Channel Multiplexor Configuration, array offset: 0x10014, array step: 0x10000 */
    __IO uint16_t CH_MATTR; /**< Memory Attributes Register, array offset: 0x10018, array step: 0x8000 */
} edma_core_channel_t;

/*!@brief edma core TCD struture definition */
typedef struct _edma_core_tcd
{
    __IO uint32_t SADDR;     /*!< SADDR register, used to save source address */
    __IO uint16_t SOFF;      /*!< SOFF register, save offset bytes every transfer */
    __IO uint16_t ATTR;      /*!< ATTR register, source/destination transfer size and modulo */
    __IO uint32_t NBYTES;    /*!< Nbytes register, minor loop length in bytes */
    __IO uint32_t SLAST;     /*!< SLAST register */
    __IO uint32_t DADDR;     /*!< DADDR register, used for destination address */
    __IO uint16_t DOFF;      /*!< DOFF register, used for destination offset */
    __IO uint16_t CITER;     /*!< CITER register, current minor loop numbers, for unfinished minor loop.*/
    __IO uint32_t DLAST_SGA; /*!< DLASTSGA register, next tcd address used in scatter-gather mode */
    __IO uint16_t CSR;       /*!< CSR register, for TCD control status */
    __IO uint16_t BITER;     /*!< BITER register, begin minor loop count. */
} edma_core_tcd_t;

/*!@brief EDMA typedef */
typedef edma_core_channel_t EDMA_ChannelType;
typedef edma_core_tcd_t EDMA_TCDType;
typedef void EDMA_Type;

/*!@brief EDMA base address convert macro */
#define EDMA_BASE(base)
#define EDMA_CHANNEL_BASE(base, channel) \
    ((edma_core_channel_t *)((uint32_t)base + EDMA_CHANNEL_OFFSET + channel * EDMA_CHANNEL_ARRAY_STEP(base)))
#define EDMA_TCD_BASE(base, channel) \
    ((edma_core_tcd_t *)((uint32_t)base + EDMA_CHANNEL_OFFSET + channel * EDMA_CHANNEL_ARRAY_STEP(base) + 0x20U))
#define EDMA_MP_BASE(base) ((edma_core_mp_t *)((uint32_t)base))

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*!
 * @}
 */

#endif /* _FSL_EDMA_CORE_H_ */
