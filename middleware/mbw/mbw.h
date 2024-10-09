
#ifndef __MBW_H__
#define __MBW_H__

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "mtu.h"

typedef struct _timeval
{
    uint32_t tv_sec;
    uint32_t tv_usec;
} timeval_t;

typedef struct _timezone {
   uint32_t tz_minuteswest;
   uint32_t tz_dsttime;
} timezone_t;

int gettimeofday(timeval_t *tv, timezone_t *tz);
void *mempcpy(void *restrict dest, const void *restrict src, size_t n);

#define MAX_MEM_REGIONS (8)

typedef struct _my_mem
{
   uint32_t memStart;
   uint32_t memSize;
   uint32_t regionStart[MAX_MEM_REGIONS];
   uint32_t regionSize[MAX_MEM_REGIONS];
} my_mem_t;

extern my_mem_t g_myMem;
void *my_calloc(size_t nitems, size_t size);
void my_free(void *ptr);

int mbw_main(uint32_t testno, uint32_t showavg, uint32_t nr_loops, uint64_t block_size, uint32_t mem_start, uint32_t mem_size) ;

#endif // __MBW_H__
