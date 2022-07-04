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
extern const uint32_t customLUT[28];
extern const uint32_t customLUT_ISSI[28];
/*******************************************************************************
 * Code
 ******************************************************************************/
status_t flexspi_psram_write_mcr(FLEXSPI_Type *base, uint8_t regAddr, uint32_t *mrVal)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write register */
    flashXfer.deviceAddress = regAddr;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = PSRAM_CMD_LUT_SEQ_IDX_WRITEREG;
    flashXfer.data          = mrVal;
    flashXfer.dataSize      = 1;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t flexspi_psram_get_mcr(FLEXSPI_Type *base, uint8_t regAddr, uint32_t *mrVal)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Read register */
    flashXfer.deviceAddress = regAddr;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = PSRAM_CMD_LUT_SEQ_IDX_READREG;
    flashXfer.data          = mrVal;
    flashXfer.dataSize      = 2;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t flexspi_psram_ipcommand_read_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t size)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Read data */
    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = PSRAM_CMD_LUT_SEQ_IDX_READDATA;
    flashXfer.data          = buffer;
    flashXfer.dataSize      = size;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

void flexspi_psram_ahbcommand_read_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t size)
{
    uint32_t *startAddr = (uint32_t *)(EXAMPLE_FLEXSPI_AMBA_BASE + address);
    memcpy(buffer, startAddr, size);
}

status_t flexspi_psram_ipcommand_write_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t size)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write data */
    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = PSRAM_CMD_LUT_SEQ_IDX_WRITEDATA;
    flashXfer.data          = buffer;
    flashXfer.dataSize      = size;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

void flexspi_psram_ahbcommand_write_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t size)
{
    uint32_t *startAddr = (uint32_t *)(EXAMPLE_FLEXSPI_AMBA_BASE + address);
    memcpy(startAddr, buffer, size);
}

status_t flexspi_psram_reset(FLEXSPI_Type *base)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write data */
    flashXfer.deviceAddress = 0x0U;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = PSRAM_CMD_LUT_SEQ_IDX_RESET;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status == kStatus_Success)
    {
        /* for loop of 150000 is about 1ms (@600 MHz CPU) */
        for (uint32_t i = 10000000U; i > 0; i--)
        {
            __NOP();
        }
    }
    return status;
}

void flexspi_psram_init(FLEXSPI_Type *base)
{
    flexspi_config_t config;
    status_t status;

    /*Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus. */
    config.enableCombination              = true;
    config.ahbConfig.enableAHBPrefetch    = true;
    config.ahbConfig.enableAHBBufferable  = true;
    config.ahbConfig.enableReadAddressOpt = true;  //for psram, must set it to 1
    config.ahbConfig.enableAHBCachable    = true;
    config.rxSampleClock                  = kFLEXSPI_ReadSampleClkExternalInputFromDqsPad;
    
    FLEXSPI_Init(base, &config);

    /* Configure flash settings according to serial flash feature. */
    FLEXSPI_SetFlashConfig(base, &deviceconfig, kFLEXSPI_PortA1);

    /* Update LUT table. */
    //Apmemory
    FLEXSPI_UpdateLUT(base, 0, customLUT, ARRAY_SIZE(customLUT));
    //ISSI
    //FLEXSPI_UpdateLUT(base, 0, customLUT_ISSI, ARRAY_SIZE(customLUT_ISSI));

    /* Do software reset. */
    FLEXSPI_SoftwareReset(base);
#if 1    //for Apmemory PSRAM, set it to 1
    /* Reset psram. */
    status = flexspi_psram_reset(base);
    if (status != kStatus_Success)
    {
        while(1);
    }
#endif
}
