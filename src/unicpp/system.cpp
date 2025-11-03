#include "system.hpp"

#include "thread.hpp"
#include "token.hpp"

#ifdef PROG_SYS_WIN

dfa SysVer SysVerGet()
{
    static SysVer s_cache = 0;
    ifu (s_cache == 0)
    {
        cx AU teb = ThdTeb();
        ifl (teb != NUL)
        {
            cx AU peb = teb->ProcessEnvironmentBlock;
            cx AU v1 = U8(U2(peb->OSMajorVersion)) << (sizb(U2) * 3);
            cx AU v2 = U8(U2(peb->OSMinorVersion)) << (sizb(U2) * 2);
            cx AU v3 = U8(U2(peb->OSBuildNumber)) << (sizb(U2) * 1);
            s_cache = (v1 | v2 | v3);
        }
    }
    ret s_cache;
}
dfa ER SysShutdown()
{
    ProcSecTokenPriviEnable(SecTokenPriviId::Shutdown_);
    NtShutdownSystem_(SHUTDOWN_ACTION_::ShutdownPowerOff);
    rete(ErrVal::SYS);
}
dfa ER SysBsod()
{
    ProcSecTokenPriviEnable(SecTokenPriviId::Shutdown_);
    HARDERROR_RESPONSE_ response;
    NtRaiseHardError_(STATUS_SYSTEM_SHUTDOWN, 0, 0, NUL, HARDERROR_RESPONSE_OPTION_::OptionShutdownSystem, &response);
    NtShutdownSystem_(SHUTDOWN_ACTION_::ShutdownReboot);
    rete(ErrVal::SYS);
}

#endif
