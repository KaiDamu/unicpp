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
    T1 sign = 1;
    if (*src == '-')
    {
        sign = -1;
        ++src;
    }
    cx T2* p = src;
    while (IsNumBase10<T2>(*p))
        dst = dst * 10 + (*p++) - '0';
    dst *= sign;
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

tpl2 dfa SI IntToStr(T1* dst, T2 src)
{
    cx AU len = LenInt<T2>(src);
    dst[len] = '\0';
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
