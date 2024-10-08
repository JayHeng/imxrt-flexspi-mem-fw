/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <ctype.h>
#include <stdio.h>
#include "board.h"
#include "fsl_usart.h"
#include "mtu.h"
#include "mtu_uart.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

   
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Welcome string. */
uint8_t s_tipString[] =
"i.MXRT flexspi memory test set FW v0.1\r\n\
Board receives command from UART1 then executes command.";

/*
  Ring buffer for data input and output, in this example, input data are saved
  to ring buffer in IRQ handler. The main function polls the ring buffer status,
  if there are new data, then send them out.
  Ring buffer full: (((g_rxIndex + 1) % DEMO_RING_BUFFER_SIZE) == g_txIndex)
  Ring buffer empty: (g_rxIndex == g_txIndex)
*/
uint8_t g_demoRingBuffer[DEMO_RING_BUFFER_SIZE];
volatile uint16_t g_txIndex; /* Index of the command data that has been execute. */
volatile uint16_t g_rxIndex; /* Index of the memory to save new arrived command data. */

/*******************************************************************************
 * Code
 ******************************************************************************/

#if __ICCARM__
size_t __write(int handle, const unsigned char *buf, size_t size)
{
    /* Send data. */  
    (void)USART_WriteBlocking(DEMO_UART, buf, size);

    return size;
}
#endif // __ICCARM__

void BOARD_UART_IRQ_HANDLER(void)
{
    uint8_t data;
    uint16_t tmprxIndex = g_rxIndex;
    uint16_t tmptxIndex = g_txIndex;

    /* If new data arrived. */
    if ((kUSART_RxFifoNotEmptyFlag | kUSART_RxError) & USART_GetStatusFlags(DEMO_UART))
    {
        data = USART_ReadByte(DEMO_UART);

        /* If ring buffer is not full, add data to ring buffer. */
        if (((tmprxIndex + 1) % DEMO_RING_BUFFER_SIZE) != tmptxIndex)
        {
            g_demoRingBuffer[g_rxIndex] = data;
            g_rxIndex++;
            g_rxIndex %= DEMO_RING_BUFFER_SIZE;
        }
    }
    SDK_ISR_EXIT_BARRIER;
}

void mtu_init_uart(void)
{
    usart_config_t config;

    /* attach FRG0 clock to FLEXCOMM0 (debug console) */
    CLOCK_SetFRGClock(BOARD_DEBUG_UART_FRG_CLK);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUSART_ParityDisabled;
     * config.stopBitCount = kUSART_OneStopBit;
     * config.loopback = false;
     * config.enableTxFifo = false;
     * config.enableRxFifo = false;
     */
    USART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    USART_Init(DEMO_UART, &config, BOARD_DEBUG_UART_CLK_FREQ);

    /* Send s_tipString out. */
    USART_WriteBlocking(DEMO_UART, s_tipString, sizeof(s_tipString) / sizeof(s_tipString[0]));
    printf("\r\n");
    
    /* Enable RX interrupt. */
    USART_EnableInterrupts(DEMO_UART, kUSART_RxLevelInterruptEnable | kUSART_RxErrorInterruptEnable);
    EnableIRQ(BOARD_UART_IRQ);
}

void mtu_uart_sendhex(uint8_t *src, uint32_t lenInBytes)
{
    USART_WriteBlocking(DEMO_UART, src, lenInBytes);
}


