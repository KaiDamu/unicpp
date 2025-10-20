#include "thread.hpp"

tpl<SI TBound1, SI TBound2> dfa NT ThdYield(SI spinCntCur)
{
#if defined(PROG_SYS_WIN)
    if (spinCntCur < TBound1)
        YieldProcessor();
    else if (spinCntCur < TBound2)
        NtYieldExecution_();
    else
    {
        LARGE_INTEGER_ tmp(0);
        NtDelayExecution_(NO, &tmp);
    }
#elif defined(PROG_SYS_ESP32)
    if (spinCntCur < TBound1)
        NoOpe();
    else if (spinCntCur < TBound2)
        taskYIELD();
    else
        vTaskDelay(1);
#else
    unimp;
#endif
}
