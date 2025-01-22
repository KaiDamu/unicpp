#pragma once

class ThdTask
{
  private:
    std::function<NT()> m_fn;

  public:
    dfa ThdTask() : m_fn([]() noex {})
    {
    }
    dfa ThdTask(cx std::function<NT()>& fn) : m_fn(fn)
    {
    }
    dfa ThdTask(cx ThdTask& other) : m_fn(other.m_fn)
    {
    }
    dfa ThdTask(ThdTask&& other) noex
    {
        m_fn = std::move(other.m_fn);
        ifu (!m_fn)
            m_fn = []() noex {};
    }
    dfa ~ThdTask() = default;

  public:
    dfa ThdTask& operator=(cx ThdTask& other)
    {
        ifl (tx != &other)
            m_fn = other.m_fn;
        ret *tx;
    }
    dfa ThdTask& operator=(ThdTask&& other) noex
    {
        ifl (tx != &other)
        {
            m_fn = std::move(other.m_fn);
            ifu (!m_fn)
                m_fn = []() noex {};
        }
        ret *tx;
    }
    dfa NT Set(cx std::function<NT()>& fn)
    {
        m_fn = fn;
    }
    dfa NT Call() cx
    {
        ifl (m_fn)
            m_fn();
    }
};

#ifdef PROG_SYS_WIN

dfa NT ThdWait(TmMain ms)
{
    ifu (ms <= TmMain(0))
        ret;
    cx AU t = S8(ms * TmMain(-10000));
    LARGE_INTEGER tmp = {};
    tmp.QuadPart = t;
    NtDelayExecution(FALSE, &tmp);
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
    dfa HANDLE Hdl() cx
    {
        ret m_hdl;
    }
    dfa U4 Id() cx
    {
        ret m_id;
    }
    dfa ER RetVal(U4& out) cx
    {
        out = -1;
        ifu (m_hdl == NUL)
            rete(ErrVal::NO_INIT);
        DWORD r;
        cx BOOL result = GetExitCodeThread(m_hdl, &r);
        ifu (result == 0)
            rete(ErrVal::THD);
        ifu (r == STILL_ACTIVE)
            rete(ErrVal::YES_ACTIVE);
        out = r;
        rets;
    }
    dfa BO IsActive() cx
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
            rete(ErrVal::YES_ACTIVE);
        ifu (CloseHandle(m_hdl) == 0)
            rete(ErrVal::THD);
        m_hdl = NUL;
        m_id = 0;
        rets;
    }
    dfa ER Wait() cx
    {
        if (tx->IsActive() == NO)
            rets;
        ifu (WaitForSingleObject(m_hdl, INFINITE) != WAIT_OBJECT_0)
            rete(ErrVal::THD);
        rets;
    }
    dfa ER Start(LPTHREAD_START_ROUTINE fn, LPVOID param)
    {
        ifcx (!IS_THD_SUPPORT)
            rete(ErrVal::NO_SUPPORT);
        ifu (tx->IsActive())
            rete(ErrVal::YES_ACTIVE);
        ife (tx->Close())
            retep;
        m_id = 0;
        m_hdl = CreateThread(NUL, 0, fn, param, 0, (DWORD*)&m_id);
        ifu (m_hdl == NUL)
            rete(ErrVal::THD);
        rets;
    }
    dfa ER Stop() cx
    {
        if (tx->IsActive() == NO)
            rets;
        ifu (TerminateThread(m_hdl, U4(-1)) == 0)
            rete(ErrVal::THD);
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
    dfa NT TaskAdd(cx std::span<ThdTask>& tasks)
    {
        m_lock.Lock();
        m_taskQueue.Add(tasks);
        m_taskCnt += tasks.size();
        m_lock.Unlock();
        m_lock.PassCnt(tasks.size());
    }
    dfa SI TaskCnt() cx
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
            mgr.m_lock.Wait();
        cx AU task = mgr.m_taskQueue.Peek();
        mgr.m_taskQueue.Del();
        mgr.m_lock.Unlock();
        task.Call();
        mgr.m_lock.Lock();
        --mgr.m_taskCnt;
        mgr.m_lock.Unlock();
        mgr.m_evtWait.Set(YES);
    }
    ret 0;
}

#endif
