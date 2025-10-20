#pragma once

#include "type.hpp"

dfa NT SortRadixAscF4(F4* buf, SI cnt);

tpl1 dfa BO _CmpAsc(cx T1& a, cx T1& b);
tpl1 dfa BO _CmpDesc(cx T1& a, cx T1& b);

tpl<typename T1, BO Cmp(cx T1&, cx T1&)> dfa NT SortQuick(T1* low, T1* high);
tpl<typename T1, BO Cmp(cx T1&, cx T1&)> dfa NT SortQuick(T1* buf, SI cnt);

tpl1 dfa NT SortQuickAsc(T1* buf, SI cnt);
tpl1 dfa NT SortQuickDesc(T1* buf, SI cnt);

tpl1 dfa NT SortShellAsc(T1* buf, SI cnt);

#include "sort.ipp"
