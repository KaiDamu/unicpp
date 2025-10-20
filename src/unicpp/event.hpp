#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "macro-off.hpp"
    #include <windows.h>
    #include "macro-on.hpp"
#endif
#include "value.hpp"

class EvtFast
{
  private:
    volatile U4 m_isSignaled;

  public:
    dfa NT Set(BO isSignaled);
    dfa NT Wait(BO doResetAfter = NO);

  public:
    dfa EvtFast();
};

#ifdef PROG_SYS_WIN

class _EvtWin
{
  private:
    HANDLE m_hdl;

  public:
    dfa ER Free();
    dfa ER Init();
    dfa ER Set(BO state);
    dfa ER Wait(BO stateAfter);

  public:
    dfa _EvtWin();
    dfa ~_EvtWin();
};

#endif

#include "event.ipp"
