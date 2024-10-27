#pragma once

thdlocal Pos2<F4> g_curPosOfs;

dfa ER CurPosGet(Pos2<F4>& pos)
{
    pos = Pos2<F4>(0, 0);
    POINT tmp = {};
    ifu (GetCursorPos(&tmp) == 0)
        rete(ERR_CUR);
    pos = Pos2<F4>(F4(tmp.x) + g_curPosOfs.x, F4(tmp.y) + g_curPosOfs.y);
    rets;
}
dfa ER CurPosSet(cx Pos2<F4>& pos)
{
    cx S4 x = RoundF4ToS4(pos.x);
    cx S4 y = RoundF4ToS4(pos.y);
    ifu (SetCursorPos(x, y) == 0)
        rete(ERR_CUR);
    g_curPosOfs = Pos2<F4>(pos.x - F4(x), pos.y - F4(y));
    rets;
}
dfa ER CurPosMove(cx Pos2<F4>& pos)
{
    Pos2<F4> tmp;
    ife (CurPosGet(tmp))
        retep;
    tmp = Pos2<F4>(tmp.x + pos.x, tmp.y + pos.y);
    ife (CurPosSet(tmp))
        retep;
    rets;
}

dfa ER _MousKeyPress(InputKey key, BO isDown)
{
    AU codeVk = InputKeyToCodeVk(key);
    INPUT ip = {};
    ip.type = INPUT_MOUSE;
    switch (codeVk)
    {
    case VK_LBUTTON:
        ip.mi.dwFlags = isDown ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
        break;
    case VK_RBUTTON:
        ip.mi.dwFlags = isDown ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
        break;
    case VK_MBUTTON:
        ip.mi.dwFlags = isDown ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
        break;
    default:
        rete(ERR_MOUS);
    }
    ip.mi.dwExtraInfo = GetMessageExtraInfo();
    ifu (SendInput(1, &ip, siz(ip)) != 1)
        rete(ERR_MOUS);
    rets;
}
dfa ER MousKeyPressDown(InputKey key)
{
    ret _MousKeyPress(key, YES);
}
dfa ER MousKeyPressUp(InputKey key)
{
    ret _MousKeyPress(key, NO);
}
dfa ER MousKeyPress(InputKey key, TmMain hold = 50, TmMain delay = 40)
{
    ife (MousKeyPressDown(key))
        retep;
    ThdWait(hold);
    ife (MousKeyPressUp(key))
        retep;
    ThdWait(delay);
    rets;
}
