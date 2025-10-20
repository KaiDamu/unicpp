#include "event.hpp"

#include "error.hpp"
#include "atomic.hpp"
#include "thread.hpp"

dfa NT EvtFast::Set(BO isSignaled)
{
#ifdef PROG_THD_CNT_MULTI
    AtomSetU4(m_isSignaled, U4(isSignaled));
#endif
}
dfa NT EvtFast::Wait(BO doResetAfter)
{
#ifdef PROG_THD_CNT_MULTI
    SI spinCnt = 0;
    while (m_isSignaled == 0)
        ThdYield<64, 4000>(spinCnt++);
    if (doResetAfter)
        tx->Set(NO);
#endif
}
dfa EvtFast::EvtFast() : m_isSignaled(0)
{
}

#ifdef PROG_SYS_WIN

dfa ER _EvtWin::Free()
{
    if (m_hdl == NUL)
        rets;
    ifu (CloseHandle(m_hdl) == 0)
        rete(ErrVal::EVT);
    m_hdl = NUL;
    rets;
}
dfa ER _EvtWin::Init()
{
    ife (tx->Free())
        retep;
    m_hdl = CreateEventW(NUL, YES, NO, NUL);
    ifu (m_hdl == NUL)
        rete(ErrVal::EVT);
    rets;
}
dfa ER _EvtWin::Set(BO state)
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
dfa ER _EvtWin::Wait(BO stateAfter)
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
dfa _EvtWin::_EvtWin()
{
    m_hdl = NUL;
    tx->Init();
}
dfa _EvtWin::~_EvtWin()
{
    tx->Free();
}

#endif
