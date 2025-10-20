#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "macro-off.hpp"
    #include <windows.h>
    #include "macro-on.hpp"
#endif
#include "type.hpp"

dfa NT AtomSetU4(volatile U4& dst, U4 set);
dfa BO AtomCmpSetU4(volatile U4& dst, U4 cmp, U4 set);

#include "atomic.ipp"
