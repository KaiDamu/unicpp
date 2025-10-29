#pragma once

#include "preprocess.hpp"
#include <time.h>
#ifdef PROG_COMPILER_MSVC
    #include <intrin.h>
#endif
#ifdef PROG_SYS_WIN
    #include "macro-off.hpp"
    #include <windows.h>
    #include "macro-on.hpp"
#endif
#include "typeconvert.hpp"

using TmUnix = U8;  // time in unix format - seconds since 1970-01-01 00:00:00 UTC
using TmLdap = U8;  // time in ldap format - 100-nanoseconds since 1601-01-01 00:00:00 UTC
using TmCpu = U8;   // time in cpu cycles - number of cpu cycles since cpu start/boot
using TmCpuMs = U8; // time in milliseconds, normalized from cpu cycles
using TmMain = F8;  // time in milliseconds, since the start of the program

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

extern U8 g_timeMainSub;
extern F8 g_timeMainMul;
extern TmCpu g_cpuTscPerMs;
extern TmCpu g_cpuTscMsMul;

dfa TmUnix LdapToUnix(TmLdap ldap);
dfa TmLdap UnixToLdap(TmUnix tmUnix);

dfa TmUnix TimeUnix();

dfa TmUnix TmUnixBuild(U4 year = 0, U4 month = 0, U4 day = 0, U4 hour = 0, U4 minute = 0, U4 second = 0);

dfa TmCpu CpuTsc();

dfa ER _TimeMainInit();

dfa TmMain TimeMain();

dfa NT _CpuTscMsInit();

dfa TmCpuMs CpuTscMs();

class Timer
{
  private:
    TmMain m_time;
    TmMain m_timeStart;
    BO m_isRunning;

  private:
    dfa TmMain TimeGet();

  public:
    dfa TmMain Read();
    dfa NT Start();
    dfa TmMain Pause();
    dfa TmMain Restart();

  public:
    dfa Timer();
    dfa ~Timer();
};

#ifdef PROG_SYS_WIN

dfa ER TimeResClr();
dfa ER TimeResSet(TmMain ms, BO doForce);

class TimeHackChecker
{
  private:
    TmCpuMs m_diffMaxProc;
    TmCpuMs m_diffMaxSys;
    TmCpuMs m_diffValidProc;
    TmCpuMs m_diffValidSys;

  public:
    dfa BO IsValidProc() cx;
    dfa BO IsValidSys() cx;
    dfa BO IsValid() cx;
    dfa NT InitProc(TmCpuMs diffMax);
    dfa NT InitSys(TmCpuMs diffMax);
    dfa TimeHackChecker();
};

#endif

#include "time.ipp"
