/*
* Very simple but very effective user-space memory tester.
* Originally by Simon Kirby <sim@stormix.com> <sim@neato.org>
* Version 2 by Charles Cazabon <charlesc-memtester@pyropus.ca>
* Version 3 not publicly released.
* Version 4 rewrite:
* Copyright (C) 2004-2012 Charles Cazabon <charlesc-memtester@pyropus.ca>
* Licensed under the terms of the GNU General Public License version 2 (only).
* See the file COPYING for details.
*
* This file contains the functions for the actual tests, called from the
* main routine in memtester.c.  See other comments in that file.
*
*/

#include "fsl_debug_console.h"
#include "memtester.h"

union {
   unsigned char bytes[UL_LEN/8];
   ul val;
} mword8;

union {
   unsigned short u16s[UL_LEN/16];
   ul val;
} mword16;

#define __version__ "4.3.0"

char progress[] = "-\\|/";
#define PROGRESSLEN 4
#define PROGRESSOFTEN 2500
#define ONE 0x00000001L

#define EXIT_FAIL_NONSTARTER    0x01
#define EXIT_FAIL_ADDRESSLINES  0x02
#define EXIT_FAIL_OTHERTEST     0x04


struct test tests[] = {
#if 1
   { "Random Value", test_random_value },
   { "Compare XOR", test_xor_comparison },
   { "Compare SUB", test_sub_comparison },
   { "Compare MUL", test_mul_comparison },
   { "Compare DIV",test_div_comparison },
   { "Compare OR", test_or_comparison },
   { "Compare AND", test_and_comparison },
   { "Sequential Increment", test_seqinc_comparison },
   { "Solid Bits", test_solidbits_comparison },
   { "Block Sequential", test_blockseq_comparison },
   { "Checkerboard", test_checkerboard_comparison },
#endif
   { "Bit Spread", test_bitspread_comparison },
   { "Bit Flip", test_bitflip_comparison },
   { "Walking Ones", test_walkbits1_comparison },
   { "Walking Zeroes", test_walkbits0_comparison },
#ifdef TEST_NARROW_WRITES  
    { "8-bit Writes", test_8bit_wide_random },
    { "16-bit Writes", test_16bit_wide_random },
#endif
    { NULL, NULL }
};


/* Global vars - so tests have access to this information  ********************/
int use_phys = 0;
off_t physaddrbase = 0;
extern int fail_stop;

/* Function definitions. */

int compare_regions(ulv *bufa, ulv *bufb, size_t count) {
    int r = 0;
    size_t i;
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    ulv c1;
    ulv c2;
    off_t physaddr;
    
    __asm volatile( "dsb" );
    for (i = 0; i < count; i++, p1++, p2++) {
    //for (i = 0; i < count; i++) {
        c1 = *p1;
        c2 = *p2;
        if (c1 != c2) {
            if (use_phys) {
                physaddr = physaddrbase + (i * sizeof(ul));
                //physaddr = physaddrbase;
                PRINTF("FAILURE: 0x%08x != 0x%08x at physical address "
                       "0x%08x.\r\n", c1, c2, physaddr);
            } else {

                PRINTF("FAILURE: 0x%08x != 0x%08x at offset 0x%08x.\r\n", 
                       (ul) *p1, (ul) *p2, (ul) (i * sizeof(ul)));
                
            }
            /* PRINTF("Skipping to next test..."); */
            r = -1;
        }
    }
    return r;
}
/*******************************************************************************
*
*
*/
int test_stuck_address(ulv *bufa, size_t count) {
    ulv *p1 = bufa;
    unsigned int j;
    size_t i;
    off_t physaddr;
    
    PRINTF("           ");
    for (j = 0; j < 16; j++) {
        PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
        p1 = (ulv *) bufa;
        PRINTF("setting %3u", j);
        for (i = 0; i < count; i++) {
            *p1 = ((j + i) % 2) == 0 ? (ul) p1 : ~((ul) p1);
            *p1++;
        }
        PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
        PRINTF("testing %3u", j);
        p1 = (ulv *) bufa;
        for (i = 0; i < count; i++, p1++) {
            if (*p1 != (((j + i) % 2) == 0 ? (ul) p1 : ~((ul) p1))) {
                if (use_phys) {
                    physaddr = physaddrbase + (i * sizeof(ul));
                    PRINTF("FAILURE: possible bad address line at physical "
                           "address 0x%08x = 0x%08x.\r\n", physaddr, *p1);
                } else {
                    PRINTF("FAILURE: possible bad address line at offset "
                           "0x%08x.\r\n", (ul) (i * sizeof(ul)));
                }
                PRINTF("Skipping to next test...\r\n");
                return -1;
            }
        }
    }
    PRINTF("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
    return 0;
}
/*******************************************************************************
*
*
*/
int test_random_value(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    ul j = 0;
    size_t i;
    
    PUTCHAR(' ');
    for (i = 0; i < count; i++) {
        *p1++ = *p2++ = rand_ul();
        if (!(i % PROGRESSOFTEN)) {
            PUTCHAR('\b');
            PUTCHAR(progress[++j % PROGRESSLEN]);
        }
    }
    PRINTF("\b \b");
    return compare_regions(bufa, bufb, count);
}
/*******************************************************************************
*
*
*/
int test_xor_comparison(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();
    
    for (i = 0; i < count; i++) {
        *p1++ ^= q;
        *p2++ ^= q;
    }
    return compare_regions(bufa, bufb, count);
}
/*******************************************************************************
*
*
*/
int test_sub_comparison(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();
    
    for (i = 0; i < count; i++) {
        *p1++ -= q;
        *p2++ -= q;
    }
    return compare_regions(bufa, bufb, count);
}
/*******************************************************************************
*
*
*/
int test_mul_comparison(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();
    
    for (i = 0; i < count; i++) {
        *p1++ *= q;
        *p2++ *= q;
    }
    return compare_regions(bufa, bufb, count);
}
/*******************************************************************************
*
*
*/
int test_div_comparison(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();
    
    for (i = 0; i < count; i++) {
        if (!q) {
            q++;
        }
        *p1++ /= q;
        *p2++ /= q;
    }
    return compare_regions(bufa, bufb, count);
}
/*******************************************************************************
*
*
*/
int test_or_comparison(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();
    
    for (i = 0; i < count; i++) {
        *p1++ |= q;
        *p2++ |= q;
    }
    return compare_regions(bufa, bufb, count);
}
/*******************************************************************************
*
*
*/
int test_and_comparison(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();
    
    for (i = 0; i < count; i++) {
        *p1++ &= q;
        *p2++ &= q;
    }
    return compare_regions(bufa, bufb, count);
}
/*******************************************************************************
*
*
*/
int test_seqinc_comparison(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();
    
    for (i = 0; i < count; i++) {
        *p1++ = *p2++ = (i + q);
    }
    return compare_regions(bufa, bufb, count);
}
/*******************************************************************************
*
*
*/
int test_solidbits_comparison(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    ul q;
    size_t i;
    
    PRINTF("           ");
    for (j = 0; j < 64; j++) {
        PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
        q = (j % 2) == 0 ? UL_ONEBITS : 0;
        PRINTF("setting %3u", j);
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        for (i = 0; i < count; i++) {
            *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
        }
        PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
        PRINTF("testing %3u", j);
        if (compare_regions(bufa, bufb, count)) {
            return -1;
        }
    }
    PRINTF("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
    return 0;
}
/*******************************************************************************
*
*
*/
int test_checkerboard_comparison(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    ul q;
    size_t i;
    
    PRINTF("           ");
    for (j = 0; j < 64; j++) {
        PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
        q = (j % 2) == 0 ? CHECKERBOARD1 : CHECKERBOARD2;
        PRINTF("setting %3u", j);
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        for (i = 0; i < count; i++) {
            *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
        }
        PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
        PRINTF("testing %3u", j);
        if (compare_regions(bufa, bufb, count)) {
            return -1;
        }
    }
    PRINTF("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
    return 0;
}
/*******************************************************************************
*
*
*/
int test_blockseq_comparison(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;
    
    PRINTF("           ");
    for (j = 0; j < 256; j++) {
        PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        PRINTF("setting %3u", j);
        for (i = 0; i < count; i++) {
            *p1++ = *p2++ = (ul) UL_BYTE(j);
        }
        PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
        PRINTF("testing %3u", j);
        if (compare_regions(bufa, bufb, count)) {
            return -1;
        }
    }
    PRINTF("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
    return 0;
}
/*******************************************************************************
*
*
*/
int test_walkbits0_comparison(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;
    
    PRINTF("           ");
    for (j = 0; j < UL_LEN * 2; j++) {
        PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        PRINTF("setting %3u", j);
        for (i = 0; i < count; i++) {
            if (j < UL_LEN) { /* Walk it up. */
                *p1++ = *p2++ = ONE << j;
            } else { /* Walk it back down. */
                *p1++ = *p2++ = ONE << (UL_LEN * 2 - j - 1);
            }
        }
        PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
        PRINTF("testing %3u", j);
        if (compare_regions(bufa, bufb, count)) {
            return -1;
        }
    }
    PRINTF("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
    return 0;
}
/*******************************************************************************
*
*
*/
int test_walkbits1_comparison(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;
    
    PRINTF("           ");
    for (j = 0; j < UL_LEN * 2; j++) {
        PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        PRINTF("setting %3u", j);
        for (i = 0; i < count; i++) {
            if (j < UL_LEN) { /* Walk it up. */
                *p1++ = *p2++ = UL_ONEBITS ^ (ONE << j);
            } else { /* Walk it back down. */
                *p1++ = *p2++ = UL_ONEBITS ^ (ONE << (UL_LEN * 2 - j - 1));
            }
        }
        PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
        PRINTF("testing %3u", j);
        if (compare_regions(bufa, bufb, count)) {
            return -1;
        }
    }
    PRINTF("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
    return 0;
}
/*******************************************************************************
*
*
*/
int test_bitspread_comparison(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;
    
    PRINTF("           ");
    for (j = 0; j < UL_LEN * 2; j++) {
        PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        PRINTF("setting %3u", j);
        for (i = 0; i < count; i++) {
            if (j < UL_LEN) { /* Walk it up. */
                *p1++ = *p2++ = (i % 2 == 0)
                    ? (ONE << j) | (ONE << (j + 2))
                        : UL_ONEBITS ^ ((ONE << j)
                                        | (ONE << (j + 2)));
            } else { /* Walk it back down. */
                *p1++ = *p2++ = (i % 2 == 0)
                    ? (ONE << (UL_LEN * 2 - 1 - j)) | (ONE << (UL_LEN * 2 + 1 - j))
                        : UL_ONEBITS ^ (ONE << (UL_LEN * 2 - 1 - j)
                                        | (ONE << (UL_LEN * 2 + 1 - j)));
            }
        }
        PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
        PRINTF("testing %3u", j);
        if (compare_regions(bufa, bufb, count)) {
            return -1;
        }
    }
    PRINTF("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
    return 0;
}
/*******************************************************************************
*
*
*/
int test_bitflip_comparison(ulv *bufa, ulv *bufb, size_t count) {
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j, k;
    ul q;
    size_t i;
    
    PRINTF("           ");
    for (k = 0; k < UL_LEN; k++) {
        q = ONE << k;
        for (j = 0; j < 8; j++) {
            PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
            q = ~q;
            PRINTF("setting %3u", k * 8 + j);
            p1 = (ulv *) bufa;
            p2 = (ulv *) bufb;
            for (i = 0; i < count; i++) {
                *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
            }
            PRINTF("\b\b\b\b\b\b\b\b\b\b\b");
            PRINTF("testing %3u", k * 8 + j);
            if (compare_regions(bufa, bufb, count)) {
                return -1;
            }
        }
    }
    PRINTF("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
    return 0;
}
/*******************************************************************************
*
*
*/
#ifdef TEST_NARROW_WRITES    
int test_8bit_wide_random(ulv* bufa, ulv* bufb, size_t count) {
    u8v *p1, *t;
    ulv *p2;
    int attempt;
    unsigned int b, j = 0;
    size_t i;
    
    PUTCHAR(' ');
    for (attempt = 0; attempt < 2;  attempt++) {
        if (attempt & 1) {
            p1 = (u8v *) bufa;
            p2 = bufb;
        } else {
            p1 = (u8v *) bufb;
            p2 = bufa;
        }
        for (i = 0; i < count; i++) {
            t = mword8.bytes;
            *p2++ = mword8.val = rand_ul();
            for (b=0; b < UL_LEN/8; b++) {
                *p1++ = *t++;
            }
            if (!(i % PROGRESSOFTEN)) {
                PUTCHAR('\b');
                PUTCHAR(progress[++j % PROGRESSLEN]);
            }
        }
        if (compare_regions(bufa, bufb, count)) {
            return -1;
        }
    }
    PRINTF("\b \b");
    return 0;
}
/*******************************************************************************
*
*
*/
int test_16bit_wide_random(ulv* bufa, ulv* bufb, size_t count) {
    u16v *p1, *t;
    ulv *p2;
    int attempt;
    unsigned int b, j = 0;
    size_t i;
    
    PUTCHAR( ' ' );
    for (attempt = 0; attempt < 2; attempt++) {
        if (attempt & 1) {
            p1 = (u16v *) bufa;
            p2 = bufb;
        } else {
            p1 = (u16v *) bufb;
            p2 = bufa;
        }
        for (i = 0; i < count; i++) {
            t = mword16.u16s;
            *p2++ = mword16.val = rand_ul();
            for (b = 0; b < UL_LEN/16; b++) {
                *p1++ = *t++;
            }
            if (!(i % PROGRESSOFTEN)) {
                PUTCHAR('\b');
                PUTCHAR(progress[++j % PROGRESSLEN]);
            }
        }
        if (compare_regions(bufa, bufb, count)) {
            return -1;
        }
    }
    PRINTF("\b \b");
    return 0;
}
#endif
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* Input parameters:
* ---- phystestbase : memory base address
* ---- wantraw      : memory size
* ---- memsuffix    : memory unit, B,K,M,G for B, KB, MB, GB
* ---- loop         : memory test code loop times
* ---- pagesize     : memory pase size (Bytes)
*/
int memtester_main(ul phystestbase, ul wantraw, char memsuffix, ul loops, ul pagesize) 
{
    ul loop, i;
    size_t  wantmb, wantbytes, wantbytes_orig, bufsize,
         halflen, count;
    //ptrdiff_t pagesizemask;
    void volatile *buf, *aligned;
    ulv *bufa, *bufb;
    int exit_code = 0;
    int memshift;
    size_t maxbytes = -1; /* addressable memory, in bytes */
    size_t maxmb = (maxbytes >> 20) + 1; /* addressable memory, in MB */
    ul testmask = 0;

    physaddrbase = phystestbase;
    PRINTF("\r\n\r\nmemtester version " __version__ " (%d-bit)\r\n", UL_LEN);
    PRINTF("Copyright (C) 2001-2012 Charles Cazabon.\r\n");
    PRINTF("Licensed under the GNU General Public License version 2 (only).\r\n");
    PRINTF("\r\n");

    //PRINTF("pagesize is %d\r\n", pagesize);
    //pagesizemask = (ptrdiff_t) ~(pagesize - 1);
    //PRINTF("pagesizemask is 0x%x\r\n", pagesizemask);
    
    switch (memsuffix) {
          case 'G':
          case 'g':
              memshift = 30; /* gigabytes */
              break;
          case 'M':
          case 'm':
              memshift = 20; /* megabytes */
              break;
          case 'K':
          case 'k':
              memshift = 10; /* kilobytes */
              break;
          case 'B':
          case 'b':
              memshift = 0; /* bytes*/
              break;
          default:
              PRINTF("ERR!!! memsuffix input not B|K|M|G  \r\n");
              goto __MMETESTER_EXIT__;
      }
#if 0
    /*physaddrbase check...*/
    if(physaddrbase & (pagesize-1)){
        PRINTF("ERR!! bad physaddrbase, does not start on page boundary \r\n");
        goto __MMETESTER_EXIT__;
    }
#endif
    /*set use_phys*/
    use_phys = 1;

    wantbytes_orig = wantbytes = ((size_t) wantraw << memshift);
    wantmb = (wantbytes_orig >> 20);
    if (wantmb > maxmb) {
        PRINTF("This system can only address %llu MB.\r\n", (ull) maxmb);
        goto __MMETESTER_EXIT__;
    }
#if 0
    if (wantbytes < pagesize) {
        PRINTF("bytes %d < pagesize %d -- memory argument too large?\r\n",
                wantbytes, pagesize);
        goto __MMETESTER_EXIT__;
    }
#endif
    PRINTF("want %dMB (%d bytes)\r\n", (ull)wantmb, (ull)wantbytes);
    buf = NULL;

    bufsize = wantbytes; /* accept no less */
    buf = (unsigned int *)physaddrbase;
    aligned = buf;  

    halflen = bufsize / 2;
    count = halflen / sizeof(ul);
    bufa = (ulv *) aligned;
    bufb = (ulv *) ((size_t) aligned + halflen);

    for(loop=1; ((!loops) || loop <= loops); loop++) {
        PRINTF("Loop %d", loop);
        if (loops) {
            PRINTF("/%d", loops);
        }
        PRINTF(":\r\n");
        PRINTF("  %s: ", "Stuck Address");
        if (!test_stuck_address(aligned, bufsize / sizeof(ul))) {
             PRINTF("ok\r\n");
        } else {
            exit_code |= EXIT_FAIL_ADDRESSLINES;
            if (fail_stop)
              break;
        }
        for (i=0;;i++) {
            if (!tests[i].name) break;
            /* If using a custom testmask, only run this test if the
               bit corresponding to this test was set by the user.
             */
            if (testmask && (!((1 << i) & testmask))) {
                continue;
            }
            PRINTF("  %s: ", tests[i].name);
            if (!tests[i].fp(bufa, bufb, count)) {
                PRINTF("ok\r\n");
            } else {
                exit_code |= EXIT_FAIL_OTHERTEST;
                if (fail_stop)
                  break;
            }
        }
        PRINTF("\r\n");
    }
    if (exit_code)
      PRINTF("Done and Failed!\r\n");
    else
      PRINTF("Done and Passed!\r\n");
    PRINTF("exit_code 0x%x \r\n", exit_code);
    
__MMETESTER_EXIT__:
    /* sent '\a' to tell DRAM tool the test finished */
    PRINTF("\a");
    return 0;
}