#include "char.hpp"

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
