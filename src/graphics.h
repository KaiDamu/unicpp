#pragma once

tpl1 dfa BO Line2Clip(Line2<T1>& dst, cx Line2<T1>& src, cx Rect2<T1>& keep)
{
    dst = src;

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

    AU outCodeA = outCodeGet(dst.a, keep);
    AU outCodeB = outCodeGet(dst.b, keep);

    do
    {
        if (!(outCodeA | outCodeB))
        {
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
                cx AU hPrev = ValPrev(keep.size.h);
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
                cx AU wPrev = ValPrev(keep.size.w);
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
                outCodeA = outCodeGet(dst.a, keep);
            }
            else
            {
                dst.b = pt;
                outCodeB = outCodeGet(dst.b, keep);
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
