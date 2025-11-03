#include "ucnetsrv.hpp"

#ifdef PROG_SYS_WIN

dfa TypeTraits<ucnet::CliSrv>::IdT& TypeTraits<ucnet::CliSrv>::id(ucnet::CliSrv& obj)
{
    ret obj.m_sessionId;
}
dfa cx TypeTraits<ucnet::CliSrv>::IdT& TypeTraits<ucnet::CliSrv>::id(cx ucnet::CliSrv& obj)
{
    ret obj.m_sessionId;
}
dfa TypeTraits<ucnet::CliSrv>::IdSecT& TypeTraits<ucnet::CliSrv>::idSec(ucnet::CliSrv& obj)
{
    ret obj.m_userName;
}
dfa TypeTraits<ucnet::CliSrv>::LockT& TypeTraits<ucnet::CliSrv>::lock(ucnet::CliSrv& obj)
{
    ret obj.m_lock;
}

dfa BO TypeTraits<ucnet::CliSrv>::HasIdSec(ucnet::CliSrv& obj)
{
    ret obj.m_userName.size() > 0;
}
dfa NT TypeTraits<ucnet::CliSrv>::Lock(ucnet::CliSrv& obj)
{
    ret obj.m_lock.Lock();
}
dfa NT TypeTraits<ucnet::CliSrv>::Unlock(ucnet::CliSrv& obj)
{
    ret obj.m_lock.Unlock();
}

#endif
