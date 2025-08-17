#pragma once

#ifdef PROG_SYS_WIN

namespace Ucnet
{

class RoomList;
class CliSrv;
class CliList;
class Srv;

struct CliRef
{
    CliSrv* ptr;
    TSessionId sid;

    dfa NT Clr()
    {
        ptr = NUL;
        sid = 0;
    }
    dfa CliRef(CliSrv* ptr = NUL, TSessionId sid = 0) : ptr(ptr), sid(sid)
    {
    }
};

class RoomList
{
  public:
    struct Room
    {
        using TId = U8;

        TId id;
        std::string name;
        Sha512Hash pwHash;
        SI cliCntMax;
        CliRef owner;
        std::vector<CliRef> cliList;
        ThdLockFast cliListLock;
    };

  private:
    std::unordered_map<Room::TId, Room> m_list;
    ThdLockFast m_listLock;
};

class CliSrv : public CliBase
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

    dfa CliSrv();
};

class CliList
{
  private:
    ArrSFree<CliSrv> m_buf;
    std::unordered_map<TSessionId, CliSrv*> m_mapBySessionId;
    std::unordered_map<std::string, CliSrv*> m_mapByUserName;
    mutable ThdLockFast m_lock;

  private:
    dfa BO _IsValid(cx CliRef& cli) cx;

  public:
    dfa NT Clr();
    dfa SI Cnt() cx;
    dfa NT List(std::vector<CliRef>& cliList) cx;
    dfa ER New(CliRef& cli);
    dfa ER Del(cx CliRef& cli);
    dfa ER Auth(CliRef& cli);
    dfa NT Init(SI cliCntMax);
    dfa NT Free();
};

class Srv
{
  public:
    using MsgCallbFnT = BaseMsgCallbFnT<CliSrv>;
    using MsgCallbDat = BaseMsgCallbDat<CliSrv>;

  private:
    SockTcp m_sock;
    Thd m_thd;
    CliList m_cliList;
    RoomList m_roomList;
    std::array<MsgCallbDat, TMsgType(MsgType::CNT)> m_msgCallbList;

  private:
    dfa ER _DefaMsgCallbSet();

  public:
    dfa ER _PostReadPreMsgCallbProc(CliSrv& cli, MsgDatAny* msgDat);
    dfa ER _CallMsgCallbFn(CliSrv& cli, cx MsgDatAny& msgDat);

  public:
    dfa ER CliThdFn(CliSrv& cli);

  public:
    dfa ER Open(U2 port, SI cliCntMax);
    dfa ER Close();
    dfa ER Accept();
    dfa ER Release(CliSrv*& cli);
    dfa TMsgNum MsgWrite(CliSrv& cli, cx MsgDatAny& msgDat, EvtFast* evt = NUL);
    dfa NT CliAuthToNoUser(CliSrv& cli);
    tpl<MsgType TMsg, typename TFn> dfa NT MsgCallbSet(TFn&& fn, GA ctx = NUL);

  public:
    dfa Srv();
    dfa ~Srv();
};

} // namespace Ucnet

#endif
