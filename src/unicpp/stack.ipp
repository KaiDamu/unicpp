#include "stack.hpp"

tpl1 dfa SI DStack<T1>::Len() cx
{
    ret m_arr.Pos();
}
tpl1 dfa BO DStack<T1>::IsEmpty() cx
{
    ret (tx->Len() == 0);
}
tpl1 dfa T1* DStack<T1>::Get() cx
{
    ifu (tx->IsEmpty())
        ret NUL;
    ret m_arr.Cur() - 1;
}
tpl1 dfa NT DStack<T1>::Trim()
{
    m_arr.Resize(tx->Len(), tx->Len());
}
tpl1 dfa NT DStack<T1>::Sub()
{
    Assert(tx->Len() >= 1);
    m_arr.CurMove(-1);
}
tpl1 dfa NT DStack<T1>::Sub(T1& dat)
{
    Assert(tx->Len() >= 1);
    m_arr.CurMove(-1);
    dat = *m_arr.Cur();
}
tpl1 dfa NT DStack<T1>::Sub(T1* dat, SI cnt)
{
    Assert(tx->Len() >= cnt);
    ite (i, i < cnt)
    {
        m_arr.CurMove(-1);
        dat = *m_arr.Cur();
    }
}
tpl1 dfa NT DStack<T1>::Add(cx T1& dat)
{
    ifu (tx->Len() + 1 > m_arr.Cap())
        m_arr.Resize(SI(F8(tx->Len() + 1) * DSTACK_ADD_ALLOC_SCALE), tx->Len());
    m_arr.Write(dat);
}
tpl1 dfa NT DStack<T1>::Add(cx T1* dat, SI cnt)
{
    ifu (tx->Len() + cnt > m_arr.Cap())
        m_arr.Resize(SI(F8(tx->Len() + 1) * DSTACK_ADD_ALLOC_SCALE), tx->Len());
    m_arr.Write(dat, cnt);
}
tpl1 dfa NT DStack<T1>::Set(cx T1& dat)
{
    m_arr.Req(1);
    m_arr.Set(dat);
}
tpl1 dfa NT DStack<T1>::Set(cx T1* dat, SI cnt)
{
    m_arr.Req(cnt);
    m_arr.Set(dat, cnt);
}
tpl1 dfa NT DStack<T1>::Clr()
{
    m_arr.Del();
}
