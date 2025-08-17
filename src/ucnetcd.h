#pragma once

#ifdef PROG_SYS_WIN

namespace Ucnet
{

// common client state data
class Cd
{
  public: // NOTE: public by current design
    std::vector<U1> m_tmpBufWrite;
    std::vector<U1> m_tmpBufRead;
    std::unordered_map<TMsgNum, MsgPend> m_msgPendList;
    ThdLockFast m_msgPendListLock;
    TProtoVer m_ver;
    TMsgNum m_msgNumToWrite;
    TMsgNum m_msgNumToRead;
    TSessionId m_sessionId;
    std::string m_userName;
    BO m_doDisconnect;
    SI m_hdrSize;

  protected:
    dfa NT ClrBase(BO isCli);
    dfa NT HdrSizeUpd();

  public:
    dfa ER _MsgWriteBase(TMsgNum& msgNum, cx SockTcp& sock, cx MsgDatAny& msgDat, EvtFast* evt);
    dfa ER _MsgReadBase(std::unique_ptr<MsgDatAny>& msgDat, cx SockTcp& sock);

  public:
    dfa NT MsgResWait(TMsgNum msgNum);
    dfa NT MsgResWaitAll();

    dfa Cd();
};

} // namespace Ucnet

#endif
