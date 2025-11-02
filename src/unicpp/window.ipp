#include "window.hpp"

#ifdef PROG_SYS_WIN

tpl<WinMsgType TType> dfa WinMsgType _WinMsgAnyT<TType>::Type() cx
{
    ret TType;
}

#endif
