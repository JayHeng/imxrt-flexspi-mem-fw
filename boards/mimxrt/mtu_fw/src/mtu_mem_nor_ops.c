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

void mtu_mixspi_nor_flash_init(mixspi_user_config_t *userConfig, flexspi_device_config_t *deviceconfig)
{
    flexspi_config_t config;

    bsp_flexspi_clock_init();

    /*Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus. */
    config.ahbConfig.enableAHBPrefetch    = true;
    config.ahbConfig.enableAHBBufferable  = true;
    config.ahbConfig.enableReadAddressOpt = true;
    config.ahbConfig.enableAHBCachable    = true;
    config.rxSampleClock                  = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
    FLEXSPI_Init(userConfig->mixspiBase, &config);

    /* Configure flash settings according to serial flash feature. */
    FLEXSPI_SetFlashConfig(userConfig->mixspiBase, deviceconfig, userConfig->mixspiPort);

    /* Update LUT table. */
    FLEXSPI_UpdateLUT(userConfig->mixspiBase, 0, userConfig->mixspiCustomLUTVendor, CUSTOM_LUT_LENGTH);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(userConfig->mixspiBase);
}
