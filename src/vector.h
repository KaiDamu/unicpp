#pragma once

tpl1 class Pos2 {
public:
	T1 x;
	T1 y;
public:
	dfa NT Set(cx T1& x_, cx T1& y_) {
		tx->x = x_;
		tx->y = y_;
	}
public:
	dfa Pos2(cx T1& x_, cx T1& y_) : x(x_), y(y_) {}
	dfa Pos2() : x(T1(0)), y(T1(0)) {}
public:
	dfa NT operator = (cx Pos2<T1>& pos) { x = pos.x; y = pos.y; }
	dfa NT operator += (cx Pos2<T1>& pos) { x += pos.x; y += pos.y; }
	dfa NT operator -= (cx Pos2<T1>& pos) { x -= pos.x; y -= pos.y; }
	dfa NT operator *= (cx Pos2<T1>& pos) { x *= pos.x; y *= pos.y; }
	dfa NT operator /= (cx Pos2<T1>& pos) { x /= pos.x; y /= pos.y; }
	dfa NT operator = (cx T1& val) { x = val; y = val; }
	dfa NT operator += (cx T1& val) { x += val; y += val; }
	dfa NT operator -= (cx T1& val) { x -= val; y -= val; }
	dfa NT operator *= (cx T1& val) { x *= val; y *= val; }
	dfa NT operator /= (cx T1& val) { x /= val; y /= val; }
	dfa Pos2<T1> operator + (cx Pos2<T1>& pos) const { ret Pos2<T1>(x + pos.x, y + pos.y); }
	dfa Pos2<T1> operator - (cx Pos2<T1>& pos) const { ret Pos2<T1>(x - pos.x, y - pos.y); }
	dfa Pos2<T1> operator * (cx Pos2<T1>& pos) const { ret Pos2<T1>(x * pos.x, y * pos.y); }
	dfa Pos2<T1> operator / (cx Pos2<T1>& pos) const { ret Pos2<T1>(x / pos.x, y / pos.y); }
	dfa Pos2<T1> operator + (cx T1& val) const { ret Pos2<T1>(x + val, y + val); }
	dfa Pos2<T1> operator - (cx T1& val) const { ret Pos2<T1>(x - val, y - val); }
	dfa Pos2<T1> operator * (cx T1& val) const { ret Pos2<T1>(x * val, y * val); }
	dfa Pos2<T1> operator / (cx T1& val) const { ret Pos2<T1>(x / val, y / val); }
};
