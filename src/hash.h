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
    NtQuerySystemInformation = 0x90DC89908361D8F9,  // "ntdll.dll!NtQuerySystemInformation"
    NtQueryInformationProcess = 0x97660899AF920B9B, // "ntdll.dll!NtQueryInformationProcess"
    NtOpenProcess = 0x18A1232E3C0B0A91,             // "ntdll.dll!NtOpenProcess"
    NtReadVirtualMemory = 0x068D37D5AD0FFC34,       // "ntdll.dll!NtReadVirtualMemory"
    NtClose = 0x0EA8815DD2EFDD00,                   // "ntdll.dll!NtClose"
};
