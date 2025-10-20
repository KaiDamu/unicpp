#include "window.hpp"

#ifdef PROG_SYS_WIN

    #include "process.hpp"

dfa Win::Cache::Cache()
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
dfa Win::Cache::~Cache()
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

dfa HD Win::Hdl() cx
{
    ret HD(m_hdl);
}
dfa ER Win::Sel(SelType selType, CXGA param1, CXGA param2)
{
    switch (selType)
    {
    case SelType::NONE: {
        m_hdl = NUL;
        rets;
    }
    case SelType::FOCUS: {
        m_hdl = GetForegroundWindow();
        ifu (m_hdl == NUL)
            rete(ErrVal::WIN);
        rets;
    }
    case SelType::TITLE_CLASS: {
        cx AU titleStr = (cx CH*)param1;
        cx AU classStr = (cx CH*)param2;
        m_hdl = FindWindowW(classStr, titleStr);
        ifu (m_hdl == NUL)
            rete(ErrVal::WIN);
        rets;
    }
    case SelType::CON: {
        m_hdl = GetConsoleWindow();
        ifu (m_hdl == NUL)
            rete(ErrVal::WIN);
        rets;
    }
    case SelType::DESKTOP: {
        m_hdl = GetDesktopWindow();
        ifu (m_hdl == NUL)
            rete(ErrVal::WIN);
        rets;
    }
    default: {
        rete(ErrVal::NO_SUPPORT);
    }
    }
}
dfa std::wstring Win::TitleStrGet()
{
    cx AU strLen = SI(GetWindowTextLengthW(m_hdl));
    if (strLen == 0)
        ret std::wstring(L"");
    cx AU strLenx = strLen + STR_EX_LEN;
    std::vector<CH> str(strLenx);
    ifu (GetWindowTextW(m_hdl, str.data(), int(strLenx)) == 0)
        ret std::wstring(L"");
    ret std::wstring(str.data());
}
dfa ER Win::TitleStrSet(cx CH* str)
{
    ifu (SetWindowTextW(m_hdl, str) == 0)
        rete(ErrVal::WIN);
    rets;
}
dfa std::wstring Win::ClassStrGet()
{
    cx AU strLen = SI(GetClassNameW(m_hdl, NUL, 0));
    if (strLen == 0)
        ret std::wstring(L"");
    cx AU strLenx = strLen + STR_EX_LEN;
    std::vector<CH> str(strLenx);
    ifu (GetClassNameW(m_hdl, str.data(), int(strLenx)) == 0)
        ret std::wstring(L"");
    ret std::wstring(str.data());
}
dfa ER Win::RectOuterGet(Rect2<SI>& rect)
{
    ife (ProcDpiAwareSet())
        retep;
    RECT rect_;
    ifu (GetWindowRect(m_hdl, &rect_) == 0)
        rete(ErrVal::WIN);
    rect.pos.x = SI(rect_.left);
    rect.pos.y = SI(rect_.top);
    rect.size.w = SI(rect_.right) - SI(rect_.left);
    rect.size.h = SI(rect_.bottom) - SI(rect_.top);
    rets;
}
dfa ER Win::RectOuterSet(cx Rect2<SI>& rect)
{
    ife (ProcDpiAwareSet())
        retep;
    cx AU doSetPos = (rect.pos.x != -1) && (rect.pos.y != -1);
    cx AU doSetSize = (rect.size.w != -1) && (rect.size.h != -1);
    if (doSetPos)
    {
        if (doSetSize)
        {
            ifu (MoveWindow(m_hdl, int(rect.pos.x), int(rect.pos.y), int(rect.size.w), int(rect.size.h), TRUE) == 0)
                rete(ErrVal::WIN);
        }
        else
        {
            ifu (SetWindowPos(m_hdl, NUL, int(rect.pos.x), int(rect.pos.y), 0, 0, SWP_NOSIZE | SWP_NOZORDER) == 0)
                rete(ErrVal::WIN);
        }
    }
    else
    {
        if (doSetSize)
        {
            ifu (SetWindowPos(m_hdl, NUL, 0, 0, int(rect.size.w), int(rect.size.h), SWP_NOMOVE | SWP_NOZORDER) == 0)
                rete(ErrVal::WIN);
        }
        else
        {
            rets;
        }
    }
    rets;
}
dfa ER Win::RectInnerSizeGet(Size2<SI>& size)
{
    ife (ProcDpiAwareSet())
        retep;
    RECT rect_;
    ifu (GetClientRect(m_hdl, &rect_) == 0)
        rete(ErrVal::WIN);
    size.w = SI(rect_.right) - SI(rect_.left);
    size.h = SI(rect_.bottom) - SI(rect_.top);
    rets;
}
dfa ER Win::RectInnerGet(Rect2<SI>& rect)
{
    ife (tx->RectInnerSizeGet(rect.size))
        retep;

    // if (IsNearZero(FractionPart(ScnDpiMulGet())))
    if (YES)
    {
        POINT pt = {0, 0};
        ifu (ClientToScreen(m_hdl, &pt) == 0)
            rete(ErrVal::WIN);
        rect.pos.x = SI(pt.x);
        rect.pos.y = SI(pt.y);
    }
    else
    {
        // we have to do it manually to avoid rounding errors

        Rect2<SI> rectOuter;
        ife (tx->RectOuterGet(rectOuter))
            retep;

        RECT rectOfs = {0, 0, LONG(rect.size.w), LONG(rect.size.h)};
        ifu (AdjustWindowRectEx(&rectOfs, GetWindowLongW(m_hdl, GWL_STYLE), FALSE, GetWindowLongW(m_hdl, GWL_EXSTYLE)) == 0)
            rete(ErrVal::WIN);

        Pos2<SI> posBase;
        posBase.x = rectOuter.pos.x - SI(rectOfs.left);
        posBase.y = rectOuter.pos.y - SI(rectOfs.top);

        cx SI ofsEx = 0; // correct value varied between 0 and 6 while testing...

        rect.pos.x = posBase.x + ofsEx;
        rect.pos.y = posBase.y + ofsEx;
    }

    rets;
}
dfa ER Win::RectInnerSet(cx Rect2<SI>& rect)
{
    Rect2<SI> rectOuterCur;
    Rect2<SI> rectInnerCur;
    ifep(tx->RectOuterGet(rectOuterCur));
    ifep(tx->RectInnerGet(rectInnerCur));

    Rect2<SI> rectOuterDst;
    rectOuterDst = rect;
    rectOuterDst.pos -= rectInnerCur.pos - rectOuterCur.pos;
    rectOuterDst.size += rectOuterCur.size - rectInnerCur.size;
    ifep(tx->RectOuterSet(rectOuterDst));

    rets;
}
dfa BO Win::IsValid()
{
    ret (m_hdl != NUL) && (IsWindow(m_hdl) != 0);
}
dfa BO Win::IsFocused()
{
    ret m_hdl == GetForegroundWindow();
}
dfa ER Win::Focus()
{
    ifu (SetForegroundWindow(m_hdl) == 0)
        rete(ErrVal::WIN);
    rets;
}

dfa Win::Win(SelType selType, CXGA param1, CXGA param2) : m_hdl(NUL)
{
    tx->Sel(selType, param1, param2);
}
dfa Win::~Win()
{
}

#endif
