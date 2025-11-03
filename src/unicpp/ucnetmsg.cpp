#include "ucnetmsg.hpp"

#ifdef PROG_SYS_WIN

namespace ucnet
{

dfa ER MsgDatAny::Trans(serial::TransIo& io)
{
    rets;
}
dfa ER MsgDatAny::WriteTo(std::vector<U1>& buf, SI& curI) cx
{
    AU tuple = std::tie(buf, curI);
    AU io = serial::TransIo(&tuple);
    ret const_cast<MsgDatAny*>(tx)->Trans(io);
}
dfa ER MsgDatAny::ReadFrom(cx U1* cur, cx U1* end)
{
    AU tuple = std::tie(cur, end);
    AU io = serial::TransIo(&tuple);
    ret tx->Trans(io);
}

dfa ER MsgDatVerReq::Trans(serial::TransIo& io)
{
    ifep(serial::Trans(io, verCx, verMin, verMax));
    rets;
}
dfa ER MsgDatVerRes::Trans(serial::TransIo& io)
{
    ifep(serial::Trans(io, verCx, ver, msgNumToWrite, msgNumToRead, sessionId, userName));
    rets;
}
dfa ER MsgDatDbg::Trans(serial::TransIo& io)
{
    ifep(serial::Trans(io, text));
    rets;
}
dfa ER MsgDatPing::Trans(serial::TransIo& io)
{
    ifep(serial::Trans(io, time));
    rets;
}
dfa ER MsgDatRoomPresenceReq::Trans(serial::TransIo& io)
{
    ifu (act != Act::LIST && act != Act::JOIN && act != Act::LEAVE)
        rete(ErrVal::NET_MSG_NO_VALID);
    ifep(serial::Trans(io, act));
    if (act == Act::JOIN)
        ifep(serial::Trans(io, name, pwHash));
    rets;
}
dfa ER MsgDatRoomPresenceRes::Trans(serial::TransIo& io)
{
    ifu (act != Act::LIST)
        rete(ErrVal::NET_MSG_NO_VALID);
    ifep(serial::Trans(io, act));
    if (act == Act::LIST)
        ifep(serial::Trans(io, roomList));
    rets;
}
dfa ER MsgDatRoomCfg::Trans(serial::TransIo& io)
{
    ifep(serial::Trans(io, id, act));
    switch (act)
    {
    case Act::NEW:
        ifep(serial::Trans(io, name, pwHash, cliCntMax));
        break;
    case Act::DEL:
        break;
    case Act::UPD_NAME:
        ifep(serial::Trans(io, name));
        break;
    case Act::UPD_PW:
        ifep(serial::Trans(io, pwHash));
        break;
    case Act::UPD_CLI_CNT:
        ifep(serial::Trans(io, cliCntMax));
        break;
    default:
        rete(ErrVal::NET_MSG_NO_VALID);
    }
    rets;
}
dfa ER MsgDatRoomMsg::Trans(serial::TransIo& io)
{
    ifu (act != Act::NEW)
        rete(ErrVal::NET_MSG_NO_VALID);
    ifep(serial::Trans(io, act));
    if (act == Act::NEW)
        ifep(serial::Trans(io, msg));
    rets;
}

} // namespace ucnet

namespace serial
{

tpl0 dfa ER ReadType(cx U1*& cur, cx U1* end, std::vector<ucnet::MsgDatRoomPresenceRes::ListElem>& val)
{
    ZzVarint elemCnt;
    ifep(serial::Read(cur, end, elemCnt));
    val.clear();
    val.resize(elemCnt.val);
    for (AU& room : val)
        ifep(serial::Read(cur, end, room.name, room.cliCnt, room.cliCntMax));
    rets;
}
tpl0 dfa ER WriteType(std::vector<U1>& buf, SI& curI, cx std::vector<ucnet::MsgDatRoomPresenceRes::ListElem>& val)
{
    ifep(serial::Write(buf, curI, ZzVarint(val.size())));
    for (cx AU& elem : val)
        ifep(serial::Write(buf, curI, elem.name, elem.cliCnt, elem.cliCntMax));
    rets;
}

} // namespace serial

#endif
