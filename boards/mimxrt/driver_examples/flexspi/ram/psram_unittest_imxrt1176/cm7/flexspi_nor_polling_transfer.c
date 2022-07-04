/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"

#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "memtester.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DRAM_SIZE        0x1000000U   //16MBytes, Ap
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
typedef struct _hyperram_test_config {
  uint32_t base_addr;
  uint32_t test_size;
  uint32_t loop_num;
  uint32_t freq;
  uint32_t fail_stop;
  uint32_t enable_cache;
} hyperram_test_config;

hyperram_test_config test_config;
int fail_stop;
/*******************************************************************************
 * Variables
 ******************************************************************************/

extern status_t flexspi_psram_write_mcr(FLEXSPI_Type *base, uint8_t regAddr, uint32_t *mrVal);
extern status_t flexspi_psram_get_mcr(FLEXSPI_Type *base, uint8_t regAddr, uint32_t *mrVal);
extern status_t flexspi_psram_ipcommand_read_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t size);
extern status_t flexspi_psram_ipcommand_write_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t size);
extern void flexspi_psram_ahbcommand_read_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t size);
extern void flexspi_psram_ahbcommand_write_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t size);
extern status_t flexspi_psram_reset(FLEXSPI_Type *base);
extern void flexspi_psram_init(FLEXSPI_Type *base);

static uint8_t s_psram_write_buffer[1024];
static uint8_t s_psram_read_buffer[1024];
/*******************************************************************************
 * Code
 ******************************************************************************/
flexspi_device_config_t deviceconfig = {
    .flexspiRootClk       = 332000000,
    .isSck2Enabled        = false,
    .flashSize            = 0x4000, /* 128Mb=16MB=16*1024KByte */ //Ap
    .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval           = 2,
    .CSHoldTime           = 3,
    .CSSetupTime          = 3,
    .dataValidTime        = 1,
    .columnspace          = 0,  //Ap
    .enableWordAddress    = false,  //for psram, must set it to 0
    .AWRSeqIndex          = PSRAM_CMD_LUT_SEQ_IDX_WRITEDATA,
    .AWRSeqNumber         = 1,
    .ARDSeqIndex          = PSRAM_CMD_LUT_SEQ_IDX_READDATA,
    .ARDSeqNumber         = 1,
    .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
    .enableWriteMask      = true,  //for psram, must set it to 1
};

const uint32_t customLUT[28] = {
    /* Data read - Linear burst */
    [4 * PSRAM_CMD_LUT_SEQ_IDX_READDATA] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * PSRAM_CMD_LUT_SEQ_IDX_READDATA + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x07, kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04),

    /* Data write - Linear burst */
    [4 * PSRAM_CMD_LUT_SEQ_IDX_WRITEDATA] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0xA0, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * PSRAM_CMD_LUT_SEQ_IDX_WRITEDATA + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x07, kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04),

    /* Mode register read */
    [4 * PSRAM_CMD_LUT_SEQ_IDX_READREG] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x40, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * PSRAM_CMD_LUT_SEQ_IDX_READREG + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x07, kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04),

    /* Mode register write  */
    [4 * PSRAM_CMD_LUT_SEQ_IDX_WRITEREG] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0xC0, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * PSRAM_CMD_LUT_SEQ_IDX_WRITEREG + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),
    
    /* Reset */
    [4 * PSRAM_CMD_LUT_SEQ_IDX_RESET] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0xFF, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_8PAD, 0x03),
        
    /* Data read - Normal */
    [4 * PSRAM_CMD_LUT_SEQ_IDX_READDATA_NORMAL] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * PSRAM_CMD_LUT_SEQ_IDX_READDATA_NORMAL + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x07, kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04),

    /* Data write - Normal */
    [4 * PSRAM_CMD_LUT_SEQ_IDX_WRITEDATA_NORMAL] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x80, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * PSRAM_CMD_LUT_SEQ_IDX_WRITEDATA_NORMAL + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x07, kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04),
};

status_t flexspi_psram_config(void)
{
    volatile uint32_t i = 0;
    status_t status;

    uint32_t mr0mr1[1] = {0};
    uint32_t mr2mr3[1] = {0};
    uint32_t mr4mr8[1] = {0};
    uint32_t mr0Val[1] = {0};
    uint32_t mr4Val[1] = {0};
    uint32_t mr8Val[1] = {0};
    
    status = flexspi_psram_get_mcr(EXAMPLE_FLEXSPI, 0x0, mr0mr1);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("mr0mr1Val : 0x%x\r\n", mr0mr1[0]);
      
    status = flexspi_psram_get_mcr(EXAMPLE_FLEXSPI, 0x2, mr2mr3);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("mr2mr3Val : 0x%x\r\n", mr2mr3[0]);

    status = flexspi_psram_get_mcr(EXAMPLE_FLEXSPI, 0x4, mr4mr8);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("mr4mr8Val : 0x%x\r\n", mr4mr8[0]);
#if 0
    /* Enable RBX, burst length set to 1K. - MR8 */
    mr8Val[0] = (mr4mr8[0] & 0xFF00U) >> 8U;
    mr8Val[0] = mr8Val[0] | 0x0F;
    status    = flexspi_psram_write_mcr(EXAMPLE_FLEXSPI, 0x8, mr8Val);
    if (status != kStatus_Success)
    {
        return status;
    }
#endif
    /* Set LC code to 0x03(LC=6, maximum frequency 166M) - MR0[5:2]. */
    mr0Val[0] = mr0mr1[0] & 0x00FFU;
    //mr0Val[0] = (mr0Val[0] & ~0x3C) | (3 << 2U);//LC=6, 166M
    mr0Val[0] = (mr0Val[0] & ~0x3C) | (4 << 2U);//LC=7, 200M
    status = flexspi_psram_write_mcr(EXAMPLE_FLEXSPI, 0x0, mr0Val);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Set WLC code to 0x06(WLC=6, maximum frequency 166M) - MR4[7:5]. */
    mr4Val[0] = mr4mr8[0] & 0x00FFU;
    //mr4Val[0] = (mr4Val[0] & ~0xE0) | (6 << 5U); //WLC=6
    mr4Val[0] = (mr4Val[0] & ~0xE0) | (1 << 5U);//WLC=7
    status = flexspi_psram_write_mcr(EXAMPLE_FLEXSPI, 0x4, mr4Val);
    if (status != kStatus_Success)
    {
        return status;
    }
    
    status = flexspi_psram_get_mcr(EXAMPLE_FLEXSPI, 0x0, mr0mr1);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("mr0mr1Val : 0x%x\r\n", mr0mr1[0]);

    status = flexspi_psram_get_mcr(EXAMPLE_FLEXSPI, 0x4, mr4mr8);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("mr4mr8Val : 0x%x\r\n", mr4mr8[0]);

    return 0;
}

int main(void)
{
    uint32_t i = 0;
    uint32_t coreclk = 0;
    uint32_t ahbclk = 0;
    uint32_t hyperbusclk = 0;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    SCB_DisableDCache();
#if 1
    // Set flexspi root clock to 332MHZ.
    clock_root_config_t rootCfg = {0};
    CLOCK_InitPfd(kCLOCK_Pll_SysPll2, kCLOCK_Pfd2, 29);
    rootCfg.mux = 6;
    rootCfg.div = 0;
//    CLOCK_InitPfd(kCLOCK_Pll_SysPll2, kCLOCK_Pfd2, 24);
//    rootCfg.mux = 6;
//    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
#endif
    coreclk = CLOCK_GetRootClockFreq(kCLOCK_Root_M7);
    ahbclk = CLOCK_GetRootClockFreq(kCLOCK_Root_Bus);
    hyperbusclk = CLOCK_GetRootClockFreq(kCLOCK_Root_Flexspi1);
    PRINTF("coreclk: %d; ahbclk: %d; hyperbusclk: %d\r\n", coreclk, ahbclk, hyperbusclk);

    PRINTF("FLEXSPI PSRAM example started!\r\n");

    flexspi_psram_init(EXAMPLE_FLEXSPI);
    
    flexspi_psram_config();  //just for Apmemory

    for (i = 0; i < sizeof(s_psram_write_buffer); i++)
    {
        s_psram_write_buffer[i] = i;
    }
    
    /* IP command write/read, should notice that the start address should be even address and the write address/size
     * should be 1024 aligned.*/
    for (i = 0; i < DRAM_SIZE; i += 1024)
    {
        flexspi_psram_ipcommand_write_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_psram_write_buffer,
                                               sizeof(s_psram_write_buffer));
        
        flexspi_psram_ipcommand_read_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_psram_read_buffer,
                                              sizeof(s_psram_read_buffer));

        if (memcmp(s_psram_read_buffer, s_psram_write_buffer, sizeof(s_psram_write_buffer)) != 0)
        {
            PRINTF("IP Command Read/Write data Failure at 0x%x - 0x%x!\r\n", i, i + 1023);
            return -1;
        }
    }
    
    PRINTF("IP Command Read/Write data succeed at all address range !\r\n");
    
    /* Need to reset FlexSPI controller between IP/AHB access. */
    FLEXSPI_SoftwareReset(EXAMPLE_FLEXSPI);

    for (i = 0; i < sizeof(s_psram_write_buffer); i++)
    {
        s_psram_write_buffer[i] = (i + 0xFFU);
    }
    
    memset(s_psram_read_buffer, 0, sizeof(s_psram_read_buffer));

    for (i = 0; i < DRAM_SIZE; i += 1024)
    {
        L1CACHE_InvalidateDCache();
        flexspi_psram_ahbcommand_write_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_psram_write_buffer,
                                                sizeof(s_psram_write_buffer));
        DCACHE_CleanInvalidateByRange((EXAMPLE_FLEXSPI_AMBA_BASE + i),1024);
        flexspi_psram_ahbcommand_read_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_psram_read_buffer,
                                               sizeof(s_psram_write_buffer));

        if (memcmp(s_psram_read_buffer, s_psram_write_buffer, sizeof(s_psram_write_buffer)) != 0)
        {
            PRINTF("AHB Command Read/Write data Failure at 0x%x - 0x%x!\r\n", i, i + 1023);
            return -1;
        }
    }
    
    for (i = 0; i < sizeof(s_psram_write_buffer); i++)
    {
        s_psram_write_buffer[i] = i;
    }
    memset(s_psram_read_buffer, 0, sizeof(s_psram_read_buffer));

    for (i = 1; i < DRAM_SIZE - 1024; i += 1024)
    {
        L1CACHE_InvalidateDCache();
        flexspi_psram_ahbcommand_write_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_psram_write_buffer,
                                                sizeof(s_psram_write_buffer));
        DCACHE_CleanInvalidateByRange((EXAMPLE_FLEXSPI_AMBA_BASE + i),1024);
        flexspi_psram_ahbcommand_read_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_psram_read_buffer,
                                               sizeof(s_psram_read_buffer));

        if (memcmp(s_psram_read_buffer, s_psram_write_buffer, sizeof(s_psram_write_buffer)) != 0)
        {
            PRINTF("AHB Command Read/Write data Failure at 0x%x - 0x%x!\r\n", i, i + 1023);
            return -1;
        }
    }
    
    for (i = 0; i < sizeof(s_psram_write_buffer); i++)
    {
        s_psram_write_buffer[i] = (i + 0xFFU);
    }
    memset(s_psram_read_buffer, 0, sizeof(s_psram_read_buffer));

    for (i = 2; i < DRAM_SIZE - 1024; i += 1024)
    {
        L1CACHE_InvalidateDCache();
        flexspi_psram_ahbcommand_write_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_psram_write_buffer,
                                                sizeof(s_psram_write_buffer));
        DCACHE_CleanInvalidateByRange((EXAMPLE_FLEXSPI_AMBA_BASE + i),1024);
        flexspi_psram_ahbcommand_read_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_psram_read_buffer,
                                               sizeof(s_psram_read_buffer));

        if (memcmp(s_psram_read_buffer, s_psram_write_buffer, sizeof(s_psram_write_buffer)) != 0)
        {
            PRINTF("AHB Command Read/Write data Failure at 0x%x - 0x%x!\r\n", i, i + 1023);
            return -1;
        }
    }
    
    for (i = 0; i < sizeof(s_psram_write_buffer); i++)
    {
        s_psram_write_buffer[i] = i;
    }
    memset(s_psram_read_buffer, 0, sizeof(s_psram_read_buffer));

    for (i = 3; i < DRAM_SIZE - 1024; i += 1024)
    {
        L1CACHE_InvalidateDCache();
        flexspi_psram_ahbcommand_write_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_psram_write_buffer,
                                                sizeof(s_psram_write_buffer));
        DCACHE_CleanInvalidateByRange((EXAMPLE_FLEXSPI_AMBA_BASE + i),1024);
        flexspi_psram_ahbcommand_read_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_psram_read_buffer,
                                               sizeof(s_psram_read_buffer));

        if (memcmp(s_psram_read_buffer, s_psram_write_buffer, sizeof(s_psram_write_buffer)) != 0)
        {
            PRINTF("AHB Command Read/Write data Failure at 0x%x - 0x%x!\r\n", i, i + 1023);
            return -1;
        }
    }

    PRINTF("AHB Command Read/Write data succeed at all address range !\r\n");  
   
#if 1
    /*stress test*/
    test_config.base_addr = 0x30000000;
    //test_config.test_size = 16 * 1024 * 1024;//Apmemory
    test_config.loop_num = 1;
    test_config.freq = hyperbusclk;
    test_config.fail_stop = 0;
    test_config.enable_cache = 0;
    
    if (!test_config.enable_cache) {
        /* Disable D cache */
        SCB_DisableDCache();
    }

    PRINTF("\r\n########## Print out from target board ##########\r\n");
    PRINTF("\r\nHyperRAM test setting:\r\n");
    PRINTF("                  Base Address: 0x%x;\r\n", test_config.base_addr);
    PRINTF("                  Test Size: %d Bytes;\r\n", test_config.test_size);
    PRINTF("                  Test Loop: %d;\r\n", test_config.loop_num);    
    PRINTF("                  DRAM Freq: %d;\r\n",test_config.freq);
    PRINTF("                  Fail Stop: %d;\r\n", test_config.fail_stop);
    PRINTF("                  Enable Cache: %d;\r\n", test_config.enable_cache);
    PRINTF("                  Core clock: %d;\r\n", coreclk);
    PRINTF("                  AHB clock: %d;\r\n", ahbclk);
    PRINTF("                  HyperRAM clock: %d;\r\n", hyperbusclk);

    fail_stop = test_config.fail_stop;
    /* Run memory stress test: 8-MByte,loop=1,page_size = 1kbyte */
    memtester_main(test_config.base_addr, test_config.test_size, 'B', test_config.loop_num, (1*1024));
#endif

    while (1)
    {
    }
}
