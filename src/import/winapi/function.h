#pragma once

#define _importfnwin(retType) __declspec(dllimport) retType WINAPI

#ifdef PROG_COMPILER_MSVC
    #pragma comment(lib, "ntdll.lib")
    #pragma comment(lib, "user32.lib")
#endif

extern "C"
{
    _importfnwin(NTSTATUS) LsaOpenPolicy(PLSA_UNICODE_STRING, PLSA_OBJECT_ATTRIBUTES, ACCESS_MASK, PLSA_HANDLE);
    _importfnwin(NTSTATUS) LsaLookupSids(LSA_HANDLE, ULONG, PSID*, PLSA_REFERENCED_DOMAIN_LIST*, PLSA_TRANSLATED_NAME*);
    _importfnwin(NTSTATUS) LsaFreeMemory(PVOID);
    _importfnwin(NTSTATUS) LsaClose(LSA_HANDLE);
#ifdef PROG_COMPILER_GCC
    _importfnwin(BOOL) SleepConditionVariableCS(PCONDITION_VARIABLE, PCRITICAL_SECTION, DWORD);
    _importfnwin(void) WakeConditionVariable(PCONDITION_VARIABLE);
    _importfnwin(void) WakeAllConditionVariable(PCONDITION_VARIABLE);
    _importfnwin(void) InitializeConditionVariable(PCONDITION_VARIABLE);
#endif
}
