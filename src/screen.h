#pragma once

dfa HMONITOR _ScnMainHdlGet()
{
    cx HMONITOR hdl = MonitorFromWindow(NUL, MONITOR_DEFAULTTOPRIMARY);
    ret hdl;
}
dfa ER ScnSizeGet(Size2<SI>& size)
{
    size = Size2<SI>(0, 0);
    ife (ProcDpiAwareSet())
        retep;
    cx HMONITOR scn = _ScnMainHdlGet();
    ifu (scn == NUL)
        rete(ERR_SCN);
    MONITORINFO info = {};
    info.cbSize = siz(info);
    ifu (GetMonitorInfoW(scn, &info) == 0)
        rete(ERR_SCN);
    size.w = info.rcMonitor.right - info.rcMonitor.left;
    size.h = info.rcMonitor.bottom - info.rcMonitor.top;
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
