/*
 * Very simple but very effective user-space memory tester.
 * Originally by Simon Kirby <sim@stormix.com> <sim@neato.org>
 * Version 2 by Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Version 3 not publicly released.
 * Version 4 rewrite:
 * Copyright (C) 2004-2010 Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Licensed under the terms of the GNU General Public License version 2 (only).
 * See the file COPYING for details.
 *
 * This file contains typedefs, structure, and union definitions.
 *
 */

#ifndef __MEMTESTER_H__
#define __MEMTESTER_H__

#include "fsl_common.h"

typedef unsigned int ul;
typedef unsigned int ull;
typedef unsigned int volatile ulv;
typedef unsigned char volatile u8v;
typedef unsigned short volatile u16v;
typedef unsigned int off_t;

struct test {
    char *name;
    int (*fp)();
};

#define rand32() ((unsigned int) rand() | ( (unsigned int) rand() << 16))

#define ULONG_MAX       (4294967295UL)
#if (ULONG_MAX == 4294967295UL)
    #define rand_ul() rand32()
    #define UL_ONEBITS 0xffffffff
    #define UL_LEN 32
    #define CHECKERBOARD1 0x55555555
    #define CHECKERBOARD2 0xaaaaaaaa
    #define UL_BYTE(x) ((x | x << 8 | x << 16 | x << 24))
#elif (ULONG_MAX == 18446744073709551615ULL)
    #define rand64() (((ul) rand32()) << 32 | ((ul) rand32()))
    #define rand_ul() rand64()
    #define UL_ONEBITS 0xffffffffffffffffUL
    #define UL_LEN 64
    #define CHECKERBOARD1 0x5555555555555555
    #define CHECKERBOARD2 0xaaaaaaaaaaaaaaaa
    #define UL_BYTE(x) (((ul)x | (ul)x<<8 | (ul)x<<16 | (ul)x<<24 | (ul)x<<32 | (ul)x<<40 | (ul)x<<48 | (ul)x<<56))
#else
    #error long on this platform is not 32 or 64 bits
#endif

#define TEST_NARROW_WRITES 


/*******************************************************************************
*
*/  
void fflush(char data);
/*******************************************************************************
*
*/  
void putchar(char ch);
/*******************************************************************************
* For Ultra LPDDR2, 
* Column addressing : 1K(A[9:0])
* Page Size = 2^10 = 1024 = 1K * Memory Unit(16bit) = 2KB
*/  
int memtester_pagesize(void) ;
/* Function definitions. */

int compare_regions(ulv *bufa, ulv *bufb, size_t count) ;
/*******************************************************************************
*
*
*/
int test_stuck_address(ulv *bufa, size_t count) ;
/*******************************************************************************
*
*
*/
int test_random_value(ulv *bufa, ulv *bufb, size_t count) ;
/*******************************************************************************
*
*
*/
int test_xor_comparison(ulv *bufa, ulv *bufb, size_t count);
/*******************************************************************************
*
*
*/
int test_sub_comparison(ulv *bufa, ulv *bufb, size_t count) ;
/*******************************************************************************
*
*
*/
int test_mul_comparison(ulv *bufa, ulv *bufb, size_t count);
/*******************************************************************************
*
*
*/
int test_div_comparison(ulv *bufa, ulv *bufb, size_t count) ;
/*******************************************************************************
*
*
*/
int test_or_comparison(ulv *bufa, ulv *bufb, size_t count) ;
/*******************************************************************************
*
*
*/
int test_and_comparison(ulv *bufa, ulv *bufb, size_t count);
/*******************************************************************************
*
*
*/
int test_seqinc_comparison(ulv *bufa, ulv *bufb, size_t count) ;
/*******************************************************************************
*
*
*/
int test_solidbits_comparison(ulv *bufa, ulv *bufb, size_t count) ;

/*******************************************************************************
*
*
*/
int test_checkerboard_comparison(ulv *bufa, ulv *bufb, size_t count) ;
/*******************************************************************************
*
*
*/
int test_blockseq_comparison(ulv *bufa, ulv *bufb, size_t count) ;
/*******************************************************************************
*
*
*/
int test_walkbits0_comparison(ulv *bufa, ulv *bufb, size_t count) ;
/*******************************************************************************
*
*
*/
int test_walkbits1_comparison(ulv *bufa, ulv *bufb, size_t count) ;
/*******************************************************************************
*
*
*/
int test_bitspread_comparison(ulv *bufa, ulv *bufb, size_t count) ;
/*******************************************************************************
*
*
*/
int test_bitflip_comparison(ulv *bufa, ulv *bufb, size_t count) ;
/*******************************************************************************
*
*
*/
#ifdef TEST_NARROW_WRITES    
int test_8bit_wide_random(ulv* bufa, ulv* bufb, size_t count) ;
/*******************************************************************************
*
*
*/
int test_16bit_wide_random(ulv* bufa, ulv* bufb, size_t count);
#endif

int memtester_main(ul physaddrbase, ul wantraw, char memsuffix, ul loops,ul pagesize);
#endif