#pragma once

#define _importfnwin(retType) __declspec(dllimport) retType WINAPI

#ifdef PROG_COMPILER_MSVC
	#pragma comment(lib, "ntdll.lib")
#endif

extern "C" {
	_importfnwin(NTSTATUS)NtSetTimerResolution(ULONG, BOOLEAN, PULONG);
	_importfnwin(NTSTATUS)NtAllocateVirtualMemory(HANDLE, PVOID*, ULONG_PTR, PSIZE_T, ULONG, ULONG);
	_importfnwin(NTSTATUS)NtFreeVirtualMemory(HANDLE, PVOID*, PSIZE_T, ULONG);
	_importfnwin(NTSTATUS)NtDelayExecution(BOOLEAN, PLARGE_INTEGER);
	_importfnwin(NTSTATUS)NtSetInformationFile(HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS);
	_importfnwin(NTSTATUS)NtReadFile(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, PLARGE_INTEGER, PULONG);
	_importfnwin(NTSTATUS)NtQueryInformationFile(HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS);
	_importfnwin(VOID)RtlExitUserProcess(NTSTATUS);
}
