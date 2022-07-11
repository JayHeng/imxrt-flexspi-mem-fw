/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_CACHE_H_
#define _FSL_CACHE_H_

#include "fsl_common.h"

/*!
 * @addtogroup xcache
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief cache driver version. */
#define FSL_CACHE_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*@}*/

/*! @brief cache line size. */
#define XCACHE_LINESIZE_BYTE (FSL_FEATURE_XCACHE_LINESIZE_BYTE)

#if (defined(FSL_FEATURE_SOC_XCACHE_POLSEL_COUNT) && (FSL_FEATURE_SOC_XCACHE_POLSEL_COUNT > 0))
/*! @brief cache region number. */
#define XCACHE_REGION_NUM (3U)
/*! @brief cache region alignment. */
#define XCACHE_REGION_ALIGNMENT (0x400U)

/*! @brief Level 2 cache controller way size. */
typedef enum _xcache_policy
{
    kXCACHE_PolicyNonCacheable = 0, /*!< Non-cacheable */
    kXCACHE_PolicyWriteThrough = 1, /*!< Write through */
    kXCACHE_PolicyWriteBack    = 2, /*!< Write back */
} xcache_policy_t;

/*! @brief XCACHE configuration structure. */
typedef struct _xcache_config
{
    /*!< The cache controller can divide whole memory into 3 regions.
     * Boundary address is the FlexSPI internal address (start from 0) instead of system
     * address (start from FlexSPI AMBA base) to split adjacent regions and must be 1KB
     * aligned. The boundary address itself locates in upper region. */
    uint32_t boundaryAddr[XCACHE_REGION_NUM - 1];
    /*!< Cacheable policy for each region. */
    xcache_policy_t policy[XCACHE_REGION_NUM];
} xcache_config_t;
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name cache control for XCACHE
 *@{
 */

#if (defined(FSL_FEATURE_SOC_XCACHE_POLSEL_COUNT) && (FSL_FEATURE_SOC_XCACHE_POLSEL_COUNT > 0))
/*!
 * @brief Returns an instance number given periphearl base address.
 *
 * @param base The peripheral base address.
 * @return XCACHE_POLSEL instance number starting from 0.
 */
uint32_t XCACHE_GetInstance(XCACHE_POLSEL_Type *base);
#endif

/*!
 * brief Returns an instance number given physical memory address.
 *
 * param address The physical memory address.
 * @return XCACHE instance number starting from 0.
 */
uint32_t XCACHE_GetInstanceByAddr(uint32_t address);

#if (defined(FSL_FEATURE_SOC_XCACHE_POLSEL_COUNT) && (FSL_FEATURE_SOC_XCACHE_POLSEL_COUNT > 0))
/*!
 * @brief Initializes an XCACHE instance with the user configuration structure.
 *
 * This function configures the XCACHE module with user-defined settings. Call the XCACHE_GetDefaultConfig() function
 * to configure the configuration structure and get the default configuration.
 *
 * @param base XCACHE_POLSEL peripheral base address.
 * @param config Pointer to a user-defined configuration structure.
 * @retval kStatus_Success XCACHE initialize succeed
 */
status_t XCACHE_Init(XCACHE_POLSEL_Type *base, const xcache_config_t *config);

/*!
 * @brief Gets the default configuration structure.
 *
 * This function initializes the XCACHE configuration structure to a default value. The default
 * values are first region covers whole cacheable area, and policy set to write back.
 *
 * @param config Pointer to a configuration structure.
 */
void XCACHE_GetDefaultConfig(xcache_config_t *config);
#endif

/*!
 * @brief Enables the cache.
 *
 * @param base XCACHE peripheral base address.
 *
 */
void XCACHE_EnableCache(XCACHE_Type *base);

/*!
 * @brief Disables the cache.
 *
 * @param base XCACHE peripheral base address.
 *
 */
void XCACHE_DisableCache(XCACHE_Type *base);

/*!
 * @brief Invalidates the cache.
 *
 * @param base XCACHE peripheral base address.
 *
 */
void XCACHE_InvalidateCache(XCACHE_Type *base);

/*!
 * @brief Invalidates cache by range.
 *
 * @param address The physical address of cache.
 * @param size_byte size of the memory to be invalidated.
 * @note Address and size should be aligned to "XCACHE_LINESIZE_BYTE".
 * The startAddr here will be forced to align to XCACHE_LINESIZE_BYTE if
 * startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
void XCACHE_InvalidateCacheByRange(uint32_t address, uint32_t size_byte);

/*!
 * @brief Cleans the cache.
 *
 * @param base XCACHE peripheral base address.
 *
 */
void XCACHE_CleanCache(XCACHE_Type *base);

/*!
 * @brief Cleans cache by range.
 *
 * @param address The physical address of cache.
 * @param size_byte size of the memory to be cleaned.
 * @note Address and size should be aligned to "XCACHE_LINESIZE_BYTE".
 * The startAddr here will be forced to align to XCACHE_LINESIZE_BYTE if
 * startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
void XCACHE_CleanCacheByRange(uint32_t address, uint32_t size_byte);

/*!
 * @brief Cleans and invalidates the cache.
 *
 * @param base XCACHE peripheral base address.
 *
 */
void XCACHE_CleanInvalidateCache(XCACHE_Type *base);

/*!
 * @brief Cleans and invalidate cache by range.
 *
 * @param address The physical address of cache.
 * @param size_byte size of the memory to be Cleaned and Invalidated.
 * @note Address and size should be aligned to "XCACHE_LINESIZE_BYTE".
 * The startAddr here will be forced to align to XCACHE_LINESIZE_BYTE if
 * startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
void XCACHE_CleanInvalidateCacheByRange(uint32_t address, uint32_t size_byte);

/*!
 * @brief Enables/disables the write buffer.
 *
 * @param base XCACHE peripheral base address.
 * @param enable The enable or disable flag.
 *       true  - enable the write buffer.
 *       false - disable the write buffer.
 */
void XCACHE_EnableWriteBuffer(XCACHE_Type *base, bool enable);

/*@}*/

/*!
 * @name Unified Cache Control for all caches
 *@{
 */

/*!
 * @brief Invalidates instruction cache by range.
 *
 * @param address The physical address.
 * @param size_byte size of the memory to be invalidated.
 * @note Address and size should be aligned to XCACHE_LINESIZE_BYTE due to the cache operation unit
 * FSL_FEATURE_XCACHE_LINESIZE_BYTE. The startAddr here will be forced to align to the cache line
 * size if startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
static inline void ICACHE_InvalidateByRange(uint32_t address, uint32_t size_byte)
{
    XCACHE_InvalidateCacheByRange(address, size_byte);
}

/*!
 * @brief Invalidates data cache by range.
 *
 * @param address The physical address.
 * @param size_byte size of the memory to be invalidated.
 * @note Address and size should be aligned to XCACHE_LINESIZE_BYTE due to the cache operation unit
 * FSL_FEATURE_XCACHE_LINESIZE_BYTE. The startAddr here will be forced to align to the cache line
 * size if startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
static inline void DCACHE_InvalidateByRange(uint32_t address, uint32_t size_byte)
{
    XCACHE_InvalidateCacheByRange(address, size_byte);
}

/*!
 * @brief Clean data cache by range.
 *
 * @param address The physical address.
 * @param size_byte size of the memory to be cleaned.
 * @note Address and size should be aligned to XCACHE_LINESIZE_BYTE due to the cache operation unit
 * FSL_FEATURE_XCACHE_LINESIZE_BYTE. The startAddr here will be forced to align to the cache line
 * size if startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
static inline void DCACHE_CleanByRange(uint32_t address, uint32_t size_byte)
{
    XCACHE_CleanCacheByRange(address, size_byte);
}

/*!
 * @brief Cleans and Invalidates data cache by range.
 *
 * @param address The physical address.
 * @param size_byte size of the memory to be Cleaned and Invalidated.
 * @note Address and size should be aligned to XCACHE_LINESIZE_BYTE due to the cache operation unit
 * FSL_FEATURE_XCACHE_LINESIZE_BYTE. The startAddr here will be forced to align to the cache line
 * size if startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
static inline void DCACHE_CleanInvalidateByRange(uint32_t address, uint32_t size_byte)
{
    XCACHE_CleanInvalidateCacheByRange(address, size_byte);
}

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_CACHE_H_*/
