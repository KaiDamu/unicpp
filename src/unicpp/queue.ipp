#include "queue.hpp"

#include "debug.hpp"

tpl1 dfa BO DQueue<T1>::IsEmpty() cx
{
    ret m_dat.empty();
}
tpl1 dfa SI DQueue<T1>::Len() cx
{
    ret m_dat.size();
}
tpl1 dfa NT DQueue<T1>::Add(cx T1& dat)
{
    m_dat.emplace_back(dat);
}
tpl1 dfa NT DQueue<T1>::Add(cx std::span<T1>& dat)
{
    m_dat.insert(m_dat.end(), dat.begin(), dat.end());
}
tpl1 dfa NT DQueue<T1>::AddFirst(cx T1& dat)
{
    m_dat.emplace_front(dat);
}
tpl1 dfa T1& DQueue<T1>::Get(T1& dat)
{
    Assert(!tx->IsEmpty());
    dat = std::move(m_dat.front());
    m_dat.pop_front();
    ret dat;
}
tpl1 dfa T1& DQueue<T1>::GetLast(T1& dat)
{
    Assert(!tx->IsEmpty());
    dat = std::move(m_dat.back());
    m_dat.pop_back();
    ret dat;
}
tpl1 dfa T1& DQueue<T1>::Peek()
{
    Assert(!tx->IsEmpty());
    ret m_dat.front();
}
tpl1 dfa T1& DQueue<T1>::PeekLast()
{
    Assert(!tx->IsEmpty());
    ret m_dat.back();
}
tpl1 dfa NT DQueue<T1>::Del()
{
    m_dat.pop_front();
}
tpl1 dfa NT DQueue<T1>::DelLast()
{
    m_dat.pop_back();
}
tpl1 dfa NT DQueue<T1>::Clr()
{
    m_dat.clear();
}

tpl1 dfa T1& DQueue<T1>::operator[](SI i)
{
    ret m_dat[i];
}
