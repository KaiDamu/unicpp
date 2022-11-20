#pragma once

#define _importfnwin(retType) __declspec(dllimport) retType WINAPI

extern "C" {
	_importfnwin(NTSTATUS)NtSetTimerResolution(ULONG, BOOLEAN, PULONG);
}
