#pragma once

using ThdEntryFnT = S4(__stdcall*)(GA param);

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
        ifl (m_fn != 0)
            m_fn();
    }
};

#ifdef PROG_SYS_WIN

// pre-defined:
dfa S4 ThdTaskMgrThdFn(GA pMgr);

dfa TEB_* ThdTeb()
{
    #ifdef PROG_ADR_SIZE_4
    ret(TEB_*)__readfsdword(0x18);
    #endif
    #ifdef PROG_ADR_SIZE_8
    ret(TEB_*)__readgsqword(0x30);
    #endif
    ret NUL;
}
dfa HD ThdCurHdl()
{
    ret HD(-2);
}
dfa HD ThdCurId()
{
    ret ThdTeb()->ClientId.UniqueThread;
}
dfa HD ThdIdByHdl(HD hdl)
{
    ifu (hdl == ThdCurHdl())
        ret ThdCurId();
    THREAD_BASIC_INFORMATION_ info;
    ifu (NtQueryInformationThread_(hdl, THREADINFOCLASS_::ThreadBasicInformation, &info, siz(info), NUL) != STATUS_SUCCESS)
        ret HD(0);
    ret info.ClientId.UniqueThread;
}

dfa NT ThdWait(TmMain ms)
{
    ifu (ms <= TmMain(0))
        ret;
    cx AU t = S8(ms * TmMain(-10000));
    LARGE_INTEGER_ tmp = {};
    tmp.QuadPart = t;
    NtDelayExecution_(NO, &tmp);
}

dfa SI SysCpuCnt()
{
    static SI s_cache = -1;
    ifu (s_cache == -1)
    {
        SYSTEM_BASIC_INFORMATION_ info;
        ifu (NtQuerySystemInformation_(SYSTEM_INFORMATION_CLASS_::SystemBasicInformation, &info, siz(info), NUL) != STATUS_SUCCESS)
            ret 0;
        s_cache = SI(info.NumberOfProcessors);
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
    volatile S4 isLocked;

  public:
    dfa NT Lock()
    {
    #ifdef PROG_THD_CNT_MULTI
        while (_InterlockedCompareExchange(&isLocked, YES, NO) != NO)
            NtYieldExecution_();
    #endif
    }
    dfa NT Unlock()
    {
    #ifdef PROG_THD_CNT_MULTI
        isLocked = NO;
    #endif
    }

  public:
    dfa ThdLockFast() : isLocked(NO)
    {
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
    HD m_hdl;
    BO m_isOwned;
    HD m_tid;

  public:
    dfa HD Hdl() cx
    {
        ret m_hdl;
    }
    dfa HD Id()
    {
        ifu (m_tid == 0 && m_hdl != NUL)
            m_tid = ThdIdByHdl(m_hdl);
        ret m_tid;
    }
    dfa NT Disown()
    {
        m_isOwned = NO;
    }
    dfa BO IsActive() cx
    {
        ifu (m_hdl == NUL)
            ret NO;
        LARGE_INTEGER_ timeout(0);
        cx AU status = NtWaitForSingleObject_(m_hdl, NO, &timeout);
        if (status == STATUS_TIMEOUT)
            ret YES;
        ret NO;
    }
    dfa ER RetVal(S4& out) cx
    {
        out = -1;
        ifu (m_hdl == NUL)
            rete(ErrVal::NO_INIT);
        THREAD_BASIC_INFORMATION_ info;
        cx AU status = NtQueryInformationThread_(m_hdl, THREADINFOCLASS_::ThreadBasicInformation, &info, siz(info), NUL);
        ifu (status != STATUS_SUCCESS)
            rete(ErrVal::THD);
        ifu (info.ExitStatus == STATUS_PENDING)
            rete(ErrVal::YES_ACTIVE);
        out = S4(info.ExitStatus);
        rets;
    }
    dfa ER Wait() cx
    {
        ifu (m_hdl == NUL)
            rets;
        cx AU status = NtWaitForSingleObject_(m_hdl, NO, NUL);
        ifu (status != STATUS_SUCCESS)
            rete(ErrVal::THD);
        rets;
    }
    dfa ER Open(HD tid, U4 access)
    {
        ifu (m_hdl != NUL)
            rete(ErrVal::YES_INIT);
        OBJECT_ATTRIBUTES_ oa;
        CLIENT_ID_ ci = {};
        ci.UniqueThread = tid;
        cx AU status = NtOpenThread_(&m_hdl, access, &oa, &ci);
        ifu (status != STATUS_SUCCESS)
        {
            m_hdl = NUL;
            rete(ErrVal::THD);
        }
        rets;
    }
    dfa ER Close()
    {
        if (m_hdl == NUL)
            rets;
        cx AU status = NtClose_(m_hdl);
        ifu (status != STATUS_SUCCESS)
            rete(ErrVal::THD);
        m_hdl = NUL;
        m_isOwned = NO;
        m_tid = 0;
        rets;
    }
    dfa ER New(ThdEntryFnT entryAdr, GA entryParam = NUL, HD hdlProc = ProcCurHdl(), U4 access = THREAD_ALL_ACCESS, U4 flags = 0)
    {
        ifcx (!IS_THD_SUPPORT)
            rete(ErrVal::NO_SUPPORT);
        ifu (m_hdl != NUL)
            rete(ErrVal::YES_INIT);
        CLIENT_ID_ cid = {};
        U1 buf[siz(PS_ATTRIBUTE_LIST_) + siz(PS_ATTRIBUTE_)];
        AU pal = (PS_ATTRIBUTE_LIST_*)buf;
        pal->TotalLength = siz(PS_ATTRIBUTE_LIST_) + siz(PS_ATTRIBUTE_);
        pal->Attributes[0].Attribute = PS_ATTRIBUTE_CLIENT_ID;
        pal->Attributes[0].Size = siz(cid);
        pal->Attributes[0].ValuePtr = &cid;
        pal->Attributes[0].ReturnLength = NUL;
        cx AU status = NtCreateThreadEx_(&m_hdl, access, NUL, hdlProc, (PUSER_THREAD_START_ROUTINE_)entryAdr, entryParam, flags, 0, 0, 0, pal);
        ifu (status != STATUS_SUCCESS)
        {
            m_hdl = NUL;
            rete(ErrVal::THD);
        }
        m_isOwned = YES;
        m_tid = cid.UniqueThread;
        rets;
    }
    dfa ER Del(S4 exitCode = -1) cx
    {
        if (m_hdl == NUL)
            rets;
        cx AU status = NtTerminateThread_(m_hdl, NTSTATUS(exitCode));
        ifu (status != STATUS_SUCCESS)
            rete(ErrVal::THD);
        ife (tx->Wait())
            retep;
        rets;
    }

  public:
    dfa Thd() : m_hdl(NUL), m_isOwned(NO), m_tid(0)
    {
    }
    dfa ~Thd()
    {
        if (m_isOwned)
            tx->Del();
        tx->Close();
    }
};

class ThdTaskMgr
{
  private:
    std::vector<Thd> m_thdList;
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
        ite (i, i < SI(m_thdList.size()))
        {
            ife (m_thdList[i].Del())
                retep;
            ife (m_thdList[i].Close())
                retep;
        }
        m_thdList.clear();
        rets;
    }
    dfa ER Init(SI cnt)
    {
        m_thdList.resize(cnt);
        ite (i, i < cnt)
        {
            ife (m_thdList[i].New(ThdTaskMgrThdFn, tx))
                retep;
        }
        rets;
    }
    dfa ER Init()
    {
        ret tx->Init(SysCpuCnt());
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
    friend dfa S4 ThdTaskMgrThdFn(GA pMgr);
};

dfa S4 ThdTaskMgrThdFn(GA pMgr)
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
