/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MTU_MEM_RAM_OPS_H_
#define _MTU_MEM_RAM_OPS_H_

#include "mtu.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

// FlexSPI LUT seq defn (common)
#define PSRAM_CMD_LUT_SEQ_IDX_READDATA   0
#define PSRAM_CMD_LUT_SEQ_IDX_WRITEDATA  1
#define PSRAM_CMD_LUT_SEQ_IDX_READREG    2
#define PSRAM_CMD_LUT_SEQ_IDX_WRITEREG   3
#define PSRAM_CMD_LUT_SEQ_IDX_RESET      4
#define PSRAM_CMD_LUT_SEQ_IDX_READID     5

// Supported PSRAM protocol type
typedef enum _psram_protocol_type
{
    kPsramProtocolType_QSPI     = 0,
    kPsramProtocolType_QPI      = 1,
    kPsramProtocolType_XCCELA   = 2,
    kPsramProtocolType_HYPERBUS = 3,
    kPsramProtocolType_OctaBus  = 4,   // JESD251

    kPsramProtocolType_MAX      = 5,
} psram_protocol_type_t;

// PSRAM status/cfg register r/w access helper
typedef struct _psram_reg_access
{
    uint8_t regNum;
    uint8_t regSeqIdx;
    uint8_t reserved[2];
    uint32_t regAddr;
    union
    {
        struct
        {
            uint32_t reg1 : 8;
            uint32_t reg2 : 8;
            uint32_t reg3 : 8;
            uint32_t reg4 : 8;
        } B;
        uint32_t U;
    } regValue;
} psram_reg_access_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * API
 ******************************************************************************/

status_t mtu_mixspi_psram_write_register(mixspi_user_config_t *userConfig, psram_reg_access_t *regAccess);

status_t mtu_mixspi_psram_read_register(mixspi_user_config_t *userConfig, psram_reg_access_t *regAccess);

status_t mtu_mixspi_psram_read_id(mixspi_user_config_t *userConfig, uint16_t *buffer);

status_t mtu_mixspi_psram_reset(mixspi_user_config_t *userConfig);


status_t mtu_mixspi_psram_ipg_write_data(mixspi_user_config_t *userConfig, uint32_t address, uint32_t *buffer, uint32_t length);
status_t mtu_mixspi_psram_ipg_read_data(mixspi_user_config_t *userConfig, uint32_t address, uint32_t *buffer, uint32_t length);
void     mtu_mixspi_psram_ahb_write_data(mixspi_user_config_t *userConfig, uint32_t address, uint32_t *buffer, uint32_t length);
void     mtu_mixspi_psram_ahb_read_data(mixspi_user_config_t *userConfig, uint32_t address, uint32_t *buffer, uint32_t length);

#endif /* _MTU_MEM_RAM_OPS_H_ */
