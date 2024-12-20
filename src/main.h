#pragma once

dfa ER MainInit()
{
#ifdef PROG_SYS_WIN
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS); // hint for performance, error not handled
#endif
    TimeResSet(1, 0, NO); // hint for performance, error not handled
    ife (_TimeMainInit())
        rete(ErrVal::TIME);
    rets;
}
dfa ER MainFree()
{
    TimeResClr(); // hint for performance, error not handled
    rets;
}

#ifndef UCPP_MAIN_NO

dfa ER Main();

dfa ER _Main()
{
    ife (MainInit())
        retep;
    ife (Main())
        retep;
    ife (MainFree())
        retep;
    rets;
}

int main() // define UCPP_MAIN_NO if you're using your own main function
{
    ifdbg (YES)
    {
        ConWriteInfo("Start of program");
    }
    ErrVal errVal = ErrVal::NONE;
    ife (_Main())
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
    ret int(errVal);
}

#endif
