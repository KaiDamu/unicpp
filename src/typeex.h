#pragma once

tpl1 constexpr BO IsTypeF = NO;
tpl0 constexpr BO IsTypeF<F4> = YES;
tpl0 constexpr BO IsTypeF<F8> = YES;

tpl2 dfa T1& AsType(T2& src)
{
    static_assert(siz(T1) == siz(T2), "AsType: size of types must be the same");
    ret reinterpret_cast<T1&>(src);
}
tpl2 dfa cx T1& AsType(cx T2& src)
{
    static_assert(siz(T1) == siz(T2), "AsType: size of types must be the same");
    ret reinterpret_cast<cx T1&>(src);
}

tpl2 dfa NT ToType(T1& dst, cx T2& src) = delete;
tpl2 dfa T1 ToType(cx T2& src)
{
    T1 dst;
    ToType<T1, T2>(dst, src);
    ret dst;
}
