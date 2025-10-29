#include "windowcapture.hpp"

#ifdef PROG_SYS_WIN

dfa WinCapture::Cache::Cache()
{
    gdi.isValid = NO;
    gdi.winHdl = NUL;
    gdi.curDC = NUL;
    gdi.memDC = NUL;
    gdi.bmp = NUL;
    gdi.pixels = NUL;
    gdi.size = Size2<SI>(0, 0);
    gdi.ofs = Pos2<SI>(0, 0);

    dxgi.isValid = NO;
    dxgi.dev.Reset();
    dxgi.devCtx.Reset();
    dxgi.outCpy.Reset();
    dxgi.tex.Reset();
    dxgi.size = Size2<SI>(0, 0);
}
dfa WinCapture::Cache::~Cache()
{
    ifu (gdi.isValid)
    {
        if (gdi.bmp != NUL)
            DeleteObject(gdi.bmp);
        if (gdi.memDC != NUL)
            DeleteDC(gdi.memDC);
        if (gdi.curDC != NUL)
            ReleaseDC(gdi.winHdl, gdi.curDC);
    }
    ifu (dxgi.isValid)
    {
        if (dxgi.tex != NUL)
            dxgi.tex.Reset();
        if (dxgi.outCpy != NUL)
            dxgi.outCpy.Reset();
        if (dxgi.devCtx != NUL)
            dxgi.devCtx.Reset();
        if (dxgi.dev != NUL)
            dxgi.dev.Reset();
    }
}

dfa Win* WinCapture::WinGet()
{
    ret m_win;
}
dfa cx Win* WinCapture::WinGet() cx
{
    ret m_win;
}
dfa NT WinCapture::WinSet(Win* win)
{
    m_win = win;
}

dfa WinCapture::WinCapture(Win* win)
{
    tx->WinSet(win);
}
dfa WinCapture::~WinCapture()
{
    tx->WinSet(NUL);
}

#endif
