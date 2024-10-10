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
/*
 * If cache is enabled, this example should maintain the cache to make sure
 * CPU core accesses the memory, not cache only.
 */
#define MTU_CACHE_MAINTAIN       (1)

#define MTU_FEATURE_PACKET_CRC   (1)

#define MTU_FEATURE_PIN_TEST        (1)
#define MTU_FEATURE_EXT_MEMORY      (1)
#define MTU_FEATURE_PERF_TEST       (1)
#define MTU_FEATURE_PERF_TEST_MBW   (1)
#define MTU_FEATURE_STRESS_TEST     (1)

#endif /* _MTU_CONFIG_H_ */
