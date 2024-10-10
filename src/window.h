#pragma once

dfa HD WinCurHdlGet()
{
    ret HD(GetForegroundWindow());
}
dfa SI WinCurTitleStrGet(CH* str, SI strLenMax)
{
    cx SI strLen = GetWindowTextW(HWND(WinCurHdlGet()), str, int(strLenMax));
    ret strLen;
}
dfa ER WinCurTitleStrSet(cx CH* str)
{
    ifu (SetWindowTextW(HWND(WinCurHdlGet()), str) == 0)
        rete(ERR_WIN);
    rets;
}
dfa SI WinCurClassStrGet(CH* str, SI strLenMax)
{
    cx SI strLen = GetClassNameW(HWND(WinCurHdlGet()), str, int(strLenMax));
    ret strLen;
}
dfa BO WinIsValid(HD hdl)
{
    ret BO(IsWindow(HWND(hdl)) != 0);
}
dfa HD WinHdlGetByTitleStr(cx CH* str)
{
    ret HD(FindWindowExW(NUL, NUL, NUL, str));
}
dfa HD WinHdlGetByClassStr(cx CH* str)
{
    ret HD(FindWindowExW(NUL, NUL, str, NUL));
}
dfa HD WinHdlGetByTitleStrClassStr(cx CH* titleStr, cx CH* classStr)
{
    ret HD(FindWindowExW(NUL, NUL, classStr, titleStr));
}
dfa ER WinFocusSet(HD hdl)
{
    ifu (SetForegroundWindow(HWND(hdl)) == 0)
        rete(ERR_WIN);
    rets;
}
