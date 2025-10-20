#include "lock.hpp"

#include "atomic.hpp"
#include "thread.hpp"

dfa NT ThdLockFast::Lock()
{
#ifdef PROG_THD_CNT_MULTI
    while (!AtomCmpSetU4(m_isLocked, NO, YES))
        ThdYield();
#endif
}
dfa NT ThdLockFast::Unlock()
{
#ifdef PROG_THD_CNT_MULTI
    m_isLocked = NO;
#endif
}
dfa ThdLockFast::ThdLockFast() : m_isLocked(NO)
{
}
dfa ThdLockFast::~ThdLockFast()
{
    // FEATURE: the lock is not unlocked at destruction, which is intentional
}

dfa ThdLockFastAu::ThdLockFastAu(ThdLockFast& lock) : m_lock(lock)
{
    m_lock.Lock();
}
dfa ThdLockFastAu::~ThdLockFastAu()
{
    m_lock.Unlock();
}

#ifdef PROG_SYS_WIN

dfa CRITICAL_SECTION& ThdLock::Hdl()
{
    ret m_hdl;
}
dfa NT ThdLock::Lock()
{
    #ifdef PROG_THD_CNT_MULTI
    EnterCriticalSection(&m_hdl);
    #endif
}
dfa NT ThdLock::Unlock()
{
    #ifdef PROG_THD_CNT_MULTI
    LeaveCriticalSection(&m_hdl);
    #endif
}
dfa ThdLock::ThdLock()
{
    InitializeCriticalSection(&m_hdl);
}
dfa ThdLock::~ThdLock()
{
    DeleteCriticalSection(&m_hdl);
}

dfa CONDITION_VARIABLE& ThdCondVar::Hdl()
{
    ret m_hdl;
}
dfa NT ThdCondVar::Wait(CRITICAL_SECTION& critSect)
{
    SleepConditionVariableCS(&m_hdl, &critSect, INFINITE);
}
dfa NT ThdCondVar::PassOne()
{
    WakeConditionVariable(&m_hdl);
}
dfa NT ThdCondVar::PassCnt(SI cnt)
{
    ite (i, i < cnt)
        WakeConditionVariable(&m_hdl);
}
dfa NT ThdCondVar::PassAll()
{
    WakeAllConditionVariable(&m_hdl);
}
dfa ThdCondVar::ThdCondVar()
{
    InitializeConditionVariable(&m_hdl);
}
dfa ThdCondVar::~ThdCondVar()
{
    ;
}

dfa NT ThdLockMulti::Lock()
{
    m_lock.Lock();
}
dfa NT ThdLockMulti::Unlock()
{
    m_lock.Unlock();
}
dfa NT ThdLockMulti::Wait()
{
    m_condVar.Wait(m_lock.Hdl());
}
dfa NT ThdLockMulti::PassOne()
{
    m_condVar.PassOne();
}
dfa NT ThdLockMulti::PassCnt(SI cnt)
{
    m_condVar.PassCnt(cnt);
}
dfa NT ThdLockMulti::PassAll()
{
    m_condVar.PassAll();
}
dfa ThdLockMulti::ThdLockMulti()
{
    ;
}
dfa ThdLockMulti::~ThdLockMulti()
{
    ;
}

#endif
