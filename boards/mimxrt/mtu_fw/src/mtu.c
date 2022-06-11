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

#define DEMO_LPUART ((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 512

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

   
/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t s_tipString[] =
"i.MXRT117x flexspi memory test set v1.0\r\n\
Board receives command from UART1 then executes command";

/*
  Ring buffer for data input and output, in this example, input data are saved
  to ring buffer in IRQ handler. The main function polls the ring buffer status,
  if there are new data, then send them out.
  Ring buffer full: (((s_rxIndex + 1) % DEMO_RING_BUFFER_SIZE) == s_txIndex)
  Ring buffer empty: (s_rxIndex == s_txIndex)
*/
uint8_t s_demoRingBuffer[DEMO_RING_BUFFER_SIZE];
volatile uint16_t s_txIndex; /* Index of the command data that has been execute. */
volatile uint16_t s_rxIndex; /* Index of the memory to save new arrived command data. */

/* Hold one command string */
uint8_t s_demoCmdPacket[DEMO_RING_BUFFER_SIZE / 4];

/*******************************************************************************
 * Code
 ******************************************************************************/

#if __ICCARM__
size_t __write(int handle, const unsigned char *buf, size_t size)
{
    /* Send data. */  
    (void)LPUART_WriteBlocking(DEMO_LPUART, buf, size);

    return size;
}
#endif // __ICCARM__

void BOARD_UART_IRQ_HANDLER(void)
{
    uint8_t data;
    uint16_t tmprxIndex = s_rxIndex;
    uint16_t tmptxIndex = s_txIndex;

    /* If new data arrived. */
    if ((kLPUART_RxDataRegFullFlag)&LPUART_GetStatusFlags(DEMO_LPUART))
    {
        data = LPUART_ReadByte(DEMO_LPUART);

        /* If ring buffer is not full, add data to ring buffer. */
        if (((tmprxIndex + 1) % DEMO_RING_BUFFER_SIZE) != tmptxIndex)
        {
            s_demoRingBuffer[s_rxIndex] = data;
            s_rxIndex++;
            s_rxIndex %= DEMO_RING_BUFFER_SIZE;
        }
    }
    SDK_ISR_EXIT_BARRIER;
}

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

    LPUART_Init(DEMO_LPUART, &config, BOARD_DebugConsoleSrcFreq());

    /* Send s_tipString out. */
    LPUART_WriteBlocking(DEMO_LPUART, s_tipString, sizeof(s_tipString) / sizeof(s_tipString[0]));
    printf("\r\n");
    
    /* Enable RX interrupt. */
    LPUART_EnableInterrupts(DEMO_LPUART, kLPUART_RxDataRegFullInterruptEnable);
    EnableIRQ(BOARD_UART_IRQ);
}

void mtu_get_command_from_buffer(void)
{
    uint32_t packetTag = 0;
    uint32_t gotTagBytes = 0;
    while(1)
    {
        uint16_t tmprxIndex = s_rxIndex;
        if (tmprxIndex != s_txIndex)
        {
            packetTag += (uint32_t)s_demoRingBuffer[s_txIndex] << (gotTagBytes * 8);
            gotTagBytes++;
            s_txIndex++;
            s_txIndex %= DEMO_RING_BUFFER_SIZE;
            if (gotTagBytes == FRAMING_PACKET_TAG_BYTES)
            {
                if (packetTag == FRAMING_PACKET_TAG_VALUE)
                {
                    break;
                }
                gotTagBytes = 0;
            }
        }
    }
    printf("--Received command!\r\n");
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
        //if (mtu_is_command_valid())
        //{
        //    mtu_execute_command();
        //}
    }
}

