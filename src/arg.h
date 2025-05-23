#pragma once

using AL = va_list;

#define AlNew(al, ...) va_start(al, __VA_ARGS__)
#define AlDel(al) va_end(al)

SI g_argCnt = 0;
CH** g_argVal = NUL;

#ifndef PROG_SYS_WIN

CS** g_argValStd = NUL;

#endif

dfa NT _ArgInit()
{
    ifl (g_argVal != NUL)
        ret;
#ifdef PROG_SYS_WIN
    int tmp = 0;
    g_argVal = CommandLineToArgvW(ProcArgFullGet(), &tmp);
    g_argCnt = tmp;
    Assert(g_argVal != NUL);
#else
    g_argVal = new CH*[g_argCnt];
    ite (i, i < g_argCnt)
    {
        g_argVal[i] = new CH[StrLen(g_argValStd[i]) + STR_EX_LEN];
        CsstrToChstr(g_argVal[i], g_argValStd[i]);
    }
#endif
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
