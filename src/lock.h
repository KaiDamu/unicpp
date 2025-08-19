#pragma once

class ThdLock
{
  private:
    CRITICAL_SECTION m_hdl;

  public:
    dfa CRITICAL_SECTION& Hdl()
    {
        ret m_hdl;
    }
    dfa NT Lock()
    {
#ifdef PROG_THD_CNT_MULTI
        EnterCriticalSection(&m_hdl);
#endif
    }
    dfa NT Unlock()
    {
#ifdef PROG_THD_CNT_MULTI
        LeaveCriticalSection(&m_hdl);
#endif
    }

  public:
    dfa ThdLock()
    {
        InitializeCriticalSection(&m_hdl);
    }
    dfa ~ThdLock()
    {
        DeleteCriticalSection(&m_hdl);
    }
};

class ThdLockFast
{
  private:
    volatile S4 m_isLocked;

  public:
    dfa NT Lock()
    {
#ifdef PROG_THD_CNT_MULTI
        while (_InterlockedCompareExchange(&m_isLocked, YES, NO) != NO)
            NtYieldExecution_();
#endif
    }
    dfa NT Unlock()
    {
#ifdef PROG_THD_CNT_MULTI
        m_isLocked = NO;
#endif
    }

  public:
    dfa ThdLockFast() : m_isLocked(NO)
    {
    }
    dfa ~ThdLockFast()
    {
        // FEATURE: the lock is not unlocked at destruction, which is intentional
    }
};

class ThdLockFastAu
{
  private:
    ThdLockFast& m_lock;

  public:
    dfa ThdLockFastAu(ThdLockFast& lock) : m_lock(lock)
    {
        m_lock.Lock();
    }
    dfa ~ThdLockFastAu()
    {
        m_lock.Unlock();
    }
};

class ThdCondVar
{
  private:
    CONDITION_VARIABLE m_hdl;

  public:
    dfa CONDITION_VARIABLE& Hdl()
    {
        ret m_hdl;
    }
    dfa NT Wait(CRITICAL_SECTION& critSect)
    {
        SleepConditionVariableCS(&m_hdl, &critSect, INFINITE);
    }
    dfa NT PassOne()
    {
        WakeConditionVariable(&m_hdl);
    }
    dfa NT PassCnt(SI cnt)
    {
        ite (i, i < cnt)
            WakeConditionVariable(&m_hdl);
    }
    dfa NT PassAll()
    {
        WakeAllConditionVariable(&m_hdl);
    }

  public:
    dfa ThdCondVar()
    {
        InitializeConditionVariable(&m_hdl);
    }
    dfa ~ThdCondVar()
    {
        ;
    }
};

class ThdLockMulti
{
  private:
    ThdLock m_lock;
    ThdCondVar m_condVar;

  public:
    dfa NT Lock()
    {
        m_lock.Lock();
    }
    dfa NT Unlock()
    {
        m_lock.Unlock();
    }
    dfa NT Wait()
    {
        m_condVar.Wait(m_lock.Hdl());
    }
    dfa NT PassOne()
    {
        m_condVar.PassOne();
    }
    dfa NT PassCnt(SI cnt)
    {
        m_condVar.PassCnt(cnt);
    }
    dfa NT PassAll()
    {
        m_condVar.PassAll();
    }

  public:
    dfa ThdLockMulti()
    {
        ;
    }
    dfa ~ThdLockMulti()
    {
        ;
    }
};
