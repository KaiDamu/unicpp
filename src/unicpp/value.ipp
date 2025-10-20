#include "value.hpp"

tpl1 cxex dfa T1 Pi()
{
    ret static_cast<T1>(3.14159265358979323846);
}
tpl0 cxex dfa F4 Pi<F4>()
{
    ret 3.14159265358979f;
}
tpl0 cxex dfa F8 Pi<F8>()
{
    ret 3.14159265358979323846;
}
tpl1 cxex dfa T1 Tau()
{
    ret Pi<T1>() + Pi<T1>();
}

tpl1 cxex dfa SI VarintSizeMax()
{
    ret (sizb(T1) + 6) / 7;
}
tpl<typename T1, U1 TBase> cxex dfa SI VarbaseintSizeMax()
{
    static_assert(TBase > 0 && TBase < 8, "TBase must be in (0, 8)");
    ret (sizb(T1) + (TBase - 1)) / TBase;
}
