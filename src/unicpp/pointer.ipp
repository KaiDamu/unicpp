#include "pointer.hpp"

#include "memory.hpp"

tpl1 dfa NT Ptr<T1>::Del()
{
    Assert(m_val != NUL);
    delete[] m_val;
    m_val = NUL;
}
tpl1 dfa NT Ptr<T1>::New(SI cnt)
{
    Assert(m_val == NUL);
    m_val = new T1[cnt];
}
tpl1 dfa T1* Ptr<T1>::Get() cx
{
    ret m_val;
}
tpl1 dfa NT Ptr<T1>::Set(T1* ptr)
{
    m_val = ptr;
}
tpl1 dfa NT Ptr<T1>::Resize(SI cntAlloc, SI cntCpy)
{
    Assert(m_val != NUL);
    Assert(cntAlloc >= cntCpy);
    m_val = reinterpret_cast<T1*>(MemResize(m_val, cntAlloc * siz(T1), cntCpy * siz(T1)));
    new (m_val + cntCpy) T1[cntAlloc - cntCpy]();
}
tpl1 dfa T1& Ptr<T1>::operator[](SI i) cx
{
    ret m_val[i];
}
tpl1 dfa Ptr<T1>::Ptr()
{
    m_val = NUL;
}
tpl1 dfa Ptr<T1>::Ptr(SI cnt)
{
    m_val = NUL;
    tx->New(cnt);
}
