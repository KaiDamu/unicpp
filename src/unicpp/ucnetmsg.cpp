#include "ucnetmsg.hpp"

#ifdef PROG_SYS_WIN

namespace Ucnet
{

dfa ER MsgDatAny::Trans(Serial::TransIo& io)
{
    rets;
}
dfa ER MsgDatAny::WriteTo(std::vector<U1>& buf, SI& curI) cx
{
    AU tuple = std::tie(buf, curI);
    AU io = Serial::TransIo(&tuple);
    ret const_cast<MsgDatAny*>(tx)->Trans(io);
}
dfa ER MsgDatAny::ReadFrom(cx U1* cur, cx U1* end)
{
    AU tuple = std::tie(cur, end);
    AU io = Serial::TransIo(&tuple);
    ret tx->Trans(io);
}

dfa ER MsgDatVerReq::Trans(Serial::TransIo& io)
{
    ifep(Serial::Trans(io, verCx, verMin, verMax));
    rets;
}
dfa ER MsgDatVerRes::Trans(Serial::TransIo& io)
{
    ifep(Serial::Trans(io, verCx, ver, msgNumToWrite, msgNumToRead, sessionId, userName));
    rets;
}
dfa ER MsgDatDbg::Trans(Serial::TransIo& io)
{
    ifep(Serial::Trans(io, text));
    rets;
}
dfa ER MsgDatPing::Trans(Serial::TransIo& io)
{
    ifep(Serial::Trans(io, time));
    rets;
}
dfa ER MsgDatRoomPresenceReq::Trans(Serial::TransIo& io)
{
    ifu (act != Act::LIST && act != Act::JOIN && act != Act::LEAVE)
        rete(ErrVal::NET_MSG_NO_VALID);
    ifep(Serial::Trans(io, act));
    if (act == Act::JOIN)
        ifep(Serial::Trans(io, name, pwHash));
    rets;
}
dfa ER MsgDatRoomPresenceRes::Trans(Serial::TransIo& io)
{
    ifu (act != Act::LIST)
        rete(ErrVal::NET_MSG_NO_VALID);
    ifep(Serial::Trans(io, act));
    if (act == Act::LIST)
        ifep(Serial::Trans(io, roomList));
    rets;
}
dfa ER MsgDatRoomCfg::Trans(Serial::TransIo& io)
{
    ifep(Serial::Trans(io, id, act));
    switch (act)
    {
    case Act::NEW:
        ifep(Serial::Trans(io, name, pwHash, cliCntMax));
        break;
    case Act::DEL:
        break;
    case Act::UPD_NAME:
        ifep(Serial::Trans(io, name));
        break;
    case Act::UPD_PW:
        ifep(Serial::Trans(io, pwHash));
        break;
    case Act::UPD_CLI_CNT:
        ifep(Serial::Trans(io, cliCntMax));
        break;
    default:
        rete(ErrVal::NET_MSG_NO_VALID);
    }
    rets;
}
dfa ER MsgDatRoomMsg::Trans(Serial::TransIo& io)
{
    ifu (act != Act::NEW)
        rete(ErrVal::NET_MSG_NO_VALID);
    ifep(Serial::Trans(io, act));
    if (act == Act::NEW)
        ifep(Serial::Trans(io, msg));
    rets;
}

} // namespace Ucnet

namespace Serial
{

tpl0 dfa ER ReadType(cx U1*& cur, cx U1* end, std::vector<Ucnet::MsgDatRoomPresenceRes::ListElem>& val)
{
    ZzVarint elemCnt;
    ifep(Serial::Read(cur, end, elemCnt));
    val.clear();
    val.resize(elemCnt.val);
    for (AU& room : val)
        ifep(Serial::Read(cur, end, room.name, room.cliCnt, room.cliCntMax));
    rets;
}
tpl0 dfa ER WriteType(std::vector<U1>& buf, SI& curI, cx std::vector<Ucnet::MsgDatRoomPresenceRes::ListElem>& val)
{
    ifep(Serial::Write(buf, curI, ZzVarint(val.size())));
    for (cx AU& elem : val)
        ifep(Serial::Write(buf, curI, elem.name, elem.cliCnt, elem.cliCntMax));
    rets;
}

} // namespace Serial

#endif
