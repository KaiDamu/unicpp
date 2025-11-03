#include "process.hpp"

#include "thread.hpp"
#include "memory.hpp"
#include "token.hpp"
#ifdef PROG_SYS_WIN
    #include <shellapi.h>
    #include <shellscalingapi.h>
#endif

dfa NT ProcCrash()
{
    volatile U1 x = U1(clock());
    volatile U1 y = *reinterpret_cast<U1*>(UA(x ^ x));
    y = y / (x ^ x);
}
dfa NT ProcExit(U4 retVal)
{
#ifdef PROG_SYS_WIN
    RtlExitUserProcess_(retVal);
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

dfa HD ProcCurHdl()
{
    ret HD(-1);
}
dfa HD ProcCurId()
{
    ret ThdTeb()->ClientId.UniqueProcess;
}

dfa LDR_DATA_TABLE_ENTRY_* _ProcMdlEntryGet(SI i)
{
    cx AU teb = ThdTeb();
    cx AU list = &teb->ProcessEnvironmentBlock->Ldr->InLoadOrderModuleList;
    for (AU cur = list->Flink; cur != list; cur = cur->Flink)
    {
        cx AU entry = (LDR_DATA_TABLE_ENTRY_*)cur;
        if (i == 0)
            ret entry;
        --i;
    }
    ret NUL;
}
dfa LDR_DATA_TABLE_ENTRY_* _ProcDllEntryGet(cx CH* dllName)
{
    cx AU teb = ThdTeb();
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

dfa HD ProcMdlCurAdrGet()
{
    AU scanAdr = UA(AdrOfNextOpe());
    scanAdr = AlignBit<UA, 0x1000>(scanAdr) - 0x1000;
    while (YES)
    {
        cx AU& dosHdrGuess = (IMAGE_DOS_HEADER_*)scanAdr;
        if ((dosHdrGuess->e_magic == IMAGE_DOS_SIGNATURE) && IsBetween<S4>(dosHdrGuess->e_lfanew, siz(IMAGE_DOS_HEADER_), 0x0F00))
        {
            cx AU& peHdrGuess = (IMAGE_NT_HEADERS_*)(scanAdr + dosHdrGuess->e_lfanew);
            if (peHdrGuess->Signature == IMAGE_NT_SIGNATURE)
            {
                cx AU& magicGuess = peHdrGuess->OptionalHeader.Magic;
                if ((magicGuess == IMAGE_NT_OPTIONAL_HDR32_MAGIC) || (magicGuess == IMAGE_NT_OPTIONAL_HDR64_MAGIC) || (magicGuess == IMAGE_ROM_OPTIONAL_HDR_MAGIC))
                    ret HD(scanAdr);
            }
        }
        scanAdr -= 0x1000;
    }
}

dfa HD ProcDllLoad(cx CH* dllName)
{
    cx UNICODE_STRING_ str(dllName);
    GA result;
    ifu (LdrLoadDll_(NUL, NUL, &str, &result) != STATUS_SUCCESS)
        ret NUL;
    ret (HD)result;
}
dfa HD ProcDllAdrGet(cx CH* dllName, BO doLoad)
{
    cx AU entry = _ProcDllEntryGet(dllName);
    ifl (entry != NUL)
        ret entry->DllBase;
    if (!doLoad)
        ret NUL;
    ret ProcDllLoad(dllName);
}
dfa ER ProcDllUnload(cx CH* dllName, BO doForce)
{
    cx AU entry = _ProcDllEntryGet(dllName);
    if (entry == NUL)
        rets;
    ret _ProcDllUnload((HD)entry->DllBase, doForce);
}
dfa ER ProcDllUnload(HD dll, BO doForce)
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
        {
            cx AU fnAdr = baseAdr + fnList[ordinalList[i]];
            if (UA(fnAdr) - UA(exportDir) < UA(exportDirInfo.Size))
            {
                AU dllFnNameCur = fnAdr;
                U1 nameBuf[256];
                AU nameCur = nameBuf;
                do
                {
                    *nameCur++ = *dllFnNameCur++;
                    *nameCur++ = '\0';
                } while (*dllFnNameCur != '.');
                *nameCur++ = *dllFnNameCur++;
                *nameCur++ = '\0';
                cxex U8 DLL_EXT = 0x0000006C006C0064;
                MemCpy(nameCur, &DLL_EXT, siz(DLL_EXT));
                AU dllNameRedir = (CH*)nameBuf;
                AU fnNameRedir = (CS*)dllFnNameCur;
                ret ProcFnAdrGet(ProcDllAdrGet(dllNameRedir, YES), fnNameRedir);
            }
            ret AsType<GAFN>(fnAdr);
        }
    }
    ret NUL;
}

dfa ER MdlFnCache::CacheDll(cx CH* dllName)
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
        cx AU fnAdr = baseAdr + fnList[ordinalList[i]];
        if (UA(fnAdr) - UA(exportDir) < UA(exportDirInfo.Size))
        {
            AU dllFnNameCur = fnAdr;
            U1 nameBuf[256];
            AU nameCur = nameBuf;
            do
            {
                *nameCur++ = *dllFnNameCur++;
                *nameCur++ = '\0';
            } while (*dllFnNameCur != '.');
            *nameCur++ = *dllFnNameCur++;
            *nameCur++ = '\0';
            cxex U8 DLL_EXT = 0x0000006C006C0064;
            MemCpy(nameCur, &DLL_EXT, siz(DLL_EXT));
            AU dllNameRedir = (CH*)nameBuf;
            AU fnNameRedir = (CS*)dllFnNameCur;
            m_fnCache[hash] = ProcFnAdrGet(ProcDllAdrGet(dllNameRedir, YES), fnNameRedir);
        }
        else
        {
            m_fnCache[hash] = AsType<GAFN>(fnAdr);
        }
    }
    rets;
}
dfa GAFN MdlFnCache::FnAdrGet(FNV1A64 hash)
{
    cx AU it = m_fnCache.find(hash);
    ifu (it == m_fnCache.end())
        ret NUL;
    ret it->second;
}
dfa MdlFnCache::MdlFnCache()
{
}

dfa cx CH* ProcFilePath()
{
    static CH s_cache[PATH_LENX_MAX] = {};
    ifu (s_cache[0] == '\0')
    {
        cx AU entry = _ProcMdlEntryGet(0);
        ifu (entry == NUL)
            ret NUL;
        cx AU& path = entry->FullDllName;
        MemCpy(s_cache, path.Buffer, path.Length);
        s_cache[path.Length / siz(CH)] = '\0';
    }
    ret s_cache;
}
dfa cx CH* ProcDirPath()
{
    static CH s_cache[PATH_LENX_MAX] = {};
    ifu (s_cache[0] == '\0')
    {
        StrCpy(s_cache, ProcFilePath());
        PathDirUp(s_cache);
    }
    ret s_cache;
}
dfa SI ProcWorkPathGet(CH* path)
{
    cx AU& path_ = ThdTeb()->ProcessEnvironmentBlock->ProcessParameters->CurrentDirectory.DosPath;
    MemCpy(path, path_.Buffer, path_.Length);
    cx AU pathLen = SI(path_.Length / siz(CH));
    cx BO hasTrailSlash = (path[pathLen - 1] == CH_PATH_DIR && path[pathLen - 2] != CH_PATH_DRIVE);
    path[pathLen - hasTrailSlash] = '\0';
    ret SI(pathLen - hasTrailSlash);
}
dfa ER ProcWorkPathSet(cx CH* path)
{
    ifu (SetCurrentDirectoryW(path) == 0)
        rete(ErrVal::PROC);
    rets;
}
dfa SI ProcWorkPathLen()
{
    cx AU& path = ThdTeb()->ProcessEnvironmentBlock->ProcessParameters->CurrentDirectory.DosPath;
    cx AU pathLen = SI(path.Length / siz(CH));
    cx BO hasTrailSlash = (path.Buffer[pathLen - 1] == CH_PATH_DIR && path.Buffer[pathLen - 2] != CH_PATH_DRIVE);
    ret SI(pathLen - hasTrailSlash);
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
dfa ER ProcPrioSet(U1 prio, BO isFocus)
{
    PROCESS_PRIORITY_CLASS_ info = {};
    info.Foreground = isFocus;
    info.PriorityClass = prio;
    cx AU status = NtSetInformationProcess_(ProcCurHdl(), PROCESSINFOCLASS_::ProcessPriorityClass, &info, siz(info));
    ifu (status != STATUS_SUCCESS)
        rete(ErrVal::PROC);
    rets;
}

dfa ER ProcNewByPath(cx CH* path)
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

dfa HD _Proc::Hdl() cx
{
    ret m_hdl;
}
dfa ER _Proc::Open(HD pid, U4 access)
{
    ifu (m_hdl != NUL)
        rete(ErrVal::YES_INIT);
    OBJECT_ATTRIBUTES_ oa;
    CLIENT_ID_ ci = {};
    ci.UniqueProcess = pid;
    cx AU status = NtOpenProcess_(&m_hdl, access, &oa, &ci);
    ifu (status != STATUS_SUCCESS)
    {
        m_hdl = NUL;
        rete(ErrVal::PROC);
    }
    rets;
}
dfa ER _Proc::Close()
{
    if (m_hdl == NUL)
        rets;
    cx AU status = NtClose_(m_hdl);
    ifu (status != STATUS_SUCCESS)
        rete(ErrVal::PROC);
    m_hdl = NUL;
    rets;
}
dfa _Proc::_Proc() : m_hdl(NUL)
{
}
dfa _Proc::~_Proc()
{
    tx->Close();
}

dfa ER _ProcListGet(std::vector<SYSTEM_PROCESS_INFORMATION_>& out)
{
    // WARNING: the 'Threads' field is not included in the output structure
    out.clear();
    std::vector<U1> buf;
    U4 bufSize = 0;
    while (YES)
    {
        cx AU status = NtQuerySystemInformation_(SYSTEM_INFORMATION_CLASS_::SystemProcessInformation, buf.data(), bufSize, &bufSize);
        if (status == STATUS_SUCCESS)
            break;
        if (status != STATUS_INFO_LENGTH_MISMATCH)
            rete(ErrVal::PROC);
        bufSize += BYTE_IN_KB;
        buf.resize(bufSize);
    }
    AU procInfoBase = (SYSTEM_PROCESS_INFORMATION_*)buf.data();
    AU procInfoCur = procInfoBase;
    SI procCnt = 0;
    while (procInfoCur->NextEntryOffset != 0)
    {
        procInfoCur = (SYSTEM_PROCESS_INFORMATION_*)((U1*)procInfoCur + procInfoCur->NextEntryOffset);
        ++procCnt;
    }
    out.reserve(procCnt);
    procInfoCur = procInfoBase;
    ite (i, i < procCnt)
    {
        out.push_back(*procInfoCur);
        procInfoCur = (SYSTEM_PROCESS_INFORMATION_*)((U1*)procInfoCur + procInfoCur->NextEntryOffset);
    }
    rets;
}
dfa ER _ProcIsUserLocalSystemByHdl(BO& isTrue, HD hdl)
{
    isTrue = NO;
    SecToken token;
    ife (token.OpenByProc(hdl, TOKEN_QUERY, 0))
        retep;
    cx AU sidUser = token.SidUser();
    ifu (sidUser == NUL)
        rete(ErrVal::TOKEN);
    isTrue = SidIsSame(*sidUser, *SidLocalSystem());
    rets;
}
dfa ER _ProcIsUserLocalSystemByPid(BO& isTrue, HD pid)
{
    isTrue = NO;
    _Proc proc;
    ife (proc.Open(pid, PROCESS_QUERY_LIMITED_INFORMATION))
        retep;
    ret _ProcIsUserLocalSystemByHdl(isTrue, proc.Hdl());
}

dfa ER ProcUsernameGet(HD proc, std::wstring* user, std::wstring* domain)
{
    if (user != NUL)
        user->clear();
    if (domain != NUL)
        domain->clear();
    SecToken token;
    ife (token.OpenByProc(proc, TOKEN_QUERY, 0))
        retep;
    cx AU sidUser = token.SidUser();
    ifu (sidUser == NUL)
        rete(ErrVal::TOKEN);
    LSA_OBJECT_ATTRIBUTES loa = {};
    loa.Length = siz(loa);
    LSA_HANDLE policy;
    ifu (LsaOpenPolicy(NUL, &loa, POLICY_LOOKUP_NAMES, &policy) != STATUS_SUCCESS)
        rete(ErrVal::PROC);
    cx AU sidUserParam = const_cast<PSID*>((CXGA cx*)&sidUser);
    LSA_REFERENCED_DOMAIN_LIST* domainList = NUL;
    LSA_TRANSLATED_NAME* userList = NUL;
    ifu (LsaLookupSids(policy, 1, sidUserParam, &domainList, &userList) != STATUS_SUCCESS)
        rete(ErrVal::TOKEN);
    if (domainList)
    {
        if ((domain != NUL) && (domainList->Entries > 0))
            domain->assign(domainList->Domains[0].Name.Buffer, domainList->Domains[0].Name.Length / siz(WCHAR));
        ifu (LsaFreeMemory(domainList) != STATUS_SUCCESS)
            rete(ErrVal::MEM);
    }
    if (userList)
    {
        if (user != NUL)
            user->assign(userList->Name.Buffer, userList->Name.Length / siz(WCHAR));
        ifu (LsaFreeMemory(userList) != STATUS_SUCCESS)
            rete(ErrVal::MEM);
    }
    ifu (LsaClose(policy) != STATUS_SUCCESS)
        rete(ErrVal::PROC);
    rets;
}

dfa NT Proc::__Drop()
{
    m_info.hProcess = NUL;
}
dfa HANDLE Proc::Hdl() cx
{
    ret m_info.hProcess;
}
dfa ER Proc::RetVal(U4& out) cx
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
dfa BO Proc::IsActive() cx
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
dfa ER Proc::Close()
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
dfa ER Proc::Wait() cx
{
    if (tx->IsActive() == NO)
        rets;
    ifu (WaitForSingleObject(m_info.hProcess, INFINITE) != WAIT_OBJECT_0)
        rete(ErrVal::PROC);
    rets;
}
dfa ER Proc::Start(cx CH* path, cx CH* args, cx CH* workPath)
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
dfa ER Proc::StartElevated(cx CH* path, cx CH* args, cx CH* workPath)
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
dfa ER Proc::Stop() cx
{
    if (tx->IsActive() == NO)
        rets;
    ifu (TerminateProcess(m_info.hProcess, U4(-1)) == 0)
        rete(ErrVal::PROC);
    ife (tx->Wait())
        retep;
    rets;
}
dfa Proc::Proc()
{
    MemSet(&m_info, 0, siz(m_info));
}
dfa Proc::~Proc()
{
    tx->Stop();
    tx->Close();
}

dfa ER ProcRestartElevated()
{
    if (ProcIsElevated())
        rets;
    Proc proc;
    CH workPath[PATH_LENX_MAX];
    ProcWorkPathGet(workPath);
    ife (proc.StartElevated(ProcFilePath(), StrArgSkip(ProcArgFullGet()), workPath))
        retep;
    proc.__Drop();
    ProcExit(0);
    rets;
}

dfa TmCpu ProcTimeCpu(HD proc)
{
    PROCESS_CYCLE_TIME_INFORMATION_ info;
    ifu (NtQueryInformationProcess_(ProcCurHdl(), PROCESSINFOCLASS_::ProcessCycleTime, &info, siz(info), NUL) != STATUS_SUCCESS)
        ret TmCpu(0);
    ret TmCpu(info.AccumulatedCycles);
}

dfa TmCpu TimerCpu::TimeGet()
{
    ret ProcTimeCpu(ProcCurHdl());
}
dfa TmCpu TimerCpu::Read()
{
    if (m_isRunning)
    {
        cx AU timeNow = tx->TimeGet();
        m_time += timeNow - m_timeStart;
        m_timeStart = timeNow;
    }
    ret m_time;
}
dfa TmCpu TimerCpu::ReadK()
{
    ret tx->Read() / TmCpu(1000);
}
dfa TmCpu TimerCpu::ReadM()
{
    ret tx->Read() / TmCpu(1000000);
}
dfa NT TimerCpu::Start()
{
    ifu (m_isRunning)
        ret;
    m_isRunning = YES;
    m_timeStart = tx->TimeGet();
}
dfa TmCpu TimerCpu::Pause()
{
    ifu (!m_isRunning)
        ret m_time;
    m_isRunning = NO;
    m_time += tx->TimeGet() - m_timeStart;
    ret m_time;
}
dfa TmCpu TimerCpu::Restart()
{
    cx AU timeNow = tx->TimeGet();
    cx AU timeRet = m_isRunning ? (m_time + timeNow - m_timeStart) : m_time;
    m_isRunning = YES;
    m_time = TmCpu(0);
    m_timeStart = timeNow;
    ret timeRet;
}
dfa TimerCpu::TimerCpu() : m_time(), m_timeStart(), m_isRunning()
{
    tx->Restart();
}
dfa TimerCpu::~TimerCpu()
{
}

dfa NT ProcGlobalStr::Init()
{
    m_hdl = NUL;
    m_str[0] = '\0';
}
dfa NT ProcGlobalStr::__Drop()
{
    tx->Init();
}
dfa BO ProcGlobalStr::IsOpenCur() cx
{
    ret m_hdl != NUL;
}
dfa cx CH* ProcGlobalStr::Str() cx
{
    ret m_str;
}
dfa BO ProcGlobalStr::IsOpen(cx CH* str) cx
{
    SetLastError(0);
    cx HANDLE hdl = CreateMutexW(NUL, NO, str);
    ifu (hdl == NUL)
        ret NO;
    cx BO r = (GetLastError() == ERROR_ALREADY_EXISTS);
    CloseHandle(hdl);
    ret r;
}
dfa ER ProcGlobalStr::Close()
{
    if (tx->IsOpenCur() == NO)
        rets;
    ifu (CloseHandle(m_hdl) == 0)
        rete(ErrVal::PROC);
    m_hdl = NUL;
    rets;
}
dfa ER ProcGlobalStr::Open(cx CH* str)
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
dfa ER ProcGlobalStr::WaitIsClose(cx CH* str) cx
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
dfa ProcGlobalStr::ProcGlobalStr()
{
    tx->Init();
}
dfa ProcGlobalStr::ProcGlobalStr(cx CH* str)
{
    tx->Init();
    tx->Open(str);
}
dfa ProcGlobalStr::~ProcGlobalStr()
{
    tx->Close();
}

dfa BO ProcGlobalTake(cx CH* str)
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
