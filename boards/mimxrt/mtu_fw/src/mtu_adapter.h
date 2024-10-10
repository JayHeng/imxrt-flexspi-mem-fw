/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MTU_ADAPTER_H_
#define _MTU_ADAPTER_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

// Definition to select spi peripheral
#ifndef MTU_MIXSPI_MODULE
#define MTU_MIXSPI_MODULE MTU_MIXSPI_MODULE_IS_FLEXSPI
#endif

#if MTU_MIXSPI_MODULE == MTU_MIXSPI_MODULE_IS_FLEXSPI
#include "fsl_flexspi.h"
#define mixspi_read_sample_clock_t   flexspi_read_sample_clock_t  
#define mixspi_port_t                flexspi_port_t
#define MIXSPI_Type                  FLEXSPI_Type
#elif MTU_MIXSPI_MODULE == MTU_MIXSPI_MODULE_IS_QUADSPI
#include "fsl_qspi.h"
#define mixspi_read_sample_clock_t   qspi_dqs_read_sample_clock_t
#define mixspi_port_t                uint32_t
#define MIXSPI_Type                  QuadSPI_Type
#elif MTU_MIXSPI_MODULE == MTU_MIXSPI_MODULE_IS_XSPI
#include "fsl_xspi.h"
#define mixspi_read_sample_clock_t   xspi_sample_clk_source_t
#define mixspi_port_t                xspi_target_group_t
#define MIXSPI_Type                  XSPI_Type
#endif

// mem property info for operation
typedef struct _mixspi_user_config
{
    uint32_t                   instance;
    MIXSPI_Type                *mixspiBase;
    mixspi_port_t               mixspiPort;
    mixspi_root_clk_freq_t      mixspiRootClkFreq;
    mixspi_read_sample_clock_t  mixspiReadSampleClock;
    const uint32_t             *mixspiCustomLUTVendor;

    uint8_t  flashBusyStatusPol;
    uint8_t  flashBusyStatusOffset;
    uint8_t  flashMixStatusMask;
    uint8_t  reserved;
} mixspi_user_config_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MTU_ADAPTER_H_ */
