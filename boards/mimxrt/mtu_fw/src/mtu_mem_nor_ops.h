/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MTU_MEM_NOR_OPS_H_
#define _MTU_MEM_NOR_OPS_H_

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

// NOR property info for operation
typedef struct _mixspi_user_config
{
    MIXSPI_Type                *mixspiBase;
    mixspi_port_t               mixspiPort;
    mixspi_root_clk_freq_t      mixspiRootClkFreq;
    mixspi_read_sample_clock_t  mixspiReadSampleClock;
    const uint32_t             *mixspiCustomLUTVendor;
} mixspi_user_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

void mtu_mixspi_nor_flash_init(mixspi_user_config_t *userConfig, flexspi_device_config_t *deviceconfig);

status_t mtu_mixspi_nor_get_jedec_id(mixspi_user_config_t *userConfig, uint32_t *vendorId);

#endif /* _MTU_MEM_NOR_OPS_H_ */