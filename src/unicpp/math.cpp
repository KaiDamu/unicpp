#include "math.hpp"

#ifdef PROG_COMPILER_MSVC
    #include <intrin.h>
#endif

dfa U8 MulU16(U8 a, U8 b, U8& hi)
{
#if IS_U16_SUPPORT
    cx AU r = U16(a) * U16(b);
    hi = U8(r >> sizb(U8));
    ret U8(r);
#elif defined(PROG_COMPILER_MSVC)
    ret _umul128(a, b, &hi);
#else
    cx AU aLo = U8(U4(a));
    cx AU aHi = a >> sizb(U4);
    cx AU bLo = U8(U4(b));
    cx AU bHi = b >> sizb(U4);
    cx AU p0 = aLo * bLo;
    cx AU p1 = aLo * bHi;
    cx AU p2 = aHi * bLo;
    cx AU p3 = aHi * bHi;
    cx AU mid = (p0 >> sizb(U4)) + (p1 & 0xFFFFFFFF) + (p2 & 0xFFFFFFFF);
    hi = p3 + (p1 >> sizb(U4)) + (p2 >> sizb(U4)) + (mid >> sizb(U4));
    ret (p0 & 0xFFFFFFFF) | (mid << sizb(U4));
#endif
}
dfa SI BitToByteSize(SI size)
{
    ret DivCeil(size, BIT_IN_BYTE);
}
dfa BO VarintIsIncomplete(cx U1* dat, SI size)
{
    ifu (size < 1)
        ret YES;
    ret (dat[size - 1] & 0x80) ? YES : NO;
}
dfa BO VarbaseintIsIncomplete(cx U1* dat, SI size, U1 base)
{
    ifu (size < 1)
        ret YES;
    ret dat[size - 1] >= U1(U1(0xFF) << base);
}
dfa U1 ByteObfuscate(U1 val, U1 i)
{
    ret U1((val ^ 0x55) + (i ^ 0xAA));
}
dfa U1 ByteUnobfuscate(U1 val, U1 i)
{
    ret U1(val - (i ^ 0xAA)) ^ 0x55;
}

tpl0 dfa F4 Abs(F4 val)
{
    cx U4& valBits = AsType<U4>(val);
    ret AsType<F4>(valBits & U4(0x7FFFFFFF));
}
tpl0 dfa F8 Abs(F8 val)
{
    cx U8& valBits = AsType<U8>(val);
    ret AsType<F8>(valBits & U8(0x7FFFFFFFFFFFFFFF));
}
tpl0 dfa S8 RoundToInt<>(F4 val)
{
#if IS_SSE_SUPPORT
    ret S8(_mm_cvtss_si32(_mm_set_ss(val)));
#else
    ret S8(std::llround(val));
#endif
}
tpl0 dfa S8 RoundToInt<>(F8 val)
{
#if IS_SSE2_SUPPORT
    ret S8(_mm_cvtsd_si64(_mm_set_sd(val)));
#else
    ret S8(std::llround(val));
#endif
}
tpl0 dfa F4 Sqrt<F4>(F4 val)
{
#ifdef PROG_COMPILER_GCC
    ifu (val < F4(0))
        ret -__builtin_sqrtf(-val); // non-standard
    ret __builtin_sqrtf(val);
#else
    ifu (val < F4(0))
        ret -sqrtf(-val); // non-standard
    ret sqrtf(val);
#endif
}
tpl0 dfa F8 Sqrt<F8>(F8 val)
{
#ifdef PROG_COMPILER_GCC
    ifu (val < F8(0))
        ret -__builtin_sqrt(-val); // non-standard
    ret __builtin_sqrt(val);
#else
    ifu (val < F8(0))
        ret -sqrt(-val); // non-standard
    ret sqrt(val);
#endif
}
tpl0 dfa F4 Sin<F4>(F4 val)
{
#ifdef PROG_COMPILER_GCC
    ret __builtin_sinf(val);
#else
    ret sinf(val);
#endif
}
tpl0 dfa F4 Cos<F4>(F4 val)
{
#ifdef PROG_COMPILER_GCC
    ret __builtin_cosf(val);
#else
    ret cosf(val);
#endif
}
tpl0 dfa F4 Atan<F4>(F4 x, F4 y)
{
#ifdef PROG_COMPILER_GCC
    ret __builtin_atan2f(y, x);
#else
    ret atan2f(y, x);
#endif
}
tpl0 dfa SI LenBin<>(U8 val)
{
    ifu (val == 0)
        ret 1;
#if defined(PROG_COMPILER_GCC)
    ret 64 - __builtin_clzll(val);
#elif defined(PROG_COMPILER_MSVC)
    unsigned long r;
    _BitScanReverse64(&r, val);
    ret r + 1;
#else
    SI len = 0;
    while (val > 0)
    {
        val >>= 1;
        ++len;
    }
    ret len;
#endif
}
tpl0 dfa SI LenBin<>(U4 val)
{
    ifu (val == 0)
        ret 1;
#if defined(PROG_COMPILER_GCC)
    ret 32 - __builtin_clz(val);
#elif defined(PROG_COMPILER_MSVC)
    unsigned long r;
    _BitScanReverse(&r, val);
    ret r + 1;
#else
    SI len = 0;
    while (val > 0)
    {
        val >>= 1;
        ++len;
    }
    ret len;
#endif
}
tpl0 dfa SI LenBin<>(U2 val)
{
    ret LenBin<U4>(val);
}
tpl0 dfa SI LenBin<>(U1 val)
{
    ret LenBin<U4>(val);
}
