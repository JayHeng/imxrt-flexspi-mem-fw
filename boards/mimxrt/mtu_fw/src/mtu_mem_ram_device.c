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
    volatile uint32_t i = 0;
    status_t status;

    uint16_t mr0mr1;
    uint16_t mr2mr3;
    uint16_t mr4mr8;
    uint8_t mr0Val;
    uint8_t mr4Val;
    uint8_t mr8Val;
    psram_reg_access_t regAccess;

    /* Reset hyperram. */
    status = mtu_mixspi_psram_reset(userConfig);
    if (status != kStatus_Success)
    {
        return status;
    }
    
    /* Read mr0mr1 register value, the default value is 0x8d09 */
    regAccess.regNum = 2;
    regAccess.regAddr  = 0x0;
    status = mtu_mixspi_psram_read_register(userConfig, &regAccess);
    if (status != kStatus_Success)
    {
        return status;
    }
    mr0mr1 = regAccess.regValue.U & 0xFFFF;
    printf("mr0mr1 register default value : 0x%x\r\n", mr0mr1);
    
    /* Read mr2mr3 register value, the default value is 0xa093 */
    regAccess.regNum = 2;
    regAccess.regAddr  = 0x2;
    status = mtu_mixspi_psram_read_register(userConfig, &regAccess);
    if (status != kStatus_Success)
    {
        return status;
    }
    mr2mr3 = regAccess.regValue.U & 0xFFFF;
    printf("mr2mr3 register default value : 0x%x\r\n", mr2mr3);

    /* Read mr4mr8 register value, the default value is 0x540 */
    regAccess.regNum = 2;
    regAccess.regAddr = 0x4;
    status = mtu_mixspi_psram_read_register(userConfig, &regAccess);
    if (status != kStatus_Success)
    {
        return status;
    }
    mr4mr8 = regAccess.regValue.U & 0xFFFF;
    printf("mr4mr8 register default value : 0x%x\r\n", mr4mr8);

    /* Enable RBX, burst length set to 1K. - MR8 */
    mr8Val = (mr4mr8 & 0xFF00U) >> 8U;
    mr8Val |= 0x0F;
    regAccess.regNum = 1;
    regAccess.regAddr = 0x8;
    regAccess.regValue.U = mr8Val;
    status    = mtu_mixspi_psram_write_register(userConfig, &regAccess);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Set LC code to 
     * 0x03(LC=6, maximum frequency 166M) - MR0[5:2]. 
     * 0x04(LC=7, maximum frequency 200M) - MR0[5:2].
    */
    mr0Val = mr0mr1 & 0x00FFU;
    if (userConfig->instance == 1)
    {
        mr0Val = (mr0Val & ~0x3C) | (4 << 2U); /* LC=7, 200M */
    }
    else if (userConfig->instance == 2)
    {
        mr0Val = (mr0Val & ~0x3C) | (3 << 2U); /* LC=6, 166M */
    }
    mr0Val = (mr0Val & ~0x3) | (0);/* config drive strength from half to full */
    regAccess.regNum = 1;
    regAccess.regAddr = 0x0;
    regAccess.regValue.U = mr0Val;
    status    = mtu_mixspi_psram_write_register(userConfig, &regAccess);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Set WLC code to 
     * 0x06(WLC=6, maximum frequency 166M) - MR4[7:5]. 
     * 0x01(WLC=7, maximum frequency 200M) - MR4[7:5].
    */
    mr4Val = mr4mr8 & 0x00FFU;
    if (userConfig->instance == 1)
    {
        mr4Val = (mr4Val & ~0xE0U) | (1U << 5U); /* WLC=7, 200M */
    } 
    else if (userConfig->instance == 2)
    {
        mr4Val = (mr4Val & ~0xE0U) | (6U << 5U); /* WLC=6, 166M */
    }
    regAccess.regNum = 1;
    regAccess.regAddr = 0x4;
    regAccess.regValue.U = mr4Val;
    status    = mtu_mixspi_psram_write_register(userConfig, &regAccess);
    if (status != kStatus_Success)
    {
        return status;
    }

    regAccess.regNum = 2;
    regAccess.regAddr  = 0x0;
    status = mtu_mixspi_psram_read_register(userConfig, &regAccess);
    if (status != kStatus_Success)
    {
        return status;
    }
    mr0mr1 = regAccess.regValue.U & 0xFFFF;
    printf("read mr0mr1 register value : 0x%x\r\n", mr0mr1);

    regAccess.regNum = 2;
    regAccess.regAddr = 0x4;
    status = mtu_mixspi_psram_read_register(userConfig, &regAccess);
    if (status != kStatus_Success)
    {
        return status;
    }
    mr4mr8 = regAccess.regValue.U & 0xFFFF;
    printf("read mr4mr8 register value : 0x%x\r\n", mr4mr8);

    return kStatus_Success;
}

status_t mtu_psram_set_registers_for_issi(mixspi_user_config_t *userConfig)
{
    uint16_t identification = 0x00U;
    uint16_t registerVal    = 0x00U;
    psram_reg_access_t regAccess;
    status_t status = kStatus_Success;

    /* Read identification: the Manufacturer ID of ISSI's PSRAM(IS66/67WVQ8M4DALL) is 0x03U  */
    do
    {
        status = mtu_mixspi_psram_read_id(userConfig, &identification);
        if ((status != kStatus_Success) || (identification & 0x03U) != 0x03U)
        {
            status = kStatus_Fail;
            break;
        }

        /* Read configuration register: the default setting is 0xF052(see table 6.1 Configuration Register in
           PSRAM's(IS66/67WVQ8M4DALL) datasheet), which Latency code(CR[7:4]) is 0101b, which supported max frequency
           is 200MHz.*/
        regAccess.regNum = 2;
        regAccess.regAddr = 0x04UL << 9;
        status = mtu_mixspi_psram_read_register(userConfig, &regAccess);
        registerVal = regAccess.regValue.U & 0xFFFF;
        if ((status != kStatus_Success) || registerVal != 0xF052U)
        {
            status = kStatus_Fail;
            break;
        }

        /* Initial access latency configuration, which is located in bit3 of CR. */
        registerVal |= (uint16_t)(0x01UL << 3);

        /* Write configuration register: */
        regAccess.regNum = 2;
        regAccess.regAddr = 0x04UL << 9;
        regAccess.regValue.U = registerVal & 0xFFFF;
        status = mtu_mixspi_psram_write_register(userConfig, &regAccess);
        if ((status != kStatus_Success) || registerVal != 0xF05AU)
        {
            status = kStatus_Fail;
            break;
        }

        /* Reset */
        registerVal = 0x00U;

        /* Read configuration register: changes default Variable Latency into Fixed Latency: 0xF05A.
           Note: FlexSPI only supports fixed latency mode for ISSI's psram. */
        regAccess.regNum = 2;
        regAccess.regAddr = 0x04UL << 9;
        status = mtu_mixspi_psram_read_register(userConfig, &regAccess);
        registerVal = regAccess.regValue.U & 0xFFFF;
        if ((status != kStatus_Success) || registerVal != 0xF05AU)
        {
            status = kStatus_Fail;
            break;
        }
    } while (false);

    return status;
}
