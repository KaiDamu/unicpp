#pragma once

tpl1 class Vec2
{
  public:
    T1 a;
    T1 b;

    dfa Vec2& operator=(cx Vec2& other)
    {
        a = other.a;
        b = other.b;
        ret *tx;
    }
    dfa Vec2& operator=(Vec2&& other)
    {
        a = move(other.a);
        b = move(other.b);
        ret *tx;
    }
    dfa BO operator==(cx Vec2& other) cx
    {
        ret a == other.a && b == other.b;
    }
    dfa BO operator!=(cx Vec2& other) cx
    {
        ret !(*tx == other);
    }
    dfa Vec2 operator+(cx Vec2& other) cx
    {
        ret Vec2(a + other.a, b + other.b);
    }
    dfa Vec2 operator-(cx Vec2& other) cx
    {
        ret Vec2(a - other.a, b - other.b);
    }
    dfa Vec2 operator*(T1 scalar) cx
    {
        ret Vec2(a * scalar, b * scalar);
    }
    dfa Vec2 operator/(T1 scalar) cx
    {
        ret Vec2(a / scalar, b / scalar);
    }
    dfa Vec2 operator-() cx
    {
        ret Vec2(-a, -b);
    }

    dfa Vec2() : a(T1(0)), b(T1(0))
    {
    }
    dfa Vec2(T1 a, T1 b) : a(a), b(b)
    {
    }
    dfa Vec2(cx Vec2& other) : a(other.a), b(other.b)
    {
    }
    dfa Vec2(Vec2&& other) : a(move(other.a)), b(move(other.b))
    {
    }
    dfa ~Vec2() = default;
};

tpl1 class Pos2
{
  public:
    T1 x;
    T1 y;

    dfa Pos2& operator=(cx Pos2& other)
    {
        x = other.x;
        y = other.y;
        ret *tx;
    }
    dfa Pos2& operator=(Pos2&& other)
    {
        x = move(other.x);
        y = move(other.y);
        ret *tx;
    }
    dfa BO operator==(cx Pos2& other) cx
    {
        ret x == other.x && y == other.y;
    }
    dfa BO operator!=(cx Pos2& other) cx
    {
        ret !(*tx == other);
    }
    dfa Pos2 operator+(cx Pos2& other) cx
    {
        ret Pos2(x + other.x, y + other.y);
    }
    dfa Pos2 operator-(cx Pos2& other) cx
    {
        ret Pos2(x - other.x, y - other.y);
    }
    dfa Pos2 operator*(T1 scalar) cx
    {
        ret Pos2(x * scalar, y * scalar);
    }
    dfa Pos2 operator/(T1 scalar) cx
    {
        ret Pos2(x / scalar, y / scalar);
    }
    dfa Pos2 operator-() cx
    {
        ret Pos2(-x, -y);
    }

    dfa Pos2& operator+=(cx Pos2& other)
    {
        x += other.x;
        y += other.y;
        ret *tx;
    }
    dfa Pos2& operator-=(cx Pos2& other)
    {
        x -= other.x;
        y -= other.y;
        ret *tx;
    }
    dfa Pos2& operator*=(T1 scalar)
    {
        x *= scalar;
        y *= scalar;
        ret *tx;
    }
    dfa Pos2& operator/=(T1 scalar)
    {
        x /= scalar;
        y /= scalar;
        ret *tx;
    }

    dfa Pos2() : x(T1(0)), y(T1(0))
    {
    }
    dfa Pos2(T1 x, T1 y) : x(x), y(y)
    {
    }
    dfa Pos2(cx Pos2& other) : x(other.x), y(other.y)
    {
    }
    dfa Pos2(Pos2&& other) : x(move(other.x)), y(move(other.y))
    {
    }
    dfa ~Pos2() = default;
};

tpl1 class Size2
{
  public:
    T1 w;
    T1 h;

    dfa Size2& operator=(cx Size2& other)
    {
        w = other.w;
        h = other.h;
        ret *tx;
    }
    dfa Size2& operator=(Size2&& other)
    {
        w = move(other.w);
        h = move(other.h);
        ret *tx;
    }
    dfa BO operator==(cx Size2& other) cx
    {
        ret w == other.w && h == other.h;
    }
    dfa BO operator!=(cx Size2& other) cx
    {
        ret !(*tx == other);
    }
    dfa Size2 operator+(cx Size2& other) cx
    {
        ret Size2(w + other.w, h + other.h);
    }
    dfa Size2 operator-(cx Size2& other) cx
    {
        ret Size2(w - other.w, h - other.h);
    }
    dfa Size2 operator*(T1 scalar) cx
    {
        ret Size2(w * scalar, h * scalar);
    }
    dfa Size2 operator/(T1 scalar) cx
    {
        ret Size2(w / scalar, h / scalar);
    }
    dfa Size2 operator-() cx
    {
        ret Size2(-w, -h);
    }

    dfa Size2() : w(T1(0)), h(T1(0))
    {
    }
    dfa Size2(T1 w, T1 h) : w(w), h(h)
    {
    }
    dfa Size2(cx Size2& other) : w(other.w), h(other.h)
    {
    }
    dfa Size2(Size2&& other) : w(move(other.w)), h(move(other.h))
    {
    }
    dfa ~Size2() = default;

    dfa T1 Area() cx
    {
        ret w * h;
    }
};

tpl1 dfa T1 Dist(cx Pos2<T1>& p1, cx Pos2<T1>& p2)
{
    ret Sqrt<T1>(Pow2<T1>(p1.x - p2.x) + Pow2<T1>(p1.y - p2.y));
}

tpl1 dfa T1 Dist0(cx Pos2<T1>& p)
{
    ret Dist0(p.x, p.y);
}
