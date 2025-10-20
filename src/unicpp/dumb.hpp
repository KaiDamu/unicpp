#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "type.hpp"
#endif

#ifdef PROG_SYS_WIN

dfa NT DumbSysBreakThdSpam();
dfa NT DumbSysBreakMemFill();
dfa NT DumbClipbBreakLock();
dfa NT DumbDeskBreakVoid();
dfa NT DumbSysStopBlue();
dfa NT DumbSysCurBreak();

// TODO:
// DumbScnBreakSize
// DumbFileLockSelf
// DumbDirSpam
// DumbWinSpam
// DumbSndBreak
// DumbNetBreak
// DumbBluetoothBreak
// DumbPersonalBreak

#endif

#include "dumb.ipp"
