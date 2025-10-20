#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "macro-off.hpp"
    #include <windows.h>
    #include "macro-on.hpp"
    #include "container.hpp"
#endif

#ifdef PROG_SYS_WIN

dfa HMONITOR _ScnMainHdlGet();
dfa ER ScnRectGet(Rect2<SI>& rect);
dfa ER ScnSizeList(std::vector<Size2<SI>>& sizes);
dfa ER ScnSizeGet(Size2<SI>& size);
dfa ER ScnSizeSet(cx Size2<SI>& size, BO isPersistent = NO);
dfa ER ScnDiagonalGet(F4& size);
dfa F4 ScnDpiMulGet(BO doUpd = NO);
dfa ER ScnUpdForce();

#endif

#include "screen.ipp"
