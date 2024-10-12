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
    dfa NT Add(cx span<T1>& dat)
    {
        m_dat.insert(m_dat.end(), dat.begin(), dat.end());
    }
    dfa T1& Get(T1& dat)
    {
        Assert(!tx->IsEmpty());
        dat = move(m_dat.front());
        m_dat.pop_front();
        ret dat;
    }
    dfa NT Clr()
    {
        m_dat.clear();
    }

    DQueue() = default;
    ~DQueue() = default;
};
