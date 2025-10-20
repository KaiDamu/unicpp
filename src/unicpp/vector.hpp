#pragma once

#include "typeex.hpp"

tpl1 class Vec2
{
  public:
    union {
        struct
        {
            T1 a;
            T1 b;
        };
        struct
        {
            T1 x;
            T1 y;
        };
        struct
        {
            T1 w;
            T1 h;
        };
    };

    dfa cxex Vec2() noex;
    dfa cxex Vec2(T1 a, T1 b) noex;
    dfa cxex Vec2(cx Vec2& other) noex;
    dfa cxex Vec2(Vec2&& other) noex;
    dfa ~Vec2() = default;

    dfa Vec2& operator=(cx Vec2& other);
    dfa Vec2& operator=(Vec2&& other) noex;
    dfa cxex BO operator==(cx Vec2& other) cx;
    dfa cxex BO operator!=(cx Vec2& other) cx;
    dfa cxex Vec2 operator+(cx Vec2& other) cx;
    dfa cxex Vec2 operator-(cx Vec2& other) cx;
    dfa cxex Vec2 operator*(T1 scalar) cx;
    dfa cxex Vec2 operator/(T1 scalar) cx;
    dfa cxex Vec2 operator*(cx Vec2& other) cx;
    dfa cxex Vec2 operator/(cx Vec2& other) cx;
    dfa cxex Vec2 operator-() cx;
    dfa Vec2& operator+=(cx Vec2& other);
    dfa Vec2& operator-=(cx Vec2& other);
    dfa Vec2& operator*=(T1 scalar);
    dfa Vec2& operator/=(T1 scalar);

    dfa NT Clr();
    dfa T1 Area() cx;
};

tpl1 class Vec3
{
  public:
    union {
        struct
        {
            T1 a;
            T1 b;
            T1 c;
        };
        struct
        {
            T1 x;
            T1 y;
            T1 z;
        };
        struct
        {
            T1 w;
            T1 h;
            T1 d;
        };
    };

    dfa cxex Vec3() noex;
    dfa cxex Vec3(T1 a, T1 b, T1 c) noex;
    dfa cxex Vec3(cx Vec3& other) noex;
    dfa cxex Vec3(Vec3&& other) noex;
    dfa ~Vec3() = default;

    dfa Vec3& operator=(cx Vec3& other);
    dfa Vec3& operator=(Vec3&& other) noex;
    dfa cxex BO operator==(cx Vec3& other) cx;
    dfa cxex BO operator!=(cx Vec3& other) cx;
    dfa cxex Vec3 operator+(cx Vec3& other) cx;
    dfa cxex Vec3 operator-(cx Vec3& other) cx;
    dfa cxex Vec3 operator*(T1 scalar) cx;
    dfa cxex Vec3 operator/(T1 scalar) cx;
    dfa cxex Vec3 operator*(cx Vec3& other) cx;
    dfa cxex Vec3 operator/(cx Vec3& other) cx;
    dfa cxex Vec3 operator-() cx;
    dfa Vec3& operator+=(cx Vec3& other);
    dfa Vec3& operator-=(cx Vec3& other);
    dfa Vec3& operator*=(T1 scalar);
    dfa Vec3& operator/=(T1 scalar);

    dfa NT Clr();
    dfa T1 Vol() cx;
};

tpl1 using Pos2 = Vec2<T1>;
tpl1 using Size2 = Vec2<T1>;

tpl1 using Pos3 = Vec3<T1>;
tpl1 using Size3 = Vec3<T1>;

tpl1 dfa T1 Dist0(cx Pos2<T1>& p);
tpl1 dfa T1 Dist(cx Pos2<T1>& p1, cx Pos2<T1>& p2);

tpl1 dfa Vec2<S8> RoundToInt(Vec2<T1> val);

tpl1 dfa T1 Vec2Len(cx Vec2<T1>& vec);
tpl1 dfa T1 Vec2Dot(cx Vec2<T1>& a, cx Vec2<T1>& b);
tpl1 dfa T1 Vec2Cross(cx Vec2<T1>& a, cx Vec2<T1>& b);
tpl1 dfa Vec2<T1> Vec2Get(cx Pos2<T1>& src, cx Pos2<T1>& dst);
tpl1 dfa Vec2<T1> Vec2Normalize(cx Vec2<T1>& vec);
tpl1 dfa Vec2<T1> Vec2Perp(cx Vec2<T1>& vec);

tpl1 dfa SI Size2ChunkI(cx Size2<T1>& units, cx Size2<SI>& chunks, T1 chunkSize, SI unitI);
tpl1 dfa Size2<SI> Size2ChunkCnt(cx Size2<T1>& size, T1 chunkSize);
tpl1 dfa T1 Size2ChunkSizeSuggest(cx Size2<T1>& size, SI longChunkCnt, F4 qualityMin = 0.9f, F4 qualityLoss = 0.15f);

#include "vector.ipp"
