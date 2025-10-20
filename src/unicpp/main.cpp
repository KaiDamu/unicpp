#include "main.hpp"

#include "console.hpp"
#include "time.hpp"
#include "process.hpp"

dfa ER MainInit(MainInitCtx& ctx)
{
#ifndef PROG_SYS_WIN
    g_argCnt = ctx.argc;
    g_argValStd = ctx.argv;
#endif
#ifdef PROG_SYS_WIN
    ife (UniNtLoad())
        retep;
#endif
#ifdef PROG_SYS_WIN
    ProcPrioSet(PROCESS_PRIORITY_CLASS_HIGH, YES); // hint for performance, error not handled
    TimeResSet(0.1, NO);                           // hint for performance, error not handled
#endif
    ife (_TimeMainInit())
        retep;
    rets;
}
dfa ER MainFree()
{
#ifdef PROG_SYS_WIN
    TimeResClr(); // hint for performance, error not handled
#endif
    rets;
}

#ifndef UCPP_MAIN_TYPE_NONE

dfa ER _Main(MainInitCtx& ctx)
{
    ife (MainInit(ctx))
        retep;
    ife (Main())
        retep;
    ife (MainFree())
        retep;
    rets;
}

    #if defined(UCPP_MAIN_TYPE_BASE)
extern "C" void _start()
    #elif defined(UCPP_MAIN_TYPE_STD)
        #if defined(PROG_SYS_WIN)
int main() // define UCPP_MAIN_TYPE_NONE if you're using your own main function
        #elif defined(PROG_SYS_ESP32)
extern "C" void app_main() // define UCPP_MAIN_TYPE_NONE if you're using your own main function
        #else
int main(int argc, char** argv) // define UCPP_MAIN_TYPE_NONE if you're using your own main function
        #endif
    #else
inline int _main_unused()
    #endif
{
    ifdbg (YES)
    {
        ConWriteInfo("Start of program");
    }
    ErrVal errVal = ErrVal::NONE;
    MainInitCtx ctx = {};
    #if !defined(PROG_SYS_WIN)
        #if defined(PROG_SYS_ESP32)
    ctx.argc = 0;
    ctx.argv = NUL;
        #else
    ctx.argc = argc;
    ctx.argv = argv;
        #endif
    #endif
    ife (_Main(ctx))
        errVal = ErrLastGet();
    ifdbg (YES)
    {
        cx AU t = TimeMain();
        ifu (errVal != ErrVal::NONE)
            ConWriteErr("Main error code : %u", errVal);
        ConWriteInfo("Main execution time : %.3f ms", t);
        ConWriteInfo("End of program");
        ConWait();
    }
    _ConFree(); // error ignored
    ProcExit(U4(errVal));
    #if !defined(UCPP_MAIN_TYPE_BASE) && !defined(PROG_SYS_ESP32)
    ret int(errVal);
    #endif
}

#endif
