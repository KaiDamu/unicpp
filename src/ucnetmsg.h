#pragma once

#ifdef PROG_SYS_WIN

namespace Ucnet
{

cxex SI MSG_SIZE_MAX = 4 * BYTE_IN_MB;
cxex U4 MSG_TYPE_VER_CX = 0x1B204B73;

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

using TMsgDatWriteToBuf = ArrSbo<U1, 256>;
struct MsgDatAny
{
    dfa virtual MsgType Type() cx = 0;
    dfa virtual ER WriteTo(TMsgDatWriteToBuf& buf, U1*& bufCur) cx
    {
        rets;
    }
    dfa virtual ER ReadFrom(cx U1* buf, SI size)
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
    BO isReq;
    U4 verCx;
    TProtoVer verMin;
    TProtoVer verMax;
    TMsgNum msgNumToWrite;
    TMsgNum msgNumToRead;
    TSessionId sessionId;
    std::string userName;

    dfa virtual ER WriteTo(TMsgDatWriteToBuf& buf, U1*& bufCur) cx override
    {
        AU datSize = siz(isReq) + siz(verCx) + siz(verMin);
        if (isReq)
            datSize += siz(verMax);
        else
            datSize += siz(msgNumToWrite) + siz(msgNumToRead) + siz(sessionId) + siz(U1) + (userName.size() * siz(CS));
        buf.ReqUpdCur(SI(bufCur - buf.Dat()) + datSize, bufCur);

        MemCpyUpdCur(bufCur, &isReq, siz(isReq));
        MemCpyUpdCur(bufCur, &verCx, siz(verCx));
        MemCpyUpdCur(bufCur, &verMin, siz(verMin));
        if (isReq)
        {
            MemCpyUpdCur(bufCur, &verMax, siz(verMax));
        }
        else
        {
            MemCpyUpdCur(bufCur, &msgNumToWrite, siz(msgNumToWrite));
            MemCpyUpdCur(bufCur, &msgNumToRead, siz(msgNumToRead));
            MemCpyUpdCur(bufCur, &sessionId, siz(sessionId));
            cx AU userNameSize = U1(userName.size() * siz(CS));
            MemCpyUpdCur(bufCur, &userNameSize, siz(userNameSize));
            MemCpyUpdCur(bufCur, userName.data(), userNameSize);
        }
        rets;
    }
    dfa virtual ER ReadFrom(cx U1* buf, SI size) override
    {
        cx AU datSize = siz(isReq) + siz(verCx) + siz(verMin);
        ifu (size < datSize)
            rete(ErrVal::NET_MSG_NO_VALID);
        MemCpyUpdCurSrc(&isReq, buf, siz(isReq));
        MemCpyUpdCurSrc(&verCx, buf, siz(verCx));
        MemCpyUpdCurSrc(&verMin, buf, siz(verMin));
        if (isReq)
        {
            ifu (size < datSize + siz(verMax))
                rete(ErrVal::NET_MSG_NO_VALID);
            MemCpyUpdCurSrc(&verMax, buf, siz(verMax));
        }
        else
        {
            ifu (size < datSize + siz(msgNumToWrite) + siz(msgNumToRead) + siz(sessionId) + siz(U1))
                rete(ErrVal::NET_MSG_NO_VALID);
            MemCpyUpdCurSrc(&msgNumToWrite, buf, siz(msgNumToWrite));
            MemCpyUpdCurSrc(&msgNumToRead, buf, siz(msgNumToRead));
            MemCpyUpdCurSrc(&sessionId, buf, siz(sessionId));
            U1 userNameSize;
            MemCpyUpdCurSrc(&userNameSize, buf, siz(userNameSize));
            ifu (size != datSize + siz(msgNumToWrite) + siz(msgNumToRead) + siz(sessionId) + siz(U1) + userNameSize)
                rete(ErrVal::NET_MSG_NO_VALID);
            userName.assign((cx CS*)buf, userNameSize / siz(CS));
        }
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

    dfa virtual ER WriteTo(TMsgDatWriteToBuf& buf, U1*& bufCur) cx override
    {
        cx AU datSize = SI(text.size()) * siz(CS);
        buf.ReqUpdCur(SI(bufCur - buf.Dat()) + datSize, bufCur);

        MemCpyUpdCur(bufCur, text.data(), SI(text.size()) * siz(CS));
        rets;
    }
    dfa virtual ER ReadFrom(cx U1* buf, SI size) override
    {
        text.assign((cx CS*)buf, size);
        rets;
    }
};
struct MsgDatPing : _MsgDatAnyT<MsgType::PING>
{
    TmMain time;

    dfa virtual ER WriteTo(TMsgDatWriteToBuf& buf, U1*& bufCur) cx override
    {
        cx AU datSize = siz(time);
        buf.ReqUpdCur(SI(bufCur - buf.Dat()) + datSize, bufCur);

        MemCpyUpdCur(bufCur, &time, siz(time));
        rets;
    }
    dfa virtual ER ReadFrom(cx U1* buf, SI size) override
    {
        cx AU datSize = siz(time);
        ifu (size != datSize)
            rete(ErrVal::NET_MSG_NO_VALID);

        MemCpyUpdCurSrc(&time, buf, siz(time));
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
