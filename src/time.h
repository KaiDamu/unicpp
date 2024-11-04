#pragma once

using TmUnix = U8; // time in unix format
using TmLdap = U8; // time in ldap format
using TmCpu = U8;  // time in cpu cycles
using TmMain = F8; // time in milliseconds, since the start of the program

enum class TimeUnit : U1
{
    NONE = 0,
    USEC,
    MSEC,
    SEC,
    MINUTE,
    HOUR,
    DAY,
    WEEK,
    MONTH,
    YEAR,
};

U8 g_timeMainOfs = 0;
F8 g_timeMainDiv = 0.0;

dfa TmUnix LdapToUnix(TmLdap ldap)
{
    ifu (ldap < 116444736000000000)
        ret 0;
    ret ldap / 10000000 - 11644473600;
}
dfa TmLdap UnixToLdap(TmUnix unix)
{
    ret unix * 10000000 + 116444736000000000;
}

dfa TmCpu CpuTsc()
{
    union {
        U8 val;
        U4 part[2];
    };
#ifdef PROG_COMPILER_GCC
    __asm__(".byte 0x0f, 0x31" : "=a"(part[0]), "=d"(part[1]));
#else
    val = __rdtsc();
#endif
    ret val;
}

dfa ER TimeResClr()
{
    ULONG resNew;
    ifu (NtSetTimerResolution(0, FALSE, &resNew) != STATUS_SUCCESS)
    {
        rete(ErrVal::TIME_RES);
    }
    rets;
}
dfa ER TimeResSet(S4 ms, S4 us, BO force)
{
    cx ULONG resReq = ULONG((ms * 1000 + us) * 10);
    ULONG resNew;
    ifu (NtSetTimerResolution(resReq, TRUE, &resNew) != STATUS_SUCCESS)
    {
        rete(ErrVal::TIME_RES);
    }
    ifu (force && (resReq != resNew))
    {
        TimeResClr();
        rete(ErrVal::TIME_RES);
    }
    rets;
}

dfa ER _TimeMainInit()
{
    LARGE_INTEGER val;
    ifu (QueryPerformanceFrequency(&val) == 0)
    {
        rete(ErrVal::TIME);
    }
    cx F8 timeMainDivF = S8ToF8(val.QuadPart) / 1000.0;
    ifu (QueryPerformanceCounter(&val) == 0)
    {
        rete(ErrVal::TIME);
    }
    cx U8 timeMainOfs = val.QuadPart;
    g_timeMainOfs = timeMainOfs;
    g_timeMainDiv = timeMainDivF;
    rets;
}

dfa TmMain TimeMain()
{
    LARGE_INTEGER val;
    QueryPerformanceCounter(&val);
    ret U8ToF8(val.QuadPart - g_timeMainOfs) / g_timeMainDiv;
}

dfa TmUnix TimeUnix()
{
    ret time(NUL);
}

dfa TmUnix TmUnixBuild(U4 year = 0, U4 month = 0, U4 day = 0, U4 hour = 0, U4 minute = 0, U4 second = 0)
{
    tm tm;
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    tm.tm_isdst = -1;
    ret TmUnix(mktime(&tm));
}

class Timer
{
  private:
    TmMain m_time;
    TmMain m_timeStart;
    BO m_isRunning;

  private:
    dfa TmMain TimeGet()
    {
        ret TimeMain();
    }

  public:
    dfa TmMain Read()
    {
        if (m_isRunning)
        {
            cx AU timeNow = tx->TimeGet();
            m_time += timeNow - m_timeStart;
            m_timeStart = timeNow;
        }
        ret m_time;
    }
    dfa NT Start()
    {
        ifu (m_isRunning)
            ret;
        m_isRunning = YES;
        m_timeStart = tx->TimeGet();
    }
    dfa TmMain Pause()
    {
        ifu (!m_isRunning)
            ret m_time;
        m_isRunning = NO;
        m_time += tx->TimeGet() - m_timeStart;
        ret m_time;
    }
    dfa TmMain Restart()
    {
        cx AU timeNow = tx->TimeGet();
        cx AU timeRet = m_isRunning ? (m_time + timeNow - m_timeStart) : m_time;
        m_isRunning = YES;
        m_time = 0.0;
        m_timeStart = timeNow;
        ret timeRet;
    }

  public:
    dfa Timer() : m_time(), m_timeStart(), m_isRunning()
    {
        tx->Restart();
    }
    dfa ~Timer()
    {
    }
};
