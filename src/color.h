#pragma once

/// [value]
cxex F4 COL_HSV_WEIGHT_H = 0.5f;
cxex F4 COL_HSV_WEIGHT_S = 0.3f;
cxex F4 COL_HSV_WEIGHT_V = 0.2f;

/// [color]
struct ColRgb
{
    U1 r; // range: 0 - 255
    U1 g; // range: 0 - 255
    U1 b; // range: 0 - 255

    dfa cxex ColRgb() : r(), g(), b()
    {
    }
    dfa cxex ColRgb(U1 r, U1 g, U1 b) : r(r), g(g), b(b)
    {
    }
    dfa cxex ColRgb(cx ColRgb& other) : r(other.r), g(other.g), b(other.b)
    {
    }
    dfa cxex ColRgb(ColRgb&& other) noex : r(move(other.r)), g(move(other.g)), b(move(other.b))
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
    dfa cxex BO operator==(cx ColRgb& other) cx
    {
        ret r == other.r && g == other.g && b == other.b;
    }
    dfa cxex BO operator!=(cx ColRgb& other) cx
    {
        ret r != other.r || g != other.g || b != other.b;
    }
    dfa cxex ColRgb operator+(cx ColRgb& other) cx
    {
        ret ColRgb(r + other.r, g + other.g, b + other.b);
    }
    dfa cxex ColRgb operator-(cx ColRgb& other) cx
    {
        ret ColRgb(r - other.r, g - other.g, b - other.b);
    }
    dfa cxex ColRgb operator*(F4 scalar) cx
    {
        ret ColRgb(r * scalar, g * scalar, b * scalar);
    }
    dfa cxex ColRgb operator/(F4 scalar) cx
    {
        ret ColRgb(r / scalar, g / scalar, b / scalar);
    }
    dfa cxex ColRgb operator-() cx
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

    dfa cxex ColRgbN() : r(), g(), b()
    {
    }
    dfa cxex ColRgbN(F4 r, F4 g, F4 b) : r(r), g(g), b(b)
    {
    }
    dfa cxex ColRgbN(cx ColRgbN& other) : r(other.r), g(other.g), b(other.b)
    {
    }
    dfa cxex ColRgbN(ColRgbN&& other) noex : r(move(other.r)), g(move(other.g)), b(move(other.b))
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
    dfa cxex BO operator==(cx ColRgbN& other) cx
    {
        ret r == other.r && g == other.g && b == other.b;
    }
    dfa cxex BO operator!=(cx ColRgbN& other) cx
    {
        ret r != other.r || g != other.g || b != other.b;
    }
    dfa cxex ColRgbN operator+(cx ColRgbN& other) cx
    {
        ret ColRgbN(r + other.r, g + other.g, b + other.b);
    }
    dfa cxex ColRgbN operator-(cx ColRgbN& other) cx
    {
        ret ColRgbN(r - other.r, g - other.g, b - other.b);
    }
    dfa cxex ColRgbN operator*(F4 scalar) cx
    {
        ret ColRgbN(r * scalar, g * scalar, b * scalar);
    }
    dfa cxex ColRgbN operator/(F4 scalar) cx
    {
        ret ColRgbN(r / scalar, g / scalar, b / scalar);
    }
    dfa cxex ColRgbN operator-() cx
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

    dfa cxex ColRgba() : r(), g(), b(), a()
    {
    }
    dfa cxex ColRgba(U1 r, U1 g, U1 b, U1 a) : r(r), g(g), b(b), a(a)
    {
    }
    dfa cxex ColRgba(U4 val) : val(val)
    {
    }
    dfa cxex ColRgba(cx ColRgba& other) : val(other.val)
    {
    }
    dfa cxex ColRgba(ColRgba&& other) noex : val(move(other.val))
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
    dfa cxex BO operator==(cx ColRgba& other) cx
    {
        ret val == other.val;
    }
    dfa cxex BO operator!=(cx ColRgba& other) cx
    {
        ret val != other.val;
    }
    dfa cxex ColRgba operator+(cx ColRgba& other) cx
    {
        ret ColRgba(r + other.r, g + other.g, b + other.b, a + other.a);
    }
    dfa cxex ColRgba operator-(cx ColRgba& other) cx
    {
        ret ColRgba(r - other.r, g - other.g, b - other.b, a - other.a);
    }
    dfa cxex ColRgba operator*(F4 scalar) cx
    {
        ret ColRgba(r * scalar, g * scalar, b * scalar, a * scalar);
    }
    dfa cxex ColRgba operator/(F4 scalar) cx
    {
        ret ColRgba(r / scalar, g / scalar, b / scalar, a / scalar);
    }
    dfa cxex ColRgba operator-() cx
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

    dfa cxex ColVN() : v()
    {
    }
    dfa cxex ColVN(F4 v) : v(v)
    {
    }
    dfa cxex ColVN(cx ColVN& other) : v(other.v)
    {
    }
    dfa cxex ColVN(ColVN&& other) noex : v(move(other.v))
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
    dfa cxex BO operator==(cx ColVN& other) cx
    {
        ret v == other.v;
    }
    dfa cxex BO operator!=(cx ColVN& other) cx
    {
        ret v != other.v;
    }
    dfa cxex ColVN operator+(cx ColVN& other) cx
    {
        ret ColVN(v + other.v);
    }
    dfa cxex ColVN operator-(cx ColVN& other) cx
    {
        ret ColVN(v - other.v);
    }
    dfa cxex ColVN operator*(F4 scalar) cx
    {
        ret ColVN(v * scalar);
    }
    dfa cxex ColVN operator/(F4 scalar) cx
    {
        ret ColVN(v / scalar);
    }
    dfa cxex ColVN operator-() cx
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

    dfa cxex BO operator<(cx ColVN& other) cx
    {
        ret v < other.v;
    }
    dfa cxex BO operator>(cx ColVN& other) cx
    {
        ret v > other.v;
    }
    dfa cxex BO operator<=(cx ColVN& other) cx
    {
        ret v <= other.v;
    }
    dfa cxex BO operator>=(cx ColVN& other) cx
    {
        ret v >= other.v;
    }
    dfa cxex ColVN operator*(cx ColVN& other) cx
    {
        ret ColVN(v * other.v);
    }
    dfa cxex ColVN operator/(cx ColVN& other) cx
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

    dfa cxex ColHsvN() : h(), s(), v()
    {
    }
    dfa cxex ColHsvN(F4 h, F4 s, F4 v) : h(h), s(s), v(v)
    {
    }
    dfa cxex ColHsvN(cx ColHsvN& other) : h(other.h), s(other.s), v(other.v)
    {
    }
    dfa cxex ColHsvN(ColHsvN&& other) noex : h(move(other.h)), s(move(other.s)), v(move(other.v))
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
    dfa cxex BO operator==(cx ColHsvN& other) cx
    {
        ret h == other.h && s == other.s && v == other.v;
    }
    dfa cxex BO operator!=(cx ColHsvN& other) cx
    {
        ret h != other.h || s != other.s || v != other.v;
    }
    dfa cxex ColHsvN operator+(cx ColHsvN& other) cx
    {
        ret ColHsvN(h + other.h, s + other.s, v + other.v);
    }
    dfa cxex ColHsvN operator-(cx ColHsvN& other) cx
    {
        ret ColHsvN(h - other.h, s - other.s, v - other.v);
    }
    dfa cxex ColHsvN operator*(F4 scalar) cx
    {
        ret ColHsvN(h * scalar, s * scalar, v * scalar);
    }
    dfa cxex ColHsvN operator/(F4 scalar) cx
    {
        ret ColHsvN(h / scalar, s / scalar, v / scalar);
    }
    dfa cxex ColHsvN operator-() cx
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
    dfa NT Resize(cx Size2<SI>& size)
    {
        tx->size = size;
        cx AU pixelCnt = size.Area();
        if (TO(pixelCnt)(pixels.size()) != pixelCnt)
            pixels.resize(pixelCnt);
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
tpl0 dfa NT ToType(ColRgba& dst, cx ColRgbN& src)
{
    dst.r = U1(src.r * 255.0f);
    dst.g = U1(src.g * 255.0f);
    dst.b = U1(src.b * 255.0f);
    dst.a = 255;
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
    cx AU i = U1(h);
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
tpl0 dfa NT ToType(ColRgba& dst, cx ColHsvN& src)
{
    ColRgbN tmp;
    ToType(tmp, src);
    ToType(dst, tmp);
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
tpl0 dfa NT ToType(ColVN& dst, cx ColVN& src)
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
tpl0 dfa NT ToType(ColRgba& dst, cx ColVN& src)
{
    dst.r = U1(src.v * 255.0f);
    dst.g = U1(src.v * 255.0f);
    dst.b = U1(src.v * 255.0f);
    dst.a = 255;
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
struct ColGridVNCmpInfo
{
    enum class Mode : U1
    {
        LINEAR = 0,
        CHUNK_MAX = 1,
    };

    Mode mode;
    SI longChunkCntSuggest;

    ColGridVNCmpInfo()
    {
        mode = Mode::LINEAR;
        longChunkCntSuggest = 0;
    }
};
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
dfa ER ColGridVNCmp(F4& resultDiff, cx ColGrid<ColVN>& subGrid, cx ColGrid<ColVN>& mainGrid, cx ColGridVNCmpInfo& info)
{
    ifu (subGrid.size != mainGrid.size)
        rete(ErrVal::NO_VALID);

    if (info.mode == ColGridVNCmpInfo::Mode::LINEAR)
    {
        AU diffSum = F8(0);
        cx AU iEnd = SI(subGrid.pixels.size());
        ite (i, i < iEnd)
            diffSum += F8(Diff(subGrid.pixels[i].v, mainGrid.pixels[i].v));
        resultDiff = F4(diffSum / F8(subGrid.pixels.size()));
    }
    else if (info.mode == ColGridVNCmpInfo::Mode::CHUNK_MAX)
    {
        cx AU chunkSize = Size2ChunkSizeSuggest(subGrid.size, info.longChunkCntSuggest, 0.9f, 0.15f);
        cx AU chunkCnt = Size2ChunkCnt(subGrid.size, chunkSize);
        struct Chunk
        {
            F8 diffSum;
            SI diffCnt;

            dfa Chunk() : diffSum(0), diffCnt(0)
            {
            }
        };
        std::vector<Chunk> chunks(chunkCnt.Area());

        cx AU iEnd = SI(subGrid.pixels.size());
        ite (i, i < iEnd)
        {
            AU& chunk = chunks[Size2ChunkI(subGrid.size, chunkCnt, chunkSize, i)];
            chunk.diffSum += F8(Diff(subGrid.pixels[i].v, mainGrid.pixels[i].v));
            ++chunk.diffCnt;
        }

        AU diffMax = F8(0);
        for (cx AU& chunk : chunks)
        {
            cx AU diff = chunk.diffSum / F8(chunk.diffCnt);
            if (diff > diffMax)
                diffMax = diff;
        }

        resultDiff = F4(diffMax);
    }
    else
    {
        rete(ErrVal::NO_SUPPORT);
    }

    rets;
}
dfa ER ColGridHsvNCmp(F4& resultDiff, cx ColGrid<ColHsvN>& subGrid, cx ColGrid<ColHsvN>& mainGrid, cx ColGridHsvNCmpInfo& info)
{
    resultDiff = 1.0f;

    // check pos & size
    ifu (info.subGridPos.x + subGrid.size.w > mainGrid.size.w || info.subGridPos.y + subGrid.size.h > mainGrid.size.h)
    {
        rete(ErrVal::HIGH_SIZE);
    }
    ifu (info.subGridPos.x < 0 || info.subGridPos.y < 0)
    {
        rete(ErrVal::LOW_SIZE);
    }
    ifu ((info.subGridMask != NUL) && (info.subGridMask->size.w != subGrid.size.w || info.subGridMask->size.h != subGrid.size.h))
    {
        rete(ErrVal::NO_VALID);
    }
    ifu ((info.subGridEdges != NUL) && (info.subGridEdges->size.w != subGrid.size.w || info.subGridEdges->size.h != subGrid.size.h))
    {
        rete(ErrVal::NO_VALID);
    }
    ifu ((info.mainGridEdges != NUL) && (info.mainGridEdges->size.w != mainGrid.size.w || info.mainGridEdges->size.h != mainGrid.size.h))
    {
        rete(ErrVal::NO_VALID);
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
tpl2 dfa ER ColGridLerp(ColGrid<T1>& out, cx ColGrid<T1>& a, cx ColGrid<T1>& b, T2 t)
{
    ifu (a.size != b.size)
        rete(ErrVal::NO_VALID);

    out.Resize(a.size);
    cx AU iEnd = SI(a.pixels.size());
    ite (i, i < iEnd)
        out.pixels[i] = Lerp<T1, T2>(a.pixels[i], b.pixels[i], t);

    rets;
}
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
tpl1 dfa NT ColGridEdgeMarkAll(ColGrid<T1>& colGrid, cx T1& colObj, cx T1& colMark)
{
    ite (x, x < colGrid.size.w)
    {
        AU& pixel = colGrid.pixels[x];
        if (pixel == colObj)
            pixel = colMark;
    }

    for (SI y = 1; y < colGrid.size.h - 1; ++y)
    {
        BO isIn = NO;

        for (SI x = 0; x < colGrid.size.w; ++x)
        {
            AU& pixel = colGrid.Pixel(Pos2<SI>(x, y));
            if (isIn)
            {
                if (pixel != colObj)
                {
                    colGrid.Pixel(Pos2<SI>(x - 1, y)) = colMark;
                    isIn = NO;
                }
                else
                {
                    cx AU& pixelTop = colGrid.Pixel(Pos2<SI>(x, y - 1));
                    cx AU& pixelBottom = colGrid.Pixel(Pos2<SI>(x, y + 1));
                    if (((pixelTop != colObj) && (pixelTop != colMark)) || ((pixelBottom != colObj) && (pixelBottom != colMark)))
                        pixel = colMark;
                }
            }
            else if (pixel == colObj)
            {
                pixel = colMark;
                isIn = YES;
            }
        }

        if (isIn == YES)
            colGrid.Pixel(Pos2<SI>(colGrid.size.w - 1, y)) = colMark;
    }

    {
        AU* pixels = colGrid.pixels.data() + (colGrid.size.h - 1) * colGrid.size.w;
        ite (x, x < colGrid.size.w)
        {
            AU& pixel = pixels[x];
            if (pixel == colObj)
                pixel = colMark;
        }
    }
}
tpl1 dfa NT ColGridFloodFillAt(ColGrid<T1>& colGrid, SI& fillCnt, SI fillOrigin, cx T1& colObj, cx T1& colMark, std::vector<SI>& stack)
{
    fillCnt = 0;
    stack.clear();
    stack.emplace_back(fillOrigin);

    while (!stack.empty())
    {
        cx AU curI = stack.back();
        stack.pop_back();

        AU& pixel = colGrid.pixels[curI];

        if (pixel != colObj)
            continue;

        pixel = colMark;
        ++fillCnt;

        cx AU testPixel = [&](SI testI) -> NT {
            if (colGrid.pixels[testI] == colObj)
                stack.emplace_back(testI);
        };

        testPixel(curI - 1);
        testPixel(curI + 1);
        testPixel(curI - colGrid.size.w);
        testPixel(curI + colGrid.size.w);
    }
}
tpl1 dfa NT ColGridFloodFillAt(ColGrid<T1>& colGrid, SI fillOrigin, cx T1& colObj, cx T1& colMark)
{
    ifu (colGrid.pixels[fillOrigin] != colObj)
        ret;

    SI fillCnt;
    std::vector<SI> stack;
    stack.reserve(16);

    ColGridFloodFillAt(colGrid, fillCnt, fillOrigin, colObj, colMark, stack);
}
tpl1 dfa NT ColGridFloodFillAll(ColGrid<T1>& colGrid, std::vector<SI>& fillOrigins, std::vector<SI>& fillCnts, cx T1& colObj, cx T1& colMark)
{
    fillOrigins.clear();
    fillCnts.clear();

    SI fillCnt;
    std::vector<SI> stack;
    stack.reserve(16);

    for (SI y = 1; y < colGrid.size.h - 1; ++y)
    {
        for (SI x = 1; x < colGrid.size.w - 1; ++x)
        {
            cx AU i = x + y * colGrid.size.w;
            ifl (colGrid.pixels[i] != colObj)
                continue;

            ColGridFloodFillAt(colGrid, fillCnt, i, colObj, colMark, stack);
            fillOrigins.emplace_back(i);
            fillCnts.emplace_back(fillCnt);
        }
    }
}

/// [save/load color grid]
tpl1 dfa ER ColGridSaveFile(cx ColGrid<T1>& colGrid, cx CH* path)
{
    cx AU pathExt = PathExtPtr(path);
    ifu (StrCmp(pathExt, L"bmp") != 0)
    {
        rete(ErrVal::NO_SUPPORT);
    }

    FileMem file;
    ife (file.OpenWrite(path))
    {
        rete(ErrVal::FILE);
    }

    cx AU rowSize = AlignBit(colGrid.size.w * siz(ColRgb), siz(U4));
    cx AU imgSize = rowSize * colGrid.size.h;

    BITMAPINFOHEADER infoHdr = {};
    infoHdr.biSize = U4(siz(infoHdr));
    infoHdr.biWidth = S4(colGrid.size.w);
    infoHdr.biHeight = S4(colGrid.size.h);
    infoHdr.biPlanes = 1;
    infoHdr.biBitCount = sizb(ColRgb);
    infoHdr.biCompression = BI_RGB;
    infoHdr.biSizeImage = U4(imgSize);

    BITMAPFILEHEADER fileHdr = {};
    fileHdr.bfType = 0x4D42;
    fileHdr.bfOffBits = U4(siz(fileHdr) + siz(infoHdr));
    fileHdr.bfSize = fileHdr.bfOffBits + U4(imgSize);

    ife (file.Write(&fileHdr, siz(fileHdr)))
        retep;
    ife (file.Write(&infoHdr, siz(infoHdr)))
        retep;

    vector<U1> rowDat(rowSize);
    for (SI y = colGrid.size.h - 1; y >= 0; --y)
    {
        AU curIn = colGrid.pixels.data() + (y * colGrid.size.w);
        AU curOut = rowDat.data();

        ite (x, x < colGrid.size.w)
        {
            cx AU colRgb = ToType<ColRgb, T1>(*curIn++);
            *curOut++ = colRgb.b;
            *curOut++ = colRgb.g;
            *curOut++ = colRgb.r;
        }

        cx AU padSize = rowSize - colGrid.size.w * siz(ColRgb);
        MemSet(curOut, 0, padSize);

        ife (file.Write(rowDat.data(), rowDat.size()))
            retep;
    }

    ife (file.Close())
    {
        rete(ErrVal::FILE);
    }
    rets;
}
tpl1 dfa ER ColGridLoadFile(ColGrid<T1>& colGrid, cx CH* path)
{
    cx AU pathExt = PathExtPtr(path);
    ifu (StrCmp(pathExt, L"bmp") != 0)
    {
        rete(ErrVal::NO_SUPPORT);
    }

    FileMem file;
    ife (file.OpenRead(path))
    {
        rete(ErrVal::FILE);
    }

    BITMAPFILEHEADER fileHdr = {};
    ife (file.Read(&fileHdr, siz(fileHdr)))
        retep;

    ifu (fileHdr.bfType != 0x4D42)
    {
        rete(ErrVal::FILE);
    }

    BITMAPINFOHEADER infoHdr = {};
    ife (file.Read(&infoHdr, siz(infoHdr)))
        retep;

    ifu (infoHdr.biPlanes != 1 || infoHdr.biBitCount != sizb(ColRgb) || infoHdr.biCompression != BI_RGB)
    {
        rete(ErrVal::NO_SUPPORT);
    }
    cx AU imgW = SI(infoHdr.biWidth);
    cx AU imgH = SI(Abs(infoHdr.biHeight));
    cx AU isTopDown = (infoHdr.biHeight < 0);
    cx AU rowSize = AlignBit(imgW * siz(ColRgb), siz(U4));

    colGrid.size.w = imgW;
    colGrid.size.h = imgH;
    colGrid.pixels.resize(imgW * imgH);

    ife (file.CurSet(SI(fileHdr.bfOffBits)))
        retep;

    vector<U1> rowDat(rowSize);
    ite (i, i < imgH)
    {
        ife (file.Read(rowDat.data(), rowDat.size()))
            retep;

        AU curIn = rowDat.data();
        AU curOut = colGrid.pixels.data() + ((isTopDown ? i : (imgH - 1 - i)) * imgW);

        ite (x, x < imgW)
        {
            ToType<T1, ColRgb>(*curOut++, ColRgb(*(curIn + 2), *(curIn + 1), *curIn));
            curIn += siz(ColRgb);
        }
    }

    ife (file.Close())
    {
        rete(ErrVal::FILE);
    }
    rets;
}
