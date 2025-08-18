#pragma once

tpl1 class DatIte
{
  private:
    T1* m_ptr;
    T1* m_cur;
    SI m_cap;

  private:
    dfa NT Init()
    {
        m_ptr = NUL;
        m_cur = NUL;
        m_cap = 0;
    }

  public:
    dfa T1* Ptr() cx
    {
        ret m_ptr;
    }
    dfa T1* Cur() cx
    {
        ret m_cur;
    }
    dfa SI Cap() cx
    {
        ret m_cap;
    }
    dfa SI Pos() cx
    {
        ret m_cur - m_ptr;
    }
    dfa NT Src(T1* ptr, SI cap)
    {
        m_ptr = ptr;
        m_cur = ptr;
        m_cap = cap;
    }
    dfa NT Src(T1* ptr)
    {
        tx->Src(ptr, SI_VAL_MAX);
    }
    dfa NT Read(T1& dat)
    {
        Assert(tx->Pos() < m_cap);
        dat = *m_cur;
        ++m_cur;
    }
    dfa NT Read(T1* dat, SI cnt)
    {
        Assert(tx->Pos() + cnt <= m_cap);
        MemCpy(dat, m_cur, cnt * siz(T1));
        m_cur += cnt;
    }
    dfa NT Write(cx T1& dat)
    {
        Assert(tx->Pos() < m_cap);
        *m_cur = dat;
        ++m_cur;
    }
    dfa NT Write(cx T1* dat, SI cnt)
    {
        Assert(tx->Pos() + cnt <= m_cap);
        MemCpyUpdCur(m_cur, dat, cnt * siz(T1));
    }
    dfa NT CurMove(SI cnt)
    {
        m_cur += cnt;
    }
    dfa NT CurSet(SI pos)
    {
        m_cur = tx->Ptr() + pos;
    }
    dfa NT CurClr()
    {
        tx->CurSet(0);
    }
    dfa NT Set(cx T1& dat)
    {
        tx->CurClr();
        tx->Write(dat);
    }
    dfa NT Set(cx T1* dat, SI cnt)
    {
        tx->CurClr();
        tx->Write(dat, cnt);
    }

  public:
    dfa SI ReadLine(T1* dat)
    {
        AU curBase = m_cur;
        AU curEnd = m_ptr + m_cap;
        ifu (m_cur == curEnd)
            ret -1;
        while ((m_cur != curEnd) && (IsEol<T1>(*m_cur) == NO))
            *dat++ = *m_cur++;
        cx SI len = m_cur - curBase;
        if (m_cur != curEnd)
        {
            ++m_cur;
            if ((m_cur != curEnd) && (*m_cur == CH_LF) && (*(m_cur - 1) == CH_CR))
                ++m_cur;
        }
        ret len;
    }
    dfa SI NextLineLen() cx
    {
        AU cur = m_cur;
        AU curBase = cur;
        AU curEnd = m_ptr + m_cap;
        ifu (cur == curEnd)
            ret -1;
        while ((cur != curEnd) && (IsEol<T1>(*cur) == NO))
            ++cur;
        ret cur - curBase;
    }
    dfa BO HasNextLine() cx
    {
        ret (m_cur != m_ptr + m_cap);
    }

  public:
    dfa T1& operator[](SI i) cx
    {
        Assert(i < m_cap);
        ret m_ptr[i];
    }

  public:
    dfa DatIte()
    {
        tx->Init();
    }
};

// dynamic array with read/write cursor support
tpl1 class Arr
{
  private:
    Ptr<T1> m_ptr;
    T1* m_cur;
    SI m_cap;

  private:
    dfa NT Init()
    {
        m_cur = NUL;
        m_cap = 0;
    }

  public:
    dfa NT __Drop()
    {
        m_ptr.Set(NUL);
    }
    dfa NT __Set(T1* ptr)
    {
        m_ptr.Set(ptr);
    }
    dfa NT __CapMax()
    {
        m_cap = SI_VAL_MAX;
    }

  public:
    dfa DatIte<T1> ToDatIte() cx
    {
        DatIte<T1> datIte;
        datIte.Src(m_ptr.Get(), m_cap);
        datIte.CurMove(m_cur - m_ptr.Get());
        ret datIte;
    }

  public:
    dfa NT Del()
    {
        ifu (m_ptr.Get() == NUL)
            ret;
        m_ptr.Del();
        m_cur = NUL;
        m_cap = 0;
    }
    dfa NT New(SI cnt)
    {
        ifu (m_ptr.Get() != NUL)
            tx->Del();
        m_ptr.New(cnt);
        m_cur = m_ptr.Get();
        m_cap = cnt;
    }
    dfa T1* Ptr() cx
    {
        ret m_ptr.Get();
    }
    dfa T1* Cur() cx
    {
        ret m_cur;
    }
    dfa SI Cap() cx
    {
        ret m_cap;
    }
    dfa SI Pos() cx
    {
        ret m_cur - m_ptr.Get();
    }
    dfa T1 Read()
    {
        Assert(tx->Pos() < m_cap);
        T1 dat = *m_cur;
        ++m_cur;
        ret dat;
    }
    dfa NT Read(T1& dat)
    {
        Assert(tx->Pos() < m_cap);
        dat = *m_cur;
        ++m_cur;
    }
    dfa NT Read(T1* dat, SI cnt)
    {
        Assert(tx->Pos() + cnt <= m_cap);
        MemCpy(dat, m_cur, cnt * siz(T1));
        m_cur += cnt;
    }
    dfa NT Write(cx T1& dat)
    {
        Assert(tx->Pos() < m_cap);
        *m_cur = dat;
        ++m_cur;
    }
    dfa NT Write(cx T1* dat, SI cnt)
    {
        Assert(tx->Pos() + cnt <= m_cap);
        MemCpyUpdCur(m_cur, dat, cnt * siz(T1));
    }
    dfa NT CurMove(SI cnt)
    {
        m_cur += cnt;
    }
    dfa NT CurSet(SI pos)
    {
        m_cur = tx->Ptr() + pos;
    }
    dfa NT CurClr()
    {
        tx->CurSet(0);
    }
    dfa NT Set(cx T1& dat)
    {
        tx->CurClr();
        tx->Write(dat);
    }
    dfa NT Set(cx T1* dat, SI cnt)
    {
        tx->CurClr();
        tx->Write(dat, cnt);
    }
    dfa NT Resize(SI cntAlloc, SI cntCpy)
    {
        cx AU pos = tx->Pos();
        ifu (m_ptr.Get() == NUL)
            m_ptr.New(cntAlloc);
        else
            m_ptr.Resize(cntAlloc, cntCpy);
        tx->CurSet(Min<SI>(pos, cntAlloc));
        m_cap = cntAlloc;
    }
    dfa NT Req(SI cntReq, SI cntAlloc, SI cntCpy)
    {
        ifu (m_cap < cntReq)
            tx->Resize(cntAlloc, cntCpy);
    }
    dfa NT Req(SI cntReq, SI cntAlloc)
    {
        tx->Req(cntReq, cntAlloc, tx->Pos());
    }
    dfa NT Req(SI cntReq)
    {
        tx->Req(cntReq, cntReq, tx->Pos());
    }

  public:
    dfa T1& operator[](SI i) cx
    {
        Assert(i < m_cap);
        ret m_ptr[i];
    }
    dfa operator DatIte<T1>() cx
    {
        ret tx->ToDatIte();
    }
    dfa operator T1*() cx
    {
        ret m_ptr.Get();
    }

  public:
    dfa Arr()
    {
        tx->Init();
    }
    dfa Arr(SI cnt)
    {
        tx->Init();
        tx->New(cnt);
    }
    dfa ~Arr()
    {
        tx->Del();
    }
};

// dynamic array with small buffer optimization (auto dealloc)
tpl<typename T1, SI sboLen> class ArrSbo
{
  private:
    T1 m_bufStack[sboLen];
    std::vector<T1> m_bufHeap;
    T1* m_buf;

  public:
    dfa SI Cnt() cx
    {
        if (m_buf == m_bufStack)
            ret sboLen;
        ret m_bufHeap.size();
    }
    dfa SI Size() cx
    {
        ret tx->Cnt() * siz(T1);
    }
    dfa NT Resize(SI cntAlloc, SI cntCpy)
    {
        if (m_buf == m_bufStack)
        {
            ifl (cntAlloc > sboLen)
            {
                m_bufHeap.resize(cntAlloc);
                m_buf = m_bufHeap.data();
                MemCpy(m_buf, m_bufStack, Min(cntCpy, sboLen) * siz(T1));
            }
        }
        else
        {
            ifu (cntAlloc <= sboLen)
            {
                MemCpy(m_bufStack, m_buf, Min(cntCpy, sboLen, m_bufHeap.size()) * siz(T1));
                m_buf = m_bufStack;
                m_bufHeap.clear();
                m_bufHeap.shrink_to_fit();
            }
            else
            {
                m_bufHeap.resize(cntAlloc);
                m_buf = m_bufHeap.data();
            }
        }
    }
    dfa NT Req(SI cntReq, SI cntAlloc, SI cntCpy)
    {
        ifu (tx->Cnt() < cntReq)
            tx->Resize(cntAlloc, cntCpy);
    }
    dfa NT Req(SI cntReq, SI cntAlloc)
    {
        tx->Req(cntReq, cntAlloc, tx->Cnt());
    }
    dfa NT Req(SI cntReq)
    {
        tx->Req(cntReq, cntReq, tx->Cnt());
    }
    dfa NT ReqUpdCur(SI cntReq, T1*& cur)
    {
        cx AU pos = cur - m_buf;
        tx->Req(cntReq, cntReq, tx->Cnt());
        cur = m_buf + pos;
    }
    dfa T1* Dat() cx
    {
        ret m_buf;
    }
    dfa T1& operator[](SI i) cx
    {
        ret m_buf[i];
    }

  public:
    dfa ArrSbo() : m_buf(m_bufStack)
    {
    }
    dfa ArrSbo(SI cnt) : m_buf(m_bufStack)
    {
        tx->Req(cnt, cnt, 0);
    }
};

// dynamic array, no copy on resize, no safety checks (auto dealloc)
tpl1 class ArrDFast
{
  private:
    T1* m_buf;
    SI m_cnt;

  public:
    dfa SI Cnt() cx
    {
        ret m_cnt;
    }
    dfa SI Size() cx
    {
        ret m_cnt * siz(T1);
    }
    dfa NT Del()
    {
        MemDel(m_buf);
        m_buf = NUL;
        m_cnt = 0;
    }
    dfa NT New(SI cnt)
    {
        m_buf = (T1*)MemNew((m_cnt = cnt) * siz(T1));
    }
    dfa NT ReNew(SI cnt)
    {
        MemDel(m_buf);
        m_buf = (T1*)MemNew((m_cnt = cnt) * siz(T1));
    }
    dfa T1* Dat() cx
    {
        ret m_buf;
    }
    dfa T1& operator[](SI i) cx
    {
        ret m_buf[i];
    }

  public:
    dfa ArrDFast() : m_buf(NUL), m_cnt(0)
    {
    }
    dfa ArrDFast(SI cnt)
    {
        m_buf = (T1*)MemNew((m_cnt = cnt) * siz(T1));
    }
    dfa ~ArrDFast()
    {
        if (m_buf != NUL)
            MemDel(m_buf);
    }
};

// static array after alloc, makes 1 cnt sub-alloc/dealloc calls (auto dealloc)
tpl1 class ArrSFree
{
  private:
    struct Elem
    {
        alignas(T1) U1 dat[siz(T1)];
        T1** usePtr;
    };
    static_assert(alignof(Elem) % alignof(T1) == 0, "ArrSFree bad Elem align");

  private:
    Elem* m_bufDat;
    T1** m_bufPtr;
    T1** m_free;
    T1** m_use;
    T1** m_freeEndCur;
    T1** m_useEndCur;
    SI m_cap;

  private:
    dfa NT _Init()
    {
        m_bufDat = NUL;
        m_bufPtr = NUL;
        m_free = NUL;
        m_use = NUL;
        m_freeEndCur = NUL;
        m_useEndCur = NUL;
        m_cap = 0;
    }

  public:
    dfa SI Cnt() cx
    {
        ret SI(m_useEndCur - m_use);
    }
    dfa SI Rem() cx
    {
        ret SI(m_freeEndCur - m_free);
    }
    dfa SI Cap() cx
    {
        ret m_cap;
    }
    dfa T1** Dat() cx
    {
        ret m_use;
    }
    dfa NT Del()
    {
        if (m_bufDat == NUL)
            ret;
        while (m_useEndCur != m_use)
            MemFreeAt(*--m_useEndCur);
        MemDelNoInit<Elem>(m_bufDat);
        MemDelNoInit<T1*>(m_bufPtr);
        tx->_Init();
    }
    dfa NT New(SI cap)
    {
        tx->Del();
        m_bufDat = MemNewNoInit<Elem>(cap);
        m_bufPtr = MemNewNoInit<T1*>(cap << 1);
        m_free = m_bufPtr;
        m_use = m_bufPtr + cap;
        m_freeEndCur = m_free + cap;
        m_useEndCur = m_use;
        m_cap = cap;
        ite (i, i < cap)
            m_free[i] = (T1*)(m_bufDat + i);
    }
    dfa NT Clr()
    {
        ifu (m_bufDat == NUL)
            ret;
        while (m_useEndCur != m_use)
        {
            MemFreeAt(*--m_useEndCur);
            *m_freeEndCur++ = *m_useEndCur;
        }
    }
    dfa NT ElemDel(T1* ptr)
    {
        *m_freeEndCur++ = ptr;
        MemFreeAt(ptr);
        AU delElem = (Elem*)ptr;
        --m_useEndCur;
        ((Elem*)*m_useEndCur)->usePtr = delElem->usePtr;
        *(delElem->usePtr) = *m_useEndCur;
    }
    tpl<typename... TArgs> dfa T1* ElemNew(TArgs&&... args)
    {
        ifu (m_freeEndCur == m_free) // out of free elements
            ret NUL;
        AU newObj = *(m_freeEndCur - 1);
        MemInitAt(newObj, std::forward<TArgs>(args)...);
        --m_freeEndCur;
        ((Elem*)newObj)->usePtr = m_useEndCur;
        *m_useEndCur++ = newObj;
        ret newObj;
    }
    dfa T1& operator[](SI i)
    {
        ret *(m_use[i]);
    }
    dfa cx T1& operator[](SI i) cx
    {
        ret *(m_use[i]);
    }

  public:
    dfa ArrSFree()
    {
        tx->_Init();
    }
    dfa ArrSFree(SI cap)
    {
        tx->_Init();
        tx->New(cap);
    }
    dfa ~ArrSFree()
    {
        tx->Del();
    }
};

// dynamic array, no copy on resize, no safety checks, no auto dealloc, base ptr shift possible
tpl1 class ArrDFastNS
{
  private:
    T1* m_bufBase;
    T1* m_buf;
    SI m_cnt;

  private:
    dfa NT _New(SI cnt)
    {
        m_buf = m_bufBase = (T1*)MemNew((m_cnt = cnt) * siz(T1));
    }

  public:
    dfa SI Cnt() cx
    {
        ret m_cnt;
    }
    dfa SI Size() cx
    {
        ret m_cnt * siz(T1);
    }
    dfa NT Shift(SI cnt)
    {
        m_buf += cnt;
        m_cnt -= cnt;
    }
    dfa NT ShiftClr()
    {
        tx->Shift(SI(m_bufBase - m_buf));
    }
    dfa NT Del()
    {
        MemDel(m_bufBase);
        m_buf = m_bufBase = NUL;
        m_cnt = 0;
    }
    dfa NT New(SI cnt)
    {
        tx->_New(cnt);
    }
    dfa NT ReNew(SI cnt)
    {
        MemDel(m_bufBase);
        tx->_New(cnt);
    }
    dfa NT ReqShiftClr(SI cnt)
    {
        if (cnt <= m_cnt)
            ret;
        if (m_bufBase != NUL)
            MemDel(m_bufBase);
        tx->_New(cnt);
    }
    dfa T1* Dat() cx
    {
        ret m_buf;
    }
    dfa T1& operator[](SI i) cx
    {
        ret m_buf[i];
    }

  public:
    dfa ArrDFastNS() : m_bufBase(NUL), m_buf(NUL), m_cnt(0)
    {
    }
    dfa ArrDFastNS(SI cnt)
    {
        tx->_New(cnt);
    }
    dfa ~ArrDFastNS()
    {
    }
};
