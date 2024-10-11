/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MTU_MEM_RAM_DEVICE_H_
#define _MTU_MEM_RAM_DEVICE_H_

#include "mtu.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * API
 ******************************************************************************/

status_t mtu_psram_set_registers_for_apmemory(mixspi_user_config_t *userConfig);

status_t mtu_psram_set_registers_for_issi(mixspi_user_config_t *userConfig);

#endif /* _MTU_MEM_RAM_DEVICE_H_ */
