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
#include "mtu_flexspi_nor_test.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

   
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Current command tag. */
uint8_t s_currentCmdTag = 0;

/*! @brief Pin unit test packet. */
pin_unittest_packet_t s_pinUnittestPacket;

/*! @brief Config system packet. */
config_system_packet_t s_configSystemPacket;

/*! @brief R/W Test packet. */
rw_test_packet_t s_rwTestPacket;

/*******************************************************************************
 * Code
 ******************************************************************************/

void mtu_get_command_from_buffer(void)
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
                    s_currentCmdTag = g_demoRingBuffer[g_txIndex];
                    switch (s_currentCmdTag)
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

                        default:
                            isPacketTagFound = false;
                            break;
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

void mtu_switch_print_mode(bool isAsciiMode)
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

bool mtu_is_command_valid(void)
{
    bool isCmdValid = false;
    switch (s_currentCmdTag)
    {
        case kCommandTag_PinUnittest:
            isCmdValid = true;
            mtu_switch_print_mode(true);
            printf("--Received Pin Unittest command. \r\n");
            break;

        case kCommandTag_ConfigSystem:
            isCmdValid = true;
            mtu_switch_print_mode(true);
            printf("--Received Config System command. \r\n");
            break;

        case kCommandTag_RunRwTest:
            isCmdValid = true;
            mtu_switch_print_mode(true);
            printf("--Received R/W test command. \r\n");
            break;

        default:
            break;
    }
    
    return isCmdValid;
}

void mtu_execute_command(void)
{
    switch (s_currentCmdTag)
    {
        case kCommandTag_PinUnittest:
            printf("--You can check wave on enabled pins now. \r\n");
            printf("--adc1_in0 can be used to sample pin wave. \r\n");
            mtu_switch_print_mode(false);
            mtu_deinit_timer();
            bsp_flexspi_pinmux_config(&s_pinUnittestPacket, true);
            bsp_adc_init();
            mtu_init_timer(s_pinUnittestPacket.unittestEn.pulseInMs, (void *)bsp_flexspi_gpios_toggle);
            break;

        case kCommandTag_ConfigSystem:
            mtu_deinit_timer();
            bsp_adc_deinit();
            bsp_flexspi_pinmux_config(&s_configSystemPacket, false);
            mtu_init_flash();
            break;

        case kCommandTag_RunRwTest:
            mtu_rw_flash();
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
        mtu_get_command_from_buffer();
        if (mtu_is_command_valid())
        {
            mtu_execute_command();
        }
    }
}

