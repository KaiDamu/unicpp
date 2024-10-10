#pragma once

#define _importfnstdc(retType) __declspec(dllimport) retType __cdecl

extern "C"
{
#ifdef PROG_COMPILER_GCC
    _importfnstdc(double) sqrt(double);
    _importfnstdc(float) sqrtf(float);
#endif
}
