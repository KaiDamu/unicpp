#include "graphics.hpp"

cx ColRgba COL_GRID_FILLIN_WALL = ColRgba(0xFE, 0x02, 0xFC, 0x04);

#ifdef PROG_SYS_WIN

    #include "memory.hpp"
    #include "screen.hpp"

Thd g_scnDrawThd;
S4 g_scnDrawThdDelay = 0;
HWND g_scnDrawWinHdl = NUL;

dfa NT ScnDrawCtx::FpsMax(SI fpsMax)
{
    m_fpsMax = fpsMax;
    m_fpsTimer = TimeMain();
}

dfa NT ScnDrawCtx::DrawClr(cx ColRgba& col)
{
    ColGridDrawClr(m_scnGrid, col);
}
dfa NT ScnDrawCtx::DrawPt(cx Pos2<SI>& pt, cx ColRgba& col)
{
    ColGridDrawPt(m_scnGrid, pt, col);
}
dfa NT ScnDrawCtx::DrawLine(cx Line2<SI>& line, SI thickness, cx ColRgba& col, BO doFill)
{
    ColGridDrawLine(m_scnGrid, line, thickness, col, doFill);
}
dfa NT ScnDrawCtx::DrawRect(cx Rect2<SI>& rect, cx ColRgba& col, BO doFill)
{
    ColGridDrawRect(m_scnGrid, rect, col, doFill);
}
dfa NT ScnDrawCtx::DrawTriangle(cx Triangle2<SI>& triangle, cx ColRgba& col, BO doFill)
{
    ColGridDrawTriangle(m_scnGrid, triangle, col, doFill);
}
dfa NT ScnDrawCtx::DrawCircle(cx Circle2<SI>& circle, cx ColRgba& col, BO doFill)
{
    ColGridDrawCircle(m_scnGrid, circle, col, doFill);
}
dfa NT ScnDrawCtx::DrawUcppLogo(cx Rect2<SI>& rect)
{
    ColGridDrawUcppLogo(m_scnGrid, rect);
}
dfa NT ScnDrawCtx::DrawColGrid(cx ColGrid<ColRgba>& gridSrc, cx Pos2<SI>& pos)
{
    ColGridDrawColGrid(m_scnGrid, gridSrc, pos);
}

dfa ER ScnDrawCtx::MainBegin()
{
    MemSet(m_scnGrid.pixels.data(), 0, m_scnGrid.pixels.size() * siz(m_scnGrid.pixels[0]));
    rets;
}
dfa ER ScnDrawCtx::MainEnd()
{
    HGDIOBJ oldBitmap = SelectObject(m_hdcMem, m_hBitmap);

    cx ColRgba* srcPixels = m_scnGrid.pixels.data();
    RGBQUAD* dstPixels = static_cast<RGBQUAD*>(m_pBits);

    cx AU pixelCnt = SI(m_scnGrid.pixels.size());
    ite (i, i < pixelCnt)
    {
        dstPixels[i].rgbRed = srcPixels[i].r;
        dstPixels[i].rgbGreen = srcPixels[i].g;
        dstPixels[i].rgbBlue = srcPixels[i].b;
        dstPixels[i].rgbReserved = srcPixels[i].a;
    }

    POINT ptPos = {0, 0};
    SIZE sizeWnd = {LONG(m_scnGrid.size.w), LONG(m_scnGrid.size.h)};
    POINT ptSrc = {0, 0};
    BLENDFUNCTION blend = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

    UpdateLayeredWindow(g_scnDrawWinHdl, m_hdcScreen, &ptPos, &sizeWnd, m_hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);

    SelectObject(m_hdcMem, oldBitmap);

    g_scnDrawThdDelay = 0;

    if (m_fpsMax > 0)
    {
        cx AU timeElapsedMs = TimeMain() - m_fpsTimer;
        cx AU timeSleepMs = TmMain(1000) / TmMain(m_fpsMax) - timeElapsedMs;
        ThdWait(timeSleepMs);
        m_fpsTimer = TimeMain();
    }

    rets;
}
dfa ER ScnDrawCtx::Init()
{
    RECT winRect;
    ifu (GetClientRect(g_scnDrawWinHdl, &winRect) == 0)
        rete(ErrVal::WIN);
    m_scnGrid.size = Size2<SI>(winRect.right - winRect.left, winRect.bottom - winRect.top);
    m_scnGrid.pixels.resize(m_scnGrid.size.Area());

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = siz(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = LONG(m_scnGrid.size.w);
    bmi.bmiHeader.biHeight = LONG(-m_scnGrid.size.h);
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    m_hdcScreen = GetDC(NUL);
    m_pBits = NUL;
    m_hBitmap = CreateDIBSection(m_hdcScreen, &bmi, DIB_RGB_COLORS, &m_pBits, NUL, 0);

    ifu (!m_hBitmap || !m_pBits)
    {
        ReleaseDC(NUL, m_hdcScreen);
        m_scnGrid.pixels.resize(0);
        rete(ErrVal::WIN);
    }

    m_hdcMem = CreateCompatibleDC(m_hdcScreen);

    m_fpsMax = 0;

    rets;
}
dfa ER ScnDrawCtx::Free()
{
    DeleteObject(m_hBitmap);
    DeleteDC(m_hdcMem);
    ReleaseDC(NUL, m_hdcScreen);
    m_scnGrid.pixels.resize(0);

    rets;
}

dfa LRESULT CALLBACK _ScnDrawWinProc(HWND win, UINT code, WPARAM wp, LPARAM lp)
{
    switch (code)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        ret DefWindowProcW(win, code, wp, lp);
    }
    ret 0;
}

dfa S4 _ScnDrawThd(GA code)
{
    AU& codeInt = *reinterpret_cast<U1*>(code);
    Size2<SI> scnSize;
    ife (ScnSizeGet(scnSize))
    {
        codeInt = SCN_DRAW_THD_CODE_ERR_YES;
        ret 1;
    }
    HINSTANCE instance = GetModuleHandleW(NUL);
    WNDCLASSEXW winClass = {};
    winClass.cbSize = siz(winClass);
    winClass.lpfnWndProc = _ScnDrawWinProc;
    winClass.hInstance = instance;
    winClass.hCursor = LoadCursorA(NUL, MAKEINTRESOURCEA(32512));
    winClass.lpszClassName = L"_gr_scn_thd_win";
    cx AU winClassHdl = RegisterClassExW(&winClass);
    ifu (winClassHdl == 0)
    {
        codeInt = SCN_DRAW_THD_CODE_ERR_YES;
        ret 1;
    }
    g_scnDrawWinHdl = CreateWindowExW(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST, (LPCWSTR)(uintptr_t)winClassHdl, L"", WS_POPUP, 0, 0, int(scnSize.w), int(scnSize.h), NUL,
                                      NUL, instance, NUL);
    ifu (g_scnDrawWinHdl == NUL)
    {
        UnregisterClassW((LPCWSTR)(uintptr_t)winClassHdl, instance);
        codeInt = SCN_DRAW_THD_CODE_ERR_YES;
        ret 1;
    }
    ShowWindow(g_scnDrawWinHdl, SW_SHOW);                           // unchecked
    UpdateWindow(g_scnDrawWinHdl);                                  // unchecked
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST); // hint for performance, error not handled
    codeInt = SCN_DRAW_THD_CODE_ERR_NO;
    g_scnDrawThdDelay = 0;
    MSG msg;
    while (YES)
    {
        if (PeekMessageW(&msg, NUL, 0, 0, PM_REMOVE) == 0)
        {
            ++g_scnDrawThdDelay;
            if (g_scnDrawThdDelay <= 0)
                continue;
            cx AU delay = Clamp<TmMain>(TmMain(g_scnDrawThdDelay) / 100 - 75, 1, 100); // maybe this should be configurable
            ThdWait(delay);
        }
        else
        {
            ifu (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    DestroyWindow(g_scnDrawWinHdl); // unchecked
    UnregisterClassW((LPCWSTR)(uintptr_t)winClassHdl, instance); // unchecked
    ret 0;
}

dfa ER ScnDrawInit()
{
    volatile AU code = SCN_DRAW_THD_CODE_WAIT;
    ife (g_scnDrawThd.New(_ScnDrawThd, const_cast<U1*>(&code)))
        retep;
    while (code == SCN_DRAW_THD_CODE_WAIT)
        NtYieldExecution_();
    ifu (code == SCN_DRAW_THD_CODE_ERR_YES)
        rete(ErrVal::SCN);
    rets;
}
dfa ER ScnDrawFree()
{
    ifu (PostThreadMessageW(DWORD(UA(g_scnDrawThd.Id())), WM_QUIT, 0, 0) == 0)
        rete(ErrVal::THD);
    ife (g_scnDrawThd.Wait())
        retep;
    ife (g_scnDrawThd.Close())
        retep;
    rets;
}

#endif
