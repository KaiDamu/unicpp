#pragma once

constexpr SI PIPE_BUF_SIZE = 65536;

class PipeIn
{
  private:
    FileWin m_hdl;

  private:
    dfa ER _Connect(cx CH* pipeName)
    {
        CH path[PATH_LEN_MAX] = L"\\\\.\\pipe\\";
        StrAdd(path, pipeName);
        ife (m_hdl.Open(path, GENERIC_READ | GENERIC_WRITE, 0, OPEN_EXISTING, 0))
            retep;
        DWORD mode = PIPE_READMODE_MESSAGE;
        ifu (SetNamedPipeHandleState(m_hdl.Hdl(), &mode, NUL, NUL) == 0)
            rete(ERR_PIPE);
        rets;
    }

  public:
    dfa BO IsConnected()
    {
        ret m_hdl.IsOpen();
    }
    dfa ER Connect(cx CH* pipeName, SI tryCntMax, TmMain tryDelay)
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
        rete(ERR_HIGH_WAIT);
    }
    dfa ER Close()
    {
        ret m_hdl.Close();
    }
    dfa ER ReadWait(Arr<U1>& buf)
    {
        SI readResult = 0;
        SI readSize = 0;
        ife (m_hdl.Read(&readSize, siz(readSize), readResult))
        {
            tx->Close();
            rete(ERR_PIPE);
        }
        buf.Req(readSize, readSize, 0);
        buf.CurClr();
        SI readSizeRem = readSize;
        while (readSizeRem != 0)
        {
            ife (m_hdl.Read(buf.Cur(), readSizeRem, readResult))
            {
                tx->Close();
                rete(ERR_PIPE);
            }
            buf.CurMove(readResult);
            readSizeRem -= readResult;
        }
        rets;
    }

  public:
    dfa PipeIn()
    {
    }
};

class PipeOut
{
  private:
    HANDLE m_hdl;

  public:
    dfa ER Create(cx CH* pipeName)
    {
        CH path[PATH_LEN_MAX] = L"\\\\.\\pipe\\";
        StrAdd(path, pipeName);
        m_hdl = CreateNamedPipeW(path, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 1, PIPE_BUF_SIZE, PIPE_BUF_SIZE, 0, NUL);
        ifu (m_hdl == INVALID_HANDLE_VALUE)
            rete(ERR_PIPE);
        ifu (ConnectNamedPipe(m_hdl, NUL) == 0)
        {
            CloseHandle(m_hdl);
            m_hdl = INVALID_HANDLE_VALUE;
            rete(ERR_PIPE);
        }
        rets;
    }
    dfa ER Close()
    {
        if (m_hdl == INVALID_HANDLE_VALUE)
            rets;
        ifu (CloseHandle(m_hdl) == 0)
            rete(ERR_FILE);
        m_hdl = INVALID_HANDLE_VALUE;
        rets;
    }
    dfa ER Write(CXGA buf, SI size, SI& result)
    {
        result = 0;
        cx SI pieceCnt = size / PIPE_BUF_SIZE + (size % PIPE_BUF_SIZE != 0);
        DWORD tmp;
        ifu (WriteFile(m_hdl, &size, siz(size), &tmp, NUL) == 0)
            rete(ERR_FILE);
        SI i = 0;
        while (i < pieceCnt)
        {
            cx U1* piece = (cx U1*)buf + (i * PIPE_BUF_SIZE);
            cx SI pieceSize = (i + 1 == pieceCnt) ? (size % PIPE_BUF_SIZE) : PIPE_BUF_SIZE;
            ifu (WriteFile(m_hdl, piece, DWORD(pieceSize), &tmp, NUL) == 0)
                rete(ERR_FILE);
            result += tmp;
            ++i;
        }
        rets;
    }
    dfa ER Write(CXGA buf, SI size)
    {
        SI result;
        ret tx->Write(buf, size, result);
    }

  public:
    dfa PipeOut()
    {
        m_hdl = INVALID_HANDLE_VALUE;
    }
};
