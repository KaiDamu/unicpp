#pragma once

tpl2 class Tuple2
{
  public:
    T1 dat1;
    T2 dat2;

  public:
    dfa NT Set(cx T1& dat1_, cx T2& dat2_)
    {
        tx->dat1 = dat1_;
        tx->dat2 = dat2_;
    }

  public:
    dfa Tuple2(cx T1& dat1_, cx T2& dat2_) : dat1(dat1_), dat2(dat2_)
    {
    }
    dfa Tuple2() : dat1(T1(0)), dat2(T2(0))
    {
    }

  public:
    dfa NT operator=(cx Tuple2<T1, T2>& dat)
    {
        dat1 = dat.dat1;
        dat2 = dat.dat2;
    }
    dfa NT operator+=(cx Tuple2<T1, T2>& dat)
    {
        dat1 += dat.dat1;
        dat2 += dat.dat2;
    }
    dfa NT operator-=(cx Tuple2<T1, T2>& dat)
    {
        dat1 -= dat.dat1;
        dat2 -= dat.dat2;
    }
    dfa NT operator*=(cx Tuple2<T1, T2>& dat)
    {
        dat1 *= dat.dat1;
        dat2 *= dat.dat2;
    }
    dfa NT operator/=(cx Tuple2<T1, T2>& dat)
    {
        dat1 /= dat.dat1;
        dat2 /= dat.dat2;
    }
    dfa Tuple2<T1, T2> operator+(cx Tuple2<T1, T2>& dat) cx
    {
        ret Tuple2<T1, T2>(dat1 + dat.dat1, dat2 + dat.dat2);
    }
    dfa Tuple2<T1, T2> operator-(cx Tuple2<T1, T2>& dat) cx
    {
        ret Tuple2<T1, T2>(dat1 - dat.dat1, dat2 - dat.dat2);
    }
    dfa Tuple2<T1, T2> operator*(cx Tuple2<T1, T2>& dat) cx
    {
        ret Tuple2<T1, T2>(dat1 * dat.dat1, dat2 * dat.dat2);
    }
    dfa Tuple2<T1, T2> operator/(cx Tuple2<T1, T2>& dat) cx
    {
        ret Tuple2<T1, T2>(dat1 / dat.dat1, dat2 / dat.dat2);
    }
};

tpl2 class Keyval
{
  public:
    T1 key;
    T2 val;

  public:
    dfa NT Set(cx T1& key_, cx T2& val_)
    {
        tx->key = key_;
        tx->val = val_;
    }

  public:
    dfa Keyval(cx T1& key_, cx T2& val_) : key(key_), val(val_)
    {
    }
    dfa Keyval() : key(T1(0)), val(T2(0))
    {
    }

  public:
    dfa NT operator=(cx Keyval<T1, T2>& keyval)
    {
        key = keyval.key;
        val = keyval.val;
    }
    dfa NT operator+=(cx Keyval<T1, T2>& keyval)
    {
        key += keyval.key;
        val += keyval.val;
    }
    dfa NT operator-=(cx Keyval<T1, T2>& keyval)
    {
        key -= keyval.key;
        val -= keyval.val;
    }
    dfa NT operator*=(cx Keyval<T1, T2>& keyval)
    {
        key *= keyval.key;
        val *= keyval.val;
    }
    dfa NT operator/=(cx Keyval<T1, T2>& keyval)
    {
        key /= keyval.key;
        val /= keyval.val;
    }
    dfa Keyval<T1, T2> operator+(cx Keyval<T1, T2>& keyval) cx
    {
        ret Keyval<T1, T2>(key + keyval.key, val + keyval.val);
    }
    dfa Keyval<T1, T2> operator-(cx Keyval<T1, T2>& keyval) cx
    {
        ret Keyval<T1, T2>(key - keyval.key, val - keyval.val);
    }
    dfa Keyval<T1, T2> operator*(cx Keyval<T1, T2>& keyval) cx
    {
        ret Keyval<T1, T2>(key * keyval.key, val * keyval.val);
    }
    dfa Keyval<T1, T2> operator/(cx Keyval<T1, T2>& keyval) cx
    {
        ret Keyval<T1, T2>(key / keyval.key, val / keyval.val);
    }
};
