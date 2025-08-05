#pragma once

enum class GrShape : U1
{
    NONE = 0,
    PT = 1,
    LINE = 2,
    RECT = 3,
    TRIANGLE = 4,
    CIRCLE = 5,
};

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
        else if (pt.x >= keep.XEnd())
            outCode |= OutCode::RIGHT;

        if (pt.y < keep.pos.y)
            outCode |= OutCode::BOTTOM;
        else if (pt.y >= keep.YEnd())
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

tpl1 dfa NT ColGridDrawClr(ColGrid<T1>& grid, cx T1& col)
{
    T1* p = grid.pixels.data();
    cx T1* cx pEnd = p + grid.pixels.size();
    while (p != pEnd)
        *p++ = col;
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
    cx F4 len = Dist0(F4(dx), F4(dy));
    cx F4 ofsX = (dy * (ValPrev(thickness) / 2.0f)) / len * -1;
    cx F4 ofsY = (dx * (ValPrev(thickness) / 2.0f)) / len;

    cxex SI ptCnt = 4;
    std::array<Pos2<SI>, ptCnt> pt;
    pt[0] = Pos2<SI>(RoundToInt(F4(line.b.x) - ofsX), RoundToInt(F4(line.b.y) - ofsY));
    pt[1] = Pos2<SI>(RoundToInt(F4(line.a.x) - ofsX), RoundToInt(F4(line.a.y) - ofsY));
    pt[2] = Pos2<SI>(RoundToInt(F4(line.a.x) + ofsX), RoundToInt(F4(line.a.y) + ofsY));
    pt[3] = Pos2<SI>(RoundToInt(F4(line.b.x) + ofsX), RoundToInt(F4(line.b.y) + ofsY));
    cxex SI lineCnt = 4;
    std::array<Line2<SI>, lineCnt> lines;
    lines[0] = Line2<SI>(pt[0], pt[1]);
    lines[1] = Line2<SI>(pt[1], pt[2]);
    lines[2] = Line2<SI>(pt[2], pt[3]);
    lines[3] = Line2<SI>(pt[3], pt[0]);

    std::array<Line2<SI>, lineCnt> linesClipped;
    std::array<BO, lineCnt> isKeep;
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
    Line2<SI> lineCross(Pos2<SI>(rect.pos.x, rect.pos.y), Pos2<SI>(ValPrev(rect.XEnd()), ValPrev(rect.YEnd())));
    Line2<SI> lineClipped;
    if (Line2Clip(lineClipped, lineCross, Rect2<SI>(Pos2<SI>(0, 0), grid.size)))
        ret;
    _ColGridDrawRect(grid, lineCross, lineClipped, col, doFill, doFill);
}

tpl1 dfa NT _ColGridDrawTriangle(ColGrid<T1>& grid, cx Triangle2<SI>& triangle, cx T1& col, BO doClamp, BO doFill)
{
    T1 colUse = doFill ? COL_GRID_FILLIN_WALL : col;

    cxex SI lineCnt = 3;
    std::array<Line2<SI>, lineCnt> lines;
    lines[0] = Line2<SI>(triangle.a, triangle.b);
    lines[1] = Line2<SI>(triangle.b, triangle.c);
    lines[2] = Line2<SI>(triangle.c, triangle.a);
    std::array<Line2<SI>, lineCnt> linesClipped;
    std::array<BO, lineCnt> isKeep;
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
        Pos2<SI> ptTopLeft = Pos2<SI>(Min<SI>(triangle.a.x, triangle.b.x, triangle.c.x), Min<SI>(triangle.a.y, triangle.b.y, triangle.c.y));
        cx AU keep = Rect2<SI>(Pos2<SI>(0, 0), grid.size);
        if (ptTopLeft.x < keep.pos.x)
            ptTopLeft.x = keep.pos.x;
        if (ptTopLeft.y < keep.pos.y)
            ptTopLeft.y = keep.pos.y;
        _ColGridFillin(grid, ptTopLeft, col);
    }
}

tpl1 dfa NT ColGridDrawTriangle(ColGrid<T1>& grid, cx Triangle2<SI>& triangle, cx T1& col, BO doFill)
{
    _ColGridDrawTriangle(grid, triangle, col, doFill, doFill);
}

tpl1 dfa NT _ColGridDrawCircle(ColGrid<T1>& grid, cx Circle2<SI>& circle, cx T1& col, BO doClamp, BO doFill)
{
    T1 colUse = doFill ? COL_GRID_FILLIN_WALL : col;

    cxex SI pointCnt = 8;

    SI x = circle.radius;
    SI y = 0;
    SI p = 1 - circle.radius;
    do
    {
        cx SI xt[pointCnt] = {x, -x, x, -x, y, -y, y, -y};
        cx SI yt[pointCnt] = {y, y, -y, -y, x, x, -x, -x};

        ite (i, i < pointCnt)
        {
            cx Pos2<SI> p2 = std::move(Pos2<SI>(circle.center.x + xt[i], circle.center.y + yt[i]));
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
        if (circle.center.x - circle.radius < 0)
        {
            cx AU xDist = Sqrt(Pow2(circle.radius) - Pow2(circle.center.x));
            cx AU yMin = Max<SI>(SI(0), circle.center.y - xDist);
            cx AU yMax = Min<SI>(SI(grid.size.h - 1), circle.center.y + xDist);
            cx Line2<SI> line(Pos2<SI>(0, yMin), Pos2<SI>(0, yMax));
            _ColGridDrawLineN(grid, line, 1, colUse, NO, NO);
        }

        // right clamp
        if (circle.center.x + circle.radius >= grid.size.w)
        {
            cx AU distToEdge = grid.size.w - 1 - circle.center.x;
            cx AU xDist = Sqrt(Pow2(circle.radius) - Pow2(distToEdge));
            cx AU yMin = Max<SI>(SI(0), circle.center.y - xDist);
            cx AU yMax = Min<SI>(SI(grid.size.h - 1), circle.center.y + xDist);
            cx Line2<SI> line(Pos2<SI>(grid.size.w - 1, yMin), Pos2<SI>(grid.size.w - 1, yMax));
            _ColGridDrawLineN(grid, line, 1, colUse, NO, NO);
        }

        // top clamp
        if (circle.center.y - circle.radius < 0)
        {
            cx AU yDist = Sqrt(Pow2(circle.radius) - Pow2(circle.center.y));
            cx AU xMin = Max<SI>(SI(0), circle.center.x - yDist);
            cx AU xMax = Min<SI>(SI(grid.size.w - 1), circle.center.x + yDist);
            cx Line2<SI> line(Pos2<SI>(xMin, 0), Pos2<SI>(xMax, 0));
            _ColGridDrawLineN(grid, line, 1, colUse, NO, NO);
        }

        // bottom clamp
        if (circle.center.y + circle.radius >= grid.size.h)
        {
            cx AU distToEdge = grid.size.h - 1 - circle.center.y;
            cx AU yDist = Sqrt(Pow2(circle.radius) - Pow2(distToEdge));
            cx AU xMin = Max<SI>(SI(0), circle.center.x - yDist);
            cx AU xMax = Min<SI>(SI(grid.size.w - 1), circle.center.x + yDist);
            cx Line2<SI> line(Pos2<SI>(xMin, grid.size.h - 1), Pos2<SI>(xMax, grid.size.h - 1));
            _ColGridDrawLineN(grid, line, 1, colUse, NO, NO);
        }
    }

    if (doFill)
    {
        Pos2<SI> ptTopLeft = Pos2<SI>(circle.center.x - circle.radius, circle.center.y - circle.radius);
        cx AU keep = Rect2<SI>(Pos2<SI>(0, 0), grid.size);
        if (ptTopLeft.x < keep.pos.x)
            ptTopLeft.x = keep.pos.x;
        if (ptTopLeft.y < keep.pos.y)
            ptTopLeft.y = keep.pos.y;
        _ColGridFillin(grid, ptTopLeft, col);
    }
}

tpl1 dfa NT ColGridDrawCircle(ColGrid<T1>& grid, cx Circle2<SI>& circle, cx T1& col, BO doFill)
{
    cx AU keep = Rect2<SI>(Pos2<SI>(0, 0), grid.size);
    ifu ((circle.center.x + circle.radius) < keep.pos.x || (circle.center.x - circle.radius) >= keep.XEnd() || (circle.center.y + circle.radius) < keep.pos.y ||
         (circle.center.y - circle.radius) >= keep.YEnd())
        ret;
    ifu (circle.radius <= 0)
        ret;
    _ColGridDrawCircle(grid, circle, col, doFill, doFill);
}

tpl1 dfa NT ColGridDrawUcppLogo(ColGrid<T1>& grid, cx Rect2<SI>& rect)
{
    cx SI shapeCnt = 11;
    cx SI valCnt = 40;

    // clang-format off
    cx GrShape shapes[shapeCnt] = {
        GrShape::CIRCLE,
        GrShape::CIRCLE,
        GrShape::RECT,
        GrShape::RECT,
        GrShape::CIRCLE,
        GrShape::CIRCLE,
        GrShape::RECT,
        GrShape::RECT,
        GrShape::RECT,
        GrShape::RECT,
        GrShape::RECT,
    };
    cx F4 vals[valCnt] = {
        0.199f, 0.350f, 0.199f,
        0.499f, 0.650f, 0.199f,
        0.199f, 0.151f, 0.101f, 0.399f,
        0.300f, 0.550f, 0.399f, 0.101f,
        0.199f, 0.350f, 0.099f,
        0.499f, 0.650f, 0.099f,
        0.199f, 0.251f, 0.101f, 0.199f,
        0.400f, 0.450f, 0.199f, 0.201f,
        0.300f, 0.151f, 0.100f, 0.399f,
        0.599f, 0.151f, 0.100f, 0.399f,
        0.699f, 0.450f, 0.300f, 0.100f,
    };
    cx ColRgba cols[shapeCnt] = {
        ColRgba(0xFF892716),
        ColRgba(0xFFB2331C),
        ColRgba(0xFF892716),
        ColRgba(0xFFB2331C),
        ColRgba(0x00000000),
        ColRgba(0x00000000),
        ColRgba(0x00000000),
        ColRgba(0x00000000),
        ColRgba(0xFF030BA3),
        ColRgba(0xFF040ECC),
        ColRgba(0xFF892716),
    };
    // clang-format on

    cx F4 rectSize = F4(Min(rect.size.w, rect.size.h));

    SI valsUnnorm[valCnt];
    ite (i, i < valCnt)
    {
        valsUnnorm[i] = SI(RoundToInt(vals[i] * rectSize));
    }

    cx SI* valCur = valsUnnorm;
    ite (i, i < shapeCnt)
    {
        switch (shapes[i])
        {
        case GrShape::RECT: {
            ColGridDrawRect(grid, Rect2<SI>({valCur[0] + rect.pos.x, valCur[1] + rect.pos.y}, {valCur[2], valCur[3]}), cols[i], YES);
            valCur += 4;
            break;
        }
        case GrShape::CIRCLE: {
            ColGridDrawCircle(grid, Circle2<SI>({valCur[0] + rect.pos.x, valCur[1] + rect.pos.y}, valCur[2]), cols[i], YES);
            valCur += 3;
            break;
        }
        default: {
            break;
        }
        }
    }
}

tpl1 dfa NT ColGridDrawColGrid(ColGrid<T1>& grid, cx ColGrid<T1>& gridSrc, cx Pos2<SI>& pos)
{
    cx AU gridSrcW = gridSrc.size.w;
    cx AU gridSrcH = gridSrc.size.h;
    cx AU gridW = grid.size.w;
    cx AU gridH = grid.size.h;

    ite (y, y < gridH)
    {
        ite (x, x < gridW)
        {
            Pos2<SI> ptSrc = Pos2<SI>(x - pos.x, y - pos.y);
            if (ptSrc.x >= 0 && ptSrc.x < gridSrcW && ptSrc.y >= 0 && ptSrc.y < gridSrcH)
                grid.Pixel(Pos2<SI>(x, y)) = gridSrc.Pixel(ptSrc);
        }
    }
}

/// /// /// [ Screen Draw section ] /// /// ///
#ifdef PROG_SYS_WIN

cxex U1 SCN_DRAW_THD_CODE_ERR_NO = 0;
cxex U1 SCN_DRAW_THD_CODE_ERR_YES = 1;
cxex U1 SCN_DRAW_THD_CODE_WAIT = 2;

Thd g_scnDrawThd;
S4 g_scnDrawThdDelay = 0;
HWND g_scnDrawWinHdl = NUL;

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
    dfa NT FpsMax(SI fpsMax)
    {
        m_fpsMax = fpsMax;
        m_fpsTimer = TimeMain();
    }

  public:
    dfa NT DrawClr(cx ColRgba& col)
    {
        ColGridDrawClr(m_scnGrid, col);
    }
    dfa NT DrawPt(cx Pos2<SI>& pt, cx ColRgba& col)
    {
        ColGridDrawPt(m_scnGrid, pt, col);
    }
    dfa NT DrawLine(cx Line2<SI>& line, SI thickness, cx ColRgba& col, BO doFill)
    {
        ColGridDrawLine(m_scnGrid, line, thickness, col, doFill);
    }
    dfa NT DrawRect(cx Rect2<SI>& rect, cx ColRgba& col, BO doFill)
    {
        ColGridDrawRect(m_scnGrid, rect, col, doFill);
    }
    dfa NT DrawTriangle(cx Triangle2<SI>& triangle, cx ColRgba& col, BO doFill)
    {
        ColGridDrawTriangle(m_scnGrid, triangle, col, doFill);
    }
    dfa NT DrawCircle(cx Circle2<SI>& circle, cx ColRgba& col, BO doFill)
    {
        ColGridDrawCircle(m_scnGrid, circle, col, doFill);
    }
    dfa NT DrawUcppLogo(cx Rect2<SI>& rect)
    {
        ColGridDrawUcppLogo(m_scnGrid, rect);
    }
    dfa NT DrawColGrid(cx ColGrid<ColRgba>& gridSrc, cx Pos2<SI>& pos)
    {
        ColGridDrawColGrid(m_scnGrid, gridSrc, pos);
    }

  public:
    dfa ER MainBegin()
    {
        MemSet(m_scnGrid.pixels.data(), 0, m_scnGrid.pixels.size() * siz(m_scnGrid.pixels[0]));
        rets;
    }
    dfa ER MainEnd()
    {
        HGDIOBJ oldBitmap = SelectObject(m_hdcMem, m_hBitmap);

        cx ColRgba* srcPixels = m_scnGrid.pixels.data();
        RGBQUAD* dstPixels = static_cast<RGBQUAD*>(m_pBits);

        cx AU pixelCnt = SI(m_scnGrid.pixels.size());
        ite (i, i < pixelCnt)
        {
            dstPixels[i].rgbRed = srcPixels[i].r;
            dstPixels[i].rgbGreen = srcPixels[i].g;
            dstPixels[i].rgbBlue = srcPixels[i].b;
            dstPixels[i].rgbReserved = srcPixels[i].a;
        }

        POINT ptPos = {0, 0};
        SIZE sizeWnd = {LONG(m_scnGrid.size.w), LONG(m_scnGrid.size.h)};
        POINT ptSrc = {0, 0};
        BLENDFUNCTION blend = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

        UpdateLayeredWindow(g_scnDrawWinHdl, m_hdcScreen, &ptPos, &sizeWnd, m_hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);

        SelectObject(m_hdcMem, oldBitmap);

        g_scnDrawThdDelay = 0;

        if (m_fpsMax > 0)
        {
            cx AU timeElapsedMs = TimeMain() - m_fpsTimer;
            cx AU timeSleepMs = TmMain(1000) / TmMain(m_fpsMax) - timeElapsedMs;
            ThdWait(timeSleepMs);
            m_fpsTimer = TimeMain();
        }

        rets;
    }
    dfa ER Init()
    {
        RECT winRect;
        ifu (GetClientRect(g_scnDrawWinHdl, &winRect) == 0)
            rete(ErrVal::WIN);
        m_scnGrid.size = Size2<SI>(winRect.right - winRect.left, winRect.bottom - winRect.top);
        m_scnGrid.pixels.resize(m_scnGrid.size.Area());

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = siz(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = LONG(m_scnGrid.size.w);
        bmi.bmiHeader.biHeight = LONG(-m_scnGrid.size.h);
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        m_hdcScreen = GetDC(NUL);
        m_pBits = NUL;
        m_hBitmap = CreateDIBSection(m_hdcScreen, &bmi, DIB_RGB_COLORS, &m_pBits, NUL, 0);

        ifu (!m_hBitmap || !m_pBits)
        {
            ReleaseDC(NUL, m_hdcScreen);
            m_scnGrid.pixels.resize(0);
            rete(ErrVal::WIN);
        }

        m_hdcMem = CreateCompatibleDC(m_hdcScreen);

        m_fpsMax = 0;

        rets;
    }
    dfa ER Free()
    {
        DeleteObject(m_hBitmap);
        DeleteDC(m_hdcMem);
        ReleaseDC(NUL, m_hdcScreen);
        m_scnGrid.pixels.resize(0);

        rets;
    }
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

dfa S4 _ScnDrawThd(GA code)
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
    g_scnDrawWinHdl = CreateWindowExW(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST, (LPCWSTR)(uintptr_t)winClassHdl, L"", WS_POPUP, 0, 0, int(scnSize.w), int(scnSize.h), NUL,
                                      NUL, instance, NUL);
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
            cx AU delay = Clamp<TmMain>(TmMain(g_scnDrawThdDelay) / 100 - 75, 1, 100); // maybe this should be configurable
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

dfa ER ScnDrawInit()
{
    volatile AU code = SCN_DRAW_THD_CODE_WAIT;
    ife (g_scnDrawThd.New(_ScnDrawThd, const_cast<U1*>(&code)))
        retep;
    while (code == SCN_DRAW_THD_CODE_WAIT)
        NtYieldExecution_();
    ifu (code == SCN_DRAW_THD_CODE_ERR_YES)
        rete(ErrVal::SCN);
    rets;
}
dfa ER ScnDrawFree()
{
    ifu (PostThreadMessageW(DWORD(UA(g_scnDrawThd.Id())), WM_QUIT, 0, 0) == 0)
        rete(ErrVal::THD);
    ife (g_scnDrawThd.Wait())
        retep;
    ife (g_scnDrawThd.Close())
        retep;
    rets;
}

#endif
