#pragma once

#include "array.hpp"

cxex F8 DSTACK_ADD_ALLOC_SCALE = 1.5;

tpl1 class DStack
{
  private:
    Arr<T1> m_arr;

  public:
    dfa SI Len() cx;
    dfa BO IsEmpty() cx;
    dfa T1* Get() cx;
    dfa NT Trim();
    dfa NT Sub();
    dfa NT Sub(T1& dat);
    dfa NT Sub(T1* dat, SI cnt);
    dfa NT Add(cx T1& dat);
    dfa NT Add(cx T1* dat, SI cnt);
    dfa NT Set(cx T1& dat);
    dfa NT Set(cx T1* dat, SI cnt);
    dfa NT Clr();
};

#include "stack.ipp"
