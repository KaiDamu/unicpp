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
    SUC,                // success message, this is the main message type for explicit success
    ERR,                // error message, this is the main message type for explicit errors
    VER_REQ,            // establish protocol version and common state
    VER_RES,            // .
    SYS_ERR_WRITE = 10, // internal error while writing a message
    SYS_ERR_READ,       // internal error while reading a message
    SYS_CLI_CONNECT,    // a new client connected to the server (raw connection established)
    SYS_CLI_DISCONNECT, // a client disconnected from the server (raw connection closed)
    SYS_SRV_CONNECT,    // the server connected to the client (raw connection established)
    SYS_SRV_DISCONNECT, // the server disconnected from the client (raw connection closed)
    DBG = 30,           // debug message for testing purposes
    PING,               // ping to check latency and keep the connection alive
    ROOM_PRESENCE_REQ,  // join or leave a room, or list them
    ROOM_PRESENCE_RES,  // .
    ROOM_CFG,           // create or update room configuration
    ROOM_MSG,           // operations on a room's message, like sending a message to a room
    CNT,                // not a valid message type, used for counting the number of message types
};

struct MsgDatAny
{
    dfa virtual MsgType Type() cx = 0;
    dfa virtual ER Trans(Serial::TransIo& io)
    {
        rets;
    }
    dfa ER WriteTo(std::vector<U1>& buf, SI& curI) cx
    {
        AU tuple = std::tie(buf, curI);
        AU io = Serial::TransIo(&tuple);
        ret const_cast<MsgDatAny*>(tx)->Trans(io);
    }
    dfa ER ReadFrom(cx U1* cur, cx U1* end)
    {
        AU tuple = std::tie(cur, end);
        AU io = Serial::TransIo(&tuple);
        ret tx->Trans(io);
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

struct MsgDatSuc : _MsgDatAnyT<MsgType::SUC>
{
};
struct MsgDatErr : _MsgDatAnyT<MsgType::ERR>
{
};
struct MsgDatVerReq : _MsgDatAnyT<MsgType::VER_REQ>
{
    static cxex U4 CX = 0x1B204B73;

    U4 verCx;
    TProtoVer verMin;
    TProtoVer verMax;

    dfa ER Trans(Serial::TransIo& io) override final
    {
        ifep(Serial::Trans(io, verCx, verMin, verMax));
        rets;
    }
};
struct MsgDatVerRes : _MsgDatAnyT<MsgType::VER_RES>
{
    static cxex U4 CX = 0x734B201B;

    U4 verCx;
    TProtoVer ver;
    TMsgNum msgNumToWrite;
    TMsgNum msgNumToRead;
    TSessionId sessionId;
    std::string userName;

    dfa ER Trans(Serial::TransIo& io) override final
    {
        ifep(Serial::Trans(io, verCx, ver, msgNumToWrite, msgNumToRead, sessionId, userName));
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

    dfa ER Trans(Serial::TransIo& io) override final
    {
        ifep(Serial::Trans(io, text));
        rets;
    }
};
struct MsgDatPing : _MsgDatAnyT<MsgType::PING>
{
    TmMain time;

    dfa ER Trans(Serial::TransIo& io) override final
    {
        ifep(Serial::Trans(io, time));
        rets;
    }
};
struct MsgDatRoomPresenceReq : _MsgDatAnyT<MsgType::ROOM_PRESENCE_REQ>
{
    enum class Act : U1
    {
        LIST = 1, // list all visible rooms
        JOIN,     // join a room
        LEAVE,    // leave the room
    };

    Act act;
    std::string name;
    Sha512Hash pwHash;
};
struct MsgDatRoomPresenceRes : _MsgDatAnyT<MsgType::ROOM_PRESENCE_RES>
{
    enum class Act : U1
    {
        LIST = 1, // list all visible rooms
    };

    struct ListElem
    {
        std::string name;
        SI cliCnt;
        SI cliCntMax;
    };

    Act act;
    std::vector<ListElem> roomList;
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
_MsgTypeMapDef(SUC, MsgDatSuc);
_MsgTypeMapDef(ERR, MsgDatErr);
_MsgTypeMapDef(VER_REQ, MsgDatVerReq);
_MsgTypeMapDef(VER_RES, MsgDatVerRes);
_MsgTypeMapDef(SYS_ERR_WRITE, MsgDatSysErrWrite);
_MsgTypeMapDef(SYS_ERR_READ, MsgDatSysErrRead);
_MsgTypeMapDef(SYS_CLI_CONNECT, MsgDatSysCliConnect);
_MsgTypeMapDef(SYS_CLI_DISCONNECT, MsgDatSysCliDisconnect);
_MsgTypeMapDef(SYS_SRV_CONNECT, MsgDatSysSrvConnect);
_MsgTypeMapDef(SYS_SRV_DISCONNECT, MsgDatSysSrvDisconnect);
_MsgTypeMapDef(DBG, MsgDatDbg);
_MsgTypeMapDef(PING, MsgDatPing);
_MsgTypeMapDef(ROOM_PRESENCE_REQ, MsgDatRoomPresenceReq);
_MsgTypeMapDef(ROOM_PRESENCE_RES, MsgDatRoomPresenceRes);
_MsgTypeMapDef(ROOM_CFG, MsgDatRoomCfg);
_MsgTypeMapDef(ROOM_MSG, MsgDatRoomMsg);

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
