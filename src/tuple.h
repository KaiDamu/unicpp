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
	dfa Pos2<T1> operator + (cx Pos2<T1>& pos) { ret Pos2<T1>(x + pos.x, y + pos.y); }
	dfa Pos2<T1> operator - (cx Pos2<T1>& pos) { ret Pos2<T1>(x - pos.x, y - pos.y); }
	dfa Pos2<T1> operator * (cx Pos2<T1>& pos) { ret Pos2<T1>(x * pos.x, y * pos.y); }
	dfa Pos2<T1> operator / (cx Pos2<T1>& pos) { ret Pos2<T1>(x / pos.x, y / pos.y); }
	dfa Pos2<T1> operator + (cx T1& val) { ret Pos2<T1>(x + val, y + val); }
	dfa Pos2<T1> operator - (cx T1& val) { ret Pos2<T1>(x - val, y - val); }
	dfa Pos2<T1> operator * (cx T1& val) { ret Pos2<T1>(x * val, y * val); }
	dfa Pos2<T1> operator / (cx T1& val) { ret Pos2<T1>(x / val, y / val); }
};

tpl2 class Tuple2 {
private:
	T1 m_dat1;
	T2 m_dat2;
public:
	dfa T1& Dat1() {
		ret m_dat1;
	}
	dfa cx T1& Dat1Cx() const {
		ret m_dat1;
	}
	dfa T2& Dat2() {
		ret m_dat2;
	}
	dfa cx T2& Dat2Cx() const {
		ret m_dat2;
	}
	dfa NT Dat1(cx T1& dat) {
		m_dat1 = dat;
	}
	dfa NT Dat2(cx T2& dat) {
		m_dat2 = dat;
	}
	dfa NT Set(cx T1& dat1, cx T2& dat2) {
		m_dat1 = dat1;
		m_dat2 = dat2;
	}
	dfa NT Set(cx Tuple2& tuple) {
		m_dat1 = tuple.m_dat1;
		m_dat2 = tuple.m_dat2;
	}
	dfa Tuple2(cx T1& dat1, cx T2& dat2) : m_dat1(dat1), m_dat2(dat2) {}
	dfa Tuple2() {}
};

tpl2 class Keyval {
private:
	Tuple2<T1, T2> m_tuple;
public:
	dfa T1& Key() {
		ret m_tuple.Dat1();
	}
	dfa cx T1& KeyCx() const {
		ret m_tuple.Dat1Cx();
	}
	dfa T2& Val() {
		ret m_tuple.Dat2();
	}
	dfa cx T2& ValCx() const {
		ret m_tuple.Dat2Cx();
	}
	dfa NT Key(cx T1& key) {
		m_tuple.Dat1(key);
	}
	dfa NT Val(cx T2& val) {
		m_tuple.Dat2(val);
	}
	dfa NT Set(cx T1& key, cx T2& val) {
		m_tuple.Set(key, val);
	}
	dfa NT Set(cx Keyval& keyval) {
		m_tuple.Set(keyval.m_tuple);
	}
	dfa Keyval(cx T1& key, cx T2& val) : m_tuple(key, val) {}
	dfa Keyval() {}
};
