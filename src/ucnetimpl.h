#pragma once

#ifdef PROG_SYS_WIN

namespace Ucnet
{

tpl3 dfa BaseMsgCallbDat<T2> _MsgCallbDatCreate(T3&& fn, GA ctx)
{
    static_assert(std::is_invocable_r_v<ER, std::decay_t<T3>, T2&, cx T1&, GA>, "T3 must be callable as ER(T2&, cx T1&, GA)");
    ret BaseMsgCallbDat<T2>{[fn_ = std::forward<T3>(fn)](T2& cli, cx MsgDatAny& msgDat, GA ctx) { ret fn_(cli, (cx T1&)msgDat, ctx); }, ctx};
}
dfa NT _MsgNumGen(TMsgNum& dst, BO isAtAuthElseInit, BO isForCliElseSrv)
{
    // format:
    // 0b ABCD DDDD DDDD DDDD DDDD DDDD DDDD DDDD
    // [A: set if before auth] [B: set if client writes this] [C: increment overflow slot] [D: incremental id]
    if (isAtAuthElseInit)
        RandCrypt(&dst, siz(dst));
    else
        dst = TMsgNum(0x006D72CA);
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
                cli._Cd().doDisconnect = YES;
            }
            else
            {
                msgDat = std::make_unique<MsgDatSysErrRead>();
            }
        }
        ife (cli._CallMsgCallbFn(*msgDat.get()))
            cli._Cd().doDisconnect = YES;
        ifu (cli.Cd().doDisconnect)
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
dfa S4 _SrvCliThdFn(GA param)
{
    AU cliDat = (Srv::CliDat*)param;
    cx AU err = cliDat->srv->CliThdFn(*cliDat);

    {
        std::unique_ptr<MsgDatAny> msgDat = std::make_unique<MsgDatSysCliDisconnect>();
        ife (cliDat->srv->_CallMsgCallbFn(*cliDat, *msgDat.get()))
            ConWriteDbg("[_SrvCliThdFn] ignoring disconnect callback error on client [%s]", cliDat->adr.ToStr().c_str());
    }

    ife (cliDat->srv->Release(cliDat))
        if (ErrLastGet() != ErrVal::NO_INIT)
            ret 1;

    ife (err)
        ret 1;
    ret 0;
}

dfa NT Cd::Clr(BO isCli)
{
    tmpBufWrite.clear();
    tmpBufRead.clear();
    ver = TProtoVer(0);
    _MsgNumGen(msgNumToWrite, NO, isCli);
    _MsgNumGen(msgNumToRead, NO, !isCli);
    sessionId = TSessionId(0);
    userName.clear();
    doDisconnect = NO;
    tx->HdrSizeUpd();
}
dfa NT Cd::HdrSizeUpd()
{
    hdrSize = siz(TMsgNum);
}
dfa ER Cd::MsgWrite(cx SockTcp& sock, cx MsgDatAny& msgDat)
{
    cx AU msgType = msgDat.Type();

    tmpBufWrite.clear();
    SI curI = 0;
    ifep(msgDat.WriteTo(tmpBufWrite, curI));
    tmpBufWrite.resize(curI);

    TMsgSize msgSize = 0;

    std::array<std::span<cx U1>, 4> bufList;
    bufList[0] = std::span<cx U1>((U1*)&msgSize, siz(msgSize));
    bufList[1] = std::span<cx U1>((U1*)&msgType, siz(msgType));
    bufList[2] = std::span<cx U1>((U1*)&msgNumToWrite, siz(msgNumToWrite));
    bufList[3] = tmpBufWrite;

    for (cx AU& buf : bufList)
        msgSize += TMsgSize(buf.size());

    ifep(sock.Write(bufList));

    ++msgNumToWrite;
    rets;
}
dfa ER Cd::MsgRead(std::unique_ptr<MsgDatAny>& msgDat, cx SockTcp& sock)
{
    msgDat.reset();
    tmpBufRead.clear();
    TMsgSize msgSize;
    SI sizeResult;
    ifep(sock.Read(&msgSize, sizeResult, siz(msgSize), siz(msgSize)));
    ifu (SI(msgSize) < siz(msgSize) || SI(msgSize) > MSG_SIZE_MAX)
        rete(ErrVal::NET_MSG_NO_VALID);
    tmpBufRead.resize(msgSize - siz(msgSize));
    ife (sock.Read(tmpBufRead.data(), sizeResult, tmpBufRead.size(), tmpBufRead.size()))
    {
        tmpBufRead.clear();
        retep;
    }
    ifu (SI(tmpBufRead.size()) < siz(MsgType) + hdrSize)
    {
        ConWriteDbg("[_MsgRead] buffer size too small");
        rete(ErrVal::NET_MSG_NO_VALID);
    }
    MsgType msgType;
    TMsgNum msgNum;
    AU bufCur = (cx U1*)tmpBufRead.data();
    MemCpyUpdCurSrc(&msgType, bufCur, siz(msgType));
    MemCpyUpdCurSrc(&msgNum, bufCur, siz(msgNum));
    ifu (msgNum != msgNumToRead)
        rete(ErrVal::NET_MSG_NO_VALID);
    using TMsgDatFactory = std::unordered_map<MsgType, std::function<std::unique_ptr<MsgDatAny>()>>;
    static cx TMsgDatFactory s_msgDatFactory = {
        {MsgType::VER, []() { ret std::make_unique<MsgDatVer>(); }},
        {MsgType::DBG, []() { ret std::make_unique<MsgDatDbg>(); }},
        {MsgType::PING, []() { ret std::make_unique<MsgDatPing>(); }},
    };
    cx AU msgKeyVal = s_msgDatFactory.find(msgType);
    ifu (msgKeyVal == s_msgDatFactory.end())
    {
        ConWriteDbg("[_MsgRead] s_msgDatFactory does not contain MsgType=%d", SI(msgType));
        rete(ErrVal::NET_MSG_NO_VALID);
    }
    msgDat = msgKeyVal->second();
    ife (msgDat->ReadFrom(bufCur, bufCur + SI(SI(tmpBufRead.size()) - (bufCur - tmpBufRead.data()))))
    {
        ConWriteDbg("[_MsgRead] ReadFrom error");
        retep;
    }
    ++msgNumToRead;
    rets;
}
dfa Cd::Cd()
{
    tx->Clr(NO);
}

dfa ER Cli::_DefaMsgCallbSet()
{
    tx->MsgCallbSet<MsgType::NONE>([](Cli& cli, cx MsgDatAny& msg, GA ctx) {
        ConWriteErr("[Server] Unhandled message [type = %u]", TMsgType(msg.Type()));
        rets; // NOTE: as the client, we try to silently ignore unhandled messages
    });
    tx->MsgCallbSet<MsgType::VER>([](Cli& cli, cx MsgDatVer& msg, GA ctx) {
        ifu (msg.isReq || (msg.verCx != MsgDatVer::CX))
        {
            ConWriteErr("[Server] Invalid version message [isReq = %u, verCx = 0x%08X]", msg.isReq, msg.verCx);
            rete(ErrVal::NET_MSG_NO_VALID);
        }
        ifu (msg.verMin != PROTO_VER)
        {
            ConWriteErr("[Server] Protocol version not supported [verMin = %u, PROTO_VER = %u]", msg.verMin, PROTO_VER);
            rete(ErrVal::NET_MSG_NO_VALID);
        }

        cli._AuthToNoUser(msg);

        ConWriteInfo("[Server] Using UcNet-v.%u", msg.verMin);
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

dfa Cd& Cli::_Cd()
{
    ret m_cd;
}
dfa ER Cli::_Write(cx MsgDatAny& msgDat)
{
    ret m_cd.MsgWrite(m_sock, msgDat);
}
dfa ER Cli::_Read(std::unique_ptr<MsgDatAny>& msgDat)
{
    ret m_cd.MsgRead(msgDat, m_sock);
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
dfa NT Cli::_AuthToNoUser(cx MsgDatVer& msgDat)
{
    m_cd.ver = msgDat.verMin;
    m_cd.msgNumToWrite = msgDat.msgNumToWrite;
    m_cd.msgNumToRead = msgDat.msgNumToRead;
    m_cd.sessionId = msgDat.sessionId;
    m_cd.userName = msgDat.userName;
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
    ret m_cd.doDisconnect;
}
dfa cx Cd& Cli::Cd() cx
{
    ret m_cd;
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
    m_cd.Clr(YES);

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
    m_cd.Clr(YES);
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
tpl<MsgType TMsg, typename TFn> dfa NT Cli::MsgCallbSet(TFn&& fn, GA ctx)
{
    m_msgCallbList[SI(TMsg)] = _MsgCallbDatCreate<typename MsgTypeMap<TMsg>::MsgDatT, Cli>(std::forward<TFn>(fn), ctx);
}
tpl<MsgType TMsg, typename TFn> dfa NT Cli::MsgCallbExSet(TFn&& fn, GA ctx)
{
    m_msgCallbExList[SI(TMsg)] = _MsgCallbDatCreate<typename MsgTypeMap<TMsg>::MsgDatT, Cli>(std::forward<TFn>(fn), ctx);
}

dfa Cli::Cli() : m_isInit(NO)
{
    tx->_DefaMsgCallbSet(); // NOTE: error ignored, currently callbacks are tied to the lifetime of the client instance
}
dfa Cli::~Cli()
{
    tx->Disconnect();
}

dfa NT Srv::CliDat::MsgWrite(cx MsgDatAny& msgDat)
{
    tx->srv->MsgWrite(*tx, msgDat);
}
dfa NT Srv::CliDat::Init(Srv* srv, SockTcp& sock, cx NetAdrV4& adr)
{
    tx->srv = srv;
    tx->sock.Move(sock);
    tx->adr = adr;
    // tx->thd; // thread is initialized outside
    tx->cd.Clr(NO);
    tx->checksumMode = Sec::ChecksumMode::NONE;
    tx->encryptMode = Sec::EncryptMode::NONE;
    tx->srvAuthMode = Sec::SrvAuthMode::NONE;
    tx->priviLv = PriviLv::INIT;
}
dfa Srv::CliDat::CliDat()
{
    SockTcp sock;
    NetAdrV4 adr;
    tx->Init(NUL, sock, adr);
}
dfa Srv::RoomDat::RoomDat()
{
    pwHash.Clr();
    owner = TSessionId(0);
    cliCntMax = 0;
}
dfa ER Srv::_Close()
{
    ifep(m_sock.Del());
    cx AU cliCnt = m_cliBuf.Cnt();
    ite (i, i < cliCnt)
    {
        AU cliDat = *m_cliBuf.Dat();
        ifep(cliDat->sock.Disconnect());
        ifep(cliDat->sock.Del());
        ifep(cliDat->thd.Wait());
        m_cliBuf.ElemDel(cliDat);
    }
    m_cliBuf.Del();
    m_cliMapBySessionId.clear();
    m_cliMapByUserName.clear();
    rets;
}
dfa ER Srv::_DefaMsgCallbSet()
{
    tx->MsgCallbSet<MsgType::NONE>([](CliDat& cli, cx MsgDatAny& msg, GA ctx) {
        ConWriteErr("[%s] Unhandled message [type = %u]", cli.adr.ToStr().c_str(), TMsgType(msg.Type()));
        rete(ErrVal::NET_MSG_NO_VALID); // NOTE: since this is the default callback, we return an error directly
    });
    tx->MsgCallbSet<MsgType::VER>([](CliDat& cli, cx MsgDatVer& msg, GA ctx) {
        ifu (!msg.isReq || (msg.verCx != MsgDatVer::CX))
        {
            ConWriteErr("[%s] Invalid version message [isReq = %u, verCx = 0x%08X]", cli.adr.ToStr().c_str(), msg.isReq, msg.verCx);
            rete(ErrVal::NET_MSG_NO_VALID);
        }
        ifu (!IsBetween(PROTO_VER, msg.verMin, msg.verMax))
        {
            ConWriteWarn("[%s] Protocol version not supported [verMin = %u, verMax = %u, PROTO_VER = %u]", cli.adr.ToStr().c_str(), msg.verMin, msg.verMax, PROTO_VER);
            rete(ErrVal::NET_NO_SUPPORT); // TODO: the server should send an error message back instead
        }
        ifu (cli.priviLv != PriviLv::INIT)
        {
            ConWriteWarn("[%s] Protocol version already established", cli.adr.ToStr().c_str());
            rete(ErrVal::NET_YES_INIT); // TODO: the server should send an error message back instead
        }

        cx AU msgNumToWriteOld = cli.cd.msgNumToWrite;
        cli.srv->CliAuthToNoUser(cli);
        cx AU msgNumToWriteNew = cli.cd.msgNumToWrite;

        MsgDatVer msgVer;
        msgVer.isReq = NO;
        msgVer.verCx = MsgDatVer::CX;
        msgVer.verMin = PROTO_VER;
        msgVer.msgNumToWrite = cli.cd.msgNumToRead;
        msgVer.msgNumToRead = cli.cd.msgNumToWrite;
        msgVer.sessionId = cli.cd.sessionId;
        msgVer.userName = cli.cd.userName;

        cli.cd.msgNumToWrite = msgNumToWriteOld;
        cli.MsgWrite(msgVer);
        cli.cd.msgNumToWrite = msgNumToWriteNew;

        ConWriteInfo("[%s] Version established [VER = %u, SID = %016llX, userName = \"%s\"]", cli.adr.ToStr().c_str(), msgVer.verMin, msgVer.sessionId, msgVer.userName.c_str());
        rets;
    });
    tx->MsgCallbSet<MsgType::SYS_CLI_CONNECT>([](CliDat& cli, cx MsgDatSysCliConnect& msg, GA ctx) {
        ConWriteInfo("[%s] Connected", cli.adr.ToStr().c_str());
        rets;
    });
    tx->MsgCallbSet<MsgType::SYS_CLI_DISCONNECT>([](CliDat& cli, cx MsgDatSysCliDisconnect& msg, GA ctx) {
        ConWriteInfo("[%s] Disconnected", cli.adr.ToStr().c_str());
        rets;
    });
    tx->MsgCallbSet<MsgType::DBG>([](CliDat& cli, cx MsgDatDbg& msg, GA ctx) {
        ConWriteInfo("[%s] Debug message [text = \"%s\"]", cli.adr.ToStr().c_str(), msg.text.c_str());

        MsgDatDbg msgDbg = msg;
        cli.MsgWrite(msgDbg);

        rets;
    });
    tx->MsgCallbSet<MsgType::PING>([](CliDat& cli, cx MsgDatPing& msg, GA ctx) {
        ConWriteInfo("[%s] Ping [time = %.2f]", cli.adr.ToStr().c_str(), msg.time);

        MsgDatPing msgPing = msg;
        cli.MsgWrite(msgPing);

        rets;
    });

    rets;
}
dfa ER Srv::_PostReadPreMsgCallbProc(CliDat& cliDat, MsgDatAny* msgDat)
{
    ifu (cliDat.priviLv <= PriviLv::INIT)
    {
        if (cliDat.priviLv == PriviLv::NONE)
            rete(ErrVal::NET_MSG_NO_PRIVI);
        cx AU msgType = msgDat->Type();
        if (msgType != MsgType::VER)
            rete(ErrVal::NET_MSG_NO_PRIVI);
    }
    rets;
}
dfa ER Srv::_CallMsgCallbFn(CliDat& cliDat, cx MsgDatAny& msgDat)
{
    // call the message specific callback function
    AU msgTypeI = SI(msgDat.Type());
    ifl (msgTypeI < SI(m_msgCallbList.size()) && m_msgCallbList[msgTypeI].fn)
        ret m_msgCallbList[msgTypeI].fn(cliDat, msgDat, m_msgCallbList[msgTypeI].ctx);
    // call the default callback function for unhandled message types
    msgTypeI = SI(MsgType::NONE);
    ifl (msgTypeI < SI(m_msgCallbList.size()) && m_msgCallbList[msgTypeI].fn)
        ret m_msgCallbList[msgTypeI].fn(cliDat, msgDat, m_msgCallbList[msgTypeI].ctx);
    // silently ignore the message if there is no default callback function
    ConWriteDbg("[Srv::_CallMsgCallbFn] ignoring received MsgType=%d from [%s] without callback function", SI(msgDat.Type()), cliDat.adr.ToStr().c_str());
    rets;
}
dfa ER Srv::CliThdFn(CliDat& cliDat)
{
    {
        std::unique_ptr<MsgDatAny> msgDat = std::make_unique<MsgDatSysCliConnect>();
        ifep(cliDat.srv->_CallMsgCallbFn(cliDat, *msgDat.get()));
    }

    while (YES)
    {
        std::unique_ptr<MsgDatAny> msgDat;
        ife (cliDat.cd.MsgRead(msgDat, cliDat.sock))
        {
            if (ErrLastGet() == ErrVal::NET_CLOSE)
            {
                cliDat.cd.doDisconnect = YES;
                jsrc(noSend);
            }
            else
            {
                msgDat = std::make_unique<MsgDatSysErrRead>();
            }
        }
        ife (cliDat.srv->_PostReadPreMsgCallbProc(cliDat, msgDat.get()))
        {
            // NOTE: currently, we disconnect the client right away if there is any issue with preprocessing the message
            ConWriteDbg("[Srv::CliThdFn] _PostReadPreMsgCallbProc rejected msg & client from [%s]", cliDat.adr.ToStr().c_str());
            cliDat.cd.doDisconnect = YES;
            jsrc(noSend);
        }
        ife (cliDat.srv->_CallMsgCallbFn(cliDat, *msgDat.get()))
            cliDat.cd.doDisconnect = YES;
        jdst(noSend);
        ifu (cliDat.cd.doDisconnect) // NOTE: error value currently not returned
            rets;                    // NOTE: this is not necessarily a graceful disconnect, it triggers Release()
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
    m_cliBuf.New(cliCntMax);
    rets;
}
dfa ER Srv::Close()
{
    if (!m_sock.IsValid())
        rets;
    m_cliListLock.Lock();
    cx AU err = tx->_Close();
    m_cliListLock.Unlock();
    ifep(err);
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
    m_cliListLock.Lock();
    cx AU cliDatNew = m_cliBuf.ElemNew();
    m_cliListLock.Unlock();
    ifu (cliDatNew == NUL)
        rete(ErrVal::NET_HIGH_CLI);
    cliDatNew->Init(tx, sockCliTmp, adrTmp);
    ife (cliDatNew->thd.New(_SrvCliThdFn, GA(cliDatNew)))
    {
        m_cliListLock.Lock();
        m_cliBuf.ElemDel(cliDatNew);
        m_cliListLock.Unlock();
        retep;
    }
    cliDatNew->thd.Disown();
    rets;
}
dfa ER Srv::Release(CliDat*& cliDat)
{
    ifu (!m_sock.IsValid())
        rete(ErrVal::NO_INIT);
    ifep(cliDat->thd.Close());
    ifep(cliDat->sock.Disconnect());
    ifep(cliDat->sock.Del());
    m_cliListLock.Lock();
    if (cliDat->cd.sessionId != TSessionId(0))
    {
        m_cliMapBySessionId.erase(cliDat->cd.sessionId);
        m_cliMapByUserName.erase(cliDat->cd.userName);
    }
    m_cliBuf.ElemDel(cliDat);
    m_cliListLock.Unlock();
    cliDat = NUL;
    rets;
}
dfa NT Srv::MsgWrite(CliDat& cliDat, cx MsgDatAny& msgDat)
{
    ife (cliDat.cd.MsgWrite(cliDat.sock, msgDat))
    {
        cx AU msgDat = std::make_unique<MsgDatSysErrWrite>();
        ife (tx->_CallMsgCallbFn(cliDat, *msgDat.get()))
            cliDat.cd.doDisconnect = YES;
    }
}
dfa NT Srv::CliAuthToNoUser(CliDat& cliDat)
{
    cliDat.cd.ver = PROTO_VER;
    _MsgNumGen(cliDat.cd.msgNumToWrite, YES, NO);
    _MsgNumGen(cliDat.cd.msgNumToRead, YES, YES);

    m_cliListLock.Lock();

    jdst(retrySessionId);
    RandCrypt(&cliDat.cd.sessionId, siz(cliDat.cd.sessionId));
    ifu (m_cliMapBySessionId.find(cliDat.cd.sessionId) != m_cliMapBySessionId.end())
        jsrc(retrySessionId);
    m_cliMapBySessionId[cliDat.cd.sessionId] = &cliDat;

    jdst(retryUserName);
    U2 userNameNum;
    RandCrypt(&userNameNum, siz(userNameNum));
    userNameNum %= 10000;
    CS userName[16];
    CsstrSetForm(userName, "anon-%04u", userNameNum);
    cliDat.cd.userName = userName;
    ifu (m_cliMapByUserName.find(cliDat.cd.userName) != m_cliMapByUserName.end())
        jsrc(retryUserName);
    m_cliMapByUserName[cliDat.cd.userName] = &cliDat;

    m_cliListLock.Unlock();

    cliDat.priviLv = PriviLv::NO_USER;
}
tpl<MsgType TMsg, typename TFn> dfa NT Srv::MsgCallbSet(TFn&& fn, GA ctx)
{
    m_msgCallbList[SI(TMsg)] = _MsgCallbDatCreate<typename MsgTypeMap<TMsg>::MsgDatT, Srv::CliDat>(std::forward<TFn>(fn), ctx);
}
dfa Srv::Srv()
{
    tx->_DefaMsgCallbSet(); // NOTE: error ignored, currently callbacks are tied to the lifetime of the server instance
}
dfa Srv::~Srv()
{
    tx->Close();
}

} // namespace Ucnet

#endif
