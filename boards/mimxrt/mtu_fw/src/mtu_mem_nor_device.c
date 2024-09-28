/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mtu_mem_nor_device.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

uint32_t mtu_flash_decode_common_capacity_id(uint8_t capacityID);
uint32_t mtu_flash_decode_adesto_capacity_id(uint8_t capacityID);
void mtu_flash_show_mem_size(uint8_t capacityID, bool isAdesto);
void mtu_flash_set_info_for_winbond(jedec_id_t *jedecID);
void mtu_flash_set_info_for_mxic(jedec_id_t *jedecID);
void mtu_flash_set_info_for_gigadevice(jedec_id_t *jedecID);
void mtu_flash_set_info_for_issi(jedec_id_t *jedecID);
void mtu_flash_set_info_for_micron(jedec_id_t *jedecID);
void mtu_flash_set_info_for_adesto(jedec_id_t *jedecID);
void mtu_flash_set_info_for_spansion(jedec_id_t *jedecID);

/*******************************************************************************
 * Variables
 *****************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t mtu_flash_decode_common_capacity_id(uint8_t capacityID)
{
    uint32_t memSizeInBytes = 0;
    //| ISSI QuadSPI       |  MXIC OctalSPI     |  Micron QuadSPI    |
    //| ISSI OctalSPI      |  MXIC QuadSPI U    |GigaDevice QuadSPI Q|
    //| MXIC QuadSPI R/L/V |                    |Spansion QuadSPI FL-L|
    //| Winbond QuadSPI    |                    |                    |
    //| Winbond OctalSPI   |                    |                    |
    //| Micron OctalSPI    |                    |                    |
    //| GigaDevice QuadSPI |                    |                    |
    //| GigaDevice OctalSPI|                    |                    |
    //|Adesto QuadSPI SL/QL|                    |                    |
    //|---------------------------------------------------------------
    //| 09h - 256Kb        |                    |                    |
    //| 10h - 512Kb        |                    |                    |
    //| 11h - 1Mb          |                    |                    |
    //| 12h - 2Mb          |                    |                    |
    //| 13h - 4Mb          |                    |                    |
    //| 14h - 8Mb          |                    |                    |
    //| 15h - 16Mb         |                    |                    |
    //| 16h - 32Mb         |                    |                    |
    //| 17h - 64Mb         |  37h - 64Mb        |   17h - 64Mb       |
    //| 18h - 128Mb        |  38h - 128Mb       |   18h - 128Mb      |
    //| 19h - 256Mb        |  39h - 256Mb       |   19h - 256Mb      |
    //| 1ah - 512Mb        |  3ah - 512Mb       |   20h - 512Mb      |
    //| 1bh - 1Gb          |  3bh - 1Gb         |   21h - 1Gb        |
    //| 1ch - 2Gb          |  3ch - 2Gb         |   22h - 2Gb        |
    if (capacityID <= 0x09)
    {
        capacityID += 6;
    }
    else if (capacityID <= 0x1c)
    {
        // Do Nothing
    }
    else if (capacityID <= 0x22)
    {
        capacityID -= 6;
    }
    else
    {
        capacityID &= 0x1F;
    }
    memSizeInBytes = 1 << capacityID;
    return memSizeInBytes;
}

uint32_t mtu_flash_decode_adesto_capacity_id(uint8_t capacityID)
{
    uint32_t memSizeInBytes = 0;
    //|Adesto QuadSPI      | Adesto QuadSPI EU  |
    //|------------------------------------------
    //|                    | 10h - 1Mb          |
    //| 03h - 2Mb          | 11h - 2Mb          |
    //| 04h - 4Mb          | 14h - 4Mb          |
    //| 05h - 8Mb          |                    |
    //| 06h - 16Mb         |                    |
    //| 07h - 32Mb         |                    |
    //| 08h - 64Mb         |                    |
    //| 09h - 128Mb        |                    |
    if (capacityID <= 0x09)
    {
        capacityID += 15;
    }
    else if (capacityID == 0x10)
    {
        capacityID = 0x11;
    }
    else if (capacityID == 0x11)
    {
        capacityID = 0x12;
    }
    else if (capacityID == 0x14)
    {
        capacityID = 0x13;
    }
    memSizeInBytes = 1 << capacityID;
    return memSizeInBytes;
}

void mtu_flash_show_mem_size(uint8_t capacityID, bool isAdesto)
{
    uint32_t flashMemSizeInKB;
    if (isAdesto)
    {
        printf("--Flash Density Code: 0x%x", capacityID);
        flashMemSizeInKB = mtu_flash_decode_adesto_capacity_id(capacityID)/ 0x400;
    }
    else
    {
        printf("--Flash Capacity ID: 0x%x", capacityID);
        flashMemSizeInKB = mtu_flash_decode_common_capacity_id(capacityID)/ 0x400;
    }
    if (flashMemSizeInKB <= 0x400)
    {
        printf(" -- %dKB.\r\n", flashMemSizeInKB);
    }
    else
    {
        printf(" -- %dMB.\r\n", flashMemSizeInKB / 0x400);
    }
}

void mtu_flash_set_info_for_winbond(jedec_id_t *jedecID)
{
    printf("--Flash Manufacturer ID: 0x%x -- Winbond Serial Flash.\r\n", jedecID->manufacturerID);
    printf("--Flash Memory Type ID: 0x%x", jedecID->memoryTypeID);
    switch (jedecID->memoryTypeID)
    {
        /////////////////////////QuadSPI////////////////////////
        case 0x30:
            printf(" -- W25X DualSPI 3.3V Series.\r\n");
            break;
        case 0x40:
            printf(" -- W25QxxxDV/FV/BV/CL/JV(-IQ/JQ) QuadlSPI 3.3V Series.\r\n");
            break;
        case 0x60:
            printf(" -- W25QxxxJW/FW/EW/NW(-IQ/IN) QuadlSPI 1.8V Series.\r\n");
            break;
        case 0x61:
            printf(" -- W25MxxxJW QuadlSPI 1.8V Series.\r\n");
            break;
        case 0x65:
            printf(" -- W25QxxxNE QuadlSPI 1.2V Series.\r\n");
            break;
        case 0x70:
            printf(" -- W25QxxxJV(-IM/JM) QuadlSPI 3.3V Series.\r\n");
            break;
        case 0x80:
            printf(" -- W25QxxxJW/NW(-IM) QuadlSPI 1.8V Series.\r\n");
            break;
        ////////////////////////OctalSPI////////////////////////
        case 0x5B:
            printf(" -- W35TxxxNW OctalSPI 1.8V Series.\r\n");
            break;
        // Missing W25H, W25R
        // Missing xxxJL, xxxDW, xxxRV
        default:
            printf(" -- Unknown Series.\r\n");
            break;
    }
    mtu_flash_show_mem_size(jedecID->capacityID, false);
}

void mtu_flash_set_info_for_mxic(jedec_id_t *jedecID)
{
    printf("--Flash Manufacturer ID: 0x%x -- MXIC Serial Flash.\r\n", jedecID->manufacturerID);
    printf("--Flash Memory Type ID: 0x%x", jedecID->memoryTypeID);
    switch (jedecID->memoryTypeID)
    {
        /////////////////////////QuadSPI////////////////////////
        case 0x20:
            printf(" -- MX25L/MX66L QuadSPI 3.3V Series.\r\n");
            break;
        case 0x23:
            printf(" -- MX25V QuadSPI 3.3V Series.\r\n");
            break;
        case 0x25:
            printf(" -- MX25U/MX66U QuadSPI 1.8V Series.\r\n");
            break;
        case 0x28:
            printf(" -- MX25R QuadSPI 1.8-3.3V Series.\r\n");
            break;
        case 0x75:
            printf(" -- MX77L QuadSPI 3.3V Series.\r\n");
            break;
        ////////////////////////OctalSPI////////////////////////
        case 0x80:
            printf(" -- MX25UM/MX66UM OctalSPI 1.8V Series.\r\n");
            break;
        case 0x81:
            printf(" -- MX25UM51345G OctalSPI 1.8V Series.\r\n");
            break;
        case 0x83:
            printf(" -- MX25UM25345G OctalSPI 1.8V Series.\r\n");
            break;
        case 0x84:
            printf(" -- MX25UWxx345G OctalSPI 1.8V Series.\r\n");
            break;
        case 0x85:
            printf(" -- MX25LM/MX66LM OctalSPI 3.3V Series.\r\n");
            break;
        // Missing MX25LW51245G, MX66LW1G45G, MX66LW2G45G
        // Missing MX25UW6445G, MX66UW12845G, MX25UW25645G, MX25UW51245G, MX66UW1G45G, MX66UW2G45G
        // Missing MX25UW6345G, MX66LW12345G, MX66UW25345G      done                 , MX66UW2G345G
        default:
            printf(" -- Unknown Series.\r\n");
            break;
    }
    mtu_flash_show_mem_size(jedecID->capacityID, false);
}

void mtu_flash_set_info_for_gigadevice(jedec_id_t *jedecID)
{
    printf("--Flash Manufacturer ID: 0x%x -- GigaDevice Serial Flash.\r\n", jedecID->manufacturerID);
    printf("--Flash Memory Type ID: 0x%x", jedecID->memoryTypeID);
    switch (jedecID->memoryTypeID)
    {
        /////////////////////////QuadSPI////////////////////////
        case 0x40:
            // GD25D DualSPI
            printf(" -- GD25Q/GD25B/GD25S QuadSPI 3.3V Series.\r\n");
            break;
        case 0x42:
            printf(" -- GD25VQ/GD25VE QuadSPI 2.5V Series.\r\n");
            break;
        case 0x47:
            printf(" -- GD55B QuadSPI 3.3V Series.\r\n");
            break;
        case 0x60:
            // GD25LD DualSPI
            printf(" -- GD25LE/GD25LQ QuadSPI 1.8V Series.\r\n");
            break;
        case 0x63:
            printf(" -- GD25LF/GD55LF QuadSPI 1.8V Series.\r\n");
            break;
        case 0x65:
            // GD25WD DualSPI
            printf(" -- GD25W/GD55W QuadSPI 1.8-3.3V Series.\r\n");
            break;
        case 0x66:
            printf(" -- GD25LT/GD55LT QuadSPI 1.8V Series.\r\n");
            break;
        case 0x67:
            printf(" -- GD25LB/GD55LB QuadSPI 1.8V Series.\r\n");
            break;
        ////////////////////////OctalSPI////////////////////////
        case 0x48:
            printf(" -- GD25X/GD55X OctalSPI 3.3V Series.\r\n");
            break;
        case 0x68:
            printf(" -- GD25LX/GD55LX OctalSPI 1.8V Series.\r\n");
            break;
        // Missing GD25F, GD25LR, GD25T, GD25R
        default:
            printf(" -- Unknown Series.\r\n");
            break;
    }
    mtu_flash_show_mem_size(jedecID->capacityID, false);
}

void mtu_flash_set_info_for_issi(jedec_id_t *jedecID)
{
    printf("--Flash Manufacturer ID: 0x%x -- ISSI Serial Flash.\r\n", jedecID->manufacturerID);
    printf("--Flash Memory Type ID: 0x%x", jedecID->memoryTypeID);
    switch (jedecID->memoryTypeID)
    {
        /////////////////////////QuadSPI////////////////////////
        case 0x40:
            printf(" -- IS25LQ/IS25LP QuadSPI 3.3V Series.\r\n");
            break;
        case 0x60:
            printf(" -- IS25LP/IS25LE QuadSPI 3.3V Series.\r\n");
            break;
        case 0x70:
            printf(" -- IS25WP/IS25WJ/IS25WE QuadSPI 1.8V Series.\r\n");
            break;
        ////////////////////////OctalSPI////////////////////////
        case 0x5A:
            printf(" -- IS25LX OctalSPI 3.3V Series.\r\n");
            break;
        case 0x5B:
            printf(" -- IS25WX OctalSPI 1.8V Series.\r\n");
            break;
        default:
            printf(" -- Unknown Series.\r\n");
            break;
    }
    mtu_flash_show_mem_size(jedecID->capacityID, false);
}

void mtu_flash_set_info_for_micron(jedec_id_t *jedecID)
{
    printf("--Flash Manufacturer ID: 0x%x -- Micron Serial Flash.\r\n", jedecID->manufacturerID);
    printf("--Flash Memory Type ID: 0x%x", jedecID->memoryTypeID);
    switch (jedecID->memoryTypeID)
    {
        /////////////////////////QuadSPI////////////////////////
        case 0xBA:
            printf(" -- MT25QL QuadSPI 3.3V Series.\r\n");
            break;
        case 0xBB:
            printf(" -- MT25QU QuadSPI 1.8V Series.\r\n");
            break;
        ////////////////////////OctalSPI////////////////////////
        case 0x5A:
            printf(" -- MT35XL OctalSPI 3.3V Series.\r\n");
            break;
        case 0x5B:
            printf(" -- MT35XU OctalSPI 1.8V Series.\r\n");
            break;
        default:
            printf(" -- Unknown Series.\r\n");
            break;
    }
    mtu_flash_show_mem_size(jedecID->capacityID, false);
}

void mtu_flash_set_info_for_adesto(jedec_id_t *jedecID)
{
    printf("--Flash Manufacturer ID: 0x%x -- Adesto Serial Flash.\r\n", jedecID->manufacturerID);
    if (jedecID->memoryTypeID != 0x42)
    {
        jedecID->capacityID = jedecID->memoryTypeID & 0x1F;
        jedecID->memoryTypeID = (jedecID->memoryTypeID & 0xE0) >> 5;
        printf("--Flash Family Code: 0x%x", jedecID->memoryTypeID);
    }
    else
    {
        printf("--Flash Memory Type ID: 0x%x", jedecID->memoryTypeID);
    }
    switch (jedecID->memoryTypeID)
    {
        /////////////////////////QuadSPI////////////////////////
        case 0x00:
            printf(" -- AT25EU QuadSPI 1.8-3.3V Series.\r\n");
            break;
        case 0x01:
            printf(" -- AT25DQ QuadSPI 2.5V Series.\r\n");
            break;
        case 0x02:
            printf(" -- AT25FF/AT25XE/AT25XV QuadSPI 1.8-3.3V Series.\r\n");
            break;
        case 0x04:
            printf(" -- AT25SF/AT25QF QuadSPI 3.3V Series.\r\n");
            break;
        // Only this type is same as other vendors
        case 0x42:
            printf(" -- AT25SL/AT25QL QuadSPI 1.8V Series.\r\n");
            break;
        ////////////////////////OctalSPI////////////////////////
        case 0x05:
            printf(" -- ATXP OctalSPI 1.8V Series.\r\n");
            break;
        default:
            printf(" -- Unknown Series.\r\n");
            break;
    }
    mtu_flash_show_mem_size(jedecID->capacityID, (jedecID->memoryTypeID != 0x42));
}

void mtu_flash_set_info_for_spansion(jedec_id_t *jedecID)
{
    printf("--Flash Manufacturer ID: 0x%x -- Spansion Serial Flash.\r\n", jedecID->manufacturerID);
    printf("--Flash Memory Type ID: 0x%x", jedecID->memoryTypeID);
    switch (jedecID->memoryTypeID)
    {
        /////////////////////////QuadSPI////////////////////////
        case 0x2A:
            printf(" -- S25HL-T QuadSPI 3.3V Series.\r\n");
            break;
        case 0x2B:
            printf(" -- S25HS-T QuadSPI 1.8V Series.\r\n");
            break;
        case 0x60:
            // S25FL-L QuadSPI
            printf(" -- S25FL-L QuadSPI 3.3V Series.\r\n");
            break;
        ////////////////////////OctalSPI////////////////////////
        case 0x5A:
            printf(" -- S28HL OctalSPI 3.3V Series.\r\n");
            break;
        case 0x5B:
            printf(" -- S28HS OctalSPI 1.8V Series.\r\n");
            break;
        default:
            printf(" -- Unknown Series.\r\n");
            break;
    }
    mtu_flash_show_mem_size(jedecID->capacityID, false);
}

void mtu_flash_validate_jedec_id(jedec_id_t *jedecID)
{
    /* Check Vendor ID. */
    switch (jedecID->manufacturerID)
    {
        // Winbond
        case WINBOND_DEVICE_VENDOR_ID:
            mtu_flash_set_info_for_winbond(jedecID);
            break;

        // MXIC
        case MXIC_DEVICE_VENDOR_ID:
            mtu_flash_set_info_for_mxic(jedecID);
            break;

        // GigaDevice
        case GIGADEVICE_DEVICE_VENDOR_ID:
            mtu_flash_set_info_for_gigadevice(jedecID);
            break;

        // ISSI
        case ISSI_DEVICE_VENDOR_ID:
            mtu_flash_set_info_for_issi(jedecID);
            break;

        // Micron
        case MICRON_DEVICE_VENDOR_ID:
        case MICRON_DEVICE_VENDOR_ID2:
            mtu_flash_set_info_for_micron(jedecID);
            break;

        // Adesto
        case ADESTO_DEVICE_VENDOR_ID:
        case RENESAS_DEVICE_VENDOR_ID:
            mtu_flash_set_info_for_adesto(jedecID);
            break;

        // Spansion
        case SPANSION_DEVICE_VENDOR_ID:
        case INFINEON_DEVICE_VENDOR_ID:
            mtu_flash_set_info_for_spansion(jedecID);
            break;

        default:
            printf("-- Unknown Manufacturer ID\r\n");
            break;
    }
}
