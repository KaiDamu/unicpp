#pragma once

#ifdef PROG_SYS_WIN

class Ucnet
{
  public:
    using TProtoVer = U2;
    using TMsgSize = U4;
    using TMsgType = U2;
    using TPriviLv = U1;
    using TMsgDatWriteToBuf = ArrSbo<U1, 256>;

    static cxex TProtoVer PROTO_VER = 1;
    static cxex SI MSG_SIZE_MAX = 4 * BYTE_IN_MB;

    enum class MsgType : TMsgType
    {
        NONE = 0,
        SYS_ERR_READ,
        SYS_CLI_CONNECT,
        SYS_CLI_DISCONNECT,
        SYS_SRV_CONNECT,
        SYS_SRV_DISCONNECT,
        DBG,
        PING,
        CNT,
    };
    enum class PriviLv : TPriviLv
    {
        NONE = 0,
        NO_USER,
        USER,
        HIGH_USER,
        MODERATOR,
        ADMIN,
        SYSTEM,
    };

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
            cx AU textLen = SI(text.size());
            buf.ReqUpdCur(SI(bufCur - buf.Dat()) + textLen, bufCur);
            MemCpy(bufCur, text.data(), textLen);
            bufCur += textLen;
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
            MemCpy(bufCur, &time, datSize);
            bufCur += datSize;
            rets;
        }
        dfa virtual ER ReadFrom(cx U1* buf, SI size) override
        {
            ifu (size != siz(TmMain))
                rete(ErrVal::NET_MSG_NO_VALID);
            MemCpy(&time, buf, siz(TmMain));
            rets;
        }
    };

    tpl<MsgType> struct MsgTypeMap;

    #define _MsgTypeMapDef(typeVal, datT)        \
        tpl0 struct MsgTypeMap<MsgType::typeVal> \
        {                                        \
            using MsgDatT = datT;                \
        };

    _MsgTypeMapDef(NONE, MsgDatAny);
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

    class Cli
    {
      public:
        using MsgCallbFnT = BaseMsgCallbFnT<Cli>;
        using MsgCallbDat = BaseMsgCallbDat<Cli>;

      private:
        BO m_isInit;
        SockTcp m_sock;
        NetAdrV4 m_srvAdr;
        Thd m_thd;
        std::array<MsgCallbDat, TMsgType(MsgType::CNT)> m_msgCallbList;

      public:
        dfa ER _Write(cx MsgDatAny& msgDat) cx;
        dfa ER _Read(std::unique_ptr<MsgDatAny>& msgDat) cx;
        dfa ER _CallMsgCallbFn(MsgDatAny* msgDat);

      public:
        dfa BO IsInit() cx;
        dfa BO IsConnected() cx;
        dfa ER Connect(cx NetAdrV4& adr);
        dfa ER Disconnect();
        dfa ER Init();
        dfa ER Free();
        tpl<Ucnet::MsgType TMsg, typename TFn> dfa NT MsgCallbSet(TFn&& fn, GA ctx = NUL);

      public:
        dfa Cli();
        dfa ~Cli();
    };

    class Srv
    {
      public:
        struct CliDat
        {
            Srv* srv;
            SockTcp sock;
            NetAdrV4 adr;
            Thd thd;

            dfa CliDat();
        };

        using MsgCallbFnT = BaseMsgCallbFnT<CliDat>;
        using MsgCallbDat = BaseMsgCallbDat<CliDat>;

      private:
        SockTcp m_sock;
        ArrSFree<CliDat> m_cliBuf;
        ThdLockFast m_cliBufLock;
        Thd m_thd;
        std::array<MsgCallbDat, TMsgType(MsgType::CNT)> m_msgCallbList;

      private:
        dfa ER _Close();

      public:
        dfa ER _CallMsgCallbFn(CliDat& cliDat, MsgDatAny* msgDat);

      public:
        dfa ER CliThdFn(CliDat& cliDat);

      public:
        dfa ER Open(U2 port, SI cliCntMax);
        dfa ER Close();
        dfa ER Accept();
        dfa ER Release(CliDat*& cliDat);
        tpl<Ucnet::MsgType TMsg, typename TFn> dfa NT MsgCallbSet(TFn&& fn, GA ctx = NUL);

      public:
        dfa Srv();
        dfa ~Srv();
    };
};

tpl3 dfa Ucnet::BaseMsgCallbDat<T2> _MsgCallbDatCreate(T3&& fn, GA ctx)
{
    static_assert(std::is_invocable_r_v<ER, std::decay_t<T3>, T2&, cx T1&, GA>, "T3 must be callable as ER(T2&, cx T1&, GA)");
    ret Ucnet::BaseMsgCallbDat<T2>{[fn_ = std::forward<T3>(fn)](T2& cli, cx Ucnet::MsgDatAny& msgDat, GA ctx) { ret fn_(cli, (cx T1&)msgDat, ctx); }, ctx};
}

dfa ER _UcnetSockWrite(cx SockTcp& sock, CXGA buf, SI size)
{
    ifu (size < siz(Ucnet::TMsgType))
        rete(ErrVal::NET_MSG_NO_VALID);
    cx AU msgSize = Ucnet::TMsgSize(siz(Ucnet::TMsgSize) + size);
    ArrDFast<U1> bufTmp(msgSize);
    MemCpy(bufTmp.Dat(), &msgSize, siz(msgSize));
    MemCpy(bufTmp.Dat() + siz(msgSize), buf, size);
    ife (sock.Write(bufTmp.Dat(), bufTmp.Size()))
        retep;
    rets;
}
dfa ER _UcnetSockRead(std::vector<U1>& buf, cx SockTcp& sock)
{
    buf.clear();
    Ucnet::TMsgSize msgSize;
    SI sizeResult;
    ife (sock.Read(&msgSize, sizeResult, siz(msgSize), siz(msgSize)))
        retep;
    ifu (SI(msgSize) < siz(msgSize) + siz(Ucnet::TMsgType) || SI(msgSize) > Ucnet::MSG_SIZE_MAX)
        rete(ErrVal::NET_MSG_NO_VALID);
    buf.resize(msgSize - siz(msgSize));
    ife (sock.Read(buf.data(), sizeResult, buf.size(), buf.size()))
    {
        buf.clear();
        retep;
    }
    rets;
}
dfa ER _UcnetMsgWrite(cx SockTcp& sock, cx Ucnet::MsgDatAny& msgDat)
{
    Ucnet::TMsgDatWriteToBuf buf;
    cx AU msgType = msgDat.Type();
    MemCpy(buf.Dat(), &msgType, siz(msgType));
    AU bufCur = buf.Dat() + siz(msgType);
    ife (msgDat.WriteTo(buf, bufCur))
        retep;
    ife (_UcnetSockWrite(sock, buf.Dat(), SI(bufCur - buf.Dat()) * siz(U1)))
        retep;
    rets;
}
dfa ER _UcnetMsgRead(std::unique_ptr<Ucnet::MsgDatAny>& msgDat, cx SockTcp& sock)
{
    msgDat.reset();
    std::vector<U1> buf;
    ife (_UcnetSockRead(buf, sock))
        retep;
    Ucnet::MsgType msgType;
    MemCpy(&msgType, buf.data(), siz(msgType));
    AU bufCur = buf.data() + siz(msgType);
    using TMsgDatFactory = std::unordered_map<Ucnet::MsgType, std::function<std::unique_ptr<Ucnet::MsgDatAny>()>>;
    static cx TMsgDatFactory s_msgDatFactory = {
        {Ucnet::MsgType::DBG, []() { ret std::make_unique<Ucnet::MsgDatDbg>(); }},
        {Ucnet::MsgType::PING, []() { ret std::make_unique<Ucnet::MsgDatPing>(); }},
    };
    cx AU msgKeyVal = s_msgDatFactory.find(msgType);
    ifu (msgKeyVal == s_msgDatFactory.end())
        rete(ErrVal::NET_MSG_NO_VALID);
    msgDat = msgKeyVal->second();
    ife (msgDat->ReadFrom(bufCur, SI(buf.size()) - siz(msgType)))
        retep;
    rets;
}
dfa S4 _UcnetCliThdFn(GA param)
{
    AU& cli = *(Ucnet::Cli*)param;
    while (YES)
    {
        ifu (!cli.IsInit())
            ret 1;
        if (!cli.IsConnected())
        {
            ThdWait(50); // TODO: use a more efficient wait mechanism
            continue;
        }

        BO doDisconnect = NO;
        std::unique_ptr<Ucnet::MsgDatAny> msgDat;
        ife (cli._Read(msgDat))
        {
            if (ErrLastGet() == ErrVal::NET_CLOSE)
            {
                msgDat = std::make_unique<Ucnet::MsgDatSysSrvDisconnect>();
                doDisconnect = YES;
            }
            else
            {
                msgDat = std::make_unique<Ucnet::MsgDatSysErrRead>();
            }
        }
        ife (cli._CallMsgCallbFn(msgDat.get()))
            doDisconnect = YES;
        ifu (doDisconnect)
            ife (cli.Disconnect())
                ConWriteDbg("[_UcnetCliThdFn] failed to disconnect client");
    }
}
dfa S4 _UcnetSrvThdFn(GA param)
{
    AU& srv = *(Ucnet::Srv*)param;
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
dfa S4 _UcnetSrvCliThdFn(GA param)
{
    AU cliDat = (Ucnet::Srv::CliDat*)param;
    cx AU err = cliDat->srv->CliThdFn(*cliDat);

    {
        std::unique_ptr<Ucnet::MsgDatAny> msgDat = std::make_unique<Ucnet::MsgDatSysCliDisconnect>();
        ife (cliDat->srv->_CallMsgCallbFn(*cliDat, msgDat.get()))
            ConWriteDbg("[_UcnetSrvCliThdFn] ignoring disconnect callback error on client [%s]", cliDat->adr.ToStr().c_str());
    }

    ife (cliDat->srv->Release(cliDat))
        if (ErrLastGet() != ErrVal::NO_INIT)
            ret 1;

    ife (err)
        ret 1;
    ret 0;
}

dfa ER Ucnet::Cli::_Write(cx Ucnet::MsgDatAny& msgDat) cx
{
    ret _UcnetMsgWrite(m_sock, msgDat);
}
dfa ER Ucnet::Cli::_Read(std::unique_ptr<Ucnet::MsgDatAny>& msgDat) cx
{
    ret _UcnetMsgRead(msgDat, m_sock);
}
dfa ER Ucnet::Cli::_CallMsgCallbFn(MsgDatAny* msgDat)
{
    // call the message specific callback function
    AU msgTypeI = SI(msgDat->Type());
    ifl (msgTypeI < m_msgCallbList.size() && m_msgCallbList[msgTypeI].fn)
        ret m_msgCallbList[msgTypeI].fn(*tx, *msgDat, m_msgCallbList[msgTypeI].ctx);
    // call the default callback function for unhandled message types
    msgTypeI = SI(Ucnet::MsgType::NONE);
    ifl (msgTypeI < m_msgCallbList.size() && m_msgCallbList[msgTypeI].fn)
        ret m_msgCallbList[msgTypeI].fn(*tx, *msgDat, m_msgCallbList[msgTypeI].ctx);
    // silently ignore the message if there is no default callback function
    ConWriteDbg("[Ucnet::Cli::_CallMsgCallbFn] ignoring received MsgType=%d without callback function", SI(msgDat->Type()));
    rets;
}

dfa BO Ucnet::Cli::IsInit() cx
{
    ret m_isInit;
}
dfa BO Ucnet::Cli::IsConnected() cx
{
    ret m_srvAdr.ip != 0;
}
dfa ER Ucnet::Cli::Connect(cx NetAdrV4& adr)
{
    ifu (tx->IsConnected() || m_sock.IsValid())
        rete(ErrVal::YES_INIT);
    ife (m_sock.New())
        retep;
    ife (m_sock.Connect(adr))
    {
        cx AU err = ErrLastGet();
        m_sock.Del();
        rete(err);
    }
    m_srvAdr = adr;

    std::unique_ptr<Ucnet::MsgDatAny> msgDat = std::make_unique<Ucnet::MsgDatSysSrvConnect>();
    ife (tx->_CallMsgCallbFn(msgDat.get()))
        retep;

    rets;
}
dfa ER Ucnet::Cli::Disconnect()
{
    if (!tx->IsConnected())
        rets;
    ifu (!m_sock.IsValid())
        rete(ErrVal::NO_INIT);
    ife (m_sock.Disconnect())
        retep;
    ife (m_sock.Del())
        retep;
    m_srvAdr.Clr();
    rets;
}
dfa ER Ucnet::Cli::Init()
{
    ifu (m_isInit)
        rete(ErrVal::YES_INIT);
    m_isInit = YES;
    ife (m_thd.New(_UcnetCliThdFn, GA(tx)))
    {
        m_isInit = NO;
        retep;
    }
    rets;
}
dfa ER Ucnet::Cli::Free()
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
tpl<Ucnet::MsgType TMsg, typename TFn> dfa NT Ucnet::Cli::MsgCallbSet(TFn&& fn, GA ctx)
{
    m_msgCallbList[SI(TMsg)] = _MsgCallbDatCreate<typename Ucnet::MsgTypeMap<TMsg>::MsgDatT, Cli>(std::forward<TFn>(fn), ctx);
}

dfa Ucnet::Cli::Cli() : m_isInit(NO)
{
}
dfa Ucnet::Cli::~Cli()
{
    tx->Disconnect();
}

dfa Ucnet::Srv::CliDat::CliDat() : srv(NUL)
{
}
dfa ER Ucnet::Srv::_Close()
{
    ife (m_sock.Del())
        retep;
    cx AU cliCnt = m_cliBuf.Cnt();
    ite (i, i < cliCnt)
    {
        AU cliDat = *m_cliBuf.Dat();
        ife (cliDat->sock.Disconnect())
            retep;
        ife (cliDat->sock.Del())
            retep;
        ife (cliDat->thd.Wait())
            retep;
        m_cliBuf.ElemDel(cliDat);
    }
    m_cliBuf.Del();
    rets;
}
dfa ER Ucnet::Srv::_CallMsgCallbFn(CliDat& cliDat, MsgDatAny* msgDat)
{
    // call the message specific callback function
    AU msgTypeI = SI(msgDat->Type());
    ifl (msgTypeI < m_msgCallbList.size() && m_msgCallbList[msgTypeI].fn)
        ret m_msgCallbList[msgTypeI].fn(cliDat, *msgDat, m_msgCallbList[msgTypeI].ctx);
    // call the default callback function for unhandled message types
    msgTypeI = SI(Ucnet::MsgType::NONE);
    ifl (msgTypeI < m_msgCallbList.size() && m_msgCallbList[msgTypeI].fn)
        ret m_msgCallbList[msgTypeI].fn(cliDat, *msgDat, m_msgCallbList[msgTypeI].ctx);
    // silently ignore the message if there is no default callback function
    ConWriteDbg("[Ucnet::Srv::_CallMsgCallbFn] ignoring received MsgType=%d from [%s] without callback function", SI(msgDat->Type()), cliDat.adr.ToStr().c_str());
    rets;
}
dfa ER Ucnet::Srv::CliThdFn(CliDat& cliDat)
{
    {
        std::unique_ptr<Ucnet::MsgDatAny> msgDat = std::make_unique<Ucnet::MsgDatSysCliConnect>();
        ife (cliDat.srv->_CallMsgCallbFn(cliDat, msgDat.get()))
            retep;
    }

    while (YES)
    {
        BO doDisconnect = NO;
        std::unique_ptr<Ucnet::MsgDatAny> msgDat;
        ife (_UcnetMsgRead(msgDat, cliDat.sock))
        {
            if (ErrLastGet() == ErrVal::NET_CLOSE)
            {
                doDisconnect = YES;
                jsrc(noSend);
            }
            else
            {
                msgDat = std::make_unique<Ucnet::MsgDatSysErrRead>();
            }
        }
        ife (cliDat.srv->_CallMsgCallbFn(cliDat, msgDat.get()))
            doDisconnect = YES;
        jdst(noSend);
        ifu (doDisconnect) // NOTE: error value currently not returned
            rets;          // NOTE: this is not necessarily a graceful disconnect, it triggers Release()
    }

    rets;
}
dfa ER Ucnet::Srv::Open(U2 port, SI cliCntMax)
{
    ifu (m_sock.IsValid())
        rete(ErrVal::YES_INIT);
    ife (m_sock.New())
        retep;
    ife (m_sock.OpenSrv(port))
    {
        cx AU err = ErrLastGet();
        m_sock.Del();
        rete(err);
    }
    ife (m_thd.New(_UcnetSrvThdFn, GA(tx)))
    {
        cx AU err = ErrLastGet();
        m_sock.Del();
        rete(err);
    }
    m_cliBuf.New(cliCntMax);
    rets;
}
dfa ER Ucnet::Srv::Close()
{
    if (!m_sock.IsValid())
        rets;
    m_cliBufLock.Lock();
    cx AU err = tx->_Close();
    m_cliBufLock.Unlock();
    ife (err)
        retep;
    ife (m_thd.Wait())
        retep;
    ife (m_thd.Close())
        retep;
    rets;
}
dfa ER Ucnet::Srv::Accept()
{
    ifu (!m_sock.IsValid())
        rete(ErrVal::NO_INIT);
    SockTcp sockCliTmp;
    NetAdrV4 adrTmp;
    ife (m_sock.Accept(sockCliTmp, adrTmp))
        retep;
    m_cliBufLock.Lock();
    cx AU cliDatNew = m_cliBuf.ElemNew();
    m_cliBufLock.Unlock();
    ifu (cliDatNew == NUL)
        rete(ErrVal::NET_HIGH_CLI);
    cliDatNew->srv = tx;
    cliDatNew->sock.Move(sockCliTmp);
    cliDatNew->adr = adrTmp;
    ife (cliDatNew->thd.New(_UcnetSrvCliThdFn, GA(cliDatNew)))
    {
        m_cliBufLock.Lock();
        m_cliBuf.ElemDel(cliDatNew);
        m_cliBufLock.Unlock();
        retep;
    }
    cliDatNew->thd.Disown();
    rets;
}
dfa ER Ucnet::Srv::Release(CliDat*& cliDat)
{
    ifu (!m_sock.IsValid())
        rete(ErrVal::NO_INIT);
    ife (cliDat->thd.Close())
        retep;
    ife (cliDat->sock.Disconnect())
        retep;
    ife (cliDat->sock.Del())
        retep;
    m_cliBufLock.Lock();
    m_cliBuf.ElemDel(cliDat);
    m_cliBufLock.Unlock();
    cliDat = NUL;
    rets;
}
tpl<Ucnet::MsgType TMsg, typename TFn> dfa NT Ucnet::Srv::MsgCallbSet(TFn&& fn, GA ctx)
{
    m_msgCallbList[SI(TMsg)] = _MsgCallbDatCreate<typename Ucnet::MsgTypeMap<TMsg>::MsgDatT, Ucnet::Srv::CliDat>(std::forward<TFn>(fn), ctx);
}
dfa Ucnet::Srv::Srv()
{
}
dfa Ucnet::Srv::~Srv()
{
    tx->Close();
}

#endif
