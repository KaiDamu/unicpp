#pragma once

#include "file.hpp"
#include "time.hpp"
#include "array.hpp"

cxex SI PIPE_BUF_SIZE = 65536;

#ifdef PROG_SYS_WIN

class PipeIn
{
  private:
    File m_hdl;

  private:
    dfa ER _Connect(cx CH* pipeName);

  public:
    dfa BO IsConnected();
    dfa ER Connect(cx CH* pipeName, SI tryCntMax, TmMain tryDelay);
    dfa ER Close();
    dfa ER ReadWait(Arr<U1>& buf);

  public:
    dfa PipeIn();
};

class PipeOut
{
  private:
    HANDLE m_hdl;

  public:
    dfa ER Create(cx CH* pipeName);
    dfa ER Close();
    dfa ER Write(CXGA buf, SI size, SI& result);
    dfa ER Write(CXGA buf, SI size);

  public:
    dfa PipeOut();
};

#endif

#include "pipe.ipp"
