#include "pipe.hpp"

#ifdef PROG_SYS_WIN

    #include "thread.hpp"

dfa ER PipeIn::_Connect(cx CH* pipeName)
{
    CH path[PATH_LEN_MAX] = L"\\\\.\\pipe\\";
    StrAdd(path, pipeName);
    ife (m_hdl._OpenWin(path, GENERIC_READ | GENERIC_WRITE, 0, OPEN_EXISTING, 0))
        retep;
    DWORD mode = PIPE_READMODE_MESSAGE;
    ifu (SetNamedPipeHandleState(m_hdl.Hdl(), &mode, NUL, NUL) == 0)
        rete(ErrVal::PIPE);
    rets;
}
dfa BO PipeIn::IsConnected()
{
    ret m_hdl.IsOpen();
}
dfa ER PipeIn::Connect(cx CH* pipeName, SI tryCntMax, TmMain tryDelay)
{
    SI tryCnt = 0;
    while (tryCnt != tryCntMax)
    {
        ++tryCnt;
        ife (tx->_Connect(pipeName))
        {
            ifl (GetLastError() == ERROR_FILE_NOT_FOUND)
                ThdWait(tryDelay);
            else
                retep;
        }
        else
        {
            rets;
        }
    }
    rete(ErrVal::HIGH_WAIT);
}
dfa ER PipeIn::Close()
{
    ret m_hdl.Close();
}
dfa ER PipeIn::ReadWait(Arr<U1>& buf)
{
    SI readResult = 0;
    SI readSize = 0;
    ife (m_hdl.Read(&readSize, siz(readSize), readResult))
    {
        tx->Close();
        rete(ErrVal::PIPE);
    }
    buf.Req(readSize, readSize, 0);
    buf.CurClr();
    SI readSizeRem = readSize;
    while (readSizeRem != 0)
    {
        ife (m_hdl.Read(buf.Cur(), readSizeRem, readResult))
        {
            tx->Close();
            rete(ErrVal::PIPE);
        }
        buf.CurMove(readResult);
        readSizeRem -= readResult;
    }
    rets;
}
dfa PipeIn::PipeIn()
{
}

dfa ER PipeOut::Create(cx CH* pipeName)
{
    CH path[PATH_LEN_MAX] = L"\\\\.\\pipe\\";
    StrAdd(path, pipeName);
    m_hdl = CreateNamedPipeW(path, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 1, PIPE_BUF_SIZE, PIPE_BUF_SIZE, 0, NUL);
    ifu (m_hdl == INVALID_HANDLE_VALUE)
        rete(ErrVal::PIPE);
    ifu (ConnectNamedPipe(m_hdl, NUL) == 0)
    {
        CloseHandle(m_hdl);
        m_hdl = INVALID_HANDLE_VALUE;
        rete(ErrVal::PIPE);
    }
    rets;
}
dfa ER PipeOut::Close()
{
    if (m_hdl == INVALID_HANDLE_VALUE)
        rets;
    ifu (CloseHandle(m_hdl) == 0)
        rete(ErrVal::FILE);
    m_hdl = INVALID_HANDLE_VALUE;
    rets;
}
dfa ER PipeOut::Write(CXGA buf, SI size, SI& result)
{
    result = 0;
    cx SI pieceCnt = size / PIPE_BUF_SIZE + (size % PIPE_BUF_SIZE != 0);
    DWORD tmp;
    ifu (WriteFile(m_hdl, &size, siz(size), &tmp, NUL) == 0)
        rete(ErrVal::FILE);
    SI i = 0;
    while (i < pieceCnt)
    {
        cx U1* piece = (cx U1*)buf + (i * PIPE_BUF_SIZE);
        cx SI pieceSize = (i + 1 == pieceCnt) ? (size % PIPE_BUF_SIZE) : PIPE_BUF_SIZE;
        ifu (WriteFile(m_hdl, piece, DWORD(pieceSize), &tmp, NUL) == 0)
            rete(ErrVal::FILE);
        result += tmp;
        ++i;
    }
    rets;
}
dfa ER PipeOut::Write(CXGA buf, SI size)
{
    SI result;
    ret tx->Write(buf, size, result);
}
dfa PipeOut::PipeOut()
{
    m_hdl = INVALID_HANDLE_VALUE;
}

#endif
