#pragma once

#include "type.hpp"

dfa NT ProcCrash();
dfa NT ProcExit(U4 retVal);
dfa NT ProcExit();

#ifdef PROG_SYS_WIN

dfa HD ProcCurHdl();
dfa HD ProcCurId();

#endif

#include "unint.hpp"
#include "time.hpp"
#include "path.hpp"

#ifdef PROG_SYS_WIN

dfa LDR_DATA_TABLE_ENTRY_* _ProcMdlEntryGet(SI i);
dfa LDR_DATA_TABLE_ENTRY_* _ProcDllEntryGet(cx CH* dllName);
dfa LDR_DATA_TABLE_ENTRY_* _ProcDllEntryGet(HD dll);
dfa ER _ProcDllUnload(HD dll, BO doForce);

dfa HD ProcMdlCurAdrGet();

dfa HD ProcDllLoad(cx CH* dllName);
dfa HD ProcDllAdrGet(cx CH* dllName, BO doLoad = YES);
dfa ER ProcDllUnload(cx CH* dllName, BO doForce = NO);
dfa ER ProcDllUnload(HD dll, BO doForce = NO);
dfa GAFN ProcFnAdrGet(HD mdl, cx CS* fnName);

class MdlFnCache
{
  private:
    std::unordered_map<FNV1A64, GAFN, StdHasherNoOpe<FNV1A64>> m_fnCache;

  public:
    dfa ER CacheDll(cx CH* dllName);
    dfa GAFN FnAdrGet(FNV1A64 hash);
    dfa MdlFnCache();
};

dfa cx CH* ProcFilePath();
dfa cx CH* ProcDirPath();
dfa SI ProcWorkPathGet(CH* path);
dfa ER ProcWorkPathSet(cx CH* path);
dfa SI ProcWorkPathLen();

dfa BO ProcIsElevated();
dfa SI ProcFilePath(CH* path);
dfa SI ProcEnvvarGet(CH* val, cx CH* envvar, SI valLenMax);
dfa ER ProcEnvvarSet(cx CH* val, cx CH* envvar);
dfa ER ProcEnvvarClr(cx CH* envvar);
dfa cx CH* ProcArgFullGet();
dfa ER ProcDpiAwareSet();
dfa ER ProcPrioSet(U1 prio, BO isFocus = NO);

dfa ER ProcNewByPath(cx CH* path);

class _Proc
{
  private:
    HD m_hdl;

  public:
    dfa HD Hdl() cx;
    dfa ER Open(HD pid, U4 access);
    dfa ER Close();
    dfa _Proc();
    dfa ~_Proc();
};

dfa ER _ProcListGet(std::vector<SYSTEM_PROCESS_INFORMATION_>& out);
dfa ER _ProcIsUserLocalSystemByHdl(BO& isTrue, HD hdl);
dfa ER _ProcIsUserLocalSystemByPid(BO& isTrue, HD pid);

dfa ER ProcUsernameGet(HD proc, std::wstring* user, std::wstring* domain);

class Proc
{
  private:
    PROCESS_INFORMATION m_info;
    std::wstring m_args;

  public:
    dfa NT __Drop();

  public:
    dfa HANDLE Hdl() cx;
    dfa ER RetVal(U4& out) cx;
    dfa BO IsActive() cx;
    dfa ER Close();
    dfa ER Wait() cx;
    dfa ER Start(cx CH* path, cx CH* args, cx CH* workPath);
    dfa ER StartElevated(cx CH* path, cx CH* args, cx CH* workPath);
    dfa ER Stop() cx;

  public:
    dfa Proc();
    dfa ~Proc();
};

dfa ER ProcRestartElevated();

dfa TmCpu ProcTimeCpu(HD proc);

class TimerCpu
{
  private:
    TmCpu m_time;
    TmCpu m_timeStart;
    BO m_isRunning;

  private:
    dfa TmCpu TimeGet();

  public:
    dfa TmCpu Read();
    dfa TmCpu ReadK();
    dfa TmCpu ReadM();
    dfa NT Start();
    dfa TmCpu Pause();
    dfa TmCpu Restart();

  public:
    dfa TimerCpu();
    dfa ~TimerCpu();
};

cxex SI PROC_GLOBAL_STR_LEN_MAX = PATH_LEN_MAX;

class ProcGlobalStr
{
  private:
    HANDLE m_hdl;
    CH m_str[PROC_GLOBAL_STR_LEN_MAX + 1];

  private:
    dfa NT Init();

  public:
    dfa NT __Drop();

  public:
    dfa BO IsOpenCur() cx;
    dfa cx CH* Str() cx;
    dfa BO IsOpen(cx CH* str) cx;
    dfa ER Close();
    dfa ER Open(cx CH* str);
    dfa ER WaitIsClose(cx CH* str) cx;

  public:
    dfa ProcGlobalStr();
    dfa ProcGlobalStr(cx CH* str);
    dfa ~ProcGlobalStr();
};

dfa BO ProcGlobalTake(cx CH* str = NUL);

#endif

#include "process.ipp"
