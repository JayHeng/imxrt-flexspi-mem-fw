/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MTU_FLEXSPI_NOT_TEST_H_
#define _MTU_FLEXSPI_NOT_TEST_H_

#include <stdint.h>
#include <stdbool.h>
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * API
 ******************************************************************************/

status_t mtu_init_flash(void);

status_t mtu_rw_flash(void);

#endif /* _MTU_FLEXSPI_NOT_TEST_H_ */
