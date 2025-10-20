#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "thread.hpp"
#endif

#include "color.hpp"
#include "container.hpp"

enum class GrShape : U1
{
    NONE = 0,
    PT = 1,
    LINE = 2,
    RECT = 3,
    TRIANGLE = 4,
    CIRCLE = 5,
};

extern cx ColRgba COL_GRID_FILLIN_WALL;

tpl1 dfa BO Line2Clip(Line2<T1>& dst, cx Line2<T1>& src, cx Rect2<T1>& keep);

tpl1 dfa BO Line2IsDiscard(cx Line2<T1>& line, cx Rect2<T1>& keep);

tpl1 dfa NT _ColGridFillin(ColGrid<T1>& grid, cx Pos2<SI>& pt, cx T1& col);

tpl1 dfa NT ColGridDrawClr(ColGrid<T1>& grid, cx T1& col);

tpl1 dfa NT ColGridDrawPt(ColGrid<T1>& grid, cx Pos2<SI>& pt, cx T1& col);

tpl1 dfa NT _ColGridDrawLine1(ColGrid<T1>& grid, cx Line2<SI>& lineClipped, cx T1& col);

tpl1 dfa NT _ColGridDrawLineN(ColGrid<T1>& grid, cx Line2<SI>& line, SI thickness, cx T1& col, BO doClamp, BO doFill);

tpl1 dfa NT ColGridDrawLine(ColGrid<T1>& grid, cx Line2<SI>& line, SI thickness, cx T1& col, BO doFill);

tpl1 dfa NT _ColGridDrawRect(ColGrid<T1>& grid, cx Line2<SI>& lineCross, cx Line2<SI>& lineClipped, cx T1& col, BO doClamp, BO doFill);

tpl1 dfa NT ColGridDrawRect(ColGrid<T1>& grid, cx Rect2<SI>& rect, cx T1& col, BO doFill);

tpl1 dfa NT _ColGridDrawTriangle(ColGrid<T1>& grid, cx Triangle2<SI>& triangle, cx T1& col, BO doClamp, BO doFill);

tpl1 dfa NT ColGridDrawTriangle(ColGrid<T1>& grid, cx Triangle2<SI>& triangle, cx T1& col, BO doFill);

tpl1 dfa NT _ColGridDrawCircle(ColGrid<T1>& grid, cx Circle2<SI>& circle, cx T1& col, BO doClamp, BO doFill);

tpl1 dfa NT ColGridDrawCircle(ColGrid<T1>& grid, cx Circle2<SI>& circle, cx T1& col, BO doFill);

tpl1 dfa NT ColGridDrawUcppLogo(ColGrid<T1>& grid, cx Rect2<SI>& rect);

tpl1 dfa NT ColGridDrawColGrid(ColGrid<T1>& grid, cx ColGrid<T1>& gridSrc, cx Pos2<SI>& pos);

/// /// /// [ Screen Draw section ] /// /// ///
#ifdef PROG_SYS_WIN

cxex U1 SCN_DRAW_THD_CODE_ERR_NO = 0;
cxex U1 SCN_DRAW_THD_CODE_ERR_YES = 1;
cxex U1 SCN_DRAW_THD_CODE_WAIT = 2;

extern Thd g_scnDrawThd;
extern S4 g_scnDrawThdDelay;
extern HWND g_scnDrawWinHdl;

class ScnDrawCtx
{
  private:
    ColGrid<ColRgba> m_scnGrid;
    HDC m_hdcScreen;
    GA m_pBits;
    HBITMAP m_hBitmap;
    HDC m_hdcMem;
    SI m_fpsMax;
    TmMain m_fpsTimer;

  public:
    dfa NT FpsMax(SI fpsMax);

  public:
    dfa NT DrawClr(cx ColRgba& col);
    dfa NT DrawPt(cx Pos2<SI>& pt, cx ColRgba& col);
    dfa NT DrawLine(cx Line2<SI>& line, SI thickness, cx ColRgba& col, BO doFill);
    dfa NT DrawRect(cx Rect2<SI>& rect, cx ColRgba& col, BO doFill);
    dfa NT DrawTriangle(cx Triangle2<SI>& triangle, cx ColRgba& col, BO doFill);
    dfa NT DrawCircle(cx Circle2<SI>& circle, cx ColRgba& col, BO doFill);
    dfa NT DrawUcppLogo(cx Rect2<SI>& rect);
    dfa NT DrawColGrid(cx ColGrid<ColRgba>& gridSrc, cx Pos2<SI>& pos);

  public:
    dfa ER MainBegin();
    dfa ER MainEnd();
    dfa ER Init();
    dfa ER Free();
};

dfa LRESULT CALLBACK _ScnDrawWinProc(HWND win, UINT code, WPARAM wp, LPARAM lp);

dfa S4 _ScnDrawThd(GA code);

dfa ER ScnDrawInit();
dfa ER ScnDrawFree();

#endif

#include "graphics.ipp"
