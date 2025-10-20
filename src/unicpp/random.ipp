#include "random.hpp"

#include "typeex.hpp"

tpl1 dfa T1 RandCtx::RandVal(T1 min, T1 max)
{
    ifcx (IsTypeSame<T1, U4>)
        ret tx->RandU4(min, max);
    else ifcx (IsTypeSame<T1, S4>)
        ret tx->RandS4(min, max);
    else ifcx (IsTypeSame<T1, F4>)
        ret tx->RandF4(min, max);
    else ifcx (IsTypeSame<T1, F8>)
        ret tx->RandF8(min, max);
    else
        static_assert(NO && siz(T1), "RandCtx::RandVal: unsupported type");
}

tpl1 dfa T1 RandVal(T1 min, T1 max)
{
    ret g_randCtx.RandVal(min, max);
}

tpl1 dfa T1 RandMag()
{
    ret T1(F8(T1(1) << RandVal<U4>(0, sizb(T1) - 1)) * RandVal<F8>(0.5, 2.0));
}

tpl1 dfa NT MixRand(T1* buf, SI cnt)
{
    ite (i, i < cnt)
        Swap<T1>(buf[i], buf[SI(RandVal<U4>(0, U4(cnt - 1)))]);
}
