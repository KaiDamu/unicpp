#pragma once

dfa NT ThdWait(S4 ms, S4 us)
{
    cx S8 t = S8(ms) * 1000 + S8(us);
    ifu (t <= 0)
        ret;
    LARGE_INTEGER tmp;
    tmp.QuadPart = t * -10;
    NtDelayExecution(FALSE, &tmp);
}
dfa NT ThdWait(S4 ms)
{
    ThdWait(ms, 0);
}

dfa SI CpuThdCnt()
{
    static SI s_cache = -1;
    ifu (s_cache == -1)
    {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        s_cache = SI(info.dwNumberOfProcessors);
    }
    ret s_cache;
}

using ThdTaskFnType = SA (*)(GA, GA, GA);

class ThdTask
{
  private:
    ThdTaskFnType m_fn;
    GA m_param1;
    GA m_param2;
    GA m_param3;

  public:
    dfa NT Set(ThdTaskFnType fn, GA param1, GA param2, GA param3)
    {
        m_fn = fn;
        m_param1 = param1;
        m_param2 = param2;
        m_param3 = param3;
    }
    dfa SA Call() const
    {
        ret m_fn(m_param1, m_param2, m_param3);
    }

  public:
    dfa ThdTask()
    {
        tx->Set(NUL, NUL, NUL, NUL);
    }
    dfa ThdTask(ThdTaskFnType fn, GA param1, GA param2, GA param3)
    {
        tx->Set(fn, param1, param2, param3);
    }
    dfa ~ThdTask()
    {
        ;
    }
};

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
        EnterCriticalSection(&m_hdl);
    }
    dfa NT Unlock()
    {
        LeaveCriticalSection(&m_hdl);
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

class Thd
{
  private:
    HANDLE m_hdl;
    U4 m_id;

  public:
    dfa NT __Drop()
    {
        m_hdl = NUL;
        m_id = 0;
    }

  public:
    dfa HANDLE Hdl() const
    {
        ret m_hdl;
    }
    dfa U4 Id() const
    {
        ret m_id;
    }
    dfa ER RetVal(U4& out) const
    {
        out = -1;
        ifu (m_hdl == NUL)
            rete(ERR_NO_INIT);
        DWORD r;
        cx BOOL result = GetExitCodeThread(m_hdl, &r);
        ifu (result == 0)
            rete(ERR_THD);
        ifu (r == STILL_ACTIVE)
            rete(ERR_YES_ACTIVE);
        out = r;
        rets;
    }
    dfa BO IsActive() const
    {
        ifu (m_hdl == NUL)
            ret NO;
        DWORD r;
        cx BOOL result = GetExitCodeThread(m_hdl, &r);
        ifu (result == 0)
            ret NO;
        if (r == STILL_ACTIVE)
            ret YES;
        ret NO;
    }
    dfa ER Close()
    {
        if (m_hdl == NUL)
            rets;
        if (tx->IsActive())
            rete(ERR_YES_ACTIVE);
        ifu (CloseHandle(m_hdl) == 0)
            rete(ERR_THD);
        m_hdl = NUL;
        m_id = 0;
        rets;
    }
    dfa ER Wait() const
    {
        if (tx->IsActive() == NO)
            rets;
        ifu (WaitForSingleObject(m_hdl, INFINITE) != WAIT_OBJECT_0)
            rete(ERR_THD);
        rets;
    }
    dfa ER Start(LPTHREAD_START_ROUTINE fn, LPVOID param)
    {
        ifu (tx->IsActive())
            rete(ERR_YES_ACTIVE);
        ife (tx->Close())
            retep;
        m_id = 0;
        m_hdl = CreateThread(NUL, 0, fn, param, 0, (DWORD*)&m_id);
        ifu (m_hdl == NUL)
            rete(ERR_THD);
        rets;
    }
    dfa ER Stop() const
    {
        if (tx->IsActive() == NO)
            rets;
        ifu (TerminateThread(m_hdl, U4(-1)) == 0)
            rete(ERR_THD);
        ife (tx->Wait())
            retep;
        rets;
    }

  public:
    dfa Thd()
    {
        m_hdl = NUL;
        m_id = 0;
    }
    dfa ~Thd()
    {
        tx->Stop();
        tx->Close();
    }
};

dfa DWORD WINAPI ThdTaskMgrThdFn(LPVOID);

class ThdTaskMgr
{
  private:
    Arr<Thd> m_thdList;
    DQueue<ThdTask> m_taskQueue;
    ThdLockMulti m_lock;
    EvtWin m_evtWait;
    SI m_taskCnt;

  public:
    dfa NT TaskAdd(cx ThdTask& task)
    {
        m_lock.Lock();
        m_taskQueue.Add(task);
        ++m_taskCnt;
        m_lock.Unlock();
        m_lock.PassOne();
    }
    dfa SI TaskCnt() const
    {
        ret m_taskCnt;
    }
    dfa ER WaitAll()
    {
        while (tx->TaskCnt() != 0)
            ife (m_evtWait.Wait(NO))
                retep;
        rets;
    }
    dfa ER Free()
    {
        ife (tx->WaitAll())
            retep;
        ite (i, i < m_thdList.Cap())
        {
            ife (m_thdList[i].Stop())
                retep;
            ife (m_thdList[i].Close())
                retep;
        }
        m_thdList.Del();
        rets;
    }
    dfa ER Init(SI cnt)
    {
        m_thdList.New(cnt);
        ite (i, i < cnt)
        {
            ife (m_thdList[i].Start(ThdTaskMgrThdFn, tx))
                retep;
        }
        rets;
    }
    dfa ER Init()
    {
        ret tx->Init(CpuThdCnt());
    }

  public:
    dfa ThdTaskMgr()
    {
        m_taskCnt = 0;
    }
    dfa ThdTaskMgr(SI cnt)
    {
        m_taskCnt = 0;
        if (cnt == 0)
            tx->Init();
        else
            tx->Init(cnt);
    }
    dfa ~ThdTaskMgr()
    {
        tx->Free();
    }

  public:
    friend dfa DWORD WINAPI ThdTaskMgrThdFn(LPVOID);
};

dfa DWORD WINAPI ThdTaskMgrThdFn(LPVOID pMgr)
{
    ThdTaskMgr& mgr = *((ThdTaskMgr*)pMgr);
    while (YES)
    {
        mgr.m_lock.Lock();
        while (mgr.m_taskQueue.IsEmpty())
        {
            mgr.m_lock.Wait();
        }
        ThdTask task;
        mgr.m_taskQueue.Get(task);
        mgr.m_lock.Unlock();
        task.Call();
        mgr.m_lock.Lock();
        --mgr.m_taskCnt;
        mgr.m_lock.Unlock();
        mgr.m_evtWait.Set(YES);
    }
    ret 0;
}
