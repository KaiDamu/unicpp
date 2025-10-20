#include "color.hpp"

#include "math.hpp"

dfa ColRgb::ColRgb() : r(), g(), b()
{
}
dfa ColRgb::ColRgb(U1 r, U1 g, U1 b) : r(r), g(g), b(b)
{
}
dfa BO ColRgb::operator==(cx ColRgb& other) cx
{
    ret r == other.r && g == other.g && b == other.b;
}
dfa BO ColRgb::operator!=(cx ColRgb& other) cx
{
    ret r != other.r || g != other.g || b != other.b;
}
dfa ColRgb ColRgb::operator+(cx ColRgb& other) cx
{
    ret ColRgb(r + other.r, g + other.g, b + other.b);
}
dfa ColRgb ColRgb::operator-(cx ColRgb& other) cx
{
    ret ColRgb(r - other.r, g - other.g, b - other.b);
}
dfa ColRgb ColRgb::operator*(F4 scalar) cx
{
    ret ColRgb(U1(r * scalar), U1(g * scalar), U1(b * scalar));
}
dfa ColRgb ColRgb::operator/(F4 scalar) cx
{
    ret ColRgb(U1(r / scalar), U1(g / scalar), U1(b / scalar));
}
dfa ColRgb ColRgb::operator-() cx
{
    ret ColRgb(-r, -g, -b);
}
dfa ColRgb& ColRgb::operator+=(cx ColRgb& other)
{
    r += other.r;
    g += other.g;
    b += other.b;
    ret *tx;
}
dfa ColRgb& ColRgb::operator-=(cx ColRgb& other)
{
    r -= other.r;
    g -= other.g;
    b -= other.b;
    ret *tx;
}
dfa ColRgb& ColRgb::operator*=(F4 scalar)
{
    r = U1(r * scalar);
    g = U1(g * scalar);
    b = U1(b * scalar);
    ret *tx;
}
dfa ColRgb& ColRgb::operator/=(F4 scalar)
{
    r = U1(r / scalar);
    g = U1(g / scalar);
    b = U1(b / scalar);
    ret *tx;
}

dfa ColRgbN::ColRgbN() : r(), g(), b()
{
}
dfa ColRgbN::ColRgbN(F4 r, F4 g, F4 b) : r(r), g(g), b(b)
{
}
dfa BO ColRgbN::operator==(cx ColRgbN& other) cx
{
    ret r == other.r && g == other.g && b == other.b;
}
dfa BO ColRgbN::operator!=(cx ColRgbN& other) cx
{
    ret r != other.r || g != other.g || b != other.b;
}
dfa ColRgbN ColRgbN::operator+(cx ColRgbN& other) cx
{
    ret ColRgbN(r + other.r, g + other.g, b + other.b);
}
dfa ColRgbN ColRgbN::operator-(cx ColRgbN& other) cx
{
    ret ColRgbN(r - other.r, g - other.g, b - other.b);
}
dfa ColRgbN ColRgbN::operator*(F4 scalar) cx
{
    ret ColRgbN(r * scalar, g * scalar, b * scalar);
}
dfa ColRgbN ColRgbN::operator/(F4 scalar) cx
{
    ret ColRgbN(r / scalar, g / scalar, b / scalar);
}
dfa ColRgbN ColRgbN::operator-() cx
{
    ret ColRgbN(-r, -g, -b);
}
dfa ColRgbN& ColRgbN::operator+=(cx ColRgbN& other)
{
    r += other.r;
    g += other.g;
    b += other.b;
    ret *tx;
}
dfa ColRgbN& ColRgbN::operator-=(cx ColRgbN& other)
{
    r -= other.r;
    g -= other.g;
    b -= other.b;
    ret *tx;
}
dfa ColRgbN& ColRgbN::operator*=(F4 scalar)
{
    r *= scalar;
    g *= scalar;
    b *= scalar;
    ret *tx;
}
dfa ColRgbN& ColRgbN::operator/=(F4 scalar)
{
    r /= scalar;
    g /= scalar;
    b /= scalar;
    ret *tx;
}

dfa ColRgba::ColRgba() : r(), g(), b(), a()
{
}
dfa ColRgba::ColRgba(U1 r, U1 g, U1 b, U1 a) : r(r), g(g), b(b), a(a)
{
}
dfa ColRgba::ColRgba(U4 val) : val(val)
{
}
dfa BO ColRgba::operator==(cx ColRgba& other) cx
{
    ret val == other.val;
}
dfa BO ColRgba::operator!=(cx ColRgba& other) cx
{
    ret val != other.val;
}
dfa ColRgba ColRgba::operator+(cx ColRgba& other) cx
{
    ret ColRgba(r + other.r, g + other.g, b + other.b, a + other.a);
}
dfa ColRgba ColRgba::operator-(cx ColRgba& other) cx
{
    ret ColRgba(r - other.r, g - other.g, b - other.b, a - other.a);
}
dfa ColRgba ColRgba::operator*(F4 scalar) cx
{
    ret ColRgba(U1(r * scalar), U1(g * scalar), U1(b * scalar), U1(a * scalar));
}
dfa ColRgba ColRgba::operator/(F4 scalar) cx
{
    ret ColRgba(U1(r / scalar), U1(g / scalar), U1(b / scalar), U1(a / scalar));
}
dfa ColRgba ColRgba::operator-() cx
{
    ret ColRgba(-r, -g, -b, -a);
}
dfa ColRgba& ColRgba::operator+=(cx ColRgba& other)
{
    r += other.r;
    g += other.g;
    b += other.b;
    a += other.a;
    ret *tx;
}
dfa ColRgba& ColRgba::operator-=(cx ColRgba& other)
{
    r -= other.r;
    g -= other.g;
    b -= other.b;
    a -= other.a;
    ret *tx;
}
dfa ColRgba& ColRgba::operator*=(F4 scalar)
{
    r = U1(r * scalar);
    g = U1(g * scalar);
    b = U1(b * scalar);
    a = U1(a * scalar);
    ret *tx;
}
dfa ColRgba& ColRgba::operator/=(F4 scalar)
{
    r = U1(r / scalar);
    g = U1(g / scalar);
    b = U1(b / scalar);
    a = U1(a / scalar);
    ret *tx;
}

dfa ColV::ColV() : v()
{
}
dfa ColV::ColV(U1 v) : v(v)
{
}
dfa BO ColV::operator==(cx ColV& other) cx
{
    ret v == other.v;
}
dfa BO ColV::operator!=(cx ColV& other) cx
{
    ret v != other.v;
}
dfa ColV ColV::operator+(cx ColV& other) cx
{
    ret ColV(v + other.v);
}
dfa ColV ColV::operator-(cx ColV& other) cx
{
    ret ColV(v - other.v);
}
dfa ColV ColV::operator*(F4 scalar) cx
{
    ret ColV(U1(v * scalar));
}
dfa ColV ColV::operator/(F4 scalar) cx
{
    ret ColV(U1(v / scalar));
}
dfa ColV ColV::operator-() cx
{
    ret ColV(-v);
}
dfa ColV& ColV::operator+=(cx ColV& other)
{
    v += other.v;
    ret *tx;
}
dfa ColV& ColV::operator-=(cx ColV& other)
{
    v -= other.v;
    ret *tx;
}
dfa ColV& ColV::operator*=(F4 scalar)
{
    v = U1(v * scalar);
    ret *tx;
}
dfa ColV& ColV::operator/=(F4 scalar)
{
    v = U1(v / scalar);
    ret *tx;
}
dfa BO ColV::operator<(cx ColV& other) cx
{
    ret v < other.v;
}
dfa BO ColV::operator>(cx ColV& other) cx
{
    ret v > other.v;
}
dfa BO ColV::operator<=(cx ColV& other) cx
{
    ret v <= other.v;
}
dfa BO ColV::operator>=(cx ColV& other) cx
{
    ret v >= other.v;
}
dfa ColV ColV::operator*(cx ColV& other) cx
{
    ret ColV(v * other.v);
}
dfa ColV ColV::operator/(cx ColV& other) cx
{
    ret ColV(v / other.v);
}
dfa ColV& ColV::operator*=(cx ColV& other)
{
    v *= other.v;
    ret *tx;
}
dfa ColV& ColV::operator/=(cx ColV& other)
{
    v /= other.v;
    ret *tx;
}

dfa ColVN::ColVN() : v()
{
}
dfa ColVN::ColVN(F4 v) : v(v)
{
}
dfa BO ColVN::operator==(cx ColVN& other) cx
{
    ret v == other.v;
}
dfa BO ColVN::operator!=(cx ColVN& other) cx
{
    ret v != other.v;
}
dfa ColVN ColVN::operator+(cx ColVN& other) cx
{
    ret ColVN(v + other.v);
}
dfa ColVN ColVN::operator-(cx ColVN& other) cx
{
    ret ColVN(v - other.v);
}
dfa ColVN ColVN::operator*(F4 scalar) cx
{
    ret ColVN(v * scalar);
}
dfa ColVN ColVN::operator/(F4 scalar) cx
{
    ret ColVN(v / scalar);
}
dfa ColVN ColVN::operator-() cx
{
    ret ColVN(-v);
}
dfa ColVN& ColVN::operator+=(cx ColVN& other)
{
    v += other.v;
    ret *tx;
}
dfa ColVN& ColVN::operator-=(cx ColVN& other)
{
    v -= other.v;
    ret *tx;
}
dfa ColVN& ColVN::operator*=(F4 scalar)
{
    v *= scalar;
    ret *tx;
}
dfa ColVN& ColVN::operator/=(F4 scalar)
{
    v /= scalar;
    ret *tx;
}
dfa BO ColVN::operator<(cx ColVN& other) cx
{
    ret v < other.v;
}
dfa BO ColVN::operator>(cx ColVN& other) cx
{
    ret v > other.v;
}
dfa BO ColVN::operator<=(cx ColVN& other) cx
{
    ret v <= other.v;
}
dfa BO ColVN::operator>=(cx ColVN& other) cx
{
    ret v >= other.v;
}
dfa ColVN ColVN::operator*(cx ColVN& other) cx
{
    ret ColVN(v * other.v);
}
dfa ColVN ColVN::operator/(cx ColVN& other) cx
{
    ret ColVN(v / other.v);
}
dfa ColVN& ColVN::operator*=(cx ColVN& other)
{
    v *= other.v;
    ret *tx;
}
dfa ColVN& ColVN::operator/=(cx ColVN& other)
{
    v /= other.v;
    ret *tx;
}

dfa ColHsvN::ColHsvN() : h(), s(), v()
{
}
dfa ColHsvN::ColHsvN(F4 h, F4 s, F4 v) : h(h), s(s), v(v)
{
}
dfa BO ColHsvN::operator==(cx ColHsvN& other) cx
{
    ret h == other.h && s == other.s && v == other.v;
}
dfa BO ColHsvN::operator!=(cx ColHsvN& other) cx
{
    ret h != other.h || s != other.s || v != other.v;
}
dfa ColHsvN ColHsvN::operator+(cx ColHsvN& other) cx
{
    ret ColHsvN(h + other.h, s + other.s, v + other.v);
}
dfa ColHsvN ColHsvN::operator-(cx ColHsvN& other) cx
{
    ret ColHsvN(h - other.h, s - other.s, v - other.v);
}
dfa ColHsvN ColHsvN::operator*(F4 scalar) cx
{
    ret ColHsvN(h * scalar, s * scalar, v * scalar);
}
dfa ColHsvN ColHsvN::operator/(F4 scalar) cx
{
    ret ColHsvN(h / scalar, s / scalar, v / scalar);
}
dfa ColHsvN ColHsvN::operator-() cx
{
    ret ColHsvN(-h, -s, -v);
}
dfa ColHsvN& ColHsvN::operator+=(cx ColHsvN& other)
{
    h += other.h;
    s += other.s;
    v += other.v;
    ret *tx;
}
dfa ColHsvN& ColHsvN::operator-=(cx ColHsvN& other)
{
    h -= other.h;
    s -= other.s;
    v -= other.v;
    ret *tx;
}
dfa ColHsvN& ColHsvN::operator*=(F4 scalar)
{
    h *= scalar;
    s *= scalar;
    v *= scalar;
    ret *tx;
}
dfa ColHsvN& ColHsvN::operator/=(F4 scalar)
{
    h /= scalar;
    s /= scalar;
    v /= scalar;
    ret *tx;
}

dfa F4 ColHsvNCmp(cx ColHsvN& a, cx ColHsvN& b, F4 wH, F4 wS, F4 wV)
{
    ret (DiffWrap(a.h, b.h, 1.0f) * 2.0f * wH) + (Diff(a.s, b.s) * wS) + (Diff(a.v, b.v) * wV);
}

dfa NT ColGridSsimInfo::Clr()
{
    imgCol = NUL;
    imgMask = NUL;
    winSize = 0;
    stepSize = 0;
    wins.clear();
}
dfa ColGridSsimInfo::ColGridSsimInfo()
{
    tx->Clr();
}

dfa NT ColGridSsimInfoGet(ColGridSsimInfo& result, cx ColGrid<ColVN>& imgCol, cx ColGrid<ColVN>& imgMask, SI winSize, SI stepSize)
{
    if (winSize == 0)
    {
        cx AU minSize = Min<SI>(imgCol.size.w, imgCol.size.h);
        winSize = Min(Clamp<SI>(RoundToInt(minSize * 0.05f), 7, 17), minSize);
    }
    if (stepSize == 0)
        stepSize = Max<SI>((winSize + 1) / 2, 1);

    result.Clr();
    result.imgCol = &imgCol;
    result.imgMask = &imgMask;
    result.winSize = winSize;
    result.stepSize = stepSize;

    cx AU& imgW = imgCol.size.w;
    cx AU& imgH = imgCol.size.h;

    result.wins.reserve((imgW / stepSize + 1) * (imgH / stepSize + 1));

    for (SI y = 0; y < imgH; y += stepSize)
        for (SI x = 0; x < imgW; x += stepSize)
        {
            cx AU xEnd = Min(x + winSize, imgW);
            cx AU yEnd = Min(y + winSize, imgH);
            F4 mean = 0;
            F4 meanSq = 0;
            F4 weightSum = 0;

            for (SI j = y; j < yEnd; ++j)
            {
                cx AU iBase = j * imgW + x;
                for (SI i = 0; i < xEnd - x; ++i)
                {
                    cx AU& val = imgCol.pixels[iBase + i].v;
                    cx AU& weight = imgMask.pixels[iBase + i].v;
                    cx AU valMulWeight = val * weight;
                    mean += valMulWeight;
                    meanSq += valMulWeight * val;
                    weightSum += weight;
                }
            }
            if (IsNearZero(weightSum))
            {
                result.wins.emplace_back(0, 0);
                continue;
            }

            mean /= weightSum;
            cx AU variance = Max(meanSq / weightSum - mean * mean, F4(0));
            result.wins.emplace_back(mean, variance);
        }
}
dfa ER ColGridCmpSsim(F4& resultDiff, cx ColGridSsimInfo& infoA, cx ColGridSsimInfo& infoB)
{
    resultDiff = 1.0f;

    ifu (infoA.imgCol->size != infoB.imgCol->size)
        rete(ErrVal::NO_VALID);
    ifu (infoA.imgMask != infoB.imgMask)
        rete(ErrVal::NO_VALID);
    ifu (infoA.winSize != infoB.winSize)
        rete(ErrVal::NO_VALID);
    ifu (infoA.stepSize != infoB.stepSize)
        rete(ErrVal::NO_VALID);

    cx AU& imgW = infoA.imgCol->size.w;
    cx AU& imgH = infoA.imgCol->size.h;
    cx AU& imgMask = *infoA.imgMask;
    cx AU& winSize = infoA.winSize;
    cx AU& stepSize = infoA.stepSize;

    cxex AU L = 1.0f - 0.0f;
    cxex AU C1 = (0.01f * L) * (0.01f * L);
    cxex AU C2 = (0.03f * L) * (0.03f * L);

    F4 ssimSum = 0;
    F4 weightSumAll = 0;
    SI winI = 0;

    for (SI y = 0; y < imgH; y += stepSize)
        for (SI x = 0; x < imgW; x += stepSize, ++winI)
        {
            cx AU xEnd = Min(x + winSize, imgW);
            cx AU yEnd = Min(y + winSize, imgH);
            F4 covariance = 0;
            F4 weightSum = 0;

            for (SI j = y; j < yEnd; ++j)
            {
                cx AU iBase = j * imgW + x;
                for (SI i = 0; i < xEnd - x; ++i)
                {
                    cx AU& weight = imgMask.pixels[iBase + i].v;
                    covariance += (infoA.imgCol->pixels[iBase + i].v - infoA.wins[winI].mean) * (infoB.imgCol->pixels[iBase + i].v - infoB.wins[winI].mean) * weight;
                    weightSum += weight;
                }
            }
            if (IsNearZero(weightSum))
                continue;

            covariance /= weightSum;
            cx AU numerator = (2 * infoA.wins[winI].mean * infoB.wins[winI].mean + C1) * (2 * covariance + C2);
            cx AU denominator = (infoA.wins[winI].mean * infoA.wins[winI].mean + infoB.wins[winI].mean * infoB.wins[winI].mean + C1) * (infoA.wins[winI].variance + infoB.wins[winI].variance + C2);
            cx AU ssim = numerator / denominator;
            ssimSum += ssim * weightSum;
            weightSumAll += weightSum;
        }

    resultDiff = 1.0f - (IsNearZero(weightSumAll) ? F4(0) : (ssimSum / weightSumAll));
    rets;
}

dfa ColGridCmpA1Info::ColGridCmpA1Info()
{
    mode = Mode::LINEAR;
    longChunkCntSuggest = 0;
}

dfa ColGridCmpA2Info::ColGridCmpA2Info()
{
    gridMask = NUL;
    gridACol = NUL;
    gridBCol = NUL;
    gridAEdges = NUL;
    gridBEdges = NUL;
    gridASsim = NUL;
    gridBSsim = NUL;
    wCol = 1.0f;
    wEdges = 1.0f;
    wSsim = 2.0f;
    wColH = COL_HSV_WEIGHT_H;
    wColS = COL_HSV_WEIGHT_S;
    wColV = COL_HSV_WEIGHT_V;
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

tpl0 dfa NT ToType(ColVN& dst, cx ColRgbN& src)
{
    dst.v = src.r * 0.299f + src.g * 0.587f + src.b * 0.114f;
}
tpl0 dfa NT ToType(ColV& dst, cx ColRgba& src)
{
    dst.v = U1((U4(src.r) * 299 + U4(src.g) * 587 + U4(src.b) * 114) / 1000);
}
tpl0 dfa NT ToType(ColVN& dst, cx ColRgba& src)
{
    dst.v = F4(U4(src.r) * 299 + U4(src.g) * 587 + U4(src.b) * 114) / 255000.0f;
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
tpl0 dfa NT ToType(ColVN& dst, cx ColHsvN& src)
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
    dst = std::move(src);
}
tpl0 dfa NT ToType(ColVN& dst, cx ColVN& src)
{
    dst = std::move(src);
}
tpl0 dfa NT ToType(ColVN& dst, cx ColV& src)
{
    dst.v = F4(src.v) / 255.0f;
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
tpl0 dfa NT ToType(ColRgba& dst, cx ColV& src)
{
    dst.r = src.v;
    dst.g = src.v;
    dst.b = src.v;
    dst.a = 255;
}
tpl0 dfa NT ToType(ColRgba& dst, cx ColVN& src)
{
    dst.r = U1(src.v * 255.0f);
    dst.g = U1(src.v * 255.0f);
    dst.b = U1(src.v * 255.0f);
    dst.a = 255;
}
tpl0 dfa NT ToType(ColRgba& dst, cx ColRgb& src)
{
    dst.r = src.r;
    dst.g = src.g;
    dst.b = src.b;
    dst.a = 255;
}
tpl0 dfa NT ToType(ColRgb& dst, cx ColRgba& src)
{
    dst.r = src.r;
    dst.g = src.g;
    dst.b = src.b;
}
tpl0 dfa NT ToType(ColRgbN& dst, cx ColRgb& src)
{
    dst.r = F4(src.r) / 255.0f;
    dst.g = F4(src.g) / 255.0f;
    dst.b = F4(src.b) / 255.0f;
}
tpl0 dfa NT ToType(ColRgbN& dst, cx ColRgba& src)
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
tpl0 dfa NT ToType(ColHsvN& dst, cx ColRgba& src)
{
    ColRgbN tmp;
    ToType(tmp, src);
    ToType(dst, tmp);
}
