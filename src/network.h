#pragma once

#ifdef PROG_SYS_WIN

    #ifdef PROG_COMPILER_MSVC
        #pragma comment(lib, "Ws2_32.lib")
    #endif

dfa ER _NetInitFree(BO isInit)
{
    static SI initCnt = 0;
    if (isInit)
    {
        ++initCnt;
        if (initCnt > 1)
            rets;
        WSADATA tmp;
        ifu (WSAStartup(0x0202, &tmp) != 0)
            rete(ErrVal::NET);
    }
    else if (initCnt > 0)
    {
        --initCnt;
        if (initCnt > 0)
            rets;
        ifu ((WSACleanup() != 0) && (WSAGetLastError() != WSANOTINITIALISED))
            rete(ErrVal::NET);
    }
    rets;
}
dfa ER _NetInit()
{
    ret _NetInitFree(YES);
}
dfa ER _NetFree()
{
    ret _NetInitFree(NO);
}

class NetAdrV4
{
  public:
    U4 ip;   // in network byte order
    U2 port; // in network byte order

  public:
    dfa NT Clr()
    {
        ip = 0;
        port = 0;
    }
    dfa U8 ToU8() cx
    {
        ret (U8(ip) << sizb(port)) | U8(port);
    }
    dfa NetAdrV4 ToNoPort() cx
    {
        NetAdrV4 result;
        result.ip = ip;
        result.port = 0;
        ret result;
    }
    dfa NT ToStr(CS* out) cx
    {
        AU cur = out;
        ite (i, i < 4)
        {
            cur += IntToStr(cur, ByteIVal(ip, i));
            *cur++ = '.';
        }
        *(cur - 1) = ':';
        cur += IntToStr(cur, RevByte(port));
        *cur = '\0';
    }
    dfa std::string ToStr() cx
    {
        CS str[32];
        tx->ToStr(str);
        ret std::string(str);
    }
    dfa BO operator==(cx NetAdrV4& other) cx
    {
        ret (ip == other.ip) && (port == other.port);
    }
    dfa BO operator!=(cx NetAdrV4& other) cx
    {
        ret (ip != other.ip) || (port != other.port);
    }

  public:
    dfa NetAdrV4(U4 ip = 0, U2 port = 0) : ip(ip), port(port)
    {
    }
    dfa NetAdrV4(U1 ip1, U1 ip2, U1 ip3, U1 ip4, U2 port = 0)
    {
        tx->ip = U4(ip4) << 24 | U4(ip3) << 16 | U4(ip2) << 8 | U4(ip1);
        tx->port = RevByte(port);
    }
};

class SockTcp
{
  private:
    SOCKET m_hdl;
    BO m_isConnected;

  public:
    dfa BO IsValid() cx
    {
        ret m_hdl != INVALID_SOCKET;
    }
    dfa BO IsConnected() cx
    {
        ret m_isConnected;
    }
    dfa HD Hdl() cx
    {
        ret HD(m_hdl);
    }
    dfa ER New()
    {
        ifu (m_hdl != INVALID_SOCKET)
            rete(ErrVal::YES_INIT);
        m_hdl = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        ifu (m_hdl == INVALID_SOCKET)
            rete(ErrVal::NET);
        m_isConnected = NO;
        rets;
    }
    dfa ER Del()
    {
        if (m_hdl == INVALID_SOCKET)
            rets;
        ifu (closesocket(m_hdl) != 0)
            rete(ErrVal::NET);
        m_hdl = INVALID_SOCKET;
        m_isConnected = NO;
        rets;
    }
    dfa ER Reset()
    {
        ife (tx->Del())
            retep;
        ife (tx->New())
            retep;
        rets;
    }
    dfa ER Connect(cx NetAdrV4& adr)
    {
        ifu (m_hdl == INVALID_SOCKET)
            rete(ErrVal::NO_INIT);
        SOCKADDR_IN adrInfo = {};
        adrInfo.sin_family = AF_INET;
        adrInfo.sin_port = adr.port;
        adrInfo.sin_addr.S_un.S_addr = adr.ip;
        ifu (connect(m_hdl, (SOCKADDR*)&adrInfo, siz(adrInfo)) != 0)
        {
            if (WSAGetLastError() == WSAECONNREFUSED)
                rete(ErrVal::NET_NO_EXIST);
            rete(ErrVal::NET);
        }
        m_isConnected = YES;
        rets;
    }
    dfa ER Disconnect()
    {
        ifu (!m_isConnected)
            rets;
        m_isConnected = NO;
        ifu (shutdown(m_hdl, SD_BOTH) != 0)
        {
            m_isConnected = YES;
            rete(ErrVal::NET);
        }
        rets;
    }
    dfa ER OpenSrv(U2 port) cx
    {
        ifu (m_hdl == INVALID_SOCKET)
            rete(ErrVal::NO_INIT);
        SOCKADDR_IN adrInfo = {};
        adrInfo.sin_family = AF_INET;
        adrInfo.sin_port = port;
        adrInfo.sin_addr.S_un.S_addr = INADDR_ANY;
        ifu (bind(m_hdl, (SOCKADDR*)&adrInfo, siz(adrInfo)) != 0)
            rete(ErrVal::YES_EXIST);
        ifu (listen(m_hdl, SOMAXCONN) != 0)
            rete(ErrVal::NET);
        rets;
    }
    dfa ER Accept(SockTcp& sockCli, NetAdrV4& adr)
    {
        adr.Clr();
        ifu (m_hdl == INVALID_SOCKET)
            rete(ErrVal::NO_INIT);
        ifu (sockCli.IsValid())
            rete(ErrVal::YES_INIT);
        SOCKADDR_IN adrInfo = {};
        int adrInfoSize = siz(adrInfo);
        cx AU result = accept(m_hdl, (SOCKADDR*)&adrInfo, &adrInfoSize);
        ifu (result == INVALID_SOCKET)
            rete(ErrVal::NET);
        sockCli.m_hdl = result;
        adr.ip = adrInfo.sin_addr.S_un.S_addr;
        adr.port = adrInfo.sin_port;
        m_isConnected = YES;
        rets;
    }
    dfa ER Write(CXGA buf, SI size) cx
    {
        ifu (m_hdl == INVALID_SOCKET)
            rete(ErrVal::NO_INIT);
        AU bufCur = (cx char*)buf;
        AU sizeRem = (int)size;
        jdst(again);
        cx AU result = send(m_hdl, bufCur, sizeRem, 0);
        ifu (result == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAECONNRESET)
                rete(ErrVal::NET_CLOSE);
            rete(ErrVal::NET);
        }
        if (sizeRem != result)
        {
            bufCur += result;
            sizeRem -= result;
            jsrc(again);
        }
        rets;
    }
    dfa ER Read(GA buf, SI& sizeResult, SI sizeMax, SI sizeMin = 1) cx
    {
        sizeResult = 0;
        ifu (m_hdl == INVALID_SOCKET)
            rete(ErrVal::NO_INIT);
        jdst(again);
        cx AU result = recv(m_hdl, (char*)buf, int(sizeMax), 0);
        ifu (result < 1)
        {
            if (result == 0)
                rete(ErrVal::NET_CLOSE);
            if (result == SOCKET_ERROR)
            {
                cx AU err = WSAGetLastError();
                if (err == WSAECONNRESET || err == WSAECONNABORTED)
                    rete(ErrVal::NET_CLOSE);
                if (err == WSAEINTR && !m_isConnected)
                    rete(ErrVal::NET_CLOSE);
                ConWriteDbg("[SockTcp::Read] recv win err %d", err);
                rete(ErrVal::NET);
            }
            ConWriteDbg("[SockTcp::Read] recv non-socketerr err");
            rete(ErrVal::NET);
        }
        sizeResult += result;
        if (sizeResult < sizeMin)
        {
            AsType<UA>(buf) += result;
            sizeMax -= result;
            jsrc(again);
        }
        rets;
    }
    dfa NT Move(SockTcp& src)
    {
        tx->Del();
        m_hdl = src.m_hdl;
        src.m_hdl = INVALID_SOCKET;
    }

  public:
    dfa SockTcp() : m_hdl(INVALID_SOCKET), m_isConnected(NO)
    {
        _NetInit(); // error ignored here, the following calls will handle it
    }
    dfa ~SockTcp()
    {
        tx->Del();
        _NetFree();
    }
};

dfa ER StrToNetAdrV4(NetAdrV4& out, cx CS* in)
{
    out.Clr();
    ifu (in == NUL)
        rets;
    AU cur = in;
    if (IsNumBase10(*cur))
    {
        ite (i, i < 4)
        {
            U4 val;
            cur += StrToInt(val, cur);
            ifu ((*cur != '.' && i != 3) || val > 0xFF)
            {
                out.Clr();
                rete(ErrVal::NO_VALID);
            }
            cur += (i != 3);
            out.ip <<= sizb(U1);
            out.ip |= val;
        }
        out.ip = RevByte(out.ip);
    }
    if (*cur == ':')
    {
        ++cur;
        U4 val;
        cur += StrToInt(val, cur);
        ifu (val > 0xFFFF)
        {
            out.Clr();
            rete(ErrVal::NO_VALID);
        }
        out.port = RevByte(U2(val));
    }
    ifu (*cur != '\0')
    {
        out.Clr();
        rete(ErrVal::NO_VALID);
    }
    rets;
}

#endif
