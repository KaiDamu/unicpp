#pragma once

tpl1 dfa T1 Min(T1 a, T1 b)
{
    ret ((a < b) ? a : b);
}
tpl1 dfa T1 Min(T1 a, T1 b, T1 c)
{
    ret Min<T1>(Min<T1>(a, b), c);
}
tpl1 dfa T1 Max(T1 a, T1 b)
{
    ret ((a > b) ? a : b);
}
tpl1 dfa T1 Max(T1 a, T1 b, T1 c)
{
    ret Max<T1>(Max<T1>(a, b), c);
}
tpl1 dfa T1 Clamp(T1 val, T1 min, T1 max)
{
    ret Min<T1>(Max<T1>(val, min), max);
}
tpl1 dfa T1 Abs(T1 val)
{
    ret ((val < 0) ? -val : val);
}
tpl1 dfa T1 Diff(T1 a, T1 b)
{
    ret Abs<T1>(a - b);
}
tpl1 dfa T1 DiffWrap(T1 a, T1 b, T1 wrapAt)
{
    cx AU diff = Diff<T1>(a, b);
    ret Min<T1>(diff, wrapAt - diff);
}
tpl1 dfa T1 Sign(T1 val)
{
    ret ((val < 0) ? -1 : 1);
}
tpl1 dfa T1 Pow2(T1 val)
{
    ret val * val;
}
tpl1 dfa T1 Sqrt(T1 val)
{
#ifdef PROG_COMPILER_GCC
    ifu (val < 0)
        ret T1(-__builtin_sqrt(-val)); // Non-standard
    ret T1(__builtin_sqrt(val));
#else
    ifu (val < 0)
        ret T1(-sqrt(-val)); // Non-standard
    ret T1(sqrt(val));
#endif
}
tpl0 dfa F4 Sqrt<F4>(F4 val)
{
#ifdef PROG_COMPILER_GCC
    ifu (val < 0)
        ret -__builtin_sqrtf(-val); // Non-standard
    ret __builtin_sqrtf(val);
#else
    ifu (val < 0)
        ret -sqrtf(-val); // Non-standard
    ret sqrtf(val);
#endif
}
tpl1 dfa T1 Dist0(T1 x, T1 y)
{
    ret Sqrt<T1>(Pow2<T1>(x) + Pow2<T1>(y));
}
tpl1 dfa T1 Dist0Fast(T1 x, T1 y)
{
    // ~2.05% error rate in equal input distribution
    cx AU xa = Abs<T1>(x);
    cx AU ya = Abs<T1>(y);
    ret (xa > ya) ? (T1(0.947543) * xa + T1(0.392485) * ya) : (T1(0.947543) * ya + T1(0.392485) * xa);
}
tpl1 dfa T1 AlignBit(T1 val, T1 size)
{
    ret (val + (size - 1)) & ~(size - 1);
}
tpl1 dfa NT Swap(T1& a, T1& b)
{
    cx T1 tmp = a;
    a = b;
    b = tmp;
}
tpl2 dfa T1 Lerp(T1 a, T1 b, T2 t)
{
    ret a + T1(T2(b - a) * t);
}

dfa S4 RoundF4ToS4(F4 val)
{
    ret S4((val < 0.0f) ? (val - 0.5f) : (val + 0.5f));
}
dfa S8 RoundF8ToS8(F8 val)
{
    ret S8((val < 0.0) ? (val - 0.5) : (val + 0.5));
}

tpl1 dfa SI VarintEncode(U1* out, T1 in)
{
    cx U1* cx outBase = out;
    while (in > 0x7F)
    {
        *out++ = U1(in & 0x7F) | U1(0x80);
        in >>= 7;
    }
    *out++ = U1(in & 0x7F);
    ret SI(out - outBase);
}
tpl1 dfa SI VarintDecode(T1& out, cx U1* in)
{
    out = 0;
    cx U1* cx inBase = in;
    do
    {
        out |= ((*in) & 0x7F) << (0x07 * U1(in - inBase));
        if (!(*in & 0x80))
        {
            ++in;
            break;
        }
        ++in;
    } while (YES);
    ret SI(in - inBase);
}

dfa U1 ByteObfuscate(U1 val, U1 i)
{
    ret U1((val ^ 0x55) + (i ^ 0xAA));
}
dfa U1 ByteUnobfuscate(U1 val, U1 i)
{
    ret U1(val - (i ^ 0xAA)) ^ 0x55;
}

tpl1 dfa T1 RotL(T1 val, SI cnt)
{
    // cnt &= sizb(T1) - 1; // disabled since 'cnt' is expected to be valid
    ret (val << cnt) | (val >> (sizb(T1) - cnt));
}
tpl1 dfa T1 RotR(T1 val, SI cnt)
{
    // cnt &= sizb(T1) - 1; // disabled since 'cnt' is expected to be valid
    ret (val >> cnt) | (val << (sizb(T1) - cnt));
}

tpl1 dfa T1 RevByte(T1 val)
{
    U1* cx b = (U1*)&val;
    ite (i, i < siz(T1) / 2)
        Swap<U1>(b[i], b[siz(T1) - 1 - i]);
    ret val;
}

tpl1 dfa SI LenInt(T1 val)
{
    SI sign = 0;
    if (val < 0)
    {
        val = -val;
        sign = 1;
    }
    SI num = 0;
    if (val < 10)
        num = 1;
    else if (val < 100)
        num = 2;
    else if (val < 1000)
        num = 3;
    else if (val < 10000)
        num = 4;
    else if (val < 100000)
        num = 5;
    else if (val < 1000000)
        num = 6;
    else if (val < 10000000)
        num = 7;
    else if (val < 100000000)
        num = 8;
    else if (val < 1000000000)
        num = 9;
    else if (val < 10000000000ULL)
        num = 10;
    else if (val < 100000000000ULL)
        num = 11;
    else if (val < 1000000000000ULL)
        num = 12;
    else if (val < 10000000000000ULL)
        num = 13;
    else if (val < 100000000000000ULL)
        num = 14;
    else if (val < 1000000000000000ULL)
        num = 15;
    else if (val < 10000000000000000ULL)
        num = 16;
    else if (val < 100000000000000000ULL)
        num = 17;
    else if (val < 1000000000000000000ULL)
        num = 18;
    else if (val < 10000000000000000000ULL)
        num = 19;
    else
        num = 20;
    ret num + sign;
}

tpl1 dfa T1 NormalizeMinmax(T1* arr, SI cnt)
{
    ifu (cnt < 1)
        ret T1(0);
    AU min = arr[0];
    AU max = arr[0];
    for (SI i = 1; i < cnt; ++i)
    {
        min = Min<T1>(min, arr[i]);
        max = Max<T1>(max, arr[i]);
    }
    cx AU range = max - min;
    if (max == min)
    {
        ite (i, i < cnt)
            arr[i] = T1(0);
    }
    else
    {
        ite (i, i < cnt)
            arr[i] = (arr[i] - min) / range;
    }
    ret range;
}
tpl1 dfa T1 NormalizeMax(T1* arr, SI cnt)
{
    ifu (cnt < 1)
        ret T1(0);
    AU max = arr[0];
    for (SI i = 1; i < cnt; ++i)
    {
        max = Max<T1>(max, arr[i]);
    }
    if (max == T1(0))
    {
        ite (i, i < cnt)
            arr[i] = T1(0);
    }
    else
    {
        ite (i, i < cnt)
            arr[i] /= max;
    }
    ret max;
}

tpl1 dfa T1 ValPrev(cx T1& val)
{
    if constexpr (std::is_integral<T1>::value)
    {
        ret val - 1;
    }
    else if constexpr (std::is_floating_point<T1>::value)
    {
        T1 epsilonScaled = std::numeric_limits<T1>::epsilon() * val;
        /*
        if (Abs<T1>(val) <= std::numeric_limits<T1>::min())
        {
            ret val - std::numeric_limits<T1>::min();
        }
        */
        ret val - epsilonScaled;
    }
    else
    {
        Assert(!"ValPrev: Unsupported type");
        ret val;
    }
}
