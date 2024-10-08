#pragma once

tpl1 class Pos2
{
public:
    T1 x;
    T1 y;

    dfa Pos2& operator=(cx Pos2& other) { x = other.x; y = other.y; ret *tx; }
    dfa Pos2& operator=(Pos2&& other) { x = std::move(other.x); y = std::move(other.y); ret *tx; }
    dfa BO operator==(cx Pos2& other) cx { ret x == other.x && y == other.y; }
    dfa BO operator!=(cx Pos2& other) cx { ret !(*tx == other); }
    dfa Pos2 operator+(cx Pos2& other) cx { ret Pos2(x + other.x, y + other.y); }
    dfa Pos2 operator-(cx Pos2& other) cx { ret Pos2(x - other.x, y - other.y); }
    dfa Pos2 operator*(T1 scalar) cx { ret Pos2(x * scalar, y * scalar); }
    dfa Pos2 operator/(T1 scalar) cx { ret Pos2(x / scalar, y / scalar); }
    dfa Pos2 operator-() cx { ret Pos2(-x, -y); }

    dfa Pos2() : x(T1(0)), y(T1(0)) { }
    dfa Pos2(T1 x, T1 y) : x(x), y(y) { }
    dfa Pos2(cx Pos2& other) : x(other.x), y(other.y) { }
    dfa Pos2(Pos2&& other) : x(std::move(other.x)), y(std::move(other.y)) { }
    dfa ~Pos2() = default;
};

tpl1 class Size2
{
public:
    T1 w;
    T1 h;

    dfa Size2& operator=(cx Size2& other) { w = other.w; h = other.h; ret *tx; }
    dfa Size2& operator=(Size2&& other) { w = std::move(other.w); h = std::move(other.h); ret *tx; }
    dfa BO operator==(cx Size2& other) cx { ret w == other.w && h == other.h; }
    dfa BO operator!=(cx Size2& other) cx { ret !(*tx == other); }
    dfa Size2 operator+(cx Size2& other) cx { ret Size2(w + other.w, h + other.h); }
    dfa Size2 operator-(cx Size2& other) cx { ret Size2(w - other.w, h - other.h); }
    dfa Size2 operator*(T1 scalar) cx { ret Size2(w * scalar, h * scalar); }
    dfa Size2 operator/(T1 scalar) cx { ret Size2(w / scalar, h / scalar); }
    dfa Size2 operator-() cx { ret Size2(-w, -h); }

    dfa Size2() : w(T1(0)), h(T1(0)) { }
    dfa Size2(T1 w, T1 h) : w(w), h(h) { }
    dfa Size2(cx Size2& other) : w(other.w), h(other.h) { }
    dfa Size2(Size2&& other) : w(std::move(other.w)), h(std::move(other.h)) { }
    dfa ~Size2() = default;

    dfa T1 Area() cx { ret w * h; }
};
