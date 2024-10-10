#pragma once

constexpr F8 DSTACK_ADD_ALLOC_SCALE = 1.5;

tpl1 class DStack
{
  private:
    Arr<T1> m_arr;

  public:
    dfa SI Len() const
    {
        ret m_arr.Pos();
    }
    dfa BO IsEmpty() const
    {
        ret (tx->Len() == 0);
    }
    dfa T1* Get() const
    {
        ifu (tx->IsEmpty())
            ret NUL;
        ret m_arr.Cur() - 1;
    }
    dfa NT Trim()
    {
        m_arr.Resize(tx->Len(), tx->Len());
    }
    dfa NT Sub()
    {
        Assert(tx->Len() >= 1);
        m_arr.CurMove(-1);
    }
    dfa NT Sub(T1& dat)
    {
        Assert(tx->Len() >= 1);
        m_arr.CurMove(-1);
        dat = *m_arr.Cur();
    }
    dfa NT Sub(T1* dat, SI cnt)
    {
        Assert(tx->Len() >= cnt);
        ite (i, i < cnt)
        {
            m_arr.CurMove(-1);
            dat = *m_arr.Cur();
        }
    }
    dfa NT Add(cx T1& dat)
    {
        ifu (tx->Len() + 1 > m_arr.Cap())
            m_arr.Resize(SI(F8(tx->Len() + 1) * DSTACK_ADD_ALLOC_SCALE), tx->Len());
        m_arr.Write(dat);
    }
    dfa NT Add(cx T1* dat, SI cnt)
    {
        ifu (tx->Len() + cnt > m_arr.Cap())
            m_arr.Resize(SI(F8(tx->Len() + 1) * DSTACK_ADD_ALLOC_SCALE), tx->Len());
        m_arr.Write(dat, cnt);
    }
    dfa NT Set(cx T1& dat)
    {
        m_arr.Req(1);
        m_arr.Set(dat);
    }
    dfa NT Set(cx T1* dat, SI cnt)
    {
        m_arr.Req(cnt);
        m_arr.Set(dat, cnt);
    }
    dfa NT Clr()
    {
        m_arr.Del();
    }
};
