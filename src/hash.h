#pragma once

using FNV1A64 = U8;
using FNV1A64L = U8;

cxex FNV1A64 FNV1A64_INIT_VAL = 0xCBF29CE484222325ULL;

dfa NT _HashFnv1a64_1(FNV1A64& hash, U1 val)
{
    hash = (hash ^ val) * 0x00000100000001B3ULL;
}
dfa NT _HashFnv1a64_2(FNV1A64& hash, SI size)
{
    hash = (hash << sizb(U1)) | (size & 0xFF);
}
tpl<typename T1, U1 M> dfa T1 _HashFnv1a64(CXGA buf, SI size, FNV1A64 init)
{
    AU cur = (cx U1*)buf;
    cx AU end = cur + size;
    FNV1A64& hash = init;
    while (cur != end)
        _HashFnv1a64_1(hash, *cur++);
    ifcx (M == 2)
        _HashFnv1a64_2(hash, size);
    ret T1(hash);
}
tpl<typename T1, U1 M> dfa T1 _HashFnv1a64Str(cx CS* str, FNV1A64 init)
{
    AU cur = (cx U1*)str;
    FNV1A64& hash = init;
    while (*cur != '\0')
        _HashFnv1a64_1(hash, *cur++);
    ifcx (M == 2)
        _HashFnv1a64_2(hash, cur - (cx U1*)str);
    ret T1(hash);
}

dfa FNV1A64 HashFnv1a64(CXGA buf, SI size, FNV1A64 init = FNV1A64_INIT_VAL)
{
    ret _HashFnv1a64<FNV1A64, 1>(buf, size, init);
}
dfa FNV1A64L HashFnv1a64l(CXGA buf, SI size, FNV1A64L init = FNV1A64_INIT_VAL)
{
    ret _HashFnv1a64<FNV1A64L, 2>(buf, size, FNV1A64(init));
}
dfa FNV1A64 HashFnv1a64Str(cx CS* str, FNV1A64 init = FNV1A64_INIT_VAL)
{
    ret _HashFnv1a64Str<FNV1A64, 1>(str, init);
}
dfa FNV1A64L HashFnv1a64lStr(cx CS* str, FNV1A64L init = FNV1A64_INIT_VAL)
{
    ret _HashFnv1a64Str<FNV1A64L, 2>(str, FNV1A64(init));
}

enum class MdlFnHash : FNV1A64
{
    LdrLoadDll = 0x43F66F86D3C067A4,                // "ntdll.dll!LdrLoadDll"
    LdrUnloadDll = 0xFC8493E3D4BC8B09,              // "ntdll.dll!LdrUnloadDll"
    NtAdjustPrivilegesToken = 0xECDBF8E39DC3F188,   // "ntdll.dll!NtAdjustPrivilegesToken"
    NtAllocateVirtualMemory = 0xF9A4A82670D9FD19,   // "ntdll.dll!NtAllocateVirtualMemory"
    NtClose = 0x0EA8815DD2EFDD00,                   // "ntdll.dll!NtClose"
    NtCreateEvent = 0x0FAEF1F8C3DC5610,             // "ntdll.dll!NtCreateEvent"
    NtCreateFile = 0x7334688B27564FBE,              // "ntdll.dll!NtCreateFile"
    NtCreateKeyedEvent = 0x6001B2CBC458EE62,        // "ntdll.dll!NtCreateKeyedEvent"
    NtCreateProcessEx = 0x68366B100519EC48,         // "ntdll.dll!NtCreateProcessEx"
    NtCreateSection = 0xEDF66E9A64A8869F,           // "ntdll.dll!NtCreateSection"
    NtCreateThreadEx = 0x74F2310CFAAD517D,          // "ntdll.dll!NtCreateThreadEx"
    NtCreateToken = 0x7791746A7256241D,             // "ntdll.dll!NtCreateToken"
    NtCreateUserProcess = 0x99E7C5711E0F4C40,       // "ntdll.dll!NtCreateUserProcess"
    NtDelayExecution = 0x62D0D897D70E466B,          // "ntdll.dll!NtDelayExecution"
    NtDuplicateObject = 0x2E0D8B2AD6DBA8C2,         // "ntdll.dll!NtDuplicateObject"
    NtDuplicateToken = 0x390FFEAD230EA7FA,          // "ntdll.dll!NtDuplicateToken"
    NtFlushInstructionCache = 0x2A8438B44D3A709C,   // "ntdll.dll!NtFlushInstructionCache"
    NtFreeVirtualMemory = 0x84B21891C1103302,       // "ntdll.dll!NtFreeVirtualMemory"
    NtImpersonateThread = 0x2B317E2929076FA5,       // "ntdll.dll!NtImpersonateThread"
    NtLoadDriver = 0x439C9C963CF7677E,              // "ntdll.dll!NtLoadDriver"
    NtMapViewOfSection = 0xD09B1F094A5E8961,        // "ntdll.dll!NtMapViewOfSection"
    NtOpenKeyEx = 0xFF3367EA540125F6,               // "ntdll.dll!NtOpenKeyEx"
    NtOpenKeyedEvent = 0xF9B7FFC77E174586,          // "ntdll.dll!NtOpenKeyedEvent"
    NtOpenProcess = 0x18A1232E3C0B0A91,             // "ntdll.dll!NtOpenProcess"
    NtOpenProcessTokenEx = 0x7E3399C0B3006553,      // "ntdll.dll!NtOpenProcessTokenEx"
    NtOpenThread = 0xEB3418764049AAE4,              // "ntdll.dll!NtOpenThread"
    NtOpenThreadTokenEx = 0xCD0B1E659F3932FC,       // "ntdll.dll!NtOpenThreadTokenEx"
    NtQueryDirectoryFile = 0x809212E1E5C0E1B5,      // "ntdll.dll!NtQueryDirectoryFile"
    NtQueryInformationFile = 0x29F3773442299460,    // "ntdll.dll!NtQueryInformationFile"
    NtQueryInformationProcess = 0x97660899AF920B9B, // "ntdll.dll!NtQueryInformationProcess"
    NtQueryInformationThread = 0x38EEBC3F3D4267A2,  // "ntdll.dll!NtQueryInformationThread"
    NtQueryInformationToken = 0x30ADCF95DB777C33,   // "ntdll.dll!NtQueryInformationToken"
    NtQueryObject = 0xC433B989E0BE2AAF,             // "ntdll.dll!NtQueryObject"
    NtQuerySystemInformation = 0x90DC89908361D8F9,  // "ntdll.dll!NtQuerySystemInformation"
    NtQueryTimerResolution = 0x2A8EF8C52DE4FE8B,    // "ntdll.dll!NtQueryTimerResolution"
    NtQueryValueKey = 0x4C79C7D07705B820,           // "ntdll.dll!NtQueryValueKey"
    NtRaiseHardError = 0x667C4AA603EC3BC5,          // "ntdll.dll!NtRaiseHardError"
    NtReadFile = 0x6D519A49D97618C6,                // "ntdll.dll!NtReadFile"
    NtReadVirtualMemory = 0x068D37D5AD0FFC34,       // "ntdll.dll!NtReadVirtualMemory"
    NtReleaseKeyedEvent = 0x3BC3B3E9970BFE53,       // "ntdll.dll!NtReleaseKeyedEvent"
    NtResetEvent = 0x3E28A3BBBE69FB99,              // "ntdll.dll!NtResetEvent"
    NtSetEvent = 0x1CC78F8D7AC6E8CC,                // "ntdll.dll!NtSetEvent"
    NtSetInformationFile = 0x56D8705FD5D5F1E4,      // "ntdll.dll!NtSetInformationFile"
    NtSetInformationProcess = 0x2575C4D304CAB68F,   // "ntdll.dll!NtSetInformationProcess"
    NtSetInformationThread = 0x3F2557CF5A1BD436,    // "ntdll.dll!NtSetInformationThread"
    NtSetInformationToken = 0xCB68D7538BCE4E17,     // "ntdll.dll!NtSetInformationToken"
    NtSetTimerResolution = 0xC314D04A18EAC16F,      // "ntdll.dll!NtSetTimerResolution"
    NtSetValueKey = 0x256C1F8D2076BB84,             // "ntdll.dll!NtSetValueKey"
    NtShutdownSystem = 0x1BC8D22D39EB3C57,          // "ntdll.dll!NtShutdownSystem"
    NtTerminateProcess = 0x8245D3153CEFF688,        // "ntdll.dll!NtTerminateProcess"
    NtTerminateThread = 0x030262C15C392BDB,         // "ntdll.dll!NtTerminateThread"
    NtUnloadDriver = 0xE350019B59F6795D,            // "ntdll.dll!NtUnloadDriver"
    NtWaitForKeyedEvent = 0x0FFD4DA4C115C542,       // "ntdll.dll!NtWaitForKeyedEvent"
    NtWaitForSingleObject = 0xF1A591FD26D0C873,     // "ntdll.dll!NtWaitForSingleObject"
    NtWriteFile = 0x50C2A7BA992CF725,               // "ntdll.dll!NtWriteFile"
    NtWriteVirtualMemory = 0xCEA0739A32888649,      // "ntdll.dll!NtWriteVirtualMemory"
    NtYieldExecution = 0x9DF8CD4A2D43943D,          // "ntdll.dll!NtYieldExecution"
    RtlAcquirePebLock = 0x448F3628D84BC4C0,         // "ntdll.dll!RtlAcquirePebLock"
    RtlAdjustPrivilege = 0xA4C60E712A549D92,        // "ntdll.dll!RtlAdjustPrivilege"
    RtlExitUserProcess = 0x00610A0CC27A81C4,        // "ntdll.dll!RtlExitUserProcess"
    RtlReleasePebLock = 0x23134CF8AC8E868F,         // "ntdll.dll!RtlReleasePebLock"
};
