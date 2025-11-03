#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "ucnetcli.hpp"
    #include "ucnetsrv.hpp"
#endif

#ifdef PROG_SYS_WIN

namespace ucnet
{

tpl3 dfa BaseMsgCallbDat<T2> _MsgCallbDatCreate(T3&& fn, GA ctx);
dfa NT _MsgNumGen(MsgNumT& dst, BO isAtAuthElseInit, BO isForCliElseSrv);
dfa S4 _CliThdFn(GA param);
dfa S4 _SrvThdFn(GA param);
dfa ER _SrvCliThdFn_(CliRef& cliRef);
dfa S4 _SrvCliThdFn(GA param);

} // namespace ucnet

#endif

#include "ucnetimpl.ipp"
