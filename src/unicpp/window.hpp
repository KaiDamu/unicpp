#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include <string>
    #include <functional>
    #include "macro-off.hpp"
    #include <windows.h>
    #include "macro-on.hpp"
    #include "container.hpp"
    #include "glmain.hpp"
#endif

#ifdef PROG_SYS_WIN

using WinMsgTypeT = U2;
enum class WinMsgType : WinMsgTypeT
{
    NONE = 0,
    DEL_REQ,
    INNER_SIZE,
    CNT,
};

struct WinMsgAny
{
    mutable BO doDefaProc = YES; // whether to call system default window procedure after processing the message

    dfa virtual WinMsgType Type() cx = 0;

    dfa virtual ~WinMsgAny() = default;
};
tpl<WinMsgType TType> struct _WinMsgAnyT : WinMsgAny
{
    dfa WinMsgType Type() cx override final;
};

struct WinMsgDelReq : _WinMsgAnyT<WinMsgType::DEL_REQ>
{
    BO isSet;
};
struct WinMsgInnerSize : _WinMsgAnyT<WinMsgType::INNER_SIZE>
{
    Size2<SI> size;
};

class Win
{
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

    using DisplaySysT = U1;
    enum class DisplaySys : DisplaySysT
    {
        SYS = 0,
        GL = 1,
    };

  public:
    struct OwnDat
    {
        HDC dev;
        std::wstring classStr;
        DisplaySys displaySys;
        gl::Ctx glCtx;
    };

  private:
    HWND m_hdl;
    OwnDat* m_ownDat;
    BO m_isDelReq;

  public:
    dfa HD Hdl() cx;
    dfa HD Dev() cx;
    dfa cx OwnDat* OwnDatPtr() cx;
    dfa BO IsOwned() cx;
    dfa ER Sel(SelType selType, CXGA param1 = NUL, CXGA param2 = NUL);
    dfa std::wstring TitleStrGet();
    dfa ER TitleStrSet(cx CH* str);
    dfa std::wstring ClassStrGet();
    dfa ER OuterRectGet(Rect2<SI>& rect);
    dfa ER OuterRectSet(cx Rect2<SI>& rect);
    dfa ER InnerSizeGet(Size2<SI>& size);
    dfa ER InnerRectGet(Rect2<SI>& rect);
    dfa ER InnerRectSet(cx Rect2<SI>& rect);
    dfa ER InnerUpd() cx;
    dfa BO IsValid() cx;
    dfa BO IsFocused() cx;
    dfa BO IsDelReq() cx;
    dfa ER Focus();
    dfa NT DelReq(BO isSet = YES);
    dfa ER New(cx Size2<SI>& innerSize, cx CH* titleStr = L"", BO doShow = YES, BO canResize = YES, DisplaySys displaySys = DisplaySys::GL);
    dfa ER Del();

  public:
    dfa Win(SelType selType = SelType::NONE, CXGA param1 = NUL, CXGA param2 = NUL);
    dfa ~Win();
};

using WinMsgProcFnT = std::function<ER(Win& win, cx WinMsgAny& msgAny)>;

dfa LRESULT CALLBACK _WinMsgProc(HWND win, UINT msg, WPARAM paramW, LPARAM paramL);
dfa ER _WinMsgProcDefa(Win& win, cx WinMsgAny& msgAny);

dfa NT WinMsgProcSet(cx WinMsgProcFnT& fn);
dfa ER WinThdMsgGet(TmMain waitMax = 0);

extern WinMsgProcFnT g_winMsgProc;
extern std::unordered_map<HWND, Win*> g_winClassMap;

#endif

#include "window.ipp"
