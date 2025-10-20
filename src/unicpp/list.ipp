#include "list.hpp"

#include "value.hpp"

tpl1 dfa T1* DList<T1>::CurDat() cx
{
    ret m_cur == NUL ? NUL : &m_cur->dat;
}
tpl1 dfa BO DList<T1>::IsEmpty() cx
{
    ret m_first == NUL;
}
tpl1 dfa SI DList<T1>::Len() cx
{
    ret m_len;
}
tpl1 dfa BO DList<T1>::CurIsFirst() cx
{
    ret m_cur == m_first;
}
tpl1 dfa BO DList<T1>::CurIsLast() cx
{
    ret m_cur == m_last;
}
tpl1 dfa T1* DList<T1>::GetCur() cx
{
    ret tx->CurDat();
}
tpl1 dfa T1* DList<T1>::GetFirst()
{
    m_cur = m_first;
    ret tx->CurDat();
}
tpl1 dfa T1* DList<T1>::GetLast()
{
    m_cur = m_last;
    ret tx->CurDat();
}
tpl1 dfa T1* DList<T1>::GetPrev()
{
    Assert(m_cur != NUL);
    m_cur = m_cur->prev;
    ret tx->CurDat();
}
tpl1 dfa T1* DList<T1>::GetNext()
{
    Assert(m_cur != NUL);
    m_cur = m_cur->next;
    ret tx->CurDat();
}
tpl1 dfa BO DList<T1>::GetCur(T1& dat) cx
{
    ifu (m_cur == NUL)
        ret NO;
    dat = m_cur->dat;
    ret YES;
}
tpl1 dfa BO DList<T1>::GetFirst(T1& dat)
{
    ifu (m_first == NUL)
        ret NO;
    m_cur = m_first;
    dat = m_cur->dat;
    ret YES;
}
tpl1 dfa BO DList<T1>::GetLast(T1& dat)
{
    ifu (m_last == NUL)
        ret NO;
    m_cur = m_last;
    dat = m_cur->dat;
    ret YES;
}
tpl1 dfa BO DList<T1>::GetPrev(T1& dat)
{
    Assert(m_cur != NUL);
    m_cur = m_cur->prev;
    ifu (m_cur == NUL)
        ret NO;
    dat = m_cur->dat;
    ret YES;
}
tpl1 dfa BO DList<T1>::GetNext(T1& dat)
{
    Assert(m_cur != NUL);
    m_cur = m_cur->next;
    ifu (m_cur == NUL)
        ret NO;
    dat = m_cur->dat;
    ret YES;
}
tpl1 dfa BO DList<T1>::SetCur(cx T1& dat)
{
    ifu (m_cur == NUL)
        ret NO;
    m_cur->dat = dat;
    ret YES;
}
tpl1 dfa T1* DList<T1>::NewFirst()
{
    DListElem* elem = new DListElem;
    elem->prev = NUL;
    elem->next = m_first;
    ifu (m_first == NUL)
        m_last = elem;
    else
        m_first->prev = elem;
    m_first = elem;
    ++m_len;
    ret &elem->dat;
}
tpl1 dfa T1* DList<T1>::NewLast()
{
    DListElem* elem = new DListElem;
    elem->prev = m_last;
    elem->next = NUL;
    ifu (m_last == NUL)
        m_first = elem;
    else
        m_last->next = elem;
    m_last = elem;
    ++m_len;
    ret &elem->dat;
}
tpl1 dfa T1* DList<T1>::NewPrev()
{
    Assert(m_cur != NUL);
    DListElem* elem = new DListElem;
    elem->prev = m_cur->prev;
    elem->next = m_cur;
    ifu (m_cur->prev == NUL)
        m_first = elem;
    else
        m_cur->prev->next = elem;
    m_cur->prev = elem;
    ++m_len;
    ret &elem->dat;
}
tpl1 dfa T1* DList<T1>::NewNext()
{
    Assert(m_cur != NUL);
    DListElem* elem = new DListElem;
    elem->prev = m_cur;
    elem->next = m_cur->next;
    ifu (m_cur->next == NUL)
        m_last = elem;
    else
        m_cur->next->prev = elem;
    m_cur->next = elem;
    ++m_len;
    ret &elem->dat;
}
tpl1 dfa NT DList<T1>::NewFirst(cx T1& dat)
{
    T1* cx datPtr = tx->NewFirst();
    *datPtr = dat;
}
tpl1 dfa NT DList<T1>::NewLast(cx T1& dat)
{
    T1* cx datPtr = tx->NewLast();
    *datPtr = dat;
}
tpl1 dfa NT DList<T1>::NewPrev(cx T1& dat)
{
    T1* cx datPtr = tx->NewPrev();
    *datPtr = dat;
}
tpl1 dfa NT DList<T1>::NewNext(cx T1& dat)
{
    T1* cx datPtr = tx->NewNext();
    *datPtr = dat;
}
tpl1 dfa NT DList<T1>::DelCur()
{
    Assert(m_cur != NUL);
    DListElem* elem = m_cur;
    ifu (m_cur->prev == NUL)
        m_first = m_cur->next;
    else
        m_cur->prev->next = m_cur->next;
    ifu (m_cur->next == NUL)
        m_last = m_cur->prev;
    else
        m_cur->next->prev = m_cur->prev;
    m_cur = m_cur->next;
    delete elem;
    --m_len;
}
tpl1 dfa NT DList<T1>::DelFirst()
{
    Assert(m_first != NUL);
    DListElem* elem = m_first;
    m_first = m_first->next;
    ifu (m_first == NUL)
        m_last = NUL;
    else
        m_first->prev = NUL;
    delete elem;
    --m_len;
}
tpl1 dfa NT DList<T1>::DelLast()
{
    Assert(m_last != NUL);
    DListElem* elem = m_last;
    m_last = m_last->prev;
    ifu (m_last == NUL)
        m_first = NUL;
    else
        m_last->next = NUL;
    delete elem;
    --m_len;
}
tpl1 dfa NT DList<T1>::DelPrev()
{
    Assert(m_cur != NUL);
    Assert(m_cur->prev != NUL);
    DListElem* elem = m_cur->prev;
    m_cur->prev = elem->prev;
    ifu (elem->prev == NUL)
        m_first = m_cur;
    else
        elem->prev->next = m_cur;
    delete elem;
    --m_len;
}
tpl1 dfa NT DList<T1>::DelNext()
{
    Assert(m_cur != NUL);
    Assert(m_cur->next != NUL);
    DListElem* elem = m_cur->next;
    m_cur->next = elem->next;
    ifu (elem->next == NUL)
        m_last = m_cur;
    else
        elem->next->prev = m_cur;
    delete elem;
    --m_len;
}
tpl1 dfa NT DList<T1>::DelAll()
{
    DListElem* elem = m_first;
    while (elem != NUL)
    {
        DListElem* next = elem->next;
        delete elem;
        elem = next;
    }
    m_first = NUL;
    m_last = NUL;
    m_cur = NUL;
    m_len = 0;
}
tpl1 dfa NT DList<T1>::Clr()
{
    tx->DelAll();
}
tpl1 dfa DList<T1>& DList<T1>::operator=(cx DList& list)
{
    tx->Clr();
    DListElem* elem = list.m_first;
    while (elem != NUL)
    {
        AU datPtr = tx->NewLast();
        *datPtr = elem->dat;
        elem = elem->next;
    }
    ret *tx;
}
tpl1 dfa DList<T1>::DList(cx DList& list) : m_first(NUL), m_last(NUL), m_cur(NUL), m_len(0)
{
    tx->Clr();
    DListElem* elem = list.m_first;
    while (elem != NUL)
    {
        AU datPtr = tx->NewLast();
        *datPtr = elem->dat;
        elem = elem->next;
    }
}
tpl1 dfa DList<T1>::DList() : m_first(NUL), m_last(NUL), m_cur(NUL), m_len(0)
{
}
tpl1 dfa DList<T1>::~DList()
{
    tx->Clr();
}
