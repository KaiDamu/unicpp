#pragma once

#include "type.hpp"

tpl1 class DList
{
  private:
    struct DListElem
    {
        DListElem* prev;
        DListElem* next;
        T1 dat;
    };

  private:
    DListElem* m_first;
    DListElem* m_last;
    DListElem* m_cur;
    SI m_len;

  private:
    dfa T1* CurDat() cx;

  public:
    dfa BO IsEmpty() cx;
    dfa SI Len() cx;

  public:
    dfa BO CurIsFirst() cx;
    dfa BO CurIsLast() cx;

  public:
    dfa T1* GetCur() cx;
    dfa T1* GetFirst();
    dfa T1* GetLast();
    dfa T1* GetPrev();
    dfa T1* GetNext();

  public:
    dfa BO GetCur(T1& dat) cx;
    dfa BO GetFirst(T1& dat);
    dfa BO GetLast(T1& dat);
    dfa BO GetPrev(T1& dat);
    dfa BO GetNext(T1& dat);

  public:
    dfa BO SetCur(cx T1& dat);

  public:
    dfa T1* NewFirst();
    dfa T1* NewLast();
    dfa T1* NewPrev();
    dfa T1* NewNext();

  public:
    dfa NT NewFirst(cx T1& dat);
    dfa NT NewLast(cx T1& dat);
    dfa NT NewPrev(cx T1& dat);
    dfa NT NewNext(cx T1& dat);

  public:
    dfa NT DelCur();
    dfa NT DelFirst();
    dfa NT DelLast();
    dfa NT DelPrev();
    dfa NT DelNext();
    dfa NT DelAll();

  public:
    dfa NT Clr();

  public:
    dfa DList& operator=(cx DList& list);
    dfa DList(cx DList& list);

  public:
    dfa DList();
    dfa ~DList();
};

#include "list.ipp"
