/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MTU_MEM_H_
#define _MTU_MEM_H_

#include <stdint.h>
#include <stdbool.h>
#include "fsl_common.h"
#include "mtu.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

// FlexSPI LUT seq defn (common)
#define NOR_CMD_LUT_SEQ_IDX_READ            0
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR     1
#define NOR_CMD_LUT_SEQ_IDX_ENABLEQE        2
#define NOR_CMD_LUT_SEQ_IDX_ENTERQPI        3
#define NOR_CMD_LUT_SEQ_IDX_ENTEROPI        3
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS      4
#define NOR_CMD_LUT_SEQ_IDX_SETDUMMY        5
#define NOR_CMD_LUT_SEQ_IDX_SETDRIVE        6
#define NOR_CMD_LUT_SEQ_IDX_UNIQUECFG       6
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE     7
#define NOR_CMD_LUT_SEQ_IDX_READREG         8
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM     10
#define NOR_CMD_LUT_SEQ_IDX_READREG2        11
// FlexSPI LUT seq defn (1bit spi)
#define NOR_CMD_LUT_SEQ_IDX_READID          12
#define NOR_CMD_LUT_SEQ_IDX_READID_QPI_1    13
#define NOR_CMD_LUT_SEQ_IDX_READID_QPI_2    14
#define NOR_CMD_LUT_SEQ_IDX_READID_OPI      15
// FlexSPI LUT seq defn (quad lut)
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS_QPI  12
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_QPI 13
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_QPI 14
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QPI 15
// FlexSPI LUT seq defn (octal lut)
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI  12
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI 13
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_OPI 14
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI 15

#define NOR_CMD_LUT_SEQ_IDX_WRITE           9
#define CUSTOM_LUT_LENGTH                   64

/*******************************************************************************
 * API
 ******************************************************************************/

status_t mtu_init_memory(void);

status_t mtu_rw_memory(void);

#endif /* _MTU_MEM_H_ */
