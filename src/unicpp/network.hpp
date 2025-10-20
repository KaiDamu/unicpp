#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include <span>
    #include <string>
    #include <winsock2.h>
    #include "type.hpp"
#endif

#ifdef PROG_SYS_WIN

    #ifdef PROG_COMPILER_MSVC
        #pragma comment(lib, "Ws2_32.lib")
    #endif

dfa ER _NetInitFree(BO isInit);
dfa ER _NetInit();
dfa ER _NetFree();

class NetAdrV4
{
  public:
    U4 ip;   // in network byte order
    U2 port; // in network byte order

  public:
    dfa NT Clr();
    dfa U8 ToU8() cx;
    dfa NetAdrV4 ToNoPort() cx;
    dfa NT ToStr(CS* out) cx;
    dfa std::string ToStr() cx;
    dfa BO operator==(cx NetAdrV4& other) cx;
    dfa BO operator!=(cx NetAdrV4& other) cx;

  public:
    dfa NetAdrV4(U4 ip = 0, U2 port = 0);
    dfa NetAdrV4(U1 ip1, U1 ip2, U1 ip3, U1 ip4, U2 port = 0);
};

class SockTcp
{
  private:
    SOCKET m_hdl;
    BO m_isConnected;

  public:
    dfa BO IsValid() cx;
    dfa BO IsConnected() cx;
    dfa HD Hdl() cx;
    dfa ER New();
    dfa ER Del();
    dfa ER Reset();
    dfa ER Connect(cx NetAdrV4& adr);
    dfa ER Disconnect();
    dfa ER OpenSrv(U2 port) cx;
    dfa ER Accept(SockTcp& sockCli, NetAdrV4& adr);
    dfa ER Write(CXGA buf, SI size) cx;
    dfa ER Write(cx std::span<cx std::span<cx U1>>& bufList) cx;
    dfa ER Read(GA buf, SI& sizeResult, SI sizeMax, SI sizeMin = 1) cx;
    dfa NT Move(SockTcp& src);

  public:
    dfa SockTcp();
    dfa ~SockTcp();
};

dfa ER StrToNetAdrV4(NetAdrV4& out, cx CS* in);

#endif

#include "network.ipp"
