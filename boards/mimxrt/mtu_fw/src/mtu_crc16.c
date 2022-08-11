/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mtu_crc16.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

/*
    crc16Config->currentCrc = 0x0000U;
    crc_config_t crcUserConfigPtr;
    CRC_GetDefaultConfig(&crcUserConfigPtr);

#if !BL_DEVICE_IS_LPC_SERIES
    crcUserConfigPtr.crcBits = kCrcBits16;
    crcUserConfigPtr.seed = crc16Config->currentCrc;
    crcUserConfigPtr.polynomial = 0x1021U;
    crcUserConfigPtr.complementChecksum = false;
    crcUserConfigPtr.reflectIn = false;
    crcUserConfigPtr.reflectOut = false;
#else // BL_DEVICE_IS_LPC_SERIES
    crcUserConfigPtr.seed = crc16Config->currentCrc;
    crcUserConfigPtr.polynomial = kCRC_Polynomial_CRC_CCITT;
    crcUserConfigPtr.reverseIn = false;
    crcUserConfigPtr.reverseOut = false;
    crcUserConfigPtr.complementIn = false;
    crcUserConfigPtr.complementOut = false;
#endif // !BL_DEVICE_IS_LPC_SERIES
*/

void crc16_init(crc16_data_t *crc16Config)
{
    // initialize running crc and byte count
    crc16Config->currentCrc = 0;
}

void crc16_update(crc16_data_t *crc16Config, const uint8_t *src, uint32_t lengthInBytes)
{
    uint32_t crc = crc16Config->currentCrc;

    uint32_t j;
    for (j = 0; j < lengthInBytes; ++j)
    {
        uint32_t i;
        uint32_t byte = src[j];
        crc ^= byte << 8;
        for (i = 0; i < 8; ++i)
        {
            uint32_t temp = crc << 1;
            if (crc & 0x8000)
            {
                temp ^= 0x1021;
            }
            crc = temp;
        }
    }

    crc16Config->currentCrc = crc;
}

void crc16_finalize(crc16_data_t *crc16Config, uint16_t *hash)
{
    *hash = crc16Config->currentCrc;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
