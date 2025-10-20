#pragma once

#include "type.hpp"

enum class ErrVal : U4
{
    NONE = 0,
    //
    SYS,
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
    GAMP,
    NET,
    SCN,
    //
    MEM_NEW,
    TIME_RES,
    DIR_NO_ENUM,
    NET_CLOSE,
    NET_VER,
    NET_NO_SUPPORT,
    NET_NO_EXIST,
    NET_YES_INIT,
    NET_HIGH_CLI,
    NET_MSG_NO_VALID,
    NET_MSG_NO_PRIVI,
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

extern thdlocal ErrVal g_errLastVal;

dfa NT ErrLastSet(ErrVal val);
dfa ErrVal ErrLastGet();

#include "error.ipp"
