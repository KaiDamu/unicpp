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

#define _UNI_NT_LOAD_FN(fnName)                                                       \
    ifu ((fnName##_ = (fnName##_T)uniNtLoadCache.FnAdrGet(MdlFnHash::fnName)) == NUL) \
        rete(ErrVal::NO_EXIST);

dfa ER UniNtLoad()
{
    MdlFnCache uniNtLoadCache;
    uniNtLoadCache.CacheDll(L"ntdll.dll");

    _UNI_NT_LOAD_FN(LdrLoadDll);
    _UNI_NT_LOAD_FN(LdrUnloadDll);
    _UNI_NT_LOAD_FN(NtQuerySystemInformation);
    _UNI_NT_LOAD_FN(NtQueryInformationProcess);
    _UNI_NT_LOAD_FN(NtOpenProcess);
    _UNI_NT_LOAD_FN(NtReadVirtualMemory);
    _UNI_NT_LOAD_FN(RtlAdjustPrivilege);
    _UNI_NT_LOAD_FN(NtWriteVirtualMemory);
    _UNI_NT_LOAD_FN(NtAllocateVirtualMemory);
    _UNI_NT_LOAD_FN(NtFlushInstructionCache);
    _UNI_NT_LOAD_FN(NtOpenProcessTokenEx);
    _UNI_NT_LOAD_FN(NtAdjustPrivilegesToken);
    _UNI_NT_LOAD_FN(NtCreateThreadEx);
    _UNI_NT_LOAD_FN(NtOpenThreadTokenEx);
    _UNI_NT_LOAD_FN(NtDuplicateToken);
    _UNI_NT_LOAD_FN(NtSetInformationToken);
    _UNI_NT_LOAD_FN(NtImpersonateThread);
    _UNI_NT_LOAD_FN(NtCreateProcessEx);
    _UNI_NT_LOAD_FN(NtCreateUserProcess);
    _UNI_NT_LOAD_FN(NtQueryObject);
    _UNI_NT_LOAD_FN(NtDuplicateObject);
    _UNI_NT_LOAD_FN(NtQueryInformationToken);
    _UNI_NT_LOAD_FN(NtClose);

    rets;
}
