#pragma once

dfa UNICODE_STRING_::UNICODE_STRING_()
{
}
dfa UNICODE_STRING_::UNICODE_STRING_(cx CH* Buffer)
{
    tx->Length = StrLen(Buffer) * siz(Buffer[0]);
    tx->MaximumLength = tx->Length + siz(Buffer[0]);
    tx->Buffer = TO(tx->Buffer)(Buffer);
}
dfa OBJECT_ATTRIBUTES_::OBJECT_ATTRIBUTES_()
{
    tx->Length = siz(OBJECT_ATTRIBUTES_);
    tx->RootDirectory = NUL;
    tx->ObjectName = NUL;
    tx->Attributes = 0;
    tx->SecurityDescriptor = NUL;
    tx->SecurityQualityOfService = NUL;
}

#define _UNI_NT_LOAD_FN(fnName)                                                       \
    ifu ((fnName##_ = (fnName##_T)uniNtLoadCache.FnAdrGet(MdlFnHash::fnName)) == NUL) \
        rete(ErrVal::NO_EXIST);

dfa ER UniNtLoad()
{
    MdlFnCache uniNtLoadCache;
    uniNtLoadCache.CacheDll(L"ntdll.dll");

    _UNI_NT_LOAD_FN(LdrLoadDll);
    _UNI_NT_LOAD_FN(LdrUnloadDll);
    _UNI_NT_LOAD_FN(NtAdjustPrivilegesToken);
    _UNI_NT_LOAD_FN(NtAllocateVirtualMemory);
    _UNI_NT_LOAD_FN(NtClose);
    _UNI_NT_LOAD_FN(NtCreateEvent);
    _UNI_NT_LOAD_FN(NtCreateFile);
    _UNI_NT_LOAD_FN(NtCreateKeyedEvent);
    _UNI_NT_LOAD_FN(NtCreateProcessEx);
    _UNI_NT_LOAD_FN(NtCreateSection);
    _UNI_NT_LOAD_FN(NtCreateThreadEx);
    _UNI_NT_LOAD_FN(NtCreateToken);
    _UNI_NT_LOAD_FN(NtCreateUserProcess);
    _UNI_NT_LOAD_FN(NtDelayExecution);
    _UNI_NT_LOAD_FN(NtDuplicateObject);
    _UNI_NT_LOAD_FN(NtDuplicateToken);
    _UNI_NT_LOAD_FN(NtFlushInstructionCache);
    _UNI_NT_LOAD_FN(NtFreeVirtualMemory);
    _UNI_NT_LOAD_FN(NtImpersonateThread);
    _UNI_NT_LOAD_FN(NtLoadDriver);
    _UNI_NT_LOAD_FN(NtMapViewOfSection);
    _UNI_NT_LOAD_FN(NtOpenKeyEx);
    _UNI_NT_LOAD_FN(NtOpenKeyedEvent);
    _UNI_NT_LOAD_FN(NtOpenProcess);
    _UNI_NT_LOAD_FN(NtOpenProcessTokenEx);
    _UNI_NT_LOAD_FN(NtOpenThread);
    _UNI_NT_LOAD_FN(NtOpenThreadTokenEx);
    _UNI_NT_LOAD_FN(NtQueryDirectoryFile);
    _UNI_NT_LOAD_FN(NtQueryInformationFile);
    _UNI_NT_LOAD_FN(NtQueryInformationProcess);
    _UNI_NT_LOAD_FN(NtQueryInformationToken);
    _UNI_NT_LOAD_FN(NtQueryObject);
    _UNI_NT_LOAD_FN(NtQuerySystemInformation);
    _UNI_NT_LOAD_FN(NtQueryTimerResolution);
    _UNI_NT_LOAD_FN(NtQueryValueKey);
    _UNI_NT_LOAD_FN(NtRaiseHardError);
    _UNI_NT_LOAD_FN(NtReadFile);
    _UNI_NT_LOAD_FN(NtReadVirtualMemory);
    _UNI_NT_LOAD_FN(NtReleaseKeyedEvent);
    _UNI_NT_LOAD_FN(NtResetEvent);
    _UNI_NT_LOAD_FN(NtSetEvent);
    _UNI_NT_LOAD_FN(NtSetInformationFile);
    _UNI_NT_LOAD_FN(NtSetInformationProcess);
    _UNI_NT_LOAD_FN(NtSetInformationThread);
    _UNI_NT_LOAD_FN(NtSetInformationToken);
    _UNI_NT_LOAD_FN(NtSetTimerResolution);
    _UNI_NT_LOAD_FN(NtSetValueKey);
    _UNI_NT_LOAD_FN(NtShutdownSystem);
    _UNI_NT_LOAD_FN(NtUnloadDriver);
    _UNI_NT_LOAD_FN(NtWaitForKeyedEvent);
    _UNI_NT_LOAD_FN(NtWaitForSingleObject);
    _UNI_NT_LOAD_FN(NtWriteFile);
    _UNI_NT_LOAD_FN(NtWriteVirtualMemory);
    _UNI_NT_LOAD_FN(NtYieldExecution);
    _UNI_NT_LOAD_FN(RtlAcquirePebLock);
    _UNI_NT_LOAD_FN(RtlAdjustPrivilege);
    _UNI_NT_LOAD_FN(RtlExitUserProcess);
    _UNI_NT_LOAD_FN(RtlReleasePebLock);

    rets;
}
