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

/*******************************************************************************
 * Definitions
 ******************************************************************************/

//! @brief Serial framing packet constants.
#define FRAMING_PACKET_TAG_BYTES (4)
#define FRAMING_PACKET_TAG_VALUE (0x47415446UL)     // ascii "FTAG" Big Endian

/*******************************************************************************
 * API
 ******************************************************************************/

void mtu_main(void);

#endif /* __MTU__ */
