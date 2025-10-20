#include "ucnetmsg.hpp"

#ifdef PROG_SYS_WIN

namespace Ucnet
{

tpl<MsgType TType, BO TIsPend> dfa MsgType _MsgDatAnyT<TType, TIsPend>::Type() cx
{
    ret TType;
}
tpl<MsgType TType, BO TIsPend> dfa BO _MsgDatAnyT<TType, TIsPend>::IsPend() cx
{
    ret TIsPend;
}

tpl1 dfa BaseMsgCallbDat<T1>::BaseMsgCallbDat(BaseMsgCallbFnT<T1> fn, GA ctx) : fn(fn), ctx(ctx)
{
}

} // namespace Ucnet

#endif
