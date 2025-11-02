#include "glex.hpp"

#ifdef PROG_SYS_WIN
    #include "glload.hpp"
#endif

namespace gl
{

#ifdef PROG_SYS_WIN

dfa ER CtxUseWin(cx Win& win)
{
    cx AU winOwnDat = win.OwnDatPtr();
    ifu (winOwnDat == NUL)
        rete(ErrVal::NO_INIT);

    if (winOwnDat->displaySys == Win::DisplaySys::GL && winOwnDat->glCtx.Hdl() == NUL)
    {
        ifep(gl::Load());
        ifep(const_cast<Win::OwnDat*>(winOwnDat)->glCtx.New(win.Dev(), 4, 6));
    }
    ifu (winOwnDat->glCtx.Hdl() == NUL)
        rete(ErrVal::NO_INIT);
    ifep(const_cast<Win::OwnDat*>(winOwnDat)->glCtx.Use());

    rets;
}

#endif

} // namespace gl
