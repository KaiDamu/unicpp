#pragma once

dfa NT ProcCrash()
{
    volatile U1 x = U1(clock());
    volatile U1 y = *reinterpret_cast<U1*>(UA(x ^ x));
    y = y / (x ^ x);
}
dfa NT ProcExit(U4 retVal)
{
#ifdef PROG_SYS_WIN
    RtlExitUserProcess(retVal);
#else
    exit(retVal);
#endif
    ProcCrash();
}
dfa NT ProcExit()
{
    ProcExit(U4(ErrLastGet()));
}

#ifdef PROG_SYS_WIN

dfa LDR_DATA_TABLE_ENTRY_* _ProcDllEntryGet(cx CH* dllName)
{
    cx AU teb = ThdTeb();
    ifu (teb == NUL)
        ret NUL;
    cx AU mdlNameSize = StrLen(dllName) * siz(dllName[0]);
    cx AU list = &teb->ProcessEnvironmentBlock->Ldr->InLoadOrderModuleList;
    for (AU cur = list->Flink->Flink; cur != list; cur = cur->Flink)
    {
        cx AU entry = (LDR_DATA_TABLE_ENTRY_*)cur;
        cx AU name = &entry->BaseDllName;
        if (name->Length != mdlNameSize)
            continue;
        cxex SI EXT_DLL_LEN = 4; // ".dll"
        cx AU cmpCnt = name->Length / siz(name->Buffer[0]) - EXT_DLL_LEN;
        ite (i, i < cmpCnt)
            if (!IsSameCi(name->Buffer[i], dllName[i]))
                jsrc(continue_2);
        ret entry;
        jdst(continue_2);
    }
    ret NUL;
}
dfa LDR_DATA_TABLE_ENTRY_* _ProcDllEntryGet(HD dll)
{
    cx AU teb = ThdTeb();
    ifu (teb == NUL)
        ret NUL;
    cx AU list = &teb->ProcessEnvironmentBlock->Ldr->InLoadOrderModuleList;
    for (AU cur = list->Flink->Flink; cur != list; cur = cur->Flink)
    {
        cx AU entry = (LDR_DATA_TABLE_ENTRY_*)cur;
        if (entry->DllBase == (GA)dll)
            ret entry;
    }
    ret NUL;
}
dfa ER _ProcDllUnload(HD dll, BO doForce)
{
    SI retryCnt = 0;
    jdst(didNotUnload);
    ifu (LdrUnloadDll_((GA)dll) != STATUS_SUCCESS)
        rete(ErrVal::PROC);
    if (doForce && (_ProcDllEntryGet(dll) != NUL))
    {
        ifu (retryCnt == 0xFFFFF)
            rete(ErrVal::HIGH_WAIT);
        ++retryCnt;
        jsrc(didNotUnload);
    }
    rets;
}

dfa HD ProcDllLoad(cx CH* dllName)
{
    cx UNICODE_STRING_ str(dllName);
    GA result;
    ifu (LdrLoadDll_(NUL, NUL, &str, &result) != STATUS_SUCCESS)
        ret NUL;
    ret(HD)result;
}
dfa HD ProcDllAdrGet(cx CH* dllName, BO doLoad = YES)
{
    cx AU entry = _ProcDllEntryGet(dllName);
    ifl (entry != NUL)
        ret entry->DllBase;
    if (!doLoad)
        ret NUL;
    ret ProcDllLoad(dllName);
}
dfa ER ProcDllUnload(cx CH* dllName, BO doForce = NO)
{
    cx AU entry = _ProcDllEntryGet(dllName);
    if (entry == NUL)
        rets;
    ret _ProcDllUnload((HD)entry->DllBase, doForce);
}
dfa ER ProcDllUnload(HD dll, BO doForce = NO)
{
    ret _ProcDllUnload(dll, doForce);
}
dfa GAFN ProcFnAdrGet(HD mdl, cx CS* fnName)
{
    cx AU baseAdr = (U1*)mdl;
    cx AU dosHdr = (IMAGE_DOS_HEADER_*)baseAdr;
    ifu (dosHdr == NUL)
        ret NUL;
    cx AU ntHdr = (IMAGE_NT_HEADERS_*)(baseAdr + dosHdr->e_lfanew);
    cx AU& exportDirInfo = ntHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    cx AU exportDir = (IMAGE_EXPORT_DIRECTORY_*)(baseAdr + exportDirInfo.VirtualAddress);
    cx AU fnList = (U4*)(baseAdr + exportDir->AddressOfFunctions);
    cx AU nameList = (U4*)(baseAdr + exportDir->AddressOfNames);
    cx AU ordinalList = (U2*)(baseAdr + exportDir->AddressOfNameOrdinals);
    ite (i, i < SI(exportDir->NumberOfNames))
    {
        cx AU name = (CS*)(baseAdr + nameList[i]);
        if (StrCmp(name, fnName) == 0)
            ret AsType<GAFN>(baseAdr + fnList[ordinalList[i]]);
    }
    ret NUL;
}

class MdlFnCache
{
  private:
    std::unordered_map<FNV1A64, GAFN> m_fnCache;

  public:
    dfa ER CacheDll(cx CH* dllName)
    {
        cx AU dll = ProcDllAdrGet(dllName, YES);
        ifu (dll == NUL)
            rete(ErrVal::PROC);
        CS dllNameBase[PATH_LENX_MAX];
        ChstrToCsstr(dllNameBase, dllName);
        cx AU dllNameBaseLen = StrToLowcase(dllNameBase) + 1;
        dllNameBase[dllNameBaseLen - 1] = '!'; // format: "name.dll!fnName"
        cx AU hashBase = HashFnv1a64(dllNameBase, dllNameBaseLen * siz(CS));
        cx AU baseAdr = (U1*)dll;
        cx AU dosHdr = (IMAGE_DOS_HEADER_*)baseAdr;
        cx AU ntHdr = (IMAGE_NT_HEADERS_*)(baseAdr + dosHdr->e_lfanew);
        cx AU& exportDirInfo = ntHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
        cx AU exportDir = (IMAGE_EXPORT_DIRECTORY_*)(baseAdr + exportDirInfo.VirtualAddress);
        cx AU fnList = (U4*)(baseAdr + exportDir->AddressOfFunctions);
        cx AU nameList = (U4*)(baseAdr + exportDir->AddressOfNames);
        cx AU ordinalList = (U2*)(baseAdr + exportDir->AddressOfNameOrdinals);
        ite (i, i < SI(exportDir->NumberOfNames))
        {
            cx AU name = (CS*)(baseAdr + nameList[i]);
            cx AU hash = HashFnv1a64Str(name, hashBase);
            cx AU fnAdr = AsType<GAFN>(baseAdr + fnList[ordinalList[i]]);
            m_fnCache[hash] = fnAdr;
        }
        rets;
    }
    dfa GAFN FnAdrGet(MdlFnHash hash)
    {
        cx AU it = m_fnCache.find(FNV1A64(hash));
        ifu (it == m_fnCache.end())
            ret NUL;
        ret it->second;
    }
    dfa MdlFnCache()
    {
    }
};

dfa HD ProcCurHdl()
{
    ret HD(-1);
}
dfa HD ProcCurId()
{
    ret ThdTeb()->ClientId.UniqueProcess;
}

dfa BO ProcIsElevated()
{
    SID_IDENTIFIER_AUTHORITY sia = {SECURITY_NT_AUTHORITY};
    PSID sid;
    ifu (AllocateAndInitializeSid(&sia, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &sid) == 0)
        ret NO;
    BOOL isElevated;
    ifu (CheckTokenMembership(NUL, sid, &isElevated) == 0)
        isElevated = NO;
    FreeSid(sid);
    ret BO(isElevated);
}
dfa SI ProcFilePath(CH* path)
{
    path[0] = '\0';
    cx SI r = GetModuleFileNameW(NUL, path, PATH_LEN_MAX);
    ifu (r == PATH_LEN_MAX)
        ret r - 1;
    ret r;
}
dfa SI ProcEnvvarGet(CH* val, cx CH* envvar, SI valLenMax)
{
    ifl (valLenMax > 0)
        val[0] = '\0';
    cx SI r = GetEnvironmentVariableW(envvar, val, DWORD(valLenMax));
    ifu (r >= valLenMax && valLenMax > 0)
        val[0] = '\0';
    ret r;
}
dfa ER ProcEnvvarSet(cx CH* val, cx CH* envvar)
{
    ifu (SetEnvironmentVariableW(envvar, val) == 0)
        rete(ErrVal::PROC);
    rets;
}
dfa ER ProcEnvvarClr(cx CH* envvar)
{
    ifu (SetEnvironmentVariableW(envvar, NUL) == 0)
        rete(ErrVal::PROC);
    rets;
}
dfa cx CH* ProcArgFullGet()
{
    ret GetCommandLineW();
}
dfa ER ProcDpiAwareSet()
{
    static BO s_isSet = NO;
    if (s_isSet)
        rets;
    HMODULE lib = NUL;
    if ((lib = LoadLibraryW(L"shcore.dll")))
    {
        typedef HRESULT(WINAPI * FnSetProcessDpiAwareness)(PROCESS_DPI_AWARENESS);
        cx FnSetProcessDpiAwareness setProcessDpiAwareness = (FnSetProcessDpiAwareness)GetProcAddress(lib, "SetProcessDpiAwareness");
        ifl (setProcessDpiAwareness != NUL)
        {
            cx AU result = setProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
            ifl (result == S_OK || result == E_ACCESSDENIED) // E_ACCESSDENIED means the process is already DPI aware
                s_isSet = YES;
        }
    }
    else if ((lib = LoadLibraryW(L"user32.dll")))
    {
        typedef BOOL(WINAPI * FnSetProcessDPIAware)();
        cx FnSetProcessDPIAware setProcessDPIAware = (FnSetProcessDPIAware)GetProcAddress(lib, "SetProcessDPIAware");
        if (setProcessDPIAware && (setProcessDPIAware() != 0))
            s_isSet = YES;
    }
    if (lib != NUL)
        FreeLibrary(lib);
    ifu (s_isSet == NO)
        rete(ErrVal::PROC);
    rets;
}
dfa ER ProcPrioSet(U1 prio, BO isFocus = NO)
{
    PROCESS_PRIORITY_CLASS info = {};
    info.Foreground = isFocus;
    info.PriorityClass = prio;
    cx AU status = NtSetInformationProcess_(ProcCurHdl(), PROCESSINFOCLASS_::ProcessPriorityClass, &info, siz(info));
    ifu (status != STATUS_SUCCESS)
        rete(ErrVal::PROC);
    rets;
}

dfa ER ProcNewFile(cx CH* path)
{
    SHELLEXECUTEINFOW info = {};
    info.cbSize = siz(info);
    info.fMask = SEE_MASK_NOCLOSEPROCESS;
    info.lpVerb = L"open";
    info.lpFile = path;
    info.nShow = SW_SHOW;
    ifu (ShellExecuteExW(&info) == NO || UA(info.hInstApp) <= 32 || info.hProcess == NUL)
        rete(ErrVal::PROC);
    rets;
}

class Proc
{
  private:
    PROCESS_INFORMATION m_info;
    std::wstring m_args;

  public:
    dfa NT __Drop()
    {
        m_info.hProcess = NUL;
    }

  public:
    dfa HANDLE Hdl() cx
    {
        ret m_info.hProcess;
    }
    dfa ER RetVal(U4& out) cx
    {
        out = -1;
        ifu (m_info.hProcess == NUL)
            rete(ErrVal::NO_INIT);
        DWORD r;
        cx BOOL result = GetExitCodeProcess(m_info.hProcess, &r);
        ifu (result == 0)
            rete(ErrVal::PROC);
        ifu (r == STILL_ACTIVE)
            rete(ErrVal::YES_ACTIVE);
        out = r;
        rets;
    }
    dfa BO IsActive() cx
    {
        ifu (m_info.hProcess == NUL)
            ret NO;
        DWORD r;
        cx BOOL result = GetExitCodeProcess(m_info.hProcess, &r);
        ifu (result == 0)
            ret NO;
        if (r == STILL_ACTIVE)
            ret YES;
        ret NO;
    }
    dfa ER Close()
    {
        if (m_info.hProcess == NUL)
            rets;
        if (tx->IsActive())
            rete(ErrVal::YES_ACTIVE);
        ifu (m_info.hThread != NUL && CloseHandle(m_info.hThread) == 0)
            rete(ErrVal::THD);
        ifu (m_info.hProcess != NUL && CloseHandle(m_info.hProcess) == 0)
            rete(ErrVal::PROC);
        MemSet(&m_info, 0, siz(m_info));
        m_args = L"";
        rets;
    }
    dfa ER Wait() cx
    {
        if (tx->IsActive() == NO)
            rets;
        ifu (WaitForSingleObject(m_info.hProcess, INFINITE) != WAIT_OBJECT_0)
            rete(ErrVal::PROC);
        rets;
    }
    dfa ER Start(cx CH* path, cx CH* args, cx CH* workPath)
    {
        ifu (tx->IsActive())
            rete(ErrVal::YES_ACTIVE);
        ife (tx->Close())
            retep;
        m_args = L"\"";
        m_args += path;
        m_args += L"\"";
        if (args != NUL && args[0] != '\0')
        {
            m_args += L" ";
            m_args += args;
        }
        STARTUPINFOW tmp = {};
        MemSet(&tmp, 0, siz(tmp));
        tmp.cb = siz(tmp);
        cx BOOL result = CreateProcessW(path, const_cast<CH*>(m_args.c_str()), NUL, NUL, NO, 0, NUL, workPath, &tmp, &m_info);
        ifu (result == 0)
        {
            MemSet(&m_info, 0, siz(m_info));
            m_args = L"";
            rete(ErrVal::PROC);
        }
        rets;
    }
    dfa ER StartElevated(cx CH* path, cx CH* args, cx CH* workPath)
    {
        if (ProcIsElevated())
            ret tx->Start(path, args, workPath);
        ifu (tx->IsActive())
            rete(ErrVal::YES_ACTIVE);
        ife (tx->Close())
            retep;
        m_args = args;
        SHELLEXECUTEINFOW sei = {};
        sei.cbSize = siz(sei);
        sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NO_CONSOLE;
        sei.lpVerb = L"runas";
        sei.lpFile = path;
        sei.lpParameters = args;
        sei.lpDirectory = workPath;
        sei.nShow = SW_SHOW;
        ifu (ShellExecuteExW(&sei) == NO || UA(sei.hInstApp) <= 32 || sei.hProcess == NUL)
        {
            m_args = L"";
            rete(ErrVal::PROC);
        }
        MemSet(&m_info, 0, siz(m_info));
        m_info.hProcess = sei.hProcess;
        rets;
    }
    dfa ER Stop() cx
    {
        if (tx->IsActive() == NO)
            rets;
        ifu (TerminateProcess(m_info.hProcess, U4(-1)) == 0)
            rete(ErrVal::PROC);
        ife (tx->Wait())
            retep;
        rets;
    }

  public:
    dfa Proc()
    {
        MemSet(&m_info, 0, siz(m_info));
    }
    dfa ~Proc()
    {
        tx->Stop();
        tx->Close();
    }
};

dfa ER ProcRestartElevated()
{
    if (ProcIsElevated())
        rets;
    Proc proc;
    ife (proc.StartElevated(ProcFilePath(), StrArgSkip(ProcArgFullGet()), ProcWorkPath()))
        retep;
    proc.__Drop();
    ProcExit(0);
    rets;
}

cxex SI PROC_GLOBAL_STR_LEN_MAX = PATH_LEN_MAX;

class ProcGlobalStr
{
  private:
    HANDLE m_hdl;
    CH m_str[PROC_GLOBAL_STR_LEN_MAX + 1];

  private:
    dfa NT Init()
    {
        m_hdl = NUL;
        m_str[0] = '\0';
    }

  public:
    dfa NT __Drop()
    {
        tx->Init();
    }

  public:
    dfa BO IsOpenCur() cx
    {
        ret m_hdl != NUL;
    }
    dfa cx CH* Str() cx
    {
        ret m_str;
    }
    dfa BO IsOpen(cx CH* str) cx
    {
        SetLastError(0);
        cx HANDLE hdl = CreateMutexW(NUL, NO, str);
        ifu (hdl == NUL)
            ret NO;
        cx BO r = (GetLastError() == ERROR_ALREADY_EXISTS);
        CloseHandle(hdl);
        ret r;
    }
    dfa ER Close()
    {
        if (tx->IsOpenCur() == NO)
            rets;
        ifu (CloseHandle(m_hdl) == 0)
            rete(ErrVal::PROC);
        m_hdl = NUL;
        rets;
    }
    dfa ER Open(cx CH* str)
    {
        ifu (tx->IsOpenCur())
            rete(ErrVal::YES_INIT);
        cx SI strLen = StrLen(str);
        ifu (strLen == 0)
            rete(ErrVal::LOW_SIZE);
        ifu (strLen > PROC_GLOBAL_STR_LEN_MAX)
            rete(ErrVal::HIGH_SIZE);
        SetLastError(0);
        m_hdl = CreateMutexW(NUL, YES, str);
        ifu (m_hdl == NUL)
            rete(ErrVal::PROC);
        ifu (GetLastError() == ERROR_ALREADY_EXISTS)
        {
            CloseHandle(m_hdl);
            m_hdl = NUL;
            rete(ErrVal::YES_EXIST);
        }
        MemCpy(m_str, str, (strLen + 1) * siz(str[0]));
        rets;
    }
    dfa ER WaitIsClose(cx CH* str) cx
    {
        SetLastError(0);
        cx HANDLE hdl = CreateMutexW(NUL, NO, str);
        ifu (hdl == NUL)
            rete(ErrVal::PROC);
        ifu ((GetLastError() == ERROR_ALREADY_EXISTS) && (WaitForSingleObject(hdl, INFINITE) != WAIT_OBJECT_0))
        {
            CloseHandle(hdl);
            rete(ErrVal::PROC);
        }
        CloseHandle(hdl);
        rets;
    }

  public:
    dfa ProcGlobalStr()
    {
        tx->Init();
    }
    dfa ProcGlobalStr(cx CH* str)
    {
        tx->Init();
        tx->Open(str);
    }
    dfa ~ProcGlobalStr()
    {
        tx->Close();
    }
};

dfa BO ProcGlobalTake(cx CH* str = NUL)
{
    CH path[PATH_LENX_MAX];
    if (str == NUL)
    {
        ProcFilePath(path);
        PathSanitize(path, path);
    }
    else
    {
        StrCpy(path, str);
    }
    ProcGlobalStr global;
    ife (global.Open(path))
        ret NO;
    global.__Drop();
    ret YES;
}

#endif
