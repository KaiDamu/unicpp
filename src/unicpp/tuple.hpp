#pragma once

#include "type.hpp"

tpl2 class Tuple2
{
  public:
    T1 dat1;
    T2 dat2;

  public:
    dfa NT Set(cx T1& dat1_, cx T2& dat2_);

  public:
    dfa Tuple2(cx T1& dat1_, cx T2& dat2_);
    dfa Tuple2();

  public:
    dfa NT operator=(cx Tuple2<T1, T2>& dat);
    dfa NT operator+=(cx Tuple2<T1, T2>& dat);
    dfa NT operator-=(cx Tuple2<T1, T2>& dat);
    dfa NT operator*=(cx Tuple2<T1, T2>& dat);
    dfa NT operator/=(cx Tuple2<T1, T2>& dat);
    dfa Tuple2<T1, T2> operator+(cx Tuple2<T1, T2>& dat) cx;
    dfa Tuple2<T1, T2> operator-(cx Tuple2<T1, T2>& dat) cx;
    dfa Tuple2<T1, T2> operator*(cx Tuple2<T1, T2>& dat) cx;
    dfa Tuple2<T1, T2> operator/(cx Tuple2<T1, T2>& dat) cx;
};

tpl2 class Keyval
{
  public:
    T1 key;
    T2 val;

  public:
    dfa NT Set(cx T1& key_, cx T2& val_);

  public:
    dfa Keyval(cx T1& key_, cx T2& val_);
    dfa Keyval();

  public:
    dfa NT operator=(cx Keyval<T1, T2>& keyval);
    dfa NT operator+=(cx Keyval<T1, T2>& keyval);
    dfa NT operator-=(cx Keyval<T1, T2>& keyval);
    dfa NT operator*=(cx Keyval<T1, T2>& keyval);
    dfa NT operator/=(cx Keyval<T1, T2>& keyval);
    dfa Keyval<T1, T2> operator+(cx Keyval<T1, T2>& keyval) cx;
    dfa Keyval<T1, T2> operator-(cx Keyval<T1, T2>& keyval) cx;
    dfa Keyval<T1, T2> operator*(cx Keyval<T1, T2>& keyval) cx;
    dfa Keyval<T1, T2> operator/(cx Keyval<T1, T2>& keyval) cx;
};

#include "tuple.ipp"
