#include "ucnetsrv.hpp"

#ifdef PROG_SYS_WIN

dfa TypeTraits<Ucnet::CliSrv>::IdT& TypeTraits<Ucnet::CliSrv>::id(Ucnet::CliSrv& obj)
{
    ret obj.m_sessionId;
}
dfa cx TypeTraits<Ucnet::CliSrv>::IdT& TypeTraits<Ucnet::CliSrv>::id(cx Ucnet::CliSrv& obj)
{
    ret obj.m_sessionId;
}
dfa TypeTraits<Ucnet::CliSrv>::IdSecT& TypeTraits<Ucnet::CliSrv>::idSec(Ucnet::CliSrv& obj)
{
    ret obj.m_userName;
}
dfa TypeTraits<Ucnet::CliSrv>::LockT& TypeTraits<Ucnet::CliSrv>::lock(Ucnet::CliSrv& obj)
{
    ret obj.m_lock;
}

dfa BO TypeTraits<Ucnet::CliSrv>::HasIdSec(Ucnet::CliSrv& obj)
{
    ret obj.m_userName.size() > 0;
}
dfa NT TypeTraits<Ucnet::CliSrv>::Lock(Ucnet::CliSrv& obj)
{
    ret obj.m_lock.Lock();
}
dfa NT TypeTraits<Ucnet::CliSrv>::Unlock(Ucnet::CliSrv& obj)
{
    ret obj.m_lock.Unlock();
}

#endif
