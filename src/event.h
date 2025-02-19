#pragma once

#ifdef PROG_SYS_WIN

class EvtWin
{
  private:
    HANDLE m_hdl;

  public:
    dfa ER Free()
    {
        if (m_hdl == NUL)
            rets;
        ifu (CloseHandle(m_hdl) == 0)
            rete(ErrVal::EVT);
        m_hdl = NUL;
        rets;
    }
    dfa ER Init()
    {
        ife (tx->Free())
            retep;
        m_hdl = CreateEventW(NUL, YES, NO, NUL);
        ifu (m_hdl == NUL)
            rete(ErrVal::EVT);
        rets;
    }
    dfa ER Set(BO state)
    {
        ifu (m_hdl == NUL)
            rete(ErrVal::NO_INIT);
        if (state)
        {
            ifu (SetEvent(m_hdl) == 0)
                rete(ErrVal::EVT);
        }
        else
        {
            ifu (ResetEvent(m_hdl) == 0)
                rete(ErrVal::EVT);
        }
        rets;
    }
    dfa ER Wait(BO stateAfter)
    {
        ifu (m_hdl == NUL)
            rete(ErrVal::NO_INIT);
        ifu (WaitForSingleObject(m_hdl, INFINITE) != WAIT_OBJECT_0)
            rete(ErrVal::EVT);
        if (stateAfter == NO)
            ife (tx->Set(stateAfter))
                retep;
        rets;
    }

  public:
    dfa EvtWin()
    {
        m_hdl = NUL;
        tx->Init();
    }
    dfa ~EvtWin()
    {
        tx->Free();
    }
};

#endif
