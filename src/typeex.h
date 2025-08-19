#pragma once

tpl1 cxex BO IsTypeU = NO;
tpl0 cxex BO IsTypeU<U1> = YES;
tpl0 cxex BO IsTypeU<U2> = YES;
tpl0 cxex BO IsTypeU<U4> = YES;
tpl0 cxex BO IsTypeU<U8> = YES;

tpl1 cxex BO IsTypeS = NO;
tpl0 cxex BO IsTypeS<S1> = YES;
tpl0 cxex BO IsTypeS<S2> = YES;
tpl0 cxex BO IsTypeS<S4> = YES;
tpl0 cxex BO IsTypeS<S8> = YES;

tpl1 cxex BO IsTypeF = NO;
tpl0 cxex BO IsTypeF<F4> = YES;
tpl0 cxex BO IsTypeF<F8> = YES;

tpl2 struct _IsTypeSame
{
    static cxex BO val = NO;
};
tpl1 struct _IsTypeSame<T1, T1>
{
    static cxex BO val = YES;
};
tpl2 cxex BO IsTypeSame = _IsTypeSame<T1, T2>::val;

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

// user specialization required for TypeTraits
tpl1 struct TypeTraits;

// example specialization for TypeTraits on AnyClass
/*
tpl0 struct TypeTraits<AnyClass>
{
    using Any1T = U8;
    using Any2T = F4;

    static Any1T& anyVal1(AnyClass& obj)
    {
        ret obj.specificAnyVal1;
    }
    static Any2T& anyVal2(AnyClass& obj)
    {
        ret obj.specificAnyVal2;
    }
};
*/
