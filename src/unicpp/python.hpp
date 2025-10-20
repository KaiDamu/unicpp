#pragma once

#include "type.hpp"

#ifdef PROG_SYS_WIN

dfa ER PythonExePathGet(CH* path);
dfa ER PythonStart(cx CH* file, cx CH* args, BO wait);

#endif

#include "python.ipp"
