/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MTU_CONFIG_H_
#define _MTU_CONFIG_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define __MTU_FW_VERSION__       "1.0"

#define MTU_SELFTEST             (0)
#define MTU_USE_EVK_SOCKET       (0)
#define MTU_USE_MEM_DC_MUX       (1)

#define MTU_FEATURE_PACKET_CRC   (1)

#define MTU_FEATURE_PINTEST         (1)
#define MTU_FEATURE_MEMORY          (1)
#define MTU_FEATURE_PERFTEST        (1)
#define MTU_FEATURE_PERFTEST_MBW    (1)
#define MTU_FEATURE_STRESSTEST      (1)

#endif /* _MTU_CONFIG_H_ */
