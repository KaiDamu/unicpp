#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include <string>
    #include "macro-off.hpp"
    #include <windows.h>
    #include "macro-on.hpp"
    #include "container.hpp"
#endif

#ifdef PROG_SYS_WIN

class Win
{
  private:
    HWND m_hdl;

  public:
    using SelTypeT = U1;
    enum class SelType : SelTypeT
    {
        NONE = 0,
        FOCUS = 1,
        TITLE_CLASS = 2,
        CON = 3,
        DESKTOP = 4,
    };

  public:
    dfa HD Hdl() cx;
    dfa ER Sel(SelType selType, CXGA param1 = NUL, CXGA param2 = NUL);
    dfa std::wstring TitleStrGet();
    dfa ER TitleStrSet(cx CH* str);
    dfa std::wstring ClassStrGet();
    dfa ER RectOuterGet(Rect2<SI>& rect);
    dfa ER RectOuterSet(cx Rect2<SI>& rect);
    dfa ER RectInnerSizeGet(Size2<SI>& size);
    dfa ER RectInnerGet(Rect2<SI>& rect);
    dfa ER RectInnerSet(cx Rect2<SI>& rect);
    dfa BO IsValid();
    dfa BO IsFocused();
    dfa ER Focus();

  public:
    dfa Win(SelType selType = SelType::NONE, CXGA param1 = NUL, CXGA param2 = NUL);
    dfa ~Win();
};

#endif

#include "window.ipp"
