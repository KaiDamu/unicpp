#pragma once

#include "type.hpp"
#ifdef PROG_SYS_WIN
    #include "window.hpp"
#endif

namespace gl
{

#ifdef PROG_SYS_WIN

dfa ER CtxUseWin(cx Win& win);

#endif

} // namespace gl

#include "glex.ipp"
