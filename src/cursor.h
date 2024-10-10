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
