#pragma once

#ifdef PROG_SYS_WIN

namespace Ucnet
{

class Srv
{
  public:
    class CliDat : public Cd
    {
      public: // NOTE: public by current design
        Srv* m_srv;
        SockTcp m_sock;
        NetAdrV4 m_adr;
        Thd m_thd;
        Sec::ChecksumMode m_checksumMode;
        Sec::EncryptMode m_encryptMode;
        Sec::SrvAuthMode m_srvAuthMode;
        PriviLv m_priviLv;

      public:
        dfa NT MsgWrite(cx MsgDatAny& msgDat, EvtFast* evt = NUL);
        dfa NT Init(Srv* srv, SockTcp& sock, cx NetAdrV4& adr);

        dfa CliDat();
    };

    struct RoomDat
    {
        std::string name;
        Sha512Hash pwHash;
        TSessionId owner;
        SI cliCntMax;
        std::unordered_set<CliDat*> cliList;
        ThdLockFast cliListLock;

        dfa RoomDat();
    };

    using MsgCallbFnT = BaseMsgCallbFnT<CliDat>;
    using MsgCallbDat = BaseMsgCallbDat<CliDat>;

  private:
    SockTcp m_sock;
    ArrSFree<CliDat> m_cliBuf;
    std::unordered_map<TSessionId, CliDat*> m_cliMapBySessionId;
    std::unordered_map<std::string, CliDat*> m_cliMapByUserName;
    ThdLockFast m_cliListLock;
    std::unordered_map<std::string, RoomDat> m_roomList;
    ThdLockFast m_roomListLock;
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
    dfa TMsgNum MsgWrite(CliDat& cliDat, cx MsgDatAny& msgDat, EvtFast* evt = NUL);
    dfa NT CliAuthToNoUser(CliDat& cliDat);
    tpl<MsgType TMsg, typename TFn> dfa NT MsgCallbSet(TFn&& fn, GA ctx = NUL);

  public:
    dfa Srv();
    dfa ~Srv();
};

} // namespace Ucnet

#endif
