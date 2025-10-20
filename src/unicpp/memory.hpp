#pragma once

#include "preprocess.hpp"
#include <memory.h>
#ifdef PROG_SYS_WIN
    #include "macro-off.hpp"
    #include <windows.h>
    #include "macro-on.hpp"
#endif
#include "typeex.hpp"

#ifdef PROG_COMPILER_GCC
    #define RetAdr() GA(__builtin_return_address(0))
#else
    #ifdef PROG_COMPILER_MSVC
        #pragma intrinsic(_ReturnAddress)
        #define RetAdr() GA(_ReturnAddress())
    #else
        #define RetAdr() GA(0)
    #endif
#endif

noinl GA AdrOfNextOpe();

dfa NT NoOpe();

dfa NT MemDel(GA ptr);
dfa GA MemNew(SI size);

dfa NT MemSet(GA dst, U1 val, SI size);
dfa NT MemCpy(GA dst, CXGA src, SI size);
dfa NT MemMove(GA dst, CXGA src, SI size);
dfa SA MemCmp(CXGA ptr1, CXGA ptr2, SI size);

dfa GA MemResize(GA ptr, SI sizeAlloc, SI sizeCpy);

dfa NT MemSet0Force(GA dst, SI size);
dfa NT MemCpyNocall(GA dst, CXGA src, SI size);

#ifdef PROG_SYS_WIN

dfa ER MemCpySys(GA dst, CXGA src, SI size, HD procDst, HD procSrc);

dfa SI MemPageSize();

#endif

dfa NT MemObfuscate(GA dst, CXGA src, SI size);
dfa NT MemUnobfuscate(GA dst, CXGA src, SI size);
dfa NT MemObfuscate(GA dst, SI size);
dfa NT MemUnobfuscate(GA dst, SI size);

GA operator new(size_t size);
GA operator new[](size_t size);
/*
    GA operator new (size_t size, GA ptr);
    GA operator new[](size_t size, GA ptr);
*/
NT operator delete(GA ptr) noex;
NT operator delete[](GA ptr) noex;
NT operator delete(GA ptr, size_t size) noex;
NT operator delete[](GA ptr, size_t size) noex;

class MemPoolTmp
{
  private:
    U1** m_listPtr;
    SI m_listPtrLen;
    SI m_sizeFree;
    U1* m_curPtr;

  private:
    dfa NT Init();

  public:
    dfa GA New(SI size);
    dfa NT DelAll();

  public:
    dfa MemPoolTmp();
    dfa ~MemPoolTmp();
};

tpl1 dfa NT MemFreeAt(T1* obj) noex;
tpl<typename T1, typename... TArgs> dfa T1* MemInitAt(T1* obj, TArgs&&... args);

tpl1 dfa NT MemDelNoInit(GA ptr) noex;
tpl1 dfa T1* MemNewNoInit(SI cnt);

tpl1 dfa NT MemCpyValBe(T1& dst, CXGA src);
tpl1 dfa NT MemCpyValBe(GA dst, T1 src);
tpl1 dfa NT MemCpyUpdCur(T1*& dstCur, CXGA src, SI size);
tpl1 dfa NT MemCpyUpdCurSrc(GA dst, cx T1*& srcCur, SI size);

#include "process.hpp"

#ifdef PROG_SYS_WIN

dfa ER MemDelSys(GA ptr, HD proc = ProcCurHdl());
dfa ER MemNewSys(GA* ptr, SI size, HD proc = ProcCurHdl(), U4 prot = PAGE_READWRITE);

#endif

#include "memory.ipp"
