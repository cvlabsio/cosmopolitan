/* clang-format off */
/* ===-- floattisf.c - Implement __floattisf -------------------------------===
 *
 *                     The LLVM Compiler Infrastructure
 *
 * This file is dual licensed under the MIT and the University of Illinois Open
 * Source Licenses. See LICENSE.TXT for details.
 *
 * ===----------------------------------------------------------------------===
 *
 * This file implements __floattisf for the compiler_rt library.
 *
 * ===----------------------------------------------------------------------===
 */

__static_yoink("huge_compiler_rt_license");

#include "third_party/compiler_rt/int_lib.h"

#ifdef CRT_HAS_128BIT

/* Returns: convert a to a float, rounding toward even. */

/* Assumption: float is a IEEE 32 bit floating point type 
 *             ti_int is a 128 bit integral type
 */

/* seee eeee emmm mmmm mmmm mmmm mmmm mmmm */

COMPILER_RT_ABI float
__floattisf(ti_int a)
{
    if (a == 0)
        return 0.0F;
    const unsigned N = sizeof(ti_int) * CHAR_BIT;
    const ti_int s = a >> (N-1);
    a = (a ^ s) - s;
    int sd = N - __clzti2(a);  /* number of significant digits */
    int e = sd - 1;             /* exponent */
    if (sd > FLT_MANT_DIG)
    {
        /*  start:  0000000000000000000001xxxxxxxxxxxxxxxxxxxxxxPQxxxxxxxxxxxxxxxxxx
        * finish: 000000000000000000000000000000000000001xxxxxxxxxxxxxxxxxxxxxxPQR
        *                                                12345678901234567890123456
        *  1 = msb 1 bit
        *  P = bit FLT_MANT_DIG-1 bits to the right of 1
        *  Q = bit FLT_MANT_DIG bits to the right of 1
        *  R = "or" of all bits to the right of Q
        */
        switch (sd)
        {
        case FLT_MANT_DIG + 1:
            a <<= 1;
            break;
        case FLT_MANT_DIG + 2:
            break;
        default:
            a = ((tu_int)a >> (sd - (FLT_MANT_DIG+2))) |
                ((a & ((tu_int)(-1) >> ((N + FLT_MANT_DIG+2) - sd))) != 0);
        };
        /* finish: */
        a |= (a & 4) != 0;  /* Or P into R */
        ++a;  /* round - this step may add a significant bit */
        a >>= 2;  /* dump Q and R */
        /* a is now rounded to FLT_MANT_DIG or FLT_MANT_DIG+1 bits */
        if (a & ((tu_int)1 << FLT_MANT_DIG))
        {
            a >>= 1;
            ++e;
        }
        /* a is now rounded to FLT_MANT_DIG bits */
    }
    else
    {
        a <<= (FLT_MANT_DIG - sd);
        /* a is now rounded to FLT_MANT_DIG bits */
    }
    float_bits fb;
    fb.u = ((su_int)s & 0x80000000) |  /* sign */
           ((e + 127) << 23)       |  /* exponent */
           ((su_int)a & 0x007FFFFF);   /* mantissa */
    return fb.f;
}

#endif /* CRT_HAS_128BIT */
