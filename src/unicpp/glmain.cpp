#include "glmain.hpp"

#include "glload.hpp"
#include "error.hpp"
#include "value.hpp"

namespace gl
{

#ifdef PROG_SYS_WIN

    #ifdef PROG_COMPILER_MSVC
        #pragma comment(lib, "opengl32.lib")
    #endif

dfa HD Ctx::Hdl() cx
{
    ret HD(m_hdl);
}
dfa HD Ctx::Dev() cx
{
    ret HD(m_dev);
}
dfa ER Ctx::New(HD dev, SI verMajor, SI verMinor)
{
    ifu (m_hdl != NUL)
        rete(ErrVal::YES_INIT);

    if (verMajor == 1 && verMinor == 0)
    {
        m_hdl = wglCreateContext(HDC(dev));
        ifu (m_hdl == NUL)
            rete(ErrVal::GL);
        m_dev = HDC(dev);
    }
    else if (verMajor == 4 && verMinor == 6)
    {
        ifu (wglCreateContextAttribsARB == NUL)
            rete(ErrVal::NO_INIT);
        cx int attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,
                            int(verMajor),
                            WGL_CONTEXT_MINOR_VERSION_ARB,
                            int(verMinor),
                            WGL_CONTEXT_FLAGS_ARB,
                            WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                            WGL_CONTEXT_PROFILE_MASK_ARB,
                            WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                            0};
        m_hdl = wglCreateContextAttribsARB(HDC(dev), NUL, attribs);
        ifu (m_hdl == NUL)
            rete(ErrVal::GL);
        m_dev = HDC(dev);
    }
    else
    {
        rete(ErrVal::NO_SUPPORT);
    }

    rets;
}
dfa ER Ctx::Del()
{
    if (m_hdl == NUL)
        rets;
    if (g_ctx != NUL && g_ctx->Hdl() == m_hdl)
        ifep(CtxUseClr());
    ifu (wglDeleteContext(m_hdl) == FALSE)
        rete(ErrVal::GL);
    m_hdl = NUL;
    m_dev = NUL;
    rets;
}
dfa ER Ctx::Use()
{
    if ((g_ctx != NUL && g_ctx->Hdl() == m_hdl) || (g_ctx == NUL && m_hdl == NUL))
        rets;
    ifu (wglMakeCurrent(m_dev, m_hdl) == FALSE)
        rete(ErrVal::GL);
    g_ctx = ((m_hdl == NUL) ? NUL : tx);
    rets;
}
dfa Ctx::Ctx() : m_hdl(NUL), m_dev(NUL)
{
}
dfa Ctx::~Ctx()
{
    tx->Del(); // error ignored
}

thdlocal Ctx* g_ctx = NUL;

dfa ER CtxUseClr()
{
    Ctx ctxNull;
    ifep(ctxNull.Use());
    rets;
}

#endif

} // namespace gl
