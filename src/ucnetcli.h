#pragma once

#ifdef PROG_SYS_WIN

namespace Ucnet
{

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
    dfa ER _Write(cx MsgDatAny& msgDat);
    dfa ER _Read(std::unique_ptr<MsgDatAny>& msgDat);
    dfa ER _CallMsgCallbFn(cx MsgDatAny& msgDat);
    dfa NT _AuthToNoUser(cx MsgDatVerRes& msgDat);

  public:
    dfa BO IsInit() cx;
    dfa BO IsConnected() cx;
    dfa BO IsDisconnecting() cx;
    dfa cx Cd& Cd() cx;
    dfa ER Connect(cx NetAdrV4& adr);
    dfa ER Disconnect();
    dfa ER Init();
    dfa ER Free();
    tpl<MsgType TMsg, typename TFn> dfa NT MsgCallbSet(TFn&& fn, GA ctx = NUL);
    tpl<MsgType TMsg, typename TFn> dfa NT MsgCallbExSet(TFn&& fn, GA ctx = NUL);

  public:
    dfa Cli();
    dfa ~Cli();
};

} // namespace Ucnet

#endif
