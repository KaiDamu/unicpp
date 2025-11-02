#pragma once

#include "type.hpp"
#ifdef PROG_SYS_WIN
    #include "macro-off.hpp"
    #include <windows.h>
    #include "macro-on.hpp"
#endif

namespace gl
{

#ifdef PROG_SYS_WIN

class Ctx
{
  private:
    HGLRC m_hdl;
    HDC m_dev;

  public:
    dfa HD Hdl() cx;
    dfa HD Dev() cx;
    dfa ER New(HD dev, SI verMajor = 4, SI verMinor = 6);
    dfa ER Del();
    dfa ER Use();

  public:
    dfa Ctx();
    dfa ~Ctx();
};

extern thdlocal Ctx* g_ctx;

dfa ER CtxUseClr();

#endif

} // namespace gl

#include "glmain.ipp"
