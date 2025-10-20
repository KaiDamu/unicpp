#pragma once

#include <span>
#include <deque>
#include "type.hpp"

tpl1 class DQueue
{
  private:
    std::deque<T1> m_dat;

  public:
    dfa BO IsEmpty() cx;
    dfa SI Len() cx;
    dfa NT Add(cx T1& dat);
    dfa NT Add(cx std::span<T1>& dat);
    dfa NT AddFirst(cx T1& dat);
    dfa T1& Get(T1& dat);
    dfa T1& GetLast(T1& dat);
    dfa T1& Peek();
    dfa T1& PeekLast();
    dfa NT Del();
    dfa NT DelLast();
    dfa NT Clr();

    dfa T1& operator[](SI i);

    dfa DQueue() = default;
    dfa ~DQueue() = default;
};

#include "queue.ipp"
