#pragma once

#ifdef PROG_SYS_WIN

namespace Ucnet
{

class CliBase
{
  public: // NOTE: public by current design
    std::vector<U1> m_tmpBufWrite;
    std::vector<U1> m_tmpBufRead;
    std::unordered_map<MsgNumT, MsgPend, StdHasherNoOpe<MsgNumT>> m_msgPendList;
    ThdLockFast m_msgPendListLock;
    ProtoVerT m_ver;
    MsgNumT m_msgNumToWrite;
    MsgNumT m_msgNumToRead;
    SessionIdT m_sessionId;
    std::string m_userName;
    BO m_doDisconnect;
    SI m_hdrSize;

  protected:
    dfa NT ClrBase(BO isCli);
    dfa NT HdrSizeUpd();

  public:
    dfa ER _MsgWriteBase(MsgNumT& msgNum, cx SockTcp& sock, cx MsgDatAny& msgDat, EvtFast* evt);
    dfa ER _MsgReadBase(std::unique_ptr<MsgDatAny>& msgDat, cx SockTcp& sock);

  public:
    dfa NT MsgResWait(MsgNumT msgNum);
    dfa NT MsgResWaitAll();

    dfa CliBase();
};

} // namespace Ucnet

#endif
