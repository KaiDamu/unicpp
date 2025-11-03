#include "window.hpp"

#ifdef PROG_SYS_WIN

    #include "process.hpp"
    #include "screen.hpp"

dfa Win::Cache::Cache()
{
    innerSize = Size2<SI>(-1, -1);
}

dfa HD Win::Hdl() cx
{
    ret HD(m_hdl);
}
dfa HD Win::Dev() cx
{
    if (m_ownDat != NUL)
        ret HD(m_ownDat->dev);
    ret HD(GetDC(m_hdl));
}
dfa cx Win::OwnDat* Win::OwnDatPtr() cx
{
    ret m_ownDat;
}
dfa BO Win::IsOwned() cx
{
    ret m_ownDat != NUL;
}
dfa ER Win::Sel(SelType selType, CXGA param1, CXGA param2)
{
    ifu (tx->IsOwned())
        rete(ErrVal::YES_INIT);

    switch (selType)
    {
    case SelType::NONE: {
        m_hdl = NUL;
        m_isDelReq = NO;
        rets;
    }
    case SelType::FOCUS: {
        m_hdl = GetForegroundWindow();
        ifu (m_hdl == NUL)
            rete(ErrVal::WIN);
        m_isDelReq = NO;
        rets;
    }
    case SelType::TITLE_CLASS: {
        cx AU titleStr = (cx CH*)param1;
        cx AU classStr = (cx CH*)param2;
        m_hdl = FindWindowW(classStr, titleStr);
        ifu (m_hdl == NUL)
            rete(ErrVal::WIN);
        m_isDelReq = NO;
        rets;
    }
    case SelType::CON: {
        m_hdl = GetConsoleWindow();
        ifu (m_hdl == NUL)
            rete(ErrVal::WIN);
        m_isDelReq = NO;
        rets;
    }
    case SelType::DESKTOP: {
        m_hdl = GetDesktopWindow();
        ifu (m_hdl == NUL)
            rete(ErrVal::WIN);
        m_isDelReq = NO;
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
    if (m_ownDat != NUL)
        ret m_ownDat->classStr;

    cx AU strLen = SI(GetClassNameW(m_hdl, NUL, 0));
    if (strLen == 0)
        ret std::wstring(L"");
    cx AU strLenx = strLen + STR_EX_LEN;
    std::vector<CH> str(strLenx);
    ifu (GetClassNameW(m_hdl, str.data(), int(strLenx)) == 0)
        ret std::wstring(L"");
    ret std::wstring(str.data());
}
dfa ER Win::OuterRectGet(Rect2<SI>& rect)
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
dfa ER Win::OuterRectSet(cx Rect2<SI>& rect)
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
dfa ER Win::InnerSizeGet(Size2<SI>& size, BO getCache)
{
    if (getCache)
    {
        size = m_cache.innerSize;
        rets;
    }
    ife (ProcDpiAwareSet())
        retep;
    RECT rect_;
    ifu (GetClientRect(m_hdl, &rect_) == 0)
        rete(ErrVal::WIN);
    size.w = SI(rect_.right) - SI(rect_.left);
    size.h = SI(rect_.bottom) - SI(rect_.top);
    m_cache.innerSize = size;
    rets;
}
dfa NT Win::InnerSizeCache(cx Size2<SI>& size)
{
    m_cache.innerSize = size;
}
dfa ER Win::InnerRectGet(Rect2<SI>& rect)
{
    ife (tx->InnerSizeGet(rect.size))
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
        ife (tx->OuterRectGet(rectOuter))
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
dfa ER Win::InnerRectSet(cx Rect2<SI>& rect)
{
    Rect2<SI> rectOuterCur;
    Rect2<SI> rectInnerCur;
    ifep(tx->OuterRectGet(rectOuterCur));
    ifep(tx->InnerRectGet(rectInnerCur));

    Rect2<SI> rectOuterDst;
    rectOuterDst = rect;
    rectOuterDst.pos -= rectInnerCur.pos - rectOuterCur.pos;
    rectOuterDst.size += rectOuterCur.size - rectInnerCur.size;
    ifep(tx->OuterRectSet(rectOuterDst));

    rets;
}
dfa ER Win::InnerUpd() cx
{
    if (m_ownDat != NUL && m_ownDat->displaySys == DisplaySys::GL)
        ifu (SwapBuffers(m_ownDat->dev) == FALSE)
            rete(ErrVal::WIN);

    rets;
}
dfa BO Win::IsValid() cx
{
    ret (m_hdl != NUL) && (IsWindow(m_hdl) != 0);
}
dfa BO Win::IsFocused() cx
{
    ret m_hdl == GetForegroundWindow();
}
dfa BO Win::IsDelReq() cx
{
    ret m_isDelReq;
}
dfa ER Win::Focus()
{
    ifu (SetForegroundWindow(m_hdl) == 0)
        rete(ErrVal::WIN);
    rets;
}
dfa NT Win::DelReq(BO isSet)
{
    m_isDelReq = isSet;
}
dfa ER Win::New(cx Size2<SI>& innerSize, cx CH* titleStr, BO doShow, BO canResize, DisplaySys displaySys)
{
    ifu (tx->IsOwned())
        rete(ErrVal::YES_INIT);

    cx AU procMdlAdr = HINSTANCE(ProcMdlCurAdrGet());

    m_ownDat = new OwnDat();
    CH classStrTmp[32];
    IntToStr(classStrTmp, ~UA(m_ownDat) ^ UA(0xA5A5));
    m_ownDat->classStr = classStrTmp;
    m_ownDat->displaySys = displaySys;
    m_ownDat->innerViSize = Size2<F4>(F4(innerSize.w), F4(innerSize.h));

    WNDCLASSEXW info = {};
    info.cbSize = siz(info);
    info.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    info.lpfnWndProc = _WinMsgProc;
    info.cbClsExtra = 0;
    info.cbWndExtra = 0;
    info.hInstance = procMdlAdr;
    info.hIcon = NUL;
    info.hCursor = LoadCursorW(NUL, IDC_ARROW);
    info.hbrBackground = NUL;
    info.lpszMenuName = NUL;
    info.lpszClassName = m_ownDat->classStr.c_str();
    info.hIconSm = NUL;
    cx AU winClass = RegisterClassExW(&info);
    ifu (winClass == 0)
        rete(ErrVal::WIN);

    if (titleStr == NUL)
        titleStr = L"";

    DWORD styleEx = WS_EX_ACCEPTFILES | WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD style = WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_CAPTION | WS_OVERLAPPED;
    if (canResize)
        style |= WS_SIZEBOX;
    cx AU win = CreateWindowExW(styleEx, LPCWSTR(UA(U2(winClass))), titleStr, style, 64, 64, 256, 256, NUL, NUL, procMdlAdr, NUL);
    ifu (win == NUL)
        rete(ErrVal::WIN);
    m_hdl = win;
    m_ownDat->dev = GetDC(win);

    Rect2<SI> scnRect;
    ifep(ScnRectGet(scnRect));
    cx AU innerRect = Rect2Center(Rect2<SI>(Pos2<SI>(0, 0), innerSize), scnRect);

    ite (i, i < 2) // workaround: the size is sometimes not set correctly on the first try after creation (thanks, Windows!)
        ifep(tx->InnerRectSet(innerRect));

    g_winClassMap[win] = tx;

    if (displaySys == DisplaySys::GL)
    {
        cx AU winHdl = HWND(m_hdl);
        cx AU winDc = GetDC(winHdl);

        PIXELFORMATDESCRIPTOR pfd = {};
        pfd.nSize = siz(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;

        cx AU pxFormId = ChoosePixelFormat(winDc, &pfd);
        ifu (pxFormId == 0)
            rete(ErrVal::WIN);

        ifu (SetPixelFormat(winDc, pxFormId, &pfd) == FALSE)
            rete(ErrVal::WIN);
    }

    if (doShow)
        ShowWindow(win, SW_SHOW);
    UpdateWindow(win);

    m_isDelReq = NO;

    rets;
}
dfa ER Win::Del()
{
    if (m_hdl == NUL)
        rets;

    ifu (DestroyWindow(m_hdl) == 0)
        rete(ErrVal::WIN);

    g_winClassMap.erase(m_hdl);

    m_hdl = NUL;

    m_isDelReq = NO;

    if (!tx->IsOwned())
        rets;

    ifep(m_ownDat->glCtx.Del());

    cx AU procMdlAdr = HINSTANCE(ProcMdlCurAdrGet());
    ifu (UnregisterClassW(m_ownDat->classStr.c_str(), procMdlAdr) == 0)
        rete(ErrVal::WIN);

    delete m_ownDat;
    m_ownDat = NUL;

    rets;
}

dfa Win::Win(SelType selType, CXGA param1, CXGA param2) : m_hdl(NUL), m_ownDat(NUL), m_isDelReq(NO)
{
    tx->Sel(selType, param1, param2);
}
dfa Win::~Win()
{
    if (tx->IsOwned())
        tx->Del(); // error ignored
}

dfa LRESULT CALLBACK _WinMsgProc(HWND win, UINT msg, WPARAM paramW, LPARAM paramL)
{
    // printf("msg = 0x%04X\n", msg);

    ifu (NO)
    {
        jdst(defaProc);
        ret DefWindowProcW(win, msg, paramW, paramL);
    }

    cx AU it = g_winClassMap.find(win);
    ifu (it == g_winClassMap.end())
        jsrc(defaProc);
    AU& winClass = *it->second;

    if (msg == WM_SYSCOMMAND)
    {
        if ((paramW & 0xFFF0) == SC_CLOSE)
        {
            WinMsgDelReq msgDat;
            msgDat.isSet = YES;
            ife (g_winMsgProc(winClass, msgDat))
                jsrc(defaProc);
            if (!msgDat.doDefaProc)
                ret 0;
        }
    }
    else if (msg == WM_SIZE)
    {
        WinMsgInnerSize msgDat;
        msgDat.size.w = SI(LOWORD(paramL));
        msgDat.size.h = SI(HIWORD(paramL));
        ife (g_winMsgProc(winClass, msgDat))
            jsrc(defaProc);
        if (!msgDat.doDefaProc)
            ret 0;
    }

    jsrc(defaProc);
}
dfa ER _WinMsgProcDefa(Win& win, cx WinMsgAny& msgAny)
{
    switch (msgAny.Type())
    {
    case WinMsgType::DEL_REQ: {
        AU& msg = (cx WinMsgDelReq&)msgAny;
        win.DelReq(msg.isSet);
        msg.doDefaProc = NO;
        break;
    }
    default:
        break;
    }

    rets;
}

dfa NT WinMsgProcSet(cx WinMsgProcFnT& fn)
{
    g_winMsgProc = fn;
}
dfa ER WinThdMsgGet(TmMain waitMax)
{
    cx AU result = MsgWaitForMultipleObjectsEx(0, NUL, DWORD(waitMax), QS_ALLINPUT, MWMO_INPUTAVAILABLE);
    ifu (result == WAIT_FAILED)
        rete(ErrVal::WIN);

    if (result == WAIT_TIMEOUT)
        rets;

    MSG msg = {};
    while (PeekMessageW(&msg, NUL, 0, 0, PM_REMOVE) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    rets;
}

WinMsgProcFnT g_winMsgProc = _WinMsgProcDefa;
std::unordered_map<HWND, Win*> g_winClassMap;

#endif
