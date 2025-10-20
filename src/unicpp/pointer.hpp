#pragma once

#include "value.hpp"
#include "debug.hpp"

tpl1 class Ptr
{
  private:
    T1* m_val;

  public:
    dfa NT Del();
    dfa NT New(SI cnt);
    dfa T1* Get() cx;
    dfa NT Set(T1* ptr);
    dfa NT Resize(SI cntAlloc, SI cntCpy);

  public:
    dfa T1& operator[](SI i) cx;

  public:
    dfa Ptr();
    dfa Ptr(SI cnt);
};

#include "pointer.ipp"
