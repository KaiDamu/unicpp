#pragma once

#ifdef PROG_SYS_WIN

namespace Ucnet
{

cxex SI MSG_SIZE_MAX = 4 * BYTE_IN_MB;

using TMsgSize = U4;
using TMsgNum = U4;

using TMsgType = U2;
enum class MsgType : TMsgType
{
    NONE = 0,           // no message type specified, also used for unhandled messages
    ERR,                // error message, this is the main message type for errors
    VER,                // establish protocol version and common state
    SYS_ERR_WRITE = 10, // internal error while writing a message
    SYS_ERR_READ,       // internal error while reading a message
    SYS_CLI_CONNECT,    // a new client connected to the server (raw connection established)
    SYS_CLI_DISCONNECT, // a client disconnected from the server (raw connection closed)
    SYS_SRV_CONNECT,    // the server connected to the client (raw connection established)
    SYS_SRV_DISCONNECT, // the server disconnected from the client (raw connection closed)
    DBG = 30,           // debug message for testing purposes
    PING,               // ping to check latency and keep the connection alive
    ROOM_PRESENCE,      // join or leave a room, or list them
    ROOM_CFG,           // create or update room configuration
    ROOM_MSG,           // operations on a room's message, like sending a message to a room
    CNT,                // not a valid message type, used for counting the number of message types
};

struct MsgDatAny
{
    dfa virtual MsgType Type() cx = 0;
    dfa virtual ER WriteTo(std::vector<U1>& buf, SI& curI) cx
    {
        rets;
    }
    dfa virtual ER ReadFrom(cx U1* cur, cx U1* end)
    {
        rets;
    }
    dfa virtual ~MsgDatAny() = default;
};
tpl<MsgType T> struct _MsgDatAnyT : MsgDatAny
{
    dfa MsgType Type() cx override final
    {
        ret T;
    }
};

struct MsgDatVer : _MsgDatAnyT<MsgType::VER>
{
    static cxex U4 CX = 0x1B204B73;

    BO isReq;
    U4 verCx;
    TProtoVer verMin;
    TProtoVer verMax;
    TMsgNum msgNumToWrite;
    TMsgNum msgNumToRead;
    TSessionId sessionId;
    std::string userName;

    dfa virtual ER WriteTo(std::vector<U1>& buf, SI& curI) cx override
    {
        ifep(Serial::Write(buf, curI, isReq, verCx, verMin));
        if (isReq)
            ifep(Serial::Write(buf, curI, verMax));
        else
            ifep(Serial::Write(buf, curI, msgNumToWrite, msgNumToRead, sessionId, userName));
        rets;
    }
    dfa virtual ER ReadFrom(cx U1* cur, cx U1* end) override
    {
        ifep(Serial::Read(cur, end, isReq, verCx, verMin));
        if (isReq)
            ifep(Serial::Read(cur, end, verMax));
        else
            ifep(Serial::Read(cur, end, msgNumToWrite, msgNumToRead, sessionId, userName));
        rets;
    }
};
struct MsgDatSysErrWrite : _MsgDatAnyT<MsgType::SYS_ERR_WRITE>
{
};
struct MsgDatSysErrRead : _MsgDatAnyT<MsgType::SYS_ERR_READ>
{
};
struct MsgDatSysCliConnect : _MsgDatAnyT<MsgType::SYS_CLI_CONNECT>
{
};
struct MsgDatSysCliDisconnect : _MsgDatAnyT<MsgType::SYS_CLI_DISCONNECT>
{
};
struct MsgDatSysSrvConnect : _MsgDatAnyT<MsgType::SYS_SRV_CONNECT>
{
};
struct MsgDatSysSrvDisconnect : _MsgDatAnyT<MsgType::SYS_SRV_DISCONNECT>
{
};
struct MsgDatDbg : _MsgDatAnyT<MsgType::DBG>
{
    std::string text;

    dfa virtual ER WriteTo(std::vector<U1>& buf, SI& curI) cx override
    {
        ifep(Serial::Write(buf, curI, text));
        rets;
    }
    dfa virtual ER ReadFrom(cx U1* cur, cx U1* end) override
    {
        ifep(Serial::Read(cur, end, text));
        rets;
    }
};
struct MsgDatPing : _MsgDatAnyT<MsgType::PING>
{
    TmMain time;

    dfa virtual ER WriteTo(std::vector<U1>& buf, SI& curI) cx override
    {
        ifep(Serial::Write(buf, curI, time));
        rets;
    }
    dfa virtual ER ReadFrom(cx U1* cur, cx U1* end) override
    {
        ifep(Serial::Read(cur, end, time));
        rets;
    }
};
struct MsgDatRoomPresence : _MsgDatAnyT<MsgType::ROOM_PRESENCE>
{
};
struct MsgDatRoomCfg : _MsgDatAnyT<MsgType::ROOM_CFG>
{
};
struct MsgDatRoomMsg : _MsgDatAnyT<MsgType::ROOM_MSG>
{
};

tpl<MsgType> struct MsgTypeMap;

    #define _MsgTypeMapDef(typeVal, datT)        \
        tpl0 struct MsgTypeMap<MsgType::typeVal> \
        {                                        \
            using MsgDatT = datT;                \
        };

_MsgTypeMapDef(NONE, MsgDatAny);
_MsgTypeMapDef(VER, MsgDatVer);
_MsgTypeMapDef(SYS_ERR_READ, MsgDatSysErrRead);
_MsgTypeMapDef(SYS_CLI_CONNECT, MsgDatSysCliConnect);
_MsgTypeMapDef(SYS_CLI_DISCONNECT, MsgDatSysCliDisconnect);
_MsgTypeMapDef(SYS_SRV_CONNECT, MsgDatSysSrvConnect);
_MsgTypeMapDef(SYS_SRV_DISCONNECT, MsgDatSysSrvDisconnect);
_MsgTypeMapDef(DBG, MsgDatDbg);
_MsgTypeMapDef(PING, MsgDatPing);

// return ERR_YES to disconnect
tpl1 using BaseMsgCallbFnT = std::function<ER(T1& cli, cx MsgDatAny& msgDat, GA ctx)>;

tpl1 struct BaseMsgCallbDat
{
    BaseMsgCallbFnT<T1> fn;
    GA ctx;

    dfa BaseMsgCallbDat(BaseMsgCallbFnT<T1> fn = {}, GA ctx = NUL) : fn(fn), ctx(ctx)
    {
    }
};

} // namespace Ucnet

#endif
