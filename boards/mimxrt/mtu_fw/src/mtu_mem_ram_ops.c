/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mtu_mem_ram_ops.h"

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

status_t mtu_mixspi_psram_write_register(mixspi_user_config_t *userConfig, psram_reg_access_t *regAccess)
{
    flexspi_transfer_t flashXfer;
    status_t status;
    uint32_t writeValue = regAccess->regValue.U;

    /* Write data */
    flashXfer.deviceAddress = regAccess->regAddr;
    flashXfer.port          = userConfig->mixspiPort;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = PSRAM_CMD_LUT_SEQ_IDX_WRITEREG;
    flashXfer.data          = &writeValue;
    flashXfer.dataSize      = regAccess->regNum;

    status = FLEXSPI_TransferBlocking(userConfig->mixspiBase, &flashXfer);

    return status;
}

status_t mtu_mixspi_psram_read_register(mixspi_user_config_t *userConfig, psram_reg_access_t *regAccess)
{
    flexspi_transfer_t flashXfer;
    status_t status;
    uint32_t regVal = 0;

    /* Read data */
    flashXfer.deviceAddress = regAccess->regAddr;
    flashXfer.port          = userConfig->mixspiPort;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = PSRAM_CMD_LUT_SEQ_IDX_READREG;
    flashXfer.data          = &regVal;
    flashXfer.dataSize      = regAccess->regNum;

    status = FLEXSPI_TransferBlocking(userConfig->mixspiBase, &flashXfer);

    regAccess->regValue.U = regVal;

    return status;
}

status_t mtu_mixspi_psram_read_id(mixspi_user_config_t *userConfig, uint16_t *buffer)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write data */
    flashXfer.deviceAddress = 0x00;
    flashXfer.port          = userConfig->mixspiPort;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = PSRAM_CMD_LUT_SEQ_IDX_READID;
    flashXfer.data          = (uint32_t *)(void *)buffer;
    flashXfer.dataSize      = 2;

    status = FLEXSPI_TransferBlocking(userConfig->mixspiBase, &flashXfer);

    return status;
}

status_t mtu_mixspi_psram_reset(mixspi_user_config_t *userConfig)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write data */
    flashXfer.deviceAddress = 0x0U;
    flashXfer.port          = userConfig->mixspiPort;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = PSRAM_CMD_LUT_SEQ_IDX_RESET;

    status = FLEXSPI_TransferBlocking(userConfig->mixspiBase, &flashXfer);

    if (status == kStatus_Success)
    {
        /* for loop of 50000 is about 1ms (@200 MHz CPU) */
        for (uint32_t i = 2000000U; i > 0; i--)
        {
            __NOP();
        }
    }
    return status;
}

status_t mixspi_psram_ipg_write_data(mixspi_user_config_t *userConfig, uint32_t address, uint32_t *buffer, uint32_t length)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write data */
    flashXfer.deviceAddress = address;
    flashXfer.port          = userConfig->mixspiPort;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = PSRAM_CMD_LUT_SEQ_IDX_WRITEDATA;
    flashXfer.data          = buffer;
    flashXfer.dataSize      = length;

    status = FLEXSPI_TransferBlocking(userConfig->mixspiBase, &flashXfer);

    return status;
}

status_t mixspi_psram_ipg_read_data(mixspi_user_config_t *userConfig, uint32_t address, uint32_t *buffer, uint32_t length)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Read data */
    flashXfer.deviceAddress = address;
    flashXfer.port          = userConfig->mixspiPort;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = PSRAM_CMD_LUT_SEQ_IDX_READDATA;
    flashXfer.data          = buffer;
    flashXfer.dataSize      = length;

    status = FLEXSPI_TransferBlocking(userConfig->mixspiBase, &flashXfer);

    return status;
}

void mixspi_psram_ahb_write_data(mixspi_user_config_t *userConfig, uint32_t address, uint32_t *buffer, uint32_t length)
{
    uint32_t *startAddr = (uint32_t *)(bsp_mixspi_get_amba_base(userConfig) + address);
    memcpy(startAddr, buffer, length);
}

void mixspi_psram_ahb_read_data(mixspi_user_config_t *userConfig, uint32_t address, uint32_t *buffer, uint32_t length)
{
    uint32_t *startAddr = (uint32_t *)(bsp_mixspi_get_amba_base(userConfig) + address);
    memcpy(buffer, startAddr, length);
}