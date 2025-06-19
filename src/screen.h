#pragma once

#ifdef PROG_SYS_WIN

dfa HMONITOR _ScnMainHdlGet()
{
    cx HMONITOR hdl = MonitorFromWindow(NUL, MONITOR_DEFAULTTOPRIMARY);
    ret hdl;
}
dfa ER ScnRectGet(Rect2<SI>& rect)
{
    rect = Rect2<SI>(0, 0, 0, 0);
    ife (ProcDpiAwareSet())
        retep;
    cx HMONITOR scn = _ScnMainHdlGet();
    ifu (scn == NUL)
        rete(ErrVal::SCN);
    MONITORINFO info = {};
    info.cbSize = siz(info);
    ifu (GetMonitorInfoW(scn, &info) == 0)
        rete(ErrVal::SCN);
    rect.pos.x = SI(info.rcMonitor.left);
    rect.pos.y = SI(info.rcMonitor.top);
    rect.size.w = SI(info.rcMonitor.right) - SI(info.rcMonitor.left);
    rect.size.h = SI(info.rcMonitor.bottom) - SI(info.rcMonitor.top);
    rets;
}
dfa ER ScnSizeList(std::vector<Size2<SI>>& sizes)
{
    sizes.clear();
    DEVMODEW devMode = {};
    devMode.dmSize = siz(devMode);
    SetLastError(0);
    ite (i, i < 1000) // 1000 is an arbitrary limit to prevent infinite loop in rare cases
    {
        if (!EnumDisplaySettingsW(NUL, i, &devMode))
        {
            ifu (GetLastError() != 0)
                rete(ErrVal::SCN);
            rets;
        }
        cx Size2<SI> size(devMode.dmPelsWidth, devMode.dmPelsHeight);
        BO isDupe = NO;
        for (cx AU& existingSize : sizes)
        {
            if (existingSize == size)
            {
                isDupe = YES;
                break;
            }
        }
        if (!isDupe)
            sizes.push_back(size);
    }
    rete(ErrVal::HIGH_SIZE);
}
dfa ER ScnSizeGet(Size2<SI>& size)
{
    Rect2<SI> rect;
    ife (ScnRectGet(rect))
        retep;
    size = rect.size;
    rets;
}
dfa ER ScnSizeSet(cx Size2<SI>& size, BO isPersistent = NO)
{
    DEVMODEW devMode = {};
    devMode.dmSize = siz(devMode);
    ifu (!EnumDisplaySettingsW(NUL, ENUM_CURRENT_SETTINGS, &devMode))
        rete(ErrVal::SCN);
    devMode.dmPelsWidth = DWORD(size.w);
    devMode.dmPelsHeight = DWORD(size.h);
    devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
    cx DWORD flags = isPersistent ? CDS_UPDATEREGISTRY : CDS_FULLSCREEN;
    ifu (ChangeDisplaySettingsW(&devMode, flags) != DISP_CHANGE_SUCCESSFUL)
        rete(ErrVal::SCN);
    rets;
}
dfa ER ScnDiagonalGet(F4& size)
{
    Size2<SI> scnSize;
    ife (ScnSizeGet(scnSize))
        retep;
    cx Pos2<F4> scnCorner(F4(scnSize.w), F4(scnSize.h));
    size = Dist0(scnCorner);
    rets;
}
dfa F4 ScnDpiMulGet(BO doUpd = NO)
{
    static F4 dpiMul;
    static BO doUpd_ = YES;

    doUpd_ |= doUpd;
    ifu (doUpd_)
    {
        doUpd_ = NO;
        ife (ProcDpiAwareSet())
            ret F4(0);
        cx UINT dpi = GetDpiForSystem();
        dpiMul = F4(dpi) / F4(96);
    }

    ret dpiMul;
}
dfa ER ScnUpdForce()
{
    cx AU win = CreateWindowExW(WS_EX_TOOLWINDOW, L"STATIC", L"", WS_POPUP, -32000, -32000, 1, 1, NUL, NUL, GetModuleHandleW(NUL), NUL);
    ifu (win == NUL)
        rete(ErrVal::WIN);
    ShowWindow(win, SW_SHOWNOACTIVATE);
    UpdateWindow(win);
    ShowWindow(win, SW_HIDE);
    DestroyWindow(win);
    rets;
}

#endif
