#pragma once

#include "type.hpp"
#include "unint.hpp"

using SysVer = U8;

#ifdef PROG_SYS_WIN

dfa SysVer SysVerGet();
dfa ER SysShutdown();
dfa ER SysBsod();

#endif

#include "system.ipp"
