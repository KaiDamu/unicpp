#pragma once

tpl<SI TI, typename T1> dfa cxex T1 ByteIVal(cx T1& val)
{
    static_assert(IsTypeU<T1> || IsTypeS<T1>, "ByteIVal: T1 must be an integer type");
    static_assert(TI >= 0 && TI < siz(T1), "ByteIVal: TI out of range");
    ret (val >> (TI << 3)) & 0xFF;
}
tpl1 dfa T1 Min(T1 a, T1 b)
{
    ret ((a < b) ? a : b);
}
tpl<typename T1, typename... Args> dfa T1 Min(T1 a, T1 b, Args... args)
{
    ret Min<T1>(Min<T1>(a, b), args...);
}
tpl1 dfa T1 Max(T1 a, T1 b)
{
    ret ((a > b) ? a : b);
}
tpl<typename T1, typename... Args> dfa T1 Max(T1 a, T1 b, Args... args)
{
    ret Max<T1>(Max<T1>(a, b), args...);
}
tpl1 dfa T1 Clamp(T1 val, T1 min, T1 max)
{
    ret Min<T1>(Max<T1>(val, min), max);
}
tpl1 dfa T1 Abs(T1 val)
{
    ret ((val < 0) ? -val : val);
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
tpl1 dfa T1 Diff(T1 a, T1 b)
{
    ifcx (IsTypeU<T1>)
        ret (a > b) ? a - b : b - a;
    else
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
tpl1 dfa T1 WholePart(T1 val)
{
    ifcx (IsTypeF<T1>)
        ret T1(S8(val));
    ret val;
}
tpl1 dfa T1 FractionPart(T1 val)
{
    ifcx (IsTypeF<T1>)
        ret val - WholePart<T1>(val);
    ret T1(0);
}
tpl1 dfa BO IsNearZero(T1 val)
{
    ifcx (IsTypeF<T1>)
        ret (Abs<T1>(val) < std::numeric_limits<T1>::epsilon());
    ret (val == T1(0));
}
tpl1 dfa T1 PowInt(T1 base, S2 exp)
{
    ifu (exp == 0)
        ret T1(1);

    cx AU isExpNeg = (exp < 0);
    if (isExpNeg)
        exp = -exp;

    AU r = T1(1);
    while (exp > 0)
    {
        if (exp & 1)
            r *= base;
        base *= base;
        exp >>= 1;
    }
    if (isExpNeg)
        r = T1(1) / r;

    ret r;
}
tpl1 dfa T1 Pow2(T1 val)
{
    ret val * val;
}
tpl1 dfa T1 Pow3(T1 val)
{
    ret val * val * val;
}
tpl1 dfa T1 Sqrt(T1 val)
{
#ifdef PROG_COMPILER_GCC
    ifu (val < T1(0))
        ret T1(-__builtin_sqrt(-val)); // non-standard
    ret T1(__builtin_sqrt(val));
#else
    ifu (val < T1(0))
        ret T1(-sqrt(-val)); // non-standard
    ret T1(sqrt(val));
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
tpl1 dfa T1 Sin(T1 val)
{
#ifdef PROG_COMPILER_GCC
    ret T1(__builtin_sin(val));
#else
    ret T1(sin(val));
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
tpl1 dfa T1 Cos(T1 val)
{
#ifdef PROG_COMPILER_GCC
    ret T1(__builtin_cos(val));
#else
    ret T1(cos(val));
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
tpl1 dfa T1 ZigzagEncode(T1 val)
{
    ret (val << 1) ^ (val >> (sizb(T1) - SI(1)));
}
tpl1 dfa T1 ZigzagDecode(T1 val)
{
    ret (val >> 1) ^ (-(val & 1));
}
tpl1 dfa T1 ZigzagAround0(T1 i)
{
    ret ZigzagDecode<T1>(i);
}
tpl1 dfa T1 ZigzagAround(T1 val, T1 i)
{
    ret val + ZigzagAround0<T1>(i);
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
tpl2 dfa T1 Lerp(cx T1& a, cx T1& b, T2 t)
{
    ret a + T1((b - a) * t);
}
tpl1 T1 DegToRad(T1 deg)
{
    ret deg * (Pi<T1>() / static_cast<T1>(180));
}
tpl1 T1 RadToDeg(T1 rad)
{
    ret rad * (static_cast<T1>(180) / Pi<T1>());
}
tpl1 dfa S8 FloorToInt(T1 val)
{
    ifcx (IsTypeF<T1>)
    {
        cx AU wholeS = S8(val);
        cx AU wholeF = T1(wholeS);
        ret (val < wholeF) ? (wholeS - S8(1)) : wholeS;
    }
    ret S8(val);
}
tpl1 dfa S8 CeilToInt(T1 val)
{
    ifcx (IsTypeF<T1>)
    {
        cx AU wholeS = S8(val);
        cx AU wholeF = T1(wholeS);
        ret (val > wholeF) ? (wholeS + S8(1)) : wholeS;
    }
    ret S8(val);
}
tpl1 dfa T1 Floor(T1 val)
{
    ifcx (IsTypeF<T1>)
    {
        cx AU wholeS = S8(val);
        cx AU wholeF = T1(wholeS);
        ret (val < wholeF) ? (wholeF - T1(1)) : wholeF;
    }
    ret T1(val);
}
tpl1 dfa T1 Ceil(T1 val)
{
    ifcx (IsTypeF<T1>)
    {
        cx AU wholeS = S8(val);
        cx AU wholeF = T1(wholeS);
        ret (val > wholeF) ? (wholeF + T1(1)) : wholeF;
    }
    ret T1(val);
}
tpl1 T1 DivCeil(T1 val, T1 div)
{
    ifcx (IsTypeF<T1>)
        ret Ceil(val / div);
    ret (val + div - 1) / div;
}
tpl1 dfa T1 CeilStep(T1 val, T1 step)
{
    ret DivCeil(val, step) * step;
}
dfa SI BitToByteSize(SI size)
{
    ret DivCeil(size, BIT_IN_BYTE);
}
tpl1 dfa S8 RoundToInt(T1 val) = delete;
tpl0 dfa S8 RoundToInt<>(F4 val)
{
    ret S8(_mm_cvtss_si32(_mm_set_ss(val)));
}
tpl0 dfa S8 RoundToInt<>(F8 val)
{
    ret S8(_mm_cvtsd_si64(_mm_set_sd(val)));
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
tpl1 dfa SI VarbaseintEncode(U1* out, T1 in, U1 base)
{
    cx U1* cx outBase = out;
    cx AU mask = U1(U1(0xFF) << base);
    while (in >= mask)
    {
        cx AU val = U1(mask | U1(in));
        *out++ = val;
        in -= val;
        in >>= base;
    }
    *out++ = U1(in);
    ret SI(out - outBase);
}
tpl1 dfa SI VarbaseintDecode(T1& out, cx U1* in, U1 base)
{
    out = 0;
    cx U1* cx inBase = in;
    cx AU mask = U1(U1(0xFF) << base);
    do
    {
        out += *in << (SI(in - inBase) * base);
        if (*in < mask)
            ret SI(in - inBase) + SI(1);
        ++in;
    } while (YES);
}
tpl1 dfa SI VarbaseintEncodeSize(T1 in, U1 base)
{
    SI size = 1;
    cx AU mask = U1(U1(0xFF) << base);
    while (in >= mask)
    {
        in = (in - U1(mask | U1(in))) >> base;
        ++size;
    }
    ret size;
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
tpl1 dfa SI LenBin(T1 val)
{
    ret LenBin<U8>(val);
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
tpl1 dfa T1 AppendInt(cx T1& high, cx T1& low)
{
    static_assert(IsTypeU<T1>, "AppendInt: T1 must be TypeU");
    AU high_ = high;
    AU low_ = low;
    do
        high_ *= 10;
    while (low_ /= 10);
    ret high_ + low;
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
    ifcx (std::is_integral<T1>::value)
    {
        ret val - 1;
    }
    else ifcx (std::is_floating_point<T1>::value)
    {
        ret val - (std::numeric_limits<T1>::epsilon() * Abs<T1>(val)); // 'val == 0' is not handled!
    }
    else
    {
        Assert(!"ValPrev: unsupported type");
        ret val;
    }
}
tpl1 dfa T1 ValNext(cx T1& val)
{
    ifcx (std::is_integral<T1>::value)
    {
        ret val + 1;
    }
    else ifcx (std::is_floating_point<T1>::value)
    {
        ret val + (std::numeric_limits<T1>::epsilon() * Abs<T1>(val)); // 'val == 0' is not handled!
    }
    else
    {
        Assert(!"ValNext: unsupported type");
        ret val;
    }
}
tpl1 dfa T1 AdamEase1(T1 t, T1 curveRaw)
{
    ret (t * (T1(1) + curveRaw)) / (t + curveRaw);
}
tpl1 dfa T1 AdamEase1CurveRawGet(T1 curve)
{
    // valid values for 'curve' are in the range of [-1.0, 1.0]
    // examples:
    // - curve = +0.3 for average ease-out
    // - curve = +0.5 for sharp ease-out
    // - curve = -0.3 for average ease-in
    // - curve = -0.5 for sharp ease-in
    // - curve =  0.0 for linear

    ifu (IsNearZero(curve))
        ret T1(1e9);

    cx AU isNeg = curve < T1(0);
    if (isNeg)
        curve = -curve;

    ifu (curve > T1(1))
        curve = T1(1);

    cx AU x = T1(0.5) - curve / T1(2);
    cx AU y = T1(0.5) + curve / T1(2);
    AU curveRaw = (x * (T1(1) - y)) / (y - x);

    if (isNeg)
        curveRaw = -(curveRaw + T1(1));

    ret curveRaw;
}
tpl1 dfa T1 AdamHill1(T1 t, T1 tMid, T1 fullness)
{
    // valid values for 't' are in the range of [0.0, 1.0]
    // valid values for 'tMid' are in the range of [0.0, 1.0]
    // valid values for 'fullness' are in the range of [-1.0, 1.0]

    cx AU tRatio = ((t < tMid) ? (t / tMid) : ((T1(1) - t) / (T1(1) - tMid)));
    cx AU valBase = tRatio + tRatio * (T1(1) - tRatio);
    cx AU val = valBase * (T1(1) + (T1(1) - valBase) * fullness);

    ret val;
}
