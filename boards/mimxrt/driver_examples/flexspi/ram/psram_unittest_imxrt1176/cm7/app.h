/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_FLEXSPI FLEXSPI1
#define FLASH_SIZE 0x4000 /* 128Mb /KByte */
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI1_AMBA_BASE
#define EXAMPLE_FLEXSPI_CLOCK kCLOCK_Flexspi1

#define PSRAM_CMD_LUT_SEQ_IDX_READDATA   0
#define PSRAM_CMD_LUT_SEQ_IDX_WRITEDATA  1
#define PSRAM_CMD_LUT_SEQ_IDX_READREG    2
#define PSRAM_CMD_LUT_SEQ_IDX_WRITEREG   3
#define PSRAM_CMD_LUT_SEQ_IDX_RESET      4
#define PSRAM_CMD_LUT_SEQ_IDX_READDATA_NORMAL  5
#define PSRAM_CMD_LUT_SEQ_IDX_WRITEDATA_NORMAL 6

#define CUSTOM_LUT_LENGTH 60

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
static inline void flexspi_clock_init(void)
{
    /*Clock setting for flexspi1*/
    CLOCK_SetRootClockDiv(kCLOCK_Root_Flexspi1, 2);
    CLOCK_SetRootClockMux(kCLOCK_Root_Flexspi1, 0);
}
/*${prototype:end}*/

#endif /* _APP_H_ */
