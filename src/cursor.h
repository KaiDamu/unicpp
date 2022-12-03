#pragma once

thdlocal Pos2<F4> g_curPosOfs;

dfa ER CurPosGet(Pos2<F4>& pos) {
	pos.Set(0.0f, 0.0f);
	POINT tmp = {};
	ifu (GetCursorPos(&tmp) == 0) rete(ERR_CUR);
	pos.Set(F4(tmp.x) + g_curPosOfs.x, F4(tmp.y) + g_curPosOfs.y);
	rets;
}
dfa ER CurPosSet(cx Pos2<F4>& pos) {
	cx S4 x = RoundF4ToS4(pos.x);
	cx S4 y = RoundF4ToS4(pos.y);
	ifu (SetCursorPos(x, y) == 0) rete(ERR_CUR);
	g_curPosOfs.Set(pos.x - F4(x), pos.y - F4(y));
	rets;
}
dfa ER CurPosMove(cx Pos2<F4>& pos) {
	Pos2<F4> tmp;
	ife (CurPosGet(tmp)) retepass;
	tmp.Set(tmp.x + pos.x, tmp.y + pos.y);
	ife (CurPosSet(tmp)) retepass;
	rets;
}
