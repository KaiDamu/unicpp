#pragma once

#include <vector>
#include "vector.hpp"

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

    dfa ColRgb();
    dfa ColRgb(U1 r, U1 g, U1 b);
    dfa ~ColRgb() = default;

    dfa BO operator==(cx ColRgb& other) cx;
    dfa BO operator!=(cx ColRgb& other) cx;
    dfa ColRgb operator+(cx ColRgb& other) cx;
    dfa ColRgb operator-(cx ColRgb& other) cx;
    dfa ColRgb operator*(F4 scalar) cx;
    dfa ColRgb operator/(F4 scalar) cx;
    dfa ColRgb operator-() cx;
    dfa ColRgb& operator+=(cx ColRgb& other);
    dfa ColRgb& operator-=(cx ColRgb& other);
    dfa ColRgb& operator*=(F4 scalar);
    dfa ColRgb& operator/=(F4 scalar);
};
struct ColRgbN
{
    F4 r; // range: 0.0 - 1.0
    F4 g; // range: 0.0 - 1.0
    F4 b; // range: 0.0 - 1.0

    dfa ColRgbN();
    dfa ColRgbN(F4 r, F4 g, F4 b);
    dfa ~ColRgbN() = default;

    dfa BO operator==(cx ColRgbN& other) cx;
    dfa BO operator!=(cx ColRgbN& other) cx;
    dfa ColRgbN operator+(cx ColRgbN& other) cx;
    dfa ColRgbN operator-(cx ColRgbN& other) cx;
    dfa ColRgbN operator*(F4 scalar) cx;
    dfa ColRgbN operator/(F4 scalar) cx;
    dfa ColRgbN operator-() cx;
    dfa ColRgbN& operator+=(cx ColRgbN& other);
    dfa ColRgbN& operator-=(cx ColRgbN& other);
    dfa ColRgbN& operator*=(F4 scalar);
    dfa ColRgbN& operator/=(F4 scalar);
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

    dfa ColRgba();
    dfa ColRgba(U1 r, U1 g, U1 b, U1 a);
    dfa ColRgba(U4 val);
    dfa ~ColRgba() = default;

    dfa BO operator==(cx ColRgba& other) cx;
    dfa BO operator!=(cx ColRgba& other) cx;
    dfa ColRgba operator+(cx ColRgba& other) cx;
    dfa ColRgba operator-(cx ColRgba& other) cx;
    dfa ColRgba operator*(F4 scalar) cx;
    dfa ColRgba operator/(F4 scalar) cx;
    dfa ColRgba operator-() cx;
    dfa ColRgba& operator+=(cx ColRgba& other);
    dfa ColRgba& operator-=(cx ColRgba& other);
    dfa ColRgba& operator*=(F4 scalar);
    dfa ColRgba& operator/=(F4 scalar);
};
struct ColRgbaN
{
    F4 r; // range: 0.0 - 1.0
    F4 g; // range: 0.0 - 1.0
    F4 b; // range: 0.0 - 1.0
    F4 a; // range: 0.0 - 1.0

    dfa ColRgbaN();
    dfa ColRgbaN(F4 r, F4 g, F4 b, F4 a);
    dfa ~ColRgbaN() = default;

    dfa BO operator==(cx ColRgbaN& other) cx;
    dfa BO operator!=(cx ColRgbaN& other) cx;
    dfa ColRgbaN operator+(cx ColRgbaN& other) cx;
    dfa ColRgbaN operator-(cx ColRgbaN& other) cx;
    dfa ColRgbaN operator*(F4 scalar) cx;
    dfa ColRgbaN operator/(F4 scalar) cx;
    dfa ColRgbaN operator-() cx;
    dfa ColRgbaN& operator+=(cx ColRgbaN& other);
    dfa ColRgbaN& operator-=(cx ColRgbaN& other);
    dfa ColRgbaN& operator*=(F4 scalar);
    dfa ColRgbaN& operator/=(F4 scalar);
};
struct ColV
{
    U1 v; // range: 0 - 255

    dfa ColV();
    dfa ColV(U1 v);
    dfa ~ColV() = default;

    dfa BO operator==(cx ColV& other) cx;
    dfa BO operator!=(cx ColV& other) cx;
    dfa ColV operator+(cx ColV& other) cx;
    dfa ColV operator-(cx ColV& other) cx;
    dfa ColV operator*(F4 scalar) cx;
    dfa ColV operator/(F4 scalar) cx;
    dfa ColV operator-() cx;
    dfa ColV& operator+=(cx ColV& other);
    dfa ColV& operator-=(cx ColV& other);
    dfa ColV& operator*=(F4 scalar);
    dfa ColV& operator/=(F4 scalar);

    dfa BO operator<(cx ColV& other) cx;
    dfa BO operator>(cx ColV& other) cx;
    dfa BO operator<=(cx ColV& other) cx;
    dfa BO operator>=(cx ColV& other) cx;
    dfa ColV operator*(cx ColV& other) cx;
    dfa ColV operator/(cx ColV& other) cx;
    dfa ColV& operator*=(cx ColV& other);
    dfa ColV& operator/=(cx ColV& other);
};
struct ColVN
{
    F4 v; // range: 0.0 - 1.0

    dfa ColVN();
    dfa ColVN(F4 v);
    dfa ~ColVN() = default;

    dfa BO operator==(cx ColVN& other) cx;
    dfa BO operator!=(cx ColVN& other) cx;
    dfa ColVN operator+(cx ColVN& other) cx;
    dfa ColVN operator-(cx ColVN& other) cx;
    dfa ColVN operator*(F4 scalar) cx;
    dfa ColVN operator/(F4 scalar) cx;
    dfa ColVN operator-() cx;
    dfa ColVN& operator+=(cx ColVN& other);
    dfa ColVN& operator-=(cx ColVN& other);
    dfa ColVN& operator*=(F4 scalar);
    dfa ColVN& operator/=(F4 scalar);

    dfa BO operator<(cx ColVN& other) cx;
    dfa BO operator>(cx ColVN& other) cx;
    dfa BO operator<=(cx ColVN& other) cx;
    dfa BO operator>=(cx ColVN& other) cx;
    dfa ColVN operator*(cx ColVN& other) cx;
    dfa ColVN operator/(cx ColVN& other) cx;
    dfa ColVN& operator*=(cx ColVN& other);
    dfa ColVN& operator/=(cx ColVN& other);
};
struct ColHsvN
{
    F4 h; // range: 0.0 - 1.0
    F4 s; // range: 0.0 - 1.0
    F4 v; // range: 0.0 - 1.0

    dfa ColHsvN();
    dfa ColHsvN(F4 h, F4 s, F4 v);
    dfa ~ColHsvN() = default;

    dfa BO operator==(cx ColHsvN& other) cx;
    dfa BO operator!=(cx ColHsvN& other) cx;
    dfa ColHsvN operator+(cx ColHsvN& other) cx;
    dfa ColHsvN operator-(cx ColHsvN& other) cx;
    dfa ColHsvN operator*(F4 scalar) cx;
    dfa ColHsvN operator/(F4 scalar) cx;
    dfa ColHsvN operator-() cx;
    dfa ColHsvN& operator+=(cx ColHsvN& other);
    dfa ColHsvN& operator-=(cx ColHsvN& other);
    dfa ColHsvN& operator*=(F4 scalar);
    dfa ColHsvN& operator/=(F4 scalar);
};

/// [color grid]
tpl1 struct ColGrid
{
    std::vector<T1> pixels;
    Size2<SI> size;

    dfa T1& Pixel(cx Pos2<SI>& pos);
    dfa cx T1& Pixel(cx Pos2<SI>& pos) cx;
    dfa NT Resize(cx Size2<SI>& size);

    dfa ColGrid();
    dfa ColGrid(cx Size2<SI>& size);
};

/// [convert color]
tpl0 dfa NT ToType(ColVN& dst, cx ColRgbN& src);
tpl0 dfa NT ToType(ColV& dst, cx ColRgba& src);
tpl0 dfa NT ToType(ColVN& dst, cx ColRgba& src);
tpl0 dfa NT ToType(ColRgba& dst, cx ColRgbN& src);
tpl0 dfa NT ToType(ColHsvN& dst, cx ColRgbN& src);
tpl0 dfa NT ToType(ColRgbN& dst, cx ColHsvN& src);
tpl0 dfa NT ToType(ColRgba& dst, cx ColHsvN& src);
tpl0 dfa NT ToType(ColVN& dst, cx ColHsvN& src);
tpl0 dfa NT ToType(ColRgbN& dst, cx ColVN& src);
tpl0 dfa NT ToType(ColRgb& dst, cx ColRgb& src);
tpl0 dfa NT ToType(ColVN& dst, cx ColVN& src);
tpl0 dfa NT ToType(ColVN& dst, cx ColV& src);
tpl0 dfa NT ToType(ColRgb& dst, cx ColRgbN& src);
tpl0 dfa NT ToType(ColRgb& dst, cx ColVN& src);
tpl0 dfa NT ToType(ColRgba& dst, cx ColV& src);
tpl0 dfa NT ToType(ColRgba& dst, cx ColVN& src);
tpl0 dfa NT ToType(ColRgba& dst, cx ColRgb& src);
tpl0 dfa NT ToType(ColRgb& dst, cx ColRgba& src);
tpl0 dfa NT ToType(ColRgbN& dst, cx ColRgb& src);
tpl0 dfa NT ToType(ColRgbN& dst, cx ColRgba& src);
tpl0 dfa NT ToType(ColVN& dst, cx ColRgb& src);
tpl0 dfa NT ToType(ColHsvN& dst, cx ColRgb& src);
tpl0 dfa NT ToType(ColHsvN& dst, cx ColRgba& src);
tpl0 dfa NT ToType(ColRgbaN& dst, cx ColRgbaN& src);
tpl0 dfa NT ToType(ColRgbaN& dst, cx ColRgba& src);
tpl0 dfa NT ToType(ColRgbaN& dst, cx ColRgbN& src);
tpl0 dfa NT ToType(ColRgbaN& dst, cx ColRgb& src);

/// [convert color grid]
tpl2 dfa NT ColGridToType(ColGrid<T1>& dst, cx ColGrid<T2>& src);

/// [compare color]
dfa F4 ColHsvNCmp(cx ColHsvN& a, cx ColHsvN& b, F4 wH = COL_HSV_WEIGHT_H, F4 wS = COL_HSV_WEIGHT_S, F4 wV = COL_HSV_WEIGHT_V);

/// [compare color grid]
struct ColGridSsimInfo
{
    struct Win
    {
        F4 mean;
        F4 variance;
    };

    cx ColGrid<ColVN>* imgCol;
    cx ColGrid<ColVN>* imgMask;
    SI winSize;
    SI stepSize;
    std::vector<Win> wins;

    dfa NT Clr();

    dfa ColGridSsimInfo();
};
dfa NT ColGridSsimInfoGet(ColGridSsimInfo& result, cx ColGrid<ColVN>& imgCol, cx ColGrid<ColVN>& imgMask, SI winSize = 0, SI stepSize = 0);
dfa ER ColGridCmpSsim(F4& resultDiff, cx ColGridSsimInfo& infoA, cx ColGridSsimInfo& infoB);
struct ColGridCmpA1Info
{
    enum class Mode : U1
    {
        LINEAR = 0,
        CHUNK_MAX = 1,
    };

    Mode mode;
    SI longChunkCntSuggest;

    dfa ColGridCmpA1Info();
};
tpl1 dfa ER ColGridCmpA1(F4& resultDiff, cx ColGrid<T1>& subGrid, cx ColGrid<T1>& mainGrid, cx ColGridCmpA1Info& info);
struct ColGridCmpA2Info
{
    cx ColGrid<ColVN>* gridMask;   // set if per-pixel weights are used (to exclude an area, or focus on an area)
    cx ColGrid<ColHsvN>* gridACol; // set if HSV color comparison is used
    cx ColGrid<ColHsvN>* gridBCol; // set if HSV color comparison is used
    cx ColGrid<ColVN>* gridAEdges; // set if edge comparison is used
    cx ColGrid<ColVN>* gridBEdges; // set if edge comparison is used
    cx ColGridSsimInfo* gridASsim; // set if ssim comparison is used
    cx ColGridSsimInfo* gridBSsim; // set if ssim comparison is used
    F4 wCol;                       // weight for color difference
    F4 wEdges;                     // weight for edge difference
    F4 wSsim;                      // weight for ssim difference
    F4 wColH;                      // sub-weight for HSV H channel
    F4 wColS;                      // sub-weight for HSV S channel
    F4 wColV;                      // sub-weight for HSV V channel

    dfa ColGridCmpA2Info();
};
tpl<BO TUseMask, BO TDoCol, BO TDoEdges, BO TDoSsim> dfa ER ColGridCmpA2(F4& resultDiff, cx ColGridCmpA2Info& info);

/// [process color grid]
tpl2 dfa ER ColGridLerp(ColGrid<T1>& out, cx ColGrid<T1>& a, cx ColGrid<T1>& b, T2 t);
dfa NT ColGridEdgesSobel1(ColGrid<ColVN>& out, cx ColGrid<ColVN>& in);
tpl1 dfa NT ColGridEdgeMarkAll(ColGrid<T1>& colGrid, cx T1& colObj, cx T1& colMark);
tpl1 dfa NT ColGridFloodFillAt(ColGrid<T1>& colGrid, SI& fillCnt, SI fillOrigin, cx T1& colObj, cx T1& colMark, std::vector<SI>& stack);
tpl1 dfa NT ColGridFloodFillAt(ColGrid<T1>& colGrid, SI fillOrigin, cx T1& colObj, cx T1& colMark);
tpl1 dfa NT ColGridFloodFillAll(ColGrid<T1>& colGrid, std::vector<SI>& fillOrigins, std::vector<SI>& fillCnts, cx T1& colObj, cx T1& colMark);

/// [save/load color grid]
#ifdef PROG_SYS_WIN
tpl1 dfa ER ColGridToFileBmp(cx ColGrid<T1>& colGrid, cx CH* path);
tpl1 dfa ER FileToColGridBmp(ColGrid<T1>& colGrid, cx CH* path);
#endif

#include "color.ipp"
