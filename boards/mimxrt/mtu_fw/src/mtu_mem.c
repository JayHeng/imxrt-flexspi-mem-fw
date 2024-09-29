/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mtu_mem.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

mixspi_user_config_t s_userConfig;

/* Common FlexSPI config */
flexspi_device_config_t s_deviceconfig = {
    .flexspiRootClk       = 30000000,
    .flashSize            = 0x4000, /* 128Mb/KByte */
    .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval           = 2,
    .CSHoldTime           = 3,
    .CSSetupTime          = 3,
    .dataValidTime        = 2,
    .columnspace          = 0,
    .enableWordAddress    = 0,
    .AWRSeqIndex          = NOR_CMD_LUT_SEQ_IDX_WRITE,
    .AWRSeqNumber         = 1,
    .ARDSeqIndex          = NOR_CMD_LUT_SEQ_IDX_READ,
    .ARDSeqNumber         = 1,
    .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
};

/* Common FlexSPI LUT */
uint32_t s_customLUTCommonMode[CUSTOM_LUT_LENGTH] = {
    /*  Normal read */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x03, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Read status register */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x04),

    /* Write Enable */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0X00),

    /* Read ID */
    [4 * NOR_CMD_LUT_SEQ_IDX_READID] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x9F, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x04),

    /* Read ID - QPI_1 */
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_QPI_1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_4PAD, 0xAF, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_4PAD, 0x04),

    /* Read ID - QPI_2 */
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_QPI_2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_4PAD, 0x9F, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_4PAD, 0x04),

    /* Read ID - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_OPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_8PAD, 0x9F, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 0x06),
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_OPI + 1] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR,  kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Dummy write, do nothing when AHB write command is triggered. */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Erase Sector */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),

    /* Page Program - single mode */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Read function register */
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x48, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x01),

    /* Read read parameters */
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x61, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x01),
};

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t mtu_memory_init(void)
{
    status_t status;
    uint32_t jedecID = 0;
    
    //s_userConfig.mixspiBase = FLEXSPI1;
    //s_userConfig.mixspiPort = kFLEXSPI_PortA1;
    memcpy(&s_customLUTCommonMode[4*NOR_CMD_LUT_SEQ_IDX_READREG1], &s_configSystemPacket.memLut[4*NOR_CMD_LUT_SEQ_IDX_READREG1], LUT_SEQUENCE_LENGTH);
    memcpy(&s_customLUTCommonMode[4*NOR_CMD_LUT_SEQ_IDX_READREG2], &s_configSystemPacket.memLut[4*NOR_CMD_LUT_SEQ_IDX_READREG2], LUT_SEQUENCE_LENGTH);
    s_userConfig.mixspiCustomLUTVendor = s_customLUTCommonMode;
    s_userConfig.mixspiReadSampleClock = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;

    mtu_mixspi_nor_flash_init(&s_userConfig, &s_deviceconfig);

    printf("--FLEXSPI%d module initialized.\r\n", s_userConfig.instance);

    /* Get JEDEC ID. */
    status = mtu_mixspi_nor_get_jedec_id(&s_userConfig, &jedecID);
    if (status != kStatus_Success)
    {
        return status;
    }
    mtu_flash_validate_jedec_id((jedec_id_t *)&jedecID);
    
    return kStatus_Success;
}

status_t mtu_memory_get_info(void)
{
    s_userConfig.flashBusyStatusOffset = 0;
    s_userConfig.flashBusyStatusPol = 1;

    flash_reg_access_t regAccess;
    regAccess.regNum = 1;
    regAccess.regAddr = 0x0;
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READSTATUS;
    mtu_mixspi_nor_read_register(&s_userConfig, &regAccess);
    printf("--Flash Status Register: 0x%x\r\n", regAccess.regValue.B.reg1);
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG1;
    mtu_mixspi_nor_read_register(&s_userConfig, &regAccess);
    printf("--Flash Custom Register1: 0x%x\r\n", regAccess.regValue.B.reg1);
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG2;
    mtu_mixspi_nor_read_register(&s_userConfig, &regAccess);
    printf("--Flash Custom Register2: 0x%x\r\n", regAccess.regValue.B.reg1);
    
    return kStatus_Success;
}

status_t mtu_memory_rwtest(void)
{
    return kStatus_Success;
}


