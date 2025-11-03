#include "mouse.hpp"

#ifdef PROG_SYS_WIN

    #include "process.hpp"
    #include "thread.hpp"

thdlocal Pos2<F4> g_curPosOfs;

dfa ER SysCurPosGet(Pos2<F4>& pos)
{
    ifep(ProcDpiAwareSet());
    pos = Pos2<F4>(0, 0);
    POINT tmp = {};
    ifu (GetCursorPos(&tmp) == 0)
        rete(ErrVal::CUR);
    pos = Pos2<F4>(F4(tmp.x) + g_curPosOfs.x, F4(tmp.y) + g_curPosOfs.y);
    rets;
}
dfa ER SysCurPosSet(cx Pos2<F4>& pos)
{
    ifep(ProcDpiAwareSet());
    cx AU x = S4(RoundToInt(pos.x));
    cx AU y = S4(RoundToInt(pos.y));
    ifu (SetCursorPos(x, y) == 0)
        rete(ErrVal::CUR);
    g_curPosOfs = Pos2<F4>(pos.x - F4(x), pos.y - F4(y));
    rets;
}
dfa ER SysCurPosMove(cx Pos2<F4>& pos)
{
    Pos2<F4> tmp;
    ifep(SysCurPosGet(tmp));
    tmp = Pos2<F4>(tmp.x + pos.x, tmp.y + pos.y);
    ifep(SysCurPosSet(tmp));
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
        rete(ErrVal::MOUS);
    }
    ip.mi.dwExtraInfo = GetMessageExtraInfo();
    ifu (SendInput(1, &ip, siz(ip)) != 1)
        rete(ErrVal::MOUS);
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
dfa ER MousKeyPress(InputKey key, TmMain hold, TmMain delay)
{
    ifep(MousKeyPressDown(key));
    ThdWait(hold);
    ifep(MousKeyPressUp(key));
    ThdWait(delay);
    rets;
}

#endif
