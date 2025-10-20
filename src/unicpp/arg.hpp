#pragma once

#include <cstdarg>
#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "macro-off.hpp"
    #include <windows.h>
    #include "macro-on.hpp"
#endif
#include "type.hpp"

using AL = va_list;

#define AlNew(al, ...) va_start(al, __VA_ARGS__)
#define AlDel(al) va_end(al)

extern SI g_argCnt;
extern CH** g_argVal;

#ifndef PROG_SYS_WIN

extern CS** g_argValStd;

#endif

dfa NT _ArgInit();
dfa SI ArgCnt();
dfa CH* ArgVal(SI i);

#include "arg.ipp"
