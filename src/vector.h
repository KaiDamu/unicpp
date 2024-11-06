#pragma once

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

    dfa constexpr Vec2() : a(), b()
    {
    }
    dfa constexpr Vec2(T1 a, T1 b) : a(a), b(b)
    {
    }
    dfa constexpr Vec2(cx Vec2& other) : a(other.a), b(other.b)
    {
    }
    dfa constexpr Vec2(Vec2&& other) noex : a(move(other.a)), b(move(other.b))
    {
    }
    dfa ~Vec2() = default;

    dfa Vec2& operator=(cx Vec2& other)
    {
        a = other.a;
        b = other.b;
        ret *tx;
    }
    dfa Vec2& operator=(Vec2&& other) noex
    {
        a = move(other.a);
        b = move(other.b);
        ret *tx;
    }
    dfa constexpr BO operator==(cx Vec2& other) cx
    {
        ret a == other.a && b == other.b;
    }
    dfa constexpr BO operator!=(cx Vec2& other) cx
    {
        ret a != other.a || b != other.b;
    }
    dfa constexpr Vec2 operator+(cx Vec2& other) cx
    {
        ret Vec2(a + other.a, b + other.b);
    }
    dfa constexpr Vec2 operator-(cx Vec2& other) cx
    {
        ret Vec2(a - other.a, b - other.b);
    }
    dfa constexpr Vec2 operator*(T1 scalar) cx
    {
        ret Vec2(a * scalar, b * scalar);
    }
    dfa constexpr Vec2 operator/(T1 scalar) cx
    {
        ret Vec2(a / scalar, b / scalar);
    }
    dfa constexpr Vec2 operator-() cx
    {
        ret Vec2(-a, -b);
    }
    dfa Vec2& operator+=(cx Vec2& other)
    {
        a += other.a;
        b += other.b;
        ret *tx;
    }
    dfa Vec2& operator-=(cx Vec2& other)
    {
        a -= other.a;
        b -= other.b;
        ret *tx;
    }
    dfa Vec2& operator*=(T1 scalar)
    {
        a *= scalar;
        b *= scalar;
        ret *tx;
    }
    dfa Vec2& operator/=(T1 scalar)
    {
        a /= scalar;
        b /= scalar;
        ret *tx;
    }

    dfa T1 Area() cx
    {
        ret w * h;
    }
};

tpl1 using Pos2 = Vec2<T1>;
tpl1 using Size2 = Vec2<T1>;

tpl1 dfa T1 Dist0(cx Pos2<T1>& p)
{
    ret Dist0<T1>(p.x, p.y);
}
tpl1 dfa T1 Dist(cx Pos2<T1>& p1, cx Pos2<T1>& p2)
{
    ret Dist0<T1>(p2.x - p1.x, p2.y - p1.y);
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
