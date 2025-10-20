#pragma once

#include <array>
#include "value.hpp"

tpl<SI size> dfa BO BigIsNeg(cx U1* val);
tpl<SI size> dfa SI BigHiBitI(cx U1* val);
tpl<SI size> dfa BO BigIsHi(cx U1* main, cx U1* cmp);
tpl<SI size> dfa NT BigCpy(U1* dst, cx U1* src);
tpl<SI size> dfa NT BigSel(U1* dst, cx U1* ifNo, cx U1* ifYes, BO cond);
tpl<SI size> dfa NT BigAdd(U1* dst, cx U1* src);
tpl<SI size> dfa U1 BigSub(U1* dst, cx U1* src);
tpl<SI size> dfa NT BigSubIfPos(U1* dst, cx U1* src);
tpl<SI size> dfa U1 BigShiftL(U1* dst, SI cnt);
tpl<SI size> dfa NT BigModAdd(U1* dst, cx U1* add, cx U1* mod);
tpl<SI size> dfa NT BigModMul(U1* dst, cx U1* mulA, cx U1* mulB, cx U1* mod);
tpl<SI size> dfa NT BigModByBuf(U1* dst, cx U1* buf, SI bufSize, cx U1* mod);

tpl<SI size> dfa NT BigSelCt(U1* dst, cx U1* ifNo, cx U1* ifYes, BO cond);

#include "bigmath.ipp"
