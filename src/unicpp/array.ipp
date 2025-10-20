#include "array.hpp"

#include "char.hpp"
#include "memory.hpp"

tpl1 dfa NT DatIte<T1>::Init()
{
    m_ptr = NUL;
    m_cur = NUL;
    m_cap = 0;
}
tpl1 dfa T1* DatIte<T1>::Ptr() cx
{
    ret m_ptr;
}
tpl1 dfa T1* DatIte<T1>::Cur() cx
{
    ret m_cur;
}
tpl1 dfa SI DatIte<T1>::Cap() cx
{
    ret m_cap;
}
tpl1 dfa SI DatIte<T1>::Pos() cx
{
    ret m_cur - m_ptr;
}
tpl1 dfa NT DatIte<T1>::Src(T1* ptr, SI cap)
{
    m_ptr = ptr;
    m_cur = ptr;
    m_cap = cap;
}
tpl1 dfa NT DatIte<T1>::Src(T1* ptr)
{
    tx->Src(ptr, SI_VAL_MAX);
}
tpl1 dfa NT DatIte<T1>::Read(T1& dat)
{
    Assert(tx->Pos() < m_cap);
    dat = *m_cur;
    ++m_cur;
}
tpl1 dfa NT DatIte<T1>::Read(T1* dat, SI cnt)
{
    Assert(tx->Pos() + cnt <= m_cap);
    MemCpy(dat, m_cur, cnt * siz(T1));
    m_cur += cnt;
}
tpl1 dfa NT DatIte<T1>::Write(cx T1& dat)
{
    Assert(tx->Pos() < m_cap);
    *m_cur = dat;
    ++m_cur;
}
tpl1 dfa NT DatIte<T1>::Write(cx T1* dat, SI cnt)
{
    Assert(tx->Pos() + cnt <= m_cap);
    MemCpyUpdCur(m_cur, dat, cnt * siz(T1));
}
tpl1 dfa NT DatIte<T1>::CurMove(SI cnt)
{
    m_cur += cnt;
}
tpl1 dfa NT DatIte<T1>::CurSet(SI pos)
{
    m_cur = tx->Ptr() + pos;
}
tpl1 dfa NT DatIte<T1>::CurClr()
{
    tx->CurSet(0);
}
tpl1 dfa NT DatIte<T1>::Set(cx T1& dat)
{
    tx->CurClr();
    tx->Write(dat);
}
tpl1 dfa NT DatIte<T1>::Set(cx T1* dat, SI cnt)
{
    tx->CurClr();
    tx->Write(dat, cnt);
}
tpl1 dfa SI DatIte<T1>::ReadLine(T1* dat)
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
tpl1 dfa SI DatIte<T1>::NextLineLen() cx
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
tpl1 dfa BO DatIte<T1>::HasNextLine() cx
{
    ret (m_cur != m_ptr + m_cap);
}
tpl1 dfa T1& DatIte<T1>::operator[](SI i) cx
{
    Assert(i < m_cap);
    ret m_ptr[i];
}
tpl1 dfa DatIte<T1>::DatIte()
{
    tx->Init();
}

tpl1 dfa NT Arr<T1>::Init()
{
    m_cur = NUL;
    m_cap = 0;
}
tpl1 dfa NT Arr<T1>::__Drop()
{
    m_ptr.Set(NUL);
}
tpl1 dfa NT Arr<T1>::__Set(T1* ptr)
{
    m_ptr.Set(ptr);
}
tpl1 dfa NT Arr<T1>::__CapMax()
{
    m_cap = SI_VAL_MAX;
}
tpl1 dfa DatIte<T1> Arr<T1>::ToDatIte() cx
{
    DatIte<T1> datIte;
    datIte.Src(m_ptr.Get(), m_cap);
    datIte.CurMove(m_cur - m_ptr.Get());
    ret datIte;
}
tpl1 dfa NT Arr<T1>::Del()
{
    ifu (m_ptr.Get() == NUL)
        ret;
    m_ptr.Del();
    m_cur = NUL;
    m_cap = 0;
}
tpl1 dfa NT Arr<T1>::New(SI cnt)
{
    ifu (m_ptr.Get() != NUL)
        tx->Del();
    m_ptr.New(cnt);
    m_cur = m_ptr.Get();
    m_cap = cnt;
}
tpl1 dfa T1* Arr<T1>::Ptr() cx
{
    ret m_ptr.Get();
}
tpl1 dfa T1* Arr<T1>::Cur() cx
{
    ret m_cur;
}
tpl1 dfa SI Arr<T1>::Cap() cx
{
    ret m_cap;
}
tpl1 dfa SI Arr<T1>::Pos() cx
{
    ret m_cur - m_ptr.Get();
}
tpl1 dfa T1 Arr<T1>::Read()
{
    Assert(tx->Pos() < m_cap);
    T1 dat = *m_cur;
    ++m_cur;
    ret dat;
}
tpl1 dfa NT Arr<T1>::Read(T1& dat)
{
    Assert(tx->Pos() < m_cap);
    dat = *m_cur;
    ++m_cur;
}
tpl1 dfa NT Arr<T1>::Read(T1* dat, SI cnt)
{
    Assert(tx->Pos() + cnt <= m_cap);
    MemCpy(dat, m_cur, cnt * siz(T1));
    m_cur += cnt;
}
tpl1 dfa NT Arr<T1>::Write(cx T1& dat)
{
    Assert(tx->Pos() < m_cap);
    *m_cur = dat;
    ++m_cur;
}
tpl1 dfa NT Arr<T1>::Write(cx T1* dat, SI cnt)
{
    Assert(tx->Pos() + cnt <= m_cap);
    MemCpyUpdCur(m_cur, dat, cnt * siz(T1));
}
tpl1 dfa NT Arr<T1>::CurMove(SI cnt)
{
    m_cur += cnt;
}
tpl1 dfa NT Arr<T1>::CurSet(SI pos)
{
    m_cur = tx->Ptr() + pos;
}
tpl1 dfa NT Arr<T1>::CurClr()
{
    tx->CurSet(0);
}
tpl1 dfa NT Arr<T1>::Set(cx T1& dat)
{
    tx->CurClr();
    tx->Write(dat);
}
tpl1 dfa NT Arr<T1>::Set(cx T1* dat, SI cnt)
{
    tx->CurClr();
    tx->Write(dat, cnt);
}
tpl1 dfa NT Arr<T1>::Resize(SI cntAlloc, SI cntCpy)
{
    cx AU pos = tx->Pos();
    ifu (m_ptr.Get() == NUL)
        m_ptr.New(cntAlloc);
    else
        m_ptr.Resize(cntAlloc, cntCpy);
    tx->CurSet(Min<SI>(pos, cntAlloc));
    m_cap = cntAlloc;
}
tpl1 dfa NT Arr<T1>::Req(SI cntReq, SI cntAlloc, SI cntCpy)
{
    ifu (m_cap < cntReq)
        tx->Resize(cntAlloc, cntCpy);
}
tpl1 dfa NT Arr<T1>::Req(SI cntReq, SI cntAlloc)
{
    tx->Req(cntReq, cntAlloc, tx->Pos());
}
tpl1 dfa NT Arr<T1>::Req(SI cntReq)
{
    tx->Req(cntReq, cntReq, tx->Pos());
}
tpl1 dfa T1& Arr<T1>::operator[](SI i) cx
{
    Assert(i < m_cap);
    ret m_ptr[i];
}
tpl1 dfa Arr<T1>::operator DatIte<T1>() cx
{
    ret tx->ToDatIte();
}
tpl1 dfa Arr<T1>::operator T1*() cx
{
    ret m_ptr.Get();
}
tpl1 dfa Arr<T1>::Arr()
{
    tx->Init();
}
tpl1 dfa Arr<T1>::Arr(SI cnt)
{
    tx->Init();
    tx->New(cnt);
}
tpl1 dfa Arr<T1>::~Arr()
{
    tx->Del();
}

tpl<typename T1, SI sboLen> dfa SI ArrSbo<T1, sboLen>::Cnt() cx
{
    if (m_buf == m_bufStack)
        ret sboLen;
    ret m_bufHeap.size();
}
tpl<typename T1, SI sboLen> dfa SI ArrSbo<T1, sboLen>::Size() cx
{
    ret tx->Cnt() * siz(T1);
}
tpl<typename T1, SI sboLen> dfa NT ArrSbo<T1, sboLen>::Resize(SI cntAlloc, SI cntCpy)
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
tpl<typename T1, SI sboLen> dfa NT ArrSbo<T1, sboLen>::Req(SI cntReq, SI cntAlloc, SI cntCpy)
{
    ifu (tx->Cnt() < cntReq)
        tx->Resize(cntAlloc, cntCpy);
}
tpl<typename T1, SI sboLen> dfa NT ArrSbo<T1, sboLen>::Req(SI cntReq, SI cntAlloc)
{
    tx->Req(cntReq, cntAlloc, tx->Cnt());
}
tpl<typename T1, SI sboLen> dfa NT ArrSbo<T1, sboLen>::Req(SI cntReq)
{
    tx->Req(cntReq, cntReq, tx->Cnt());
}
tpl<typename T1, SI sboLen> dfa NT ArrSbo<T1, sboLen>::ReqUpdCur(SI cntReq, T1*& cur)
{
    cx AU pos = cur - m_buf;
    tx->Req(cntReq, cntReq, tx->Cnt());
    cur = m_buf + pos;
}
tpl<typename T1, SI sboLen> dfa T1* ArrSbo<T1, sboLen>::Dat() cx
{
    ret m_buf;
}
tpl<typename T1, SI sboLen> dfa T1& ArrSbo<T1, sboLen>::operator[](SI i) cx
{
    ret m_buf[i];
}
tpl<typename T1, SI sboLen> dfa ArrSbo<T1, sboLen>::ArrSbo() : m_buf(m_bufStack)
{
}
tpl<typename T1, SI sboLen> dfa ArrSbo<T1, sboLen>::ArrSbo(SI cnt) : m_buf(m_bufStack)
{
    tx->Req(cnt, cnt, 0);
}

tpl1 dfa SI ArrDFast<T1>::Cnt() cx
{
    ret m_cnt;
}
tpl1 dfa SI ArrDFast<T1>::Size() cx
{
    ret m_cnt * siz(T1);
}
tpl1 dfa NT ArrDFast<T1>::Del()
{
    MemDel(m_buf);
    m_buf = NUL;
    m_cnt = 0;
}
tpl1 dfa NT ArrDFast<T1>::New(SI cnt)
{
    m_buf = (T1*)MemNew((m_cnt = cnt) * siz(T1));
}
tpl1 dfa NT ArrDFast<T1>::ReNew(SI cnt)
{
    MemDel(m_buf);
    m_buf = (T1*)MemNew((m_cnt = cnt) * siz(T1));
}
tpl1 dfa T1* ArrDFast<T1>::Dat() cx
{
    ret m_buf;
}
tpl1 dfa T1& ArrDFast<T1>::operator[](SI i) cx
{
    ret m_buf[i];
}
tpl1 dfa ArrDFast<T1>::ArrDFast() : m_buf(NUL), m_cnt(0)
{
}
tpl1 dfa ArrDFast<T1>::ArrDFast(SI cnt)
{
    m_buf = (T1*)MemNew((m_cnt = cnt) * siz(T1));
}
tpl1 dfa ArrDFast<T1>::~ArrDFast()
{
    if (m_buf != NUL)
        MemDel(m_buf);
}

tpl1 dfa NT ArrSFree<T1>::_Init()
{
    m_bufDat = NUL;
    m_bufPtr = NUL;
    m_free = NUL;
    m_use = NUL;
    m_freeEndCur = NUL;
    m_useEndCur = NUL;
    m_cap = 0;
}
tpl1 dfa SI ArrSFree<T1>::Cnt() cx
{
    ret SI(m_useEndCur - m_use);
}
tpl1 dfa SI ArrSFree<T1>::Rem() cx
{
    ret SI(m_freeEndCur - m_free);
}
tpl1 dfa SI ArrSFree<T1>::Cap() cx
{
    ret m_cap;
}
tpl1 dfa T1** ArrSFree<T1>::Dat() cx
{
    ret m_use;
}
tpl1 dfa NT ArrSFree<T1>::Del()
{
    if (m_bufDat == NUL)
        ret;
    while (m_useEndCur != m_use)
        MemFreeAt(*--m_useEndCur);
    MemDelNoInit<Elem>(m_bufDat);
    MemDelNoInit<T1*>(m_bufPtr);
    tx->_Init();
}
tpl1 dfa NT ArrSFree<T1>::New(SI cap)
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
tpl1 dfa NT ArrSFree<T1>::Clr()
{
    ifu (m_bufDat == NUL)
        ret;
    while (m_useEndCur != m_use)
    {
        MemFreeAt(*--m_useEndCur);
        *m_freeEndCur++ = *m_useEndCur;
    }
}
tpl1 dfa NT ArrSFree<T1>::ElemDel(T1* ptr)
{
    *m_freeEndCur++ = ptr;
    MemFreeAt(ptr);
    AU delElem = (Elem*)ptr;
    --m_useEndCur;
    ((Elem*)*m_useEndCur)->usePtr = delElem->usePtr;
    *(delElem->usePtr) = *m_useEndCur;
}
tpl1 tpl<typename... TArgs> dfa T1* ArrSFree<T1>::ElemNew(TArgs&&... args)
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
tpl1 dfa T1& ArrSFree<T1>::operator[](SI i)
{
    ret *(m_use[i]);
}
tpl1 dfa cx T1& ArrSFree<T1>::operator[](SI i) cx
{
    ret *(m_use[i]);
}
tpl1 dfa ArrSFree<T1>::ArrSFree()
{
    tx->_Init();
}
tpl1 dfa ArrSFree<T1>::ArrSFree(SI cap)
{
    tx->_Init();
    tx->New(cap);
}
tpl1 dfa ArrSFree<T1>::~ArrSFree()
{
    tx->Del();
}

tpl1 dfa NT ArrDFastNS<T1>::_New(SI cnt)
{
    m_buf = m_bufBase = (T1*)MemNew((m_cnt = cnt) * siz(T1));
}
tpl1 dfa SI ArrDFastNS<T1>::Cnt() cx
{
    ret m_cnt;
}
tpl1 dfa SI ArrDFastNS<T1>::Size() cx
{
    ret m_cnt * siz(T1);
}
tpl1 dfa NT ArrDFastNS<T1>::Shift(SI cnt)
{
    m_buf += cnt;
    m_cnt -= cnt;
}
tpl1 dfa NT ArrDFastNS<T1>::ShiftClr()
{
    tx->Shift(SI(m_bufBase - m_buf));
}
tpl1 dfa NT ArrDFastNS<T1>::Del()
{
    MemDel(m_bufBase);
    m_buf = m_bufBase = NUL;
    m_cnt = 0;
}
tpl1 dfa NT ArrDFastNS<T1>::New(SI cnt)
{
    tx->_New(cnt);
}
tpl1 dfa NT ArrDFastNS<T1>::ReNew(SI cnt)
{
    MemDel(m_bufBase);
    tx->_New(cnt);
}
tpl1 dfa NT ArrDFastNS<T1>::ReqShiftClr(SI cnt)
{
    if (cnt <= m_cnt)
        ret;
    if (m_bufBase != NUL)
        MemDel(m_bufBase);
    tx->_New(cnt);
}
tpl1 dfa T1* ArrDFastNS<T1>::Dat() cx
{
    ret m_buf;
}
tpl1 dfa T1& ArrDFastNS<T1>::operator[](SI i) cx
{
    ret m_buf[i];
}
tpl1 dfa ArrDFastNS<T1>::ArrDFastNS() : m_bufBase(NUL), m_buf(NUL), m_cnt(0)
{
}
tpl1 dfa ArrDFastNS<T1>::ArrDFastNS(SI cnt)
{
    tx->_New(cnt);
}
tpl1 dfa ArrDFastNS<T1>::~ArrDFastNS()
{
}
