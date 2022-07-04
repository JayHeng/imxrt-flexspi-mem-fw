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
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _semc_test_config {
  uint32_t base_addr;
  uint32_t test_size;
  uint32_t loop_num;
  uint32_t dram_freq;
  uint32_t fail_stop;
  uint32_t enable_cache;
} semc_test_config;

semc_test_config test_config;
int fail_stop;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

flexspi_device_config_t deviceconfig = {
    .flexspiRootClk       = 332000000,
    .isSck2Enabled        = false,
    .flashSize            = FLASH_SIZE,
    .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval           = 2,
    .CSHoldTime           = 3,
    .CSSetupTime          = 3,
    .dataValidTime        = 1,
    .columnspace          = 3,
    .enableWordAddress    = true,
    .AWRSeqIndex          = HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA,
    .AWRSeqNumber         = 1,
    .ARDSeqIndex          = HYPERRAM_CMD_LUT_SEQ_IDX_READDATA,
    .ARDSeqNumber         = 1,
    .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
    .enableWriteMask      = true,
};

//W957D8MFYA , W958D8NBYA, S27KL0642
uint32_t customLUT_winbond[20] = {
        /* Read Data */
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READDATA] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xA0, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READDATA + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x07),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READDATA + 2] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

        /* Write Data */
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x07),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA + 2] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

        /* Read Register */
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READREG] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xE0, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READREG + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x07),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READREG + 2] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

        /* Write Register */
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x60, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG + 2] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),
};

uint32_t customLUT_issi[20] = {
        /* Read Data */
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READDATA] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xA0, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READDATA + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x06),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READDATA + 2] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

        /* Write Data */
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x06),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA + 2] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

        /* Read Register */
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READREG] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xE0, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READREG + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x06),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READREG + 2] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

        /* Write Register */
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x60, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG + 2] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),
};


static uint8_t s_hyper_ram_write_buffer[1024];
static uint8_t s_hyper_ram_read_buffer[1024];
static uint16_t s_hyper_ram_buffer[1];
#define DRAM_SIZE 0x80000U
/*******************************************************************************
 * Variables
 ******************************************************************************/

extern status_t flexspi_hyper_ram_write_regCR0(FLEXSPI_Type *base, uint32_t *buffer);
extern status_t flexspi_hyper_ram_get_regCR0(FLEXSPI_Type *base, uint32_t *reg);
extern status_t flexspi_hyper_ram_ipcommand_write_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length);
extern void flexspi_hyper_ram_ahbcommand_write_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length);
extern status_t flexspi_hyper_ram_ipcommand_read_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length);
extern void flexspi_hyper_ram_ahbcommand_read_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length);
extern status_t flexspi_hyper_ram_get_id(FLEXSPI_Type *base, uint32_t *vendorId);
extern void flexspi_nor_flash_init(FLEXSPI_Type *base);

/*******************************************************************************
 * Code
 ******************************************************************************/

static volatile uint32_t g_msCount = 0;
void SysTick_Handler(void)
{
    g_msCount++;
}

int main(void)
{
    uint32_t i = 0;
    status_t status;
    uint32_t vendorID = 0;
    uint32_t coreclk = 0;
    uint32_t ahbclk = 0;
    uint32_t hyperbusclk = 0;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    
    // Set flexspi root clock to 332MHZ.
    clock_root_config_t rootCfg = {0};
    CLOCK_InitPfd(kCLOCK_Pll_SysPll2, kCLOCK_Pfd2, 29);
    rootCfg.mux = 6;
    rootCfg.div = 0;
    CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
    
    coreclk = CLOCK_GetRootClockFreq(kCLOCK_Root_M7);
    ahbclk = CLOCK_GetRootClockFreq(kCLOCK_Root_Bus);
    hyperbusclk = CLOCK_GetRootClockFreq(kCLOCK_Root_Flexspi1);
    PRINTF("coreclk: %d; ahbclk: %d; hyperbusclk: %d\r\n", coreclk, ahbclk, hyperbusclk);

    flexspi_nor_flash_init(EXAMPLE_FLEXSPI);

#if 0 /* write/read CR0 register*/
    s_hyper_ram_buffer[0] = 0x2F8F;
    status = flexspi_hyper_ram_write_regCR0(EXAMPLE_FLEXSPI, (uint32_t *)s_hyper_ram_buffer);
    if (status != kStatus_Success)
    {
        return status;
    }
    uint32_t CR0 = 0;
    status = flexspi_hyper_ram_get_regCR0(EXAMPLE_FLEXSPI, &CR0);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("CR0: 0x%x\r\n", CR0);
#endif
    
#if 1
    /* Get vendor ID. */
    status = flexspi_hyper_ram_get_id(EXAMPLE_FLEXSPI, &vendorID);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("Vendor ID: 0x%x\r\n", vendorID); //0x860c - W957D8MFYA; 0x860e - W958D8NBYA; 0x810c - S27KL0642

    for (i = 0; i < sizeof(s_hyper_ram_write_buffer); i++)
    {
        s_hyper_ram_write_buffer[i] = i;
    }
    
    flexspi_hyper_ram_ahbcommand_write_data(EXAMPLE_FLEXSPI, 0x2000, (uint32_t *)s_hyper_ram_write_buffer,
                                                sizeof(s_hyper_ram_write_buffer));
    
    flexspi_hyper_ram_ipcommand_read_data(EXAMPLE_FLEXSPI, 0x2000, (uint32_t *)s_hyper_ram_read_buffer,
                                              sizeof(s_hyper_ram_read_buffer));
    
    flexspi_hyper_ram_ipcommand_write_data(EXAMPLE_FLEXSPI, 0x3000, (uint32_t *)s_hyper_ram_write_buffer,
                                               sizeof(s_hyper_ram_write_buffer));
    
    flexspi_hyper_ram_ahbcommand_read_data(EXAMPLE_FLEXSPI, 0x3000, (uint32_t *)s_hyper_ram_read_buffer,
                                               sizeof(s_hyper_ram_read_buffer));

    for (i = 0; i < DRAM_SIZE; i += 2048)
    {
        flexspi_hyper_ram_ipcommand_write_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_hyper_ram_write_buffer,
                                               sizeof(s_hyper_ram_write_buffer));
        flexspi_hyper_ram_ipcommand_read_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_hyper_ram_read_buffer,
                                              sizeof(s_hyper_ram_read_buffer));

        if (memcmp(s_hyper_ram_read_buffer, s_hyper_ram_write_buffer, sizeof(s_hyper_ram_write_buffer)) != 0)
        {
            PRINTF("IP Command Read/Write data Failure at 0x%x - 0x%x!\r\n", i, i + 2047);
            return -1;
        }
    }

    PRINTF("IP Command Read/Write data succeed at all address range !\r\n");

    for (i = sizeof(s_hyper_ram_write_buffer); i > 0; i--)
    {
        s_hyper_ram_write_buffer[i] = i;
    }
    memset(s_hyper_ram_read_buffer, 0, sizeof(s_hyper_ram_read_buffer));

    for (i = 0; i < DRAM_SIZE; i += 2048)
    {
        flexspi_hyper_ram_ahbcommand_write_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_hyper_ram_write_buffer,
                                                sizeof(s_hyper_ram_write_buffer));
        flexspi_hyper_ram_ahbcommand_read_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_hyper_ram_read_buffer,
                                               sizeof(s_hyper_ram_read_buffer));

        if (memcmp(s_hyper_ram_read_buffer, s_hyper_ram_write_buffer, sizeof(s_hyper_ram_write_buffer)) != 0)
        {
            PRINTF("AHB Command Read/Write data Failure at 0x%x - 0x%x!\r\n", i, i + 2047);
            return -1;
        }
    }

    PRINTF("AHB Command Read/Write data succeed at all address range !\r\n");
#endif   
    
#if 1 /* HyperRAM stress test */
    test_config.base_addr = 0x30000000;
    //test_config.test_size = 16 * 1024 * 1024; //W957D8MFYA
    test_config.test_size = 32 * 1024 * 1024; //W958D8NBYA
    test_config.loop_num = 1;
    test_config.fail_stop = 0;
    test_config.enable_cache = 0;
    
    if (!test_config.enable_cache) {
        /* Disable D cache */
        SCB_DisableDCache();
    }

    PRINTF("\r\n########## Print out from target board ##########\r\n");
    PRINTF("\r\nDRAM test setting:\r\n");
    PRINTF("                  Base Address: 0x%x;\r\n", test_config.base_addr);
    PRINTF("                  Test Size: %d Bytes;\r\n", test_config.test_size);
    PRINTF("                  Test Loop: %d;\r\n", test_config.loop_num);    
    PRINTF("                  Fail Stop: %d;\r\n", test_config.fail_stop);
    PRINTF("                  Enable Cache: %d;\r\n", test_config.enable_cache);

    fail_stop = test_config.fail_stop;
    /* Run memory stress test: 32-MByte,loop=1,page_size = 1kbyte */
    memtester_main(test_config.base_addr, test_config.test_size, 'B', test_config.loop_num, (1*1024));
#endif
    
    while (1)
    {
    }
}
