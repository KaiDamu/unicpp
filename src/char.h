#pragma once

enum : CH
{
    CH_NUL = 0,
    CH_SOH = 1,
    CH_STX = 2,
    CH_ETX = 3,
    CH_EOT = 4,
    CH_ENQ = 5,
    CH_ACK = 6,
    CH_BEL = 7,
    CH_BS = 8,
    CH_HT = 9,
    CH_LF = 10,
    CH_VT = 11,
    CH_FF = 12,
    CH_CR = 13,
    CH_SO = 14,
    CH_SI = 15,
    CH_DLE = 16,
    CH_DC1 = 17,
    CH_DC2 = 18,
    CH_DC3 = 19,
    CH_DC4 = 20,
    CH_NAK = 21,
    CH_SYN = 22,
    CH_ETB = 23,
    CH_CAN = 24,
    CH_EM = 25,
    CH_SUB = 26,
    CH_ESC = 27,
    CH_FS = 28,
    CH_GS = 29,
    CH_RS = 30,
    CH_US = 31,
    CH_DEL = 127
};

cxex CH CH_NA = '?';

tpl1 dfa BO IsBetween(cx T1& c, cx T1& min, cx T1& max)
{
    ret c >= min && c <= max;
}
tpl1 dfa BO IsLowcase(cx T1& c)
{
    ret IsBetween<T1>(c, 'a', 'z');
}
tpl1 dfa BO IsUpcase(cx T1& c)
{
    ret IsBetween<T1>(c, 'A', 'Z');
}
tpl1 dfa BO IsNumBase2(cx T1& c)
{
    ret IsBetween<T1>(c, '0', '1');
}
tpl1 dfa BO IsNumBase8(cx T1& c)
{
    ret IsBetween<T1>(c, '0', '7');
}
tpl1 dfa BO IsNumBase10(cx T1& c)
{
    ret IsBetween<T1>(c, '0', '9');
}
tpl1 dfa BO IsNumBase16(cx T1& c)
{
    ret IsNumBase10<T1>(c) || IsBetween<T1>(c, 'A', 'F') || IsBetween<T1>(c, 'a', 'f');
}
tpl1 dfa BO IsSpace(cx T1& c)
{
    ret c == ' ';
}
tpl1 dfa BO IsTab(cx T1& c)
{
    ret c == CH_HT;
}
tpl1 dfa BO IsEol(cx T1& c)
{
    ret c == CH_CR || c == CH_LF;
}
tpl1 dfa BO IsCtrl(cx T1& c)
{
    ret IsBetween<T1>(c, CH_NUL, CH_US) || c == CH_DEL;
}
tpl1 dfa BO IsAscii(cx T1& c)
{
    ret IsBetween<T1>(c, CH_NUL, CH_DEL);
}
tpl1 dfa BO IsWspace(cx T1& c)
{
    ret IsSpace<T1>(c) || IsTab<T1>(c) || IsEol<T1>(c);
}

dfa CS CHToCS(CH val)
{
    if (val > CS_VAL_MAX)
        ret CS(CH_NA);
    ret CS(val);
}

tpl1 dfa T1 ToLowcase(cx T1& c)
{
    if (IsUpcase<T1>(c))
        ret c + 'a' - 'A';
    ret c;
}
tpl1 dfa T1 ToUpcase(cx T1& c)
{
    if (IsLowcase<T1>(c))
        ret c + 'A' - 'a';
    ret c;
}

tpl1 dfa BO IsSameCi(cx T1& c1, cx T1& c2)
{
    ret c1 == (IsLowcase<T1>(c1) ? ToLowcase<T1>(c2) : ToUpcase<T1>(c2));
}
