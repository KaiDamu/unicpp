#pragma once

#include <functional>
#include "unint.hpp"
#include "lock.hpp"
#include "event.hpp"
#include "queue.hpp"
#include "time.hpp"
#include "process.hpp"

#ifdef PROG_SYS_WIN
using ThdEntryFnT = S4(__stdcall*)(GA param);
#else
// might require a different definition, but this should be made uniform anyway...
using ThdEntryFnT = S4 (*)(GA param);
#endif

class ThdTask
{
  private:
    std::function<NT()> m_fn;

  public:
    dfa ThdTask();
    dfa ThdTask(cx std::function<NT()>& fn);
    dfa ThdTask(cx ThdTask& other);
    dfa ThdTask(ThdTask&& other) noex;
    dfa ~ThdTask() = default;

  public:
    dfa ThdTask& operator=(cx ThdTask& other);
    dfa ThdTask& operator=(ThdTask&& other) noex;
    dfa NT Set(cx std::function<NT()>& fn);
    dfa NT Call() cx;
};

dfa NT ThdYield();
tpl<SI TBound1 = 64, SI TBound2 = 4000> dfa NT ThdYield(SI spinCntCur);

dfa NT ThdWait(TmMain ms);

#ifdef PROG_SYS_WIN

dfa TEB_* ThdTeb();
dfa HD ThdCurHdl();
dfa HD ThdCurId();
dfa HD ThdIdByHdl(HD hdl);

dfa SI SysCpuCnt();

class Thd
{
  private:
    HD m_hdl;
    BO m_isOwned;
    HD m_tid;

  public:
    dfa HD Hdl() cx;
    dfa HD Id();
    dfa NT Disown();
    dfa BO IsActive() cx;
    dfa ER RetVal(S4& out) cx;
    dfa ER Wait() cx;
    dfa ER Open(HD tid, U4 access);
    dfa ER Close();
    dfa ER New(ThdEntryFnT entryAdr, GA entryParam = NUL, HD hdlProc = ProcCurHdl(), U4 access = THREAD_ALL_ACCESS, U4 flags = 0);
    dfa ER Del(S4 exitCode = -1) cx;

  public:
    dfa Thd();
    dfa ~Thd();
};

class ThdTaskMgr
{
  private:
    std::vector<Thd> m_thdList;
    DQueue<ThdTask> m_taskQueue;
    ThdLockMulti m_lock;
    EvtFast m_evtWait;
    SI m_taskCnt;

  public:
    dfa NT TaskAdd(cx ThdTask& task);
    dfa NT TaskAdd(cx std::span<ThdTask>& tasks);
    dfa SI TaskCnt() cx;
    dfa ER WaitAll();
    dfa ER Free();
    dfa ER Init(SI cnt);
    dfa ER Init();

  public:
    dfa ThdTaskMgr();
    dfa ThdTaskMgr(SI cnt);
    dfa ~ThdTaskMgr();

  public:
    friend dfa S4 ThdTaskMgrThdFn(GA pMgr);
};

dfa S4 ThdTaskMgrThdFn(GA pMgr);

#endif

#include "thread.ipp"
