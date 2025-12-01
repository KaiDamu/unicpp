#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include <string>
    #include "macro-off.hpp"
    #include <windows.h>
    #include "macro-on.hpp"
    #include "color.hpp"
#endif

#ifdef PROG_SYS_WIN

dfa ER ClipbSetText(cx CH* text);
tpl1 dfa ER ClipbSetColGrid(cx ColGrid<T1>& colGrid);
dfa ER ClipbGetText(std::wstring& text);
tpl1 dfa ER ClipbGetColGrid(ColGrid<T1>& colGrid);

#endif

#include "clipboard.ipp"
