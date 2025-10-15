#pragma once

dfa S4 F4ToS4Fast(F4 val)
{
    S4 r;
#ifdef PROG_COMPILER_GCC
    __asm__("fistpl %0" : "=m"(r) : "t"(val) : "st");
#else
    r = S4(val);
#endif
    ret r;
}

dfa F8 U8ToF8(U8 val)
{
    ret F8(val);
}
dfa F8 S8ToF8(S8 val)
{
    ret F8(val);
}

dfa F8 U4ToF8(U4 val)
{
    ret F8(val);
}
dfa F8 S4ToF8(S4 val)
{
    ret F8(val);
}
dfa F8 F4ToF8(F4 val)
{
    ret F8(val);
}

dfa U4 F8ToU4(F8 val)
{
    ret U4(val);
}
dfa S4 F8ToS4(F8 val)
{
    ret S4(val);
}
dfa F4 F8ToF4(F8 val)
{
    ret F4(val);
}

dfa F4 U4ToF4(U4 val)
{
    ret F4(val);
}
dfa F4 S4ToF4(S4 val)
{
    ret F4(val);
}

tpl2 dfa SI StrToInt(T1& dst, cx T2* src)
{
    dst = 0;
    AU p = src;

    ifcx (IsTypeU<T1>)
    {
        while (IsNumBase10<T2>(*p))
            dst = dst * T1(10) + T1((*p++) - '0');
    }
    else
    {
        AU sign = T1(1);
        if (*p == T2('-'))
        {
            ++p;
            sign = T1(-1);
        }
        while (IsNumBase10<T2>(*p))
            dst = dst * T1(10) + T1((*p++) - '0');
        dst *= sign;
    }

    ret SI(p - src);
}
tpl2 dfa SI StrToInt(T1& dst, cx T2* src, SI len)
{
    dst = 0;
    T1 sign = 1;
    if (*src == '-')
    {
        sign = -1;
        ++src;
        --len;
    }
    switch (len)
    {
    case 20:
        dst += ((*src++) - '0') * 10000000000000000000ULL;
        falltru;
    case 19:
        dst += ((*src++) - '0') * 1000000000000000000ULL;
        falltru;
    case 18:
        dst += ((*src++) - '0') * 100000000000000000ULL;
        falltru;
    case 17:
        dst += ((*src++) - '0') * 10000000000000000ULL;
        falltru;
    case 16:
        dst += ((*src++) - '0') * 1000000000000000ULL;
        falltru;
    case 15:
        dst += ((*src++) - '0') * 100000000000000ULL;
        falltru;
    case 14:
        dst += ((*src++) - '0') * 10000000000000ULL;
        falltru;
    case 13:
        dst += ((*src++) - '0') * 1000000000000ULL;
        falltru;
    case 12:
        dst += ((*src++) - '0') * 100000000000ULL;
        falltru;
    case 11:
        dst += ((*src++) - '0') * 10000000000ULL;
        falltru;
    case 10:
        dst += ((*src++) - '0') * 1000000000;
        falltru;
    case 9:
        dst += ((*src++) - '0') * 100000000;
        falltru;
    case 8:
        dst += ((*src++) - '0') * 10000000;
        falltru;
    case 7:
        dst += ((*src++) - '0') * 1000000;
        falltru;
    case 6:
        dst += ((*src++) - '0') * 100000;
        falltru;
    case 5:
        dst += ((*src++) - '0') * 10000;
        falltru;
    case 4:
        dst += ((*src++) - '0') * 1000;
        falltru;
    case 3:
        dst += ((*src++) - '0') * 100;
        falltru;
    case 2:
        dst += ((*src++) - '0') * 10;
        falltru;
    case 1:
        dst += (*src) - '0';
        falltru;
        dst *= sign;
    default:
        ret len;
    }
}
tpl2 dfa T1 StrToInt(cx T2* src)
{
    T1 dst;
    StrToInt<T1, T2>(dst, src);
    ret dst;
}
tpl2 dfa SI StrToFloat(T1& dst, cx T2* src)
{
    cx AU srcBase = src;
    dst = T1(0);

    AU sign = F8(1);
    if (*src == '-')
    {
        ++src;
        sign = F8(-1);
    }

    AU dstF = F8(0);
    src += StrToInt(dstF, src);

    if (*src == '.')
    {
        ++src;

        AU div = F8(1);
        while (IsNumBase10(*src))
        {
            dstF = dstF * F8(10) + F8((*src++) - '0');
            div *= 10.0;
        }
        dstF /= div;
    }

    if (*src == 'e')
    {
        ++src;

        AU exp = S4(0);
        src += StrToInt(exp, src);
        dstF *= PowInt(F8(10), exp);
    }

    dst = T1(dstF * sign);
    ret SI(src - srcBase);
}

tpl2 dfa SI IntToStr(T1* dst, T2 src)
{
    cx AU len = LenInt<T2>(src);
    dst[len] = '\0';
    ifcx (!IsTypeU<T2>)
        if (src < 0)
        {
            src = -src;
            dst[0] = '-';
        }
    dst += len;
    switch (len)
    {
    case 20:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 19:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 18:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 17:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 16:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 15:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 14:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 13:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 12:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 11:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 10:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 9:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 8:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 7:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 6:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 5:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 4:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 3:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    case 2:
        *(--dst) = T1(src % 10 + '0');
        src /= 10;
        falltru;
    default:
        *(--dst) = T1(src % 10 + '0');
    }
    ret len;
}
tpl2 dfa SI IntToStrBase16(T1* dst, T2 src, SI padToLen = siz(T2) * 2, T1 padVal = '0')
{
    static_assert(IsTypeU<T2>, "T2 (input int) must be an unsigned type");
    cx AU strLen = DivCeil(LenBin(src), SI(4));
    cx AU padLen = (padToLen > strLen) ? (padToLen - strLen) : SI(0);
    AU dstCur = dst;
    ite (i, i < padLen)
        *dstCur++ = padVal;
    cx AU curEnd = dstCur;
    dstCur += strLen;
    *dstCur = '\0';
    while (dstCur != curEnd)
    {
        cx AU digit = T1(src & 0xF);
        *(--dstCur) = T1(((digit >= 10) ? 7 : 0) + digit + '0');
        src >>= 4;
    }
    ret padLen + strLen;
}

tpl0 dfa NT ToType(F4& dst, cx CS* cx& src)
{
    StrToFloat(dst, src);
}
tpl0 dfa NT ToType(F4& dst, cx std::string& src)
{
    StrToFloat(dst, src.c_str());
}
tpl0 dfa NT ToType(BO& dst, cx std::string& src)
{
    dst = ((src.size() != 0) && (src[0] != '0' || src.size() > 1));
}
tpl0 dfa NT ToType(SI& dst, cx std::string& src)
{
    StrToInt(dst, src.c_str(), SI(src.size()));
}
tpl0 dfa NT ToType(std::string& dst, cx std::string& src)
{
    dst = src;
}
tpl0 dfa NT ToType(std::wstring& dst, cx std::string& src)
{
    dst = std::wstring(src.begin(), src.end());
}
