#pragma once

enum class ErrVal : U4
{
    NONE = 0,
    //
    MEM,
    TIME,
    PROC,
    THD,
    EVT,
    DRV,
    PATH,
    FILE,
    DIR,
    PIPE,
    CLIPB,
    ENV,
    TOKEN,
    CUR,
    HOOK,
    LOCK,
    WIN,
    CON,
    REG,
    KEYB,
    MOUS,
    NET,
    SCN,
    //
    MEM_NEW,
    TIME_RES,
    //
    NO_SUPPORT,
    NO_VALID,
    NO_EXIST,
    NO_INIT,
    NO_FULL,
    //
    YES_EXIST,
    YES_INIT,
    YES_ACTIVE,
    //
    LOW_SIZE,
    LOW_WAIT,
    //
    HIGH_SIZE,
    HIGH_WAIT,
    //
    NA,
};

thdlocal ErrVal g_errLastVal = ErrVal::NONE;

dfa NT ErrLastSet(ErrVal val)
{
    g_errLastVal = val;
}
dfa ErrVal ErrLastGet()
{
    ret g_errLastVal;
}
