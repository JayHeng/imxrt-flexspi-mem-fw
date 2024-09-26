/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MTU__
#define __MTU__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "fsl_common_arm.h"
#include "port_cfg_mtu.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

//! @brief Serial framing packet constants.
#define FRAMING_PACKET_TAG_BYTES (4)
#define FRAMING_PACKET_TAG_VALUE (0x47415446UL)     // ascii "FTAG" Big Endian

//! @brief Commands codes.
enum _command_tags
{
    kCommandTag_PinUnittest     = 0x01,
    kCommandTag_ConfigSystem    = 0x02,
    kCommandTag_GetMemInfo      = 0x03,
    kCommandTag_RunRwTest       = 0x04,
    kCommandTag_RunPerfTest     = 0x05,
    kCommandTag_RunStressTest   = 0x06,

    kCommandTag_TestStop        = 0xF0,

    //! Maximum linearly incrementing command tag value.
    kInvalidCommandTag,
};

//! @brief Flexspi pin connection sel code.
typedef struct _flexspi_connection
{
    uint8_t instance;
    uint8_t dataLow4bit;
    uint8_t dataHigh4bit;
    uint8_t dataTop8bit;
    uint8_t ss_b;
    uint8_t sclk;
    uint8_t sclk_n;
    uint8_t dqs0;
    uint8_t dqs1;
    uint8_t rst_b;
    uint8_t reserved0[2];
} flexspi_connection_t;

//! @brief Flexspi pin uint test option.
typedef struct _flexspi_unittest_en
{
    uint32_t pulseInMs;        // Set Pulse time to toggle GPIO
    uint8_t enableAdcSample;   // Whether to use ADC to sample GPIO
    uint8_t reserved0[3];
    union
    {
        struct
        {
            uint32_t dataLow4bit : 1;
            uint32_t dataHigh4bit : 1;
            uint32_t dataTop8bit : 1;
            uint32_t ss_b : 1;
            uint32_t sclk : 1;
            uint32_t sclk_n : 1;
            uint32_t dqs0 : 1;
            uint32_t dqs1 : 1;
            uint32_t rst_b : 1;
            uint32_t reserved0 : 23;
        } B;
        uint32_t U;
    } option;
} flexspi_unittest_en_t;

typedef struct _pin_unittest_packet
{
    flexspi_connection_t memConnection;
    flexspi_unittest_en_t unittestEn;
    uint16_t crcCheckSum;
    uint8_t reserved0[2];
} pin_unittest_packet_t;

//! @brief Flexspi memory property option.
typedef struct _memory_property
{
    uint8_t type;
    uint8_t chip;
    uint16_t speedMHz;
    uint32_t sizeInByte;
    uint8_t ioMode;
    uint8_t interfaceMode;
    uint8_t dataRateMode;
    uint8_t dummyCycles;
} memory_property_t;

typedef struct _config_system_packet
{
    uint16_t cpuSpeedMHz;
    uint8_t enableL1Cache;
    uint8_t enablePreftech;
    uint16_t prefetchBufSizeInByte;
    uint8_t reserved0[2];
    flexspi_connection_t memConnection;
    memory_property_t memProperty;
    uint16_t crcCheckSum;
    uint8_t reserved1[2];
} config_system_packet_t;

typedef struct _rw_test_packet
{
    uint32_t memStart;
    uint32_t memLen;
    uint16_t crcCheckSum;
    uint8_t reserved1[2];
} rw_test_packet_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern pin_unittest_packet_t s_pinUnittestPacket;
extern config_system_packet_t s_configSystemPacket;
extern rw_test_packet_t s_rwTestPacket;

/*******************************************************************************
 * API
 ******************************************************************************/

void mtu_main(void);

#endif /* __MTU__ */
