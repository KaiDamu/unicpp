#pragma once

#include <vector>
#include "pointer.hpp"

tpl1 class DatIte
{
  private:
    T1* m_ptr;
    T1* m_cur;
    SI m_cap;

  private:
    dfa NT Init();

  public:
    dfa T1* Ptr() cx;
    dfa T1* Cur() cx;
    dfa SI Cap() cx;
    dfa SI Pos() cx;
    dfa NT Src(T1* ptr, SI cap);
    dfa NT Src(T1* ptr);
    dfa NT Read(T1& dat);
    dfa NT Read(T1* dat, SI cnt);
    dfa NT Write(cx T1& dat);
    dfa NT Write(cx T1* dat, SI cnt);
    dfa NT CurMove(SI cnt);
    dfa NT CurSet(SI pos);
    dfa NT CurClr();
    dfa NT Set(cx T1& dat);
    dfa NT Set(cx T1* dat, SI cnt);

  public:
    dfa SI ReadLine(T1* dat);
    dfa SI NextLineLen() cx;
    dfa BO HasNextLine() cx;

  public:
    dfa T1& operator[](SI i) cx;

  public:
    dfa DatIte();
};

// dynamic array with read/write cursor support
tpl1 class Arr
{
  private:
    ::Ptr<T1> m_ptr;
    T1* m_cur;
    SI m_cap;

  private:
    dfa NT Init();

  public:
    dfa NT __Drop();
    dfa NT __Set(T1* ptr);
    dfa NT __CapMax();

  public:
    dfa DatIte<T1> ToDatIte() cx;

  public:
    dfa NT Del();
    dfa NT New(SI cnt);
    dfa T1* Ptr() cx;
    dfa T1* Cur() cx;
    dfa SI Cap() cx;
    dfa SI Pos() cx;
    dfa T1 Read();
    dfa NT Read(T1& dat);
    dfa NT Read(T1* dat, SI cnt);
    dfa NT Write(cx T1& dat);
    dfa NT Write(cx T1* dat, SI cnt);
    dfa NT CurMove(SI cnt);
    dfa NT CurSet(SI pos);
    dfa NT CurClr();
    dfa NT Set(cx T1& dat);
    dfa NT Set(cx T1* dat, SI cnt);
    dfa NT Resize(SI cntAlloc, SI cntCpy);
    dfa NT Req(SI cntReq, SI cntAlloc, SI cntCpy);
    dfa NT Req(SI cntReq, SI cntAlloc);
    dfa NT Req(SI cntReq);

  public:
    dfa T1& operator[](SI i) cx;
    dfa operator DatIte<T1>() cx;
    dfa operator T1*() cx;

  public:
    dfa Arr();
    dfa Arr(SI cnt);
    dfa ~Arr();
};

// dynamic array with small buffer optimization (auto dealloc)
tpl<typename T1, SI sboLen> class ArrSbo
{
  private:
    T1 m_bufStack[sboLen];
    std::vector<T1> m_bufHeap;
    T1* m_buf;

  public:
    dfa SI Cnt() cx;
    dfa SI Size() cx;
    dfa NT Resize(SI cntAlloc, SI cntCpy);
    dfa NT Req(SI cntReq, SI cntAlloc, SI cntCpy);
    dfa NT Req(SI cntReq, SI cntAlloc);
    dfa NT Req(SI cntReq);
    dfa NT ReqUpdCur(SI cntReq, T1*& cur);
    dfa T1* Dat() cx;
    dfa T1& operator[](SI i) cx;

  public:
    dfa ArrSbo();
    dfa ArrSbo(SI cnt);
};

// dynamic array, no copy on resize, no safety checks (auto dealloc)
tpl1 class ArrDFast
{
  private:
    T1* m_buf;
    SI m_cnt;

  public:
    dfa SI Cnt() cx;
    dfa SI Size() cx;
    dfa NT Del();
    dfa NT New(SI cnt);
    dfa NT ReNew(SI cnt);
    dfa T1* Dat() cx;
    dfa T1& operator[](SI i) cx;

  public:
    dfa ArrDFast();
    dfa ArrDFast(SI cnt);
    dfa ~ArrDFast();
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
    dfa NT _Init();

  public:
    dfa SI Cnt() cx;
    dfa SI Rem() cx;
    dfa SI Cap() cx;
    dfa T1** Dat() cx;
    dfa NT Del();
    dfa NT New(SI cap);
    dfa NT Clr();
    dfa NT ElemDel(T1* ptr);
    tpl<typename... TArgs> dfa T1* ElemNew(TArgs&&... args);
    dfa T1& operator[](SI i);
    dfa cx T1& operator[](SI i) cx;

  public:
    dfa ArrSFree();
    dfa ArrSFree(SI cap);
    dfa ~ArrSFree();
};

// dynamic array, no copy on resize, no safety checks, no auto dealloc, base ptr shift possible
tpl1 class ArrDFastNS
{
  private:
    T1* m_bufBase;
    T1* m_buf;
    SI m_cnt;

  private:
    dfa NT _New(SI cnt);

  public:
    dfa SI Cnt() cx;
    dfa SI Size() cx;
    dfa NT Shift(SI cnt);
    dfa NT ShiftClr();
    dfa NT Del();
    dfa NT New(SI cnt);
    dfa NT ReNew(SI cnt);
    dfa NT ReqShiftClr(SI cnt);
    dfa T1* Dat() cx;
    dfa T1& operator[](SI i) cx;

  public:
    dfa ArrDFastNS();
    dfa ArrDFastNS(SI cnt);
    dfa ~ArrDFastNS();
};

#include "array.ipp"
