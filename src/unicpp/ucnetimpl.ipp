#include "ucnetimpl.hpp"

#ifdef PROG_SYS_WIN

namespace ucnet
{

tpl3 dfa BaseMsgCallbDat<T2> _MsgCallbDatCreate(T3&& fn, GA ctx)
{
    static_assert(std::is_invocable_r_v<ER, std::decay_t<T3>, T2&, cx T1&, GA>, "T3 must be callable as ER(T2&, cx T1&, GA)");
    ret BaseMsgCallbDat<T2>{[fn_ = std::forward<T3>(fn)](T2& cli, cx MsgDatAny& msgDat, GA ctx_) { ret fn_(cli, (cx T1&)msgDat, ctx_); }, ctx};
}

tpl<MsgType TMsg, typename TFn> dfa NT Cli::MsgCallbSet(TFn&& fn, GA ctx)
{
    m_msgCallbList[SI(TMsg)] = _MsgCallbDatCreate<typename MsgTypeMap<TMsg>::MsgDatT, Cli>(std::forward<TFn>(fn), ctx);
}
tpl<MsgType TMsg, typename TFn> dfa NT Cli::MsgCallbExSet(TFn&& fn, GA ctx)
{
    m_msgCallbExList[SI(TMsg)] = _MsgCallbDatCreate<typename MsgTypeMap<TMsg>::MsgDatT, Cli>(std::forward<TFn>(fn), ctx);
}

tpl<MsgType TMsg, typename TFn> dfa NT Srv::MsgCallbSet(TFn&& fn, GA ctx)
{
    m_msgCallbList[SI(TMsg)] = _MsgCallbDatCreate<typename MsgTypeMap<TMsg>::MsgDatT, CliSrv>(std::forward<TFn>(fn), ctx);
}

} // namespace ucnet

#endif
