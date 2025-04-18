#pragma once

#include <deque>

tpl1 class DQueue
{
  private:
    std::deque<T1> m_dat;

  public:
    dfa BO IsEmpty() cx
    {
        ret m_dat.empty();
    }
    dfa SI Len() cx
    {
        ret m_dat.size();
    }
    dfa NT Add(cx T1& dat)
    {
        m_dat.emplace_back(dat);
    }
    dfa NT Add(cx std::span<T1>& dat)
    {
        m_dat.insert(m_dat.end(), dat.begin(), dat.end());
    }
    dfa NT AddFirst(cx T1& dat)
    {
        m_dat.emplace_front(dat);
    }
    dfa T1& Get(T1& dat)
    {
        Assert(!tx->IsEmpty());
        dat = std::move(m_dat.front());
        m_dat.pop_front();
        ret dat;
    }
    dfa T1& GetLast(T1& dat)
    {
        Assert(!tx->IsEmpty());
        dat = std::move(m_dat.back());
        m_dat.pop_back();
        ret dat;
    }
    dfa T1& Peek()
    {
        Assert(!tx->IsEmpty());
        ret m_dat.front();
    }
    dfa T1& PeekLast()
    {
        Assert(!tx->IsEmpty());
        ret m_dat.back();
    }
    dfa NT Del()
    {
        m_dat.pop_front();
    }
    dfa NT DelLast()
    {
        m_dat.pop_back();
    }
    dfa NT Clr()
    {
        m_dat.clear();
    }

    dfa T1& operator[](SI i)
    {
        ret m_dat[i];
    }

    DQueue() = default;
    ~DQueue() = default;
};
