#pragma once

#ifdef PROG_SYS_WIN

namespace Ucnet
{

// common client state data, valid for the session
struct Cd
{
    TProtoVer ver;
    TMsgNum msgNumToWrite;
    TMsgNum msgNumToRead;
    TSessionId sessionId;
    std::string userName;
    BO doDisconnect;
    SI hdrSize;

    dfa NT Clr(BO isCli);
    dfa NT HdrSizeUpd();
    dfa Cd();
};

} // namespace Ucnet

#endif
