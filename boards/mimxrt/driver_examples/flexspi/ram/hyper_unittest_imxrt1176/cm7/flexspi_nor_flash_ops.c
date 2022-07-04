/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 *****************************************************************************/
extern flexspi_device_config_t deviceconfig;
extern const uint32_t customLUT_winbond[20]; 
extern const uint32_t customLUT_issi[20]; 
/*******************************************************************************
 * Code
 ******************************************************************************/
status_t flexspi_hyper_ram_write_regCR0(FLEXSPI_Type *base, uint32_t *buffer)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write register */
    flashXfer.deviceAddress = 0x1000U;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Write;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG;
    flashXfer.data = buffer;
    flashXfer.dataSize = 2;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t flexspi_hyper_ram_get_regCR0(FLEXSPI_Type *base, uint32_t *reg)
{
    flexspi_transfer_t flashXfer;
    status_t status;
    uint32_t val;

    /* Write data */
    flashXfer.deviceAddress = 0x1000U;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = HYPERRAM_CMD_LUT_SEQ_IDX_READREG;
    flashXfer.data = &val;
    flashXfer.dataSize = 2;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    *reg = val & 0xffffU;

    return status;
}

status_t flexspi_hyper_ram_ipcommand_write_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write data */
    flashXfer.deviceAddress = address;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Write;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA;
    flashXfer.data = buffer;
    flashXfer.dataSize = length;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

void flexspi_hyper_ram_ahbcommand_write_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    uint32_t *startAddr = (uint32_t *)(EXAMPLE_FLEXSPI_AMBA_BASE + address);
    memcpy(startAddr, buffer, length);

    /* Delay some time for tx buffer data finished. */
//    uint32_t i = 1000;
//    while (i--)
//    {
//        __NOP();
//    }
}

status_t flexspi_hyper_ram_ipcommand_read_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write data */
    flashXfer.deviceAddress = address;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = HYPERRAM_CMD_LUT_SEQ_IDX_READDATA;
    flashXfer.data = buffer;
    flashXfer.dataSize = length;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

void flexspi_hyper_ram_ahbcommand_read_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    uint32_t *startAddr = (uint32_t *)(EXAMPLE_FLEXSPI_AMBA_BASE + address);
    memcpy(buffer, startAddr, length);
}

status_t flexspi_hyper_ram_get_id(FLEXSPI_Type *base, uint32_t *vendorId)
{
    flexspi_transfer_t flashXfer;
    status_t status;
    uint32_t id;

    /* Write data */
    flashXfer.deviceAddress = 0x0U;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = HYPERRAM_CMD_LUT_SEQ_IDX_READREG;
    flashXfer.data = &id;
    flashXfer.dataSize = 4;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    *vendorId = id & 0xffffU;

    return status;
}

void flexspi_nor_flash_init(FLEXSPI_Type *base)
{
    flexspi_config_t config;

    //flexspi_clock_init();

    /*Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus. */
    config.rxSampleClock                  = kFLEXSPI_ReadSampleClkExternalInputFromDqsPad;
    config.enableCombination              = true;
    config.ahbConfig.enableAHBPrefetch    = true;
    config.ahbConfig.enableAHBBufferable  = true;
    config.ahbConfig.enableReadAddressOpt = true;
    config.ahbConfig.enableAHBCachable    = true;
    
    //split function test
//    config.ahbConfig.buffer[0].enablePrefetch = true;
//    config.ahbConfig.buffer[0].masterIndex    = 0;      /* 0: buffer0 for m7 core */
//    config.ahbConfig.buffer[0].bufferSize     = 256;
//    config.ahbConfig.buffer[0].priority       = 7;      /*  7 is the highest priority, 0 the lowest. */

    FLEXSPI_Init(base, &config);

    /* Configure flash settings according to serial flash feature. */
    FLEXSPI_SetFlashConfig(base, &deviceconfig, kFLEXSPI_PortA1);

    /* Update LUT table. */
    FLEXSPI_UpdateLUT(base, 0, customLUT_winbond, ARRAY_SIZE(customLUT_winbond)); 

    /* Do software reset. */
    FLEXSPI_SoftwareReset(base);
}
