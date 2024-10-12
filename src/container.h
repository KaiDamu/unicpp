#pragma once

tpl1 struct Rect2
{
    Pos2<T1> pos;
    Size2<T1> size;

    dfa T1 XMax() cx
    {
        ret pos.x + size.w;
    }
    dfa T1 YMax() cx
    {
        ret pos.y + size.h;
    }

    dfa Rect2()
    {
    }
    dfa Rect2(cx Pos2<T1>& pos, cx Size2<T1>& size) : pos(pos), size(size)
    {
    }
    dfa Rect2(T1 x, T1 y, T1 w, T1 h) : pos(x, y), size(w, h)
    {
    }
};

tpl1 struct Line2
{
    Pos2<T1> a;
    Pos2<T1> b;

    dfa Line2()
    {
    }
    dfa Line2(cx Pos2<T1>& a, cx Pos2<T1>& b) : a(a), b(b)
    {
    }
    dfa Line2(T1 x1, T1 y1, T1 x2, T1 y2) : a(x1, y1), b(x2, y2)
    {
    }
};
