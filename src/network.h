#pragma once

#ifdef PROG_SYS_WIN

    #ifdef PROG_COMPILER_MSVC
        #pragma comment(lib, "Ws2_32.lib")
    #endif

dfa ER NetInit()
{
    thdlocal BO isInitDone = NO;
    if (isInitDone)
        rets;
    WSADATA tmp;
    ifu (WSAStartup(0x0202, &tmp) != 0)
        rete(ErrVal::NET);
    isInitDone = YES;
    rets;
}
dfa ER NetFree()
{
    ifu ((WSACleanup() != 0) && (WSAGetLastError() != WSANOTINITIALISED))
        rete(ErrVal::NET);
    rets;
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

  public:
    dfa NetAdrV4(U4 ip = 0, U2 port = 0) : ip(ip), port(port)
    {
    }
};

class SockTcp
{
  private:
    SOCKET m_hdl;

  public:
    dfa BO IsValid() cx
    {
        ret m_hdl != INVALID_SOCKET;
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
        rets;
    }
    dfa ER Del()
    {
        if (m_hdl == INVALID_SOCKET)
            rets;
        ifu (closesocket(m_hdl) != 0)
            rete(ErrVal::NET);
        m_hdl = INVALID_SOCKET;
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
    dfa ER Connect(cx NetAdrV4& adr) cx
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
    dfa ER Accept(SockTcp& sockCli) cx
    {
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
    dfa ER Read(GA buf, SI& sizeResult, SI sizeMax) cx
    {
        sizeResult = 0;
        ifu (m_hdl == INVALID_SOCKET)
            rete(ErrVal::NO_INIT);
        cx AU result = recv(m_hdl, (char*)buf, int(sizeMax), 0);
        ifu (result < 1)
        {
            if (result == 0)
                rete(ErrVal::NET_CLOSE);
            if (result == SOCKET_ERROR)
            {
                if (WSAGetLastError() == WSAECONNRESET)
                    rete(ErrVal::NET_CLOSE);
                rete(ErrVal::NET);
            }
            rete(ErrVal::NET);
        }
        sizeResult = result;
        rets;
    }

  public:
    dfa SockTcp() : m_hdl(INVALID_SOCKET)
    {
    }
    dfa ~SockTcp()
    {
        tx->Del();
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
