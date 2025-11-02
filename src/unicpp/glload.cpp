#include "glload.hpp"

#ifdef PROG_SYS_WIN
    #include "process.hpp"
    #include "window.hpp"
    #include "glmain.hpp"
#endif

namespace gl
{

GAFN g_loadFn[LOAD_FN_CNT] = {};
BO g_isLoaded = NO;

} // namespace gl

namespace gl
{

#ifdef PROG_SYS_WIN

dfa ER Load()
{
    if (g_isLoaded)
        rets;

    Win winDummy;
    ifep(winDummy.New(Size2<SI>(256, 256), NUL, NO, NO, Win::DisplaySys::GL));
    cx AU winDev = winDummy.Dev();

    cx AU glDll = ProcDllAdrGet(L"opengl32.dll", YES);
    ifu (glDll == NUL)
        rete(ErrVal::PROC);

    cx AU loadFnList = [&](SI cnt) -> ER {
        ite (i, i < cnt)
        {
            if (wglGetProcAddress != NUL)
                g_loadFn[i] = wglGetProcAddress(g_loadFnName[i]);
            if (g_loadFn[i] == NUL)
                g_loadFn[i] = ProcFnAdrGet(glDll, g_loadFnName[i]);
            ifu (g_loadFn[i] == NUL)
                rete(ErrVal::NO_EXIST);
        }
        rets;
    };

    gl::Ctx glCtxLegacy;
    ifep(glCtxLegacy.New(winDev, 1, 0));
    ifep(glCtxLegacy.Use());
    ifep(loadFnList(PRE_LOAD_FN_CNT));

    gl::Ctx glCtxModern;
    ifep(glCtxModern.New(winDev, 4, 6));
    ifep(glCtxModern.Use());
    ifep(loadFnList(LOAD_FN_CNT));

    ifep(glCtxModern.Del());
    ifep(glCtxLegacy.Del());
    ifep(winDummy.Del());

    g_isLoaded = YES;
    rets;
}

#endif

} // namespace gl
