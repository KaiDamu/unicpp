#pragma once

/// [value]
constexpr F4 COL_HSV_WEIGHT_H = 0.5f;
constexpr F4 COL_HSV_WEIGHT_S = 0.3f;
constexpr F4 COL_HSV_WEIGHT_V = 0.2f;

/// [color]
struct ColRgb
{
    U1 r; // range: 0 - 255
    U1 g; // range: 0 - 255
    U1 b; // range: 0 - 255

    dfa constexpr ColRgb() : r(), g(), b()
    {
    }
    dfa constexpr ColRgb(U1 r, U1 g, U1 b) : r(r), g(g), b(b)
    {
    }
    dfa constexpr ColRgb(cx ColRgb& other) : r(other.r), g(other.g), b(other.b)
    {
    }
    dfa constexpr ColRgb(ColRgb&& other) noex : r(move(other.r)), g(move(other.g)), b(move(other.b))
    {
    }
    dfa ~ColRgb() = default;

    dfa ColRgb& operator=(cx ColRgb& other)
    {
        r = other.r;
        g = other.g;
        b = other.b;
        ret *tx;
    }
    dfa ColRgb& operator=(ColRgb&& other) noex
    {
        r = move(other.r);
        g = move(other.g);
        b = move(other.b);
        ret *tx;
    }
    dfa constexpr BO operator==(cx ColRgb& other) cx
    {
        ret r == other.r && g == other.g && b == other.b;
    }
    dfa constexpr BO operator!=(cx ColRgb& other) cx
    {
        ret r != other.r || g != other.g || b != other.b;
    }
    dfa constexpr ColRgb operator+(cx ColRgb& other) cx
    {
        ret ColRgb(r + other.r, g + other.g, b + other.b);
    }
    dfa constexpr ColRgb operator-(cx ColRgb& other) cx
    {
        ret ColRgb(r - other.r, g - other.g, b - other.b);
    }
    dfa constexpr ColRgb operator*(F4 scalar) cx
    {
        ret ColRgb(r * scalar, g * scalar, b * scalar);
    }
    dfa constexpr ColRgb operator/(F4 scalar) cx
    {
        ret ColRgb(r / scalar, g / scalar, b / scalar);
    }
    dfa constexpr ColRgb operator-() cx
    {
        ret ColRgb(-r, -g, -b);
    }
    dfa ColRgb& operator+=(cx ColRgb& other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
        ret *tx;
    }
    dfa ColRgb& operator-=(cx ColRgb& other)
    {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        ret *tx;
    }
    dfa ColRgb& operator*=(F4 scalar)
    {
        r *= scalar;
        g *= scalar;
        b *= scalar;
        ret *tx;
    }
    dfa ColRgb& operator/=(F4 scalar)
    {
        r /= scalar;
        g /= scalar;
        b /= scalar;
        ret *tx;
    }
};
struct ColRgbN
{
    F4 r; // range: 0.0 - 1.0
    F4 g; // range: 0.0 - 1.0
    F4 b; // range: 0.0 - 1.0

    dfa constexpr ColRgbN() : r(), g(), b()
    {
    }
    dfa constexpr ColRgbN(F4 r, F4 g, F4 b) : r(r), g(g), b(b)
    {
    }
    dfa constexpr ColRgbN(cx ColRgbN& other) : r(other.r), g(other.g), b(other.b)
    {
    }
    dfa constexpr ColRgbN(ColRgbN&& other) noex : r(move(other.r)), g(move(other.g)), b(move(other.b))
    {
    }
    dfa ~ColRgbN() = default;

    dfa ColRgbN& operator=(cx ColRgbN& other)
    {
        r = other.r;
        g = other.g;
        b = other.b;
        ret *tx;
    }
    dfa ColRgbN& operator=(ColRgbN&& other) noex
    {
        r = move(other.r);
        g = move(other.g);
        b = move(other.b);
        ret *tx;
    }
    dfa constexpr BO operator==(cx ColRgbN& other) cx
    {
        ret r == other.r && g == other.g && b == other.b;
    }
    dfa constexpr BO operator!=(cx ColRgbN& other) cx
    {
        ret r != other.r || g != other.g || b != other.b;
    }
    dfa constexpr ColRgbN operator+(cx ColRgbN& other) cx
    {
        ret ColRgbN(r + other.r, g + other.g, b + other.b);
    }
    dfa constexpr ColRgbN operator-(cx ColRgbN& other) cx
    {
        ret ColRgbN(r - other.r, g - other.g, b - other.b);
    }
    dfa constexpr ColRgbN operator*(F4 scalar) cx
    {
        ret ColRgbN(r * scalar, g * scalar, b * scalar);
    }
    dfa constexpr ColRgbN operator/(F4 scalar) cx
    {
        ret ColRgbN(r / scalar, g / scalar, b / scalar);
    }
    dfa constexpr ColRgbN operator-() cx
    {
        ret ColRgbN(-r, -g, -b);
    }
    dfa ColRgbN& operator+=(cx ColRgbN& other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
        ret *tx;
    }
    dfa ColRgbN& operator-=(cx ColRgbN& other)
    {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        ret *tx;
    }
    dfa ColRgbN& operator*=(F4 scalar)
    {
        r *= scalar;
        g *= scalar;
        b *= scalar;
        ret *tx;
    }
    dfa ColRgbN& operator/=(F4 scalar)
    {
        r /= scalar;
        g /= scalar;
        b /= scalar;
        ret *tx;
    }
};
struct ColRgba
{
    union {
        struct
        {
            U1 r; // range: 0 - 255
            U1 g; // range: 0 - 255
            U1 b; // range: 0 - 255
            U1 a; // range: 0 - 255
        };
        U4 val;
    };

    dfa constexpr ColRgba() : r(), g(), b(), a()
    {
    }
    dfa constexpr ColRgba(U1 r, U1 g, U1 b, U1 a) : r(r), g(g), b(b), a(a)
    {
    }
    dfa constexpr ColRgba(cx ColRgba& other) : val(other.val)
    {
    }
    dfa constexpr ColRgba(ColRgba&& other) noex : val(move(other.val))
    {
    }
    dfa ~ColRgba() = default;

    dfa ColRgba& operator=(cx ColRgba& other)
    {
        val = other.val;
        ret *tx;
    }
    dfa ColRgba& operator=(ColRgba&& other) noex
    {
        val = move(other.val);
        ret *tx;
    }
    dfa constexpr BO operator==(cx ColRgba& other) cx
    {
        ret val == other.val;
    }
    dfa constexpr BO operator!=(cx ColRgba& other) cx
    {
        ret val != other.val;
    }
    dfa constexpr ColRgba operator+(cx ColRgba& other) cx
    {
        ret ColRgba(r + other.r, g + other.g, b + other.b, a + other.a);
    }
    dfa constexpr ColRgba operator-(cx ColRgba& other) cx
    {
        ret ColRgba(r - other.r, g - other.g, b - other.b, a - other.a);
    }
    dfa constexpr ColRgba operator*(F4 scalar) cx
    {
        ret ColRgba(r * scalar, g * scalar, b * scalar, a * scalar);
    }
    dfa constexpr ColRgba operator/(F4 scalar) cx
    {
        ret ColRgba(r / scalar, g / scalar, b / scalar, a / scalar);
    }
    dfa constexpr ColRgba operator-() cx
    {
        ret ColRgba(-r, -g, -b, -a);
    }
    dfa ColRgba& operator+=(cx ColRgba& other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
        a += other.a;
        ret *tx;
    }
    dfa ColRgba& operator-=(cx ColRgba& other)
    {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        a -= other.a;
        ret *tx;
    }
    dfa ColRgba& operator*=(F4 scalar)
    {
        r *= scalar;
        g *= scalar;
        b *= scalar;
        a *= scalar;
        ret *tx;
    }
    dfa ColRgba& operator/=(F4 scalar)
    {
        r /= scalar;
        g /= scalar;
        b /= scalar;
        a /= scalar;
        ret *tx;
    }
};
struct ColVN
{
    F4 v; // range: 0.0 - 1.0

    dfa constexpr ColVN() : v()
    {
    }
    dfa constexpr ColVN(F4 v) : v(v)
    {
    }
    dfa constexpr ColVN(cx ColVN& other) : v(other.v)
    {
    }
    dfa constexpr ColVN(ColVN&& other) noex : v(move(other.v))
    {
    }
    dfa ~ColVN() = default;

    dfa ColVN& operator=(cx ColVN& other)
    {
        v = other.v;
        ret *tx;
    }
    dfa ColVN& operator=(ColVN&& other) noex
    {
        v = move(other.v);
        ret *tx;
    }
    dfa constexpr BO operator==(cx ColVN& other) cx
    {
        ret v == other.v;
    }
    dfa constexpr BO operator!=(cx ColVN& other) cx
    {
        ret v != other.v;
    }
    dfa constexpr ColVN operator+(cx ColVN& other) cx
    {
        ret ColVN(v + other.v);
    }
    dfa constexpr ColVN operator-(cx ColVN& other) cx
    {
        ret ColVN(v - other.v);
    }
    dfa constexpr ColVN operator*(F4 scalar) cx
    {
        ret ColVN(v * scalar);
    }
    dfa constexpr ColVN operator/(F4 scalar) cx
    {
        ret ColVN(v / scalar);
    }
    dfa constexpr ColVN operator-() cx
    {
        ret ColVN(-v);
    }
    dfa ColVN& operator+=(cx ColVN& other)
    {
        v += other.v;
        ret *tx;
    }
    dfa ColVN& operator-=(cx ColVN& other)
    {
        v -= other.v;
        ret *tx;
    }
    dfa ColVN& operator*=(F4 scalar)
    {
        v *= scalar;
        ret *tx;
    }
    dfa ColVN& operator/=(F4 scalar)
    {
        v /= scalar;
        ret *tx;
    }

    dfa constexpr BO operator<(cx ColVN& other) cx
    {
        ret v < other.v;
    }
    dfa constexpr BO operator>(cx ColVN& other) cx
    {
        ret v > other.v;
    }
    dfa constexpr BO operator<=(cx ColVN& other) cx
    {
        ret v <= other.v;
    }
    dfa constexpr BO operator>=(cx ColVN& other) cx
    {
        ret v >= other.v;
    }
    dfa constexpr ColVN operator*(cx ColVN& other) cx
    {
        ret ColVN(v * other.v);
    }
    dfa constexpr ColVN operator/(cx ColVN& other) cx
    {
        ret ColVN(v / other.v);
    }
    dfa ColVN& operator*=(cx ColVN& other)
    {
        v *= other.v;
        ret *tx;
    }
    dfa ColVN& operator/=(cx ColVN& other)
    {
        v /= other.v;
        ret *tx;
    }
};
struct ColHsvN
{
    F4 h; // range: 0.0 - 1.0
    F4 s; // range: 0.0 - 1.0
    F4 v; // range: 0.0 - 1.0

    dfa constexpr ColHsvN() : h(), s(), v()
    {
    }
    dfa constexpr ColHsvN(F4 h, F4 s, F4 v) : h(h), s(s), v(v)
    {
    }
    dfa constexpr ColHsvN(cx ColHsvN& other) : h(other.h), s(other.s), v(other.v)
    {
    }
    dfa constexpr ColHsvN(ColHsvN&& other) noex : h(move(other.h)), s(move(other.s)), v(move(other.v))
    {
    }
    dfa ~ColHsvN() = default;

    dfa ColHsvN& operator=(cx ColHsvN& other)
    {
        h = other.h;
        s = other.s;
        v = other.v;
        ret *tx;
    }
    dfa ColHsvN& operator=(ColHsvN&& other) noex
    {
        h = move(other.h);
        s = move(other.s);
        v = move(other.v);
        ret *tx;
    }
    dfa constexpr BO operator==(cx ColHsvN& other) cx
    {
        ret h == other.h && s == other.s && v == other.v;
    }
    dfa constexpr BO operator!=(cx ColHsvN& other) cx
    {
        ret h != other.h || s != other.s || v != other.v;
    }
    dfa constexpr ColHsvN operator+(cx ColHsvN& other) cx
    {
        ret ColHsvN(h + other.h, s + other.s, v + other.v);
    }
    dfa constexpr ColHsvN operator-(cx ColHsvN& other) cx
    {
        ret ColHsvN(h - other.h, s - other.s, v - other.v);
    }
    dfa constexpr ColHsvN operator*(F4 scalar) cx
    {
        ret ColHsvN(h * scalar, s * scalar, v * scalar);
    }
    dfa constexpr ColHsvN operator/(F4 scalar) cx
    {
        ret ColHsvN(h / scalar, s / scalar, v / scalar);
    }
    dfa constexpr ColHsvN operator-() cx
    {
        ret ColHsvN(-h, -s, -v);
    }
    dfa ColHsvN& operator+=(cx ColHsvN& other)
    {
        h += other.h;
        s += other.s;
        v += other.v;
        ret *tx;
    }
    dfa ColHsvN& operator-=(cx ColHsvN& other)
    {
        h -= other.h;
        s -= other.s;
        v -= other.v;
        ret *tx;
    }
    dfa ColHsvN& operator*=(F4 scalar)
    {
        h *= scalar;
        s *= scalar;
        v *= scalar;
        ret *tx;
    }
    dfa ColHsvN& operator/=(F4 scalar)
    {
        h /= scalar;
        s /= scalar;
        v /= scalar;
        ret *tx;
    }
};

/// [color grid]
tpl1 struct ColGrid
{
    vector<T1> pixels;
    Size2<SI> size;

    dfa T1& Pixel(cx Pos2<SI>& pos)
    {
        ret pixels[pos.y * size.w + pos.x];
    }

    dfa ColGrid()
    {
    }
    dfa ColGrid(cx Size2<SI>& size) : size(size)
    {
        pixels.resize(size.Area());
    }
};

/// [convert color]
tpl0 dfa NT ToType(ColVN& dst, cx ColRgbN& src)
{
    dst.v = src.r * 0.299f + src.g * 0.587f + src.b * 0.114f;
}
tpl0 dfa NT ToType(ColHsvN& dst, cx ColRgbN& src)
{
    cx AU max = Max(src.r, src.g, src.b);
    dst.v = max;
    if (max < 0.00001f)
    {
        dst.h = 0.0f;
        dst.s = 0.0f;
        ret;
    }

    cx AU min = Min(src.r, src.g, src.b);
    cx AU delta = max - min;
    if (delta < 0.00001f)
    {
        dst.h = 0.0f;
        dst.s = 0.0f;
        ret;
    }
    dst.s = delta / max;

    if (src.r >= max)
    {
        dst.h = (src.g - src.b) / delta;
    }
    else if (src.g >= max)
    {
        dst.h = (src.b - src.r) / delta + 2.0f;
    }
    else
    {
        dst.h = (src.r - src.g) / delta + 4.0f;
    }
    dst.h /= 6.0f;
    if (dst.h < 0.0f)
    {
        dst.h += 1.0f;
    }
}
tpl0 dfa NT ToType(ColRgbN& dst, cx ColHsvN& src)
{
    if (src.s < 0.00001f)
    {
        dst.r = src.v;
        dst.g = src.v;
        dst.b = src.v;
        ret;
    }

    AU h = src.h * 6.0f;
    if (h >= 6.0f)
    {
        h = 0.0f;
    }
    cx AU i = h;
    cx AU f = h - i;
    cx AU p = src.v * (1.0f - src.s);
    cx AU q = src.v * (1.0f - src.s * f);
    cx AU t = src.v * (1.0f - src.s * (1.0f - f));

    switch (SI(i))
    {
    case 0:
        dst.r = src.v;
        dst.g = t;
        dst.b = p;
        break;
    case 1:
        dst.r = q;
        dst.g = src.v;
        dst.b = p;
        break;
    case 2:
        dst.r = p;
        dst.g = src.v;
        dst.b = t;
        break;
    case 3:
        dst.r = p;
        dst.g = q;
        dst.b = src.v;
        break;
    case 4:
        dst.r = t;
        dst.g = p;
        dst.b = src.v;
        break;
    case 5:
        dst.r = src.v;
        dst.g = p;
        dst.b = q;
        break;
    default:
        dst.r = 0.0f;
        dst.g = 0.0f;
        dst.b = 0.0f;
        break;
    }
}
tpl0 dfa NT ToType(ColRgbN& dst, cx ColVN& src)
{
    dst.r = src.v;
    dst.g = src.v;
    dst.b = src.v;
}
tpl0 dfa NT ToType(ColRgb& dst, cx ColRgb& src)
{
    dst = move(src);
}
tpl0 dfa NT ToType(ColRgb& dst, cx ColRgbN& src)
{
    dst.r = U1(src.r * 255.0f);
    dst.g = U1(src.g * 255.0f);
    dst.b = U1(src.b * 255.0f);
}
tpl0 dfa NT ToType(ColRgb& dst, cx ColVN& src)
{
    dst.r = U1(src.v * 255.0f);
    dst.g = U1(src.v * 255.0f);
    dst.b = U1(src.v * 255.0f);
}
tpl0 dfa NT ToType(ColRgbN& dst, cx ColRgb& src)
{
    dst.r = F4(src.r) / 255.0f;
    dst.g = F4(src.g) / 255.0f;
    dst.b = F4(src.b) / 255.0f;
}
tpl0 dfa NT ToType(ColVN& dst, cx ColRgb& src)
{
    dst.v = (F4(src.r) * 0.299f + F4(src.g) * 0.587f + F4(src.b) * 0.114f) / 255.0f;
}
tpl0 dfa NT ToType(ColHsvN& dst, cx ColRgb& src)
{
    ColRgbN tmp;
    ToType(tmp, src);
    ToType(dst, tmp);
}

/// [convert color grid]
tpl2 dfa NT ColGridToType(ColGrid<T1>& dst, cx ColGrid<T2>& src)
{
    dst.size = src.size;
    dst.pixels.resize(src.pixels.size());
    cx AU iEnd = SI(src.pixels.size());
    ite (i, i < iEnd)
    {
        ToType<T1, T2>(dst.pixels[i], src.pixels[i]);
    }
}

/// [compare color]
dfa F4 ColHsvNCmp(cx ColHsvN& a, cx ColHsvN& b, F4 wH = COL_HSV_WEIGHT_H, F4 wS = COL_HSV_WEIGHT_S, F4 wV = COL_HSV_WEIGHT_V)
{
    ret (DiffWrap(a.h, b.h, 1.0f) * 2.0f * wH) + (Diff(a.s, b.s) * wS) + (Diff(a.v, b.v) * wV);
}

/// [compare color grid]
struct ColGridHsvNCmpInfo
{
    Pos2<SI> subGridPos;
    cx ColGrid<ColVN>* subGridMask;
    cx ColGrid<ColVN>* subGridEdges;
    cx ColGrid<ColVN>* mainGridEdges;
    F4 wCol;
    F4 wEdges;
    F4 wH;
    F4 wS;
    F4 wV;

    ColGridHsvNCmpInfo()
    {
        subGridPos = Pos2<SI>(0, 0);
        subGridMask = NUL;
        subGridEdges = NUL;
        mainGridEdges = NUL;
        wCol = 1.0f;
        wEdges = 1.0f;
        wH = COL_HSV_WEIGHT_H;
        wS = COL_HSV_WEIGHT_S;
        wV = COL_HSV_WEIGHT_V;
    }
};
dfa ER ColGridHsvNCmp(F4& resultDiff, cx ColGrid<ColHsvN>& subGrid, cx ColGrid<ColHsvN>& mainGrid, cx ColGridHsvNCmpInfo& info)
{
    resultDiff = 1.0f;

    // check pos & size
    ifu (info.subGridPos.x + subGrid.size.w > mainGrid.size.w || info.subGridPos.y + subGrid.size.h > mainGrid.size.h)
    {
        rete(ERR_HIGH_SIZE);
    }
    ifu (info.subGridPos.x < 0 || info.subGridPos.y < 0)
    {
        rete(ERR_LOW_SIZE);
    }
    ifu ((info.subGridMask != NUL) && (info.subGridMask->size.w != subGrid.size.w || info.subGridMask->size.h != subGrid.size.h))
    {
        rete(ERR_NO_VALID);
    }
    ifu ((info.subGridEdges != NUL) && (info.subGridEdges->size.w != subGrid.size.w || info.subGridEdges->size.h != subGrid.size.h))
    {
        rete(ERR_NO_VALID);
    }
    ifu ((info.mainGridEdges != NUL) && (info.mainGridEdges->size.w != mainGrid.size.w || info.mainGridEdges->size.h != mainGrid.size.h))
    {
        rete(ERR_NO_VALID);
    }

    // compare
    F4 diffSum = 0.0f;
    F4 diffCnt = (info.subGridMask == NUL) ? F4(subGrid.size.Area()) : 0.0f;
    cx AU doEdgesProc = (info.subGridEdges != NUL && info.mainGridEdges != NUL) ? YES : NO;
    cx AU iEnd = subGrid.size.Area();
    ite (i, i < iEnd)
    {
        cx AU iMain = ((i / subGrid.size.w) + info.subGridPos.y) * mainGrid.size.w + ((i % subGrid.size.w) + info.subGridPos.x);
        cx AU diff = ColHsvNCmp(subGrid.pixels[i], mainGrid.pixels[iMain], info.wH, info.wS, info.wV);
        if (info.subGridMask == NUL)
        {
            diffSum += diff * info.wCol;
        }
        else
        {
            cx AU& maskVal = info.subGridMask->pixels[i].v;
            diffSum += diff * info.wCol * maskVal;
            diffCnt += maskVal;
        }
        if (doEdgesProc)
        {
            cx AU diffEdge = Diff(info.subGridEdges->pixels[i].v, info.mainGridEdges->pixels[iMain].v);
            if (info.subGridMask == NUL)
            {
                diffSum += diffEdge * info.wEdges;
            }
            else
            {
                cx AU& maskVal = info.subGridMask->pixels[i].v;
                diffSum += diffEdge * info.wEdges * maskVal;
            }
        }
    }

    cx AU diffSumWMul = 1.0f / (doEdgesProc ? (info.wCol + info.wEdges) : info.wCol);
    resultDiff = diffSum * diffSumWMul / diffCnt;
    rets;
}

/// [process color grid]
dfa NT ColGridEdgesSobel1(ColGrid<ColVN>& out, cx ColGrid<ColVN>& in)
{
    cx AU w = in.size.w;
    cx AU h = in.size.h;

    out.size = Size2<SI>(w, h);
    out.pixels.resize(w * h);

    // main computation
    for (SI y = 1; y < h - 1; ++y)
    {
        for (SI x = 1; x < w - 1; ++x)
        {
            cx AU p00 = in.pixels[(y - 1) * w + (x - 1)].v;
            cx AU p01 = in.pixels[(y - 1) * w + x].v;
            cx AU p02 = in.pixels[(y - 1) * w + (x + 1)].v;
            cx AU p10 = in.pixels[y * w + (x - 1)].v;
            cx AU p12 = in.pixels[y * w + (x + 1)].v;
            cx AU p20 = in.pixels[(y + 1) * w + (x - 1)].v;
            cx AU p21 = in.pixels[(y + 1) * w + x].v;
            cx AU p22 = in.pixels[(y + 1) * w + (x + 1)].v;

            cx AU gX = (-p00) + p02 + (-2.0f * p10) + (2.0f * p12) + (-p20) + p22;
            cx AU gY = (-p00) + (-2.0f * p01) + (-p02) + p20 + (2.0f * p21) + p22;

            out.pixels[y * w + x].v = Dist0Fast(gX, gY);
        }
    }

    // set borders to zero
    for (SI x = 0; x < w; ++x)
    {
        out.pixels[x].v = 0;
        out.pixels[(h - 1) * w + x].v = 0;
    }
    for (SI y = 0; y < h; ++y)
    {
        out.pixels[y * w].v = 0;
        out.pixels[y * w + (w - 1)].v = 0;
    }

    NormalizeMax<ColVN>(out.pixels.data(), out.pixels.size());
}
