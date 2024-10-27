#pragma once

dfa HD WinHdlGet()
{
    ret HD(GetForegroundWindow());
}
dfa SI WinTitleStrGet(CH* str, SI strLenMax)
{
    cx SI strLen = GetWindowTextW(HWND(WinHdlGet()), str, int(strLenMax));
    ret strLen;
}
dfa ER WinTitleStrSet(cx CH* str)
{
    ifu (SetWindowTextW(HWND(WinHdlGet()), str) == 0)
        rete(ERR_WIN);
    rets;
}
dfa SI WinClassStrGet(CH* str, SI strLenMax)
{
    cx SI strLen = GetClassNameW(HWND(WinHdlGet()), str, int(strLenMax));
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
