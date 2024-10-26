#pragma once

/// [value]
constexpr F4 COL_HSV_WEIGHT_H = 0.5f;
constexpr F4 COL_HSV_WEIGHT_S = 0.3f;
constexpr F4 COL_HSV_WEIGHT_V = 0.2f;

/// [color]
using ColVN = F4; // range: 0.0 - 1.0
struct ColRgb
{
    U1 r; // range: 0 - 255
    U1 g; // range: 0 - 255
    U1 b; // range: 0 - 255

    dfa ColRgb operator+(cx ColRgb& col) cx
    {
        ret ColRgb(r + col.r, g + col.g, b + col.b);
    }
    dfa ColRgb operator-(cx ColRgb& col) cx
    {
        ret ColRgb(r - col.r, g - col.g, b - col.b);
    }
    dfa ColRgb operator*(cx ColRgb& col) cx
    {
        ret ColRgb(r * col.r, g * col.g, b * col.b);
    }
    dfa ColRgb operator/(cx ColRgb& col) cx
    {
        ret ColRgb(r / col.r, g / col.g, b / col.b);
    }
    dfa ColRgb operator+(U1 val) cx
    {
        ret ColRgb(r + val, g + val, b + val);
    }
    dfa ColRgb operator-(U1 val) cx
    {
        ret ColRgb(r - val, g - val, b - val);
    }
    dfa ColRgb operator*(U1 val) cx
    {
        ret ColRgb(r * val, g * val, b * val);
    }
    dfa ColRgb operator/(U1 val) cx
    {
        ret ColRgb(r / val, g / val, b / val);
    }
    dfa ColRgb& operator+=(cx ColRgb& col)
    {
        r += col.r;
        g += col.g;
        b += col.b;
        ret *tx;
    }
    dfa ColRgb& operator-=(cx ColRgb& col)
    {
        r -= col.r;
        g -= col.g;
        b -= col.b;
        ret *tx;
    }
    dfa ColRgb& operator*=(cx ColRgb& col)
    {
        r *= col.r;
        g *= col.g;
        b *= col.b;
        ret *tx;
    }
    dfa ColRgb& operator/=(cx ColRgb& col)
    {
        r /= col.r;
        g /= col.g;
        b /= col.b;
        ret *tx;
    }
    dfa ColRgb& operator+=(U1 val)
    {
        r += val;
        g += val;
        b += val;
        ret *tx;
    }
    dfa ColRgb& operator-=(U1 val)
    {
        r -= val;
        g -= val;
        b -= val;
        ret *tx;
    }
    dfa ColRgb& operator*=(U1 val)
    {
        r *= val;
        g *= val;
        b *= val;
        ret *tx;
    }
    dfa ColRgb& operator/=(U1 val)
    {
        r /= val;
        g /= val;
        b /= val;
        ret *tx;
    }

    dfa ColRgb()
    {
    }
    dfa ColRgb(U1 r, U1 g, U1 b) : r(r), g(g), b(b)
    {
    }
};
struct ColRgbN
{
    F4 r; // range: 0.0 - 1.0
    F4 g; // range: 0.0 - 1.0
    F4 b; // range: 0.0 - 1.0

    dfa ColRgbN operator+(cx ColRgbN& col) cx
    {
        ret ColRgbN(r + col.r, g + col.g, b + col.b);
    }
    dfa ColRgbN operator-(cx ColRgbN& col) cx
    {
        ret ColRgbN(r - col.r, g - col.g, b - col.b);
    }
    dfa ColRgbN operator*(cx ColRgbN& col) cx
    {
        ret ColRgbN(r * col.r, g * col.g, b * col.b);
    }
    dfa ColRgbN operator/(cx ColRgbN& col) cx
    {
        ret ColRgbN(r / col.r, g / col.g, b / col.b);
    }
    dfa ColRgbN operator+(F4 val) cx
    {
        ret ColRgbN(r + val, g + val, b + val);
    }
    dfa ColRgbN operator-(F4 val) cx
    {
        ret ColRgbN(r - val, g - val, b - val);
    }
    dfa ColRgbN operator*(F4 val) cx
    {
        ret ColRgbN(r * val, g * val, b * val);
    }
    dfa ColRgbN operator/(F4 val) cx
    {
        ret ColRgbN(r / val, g / val, b / val);
    }
    dfa ColRgbN& operator+=(cx ColRgbN& col)
    {
        r += col.r;
        g += col.g;
        b += col.b;
        ret *tx;
    }
    dfa ColRgbN& operator-=(cx ColRgbN& col)
    {
        r -= col.r;
        g -= col.g;
        b -= col.b;
        ret *tx;
    }
    dfa ColRgbN& operator*=(cx ColRgbN& col)
    {
        r *= col.r;
        g *= col.g;
        b *= col.b;
        ret *tx;
    }
    dfa ColRgbN& operator/=(cx ColRgbN& col)
    {
        r /= col.r;
        g /= col.g;
        b /= col.b;
        ret *tx;
    }
    dfa ColRgbN& operator+=(F4 val)
    {
        r += val;
        g += val;
        b += val;
        ret *tx;
    }
    dfa ColRgbN& operator-=(F4 val)
    {
        r -= val;
        g -= val;
        b -= val;
        ret *tx;
    }
    dfa ColRgbN& operator*=(F4 val)
    {
        r *= val;
        g *= val;
        b *= val;
        ret *tx;
    }
    dfa ColRgbN& operator/=(F4 val)
    {
        r /= val;
        g /= val;
        b /= val;
        ret *tx;
    }

    dfa ColRgbN& Set(cx ColRgb& rgb)
    {
        r = F4(rgb.r) / 255.0f;
        g = F4(rgb.g) / 255.0f;
        b = F4(rgb.b) / 255.0f;
        ret *tx;
    }

    dfa ColRgbN()
    {
    }
    dfa ColRgbN(F4 r, F4 g, F4 b) : r(r), g(g), b(b)
    {
    }
    dfa ColRgbN(cx ColRgb& rgb)
    {
        tx->Set(rgb);
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

    dfa ColRgba operator+(cx ColRgba& col) cx
    {
        ret ColRgba(r + col.r, g + col.g, b + col.b, a + col.a);
    }
    dfa ColRgba operator-(cx ColRgba& col) cx
    {
        ret ColRgba(r - col.r, g - col.g, b - col.b, a - col.a);
    }
    dfa ColRgba operator*(cx ColRgba& col) cx
    {
        ret ColRgba(r * col.r, g * col.g, b * col.b, a * col.a);
    }
    dfa ColRgba operator/(cx ColRgba& col) cx
    {
        ret ColRgba(r / col.r, g / col.g, b / col.b, a / col.a);
    }
    dfa ColRgba operator+(U1 val) cx
    {
        ret ColRgba(r + val, g + val, b + val, a + val);
    }
    dfa ColRgba operator-(U1 val) cx
    {
        ret ColRgba(r - val, g - val, b - val, a - val);
    }
    dfa ColRgba operator*(F4 val) cx
    {
        ret ColRgba(U1(r * val), U1(g * val), U1(b * val), U1(a * val));
    }
    dfa ColRgba operator/(F4 val) cx
    {
        ret ColRgba(U1(r / val), U1(g / val), U1(b / val), U1(a / val));
    }
    dfa ColRgba& operator+=(cx ColRgba& col)
    {
        r += col.r;
        g += col.g;
        b += col.b;
        a += col.a;
        ret *tx;
    }
    dfa ColRgba& operator-=(cx ColRgba& col)
    {
        r -= col.r;
        g -= col.g;
        b -= col.b;
        a -= col.a;
        ret *tx;
    }
    dfa ColRgba& operator*=(cx ColRgba& col)
    {
        r *= col.r;
        g *= col.g;
        b *= col.b;
        a *= col.a;
        ret *tx;
    }
    dfa ColRgba& operator/=(cx ColRgba& col)
    {
        r /= col.r;
        g /= col.g;
        b /= col.b;
        a /= col.a;
        ret *tx;
    }
    dfa ColRgba& operator+=(U1 val)
    {
        r += val;
        g += val;
        b += val;
        a += val;
        ret *tx;
    }
    dfa ColRgba& operator-=(U1 val)
    {
        r -= val;
        g -= val;
        b -= val;
        a -= val;
        ret *tx;
    }
    dfa ColRgba& operator*=(U1 val)
    {
        r *= val;
        g *= val;
        b *= val;
        a *= val;
        ret *tx;
    }
    dfa ColRgba& operator/=(U1 val)
    {
        r /= val;
        g /= val;
        b /= val;
        a /= val;
        ret *tx;
    }

    dfa ColRgba& operator=(cx ColRgba& col)
    {
        val = col.val;
        ret *tx;
    }

    dfa ColRgba()
    {
    }
    dfa ColRgba(U1 r, U1 g, U1 b, U1 a) : r(r), g(g), b(b), a(a)
    {
    }
    dfa ColRgba(U4 val) : val(val)
    {
    }
};
struct ColHsvN
{
    F4 h; // range: 0.0 - 1.0
    F4 s; // range: 0.0 - 1.0
    F4 v; // range: 0.0 - 1.0

    dfa ColHsvN operator+(cx ColHsvN& col) cx
    {
        ret ColHsvN(h + col.h, s + col.s, v + col.v);
    }
    dfa ColHsvN operator-(cx ColHsvN& col) cx
    {
        ret ColHsvN(h - col.h, s - col.s, v - col.v);
    }
    dfa ColHsvN operator*(cx ColHsvN& col) cx
    {
        ret ColHsvN(h * col.h, s * col.s, v * col.v);
    }
    dfa ColHsvN operator/(cx ColHsvN& col) cx
    {
        ret ColHsvN(h / col.h, s / col.s, v / col.v);
    }
    dfa ColHsvN operator+(F4 val) cx
    {
        ret ColHsvN(h + val, s + val, v + val);
    }
    dfa ColHsvN operator-(F4 val) cx
    {
        ret ColHsvN(h - val, s - val, v - val);
    }
    dfa ColHsvN operator*(F4 val) cx
    {
        ret ColHsvN(h * val, s * val, v * val);
    }
    dfa ColHsvN operator/(F4 val) cx
    {
        ret ColHsvN(h / val, s / val, v / val);
    }
    dfa ColHsvN& operator+=(cx ColHsvN& col)
    {
        h += col.h;
        s += col.s;
        v += col.v;
        ret *tx;
    }
    dfa ColHsvN& operator-=(cx ColHsvN& col)
    {
        h -= col.h;
        s -= col.s;
        v -= col.v;
        ret *tx;
    }
    dfa ColHsvN& operator*=(cx ColHsvN& col)
    {
        h *= col.h;
        s *= col.s;
        v *= col.v;
        ret *tx;
    }
    dfa ColHsvN& operator/=(cx ColHsvN& col)
    {
        h /= col.h;
        s /= col.s;
        v /= col.v;
        ret *tx;
    }
    dfa ColHsvN& operator+=(F4 val)
    {
        h += val;
        s += val;
        v += val;
        ret *tx;
    }
    dfa ColHsvN& operator-=(F4 val)
    {
        h -= val;
        s -= val;
        v -= val;
        ret *tx;
    }
    dfa ColHsvN& operator*=(F4 val)
    {
        h *= val;
        s *= val;
        v *= val;
        ret *tx;
    }
    dfa ColHsvN& operator/=(F4 val)
    {
        h /= val;
        s /= val;
        v /= val;
        ret *tx;
    }

    dfa ColHsvN()
    {
    }
    dfa ColHsvN(F4 h, F4 s, F4 v) : h(h), s(s), v(v)
    {
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
dfa ColVN ColRgbNToVNLumi(cx ColRgbN& rgbN)
{
    ret rgbN.r * 0.299f + rgbN.g * 0.587f + rgbN.b * 0.114f;
}
dfa ColVN ColRgbToVNLumi(cx ColRgb& rgb)
{
    ret ColRgbNToVNLumi(ColRgbN(rgb));
}
dfa NT ColRgbNToHsvN(ColHsvN& hsvN, cx ColRgbN& rgbN)
{
    cx AU max = Max(rgbN.r, rgbN.g, rgbN.b);
    hsvN.v = max;
    if (max < 0.00001f)
    {
        hsvN.h = 0.0f;
        hsvN.s = 0.0f;
        ret;
    }

    cx AU min = Min(rgbN.r, rgbN.g, rgbN.b);
    cx AU delta = max - min;
    if (delta < 0.00001f)
    {
        hsvN.h = 0.0f;
        hsvN.s = 0.0f;
        ret;
    }
    hsvN.s = delta / max;

    if (rgbN.r >= max)
    {
        hsvN.h = (rgbN.g - rgbN.b) / delta;
    }
    else if (rgbN.g >= max)
    {
        hsvN.h = (rgbN.b - rgbN.r) / delta + 2.0f;
    }
    else
    {
        hsvN.h = (rgbN.r - rgbN.g) / delta + 4.0f;
    }
    hsvN.h /= 6.0f;
    if (hsvN.h < 0.0f)
    {
        hsvN.h += 1.0f;
    }
}
dfa NT ColRgbToHsvN(ColHsvN& hsvN, cx ColRgb& rgb)
{
    ColRgbNToHsvN(hsvN, ColRgbN(rgb));
}
dfa NT ColHsvNToRgbN(ColRgbN& rgbN, cx ColHsvN& hsvN)
{
    if (hsvN.s < 0.00001f)
    {
        rgbN.r = hsvN.v;
        rgbN.g = hsvN.v;
        rgbN.b = hsvN.v;
        ret;
    }

    AU h = hsvN.h * 6.0f;
    if (h >= 6.0f)
    {
        h = 0.0f;
    }
    cx AU i = h;
    cx AU f = h - i;
    cx AU p = hsvN.v * (1.0f - hsvN.s);
    cx AU q = hsvN.v * (1.0f - hsvN.s * f);
    cx AU t = hsvN.v * (1.0f - hsvN.s * (1.0f - f));

    switch (SI(i))
    {
    case 0:
        rgbN.r = hsvN.v;
        rgbN.g = t;
        rgbN.b = p;
        break;
    case 1:
        rgbN.r = q;
        rgbN.g = hsvN.v;
        rgbN.b = p;
        break;
    case 2:
        rgbN.r = p;
        rgbN.g = hsvN.v;
        rgbN.b = t;
        break;
    case 3:
        rgbN.r = p;
        rgbN.g = q;
        rgbN.b = hsvN.v;
        break;
    case 4:
        rgbN.r = t;
        rgbN.g = p;
        rgbN.b = hsvN.v;
        break;
    case 5:
        rgbN.r = hsvN.v;
        rgbN.g = p;
        rgbN.b = q;
        break;
    default:
        rgbN.r = 0.0f;
        rgbN.g = 0.0f;
        rgbN.b = 0.0f;
        break;
    }
}
dfa NT ColVNToRgbN(ColRgbN& rgbN, cx ColVN& vN)
{
    rgbN.r = vN;
    rgbN.g = vN;
    rgbN.b = vN;
}

/// [convert color grid]
dfa NT ColGridRgbToRgbN(ColGrid<ColRgbN>& rgbN, cx ColGrid<ColRgb>& rgb)
{
    rgbN.size = rgb.size;
    rgbN.pixels.resize(rgb.pixels.size());
    cx AU iEnd = SI(rgb.pixels.size());
    ite (i, i < iEnd)
    {
        rgbN.pixels[i].Set(rgb.pixels[i]);
    }
}
dfa NT ColGridRgbNToVNLumi(ColGrid<ColVN>& vN, cx ColGrid<ColRgbN>& rgbN)
{
    vN.size = rgbN.size;
    vN.pixels.resize(rgbN.pixels.size());
    cx AU iEnd = SI(rgbN.pixels.size());
    ite (i, i < iEnd)
    {
        vN.pixels[i] = ColRgbNToVNLumi(rgbN.pixels[i]);
    }
}
dfa NT ColGridRgbToVNLumi(ColGrid<ColVN>& vN, cx ColGrid<ColRgb>& rgb)
{
    vN.size = rgb.size;
    vN.pixels.resize(rgb.pixels.size());
    cx AU iEnd = SI(rgb.pixels.size());
    ite (i, i < iEnd)
    {
        vN.pixels[i] = ColRgbToVNLumi(rgb.pixels[i]);
    }
}
dfa NT ColGridRgbNToHsvN(ColGrid<ColHsvN>& hsvN, cx ColGrid<ColRgbN>& rgbN)
{
    hsvN.size = rgbN.size;
    hsvN.pixels.resize(rgbN.pixels.size());
    cx AU iEnd = SI(rgbN.pixels.size());
    ite (i, i < iEnd)
    {
        ColRgbNToHsvN(hsvN.pixels[i], rgbN.pixels[i]);
    }
}
dfa NT ColGridRgbToHsvN(ColGrid<ColHsvN>& hsvN, cx ColGrid<ColRgb>& rgb)
{
    hsvN.size = rgb.size;
    hsvN.pixels.resize(rgb.pixels.size());
    cx AU iEnd = SI(rgb.pixels.size());
    ite (i, i < iEnd)
    {
        ColRgbToHsvN(hsvN.pixels[i], rgb.pixels[i]);
    }
}
dfa NT ColGridVNToRgbN(ColGrid<ColRgbN>& rgbN, cx ColGrid<ColVN>& vN)
{
    rgbN.size = vN.size;
    rgbN.pixels.resize(vN.pixels.size());
    cx AU iEnd = SI(vN.pixels.size());
    ite (i, i < iEnd)
    {
        ColVNToRgbN(rgbN.pixels[i], vN.pixels[i]);
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
            cx AU& maskVal = info.subGridMask->pixels[i];
            diffSum += diff * info.wCol * maskVal;
            diffCnt += maskVal;
        }
        if (doEdgesProc)
        {
            cx AU diffEdge = Diff(info.subGridEdges->pixels[i], info.mainGridEdges->pixels[iMain]);
            if (info.subGridMask == NUL)
            {
                diffSum += diffEdge * info.wEdges;
            }
            else
            {
                cx AU& maskVal = info.subGridMask->pixels[i];
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
            cx AU p00 = in.pixels[(y - 1) * w + (x - 1)];
            cx AU p01 = in.pixels[(y - 1) * w + x];
            cx AU p02 = in.pixels[(y - 1) * w + (x + 1)];
            cx AU p10 = in.pixels[y * w + (x - 1)];
            cx AU p12 = in.pixels[y * w + (x + 1)];
            cx AU p20 = in.pixels[(y + 1) * w + (x - 1)];
            cx AU p21 = in.pixels[(y + 1) * w + x];
            cx AU p22 = in.pixels[(y + 1) * w + (x + 1)];

            cx AU gX = (-p00) + p02 + (-2.0f * p10) + (2.0f * p12) + (-p20) + p22;
            cx AU gY = (-p00) + (-2.0f * p01) + (-p02) + p20 + (2.0f * p21) + p22;

            out.pixels[y * w + x] = Dist0(gX, gY);
        }
    }

    // set borders to zero
    for (SI x = 0; x < w; ++x)
    {
        out.pixels[x] = 0;
        out.pixels[(h - 1) * w + x] = 0;
    }
    for (SI y = 0; y < h; ++y)
    {
        out.pixels[y * w] = 0;
        out.pixels[y * w + (w - 1)] = 0;
    }

    NormalizeMax<ColVN>(out.pixels.data(), out.pixels.size());
}
