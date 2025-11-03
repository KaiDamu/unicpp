#include "thread.hpp"

dfa ThdTask::ThdTask() : m_fn([]() noex {})
{
}
dfa ThdTask::ThdTask(cx std::function<NT()>& fn) : m_fn(fn)
{
}
dfa ThdTask::ThdTask(cx ThdTask& other) : m_fn(other.m_fn)
{
}
dfa ThdTask::ThdTask(ThdTask&& other) noex
{
    m_fn = std::move(other.m_fn);
    ifu (!m_fn)
        m_fn = []() noex {};
}
dfa ThdTask& ThdTask::operator=(cx ThdTask& other)
{
    ifl (tx != &other)
        m_fn = other.m_fn;
    ret *tx;
}
dfa ThdTask& ThdTask::operator=(ThdTask&& other) noex
{
    ifl (tx != &other)
    {
        m_fn = std::move(other.m_fn);
        ifu (!m_fn)
            m_fn = []() noex {};
    }
    ret *tx;
}
dfa NT ThdTask::Set(cx std::function<NT()>& fn)
{
    m_fn = fn;
}
dfa NT ThdTask::Call() cx
{
    ifl (m_fn != NUL)
        m_fn();
}

dfa NT ThdYield()
{
#if defined(PROG_SYS_WIN)
    NtYieldExecution_();
#elif defined(PROG_SYS_ESP32)
    taskYIELD();
#else
    unimp;
#endif
}

dfa NT ThdWait(TmMain ms)
{
    ifu (ms <= TmMain(0))
        ret;
#if defined(PROG_SYS_WIN)
    LARGE_INTEGER_ tmp(S8(ms * TmMain(-10000)));
    NtDelayExecution_(NO, &tmp);
#elif defined(PROG_SYS_ESP32)
    vTaskDelay(CeilToInt(ms * (TmMain(configTICK_RATE_HZ) / TmMain(1000))));
#else
    unimp;
#endif
}

#ifdef PROG_SYS_WIN

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

dfa HD Thd::Hdl() cx
{
    ret m_hdl;
}
dfa HD Thd::Id()
{
    ifu (m_tid == HD(0) && m_hdl != NUL)
        m_tid = ThdIdByHdl(m_hdl);
    ret m_tid;
}
dfa NT Thd::Disown()
{
    m_isOwned = NO;
}
dfa BO Thd::IsActive() cx
{
    ifu (m_hdl == NUL)
        ret NO;
    LARGE_INTEGER_ timeout(0);
    cx AU status = NtWaitForSingleObject_(m_hdl, NO, &timeout);
    if (status == STATUS_TIMEOUT)
        ret YES;
    ret NO;
}
dfa ER Thd::RetVal(S4& out) cx
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
dfa ER Thd::Wait() cx
{
    ifu (m_hdl == NUL)
        rets;
    cx AU status = NtWaitForSingleObject_(m_hdl, NO, NUL);
    ifu (status != STATUS_SUCCESS)
        rete(ErrVal::THD);
    rets;
}
dfa ER Thd::Open(HD tid, U4 access)
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
dfa ER Thd::Close()
{
    if (m_hdl == NUL)
        rets;
    cx AU status = NtClose_(m_hdl);
    ifu (status != STATUS_SUCCESS)
        rete(ErrVal::THD);
    m_hdl = NUL;
    m_isOwned = NO;
    m_tid = HD(0);
    rets;
}
dfa ER Thd::New(ThdEntryFnT entryAdr, GA entryParam, HD hdlProc, U4 access, U4 flags)
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
dfa ER Thd::Del(S4 exitCode) cx
{
    if (m_hdl == NUL)
        rets;
    cx AU status = NtTerminateThread_(m_hdl, NTSTATUS(exitCode));
    ifu (status != STATUS_SUCCESS)
        rete(ErrVal::THD);
    ifep(tx->Wait());
    rets;
}
dfa Thd::Thd() : m_hdl(NUL), m_isOwned(NO), m_tid(HD(0))
{
}
dfa Thd::~Thd()
{
    if (m_isOwned)
        tx->Del();
    tx->Close();
}

dfa NT ThdTaskMgr::TaskAdd(cx ThdTask& task)
{
    m_lock.Lock();
    m_taskQueue.Add(task);
    ++m_taskCnt;
    m_lock.Unlock();
    m_lock.PassOne();
}
dfa NT ThdTaskMgr::TaskAdd(cx std::span<ThdTask>& tasks)
{
    m_lock.Lock();
    m_taskQueue.Add(tasks);
    m_taskCnt += tasks.size();
    m_lock.Unlock();
    m_lock.PassCnt(tasks.size());
}
dfa SI ThdTaskMgr::TaskCnt() cx
{
    ret m_taskCnt;
}
dfa ER ThdTaskMgr::WaitAll()
{
    while (tx->TaskCnt() != 0)
        m_evtWait.Wait(YES);
    rets;
}
dfa ER ThdTaskMgr::Free()
{
    ifep(tx->WaitAll());
    ite (i, i < SI(m_thdList.size()))
    {
        ifep(m_thdList[i].Del());
        ifep(m_thdList[i].Close());
    }
    m_thdList.clear();
    rets;
}
dfa ER ThdTaskMgr::Init(SI cnt)
{
    m_thdList.resize(cnt);
    ite (i, i < cnt)
    {
        ifep(m_thdList[i].New(ThdTaskMgrThdFn, tx));
    }
    rets;
}
dfa ER ThdTaskMgr::Init()
{
    ret tx->Init(SysCpuCnt());
}
dfa ThdTaskMgr::ThdTaskMgr()
{
    m_taskCnt = 0;
}
dfa ThdTaskMgr::ThdTaskMgr(SI cnt)
{
    m_taskCnt = 0;
    if (cnt == 0)
        tx->Init();
    else
        tx->Init(cnt);
}
dfa ThdTaskMgr::~ThdTaskMgr()
{
    tx->Free();
}

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
