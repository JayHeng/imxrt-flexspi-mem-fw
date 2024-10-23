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

uint32_t s_memRwBuffer[0x200/4];

/* Common FlexSPI config */
flexspi_device_config_t s_nordeviceconfig = {
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

flexspi_device_config_t s_ramdeviceconfig = {
    .flexspiRootClk       = 396000000, /* 396MHZ SPI serial clock, DDR serial clock 198M */
    .isSck2Enabled        = false,
    .flashSize            = 0x2000, /* 64Mb/KByte */
    .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval           = 2,
    .CSHoldTime           = 3,
    .CSSetupTime          = 3,
    .dataValidTime        = 1,
    .columnspace          = 0,
    .enableWordAddress    = false,
    .AWRSeqIndex          = PSRAM_CMD_LUT_SEQ_IDX_WRITEDATA,
    .AWRSeqNumber         = 1,
    .ARDSeqIndex          = PSRAM_CMD_LUT_SEQ_IDX_READDATA,
    .ARDSeqNumber         = 1,
    .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
    .enableWriteMask      = true,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t mtu_memory_init(void)
{
    status_t status;
    uint32_t jedecID = 0;
    if (s_configSystemPacket.memProperty.type == kMemType_InternalSRAM)
    {
        printf("Internal SRAM does not need to be configured.\r\n");
        return kStatus_Success;
    }
    
    s_userConfig.mixspiRootClkFreq = mtu_flash_convert_root_clk(s_configSystemPacket.memProperty.speedMHz);
    
    //s_userConfig.mixspiBase = FLEXSPI1;
    //s_userConfig.mixspiPort = kFLEXSPI_PortA1;
    memcpy(s_customLUTCommonMode, s_configSystemPacket.memProperty.memLut, CUSTOM_LUT_LENGTH * 4);
    s_userConfig.mixspiCustomLUTVendor = s_customLUTCommonMode;
    if (s_configSystemPacket.memProperty.type <= kMemType_FlashMaxIdx)
    {
        s_userConfig.mixspiReadSampleClock = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
        mtu_mixspi_mem_init(&s_userConfig, &s_nordeviceconfig);
    }
    else
    {
        s_userConfig.mixspiReadSampleClock = kFLEXSPI_ReadSampleClkExternalInputFromDqsPad;
        mtu_mixspi_mem_init(&s_userConfig, &s_ramdeviceconfig);
    }

    printf("FLEXSPI%d module initialized.\r\n", s_userConfig.instance);

    if (s_configSystemPacket.memProperty.type == kMemType_QuadSPI)
    {
        /* Get JEDEC ID. */
        status = mtu_mixspi_nor_get_jedec_id(&s_userConfig, &jedecID);
        if (status != kStatus_Success)
        {
            return status;
        }
        mtu_flash_validate_jedec_id((jedec_id_t *)&jedecID);

        status = mtu_mixspi_nor_enable_quad_mode(&s_userConfig);
        if (status != kStatus_Success)
        {
            printf("Flash failed to enter Quad I/O SDR mode.\r\n");
            return status;
        }
        printf("Flash entered Quad I/O SDR mode.\r\n");
    }
    else if (s_configSystemPacket.memProperty.type > kMemType_FlashMaxIdx)
    {
        status = mtu_psram_set_registers_for_apmemory(&s_userConfig);
        //status = mtu_psram_set_registers_for_issi(&s_userConfig);
        if (status != kStatus_Success)
        {
            printf("Ram failed to be configured.\r\n");
            return status;
        }
        printf("Ram is configured properly.\r\n");
    }
    
    return kStatus_Success;
}

status_t mtu_memory_get_info(void)
{
    if (s_configSystemPacket.memProperty.type == kMemType_InternalSRAM)
    {
        bsp_rt_system_srams_print();
        return kStatus_Success;
    }

    s_userConfig.flashBusyStatusOffset = 0;
    s_userConfig.flashBusyStatusPol = 1;

    flash_reg_access_t regAccess;
    regAccess.regNum = 1;
    regAccess.regAddr = 0x0;
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READSTATUS;
    mtu_mixspi_nor_read_register(&s_userConfig, &regAccess);
    printf("Flash Status Register: 0x%x\r\n", regAccess.regValue.B.reg1);
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG1;
    mtu_mixspi_nor_read_register(&s_userConfig, &regAccess);
    printf("Flash Custom Register1: 0x%x\r\n", regAccess.regValue.B.reg1);
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG2;
    mtu_mixspi_nor_read_register(&s_userConfig, &regAccess);
    printf("Flash Custom Register2: 0x%x\r\n", regAccess.regValue.B.reg1);
    
    return kStatus_Success;
}

static void mtu_memory_preset_rw_buffer(uint32_t patternWord)
{
    for (uint32_t i = 0; i < sizeof(s_memRwBuffer) / sizeof(uint32_t); i++)
    {
        s_memRwBuffer[i] = patternWord;
    }
}

static uint32_t mtu_memory_convert_to_offset_addr(uint32_t memStart)
{
    uint32_t offsetAddr = memStart;
    uint32_t ambaBase = bsp_mixspi_get_amba_base(&s_userConfig);
    if ((memStart >= ambaBase) && (memStart < ambaBase + MTU_MEM_MAX_MAP_SIZE))
    {
        offsetAddr -= ambaBase;
    }
    return offsetAddr;
}

status_t mtu_memory_rwtest(uint8_t memType, uint32_t memStart, uint32_t memSize, uint32_t memPattern)
{
    printf("Arg List: memStart=0x%x, memSize=0x%x, memPattern=0x%x.\n", memStart, memSize, memPattern);
    uint32_t memEnd = memStart + memSize;
    if (memType > kMemType_FlashMaxIdx)
    {
        for (uint32_t addr = memStart; addr < memEnd;)
        {
            *(uint32_t *)addr = memPattern;
            addr += 4;
        }
    }
    else
    {
        uint32_t sectorMax = memSize / 0x1000;
        uint32_t pagesPerSector = 0x1000 / 0x100;
        uint32_t offsetAddr = mtu_memory_convert_to_offset_addr(memStart);
        for (uint32_t sectorId = 0; sectorId < sectorMax; sectorId++)
        {
            uint32_t sectorAddr = offsetAddr + sectorId * 0x1000;
            status_t status = mtu_mixspi_nor_erase_sector(&s_userConfig, sectorAddr, kFlashInstMode_SPI);
            if (status != kStatus_Success)
            {
                printf("Erase flash sector failure at address 0x%x!\r\n", sectorAddr);
                return kStatus_Fail;
            }
            mtu_memory_preset_rw_buffer(memPattern);
            for (uint32_t pageId = 0; pageId < pagesPerSector; pageId++)
            {
                uint32_t pageAddr = sectorAddr + pageId * 0x100;
                status = mtu_mixspi_nor_page_program(&s_userConfig, &s_nordeviceconfig, pageAddr, (const uint32_t *)s_memRwBuffer, 0x100, kFlashInstMode_SPI);
                if (status != kStatus_Success)
                {
                    printf("Program flash page failure at address 0x%x!\r\n", pageAddr);
                    return kStatus_Fail;
                }
            }
        }
        if (offsetAddr == memStart)
        {
            memStart += bsp_mixspi_get_amba_base(&s_userConfig);
            memEnd += bsp_mixspi_get_amba_base(&s_userConfig);
        }
    }
    printf("Pattern 0x%x has been filled into MEM region [0x%x - 0x%x)\n", memPattern, memStart, memStart + memSize);
    for (uint32_t addr = memStart; addr < memEnd;)
    {
        if (*(uint32_t *)addr != memPattern)
        {
            printf("Pattern 0x%x verification is failed at address 0x%x.\n", memPattern, addr);
            return kStatus_Fail;
        }
        addr += 4;
    }

    printf("Pattern 0x%x readback verification is passed.\n", memPattern);
    return kStatus_Success;
}


