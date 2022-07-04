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
#define FLASH_SIZE 0x4000 /* 16MB   -KByte */ //W957D8MFYA
//#define FLASH_SIZE 0x8000 /* 32MB   -KByte */ //W958D8NBYA
//#define FLASH_SIZE 0x2000 /* 8MB   -KByte */ //S27KL0642, IS66WVH8M8DALL
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI1_AMBA_BASE

#define HYPERRAM_CMD_LUT_SEQ_IDX_READDATA 0
#define HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA 1
#define HYPERRAM_CMD_LUT_SEQ_IDX_READREG 2
#define HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG 3

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
