#include "typeconvert.hpp"

#include "math.hpp"
#include "char.hpp"

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
        dst *= sign;
        falltru;
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
tpl2 dfa SI IntToStrBase16(T1* dst, T2 src, SI padToLen, T1 padVal)
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
