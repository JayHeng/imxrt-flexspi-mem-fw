/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <ctype.h>
#include "mtu.h"
#include "mtu_pin.h"
#include "mtu_uart.h"
#include "mtu_timer.h"
#if MTU_FEATURE_PACKET_CRC
#include "mtu_crc16.h"
#endif
#if MTU_FEATURE_FLASH
#include "mtu_flexspi_nor_test.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void mtu_get_command_from_buffer(void);
static void mtu_switch_print_mode(bool isAsciiMode);
static bool mtu_is_packet_crc16_valid(const uint8_t *data, uint32_t length, uint16_t expectedCrc);
static bool mtu_is_command_valid(void);
static void mtu_execute_command(void);
   
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Current command tag. */
uint8_t s_currentCmdTag = kInvalidCommandTag;
uint8_t s_lastCmdTag    = kInvalidCommandTag;

/*! @brief Pin unit test packet. */
#if MTU_SELFTEST
pin_unittest_packet_t s_pinUnittestPacket = 
{
  .memConnection = {.instance = 1,
                    .dataLow4bit = 0x10,
                    .dataHigh4bit = 0x11,
                    .ss_b = 0x10,
                    .sclk = 0x10,
                    .dqs = 0xFF,
                    .sclk_n = 0xFF,
                    .rst_b = 0x00,
                    },
  .unittestEn = {.pulseInMs = 10,
                 .option.U = 0x7F
                 },
};
#else
pin_unittest_packet_t s_pinUnittestPacket;
#endif

/*! @brief Config system packet. */
config_system_packet_t s_configSystemPacket;

/*! @brief R/W Test packet. */
rw_test_packet_t s_rwTestPacket;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void mtu_get_command_from_buffer(void)
{
    bool isPacketTagFound = false;
    bool isCmdTagFound = false;
    uint32_t packetTag = 0;
    uint32_t gotTagBytes = 0;
    uint32_t remainingCmdBytes = 0;
    uint8_t *cmdPacket;
    while(1)
    {
        uint16_t tmprxIndex = g_rxIndex;
        if (tmprxIndex != g_txIndex)
        {
            if (!isPacketTagFound)
            {
                packetTag += (uint32_t)g_demoRingBuffer[g_txIndex] << (gotTagBytes * 8);
                gotTagBytes++;
                if (gotTagBytes == FRAMING_PACKET_TAG_BYTES)
                {
                    if (packetTag == FRAMING_PACKET_TAG_VALUE)
                    {
                        isPacketTagFound = true;
                    }
                    packetTag = 0;
                    gotTagBytes = 0;
                }
            }
            else
            {
                if (!isCmdTagFound)
                {
                    uint8_t currentCmdTag = g_demoRingBuffer[g_txIndex];
                    switch (currentCmdTag)
                    {
                        case kCommandTag_PinUnittest:
                            isCmdTagFound = true;
                            remainingCmdBytes = sizeof(pin_unittest_packet_t);
                            memset(&s_pinUnittestPacket, 0x0, sizeof(s_pinUnittestPacket));
                            cmdPacket = (uint8_t *)&s_pinUnittestPacket;
                            break;

                        case kCommandTag_ConfigSystem:
                            isCmdTagFound = true;
                            remainingCmdBytes = sizeof(config_system_packet_t);
                            memset(&s_configSystemPacket, 0x0, sizeof(s_configSystemPacket));
                            cmdPacket = (uint8_t *)&s_configSystemPacket;
                            break;

                        case kCommandTag_RunRwTest:
                            isCmdTagFound = true;
                            remainingCmdBytes = sizeof(rw_test_packet_t);
                            memset(&s_rwTestPacket, 0x0, sizeof(s_rwTestPacket));
                            cmdPacket = (uint8_t *)&s_rwTestPacket;
                            break;

                        case kCommandTag_TestStop:
                            isCmdTagFound = true;
                            remainingCmdBytes = 0;
                            break;

                        default:
                            isPacketTagFound = false;
                            break;
                    }
                    if (isCmdTagFound)
                    {
                        s_lastCmdTag = s_currentCmdTag;
                        s_currentCmdTag = currentCmdTag;
                    }
                }
                else
                {
                    if (remainingCmdBytes)
                    {
                        *cmdPacket = g_demoRingBuffer[g_txIndex];
                        cmdPacket++;
                        remainingCmdBytes--;
                        if (!remainingCmdBytes)
                        {
                            g_txIndex++;
                            g_txIndex %= DEMO_RING_BUFFER_SIZE;
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            g_txIndex++;
            g_txIndex %= DEMO_RING_BUFFER_SIZE;
        }
    }
}

static void mtu_switch_print_mode(bool isAsciiMode)
{
    SDK_DelayAtLeastUs(1000000, SystemCoreClock);
    if (isAsciiMode)
    {
        printf("Switch_To_ASCII_Mode\r\n");
    }
    else
    {
        printf("Switch_To_Hex_Mode\r\n");
    }
}

//! @brief Calculate crc over command packet.
#if MTU_FEATURE_PACKET_CRC
static bool mtu_is_packet_crc16_valid(const uint8_t *data, uint32_t length, uint16_t expectedCrc)
{
    uint16_t calculatedCrc;

    // Initialize the CRC16 information
    crc16_data_t crcInfo;
    crc16_init(&crcInfo);

    // Run CRC on any payload bytes
    crc16_update(&crcInfo, (uint8_t *)data, length);

    // Finalize the CRC calculations
    crc16_finalize(&crcInfo, &calculatedCrc);

    return (calculatedCrc == expectedCrc);
}
#endif

static bool mtu_is_command_valid(void)
{
    bool isCmdValid = false;
#if MTU_FEATURE_PACKET_CRC
    const uint8_t *crcStart;
    uint32_t crcLength;
    uint16_t expectedCrc;
#endif
    switch (s_currentCmdTag)
    {
        case kCommandTag_PinUnittest:
#if MTU_FEATURE_PACKET_CRC
            crcStart = (const uint8_t *)(&s_pinUnittestPacket);
            crcLength = (const uint8_t *)(&s_pinUnittestPacket.crcCheckSum) - crcStart;
            expectedCrc = s_pinUnittestPacket.crcCheckSum;
            isCmdValid = mtu_is_packet_crc16_valid(crcStart, crcLength, expectedCrc);
#else
            isCmdValid = true;
#endif
            if (isCmdValid)
            {
                printf("--Received Pin Unittest command. \r\n");
            }
            break;

        case kCommandTag_ConfigSystem:
#if MTU_FEATURE_PACKET_CRC
            crcStart = (const uint8_t *)(&s_configSystemPacket);
            crcLength = (const uint8_t *)(&s_configSystemPacket.crcCheckSum) - crcStart;
            expectedCrc = s_configSystemPacket.crcCheckSum;
            isCmdValid = mtu_is_packet_crc16_valid(crcStart, crcLength, expectedCrc);
#else
            isCmdValid = true;
#endif
            if (isCmdValid)
            {
                printf("--Received Config System command. \r\n");
            }
            break;

        case kCommandTag_RunRwTest:
#if MTU_FEATURE_PACKET_CRC
            crcStart = (const uint8_t *)(&s_rwTestPacket);
            crcLength = (const uint8_t *)(&s_rwTestPacket.crcCheckSum) - crcStart;
            expectedCrc = s_rwTestPacket.crcCheckSum;
            isCmdValid = mtu_is_packet_crc16_valid(crcStart, crcLength, expectedCrc);
#else
            isCmdValid = true;
#endif
            if (isCmdValid)
            {
                printf("--Received R/W test command. \r\n");
            }
            break;

        case kCommandTag_TestStop:
            isCmdValid = true;
            break;

        default:
            break;
    }

    if (!isCmdValid)
    {
        printf("--Received command packet, but invalid CRC found. \r\n");
    }

    return isCmdValid;
}

static void mtu_execute_command(void)
{
    switch (s_currentCmdTag)
    {
        case kCommandTag_PinUnittest:
#if MTU_FEATURE_PINTEST
            printf("--You can check wave on enabled pins now. \r\n");
#if MTU_FEATURE_PINTEST_WAVE
            bsp_adc_echo_info();
            mtu_switch_print_mode(false);
#endif
            mtu_deinit_timer();
            bsp_flexspi_pinmux_config(&s_pinUnittestPacket, true);
#if MTU_FEATURE_PINTEST_WAVE
            bsp_adc_init();
#endif
            mtu_init_timer(s_pinUnittestPacket.unittestEn.pulseInMs, (void *)bsp_flexspi_gpios_toggle);
#endif
            break;

        case kCommandTag_ConfigSystem:
#if MTU_FEATURE_FLASH
            bsp_flexspi_pinmux_config(&s_configSystemPacket, false);
            mtu_init_flash();
#endif
            break;

        case kCommandTag_RunRwTest:
#if MTU_FEATURE_FLASH
            mtu_rw_flash();
#endif
            break;

        case kCommandTag_TestStop:
            {
                if (s_lastCmdTag == kCommandTag_PinUnittest)
                {
#if MTU_FEATURE_PINTEST
                    mtu_deinit_timer();
#if MTU_FEATURE_PINTEST_WAVE
                    bsp_adc_deinit();
                    mtu_switch_print_mode(true);
#endif
#endif
                }
                printf("--Received stop command. \r\n");
            }
            break;

        default:
            break;
    }
}

/*!
 * @brief Main function
 */
void mtu_main(void)
{
    mtu_init_uart();

    while(1)
    {
#if !MTU_SELFTEST
        mtu_get_command_from_buffer();
        if (mtu_is_command_valid())
        {
            mtu_execute_command();
        }
#else
        mtu_execute_command();
        while(1);
#endif
    }
}

