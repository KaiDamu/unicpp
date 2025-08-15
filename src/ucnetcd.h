#pragma once

#ifdef PROG_SYS_WIN

namespace Ucnet
{

// common client state data, valid for the session
struct Cd
{
    std::vector<U1> tmpBufWrite;
    std::vector<U1> tmpBufRead;
    TProtoVer ver;
    TMsgNum msgNumToWrite;
    TMsgNum msgNumToRead;
    TSessionId sessionId;
    std::string userName;
    BO doDisconnect;
    SI hdrSize;

    dfa NT Clr(BO isCli);
    dfa NT HdrSizeUpd();
    dfa ER MsgWrite(cx SockTcp& sock, cx MsgDatAny& msgDat);
    dfa ER MsgRead(std::unique_ptr<MsgDatAny>& msgDat, cx SockTcp& sock);
    dfa Cd();
};

} // namespace Ucnet

#endif
