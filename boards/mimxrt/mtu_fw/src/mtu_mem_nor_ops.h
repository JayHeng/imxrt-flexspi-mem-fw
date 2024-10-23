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
#define NOR_CMD_LUT_SEQ_IDX_READREG1        8
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

// Supported Flash inst mode
typedef enum _flash_inst_mode
{
    kFlashInstMode_SPI    = 0,
    kFlashInstMode_QPI_1  = 1,
    kFlashInstMode_QPI_2  = 2,
    kFlashInstMode_OPI    = 3,
    kFlashInstMode_Hyper  = 4,

    kFlashInstMode_MAX    = 5,
} flash_inst_mode_t;

// Flash status/cfg register r/w access helper
typedef struct _flash_reg_access
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
} flash_reg_access_t;

typedef struct _mixspi_cache_status
{
    volatile bool DCacheEnableFlag;
    volatile bool ICacheEnableFlag;
    volatile bool codeCacheEnableFlag;
    volatile bool systemCacheEnableFlag;
} mixspi_cache_status_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern mixspi_user_config_t s_userConfig;

/*******************************************************************************
 * API
 ******************************************************************************/

void mtu_mixspi_mem_init(mixspi_user_config_t *userConfig, flexspi_device_config_t *deviceconfig);

status_t mtu_mixspi_nor_get_jedec_id(mixspi_user_config_t *userConfig, uint32_t *vendorId);

status_t mtu_mixspi_nor_write_register(mixspi_user_config_t *userConfig, flash_reg_access_t *regAccess);

status_t mtu_mixspi_nor_read_register(mixspi_user_config_t *userConfig, flash_reg_access_t *regAccess);

status_t mtu_mixspi_nor_enable_quad_mode(mixspi_user_config_t *userConfig);

status_t mtu_mixspi_nor_erase_sector(mixspi_user_config_t *userConfig, uint32_t address, flash_inst_mode_t flashInstMode);

status_t mtu_mixspi_nor_page_program(mixspi_user_config_t *userConfig,
                                     flexspi_device_config_t *deviceconfig,
                                     uint32_t address,
                                     const uint32_t *src,
                                     uint32_t length,
                                     flash_inst_mode_t flashInstMode);

#endif /* _MTU_MEM_NOR_OPS_H_ */
