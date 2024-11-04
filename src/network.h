#pragma once

#ifdef PROG_COMPILER_MSVC
    #pragma comment(lib, "Ws2_32.lib")
#endif

dfa ER NetInit()
{
    thdlocal BO isInitDone = NO;
    if (isInitDone)
        rets;
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        rete(ErrVal::NET);
    isInitDone = YES;
    rets;
}
dfa NT NetFree()
{
    WSACleanup(); // error ignored
}

class NetAdr
{
  public:
    U4 ip;
    U2 port;

  public:
    dfa NT Clr()
    {
        ip = 0;
        port = 0;
    }

  public:
    dfa NetAdr(U4 ip = 0, U2 port = 0) : ip(ip), port(port)
    {
    }
};

dfa U4 StrToIp(cx CS* str)
{
    ret (U4)inet_addr(str);
}
dfa U2 StrToPort(cx CS* str)
{
    ret (U2)atoi(str);
}

dfa ER DomainToIp(U4& ip, cx CS* domain)
{
    ADDRINFOA hints;
    ZeroMemory(&hints, siz(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    ADDRINFOA* result;
    ifu (getaddrinfo(domain, NUL, &hints, &result) != 0)
        rete(ErrVal::NET);
    ip = ((SOCKADDR_IN*)result->ai_addr)->sin_addr.S_un.S_addr;
    freeaddrinfo(result);
    rets;
}

dfa ER StrToNetAdrIp(NetAdr& netAdr, cx string_view& str)
{
    netAdr.Clr();
    cx AU divPos = str.find(':');
    string_view strIp;
    string_view strPort;
    if (divPos == string::npos)
    {
        strIp = str;
        strPort = "";
    }
    else
    {
        strIp = str.substr(0, divPos);
        strPort = str.substr(divPos + 1);
    }
    if (!strIp.empty())
    {
        netAdr.ip = StrToIp((divPos == string::npos) ? strIp.data() : string(strIp, 0, divPos).c_str());
    }
    if (!strPort.empty())
    {
        netAdr.port = StrToPort(strPort.data());
    }
    rets;
}
dfa ER StrToNetAdrDomain(NetAdr& netAdr, cx string_view& str)
{
    netAdr.Clr();
    cx AU divPos = str.find(':');
    string_view strDomain;
    string_view strPort;
    if (divPos == string::npos)
    {
        strDomain = str;
        strPort = "";
    }
    else
    {
        strDomain = str.substr(0, divPos);
        strPort = str.substr(divPos + 1);
    }
    if (!strDomain.empty())
    {
        U4 ip;
        ife (DomainToIp(ip, (divPos == string::npos) ? strDomain.data() : string(strDomain, 0, divPos).c_str()))
            retep;
        netAdr.ip = ip;
    }
    if (!strPort.empty())
    {
        netAdr.port = StrToPort(strPort.data());
    }
    rets;
}
dfa ER StrToNetAdr(NetAdr& netAdr, cx string_view& str)
{
    BO isIp = str.empty() || IsNumBase10(str[0]) || (str[0] == ':');
    if (isIp)
    {
        for (cx AU& c : str)
        {
            if (!IsNumBase10(c))
            {
                if (c != '.')
                    isIp = NO;
                break;
            }
        }
    }
    if (isIp)
    {
        ret StrToNetAdrIp(netAdr, str);
    }
    else
    {
        ret StrToNetAdrDomain(netAdr, str);
    }
}

dfa NT NetAdrToStr(string& str, cx NetAdr& netAdr)
{
    str.clear();
    CS ipStr[16];
    inet_ntop(AF_INET, &netAdr.ip, ipStr, siz(ipStr));
    str += ipStr;
    str += ':';
    str += std::to_string(S4(netAdr.port));
}
