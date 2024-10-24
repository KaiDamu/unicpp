#pragma once

const ColRgba COL_GRID_FILLIN_WALL = ColRgba(0xFE, 0x02, 0xFC, 0x04);

tpl1 dfa BO Line2Clip(Line2<T1>& dst, cx Line2<T1>& src, cx Rect2<T1>& keep)
{
    cx Size2<T1>& bounds = keep.size;
    dst = src;
    dst.a -= keep.pos;
    dst.b -= keep.pos;

    enum OutCode : U1
    {
        INSIDE = 0b0000,
        LEFT = 0b0001,
        RIGHT = 0b0010,
        BOTTOM = 0b0100,
        TOP = 0b1000,
    };

    cx AU outCodeGet = [&](cx Pos2<T1>& pt, cx Size2<T1>& bounds) -> U1 {
        U1 outCode = OutCode::INSIDE;

        if (pt.x < T1(0))
            outCode |= OutCode::LEFT;
        else if (pt.x >= bounds.w)
            outCode |= OutCode::RIGHT;

        if (pt.y < T1(0))
            outCode |= OutCode::BOTTOM;
        else if (pt.y >= bounds.h)
            outCode |= OutCode::TOP;

        ret outCode;
    };

    AU outCodeA = outCodeGet(dst.a, bounds);
    AU outCodeB = outCodeGet(dst.b, bounds);

    do
    {
        if (!(outCodeA | outCodeB))
        {
            dst.a += keep.pos;
            dst.b += keep.pos;
            ret NO; // should not discard
        }
        else if (outCodeA & outCodeB)
        {
            ret YES; // should discard
        }
        else
        {
            AU& outCodeSel = outCodeA ? outCodeA : outCodeB;
            Pos2<T1> pt;

            if (outCodeSel & OutCode::TOP)
            {
                cx AU hPrev = ValPrev(bounds.h);
                pt.x = dst.a.x + (dst.b.x - dst.a.x) * (hPrev - dst.a.y) / (dst.b.y - dst.a.y);
                pt.y = hPrev;
            }
            else if (outCodeSel & OutCode::BOTTOM)
            {
                pt.x = dst.a.x + (dst.b.x - dst.a.x) * (-dst.a.y) / (dst.b.y - dst.a.y);
                pt.y = T1(0);
            }
            else if (outCodeSel & OutCode::RIGHT)
            {
                cx AU wPrev = ValPrev(bounds.w);
                pt.x = wPrev;
                pt.y = dst.a.y + (dst.b.y - dst.a.y) * (wPrev - dst.a.x) / (dst.b.x - dst.a.x);
            }
            else if (outCodeSel & OutCode::LEFT)
            {
                pt.x = T1(0);
                pt.y = dst.a.y + (dst.b.y - dst.a.y) * (-dst.a.x) / (dst.b.x - dst.a.x);
            }

            if (outCodeSel == outCodeA)
            {
                dst.a = pt;
                outCodeA = outCodeGet(dst.a, bounds);
            }
            else
            {
                dst.b = pt;
                outCodeB = outCodeGet(dst.b, bounds);
            }
        }
    } while (YES);
}

tpl1 dfa BO Line2IsDiscard(cx Line2<T1>& line, cx Rect2<T1>& keep)
{
    enum OutCode : U1
    {
        INSIDE = 0b0000,
        LEFT = 0b0001,
        RIGHT = 0b0010,
        BOTTOM = 0b0100,
        TOP = 0b1000,
    };

    cx AU outCodeGet = [&](cx Pos2<T1>& pt, cx Rect2<T1>& keep) -> U1 {
        U1 outCode = OutCode::INSIDE;

        if (pt.x < keep.pos.x)
            outCode |= OutCode::LEFT;
        else if (pt.x >= keep.XMax())
            outCode |= OutCode::RIGHT;

        if (pt.y < keep.pos.y)
            outCode |= OutCode::BOTTOM;
        else if (pt.y >= keep.YMax())
            outCode |= OutCode::TOP;

        ret outCode;
    };

    ret outCodeGet(line.a, keep) & outCodeGet(line.b, keep);
}

tpl1 dfa NT _ColGridFillin(ColGrid<T1>& grid, cx Pos2<SI>& pt, cx T1& col)
{
    // check type size, then assume everything else is correct
    static_assert(siz(T1) == siz(U4), "_ColGridFillin: T1 size mismatch");

    // set up values
    U4* gridPixels = reinterpret_cast<U4*>(grid.pixels.data());
    cx U4* gridPixelsEnd = gridPixels + grid.pixels.size();
    U4* curPixel = gridPixels + (pt.y * grid.size.w + pt.x);
    cx U4 wall = AsType<U4>(COL_GRID_FILLIN_WALL);
    cx U4 colSet = AsType<U4>(col);

    while (*curPixel != wall)
    {
        ++curPixel;
    }
    U4* curPixelSaveL = curPixel;
    do
    {
        *curPixel = colSet;
        ++curPixel;
    } while (*curPixel == wall);
    U4* curPixelSaveR = curPixel - 1;

    do
    {
        curPixel = curPixelSaveL + grid.size.w - 1;
        if (curPixel >= gridPixelsEnd)
        {
            ret;
        }
        if (*curPixel == wall)
        {
            do
            {
                --curPixel;
            } while (*curPixel == wall);
            curPixelSaveL = curPixel + 1;
            // (first wall found in next row)
        }
        else
        {
            U4* curPixelEnd = curPixelSaveR + grid.size.w + 1;
            do
            {
                ++curPixel;
            } while ((curPixel != curPixelEnd) && (*curPixel != wall));
            if (*curPixel != wall)
            {
                ret;
            }
            curPixelSaveL = curPixel;
            // (first wall found in next row)
        }

        curPixel = curPixelSaveR + grid.size.w + 1;
        if (curPixel >= gridPixelsEnd)
        {
            ret;
        }
        if (*curPixel == wall)
        {
            do
            {
                ++curPixel;
            } while (*curPixel == wall);
            curPixelSaveR = curPixel - 1;
            // (last wall found in next row)
        }
        else
        {
            U4* curPixelEnd = curPixelSaveL;
            do
            {
                --curPixel;
            } while ((curPixel != curPixelEnd) && (*curPixel != wall));
            if (*curPixel != wall)
            {
                ret;
            }
            curPixelSaveR = curPixel;
            // (last wall found in next row)
        }

        curPixel = curPixelSaveL;
        U4* curPixelEnd = curPixelSaveR + 1;
        do
        {
            *curPixel = colSet;
            ++curPixel;
        } while (curPixel != curPixelEnd);
    } while (YES);

    ret;
}

tpl1 dfa NT ColGridDrawPt(ColGrid<T1>& grid, cx Pos2<SI>& pt, cx T1& col)
{
    ifu (pt.x < 0 || pt.x >= grid.size.w || pt.y < 0 || pt.y >= grid.size.h)
        ret;
    grid.Pixel(pt) = col;
}

tpl1 dfa NT _ColGridDrawLine1(ColGrid<T1>& grid, cx Line2<SI>& lineClipped, cx T1& col)
{
    SI x1 = lineClipped.a.x;
    SI y1 = lineClipped.a.y;
    cx SI x2 = lineClipped.b.x;
    cx SI y2 = lineClipped.b.y;
    cx SI dx = Diff(x1, x2);
    cx SI sx = ((x1 < x2) ? 1 : -1);
    cx SI dy = -Diff(y1, y2);
    cx SI sy = ((y1 < y2) ? 1 : -1);
    SI err = dx + dy;
    SI e2;

    do
    {
        grid.Pixel(Pos2<SI>(x1, y1)) = col;
        if (x1 == x2 && y1 == y2)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y1 += sy;
        }
    } while (YES);
}

tpl1 dfa NT _ColGridDrawLineN(ColGrid<T1>& grid, cx Line2<SI>& line, SI thickness, cx T1& col, BO doClamp, BO doFill)
{
    if (thickness == 1)
    {
        Line2<SI> lineClipped;
        if (Line2Clip(lineClipped, line, Rect2<SI>(Pos2<SI>(0, 0), grid.size)))
            ret;
        _ColGridDrawLine1(grid, lineClipped, col);
        ret;
    }

    T1 colUse = doFill ? COL_GRID_FILLIN_WALL : col;

    cx SI dx = line.b.x - line.a.x;
    cx SI dy = line.b.y - line.a.y;
    cx F4 len = Dist0<F4>(dx, dy);
    cx F4 ofsX = (dy * (ValPrev(thickness) / 2.0f)) / len * -1;
    cx F4 ofsY = (dx * (ValPrev(thickness) / 2.0f)) / len;

    constexpr SI ptCnt = 4;
    array<Pos2<SI>, ptCnt> pt;
    pt[0] = Pos2<SI>(line.b.x - ofsX, line.b.y - ofsY);
    pt[1] = Pos2<SI>(line.a.x - ofsX, line.a.y - ofsY);
    pt[2] = Pos2<SI>(line.a.x + ofsX, line.a.y + ofsY);
    pt[3] = Pos2<SI>(line.b.x + ofsX, line.b.y + ofsY);
    constexpr SI lineCnt = 4;
    array<Line2<SI>, lineCnt> lines;
    lines[0] = Line2<SI>(pt[0], pt[1]);
    lines[1] = Line2<SI>(pt[1], pt[2]);
    lines[2] = Line2<SI>(pt[2], pt[3]);
    lines[3] = Line2<SI>(pt[3], pt[0]);

    array<Line2<SI>, lineCnt> linesClipped;
    array<BO, lineCnt> isKeep;
    SI keepCnt = 0;
    ite (i, i < lineCnt)
    {
        keepCnt += (isKeep[i] = !Line2Clip(linesClipped[i], lines[i], Rect2<SI>(Pos2<SI>(0, 0), grid.size)));
        if (isKeep[i])
            _ColGridDrawLine1(grid, linesClipped[i], colUse);
    }

    if (doClamp)
    {
        if (keepCnt > lineCnt - 2)
        {
            cx AU drawLineEx = [&](SI i1, SI i2, SI i3, SI i4) -> NT {
                if (isKeep[i1] && isKeep[i2])
                {
                    if (linesClipped[i1].b != linesClipped[i2].a)
                        _ColGridDrawLine1(grid, Line2<SI>(linesClipped[i1].b, linesClipped[i2].a), colUse);
                }
                else if (isKeep[i1])
                {
                    _ColGridDrawLine1(grid, Line2<SI>(linesClipped[i1].b, linesClipped[i3].a), colUse);
                }
                else
                {
                    _ColGridDrawLine1(grid, Line2<SI>(linesClipped[i4].b, linesClipped[i2].a), colUse);
                }
            };

            drawLineEx(0, 1, 2, 3);
            drawLineEx(1, 2, 3, 0);
            drawLineEx(2, 3, 0, 1);
            drawLineEx(3, 0, 1, 2);
        }
        else
        {
            // TODO: implement
            // unimp;
            ret;
        }
    }

    if (doFill)
    {
        Pos2<SI> ptTopLeft = Pos2<SI>(Min<SI>(pt[0].x, pt[1].x, pt[2].x, pt[3].x), Min<SI>(pt[0].y, pt[1].y, pt[2].y, pt[3].y));
        cx AU keep = Rect2<SI>(Pos2<SI>(0, 0), grid.size);
        if (ptTopLeft.x < keep.pos.x)
            ptTopLeft.x = keep.pos.x;
        if (ptTopLeft.y < keep.pos.y)
            ptTopLeft.y = keep.pos.y;
        _ColGridFillin(grid, ptTopLeft, col);
    }
}

tpl1 dfa NT ColGridDrawLine(ColGrid<T1>& grid, cx Line2<SI>& line, SI thickness, cx T1& col, BO doFill)
{
    ifu (thickness < 1)
        ret;
    _ColGridDrawLineN(grid, line, thickness, col, doFill, doFill);
}

tpl1 dfa NT _ColGridDrawRect(ColGrid<T1>& grid, cx Line2<SI>& lineCross, cx Line2<SI>& lineClipped, cx T1& col, BO doClamp, BO doFill)
{
    T1 colUse = doFill ? COL_GRID_FILLIN_WALL : col;

    cx Line2<SI> line1(Pos2<SI>(lineClipped.a.x, lineClipped.a.y), Pos2<SI>(lineClipped.b.x, lineClipped.a.y)); // top
    cx Line2<SI> line2(Pos2<SI>(lineClipped.b.x, lineClipped.a.y), Pos2<SI>(lineClipped.b.x, lineClipped.b.y)); // right
    cx Line2<SI> line3(Pos2<SI>(lineClipped.b.x, lineClipped.b.y), Pos2<SI>(lineClipped.a.x, lineClipped.b.y)); // bottom
    cx Line2<SI> line4(Pos2<SI>(lineClipped.a.x, lineClipped.b.y), Pos2<SI>(lineClipped.a.x, lineClipped.a.y)); // left

    if (doClamp)
    {
        _ColGridDrawLine1(grid, line1, colUse);
        _ColGridDrawLine1(grid, line2, colUse);
        _ColGridDrawLine1(grid, line3, colUse);
        _ColGridDrawLine1(grid, line4, colUse);
    }
    else
    {
        cx AU clipXMax = ValPrev(grid.size.w);
        cx AU clipYMax = ValPrev(grid.size.h);
        if (!(lineClipped.a.y == 0 && lineClipped.a.y != lineCross.a.y))
            _ColGridDrawLine1(grid, line1, colUse);
        if (!(lineClipped.b.x == clipXMax && lineClipped.b.x != lineCross.b.x))
            _ColGridDrawLine1(grid, line2, colUse);
        if (!(lineClipped.b.y == clipYMax && lineClipped.b.y != lineCross.b.y))
            _ColGridDrawLine1(grid, line3, colUse);
        if (!(lineClipped.a.x == 0 && lineClipped.a.x != lineCross.a.x))
            _ColGridDrawLine1(grid, line4, colUse);
    }

    if (doFill)
    {
        Pos2<SI> ptTopLeft = Pos2<SI>(Min<SI>(lineClipped.a.x, lineClipped.b.x), Min<SI>(lineClipped.a.y, lineClipped.b.y));
        cx AU keep = Rect2<SI>(Pos2<SI>(0, 0), grid.size);
        if (ptTopLeft.x < keep.pos.x)
            ptTopLeft.x = keep.pos.x;
        if (ptTopLeft.y < keep.pos.y)
            ptTopLeft.y = keep.pos.y;
        _ColGridFillin(grid, ptTopLeft, col);
    }
}

tpl1 dfa NT ColGridDrawRect(ColGrid<T1>& grid, cx Rect2<SI>& rect, cx T1& col, BO doFill)
{
    Line2<SI> lineCross(Pos2<SI>(rect.pos.x, rect.pos.y), Pos2<SI>(ValPrev(rect.XMax()), ValPrev(rect.YMax())));
    Line2<SI> lineClipped;
    if (Line2Clip(lineClipped, lineCross, Rect2<SI>(Pos2<SI>(0, 0), grid.size)))
        ret;
    _ColGridDrawRect(grid, lineCross, lineClipped, col, doFill, doFill);
}

tpl1 dfa NT _ColGridDrawTriangle(ColGrid<T1>& grid, cx Pos2<SI>& pt1, cx Pos2<SI>& pt2, cx Pos2<SI>& pt3, cx T1& col, BO doClamp, BO doFill)
{
    T1 colUse = doFill ? COL_GRID_FILLIN_WALL : col;

    constexpr SI lineCnt = 3;
    array<Line2<SI>, lineCnt> lines;
    lines[0] = Line2<SI>(pt1, pt2);
    lines[1] = Line2<SI>(pt2, pt3);
    lines[2] = Line2<SI>(pt3, pt1);
    array<Line2<SI>, lineCnt> linesClipped;
    array<BO, lineCnt> isKeep;
    SI keepCnt = 0;
    ite (i, i < lineCnt)
    {
        keepCnt += (isKeep[i] = !Line2Clip(linesClipped[i], lines[i], Rect2<SI>(Pos2<SI>(0, 0), grid.size)));
        if (isKeep[i])
            _ColGridDrawLine1(grid, linesClipped[i], colUse);
    }

    if (doClamp)
    {
        if (keepCnt > lineCnt - 2)
        {
            cx AU drawLineEx = [&](SI i1, SI i2, SI i3) -> NT {
                if (isKeep[i1] && isKeep[i2])
                {
                    if (linesClipped[i1].b != linesClipped[i2].a)
                        _ColGridDrawLine1(grid, Line2<SI>(linesClipped[i1].b, linesClipped[i2].a), colUse);
                }
                else if (isKeep[i1])
                {
                    _ColGridDrawLine1(grid, Line2<SI>(linesClipped[i1].b, linesClipped[i3].a), colUse);
                }
                else
                {
                    _ColGridDrawLine1(grid, Line2<SI>(linesClipped[i3].b, linesClipped[i2].a), colUse);
                }
            };

            drawLineEx(0, 1, 2);
            drawLineEx(1, 2, 0);
            drawLineEx(2, 0, 1);
        }
        else
        {
            // TODO: implement
            // unimp;
            ret;
        }
    }

    if (doFill)
    {
        Pos2<SI> ptTopLeft = Pos2<SI>(Min<SI>(pt1.x, pt2.x, pt3.x), Min<SI>(pt1.y, pt2.y, pt3.y));
        cx AU keep = Rect2<SI>(Pos2<SI>(0, 0), grid.size);
        if (ptTopLeft.x < keep.pos.x)
            ptTopLeft.x = keep.pos.x;
        if (ptTopLeft.y < keep.pos.y)
            ptTopLeft.y = keep.pos.y;
        _ColGridFillin(grid, ptTopLeft, col);
    }
}

tpl1 dfa NT ColGridDrawTriangle(ColGrid<T1>& grid, cx Pos2<SI>& pt1, cx Pos2<SI>& pt2, cx Pos2<SI>& pt3, cx T1& col, BO doFill)
{
    _ColGridDrawTriangle(grid, pt1, pt2, pt3, col, doFill, doFill);
}

tpl1 dfa NT _ColGridDrawCircle(ColGrid<T1>& grid, cx Pos2<SI>& center, SI radius, cx T1& col, BO doClamp, BO doFill)
{
    T1 colUse = doFill ? COL_GRID_FILLIN_WALL : col;

    constexpr SI pointCnt = 8;

    SI x = radius;
    SI y = 0;
    SI p = 1 - radius;
    do
    {
        cx SI xt[pointCnt] = {x, -x, x, -x, y, -y, y, -y};
        cx SI yt[pointCnt] = {y, y, -y, -y, x, x, -x, -x};

        ite (i, i < pointCnt)
        {
            cx Pos2<SI> p2 = move(Pos2<SI>(center.x + xt[i], center.y + yt[i]));
            ifl (p2.x >= 0 && p2.x < grid.size.w && p2.y >= 0 && p2.y < grid.size.h)
                grid.Pixel(p2) = colUse;
        }

        ++y;
        if (p <= 0)
        {
            p = p + 2 * y + 1;
        }
        else
        {
            --x;
            p = p + 2 * y - 2 * x + 1;
        }
    } while (x >= y);

    if (doClamp)
    {
        // left clamp
        if (center.x - radius < 0)
        {
            cx AU xDist = Sqrt(Pow2(radius) - Pow2(center.x));
            cx AU yMin = Max<SI>(SI(0), center.y - xDist);
            cx AU yMax = Min<SI>(SI(grid.size.h - 1), center.y + xDist);
            cx Line2<SI> line(Pos2<SI>(0, yMin), Pos2<SI>(0, yMax));
            _ColGridDrawLineN(grid, line, 1, colUse, NO, NO);
        }

        // right clamp
        if (center.x + radius >= grid.size.w)
        {
            cx AU distToEdge = grid.size.w - 1 - center.x;
            cx AU xDist = Sqrt(Pow2(radius) - Pow2(distToEdge));
            cx AU yMin = Max<SI>(SI(0), center.y - xDist);
            cx AU yMax = Min<SI>(SI(grid.size.h - 1), center.y + xDist);
            cx Line2<SI> line(Pos2<SI>(grid.size.w - 1, yMin), Pos2<SI>(grid.size.w - 1, yMax));
            _ColGridDrawLineN(grid, line, 1, colUse, NO, NO);
        }

        // top clamp
        if (center.y - radius < 0)
        {
            cx AU yDist = Sqrt(Pow2(radius) - Pow2(center.y));
            cx AU xMin = Max<SI>(SI(0), center.x - yDist);
            cx AU xMax = Min<SI>(SI(grid.size.w - 1), center.x + yDist);
            cx Line2<SI> line(Pos2<SI>(xMin, 0), Pos2<SI>(xMax, 0));
            _ColGridDrawLineN(grid, line, 1, colUse, NO, NO);
        }

        // bottom clamp
        if (center.y + radius >= grid.size.h)
        {
            cx AU distToEdge = grid.size.h - 1 - center.y;
            cx AU yDist = Sqrt(Pow2(radius) - Pow2(distToEdge));
            cx AU xMin = Max<SI>(SI(0), center.x - yDist);
            cx AU xMax = Min<SI>(SI(grid.size.w - 1), center.x + yDist);
            cx Line2<SI> line(Pos2<SI>(xMin, grid.size.h - 1), Pos2<SI>(xMax, grid.size.h - 1));
            _ColGridDrawLineN(grid, line, 1, colUse, NO, NO);
        }
    }

    if (doFill)
    {
        Pos2<SI> ptTopLeft = Pos2<SI>(center.x - radius, center.y - radius);
        cx AU keep = Rect2<SI>(Pos2<SI>(0, 0), grid.size);
        if (ptTopLeft.x < keep.pos.x)
            ptTopLeft.x = keep.pos.x;
        if (ptTopLeft.y < keep.pos.y)
            ptTopLeft.y = keep.pos.y;
        _ColGridFillin(grid, ptTopLeft, col);
    }
}

tpl1 dfa NT ColGridDrawCircle(ColGrid<T1>& grid, cx Pos2<SI>& center, SI radius, cx T1& col, BO doFill)
{
    cx AU keep = Rect2<SI>(Pos2<SI>(0, 0), grid.size);
    ifu ((center.x + radius) < keep.pos.x || (center.x - radius) >= keep.XMax() || (center.y + radius) < keep.pos.y || (center.y - radius) >= keep.YMax())
        ret;
    ifu (radius <= 0)
        ret;
    _ColGridDrawCircle(grid, center, radius, col, doFill, doFill);
}

#ifdef PROG_THD_CNT_SINGLE
    #error "PROG_THD_CNT_SINGLE with INCLUDE_GR is not supported!"
#endif

constexpr U1 SCN_DRAW_THD_CODE_ERR_NO = 0;
constexpr U1 SCN_DRAW_THD_CODE_ERR_YES = 1;
constexpr U1 SCN_DRAW_THD_CODE_WAIT = 2;

Thd g_scnDrawThd;
S4 g_scnDrawThdDelay = 0;
HWND g_scnDrawWinHdl = NUL;

struct ScnDrawCtx
{
    ColGrid<ColRgba> scnGrid;
    HDC hdcScreen;
    GA pBits;
    HBITMAP hBitmap;
    HDC hdcMem;
    SI fpsMax;
    TmMain fpsTimer;
};

dfa LRESULT CALLBACK _ScnDrawWinProc(HWND win, UINT code, WPARAM wp, LPARAM lp)
{
    switch (code)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        ret DefWindowProcW(win, code, wp, lp);
    }
    ret 0;
}

dfa DWORD WINAPI _ScnDrawThd(LPVOID code)
{
    AU& codeInt = *reinterpret_cast<U1*>(code);
    Size2<SI> scnSize;
    ife (ScnSizeGet(scnSize))
    {
        codeInt = SCN_DRAW_THD_CODE_ERR_YES;
        ret 1;
    }
    HINSTANCE instance = GetModuleHandleW(NUL);
    WNDCLASSEXW winClass = {};
    winClass.cbSize = siz(winClass);
    winClass.lpfnWndProc = _ScnDrawWinProc;
    winClass.hInstance = instance;
    winClass.hCursor = LoadCursorA(NUL, MAKEINTRESOURCEA(32512));
    winClass.lpszClassName = L"_gr_scn_thd_win";
    cx AU winClassHdl = RegisterClassExW(&winClass);
    ifu (winClassHdl == 0)
    {
        codeInt = SCN_DRAW_THD_CODE_ERR_YES;
        ret 1;
    }
    g_scnDrawWinHdl =
        CreateWindowExW(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST, (LPCWSTR)(uintptr_t)winClassHdl, L"", WS_POPUP, 0, 0, scnSize.w, scnSize.h, NUL, NUL, instance, NUL);
    ifu (g_scnDrawWinHdl == NUL)
    {
        UnregisterClassW((LPCWSTR)(uintptr_t)winClassHdl, instance);
        codeInt = SCN_DRAW_THD_CODE_ERR_YES;
        ret 1;
    }
    ShowWindow(g_scnDrawWinHdl, SW_SHOW);                           // unchecked
    UpdateWindow(g_scnDrawWinHdl);                                  // unchecked
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST); // hint for performance, error not handled
    codeInt = SCN_DRAW_THD_CODE_ERR_NO;
    g_scnDrawThdDelay = 0;
    MSG msg;
    while (YES)
    {
        if (PeekMessageW(&msg, NUL, 0, 0, PM_REMOVE) == 0)
        {
            ++g_scnDrawThdDelay;
            if (g_scnDrawThdDelay <= 0)
                continue;
            cx S4 delay = Clamp<S4>(g_scnDrawThdDelay / 100 - 75, 1, 100); // maybe this should be configurable
            ThdWait(delay);
        }
        else
        {
            ifu (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    DestroyWindow(g_scnDrawWinHdl); // unchecked
    UnregisterClassW((LPCWSTR)(uintptr_t)winClassHdl, instance); // unchecked
    ret 0;
}

dfa NT ScnDrawFpsMax(ScnDrawCtx& ctx, SI fpsMax)
{
    ctx.fpsMax = fpsMax;
    ctx.fpsTimer = TimeMain();
}

dfa ER ScnDrawMainBegin(ScnDrawCtx& ctx)
{
    MemSetVal(ctx.scnGrid.pixels.data(), 0, ctx.scnGrid.pixels.size() * siz(ctx.scnGrid.pixels[0]));
    rets;
}
dfa ER ScnDrawMainEnd(ScnDrawCtx& ctx)
{
    HGDIOBJ oldBitmap = SelectObject(ctx.hdcMem, ctx.hBitmap);

    cx ColRgba* srcPixels = ctx.scnGrid.pixels.data();
    RGBQUAD* dstPixels = static_cast<RGBQUAD*>(ctx.pBits);

    cx AU pixelCnt = SI(ctx.scnGrid.pixels.size());
    ite (i, i < pixelCnt)
    {
        dstPixels[i].rgbRed = srcPixels[i].r;
        dstPixels[i].rgbGreen = srcPixels[i].g;
        dstPixels[i].rgbBlue = srcPixels[i].b;
        dstPixels[i].rgbReserved = srcPixels[i].a;
    }

    POINT ptPos = {0, 0};
    SIZE sizeWnd = {LONG(ctx.scnGrid.size.w), LONG(ctx.scnGrid.size.h)};
    POINT ptSrc = {0, 0};
    BLENDFUNCTION blend = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

    UpdateLayeredWindow(g_scnDrawWinHdl, ctx.hdcScreen, &ptPos, &sizeWnd, ctx.hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);

    SelectObject(ctx.hdcMem, oldBitmap);

    g_scnDrawThdDelay = 0;

    if (ctx.fpsMax > 0)
    {
        TmMain timeElapsedMs = TimeMain() - ctx.fpsTimer;
        TmMain timeSleepMs = TmMain(1000) / ctx.fpsMax - timeElapsedMs;
        ThdWait(timeSleepMs);
        ctx.fpsTimer = TimeMain();
    }

    rets;
}

dfa ER ScnDrawCtxInit(ScnDrawCtx& ctx)
{
    RECT winRect;
    ifu (GetClientRect(g_scnDrawWinHdl, &winRect) == 0)
        rete(ERR_WIN);
    ctx.scnGrid.size = Size2<SI>(winRect.right - winRect.left, winRect.bottom - winRect.top);
    ctx.scnGrid.pixels.resize(ctx.scnGrid.size.Area());

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = siz(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = ctx.scnGrid.size.w;
    bmi.bmiHeader.biHeight = -ctx.scnGrid.size.h;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    ctx.hdcScreen = GetDC(NUL);
    ctx.pBits = NUL;
    ctx.hBitmap = CreateDIBSection(ctx.hdcScreen, &bmi, DIB_RGB_COLORS, &ctx.pBits, NUL, 0);

    ifu (!ctx.hBitmap || !ctx.pBits)
    {
        ReleaseDC(NUL, ctx.hdcScreen);
        ctx.scnGrid.pixels.resize(0);
        rete(ERR_WIN);
    }

    ctx.hdcMem = CreateCompatibleDC(ctx.hdcScreen);

    ctx.fpsMax = 0;

    rets;
}
dfa ER ScnDrawCtxFree(ScnDrawCtx& ctx)
{
    DeleteObject(ctx.hBitmap);
    DeleteDC(ctx.hdcMem);
    ReleaseDC(NUL, ctx.hdcScreen);
    ctx.scnGrid.pixels.resize(0);

    rets;
}

dfa ER ScnDrawInit()
{
    volatile AU code = SCN_DRAW_THD_CODE_WAIT;
    ife (g_scnDrawThd.Start(_ScnDrawThd, const_cast<U1*>(&code)))
        retep;
    while (code == SCN_DRAW_THD_CODE_WAIT) // TODO: optimize
        ;
    ifu (code == SCN_DRAW_THD_CODE_ERR_YES)
        rete(ERR_SCN);
    rets;
}
dfa ER ScnDrawFree()
{
    ifu (PostThreadMessageW(g_scnDrawThd.Id(), WM_QUIT, 0, 0) == 0)
        rete(ERR_THD);
    ife (g_scnDrawThd.Wait())
        retep;
    ife (g_scnDrawThd.Close())
        retep;
    rets;
}
