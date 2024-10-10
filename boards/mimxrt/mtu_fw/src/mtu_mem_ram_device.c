/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mtu_mem_ram_device.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/



/*******************************************************************************
 * Variables
 *****************************************************************************/



/*******************************************************************************
 * Code
 ******************************************************************************/

status_t mtu_psram_set_registers_for_apmemory(mixspi_user_config_t *userConfig)
{
    uint32_t mr0mr1[1];
    uint32_t mr4mr8[1];
    uint32_t mr0Val[1];
    uint32_t mr4Val[1];
    uint32_t mr8Val[1];
    psram_reg_access_t regAccess;
    status_t status = kStatus_Success;

    /* Reset hyper ram. */
    status = mtu_mixspi_psram_reset(userConfig);
    if (status != kStatus_Success)
    {
        return status;
    }
    
    regAccess.regNum = 2;
    regAccess.regAddr = 0x0;
    status = mtu_mixspi_psram_read_register(userConfig, &regAccess);
    if (status != kStatus_Success)
    {
        return status;
    }
    mr0mr1[0] = regAccess.regValue.U;

    regAccess.regNum = 2;
    regAccess.regAddr = 0x4;
    status = mtu_mixspi_psram_read_register(userConfig, &regAccess);
    if (status != kStatus_Success)
    {
        return status;
    }
    mr4mr8[0] = regAccess.regValue.U;

    /* Enable RBX, burst length set to 1K. - MR8 */
    mr8Val[0] = (mr4mr8[0] & 0xFF00U) >> 8U;
    mr8Val[0] = mr8Val[0] | 0x0F;
    regAccess.regNum = 1;
    regAccess.regAddr = 0x8;
    regAccess.regValue.U = mr8Val[0];
    status    = mtu_mixspi_psram_write_register(userConfig, &regAccess);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Set LC code to 0x04(LC=7, maximum frequency 200M) - MR0. */
    mr0Val[0] = mr0mr1[0] & 0x00FFU;
    mr0Val[0] = (mr0Val[0] & ~0x3CU) | (4U << 2U);
    regAccess.regNum = 1;
    regAccess.regAddr = 0x0;
    regAccess.regValue.U = mr0Val[0];
    status    = mtu_mixspi_psram_write_register(userConfig, &regAccess);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Set WLC code to 0x01(WLC=7, maximum frequency 200M) - MR4. */
    mr4Val[0] = mr4mr8[0] & 0x00FFU;
    mr4Val[0] = (mr4Val[0] & ~0xE0U) | (1U << 5U);
    regAccess.regNum = 1;
    regAccess.regAddr = 0x4;
    regAccess.regValue.U = mr4Val[0];
    status    = mtu_mixspi_psram_write_register(userConfig, &regAccess);
    if (status != kStatus_Success)
    {
        return status;
    }

    return status;
}
