/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MTU_MEM_NOR_DEVICE_H_
#define _MTU_MEM_NOR_DEVICE_H_

#include "mtu.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief JEDEC id structure. */
typedef struct _jedec_id
{
    uint8_t manufacturerID;
    uint8_t memoryTypeID;
    uint8_t capacityID;
    uint8_t reserved;
} jedec_id_t;

#define WINBOND_DEVICE_VENDOR_ID    (0xEF)
#define MXIC_DEVICE_VENDOR_ID       (0xC2)
#define GIGADEVICE_DEVICE_VENDOR_ID (0xC8)
#define ISSI_DEVICE_VENDOR_ID       (0x9D)
#define MICRON_DEVICE_VENDOR_ID     (0x20)
#define MICRON_DEVICE_VENDOR_ID2    (0x2C)
#define ADESTO_DEVICE_VENDOR_ID     (0x43)
#define RENESAS_DEVICE_VENDOR_ID    (0x1F)
#define SPANSION_DEVICE_VENDOR_ID   (0x01)
#define INFINEON_DEVICE_VENDOR_ID   (0x34)

/*******************************************************************************
 * API
 ******************************************************************************/

void mtu_flash_validate_jedec_id(jedec_id_t *jedecID);

mixspi_root_clk_freq_t mtu_flash_convert_root_clk(uint32_t clkInMHz);

#endif /* _MTU_MEM_NOR_DEVICE_H_ */
