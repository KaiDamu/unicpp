#pragma once

struct MainInitCtx
{
#ifndef PROG_SYS_WIN
    SI argc;
    CS** argv;
#endif
};

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

dfa ER Main();

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

    #ifdef UCPP_MAIN_TYPE_BASE
extern "C" void _start()
    #elif defined(UCPP_MAIN_TYPE_STD)
        #ifdef PROG_SYS_WIN
int main() // define UCPP_MAIN_TYPE_NONE if you're using your own main function
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
    #ifndef PROG_SYS_WIN
    ctx.argc = argc;
    ctx.argv = argv;
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
    ProcExit(U4(errVal));
    #ifndef UCPP_MAIN_TYPE_BASE
    ret int(errVal);
    #endif
}

#endif

#ifdef UCPP_MAIN_TYPE_DLL
    #error "UCPP_MAIN_TYPE_DLL is not yet supported!"
#endif
