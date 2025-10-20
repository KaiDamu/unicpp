#pragma once

#include "value.hpp"

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

dfa CS CHToCS(CH val);

tpl1 dfa BO IsBetween(cx T1& c, cx T1& min, cx T1& max);
tpl1 dfa BO IsLowcase(cx T1& c);
tpl1 dfa BO IsUpcase(cx T1& c);
tpl1 dfa BO IsNumBase2(cx T1& c);
tpl1 dfa BO IsNumBase8(cx T1& c);
tpl1 dfa BO IsNumBase10(cx T1& c);
tpl1 dfa BO IsNumBase16(cx T1& c);
tpl1 dfa BO IsSpace(cx T1& c);
tpl1 dfa BO IsTab(cx T1& c);
tpl1 dfa BO IsEol(cx T1& c);
tpl1 dfa BO IsCtrl(cx T1& c);
tpl1 dfa BO IsAscii(cx T1& c);
tpl1 dfa BO IsWspace(cx T1& c);

tpl1 dfa T1 ToLowcase(cx T1& c);
tpl1 dfa T1 ToUpcase(cx T1& c);

tpl1 dfa BO IsSameCi(cx T1& c1, cx T1& c2);

#include "char.ipp"
