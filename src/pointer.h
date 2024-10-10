#pragma once

tpl1 class Ptr
{
  private:
    T1* m_val;

  public:
    dfa NT Del()
    {
        Assert(m_val != NUL);
        delete[] m_val;
        m_val = NUL;
    }
    dfa NT New(SI cnt)
    {
        Assert(m_val == NUL);
        m_val = new T1[cnt];
    }
    dfa T1* Get() const
    {
        ret m_val;
    }
    dfa NT Set(T1* ptr)
    {
        m_val = ptr;
    }
    dfa NT Resize(SI cntAlloc, SI cntCpy)
    {
        Assert(m_val != NUL);
        Assert(cntAlloc >= cntCpy);
        m_val = reinterpret_cast<T1*>(MemResize(m_val, cntAlloc * siz(T1), cntCpy * siz(T1)));
        new (m_val + cntCpy) T1[cntAlloc - cntCpy]();
    }

  public:
    dfa T1& operator[](SI i) const
    {
        ret m_val[i];
    }

  public:
    dfa Ptr()
    {
        m_val = NUL;
    }
    dfa Ptr(SI cnt)
    {
        m_val = NUL;
        tx->New(cnt);
    }
};
