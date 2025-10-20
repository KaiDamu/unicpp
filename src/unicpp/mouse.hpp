#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "vector.hpp"
    #include "input.hpp"
#endif

#ifdef PROG_SYS_WIN

extern thdlocal Pos2<F4> g_curPosOfs;

dfa ER SysCurPosGet(Pos2<F4>& pos);
dfa ER SysCurPosSet(cx Pos2<F4>& pos);
dfa ER SysCurPosMove(cx Pos2<F4>& pos);

dfa ER _MousKeyPress(InputKey key, BO isDown);

dfa ER MousKeyPressDown(InputKey key);
dfa ER MousKeyPressUp(InputKey key);
dfa ER MousKeyPress(InputKey key, TmMain hold = INPUT_KEY_HOLD_DEFA, TmMain delay = INPUT_KEY_DELAY_DEFA);

#endif

#include "mouse.ipp"
