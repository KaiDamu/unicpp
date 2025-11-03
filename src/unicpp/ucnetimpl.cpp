#include "ucnetimpl.hpp"

#ifdef PROG_SYS_WIN

    #include "random.hpp"
    #include "console.hpp"

namespace ucnet
{

dfa NT _MsgNumGen(MsgNumT& dst, BO isAtAuthElseInit, BO isForCliElseSrv)
{
    // format:
    // 0b ABCD DDDD DDDD DDDD DDDD DDDD DDDD DDDD
    // [A: set if before auth] [B: set if client writes this] [C: increment overflow slot] [D: incremental id]
    if (isAtAuthElseInit)
        RandCrypt(&dst, siz(dst));
    else
        dst = MsgNumT(0x006D72CA);
    dst &= 0x1FFFFFFF;
    if (!isAtAuthElseInit)
        dst |= 0x80000000;
    if (isForCliElseSrv)
        dst |= 0x40000000;
}
dfa S4 _CliThdFn(GA param)
{
    AU& cli = *(Cli*)param;
    while (YES)
    {
        ifu (!cli.IsInit())
            ret 1;
        if (!cli.IsConnected())
        {
            ThdWait(50); // TODO: use a more efficient wait mechanism
            continue;
        }

        std::unique_ptr<MsgDatAny> msgDat;
        ife (cli._Read(msgDat))
        {
            if (ErrLastGet() == ErrVal::NET_CLOSE)
            {
                msgDat = std::make_unique<MsgDatSysSrvDisconnect>();
                cli.m_doDisconnect = YES;
            }
            else
            {
                msgDat = std::make_unique<MsgDatSysErrRead>();
            }
        }
        ife (cli._CallMsgCallbFn(*msgDat.get()))
            cli.m_doDisconnect = YES;
        ifu (cli.m_doDisconnect)
            ife (cli.Disconnect())
                ConWriteDbg("[_CliThdFn] failed to disconnect client");
    }
}
dfa S4 _SrvThdFn(GA param)
{
    AU& srv = *(Srv*)param;
    while (YES)
    {
        ife (srv.Accept())
        {
            ifl (ErrLastGet() == ErrVal::NET_HIGH_CLI)
                ThdWait(100); // this is pretty efficient in practice
            else
                ret 1;
        }
    }
    ret 0;
}
dfa ER _SrvCliThdFn_(CliRef& cliRef)
{
    AU& cliUnsPtr = AsType<CliSrv*>(cliRef.ptrRaw); // WARNING: the pointer is extracted from the reference, must stay valid
    AU& cliUns = *cliUnsPtr;

    cx AU err = cliUns.m_srv->CliThdFn(cliRef);

    std::unique_ptr<MsgDatAny> msgDat = std::make_unique<MsgDatSysCliDisconnect>();
    ife (cliUns.m_srv->_CallMsgCallbFn(cliRef, *msgDat.get()))
        ConWriteDbg("[_SrvCliThdFn] ignoring disconnect callback error on client [%s]", cliUns.m_adr.ToStr().c_str());

    ife (cliUns.m_srv->Release(cliUnsPtr))
        if (ErrLastGet() != ErrVal::NO_INIT)
            rete(ErrLastGet());

    ife (err)
        rete(ErrLastGet()); // NOTE: this is not necessarily the same code paired with 'err'
    rets;
}
dfa S4 _SrvCliThdFn(GA param)
{
    cx AU cliRefHeap = (CliRef*)param;
    cx AU err = _SrvCliThdFn_(*cliRefHeap);
    delete cliRefHeap;
    ife (err)
        ret 1;
    ret 0;
}

dfa NT CliBase::ClrBase(BO isCli)
{
    m_tmpBufWrite.clear();
    m_tmpBufRead.clear();
    m_msgPendListLock.Lock();
    for (cx AU& elem : m_msgPendList)
        if (elem.second.evt != NUL)
            elem.second.evt->Set(YES);
    m_msgPendList.clear();
    m_msgPendListLock.Unlock();
    m_ver = ProtoVerT(0);
    _MsgNumGen(m_msgNumToWrite, NO, isCli);
    _MsgNumGen(m_msgNumToRead, NO, !isCli);
    m_sessionId = SessionIdT(0);
    m_userName.clear();
    m_doDisconnect = NO;
    tx->HdrSizeUpd();
}
dfa NT CliBase::HdrSizeUpd()
{
    m_hdrSize = siz(MsgNumT);
}
dfa ER CliBase::_MsgWriteBase(MsgNumT& msgNum, cx SockTcp& sock, cx MsgDatAny& msgDat, EvtFast* evt)
{
    msgNum = MsgNumT(0);

    cx AU msgType = msgDat.Type();
    cx AU msgIsPend = msgDat.IsPend();

    m_tmpBufWrite.clear();
    SI curI = 0;
    ifep(msgDat.WriteTo(m_tmpBufWrite, curI));
    m_tmpBufWrite.resize(curI);

    cx AU msgNumToWriteOverride = ((msgDat.msgNum == 0) ? m_msgNumToWrite : msgDat.msgNum);

    if (msgIsPend)
    {
        m_msgPendListLock.Lock();
        AU& msgPend = m_msgPendList[msgNumToWriteOverride]; // insert the message into the pending list
        msgPend.evt = evt;
        m_msgPendListLock.Unlock();
    }

    MsgSizeT msgSize = 0;

    std::array<std::span<cx U1>, 4> bufList;
    bufList[0] = std::span<cx U1>((cx U1*)&msgSize, siz(msgSize));
    bufList[1] = std::span<cx U1>((cx U1*)&msgType, siz(msgType));
    bufList[2] = std::span<cx U1>((cx U1*)&msgNumToWriteOverride, siz(msgNumToWriteOverride));
    bufList[3] = m_tmpBufWrite;

    for (cx AU& buf : bufList)
        msgSize += MsgSizeT(buf.size());

    ifep(sock.Write(bufList));

    if (m_msgNumToWrite == msgNumToWriteOverride)
        ++m_msgNumToWrite;
    msgNum = msgNumToWriteOverride;
    rets;
}
dfa ER CliBase::_MsgReadBase(std::unique_ptr<MsgDatAny>& msgDat, cx SockTcp& sock)
{
    msgDat.reset();
    m_tmpBufRead.clear();
    MsgSizeT msgSize;
    SI sizeResult;
    ifep(sock.Read(&msgSize, sizeResult, siz(msgSize), siz(msgSize)));
    ifu (SI(msgSize) < siz(msgSize) || SI(msgSize) > MSG_SIZE_MAX)
        rete(ErrVal::NET_MSG_NO_VALID);
    m_tmpBufRead.resize(msgSize - siz(msgSize));
    ife (sock.Read(m_tmpBufRead.data(), sizeResult, m_tmpBufRead.size(), m_tmpBufRead.size()))
    {
        m_tmpBufRead.clear();
        retep;
    }
    ifu (SI(m_tmpBufRead.size()) < siz(MsgType) + m_hdrSize)
    {
        ConWriteDbg("[MsgRead] buffer size too small");
        rete(ErrVal::NET_MSG_NO_VALID);
    }

    MsgType msgType;
    MsgNumT msgNum;
    AU bufCur = (cx U1*)m_tmpBufRead.data();
    MemCpyUpdCurSrc(&msgType, bufCur, siz(msgType));
    MemCpyUpdCurSrc(&msgNum, bufCur, siz(msgNum));

    BO doIncMsgNumToRead = YES;
    if (msgNum != m_msgNumToRead)
    {
        cx AU& it = m_msgPendList.find(msgNum);
        ifu (it == m_msgPendList.end())
        {
            ConWriteDbg("[MsgRead] msgNum=%d not found in msgPendList (msgNumToRead=%d)", SI(msgNum), SI(m_msgNumToRead));
            rete(ErrVal::NET_MSG_NO_VALID);
        }
        doIncMsgNumToRead = NO;
        m_msgPendListLock.Lock();
        if (it->second.evt != NUL)
            it->second.evt->Set(YES);
        m_msgPendList.erase(it);
        m_msgPendListLock.Unlock();
    }

    using TMsgDatFactory = std::unordered_map<MsgType, std::function<std::unique_ptr<MsgDatAny>()>, StdHasherNoOpe<MsgType>>;
    static cx TMsgDatFactory s_msgDatFactory = {
        {MsgType::VER_REQ, []() { ret std::make_unique<MsgDatVerReq>(); }},
        {MsgType::VER_RES, []() { ret std::make_unique<MsgDatVerRes>(); }},
        {MsgType::DBG, []() { ret std::make_unique<MsgDatDbg>(); }},
        {MsgType::PING, []() { ret std::make_unique<MsgDatPing>(); }},
        {MsgType::ROOM_PRESENCE_REQ, []() { ret std::make_unique<MsgDatRoomPresenceReq>(); }},
        {MsgType::ROOM_PRESENCE_RES, []() { ret std::make_unique<MsgDatRoomPresenceRes>(); }},
        {MsgType::ROOM_CFG, []() { ret std::make_unique<MsgDatRoomCfg>(); }},
        {MsgType::ROOM_MSG, []() { ret std::make_unique<MsgDatRoomMsg>(); }},
    };
    cx AU msgKeyVal = s_msgDatFactory.find(msgType);
    ifu (msgKeyVal == s_msgDatFactory.end())
    {
        ConWriteDbg("[MsgRead] s_msgDatFactory does not contain MsgType=%d", SI(msgType));
        rete(ErrVal::NET_MSG_NO_VALID);
    }

    msgDat = msgKeyVal->second();
    msgDat->msgNum = msgNum;
    ife (msgDat->ReadFrom(bufCur, bufCur + SI(SI(m_tmpBufRead.size()) - (bufCur - m_tmpBufRead.data()))))
    {
        ConWriteDbg("[MsgRead] ReadFrom error");
        retep;
    }

    if (doIncMsgNumToRead)
        ++m_msgNumToRead;
    rets;
}

dfa NT CliBase::MsgResWait(MsgNumT msgNum)
{
    m_msgPendListLock.Lock();
    cx AU& it = m_msgPendList.find(msgNum);
    if (it == m_msgPendList.end())
    {
        m_msgPendListLock.Unlock();
        ret;
    }
    cx AU evt = it->second.evt;
    m_msgPendListLock.Unlock();
    if (evt != NUL)
    {
        evt->Wait();
        ret;
    }
    SI spinCnt = 0;
    while (YES)
    {
        m_msgPendListLock.Lock();
        ifu (m_msgPendList.find(msgNum) == m_msgPendList.end())
        {
            m_msgPendListLock.Unlock();
            ret;
        }
        m_msgPendListLock.Unlock();
        ThdYield<20, 1000>(spinCnt++);
    }
}
dfa NT CliBase::MsgResWaitAll()
{
    SI spinCnt = 0;
    while (YES)
    {
        m_msgPendListLock.Lock();
        ifu (m_msgPendList.empty())
        {
            m_msgPendListLock.Unlock();
            ret;
        }
        m_msgPendListLock.Unlock();
        ThdYield<20, 1000>(spinCnt++);
    }
}

dfa CliBase::CliBase()
{
    tx->ClrBase(NO);
}

dfa ER Cli::_DefaMsgCallbSet()
{
    tx->MsgCallbSet<MsgType::NONE>([](Cli& cli, cx MsgDatAny& msg, GA ctx) {
        ConWriteErr("[Server] Unhandled message [type = %u]", MsgTypeT(msg.Type()));
        rets; // NOTE: as the client, we try to silently ignore unhandled messages
    });
    tx->MsgCallbSet<MsgType::VER_RES>([](Cli& cli, cx MsgDatVerRes& msg, GA ctx) {
        ifu (msg.verCx != MsgDatVerRes::CX)
        {
            ConWriteErr("[Server] Invalid version message [verCx = 0x%08X]", msg.verCx);
            rete(ErrVal::NET_MSG_NO_VALID);
        }
        ifu (msg.ver != PROTO_VER)
        {
            ConWriteErr("[Server] Protocol version not supported [ver = %u, PROTO_VER = %u]", msg.ver, PROTO_VER);
            rete(ErrVal::NET_MSG_NO_VALID);
        }

        cli._AuthToNoUser(msg);

        ConWriteInfo("[Server] Using UcNet-v.%u", msg.ver);
        rets;
    });
    tx->MsgCallbSet<MsgType::SYS_SRV_CONNECT>([](Cli& cli, cx MsgDatSysSrvConnect& msg, GA ctx) {
        ConWriteInfo("[Server] Connected");
        rets;
    });
    tx->MsgCallbSet<MsgType::SYS_SRV_DISCONNECT>([](Cli& cli, cx MsgDatSysSrvDisconnect& msg, GA ctx) {
        ConWriteInfo("[Server] Disconnected");
        rets;
    });
    tx->MsgCallbSet<MsgType::DBG>([](Cli& cli, cx MsgDatDbg& msg, GA ctx) {
        ConWriteInfo("[Server] Responded \"%s\"", msg.text.c_str());
        rets;
    });
    tx->MsgCallbSet<MsgType::PING>([](Cli& cli, cx MsgDatPing& msg, GA ctx) {
        ConWriteInfo("[Server] Ping is %.2f ms", TimeMain() - msg.time);
        rets;
    });

    rets;
}
dfa NT Cli::_AuthToNoUser(cx MsgDatVerRes& msgDat)
{
    m_ver = msgDat.ver;
    m_msgNumToWrite = msgDat.msgNumToWrite;
    m_msgNumToRead = msgDat.msgNumToRead;
    m_sessionId = msgDat.sessionId;
    m_userName = msgDat.userName;
}

dfa ER Cli::_Read(std::unique_ptr<MsgDatAny>& msgDat)
{
    ret tx->_MsgReadBase(msgDat, m_sock);
}
dfa ER Cli::_CallMsgCallbFn(cx MsgDatAny& msgDat)
{
    AU msgTypeI = SI(msgDat.Type());
    ifu (msgTypeI >= SI(m_msgCallbList.size()))
        rete(ErrVal::NET_MSG_NO_VALID);

    // call the message specific callback function & extended callback function if exists
    if (m_msgCallbList[msgTypeI].fn)
    {
        ifep(m_msgCallbList[msgTypeI].fn(*tx, msgDat, m_msgCallbList[msgTypeI].ctx));
        if (m_msgCallbExList[msgTypeI].fn)
        {
            jdst(callEx);
            ret m_msgCallbExList[msgTypeI].fn(*tx, msgDat, m_msgCallbExList[msgTypeI].ctx);
        }
        rets;
    }
    if (m_msgCallbExList[msgTypeI].fn)
        jsrc(callEx);

    // call the default callback function for unhandled message types
    msgTypeI = SI(MsgType::NONE);
    if (m_msgCallbList[msgTypeI].fn)
        ret m_msgCallbList[msgTypeI].fn(*tx, msgDat, m_msgCallbList[msgTypeI].ctx);

    // silently ignore the message if there is no default callback function
    ConWriteDbg("[Cli::_CallMsgCallbFn] ignoring received MsgType=%d without callback function", SI(msgDat.Type()));
    rets;
}

dfa BO Cli::IsInit() cx
{
    ret m_isInit;
}
dfa BO Cli::IsConnected() cx
{
    ret m_srvAdr.ip != 0;
}
dfa BO Cli::IsDisconnecting() cx
{
    ret m_doDisconnect;
}
dfa ER Cli::Connect(cx NetAdrV4& adr)
{
    ifu (tx->IsConnected() || m_sock.IsValid())
        rete(ErrVal::YES_INIT);
    ifep(m_sock.New());
    ife (m_sock.Connect(adr))
    {
        cx AU err = ErrLastGet();
        m_sock.Del();
        rete(err);
    }
    m_srvAdr = adr;
    tx->ClrBase(YES);

    std::unique_ptr<MsgDatAny> msgDat = std::make_unique<MsgDatSysSrvConnect>();
    ifep(tx->_CallMsgCallbFn(*msgDat.get()));

    rets;
}
dfa ER Cli::Disconnect()
{
    if (!tx->IsConnected())
        rets;
    ifu (!m_sock.IsValid())
        rete(ErrVal::NO_INIT);
    ifep(m_sock.Disconnect());
    ifep(m_sock.Del());
    m_srvAdr.Clr();
    tx->ClrBase(YES);
    rets;
}
dfa ER Cli::Init()
{
    ifu (m_isInit)
        rete(ErrVal::YES_INIT);
    m_isInit = YES;
    ife (m_thd.New(_CliThdFn, GA(tx)))
    {
        m_isInit = NO;
        retep;
    }
    rets;
}
dfa ER Cli::Free()
{
    ifu (!m_isInit)
        rets;
    m_isInit = NO;
    ife (m_thd.Wait())
    {
        m_isInit = YES;
        retep;
    }
    ife (m_thd.Close())
    {
        m_isInit = YES;
        retep;
    }
    rets;
}
dfa MsgNumT Cli::MsgWrite(cx MsgDatAny& msgDat, EvtFast* evt)
{
    MsgNumT msgNum;
    ife (tx->_MsgWriteBase(msgNum, m_sock, msgDat, evt))
    {
        if (evt != NUL)
            evt->Set(YES);

        cx AU msgDatErr = std::make_unique<MsgDatSysErrWrite>();
        ife (tx->_CallMsgCallbFn(*msgDatErr.get()))
            m_doDisconnect = YES;
    }
    ret msgNum;
}

dfa Cli::Cli() : m_isInit(NO)
{
    tx->_DefaMsgCallbSet(); // NOTE: error ignored, currently callbacks are tied to the lifetime of the client instance
}
dfa Cli::~Cli()
{
    tx->Disconnect();
}

dfa MsgNumT CliSrv::MsgWrite(cx MsgDatAny& msgDat, EvtFast* evt)
{
    CliRef cliRefUns(tx, tx->m_sessionId); // WARNING: the reference is built from the pointer, might validate what it shouldn't

    ret tx->m_srv->MsgWrite(cliRefUns, msgDat, evt);
}
dfa NT CliSrv::Init(Srv* srv, SockTcp& sock, cx NetAdrV4& adr)
{
    tx->m_srv = srv;
    tx->m_sock.Move(sock);
    tx->m_adr = adr;
    tx->ClrBase(NO);
    tx->m_checksumMode = sec::ChecksumMode::NONE;
    tx->m_encryptMode = sec::EncryptMode::NONE;
    tx->m_srvAuthMode = sec::SrvAuthMode::NONE;
    tx->m_priviLv = PriviLv::INIT;
}

dfa CliSrv::CliSrv()
{
    SockTcp sock;
    NetAdrV4 adr;
    tx->Init(NUL, sock, adr);
}
dfa CliSrv::CliSrv(Srv* srv, SockTcp& sock, cx NetAdrV4& adr)
{
    tx->Init(srv, sock, adr);
}

dfa ER Srv::_DefaMsgCallbSet()
{
    tx->MsgCallbSet<MsgType::NONE>([](CliSrv& cli, cx MsgDatAny& msg, GA ctx) {
        ConWriteErr("[%s] Unhandled message [type = %u]", cli.m_adr.ToStr().c_str(), MsgTypeT(msg.Type()));
        rete(ErrVal::NET_MSG_NO_VALID); // NOTE: since this is the default callback, we return an error directly
    });
    tx->MsgCallbSet<MsgType::VER_REQ>([](CliSrv& cli, cx MsgDatVerReq& msg, GA ctx) {
        ifu (msg.verCx != MsgDatVerReq::CX)
        {
            ConWriteErr("[%s] Invalid version message [verCx = 0x%08X]", cli.m_adr.ToStr().c_str(), msg.verCx);
            rete(ErrVal::NET_MSG_NO_VALID);
        }
        ifu (!IsBetween(PROTO_VER, msg.verMin, msg.verMax))
        {
            ConWriteWarn("[%s] Protocol version not supported [verMin = %u, verMax = %u, PROTO_VER = %u]", cli.m_adr.ToStr().c_str(), msg.verMin, msg.verMax, PROTO_VER);
            rete(ErrVal::NET_NO_SUPPORT); // TODO: the server should send an error message back instead
        }
        ifu (cli.m_priviLv != PriviLv::INIT)
        {
            ConWriteWarn("[%s] Protocol version already established", cli.m_adr.ToStr().c_str());
            rete(ErrVal::NET_YES_INIT); // TODO: the server should send an error message back instead
        }

        cx AU msgNumToWriteOld = cli.m_msgNumToWrite;
        cli.m_srv->CliAuthToNoUser(cli);
        cx AU msgNumToWriteNew = cli.m_msgNumToWrite;

        MsgDatVerRes msgVer;
        msgVer.msgNum = msg.msgNum;
        msgVer.verCx = MsgDatVerRes::CX;
        msgVer.ver = PROTO_VER;
        msgVer.msgNumToWrite = cli.m_msgNumToRead;
        msgVer.msgNumToRead = cli.m_msgNumToWrite;
        msgVer.sessionId = cli.m_sessionId;
        msgVer.userName = cli.m_userName;

        cli.m_msgNumToWrite = msgNumToWriteOld;
        cli.MsgWrite(msgVer);
        cli.m_msgNumToWrite = msgNumToWriteNew;

        ConWriteInfo("[%s] Version established [ver = %u, SID = %016llX, userName = \"%s\"]", cli.m_adr.ToStr().c_str(), msgVer.ver, msgVer.sessionId, msgVer.userName.c_str());
        rets;
    });
    tx->MsgCallbSet<MsgType::SYS_CLI_CONNECT>([](CliSrv& cli, cx MsgDatSysCliConnect& msg, GA ctx) {
        ConWriteInfo("[%s] Connected", cli.m_adr.ToStr().c_str());
        rets;
    });
    tx->MsgCallbSet<MsgType::SYS_CLI_DISCONNECT>([](CliSrv& cli, cx MsgDatSysCliDisconnect& msg, GA ctx) {
        ConWriteInfo("[%s] Disconnected", cli.m_adr.ToStr().c_str());
        rets;
    });
    tx->MsgCallbSet<MsgType::DBG>([](CliSrv& cli, cx MsgDatDbg& msg, GA ctx) {
        ConWriteInfo("[%s] Debug message [text = \"%s\"]", cli.m_adr.ToStr().c_str(), msg.text.c_str());

        MsgDatDbg msgDbg;
        msgDbg.text = msg.text;
        cli.MsgWrite(msgDbg);

        rets;
    });
    tx->MsgCallbSet<MsgType::PING>([](CliSrv& cli, cx MsgDatPing& msg, GA ctx) {
        ConWriteInfo("[%s] Ping [time = %.2f]", cli.m_adr.ToStr().c_str(), msg.time);

        MsgDatPing msgPing;
        msgPing.time = msg.time;
        cli.MsgWrite(msgPing);

        rets;
    });

    rets;
}
dfa ER Srv::_PostReadPreMsgCallbProc(CliSrv& cli, MsgDatAny* msgDat)
{
    ifu (cli.m_priviLv <= PriviLv::INIT)
    {
        if (cli.m_priviLv == PriviLv::NONE)
            rete(ErrVal::NET_MSG_NO_PRIVI);
        cx AU msgType = msgDat->Type();
        if (msgType != MsgType::VER_REQ)
            rete(ErrVal::NET_MSG_NO_PRIVI);
    }
    rets;
}
dfa ER Srv::_CallMsgCallbFn(CliRef& cliRef, cx MsgDatAny& msgDat)
{
    // get the client pointer from the client reference
    CliSrv* cliPtr;
    MthdObjListAu au(cliPtr, m_cliList, cliRef);
    ifu (cliPtr == NUL)
        rete(ErrVal::NET_NO_EXIST);
    AU& cli = *cliPtr;
    // call the message specific callback function
    AU msgTypeI = SI(msgDat.Type());
    ifl (msgTypeI < SI(m_msgCallbList.size()) && m_msgCallbList[msgTypeI].fn)
        ret m_msgCallbList[msgTypeI].fn(cli, msgDat, m_msgCallbList[msgTypeI].ctx);
    // call the default callback function for unhandled message types
    msgTypeI = SI(MsgType::NONE);
    ifl (msgTypeI < SI(m_msgCallbList.size()) && m_msgCallbList[msgTypeI].fn)
        ret m_msgCallbList[msgTypeI].fn(cli, msgDat, m_msgCallbList[msgTypeI].ctx);
    // silently ignore the message if there is no default callback function
    ConWriteDbg("[Srv::_CallMsgCallbFn] ignoring received MsgType=%d from [%s] without callback function", SI(msgDat.Type()), cli.m_adr.ToStr().c_str());
    rets;
}
dfa ER Srv::CliThdFn(CliRef& cliRef)
{
    AU& cliUnsPtr = AsType<CliSrv*>(cliRef.ptrRaw); // WARNING: the pointer is extracted from the reference, must stay valid
    AU& cliUns = *cliUnsPtr;

    {
        std::unique_ptr<MsgDatAny> msgDat = std::make_unique<MsgDatSysCliConnect>();
        ifep(cliUns.m_srv->_CallMsgCallbFn(cliRef, *msgDat.get()));
    }

    while (YES)
    {
        std::unique_ptr<MsgDatAny> msgDat;
        ife (cliUns._MsgReadBase(msgDat, cliUns.m_sock))
        {
            if (ErrLastGet() == ErrVal::NET_CLOSE)
            {
                cliUns.m_doDisconnect = YES;
                jsrc(noSend);
            }
            else
            {
                msgDat = std::make_unique<MsgDatSysErrRead>();
            }
        }
        ife (cliUns.m_srv->_PostReadPreMsgCallbProc(cliUns, msgDat.get()))
        {
            // NOTE: currently, we disconnect the client right away if there is any issue with preprocessing the message
            ConWriteDbg("[Srv::CliThdFn] _PostReadPreMsgCallbProc rejected msg & client from [%s]", cliUns.m_adr.ToStr().c_str());
            cliUns.m_doDisconnect = YES;
            jsrc(noSend);
        }
        ife (cliUns.m_srv->_CallMsgCallbFn(cliRef, *msgDat.get()))
            cliUns.m_doDisconnect = YES;
        jdst(noSend);
        ifu (cliUns.m_doDisconnect) // NOTE: error value currently not returned
            rets;                   // NOTE: this is not necessarily a graceful disconnect, it triggers Release()
    }

    rets;
}
dfa ER Srv::Open(U2 port, SI cliCntMax)
{
    ifu (m_sock.IsValid())
        rete(ErrVal::YES_INIT);
    ifep(m_sock.New());
    ife (m_sock.OpenSrv(port))
    {
        cx AU err = ErrLastGet();
        m_sock.Del();
        rete(err);
    }
    ife (m_thd.New(_SrvThdFn, GA(tx)))
    {
        cx AU err = ErrLastGet();
        m_sock.Del();
        rete(err);
    }
    m_cliList.New(cliCntMax);
    rets;
}
dfa ER Srv::Close()
{
    if (!m_sock.IsValid())
        rets;

    ifep(m_sock.Del()); // stops accepting new clients
    std::vector<CliRef> cliList;
    m_cliList.List(cliList);
    cx AU cliCnt = SI(cliList.size());
    ite (i, i < cliCnt)
    {
        CliSrv* cli;
        MthdObjListAu<CliSrv> au(cli, m_cliList, cliList[i]);
        ifl (cli != NUL)
        {
            ifep(cli->m_sock.Disconnect());
            ifep(cli->m_sock.Del());
            ifep(cli->m_thd.Wait());
        }
    }
    m_cliList.Del();

    ifep(m_thd.Wait());
    ifep(m_thd.Close());
    rets;
}
dfa ER Srv::Accept()
{
    ifu (!m_sock.IsValid())
        rete(ErrVal::NO_INIT);
    SockTcp sockCliTmp;
    NetAdrV4 adrTmp;
    ifep(m_sock.Accept(sockCliTmp, adrTmp));
    // TODO: could use application-level blacklisting here
    CliRef cliNewRef;
    ife (m_cliList.ElemNew(cliNewRef, tx, sockCliTmp, adrTmp))
        rete(ErrVal::NET_HIGH_CLI);
    cx AU cliNewPtr = m_cliList.Get(cliNewRef);
    ifl (cliNewPtr != NUL)
    {
        cx AU cliNewRefHeap = new CliRef(cliNewRef);
        cliNewPtr->m_thd.New(_SrvCliThdFn, GA(cliNewRefHeap)); // WARNING: error ignored
        cliNewPtr->m_thd.Disown();
        m_cliList.Let(cliNewPtr);
    }
    rets;
}
dfa ER Srv::Release(CliSrv*& cli)
{
    ifu (!m_sock.IsValid())
        rete(ErrVal::NO_INIT);
    ifep(cli->m_thd.Close());
    ifep(cli->m_sock.Disconnect());
    ifep(cli->m_sock.Del());
    m_cliList.ElemDel(CliRef(cli, cli->m_sessionId));
    cli = NUL;
    rets;
}
dfa MsgNumT Srv::MsgWrite(CliRef& cliRef, cx MsgDatAny& msgDat, EvtFast* evt)
{
    AU& cliUnsPtr = AsType<CliSrv*>(cliRef.ptrRaw); // WARNING: the pointer is extracted from the reference, must stay valid
    AU& cliUns = *cliUnsPtr;

    MsgNumT msgNum;
    ife (cliUns._MsgWriteBase(msgNum, cliUns.m_sock, msgDat, evt))
    {
        if (evt != NUL)
            evt->Set(YES);

        cx AU msgDatErr = std::make_unique<MsgDatSysErrWrite>();
        ife (tx->_CallMsgCallbFn(cliRef, *msgDatErr.get()))
            cliUns.m_doDisconnect = YES;
    }
    ret msgNum;
}
dfa NT Srv::CliAuthToNoUser(CliSrv& cli)
{
    cli.m_ver = PROTO_VER;
    _MsgNumGen(cli.m_msgNumToWrite, YES, NO);
    _MsgNumGen(cli.m_msgNumToRead, YES, YES);

    jdst(reGenUserName);
    U2 userNameNum;
    RandCrypt(&userNameNum, siz(userNameNum));
    userNameNum %= 10000;
    CS userName[16];
    CsstrSetForm(userName, 16, "anon-%04u", userNameNum);
    ife (m_cliList.IdSecSet(&cli, std::string(userName)))
        jsrc(reGenUserName);

    cli.m_priviLv = PriviLv::NO_USER;
}
dfa Srv::Srv()
{
    tx->_DefaMsgCallbSet(); // NOTE: error ignored, currently callbacks are tied to the lifetime of the server instance
}
dfa Srv::~Srv()
{
    tx->Close();
}

} // namespace ucnet

#endif
