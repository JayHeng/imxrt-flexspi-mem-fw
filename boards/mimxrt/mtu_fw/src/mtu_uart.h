/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MTU_UART__
#define __MTU_UART__

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Used UART instance. */
#define DEMO_LPUART ((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 512

extern uint8_t g_demoRingBuffer[];
extern volatile uint16_t g_txIndex;
extern volatile uint16_t g_rxIndex;

/*******************************************************************************
 * API
 ******************************************************************************/

void mtu_init_uart(void);

#endif /* __MTU_UART__ */
