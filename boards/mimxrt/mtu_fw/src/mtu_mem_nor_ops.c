/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mtu_mem_nor_ops.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 *****************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

void mtu_mixspi_nor_disable_cache(mixspi_cache_status_t *cacheStatus)
{
#if (defined __CORTEX_M) && (__CORTEX_M == 7U)
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    /* Disable D cache. */
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
        cacheStatus->DCacheEnableFlag = true;
    }
#endif /* __DCACHE_PRESENT */

#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    /* Disable I cache. */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
        cacheStatus->ICacheEnableFlag = true;
    }
#endif /* __ICACHE_PRESENT */

#elif (defined FSL_FEATURE_SOC_LMEM_COUNT) && (FSL_FEATURE_SOC_LMEM_COUNT != 0U)
    /* Disable code bus cache and system bus cache */
    if (LMEM_PCCCR_ENCACHE_MASK == (LMEM_PCCCR_ENCACHE_MASK & LMEM->PCCCR))
    {
        L1CACHE_DisableCodeCache();
        cacheStatus->codeCacheEnableFlag = true;
    }
    if (LMEM_PSCCR_ENCACHE_MASK == (LMEM_PSCCR_ENCACHE_MASK & LMEM->PSCCR))
    {
        L1CACHE_DisableSystemCache();
        cacheStatus->systemCacheEnableFlag = true;
    }

#elif (defined FSL_FEATURE_SOC_CACHE64_CTRL_COUNT) && (FSL_FEATURE_SOC_CACHE64_CTRL_COUNT != 0U)
    /* Disable cache */
    CACHE64_DisableCache(EXAMPLE_CACHE);
    cacheStatus->CacheEnableFlag = true;
#endif
}

void mtu_mixspi_nor_enable_cache(mixspi_cache_status_t cacheStatus)
{
#if (defined __CORTEX_M) && (__CORTEX_M == 7U)
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    if (cacheStatus.DCacheEnableFlag)
    {
        /* Enable D cache. */
        SCB_EnableDCache();
    }
#endif /* __DCACHE_PRESENT */

#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    if (cacheStatus.ICacheEnableFlag)
    {
        /* Enable I cache. */
        SCB_EnableICache();
    }
#endif /* __ICACHE_PRESENT */

#elif (defined FSL_FEATURE_SOC_LMEM_COUNT) && (FSL_FEATURE_SOC_LMEM_COUNT != 0U)
    if (cacheStatus.codeCacheEnableFlag)
    {
        /* Enable code cache. */
        L1CACHE_EnableCodeCache();
    }

    if (cacheStatus.systemCacheEnableFlag)
    {
        /* Enable system cache. */
        L1CACHE_EnableSystemCache();
    }
#elif (defined FSL_FEATURE_SOC_CACHE64_CTRL_COUNT) && (FSL_FEATURE_SOC_CACHE64_CTRL_COUNT != 0U)
    if (cacheStatus.CacheEnableFlag)
    {
        /* Enable cache. */
        CACHE64_EnableCache(EXAMPLE_CACHE);
    }
#endif
}

static status_t mtu_mixspi_nor_write_enable(mixspi_user_config_t *userConfig, uint32_t baseAddr, flash_inst_mode_t flashInstMode)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write enable */
    flashXfer.deviceAddress = baseAddr;
    flashXfer.port          = userConfig->mixspiPort;
    flashXfer.cmdType       = kFLEXSPI_Command;
    if (flashInstMode == kFlashInstMode_Hyper)
    {
        flashXfer.SeqNumber = 2;
    }
    else
    {
        flashXfer.SeqNumber = 1;
    }
    switch (flashInstMode)
    {
        case kFlashInstMode_QPI_1:
        case kFlashInstMode_QPI_2:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_QPI;
            break;

        case kFlashInstMode_OPI:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI;
            break;

        case kFlashInstMode_SPI:
        case kFlashInstMode_Hyper:
        default:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE;
            break;
    }

    status = FLEXSPI_TransferBlocking(userConfig->mixspiBase, &flashXfer);

    return status;
}

static status_t mtu_mixspi_nor_wait_bus_busy(mixspi_user_config_t *userConfig, flash_inst_mode_t flashInstMode)
{
    /* Wait status ready. */
    bool isBusy;
    uint32_t readValue;
    status_t status;
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = 0;
    flashXfer.port          = userConfig->mixspiPort;
    flashXfer.cmdType       = kFLEXSPI_Read;
    if (flashInstMode == kFlashInstMode_Hyper)
    {
        flashXfer.SeqNumber = 2;
        flashXfer.dataSize = 2;
    }
    else
    {
        flashXfer.SeqNumber = 1;
        flashXfer.dataSize = 1;
    }
    switch (flashInstMode)
    {
        case kFlashInstMode_QPI_1:
        case kFlashInstMode_QPI_2:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_READSTATUS_QPI;
            break;

        case kFlashInstMode_OPI:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI;
            break;

        case kFlashInstMode_SPI:
        case kFlashInstMode_Hyper:
        default:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_READSTATUS;
            break;
    }
    flashXfer.data     = &readValue;

    do
    {
        status = FLEXSPI_TransferBlocking(userConfig->mixspiBase, &flashXfer);

        if (status != kStatus_Success)
        {
            return status;
        }
        if (flashInstMode == kFlashInstMode_Hyper)
        {
            if (readValue & (1U << (userConfig->flashBusyStatusOffset + 8)))
            {
                isBusy = false;
            }
            else
            {
                isBusy = true;
            }
            if (readValue & ((uint16_t)userConfig->flashMixStatusMask << 8))
            {
                status = kStatus_Fail;
                break;
            }
        }
        else
        {
            if (userConfig->flashBusyStatusPol)
            {
                if (readValue & (1U << userConfig->flashBusyStatusOffset))
                {
                    isBusy = true;
                }
                else
                {
                    isBusy = false;
                }
            }
            else
            {
                if (readValue & (1U << userConfig->flashBusyStatusOffset))
                {
                    isBusy = false;
                }
                else
                {
                    isBusy = true;
                }
            }
        }

    } while (isBusy);

    return status;
}

status_t mtu_mixspi_nor_write_register(mixspi_user_config_t *userConfig, flash_reg_access_t *regAccess)
{
    flexspi_transfer_t flashXfer;
    status_t status;
    
    if (!regAccess->regNum)
    {
        return kStatus_Success;
    }

#if defined(MTU_CACHE_MAINTAIN) && MTU_CACHE_MAINTAIN
    mixspi_cache_status_t cacheStatus;
    mtu_mixspi_nor_disable_cache(&cacheStatus);
#endif

    uint32_t writeValue = regAccess->regValue.U;

    /* Write enable */
    status = mtu_mixspi_nor_write_enable(userConfig, 0, kFlashInstMode_SPI);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = 0;
    flashXfer.port          = userConfig->mixspiPort;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = regAccess->regSeqIdx;
    flashXfer.data          = &writeValue;
    flashXfer.dataSize      = regAccess->regNum;

    status = FLEXSPI_TransferBlocking(userConfig->mixspiBase, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = mtu_mixspi_nor_wait_bus_busy(userConfig, kFlashInstMode_SPI);
    bsp_mixspi_sw_delay_us(100000UL);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(userConfig->mixspiBase);

#if defined(MTU_CACHE_MAINTAIN) && MTU_CACHE_MAINTAIN
    mtu_mixspi_nor_enable_cache(cacheStatus);
#endif

    return status;
}

status_t mtu_mixspi_nor_read_register(mixspi_user_config_t *userConfig, flash_reg_access_t *regAccess)
{
    uint32_t regVal = 0;
    flexspi_transfer_t flashXfer;
    flashXfer.deviceAddress = regAccess->regAddr;
    flashXfer.port          = userConfig->mixspiPort;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = regAccess->regSeqIdx;
    flashXfer.data          = &regVal;
    flashXfer.dataSize      = regAccess->regNum;

    status_t status = FLEXSPI_TransferBlocking(userConfig->mixspiBase, &flashXfer);

    /* Do software reset or clear AHB buffer directly. */
#if defined(FSL_FEATURE_SOC_OTFAD_COUNT) && defined(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK) && \
    defined(FLEXSPI_AHBCR_CLRAHBTXBUF_MASK)
    userConfig->mixspiBase->AHBCR |= FLEXSPI_AHBCR_CLRAHBRXBUF_MASK | FLEXSPI_AHBCR_CLRAHBTXBUF_MASK;
    userConfig->mixspiBase->AHBCR &= ~(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK | FLEXSPI_AHBCR_CLRAHBTXBUF_MASK);
#else
    FLEXSPI_SoftwareReset(userConfig->mixspiBase);
#endif
    
    regAccess->regValue.U = regVal;

    return status;
}

status_t mtu_mixspi_nor_enable_quad_mode(mixspi_user_config_t *userConfig)
{
    if (s_configSystemPacket.memProperty.flashQuadEnableBytes)
    {
        flash_reg_access_t regAccess;
        regAccess.regNum = s_configSystemPacket.memProperty.flashQuadEnableBytes;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_ENABLEQE;
        regAccess.regValue.U = s_configSystemPacket.memProperty.flashQuadEnableCfg;
        return mtu_mixspi_nor_write_register(userConfig, &regAccess);
    }
    else
    {
        return kStatus_Success;
    }
}

status_t mtu_mixspi_nor_get_jedec_id(mixspi_user_config_t *userConfig, uint32_t *vendorId)
{
    flexspi_transfer_t flashXfer;
    flashXfer.deviceAddress = 0;
    flashXfer.port          = userConfig->mixspiPort;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_READID;
    flashXfer.data          = vendorId;
    flashXfer.dataSize      = 4;

    status_t status = FLEXSPI_TransferBlocking(userConfig->mixspiBase, &flashXfer);

    /* Do software reset or clear AHB buffer directly. */
#if defined(FSL_FEATURE_SOC_OTFAD_COUNT) && defined(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK) && \
    defined(FLEXSPI_AHBCR_CLRAHBTXBUF_MASK)
    userConfig->mixspiBase->AHBCR |= FLEXSPI_AHBCR_CLRAHBRXBUF_MASK | FLEXSPI_AHBCR_CLRAHBTXBUF_MASK;
    userConfig->mixspiBase->AHBCR &= ~(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK | FLEXSPI_AHBCR_CLRAHBTXBUF_MASK);
#else
    FLEXSPI_SoftwareReset(userConfig->mixspiBase);
#endif

    return status;
}

status_t mtu_mixspi_nor_erase_sector(mixspi_user_config_t *userConfig, uint32_t address, flash_inst_mode_t flashInstMode)
{
    status_t status;
    flexspi_transfer_t flashXfer;

#if defined(MTU_CACHE_MAINTAIN) && MTU_CACHE_MAINTAIN
    mixspi_cache_status_t cacheStatus;
    mtu_mixspi_nor_disable_cache(&cacheStatus);
#endif

    /* Write enable */
    status = mtu_mixspi_nor_write_enable(userConfig, address, kFlashInstMode_SPI);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = address;
    flashXfer.port          = userConfig->mixspiPort;
    flashXfer.cmdType       = kFLEXSPI_Command;
    if (flashInstMode == kFlashInstMode_Hyper)
    {
        flashXfer.SeqNumber = 4;
    }
    else
    {
        flashXfer.SeqNumber = 1;
    }
    switch (flashInstMode)
    {
        case kFlashInstMode_QPI_1:
        case kFlashInstMode_QPI_2:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_QPI;
            break;

        case kFlashInstMode_OPI:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_OPI;
            break;

        case kFlashInstMode_SPI:
        case kFlashInstMode_Hyper:
        default:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR;
            break;
    }

    status = FLEXSPI_TransferBlocking(userConfig->mixspiBase, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = mtu_mixspi_nor_wait_bus_busy(userConfig, flashInstMode);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(userConfig->mixspiBase);

#if defined(MTU_CACHE_MAINTAIN) && MTU_CACHE_MAINTAIN
    mtu_mixspi_nor_enable_cache(cacheStatus);
#endif

    return status;
}

status_t mtu_mixspi_nor_page_program(mixspi_user_config_t *userConfig,
                                     flexspi_device_config_t *deviceconfig,
                                     uint32_t address,
                                     const uint32_t *src,
                                     uint32_t length,
                                     flash_inst_mode_t flashInstMode)
{
    status_t status;
    flexspi_transfer_t flashXfer;

#if defined(MTU_CACHE_MAINTAIN) && MTU_CACHE_MAINTAIN
    mixspi_cache_status_t cacheStatus;
    mtu_mixspi_nor_disable_cache(&cacheStatus);
#endif

    mixspi_root_clk_freq_t lastRootClkFreq;
    if (flashInstMode == kFlashInstMode_Hyper)
    {
        /* Speed down flexspi clock, beacuse 50 MHz timings are only relevant when a burst write is used to load data during
         * a HyperFlash Word Program command. */
        lastRootClkFreq = userConfig->mixspiRootClkFreq;
        userConfig->mixspiRootClkFreq = kMixspiRootClkFreq_50MHz;
        bsp_mixspi_clock_init(userConfig);

        /* Get current flexspi root clock. */
        deviceconfig->flexspiRootClk = bsp_mixspi_get_clock(userConfig);

        /* Update DLL value depending on flexspi root clock. */
        FLEXSPI_UpdateDllValue(userConfig->mixspiBase, deviceconfig, userConfig->mixspiPort);

        /* Do software reset. */
        FLEXSPI_SoftwareReset(userConfig->mixspiBase);
    }

    /* Write enable */
    status = mtu_mixspi_nor_write_enable(userConfig, address, flashInstMode);

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress = address;
    flashXfer.port          = userConfig->mixspiPort;
    flashXfer.cmdType       = kFLEXSPI_Write;
    if (flashInstMode == kFlashInstMode_Hyper)
    {
        flashXfer.SeqNumber = 2;
    }
    else
    {
        flashXfer.SeqNumber = 1;
    }
    switch (flashInstMode)
    {
        case kFlashInstMode_QPI_1:
        case kFlashInstMode_QPI_2:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QPI;
            break;

        case kFlashInstMode_OPI:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI;
            break;

        case kFlashInstMode_SPI:
        case kFlashInstMode_Hyper:
        default:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM;
            break;
    }
    flashXfer.data          = (uint32_t *)src;
    flashXfer.dataSize      = length;
    status                  = FLEXSPI_TransferBlocking(userConfig->mixspiBase, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = mtu_mixspi_nor_wait_bus_busy(userConfig, flashInstMode);

    if (flashInstMode == kFlashInstMode_Hyper)
    {
        /* Speed up flexspi clock for a high read performance. */
        userConfig->mixspiRootClkFreq = lastRootClkFreq;
        bsp_mixspi_clock_init(userConfig);

        /* Get current flexspi root clock. */
        deviceconfig->flexspiRootClk = bsp_mixspi_get_clock(userConfig);

        /* Update DLL value depending on flexspi root clock. */
        FLEXSPI_UpdateDllValue(userConfig->mixspiBase, deviceconfig, userConfig->mixspiPort);
    }

    /* Do software reset or clear AHB buffer directly. */
#if defined(FSL_FEATURE_SOC_OTFAD_COUNT) && defined(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK) && \
    defined(FLEXSPI_AHBCR_CLRAHBTXBUF_MASK)
    userConfig->mixspiBase->AHBCR |= FLEXSPI_AHBCR_CLRAHBRXBUF_MASK | FLEXSPI_AHBCR_CLRAHBTXBUF_MASK;
    userConfig->mixspiBase->AHBCR &= ~(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK | FLEXSPI_AHBCR_CLRAHBTXBUF_MASK);
#else
    FLEXSPI_SoftwareReset(userConfig->mixspiBase);
#endif

#if defined(MTU_CACHE_MAINTAIN) && MTU_CACHE_MAINTAIN
    mtu_mixspi_nor_enable_cache(cacheStatus);
#endif

    return status;
}

void mtu_mixspi_mem_init(mixspi_user_config_t *userConfig, 
                               flexspi_device_config_t *deviceconfig)
{
    flexspi_config_t config;

    bsp_mixspi_clock_init(userConfig);
    bsp_mixspi_clock_source(userConfig);

    /*Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus. */
    config.ahbConfig.enableAHBPrefetch    = true;
    config.ahbConfig.enableAHBBufferable  = true;
    config.ahbConfig.enableReadAddressOpt = true;
    config.ahbConfig.enableAHBCachable    = true;
    config.rxSampleClock                  = userConfig->mixspiReadSampleClock;
#if !(defined(FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_COMBINATIONEN) && FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_COMBINATIONEN)
    flexspi_port_t port = userConfig->mixspiPort;
    if ((port == kFLEXSPI_PortA1) || (port == kFLEXSPI_PortA2))
    {
        config.enableCombination = true;
    }
    else
    {
        config.enableCombination = false;
    }
#endif
    FLEXSPI_Init(userConfig->mixspiBase, &config);

#if defined(FLEXSPI_AHBCR_ALIGNMENT_MASK)
    /* Set alignment, otherwise the prefetch burst may cross die boundary. */
    userConfig->mixspiBase->AHBCR &= ~FLEXSPI_AHBCR_ALIGNMENT_MASK;
    userConfig->mixspiBase->AHBCR |= FLEXSPI_AHBCR_ALIGNMENT(1);/* 1KB */
#endif

    /* Configure flash settings according to serial flash feature. */
    FLEXSPI_SetFlashConfig(userConfig->mixspiBase, deviceconfig, userConfig->mixspiPort);

    /* Update LUT table. */
    FLEXSPI_UpdateLUT(userConfig->mixspiBase, 0, userConfig->mixspiCustomLUTVendor, CUSTOM_LUT_LENGTH);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(userConfig->mixspiBase);
}
