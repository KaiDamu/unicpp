#pragma once

#ifdef PROG_SYS_WIN

class Ucnet
{
  public:
    using TProtoVer = U2;
    using TMsgSize = U4;
    using TMsgNum = U4;
    using TErrType = U2;
    using TMsgType = U2;
    using TSessionId = U8;
    using TPriviLv = U1;
    using TMsgDatWriteToBuf = ArrSbo<U1, 256>;

    static cxex TProtoVer PROTO_VER = 2;
    static cxex SI MSG_SIZE_MAX = 4 * BYTE_IN_MB;
    static cxex U4 MSG_TYPE_VER_CX = 0x1B204B73;

    enum class ErrType : TErrType
    {
        NONE = 0,
    };
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
        CNT,                // not a valid message type, used for counting the number of message types
    };
    enum class PriviLv : TPriviLv
    {
        NONE = 0,  // absolutely no actions allowed
        INIT,      // can only do initialization specific actions
        NO_USER,   // can do some actions that do not require user authentication
        USER,      // can do standard registered user actions, encryption is required from this level
        HIGH_USER, // allows the user to do some administrative actions on their own account
        MODERATOR, // can influence other users, but not the system
        ADMIN,     // can do almost everything, except core system actions
        SYSTEM,    // all actions are allowed
    };

    struct Sec
    {
        using TChecksumMode = U1;
        using TEncryptMode = U1;
        using TSrvAuthMode = U1;

        enum class ChecksumMode : TChecksumMode
        {
            NONE = 0,
            FAST,
            SECURE,
        };
        enum class EncryptMode : TEncryptMode
        {
            NONE = 0,
            FAST,
            SECURE,
        };
        enum class SrvAuthMode : TSrvAuthMode
        {
            NONE = 0,
            FAST,
            SECURE,
        };
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

    struct MsgDatVer : _MsgDatAnyT<MsgType::VER>
    {
        BO isReq;
        U4 verCx;
        TProtoVer verMin;
        TProtoVer verMax;
        TMsgNum msgNum;
        TSessionId sessionId;
        std::string userName;

        dfa virtual ER WriteTo(TMsgDatWriteToBuf& buf, U1*& bufCur) cx override
        {
            AU datSize = siz(isReq) + siz(verCx) + siz(verMin);
            if (isReq)
                datSize += siz(verMax);
            else
                datSize += siz(msgNum) + siz(sessionId) + siz(U1) + (userName.size() * siz(CS));
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
                MemCpyUpdCur(bufCur, &msgNum, siz(msgNum));
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
                ifu (size < datSize + siz(msgNum) + siz(sessionId) + siz(U1))
                    rete(ErrVal::NET_MSG_NO_VALID);
                MemCpyUpdCurSrc(&msgNum, buf, siz(msgNum));
                MemCpyUpdCurSrc(&sessionId, buf, siz(sessionId));
                U1 userNameSize;
                MemCpyUpdCurSrc(&userNameSize, buf, siz(userNameSize));
                ifu (size != datSize + siz(msgNum) + siz(sessionId) + siz(U1) + userNameSize)
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

    // common client state data
    struct Cd
    {
        TProtoVer ver;
        TMsgNum msgNumCur;
        TSessionId sessionId;
        std::string userName;

        dfa NT Clr();
        dfa Cd();
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
        std::array<MsgCallbDat, TMsgType(MsgType::CNT)> m_msgCallbExList;
        Cd m_cd;

      private:
        dfa ER _DefaMsgCallbSet();

      public:
        dfa Cd& _Cd();
        dfa ER _Write(cx MsgDatAny& msgDat) cx;
        dfa ER _Read(std::unique_ptr<MsgDatAny>& msgDat) cx;
        dfa ER _CallMsgCallbFn(cx MsgDatAny& msgDat);
        dfa NT _AuthToNoUser(cx MsgDatVer& msgDat);

      public:
        dfa BO IsInit() cx;
        dfa BO IsConnected() cx;
        dfa cx Cd& Cd() cx;
        dfa ER Connect(cx NetAdrV4& adr);
        dfa ER Disconnect();
        dfa ER Init();
        dfa ER Free();
        tpl<Ucnet::MsgType TMsg, typename TFn> dfa NT MsgCallbSet(TFn&& fn, GA ctx = NUL);
        tpl<Ucnet::MsgType TMsg, typename TFn> dfa NT MsgCallbExSet(TFn&& fn, GA ctx = NUL);

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
            BO doDisconnect;
            Cd cd;
            Sec::ChecksumMode checksumMode;
            Sec::EncryptMode encryptMode;
            Sec::SrvAuthMode srvAuthMode;
            PriviLv priviLv;

            dfa NT MsgWrite(cx MsgDatAny& msgDat);
            dfa NT Init(Srv* srv, SockTcp& sock, cx NetAdrV4& adr);
            dfa CliDat();
        };

        using MsgCallbFnT = BaseMsgCallbFnT<CliDat>;
        using MsgCallbDat = BaseMsgCallbDat<CliDat>;

      private:
        SockTcp m_sock;
        ArrSFree<CliDat> m_cliBuf;
        std::unordered_map<TSessionId, CliDat*> m_cliMapBySessionId;
        std::unordered_map<std::string, CliDat*> m_cliMapByUserName;
        ThdLockFast m_cliListLock;
        Thd m_thd;
        std::array<MsgCallbDat, TMsgType(MsgType::CNT)> m_msgCallbList;

      private:
        dfa ER _Close();
        dfa ER _DefaMsgCallbSet();

      public:
        dfa ER _PostReadPreMsgCallbProc(CliDat& cliDat, MsgDatAny* msgDat);
        dfa ER _CallMsgCallbFn(CliDat& cliDat, cx MsgDatAny& msgDat);

      public:
        dfa ER CliThdFn(CliDat& cliDat);

      public:
        dfa ER Open(U2 port, SI cliCntMax);
        dfa ER Close();
        dfa ER Accept();
        dfa ER Release(CliDat*& cliDat);
        dfa NT MsgWrite(CliDat& cliDat, cx MsgDatAny& msgDat);
        dfa NT CliAuthToNoUser(CliDat& cliDat);
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
        {Ucnet::MsgType::VER, []() { ret std::make_unique<Ucnet::MsgDatVer>(); }},
        {Ucnet::MsgType::DBG, []() { ret std::make_unique<Ucnet::MsgDatDbg>(); }},
        {Ucnet::MsgType::PING, []() { ret std::make_unique<Ucnet::MsgDatPing>(); }},
    };
    cx AU msgKeyVal = s_msgDatFactory.find(msgType);
    ifu (msgKeyVal == s_msgDatFactory.end())
    {
        ConWriteDbg("[_UcnetMsgRead] s_msgDatFactory does not contain MsgType=%d", SI(msgType));
        rete(ErrVal::NET_MSG_NO_VALID);
    }
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
        ife (cli._CallMsgCallbFn(*msgDat.get()))
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
        ife (cliDat->srv->_CallMsgCallbFn(*cliDat, *msgDat.get()))
            ConWriteDbg("[_UcnetSrvCliThdFn] ignoring disconnect callback error on client [%s]", cliDat->adr.ToStr().c_str());
    }

    ife (cliDat->srv->Release(cliDat))
        if (ErrLastGet() != ErrVal::NO_INIT)
            ret 1;

    ife (err)
        ret 1;
    ret 0;
}

dfa NT Ucnet::Cd::Clr()
{
    ver = TProtoVer(0);
    msgNumCur = TMsgNum(0);
    sessionId = TSessionId(0);
    userName.clear();
}
dfa Ucnet::Cd::Cd()
{
    tx->Clr();
}

dfa ER Ucnet::Cli::_DefaMsgCallbSet()
{
    tx->MsgCallbSet<MsgType::NONE>([](Cli& cli, cx MsgDatAny& msg, GA ctx) {
        ConWriteErr("[Server] Unhandled message [type = %u]", TMsgType(msg.Type()));
        rets; // NOTE: as the client, we try to silently ignore unhandled messages
    });
    tx->MsgCallbSet<MsgType::VER>([](Cli& cli, cx MsgDatVer& msg, GA ctx) {
        ifu (msg.isReq || (msg.verCx != MSG_TYPE_VER_CX))
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

        cli._Cd().Clr();

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

dfa Ucnet::Cd& Ucnet::Cli::_Cd()
{
    ret m_cd;
}
dfa ER Ucnet::Cli::_Write(cx Ucnet::MsgDatAny& msgDat) cx
{
    ret _UcnetMsgWrite(m_sock, msgDat);
}
dfa ER Ucnet::Cli::_Read(std::unique_ptr<Ucnet::MsgDatAny>& msgDat) cx
{
    ret _UcnetMsgRead(msgDat, m_sock);
}
dfa ER Ucnet::Cli::_CallMsgCallbFn(cx MsgDatAny& msgDat)
{
    AU msgTypeI = SI(msgDat.Type());
    ifu (msgTypeI >= m_msgCallbList.size())
        rete(ErrVal::NET_MSG_NO_VALID);

    // call the message specific callback function & extended callback function if exists
    ifl (m_msgCallbList[msgTypeI].fn)
    {
        ife (m_msgCallbList[msgTypeI].fn(*tx, msgDat, m_msgCallbList[msgTypeI].ctx))
            retep;
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
    msgTypeI = SI(Ucnet::MsgType::NONE);
    ifl (m_msgCallbList[msgTypeI].fn)
        ret m_msgCallbList[msgTypeI].fn(*tx, msgDat, m_msgCallbList[msgTypeI].ctx);

    // silently ignore the message if there is no default callback function
    ConWriteDbg("[Ucnet::Cli::_CallMsgCallbFn] ignoring received MsgType=%d without callback function", SI(msgDat.Type()));
    rets;
}
dfa NT Ucnet::Cli::_AuthToNoUser(cx MsgDatVer& msgDat)
{
    m_cd.ver = msgDat.verMin;
    m_cd.msgNumCur = msgDat.msgNum;
    m_cd.sessionId = msgDat.sessionId;
    m_cd.userName = msgDat.userName;
}

dfa BO Ucnet::Cli::IsInit() cx
{
    ret m_isInit;
}
dfa BO Ucnet::Cli::IsConnected() cx
{
    ret m_srvAdr.ip != 0;
}
dfa cx Ucnet::Cd& Ucnet::Cli::Cd() cx
{
    ret m_cd;
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

    std::unique_ptr<MsgDatAny> msgDat = std::make_unique<MsgDatSysSrvConnect>();
    ife (tx->_CallMsgCallbFn(*msgDat.get()))
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
tpl<Ucnet::MsgType TMsg, typename TFn> dfa NT Ucnet::Cli::MsgCallbExSet(TFn&& fn, GA ctx)
{
    m_msgCallbExList[SI(TMsg)] = _MsgCallbDatCreate<typename Ucnet::MsgTypeMap<TMsg>::MsgDatT, Cli>(std::forward<TFn>(fn), ctx);
}

dfa Ucnet::Cli::Cli() : m_isInit(NO)
{
    tx->_DefaMsgCallbSet(); // NOTE: error ignored, currently callbacks are tied to the lifetime of the client instance
}
dfa Ucnet::Cli::~Cli()
{
    tx->Disconnect();
}

dfa NT Ucnet::Srv::CliDat::MsgWrite(cx MsgDatAny& msgDat)
{
    tx->srv->MsgWrite(*tx, msgDat);
}
dfa NT Ucnet::Srv::CliDat::Init(Srv* srv, SockTcp& sock, cx NetAdrV4& adr)
{
    tx->srv = srv;
    tx->sock.Move(sock);
    tx->adr = adr;
    // tx->thd; // thread is initialized outside
    tx->doDisconnect = NO;
    tx->cd.Clr();
    tx->checksumMode = Sec::ChecksumMode::NONE;
    tx->encryptMode = Sec::EncryptMode::NONE;
    tx->srvAuthMode = Sec::SrvAuthMode::NONE;
    tx->priviLv = PriviLv::INIT;
}
dfa Ucnet::Srv::CliDat::CliDat()
{
    SockTcp sock;
    NetAdrV4 adr;
    tx->Init(NUL, sock, adr);
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
    m_cliMapBySessionId.clear();
    m_cliMapByUserName.clear();
    rets;
}
dfa ER Ucnet::Srv::_DefaMsgCallbSet()
{
    tx->MsgCallbSet<MsgType::NONE>([](CliDat& cli, cx MsgDatAny& msg, GA ctx) {
        ConWriteErr("[%s] Unhandled message [type = %u]", cli.adr.ToStr().c_str(), TMsgType(msg.Type()));
        rete(ErrVal::NET_MSG_NO_VALID); // NOTE: since this is the default callback, we return an error directly
    });
    tx->MsgCallbSet<MsgType::VER>([](CliDat& cli, cx MsgDatVer& msg, GA ctx) {
        ifu (!msg.isReq || (msg.verCx != MSG_TYPE_VER_CX))
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

        cli.srv->CliAuthToNoUser(cli);

        MsgDatVer msgVer;
        msgVer.isReq = NO;
        msgVer.verCx = MSG_TYPE_VER_CX;
        msgVer.verMin = PROTO_VER;
        msgVer.msgNum = cli.cd.msgNumCur;
        msgVer.sessionId = cli.cd.sessionId;
        msgVer.userName = cli.cd.userName;
        cli.MsgWrite(msgVer);

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
dfa ER Ucnet::Srv::_PostReadPreMsgCallbProc(CliDat& cliDat, MsgDatAny* msgDat)
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
dfa ER Ucnet::Srv::_CallMsgCallbFn(CliDat& cliDat, cx MsgDatAny& msgDat)
{
    // call the message specific callback function
    AU msgTypeI = SI(msgDat.Type());
    ifl (msgTypeI < m_msgCallbList.size() && m_msgCallbList[msgTypeI].fn)
        ret m_msgCallbList[msgTypeI].fn(cliDat, msgDat, m_msgCallbList[msgTypeI].ctx);
    // call the default callback function for unhandled message types
    msgTypeI = SI(Ucnet::MsgType::NONE);
    ifl (msgTypeI < m_msgCallbList.size() && m_msgCallbList[msgTypeI].fn)
        ret m_msgCallbList[msgTypeI].fn(cliDat, msgDat, m_msgCallbList[msgTypeI].ctx);
    // silently ignore the message if there is no default callback function
    ConWriteDbg("[Ucnet::Srv::_CallMsgCallbFn] ignoring received MsgType=%d from [%s] without callback function", SI(msgDat.Type()), cliDat.adr.ToStr().c_str());
    rets;
}
dfa ER Ucnet::Srv::CliThdFn(CliDat& cliDat)
{
    {
        std::unique_ptr<MsgDatAny> msgDat = std::make_unique<MsgDatSysCliConnect>();
        ife (cliDat.srv->_CallMsgCallbFn(cliDat, *msgDat.get()))
            retep;
    }

    while (YES)
    {
        BO doDisconnect = NO;
        std::unique_ptr<MsgDatAny> msgDat;
        ife (_UcnetMsgRead(msgDat, cliDat.sock))
        {
            if (ErrLastGet() == ErrVal::NET_CLOSE)
            {
                doDisconnect = YES;
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
            ConWriteDbg("[Ucnet::Srv::CliThdFn] _PostReadPreMsgCallbProc rejected msg & client from [%s]", cliDat.adr.ToStr().c_str());
            doDisconnect = YES;
            jsrc(noSend);
        }
        ife (cliDat.srv->_CallMsgCallbFn(cliDat, *msgDat.get()))
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
    m_cliListLock.Lock();
    cx AU err = tx->_Close();
    m_cliListLock.Unlock();
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
    // TODO: could use application-level blacklisting here
    m_cliListLock.Lock();
    cx AU cliDatNew = m_cliBuf.ElemNew();
    m_cliListLock.Unlock();
    ifu (cliDatNew == NUL)
        rete(ErrVal::NET_HIGH_CLI);
    cliDatNew->Init(tx, sockCliTmp, adrTmp);
    ife (cliDatNew->thd.New(_UcnetSrvCliThdFn, GA(cliDatNew)))
    {
        m_cliListLock.Lock();
        m_cliBuf.ElemDel(cliDatNew);
        m_cliListLock.Unlock();
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
dfa NT Ucnet::Srv::MsgWrite(CliDat& cliDat, cx MsgDatAny& msgDat)
{
    ife (_UcnetMsgWrite(cliDat.sock, msgDat))
    {
        cx AU msgDat = std::make_unique<MsgDatSysErrWrite>();
        ife (tx->_CallMsgCallbFn(cliDat, *msgDat.get()))
            cliDat.doDisconnect = YES;
    }
}
dfa NT Ucnet::Srv::CliAuthToNoUser(CliDat& cliDat)
{
    cliDat.cd.ver = PROTO_VER;
    RandCrypt(&cliDat.cd.msgNumCur, siz(cliDat.cd.msgNumCur));
    cliDat.cd.msgNumCur >>= 1; // this prevents overflow later on

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
tpl<Ucnet::MsgType TMsg, typename TFn> dfa NT Ucnet::Srv::MsgCallbSet(TFn&& fn, GA ctx)
{
    m_msgCallbList[SI(TMsg)] = _MsgCallbDatCreate<typename Ucnet::MsgTypeMap<TMsg>::MsgDatT, Ucnet::Srv::CliDat>(std::forward<TFn>(fn), ctx);
}
dfa Ucnet::Srv::Srv()
{
    tx->_DefaMsgCallbSet(); // NOTE: error ignored, currently callbacks are tied to the lifetime of the server instance
}
dfa Ucnet::Srv::~Srv()
{
    tx->Close();
}

#endif
