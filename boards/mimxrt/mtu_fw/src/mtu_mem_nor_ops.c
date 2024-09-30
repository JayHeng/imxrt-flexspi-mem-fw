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
    flash_reg_access_t regAccess;
    regAccess.regNum = s_configSystemPacket.memProperty.flashQuadEnableBytes;
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_ENABLEQE;
    regAccess.regValue.U = s_configSystemPacket.memProperty.flashQuadEnableCfg;
    return mtu_mixspi_nor_write_register(userConfig, &regAccess);
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

void mtu_mixspi_nor_flash_init(mixspi_user_config_t *userConfig, flexspi_device_config_t *deviceconfig)
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
    config.rxSampleClock                  = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
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

    /* Configure flash settings according to serial flash feature. */
    FLEXSPI_SetFlashConfig(userConfig->mixspiBase, deviceconfig, userConfig->mixspiPort);

    /* Update LUT table. */
    FLEXSPI_UpdateLUT(userConfig->mixspiBase, 0, userConfig->mixspiCustomLUTVendor, CUSTOM_LUT_LENGTH);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(userConfig->mixspiBase);
}
