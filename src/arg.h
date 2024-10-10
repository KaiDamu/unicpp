#pragma once

using AL = va_list;

#define AlNew(al, ...) va_start(al, __VA_ARGS__)
#define AlDel(al) va_end(al)

SI g_argCnt = 0;
CH** g_argVal = NUL;

dfa NT _ArgInit()
{
    ifl (g_argVal != NUL)
        ret;
    int tmp = 0;
    g_argVal = CommandLineToArgvW(ProcCurArgFullGet(), &tmp);
    g_argCnt = tmp;
    Assert(g_argVal != NUL);
}
dfa SI ArgCnt()
{
    _ArgInit();
    ret g_argCnt;
}
dfa CH* ArgVal(SI i)
{
    _ArgInit();
    Assert(i < g_argCnt);
    ret g_argVal[i];
}
