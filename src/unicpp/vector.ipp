#include "vector.hpp"

#include <utility>
#include "math.hpp"

tpl1 dfa cxex Vec2<T1>::Vec2() noex : a(), b()
{
}
tpl1 dfa cxex Vec2<T1>::Vec2(T1 a, T1 b) noex : a(a), b(b)
{
}
tpl1 dfa cxex Vec2<T1>::Vec2(cx Vec2& other) noex : a(other.a), b(other.b)
{
}
tpl1 dfa cxex Vec2<T1>::Vec2(Vec2&& other) noex : a(std::move(other.a)), b(std::move(other.b))
{
}

tpl1 dfa Vec2<T1>& Vec2<T1>::operator=(cx Vec2& other)
{
    a = other.a;
    b = other.b;
    ret *tx;
}
tpl1 dfa Vec2<T1>& Vec2<T1>::operator=(Vec2&& other) noex
{
    a = std::move(other.a);
    b = std::move(other.b);
    ret *tx;
}
tpl1 dfa cxex BO Vec2<T1>::operator==(cx Vec2& other) cx
{
    ret a == other.a && b == other.b;
}
tpl1 dfa cxex BO Vec2<T1>::operator!=(cx Vec2& other) cx
{
    ret a != other.a || b != other.b;
}
tpl1 dfa cxex Vec2<T1> Vec2<T1>::operator+(cx Vec2& other) cx
{
    ret Vec2(a + other.a, b + other.b);
}
tpl1 dfa cxex Vec2<T1> Vec2<T1>::operator-(cx Vec2& other) cx
{
    ret Vec2(a - other.a, b - other.b);
}
tpl1 dfa cxex Vec2<T1> Vec2<T1>::operator*(T1 scalar) cx
{
    ret Vec2(a * scalar, b * scalar);
}
tpl1 dfa cxex Vec2<T1> Vec2<T1>::operator/(T1 scalar) cx
{
    ret Vec2(a / scalar, b / scalar);
}
tpl1 dfa cxex Vec2<T1> Vec2<T1>::operator*(cx Vec2& other) cx
{
    ret Vec2(a * other.a, b * other.b);
}
tpl1 dfa cxex Vec2<T1> Vec2<T1>::operator/(cx Vec2& other) cx
{
    ret Vec2(a / other.a, b / other.b);
}
tpl1 dfa cxex Vec2<T1> Vec2<T1>::operator-() cx
{
    ret Vec2(-a, -b);
}
tpl1 dfa Vec2<T1>& Vec2<T1>::operator+=(cx Vec2& other)
{
    a += other.a;
    b += other.b;
    ret *tx;
}
tpl1 dfa Vec2<T1>& Vec2<T1>::operator-=(cx Vec2& other)
{
    a -= other.a;
    b -= other.b;
    ret *tx;
}
tpl1 dfa Vec2<T1>& Vec2<T1>::operator*=(T1 scalar)
{
    a *= scalar;
    b *= scalar;
    ret *tx;
}
tpl1 dfa Vec2<T1>& Vec2<T1>::operator/=(T1 scalar)
{
    a /= scalar;
    b /= scalar;
    ret *tx;
}

tpl1 dfa NT Vec2<T1>::Clr()
{
    a = 0;
    b = 0;
}
tpl1 dfa T1 Vec2<T1>::Area() cx
{
    ret w * h;
}

tpl1 dfa cxex Vec3<T1>::Vec3() noex : a(), b(), c()
{
}
tpl1 dfa cxex Vec3<T1>::Vec3(T1 a, T1 b, T1 c) noex : a(a), b(b), c(c)
{
}
tpl1 dfa cxex Vec3<T1>::Vec3(cx Vec3& other) noex : a(other.a), b(other.b), c(other.c)
{
}
tpl1 dfa cxex Vec3<T1>::Vec3(Vec3&& other) noex : a(std::move(other.a)), b(std::move(other.b)), c(std::move(other.c))
{
}

tpl1 dfa Vec3<T1>& Vec3<T1>::operator=(cx Vec3& other)
{
    a = other.a;
    b = other.b;
    c = other.c;
    ret *tx;
}
tpl1 dfa Vec3<T1>& Vec3<T1>::operator=(Vec3&& other) noex
{
    a = std::move(other.a);
    b = std::move(other.b);
    c = std::move(other.c);
    ret *tx;
}
tpl1 dfa cxex BO Vec3<T1>::operator==(cx Vec3& other) cx
{
    ret a == other.a && b == other.b && c == other.c;
}
tpl1 dfa cxex BO Vec3<T1>::operator!=(cx Vec3& other) cx
{
    ret a != other.a || b != other.b || c != other.c;
}
tpl1 dfa cxex Vec3<T1> Vec3<T1>::operator+(cx Vec3& other) cx
{
    ret Vec3(a + other.a, b + other.b, c + other.c);
}
tpl1 dfa cxex Vec3<T1> Vec3<T1>::operator-(cx Vec3& other) cx
{
    ret Vec3(a - other.a, b - other.b, c - other.c);
}
tpl1 dfa cxex Vec3<T1> Vec3<T1>::operator*(T1 scalar) cx
{
    ret Vec3(a * scalar, b * scalar, c * scalar);
}
tpl1 dfa cxex Vec3<T1> Vec3<T1>::operator/(T1 scalar) cx
{
    ret Vec3(a / scalar, b / scalar, c / scalar);
}
tpl1 dfa cxex Vec3<T1> Vec3<T1>::operator*(cx Vec3& other) cx
{
    ret Vec3(a * other.a, b * other.b, c * other.c);
}
tpl1 dfa cxex Vec3<T1> Vec3<T1>::operator/(cx Vec3& other) cx
{
    ret Vec3(a / other.a, b / other.b, c / other.c);
}
tpl1 dfa cxex Vec3<T1> Vec3<T1>::operator-() cx
{
    ret Vec3(-a, -b, -c);
}
tpl1 dfa Vec3<T1>& Vec3<T1>::operator+=(cx Vec3& other)
{
    a += other.a;
    b += other.b;
    c += other.c;
    ret *tx;
}
tpl1 dfa Vec3<T1>& Vec3<T1>::operator-=(cx Vec3& other)
{
    a -= other.a;
    b -= other.b;
    c -= other.c;
    ret *tx;
}
tpl1 dfa Vec3<T1>& Vec3<T1>::operator*=(T1 scalar)
{
    a *= scalar;
    b *= scalar;
    c *= scalar;
    ret *tx;
}
tpl1 dfa Vec3<T1>& Vec3<T1>::operator/=(T1 scalar)
{
    a /= scalar;
    b /= scalar;
    c /= scalar;
    ret *tx;
}

tpl1 dfa NT Vec3<T1>::Clr()
{
    a = 0;
    b = 0;
    c = 0;
}
tpl1 dfa T1 Vec3<T1>::Vol() cx
{
    ret w * h * d;
}

tpl1 dfa T1 Dist0(cx Pos2<T1>& p)
{
    ret Dist0<T1>(p.x, p.y);
}
tpl1 dfa T1 Dist(cx Pos2<T1>& p1, cx Pos2<T1>& p2)
{
    ret Dist0<T1>(p2.x - p1.x, p2.y - p1.y);
}

tpl1 dfa Vec2<S8> RoundToInt(Vec2<T1> val)
{
    ret Vec2<S8>(RoundToInt<T1>(val.x), RoundToInt<T1>(val.y));
}

tpl1 dfa T1 Vec2Len(cx Vec2<T1>& vec)
{
    ret Dist0<T1>(vec.x, vec.y);
}
tpl1 dfa T1 Vec2Dot(cx Vec2<T1>& a, cx Vec2<T1>& b)
{
    ret a.x * b.x + a.y * b.y;
}
tpl1 dfa T1 Vec2Cross(cx Vec2<T1>& a, cx Vec2<T1>& b)
{
    ret a.x * b.y - a.y * b.x;
}
tpl1 dfa Vec2<T1> Vec2Get(cx Pos2<T1>& src, cx Pos2<T1>& dst)
{
    ret dst - src;
}
tpl1 dfa Vec2<T1> Vec2Normalize(cx Vec2<T1>& vec)
{
    ret vec / Vec2Len<T1>(vec);
}
tpl1 dfa Vec2<T1> Vec2Perp(cx Vec2<T1>& vec)
{
    ret Vec2<T1>(-vec.y, vec.x);
}

tpl1 dfa SI Size2ChunkI(cx Size2<T1>& units, cx Size2<SI>& chunks, T1 chunkSize, SI unitI)
{
    // TODO: add IsTypeF support
    ret ((unitI % units.w) / chunkSize) + ((unitI / units.w) / chunkSize) * chunks.w;
}
tpl1 dfa Size2<SI> Size2ChunkCnt(cx Size2<T1>& size, T1 chunkSize)
{
    cx AU result = Size2<T1>(DivCeil(size.w, chunkSize), DivCeil(size.h, chunkSize));
    ifcx (IsTypeF<T1>)
        ret Size2<SI>(RoundToInt(result.w), RoundToInt(result.h));
    ret Size2<SI>(result.w, result.h);
}
tpl1 dfa T1 Size2ChunkSizeSuggest(cx Size2<T1>& size, SI longChunkCnt, F4 qualityMin, F4 qualityLoss)
{
    cx AU chunkSizeBaseF = F4(Max(size.w, size.h)) / F4(longChunkCnt);
    cx AU chunkSizeBase = T1(RoundToInt(chunkSizeBaseF));
    cx AU qualityLossWeighted = qualityLoss / chunkSizeBaseF;
    T1 i = 0;
    do
    {
        cx AU chunkSize = ZigzagAround(chunkSizeBase, i);
        cx AU remX = size.w % chunkSize;
        cx AU remY = size.h % chunkSize;
        cx AU lastW = remX ? remX : chunkSize;
        cx AU lastH = remY ? remY : chunkSize;
        cx AU minSize = Min(lastW, lastH);
        cx AU quality = F4(minSize) / F4(chunkSize);
        if (quality >= qualityMin)
            ret chunkSize;
        qualityMin -= qualityLossWeighted;
        ++i;
    } while (YES);
}
