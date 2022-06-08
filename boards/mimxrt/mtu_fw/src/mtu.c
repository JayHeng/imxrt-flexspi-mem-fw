/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <ctype.h>
#include <stdio.h>
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_lpuart.h"
#include "mtu.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

   
/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t s_tipString[] =
"i.MXRT117x flexspi memory test set v1.0\r\n\
Board receives command from UART1 then executes command";

/*******************************************************************************
 * Code
 ******************************************************************************/

void mtu_init_uart(void)
{
    lpuart_config_t config;

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kLPUART_ParityDisabled;
     * config.stopBitCount = kLPUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 0;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    LPUART_Init((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR, &config, BOARD_DebugConsoleSrcFreq());

    /* Send s_tipString out. */
    LPUART_WriteBlocking((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR, s_tipString, sizeof(s_tipString) / sizeof(s_tipString[0]));
    printf("\r\n");
}

/*!
 * @brief Main function
 */
void mtu_main(void)
{
    mtu_init_uart();

    while(1);
}

