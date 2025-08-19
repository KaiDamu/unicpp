#pragma once

#ifdef PROG_SYS_WIN

namespace Ucnet
{

class Srv;

class CliSrv : public CliBase
{
  public: // NOTE: public by current design
    Srv* m_srv;
    SockTcp m_sock;
    NetAdrV4 m_adr;
    Thd m_thd;
    ThdLockFast m_lock;
    Sec::ChecksumMode m_checksumMode;
    Sec::EncryptMode m_encryptMode;
    Sec::SrvAuthMode m_srvAuthMode;
    PriviLv m_priviLv;

  public:
    dfa NT MsgWrite(cx MsgDatAny& msgDat, EvtFast* evt = NUL);
    dfa NT Init(Srv* srv, SockTcp& sock, cx NetAdrV4& adr);

    dfa CliSrv();
    dfa CliSrv(Srv* srv, SockTcp& sock, cx NetAdrV4& adr);
};
tpl0 struct TypeTraits<CliSrv>
{
    using IdT = TO(CliSrv::m_sessionId);
    using IdSecT = TO(CliSrv::m_userName);
    using LockT = TO(CliSrv::m_lock);

    static IdT& id(CliSrv& obj)
    {
        ret obj.m_sessionId;
    }
    static cx IdT& id(cx CliSrv& obj)
    {
        ret obj.m_sessionId;
    }
    static IdSecT& idSec(CliSrv& obj)
    {
        ret obj.m_userName;
    }
    static LockT& lock(CliSrv& obj)
    {
        ret obj.m_lock;
    }

    static BO HasIdSec(CliSrv& obj)
    {
        ret obj.m_userName.size() > 0;
    }
    static NT Lock(CliSrv& obj)
    {
        ret obj.m_lock.Lock();
    }
    static NT Unlock(CliSrv& obj)
    {
        ret obj.m_lock.Unlock();
    }
};
using CliList = MthdObjList<CliSrv>;
using CliRef = CliList::Ref;

class Srv
{
  public:
    using MsgCallbFnT = BaseMsgCallbFnT<CliSrv>;
    using MsgCallbDat = BaseMsgCallbDat<CliSrv>;

  private:
    SockTcp m_sock;
    Thd m_thd;
    CliList m_cliList;
    // RoomList m_roomList;
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
    dfa NT CliAuthToNoUser(CliSrv& cli_);
    tpl<MsgType TMsg, typename TFn> dfa NT MsgCallbSet(TFn&& fn, GA ctx = NUL);

  public:
    dfa Srv();
    dfa ~Srv();
};

} // namespace Ucnet

#endif
