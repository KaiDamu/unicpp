#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "macro-off.hpp"
    #include <windows.h>
    #include "macro-on.hpp"
#endif
#include "type.hpp"

class ThdLockFast
{
  private:
    volatile U4 m_isLocked;

  public:
    dfa NT Lock();
    dfa NT Unlock();

  public:
    dfa ThdLockFast();
    dfa ~ThdLockFast();
};

class ThdLockFastAu
{
  private:
    ThdLockFast& m_lock;

  public:
    dfa ThdLockFastAu(ThdLockFast& lock);
    dfa ~ThdLockFastAu();
};

#ifdef PROG_SYS_WIN

class ThdLock
{
  private:
    CRITICAL_SECTION m_hdl;

  public:
    dfa CRITICAL_SECTION& Hdl();
    dfa NT Lock();
    dfa NT Unlock();

  public:
    dfa ThdLock();
    dfa ~ThdLock();
};

class ThdCondVar
{
  private:
    CONDITION_VARIABLE m_hdl;

  public:
    dfa CONDITION_VARIABLE& Hdl();
    dfa NT Wait(CRITICAL_SECTION& critSect);
    dfa NT PassOne();
    dfa NT PassCnt(SI cnt);
    dfa NT PassAll();

  public:
    dfa ThdCondVar();
    dfa ~ThdCondVar();
};

class ThdLockMulti
{
  private:
    ThdLock m_lock;
    ThdCondVar m_condVar;

  public:
    dfa NT Lock();
    dfa NT Unlock();
    dfa NT Wait();
    dfa NT PassOne();
    dfa NT PassCnt(SI cnt);
    dfa NT PassAll();

  public:
    dfa ThdLockMulti();
    dfa ~ThdLockMulti();
};

#endif

#include "lock.ipp"
