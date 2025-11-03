#include "serial.hpp"

#include "memory.hpp"

namespace serial
{

tpl1 dfa ER ReadType(cx U1*& cur, cx U1* end, T1& val)
{
    static_assert(std::is_trivially_copyable<T1>::value, "Unsupported type for serial::ReadType");
    ifu (SI(end - cur) < siz(T1))
        rete(ErrVal::NO_VALID);
    cur += MemCpy(&val, cur, siz(T1));
    rets;
}
tpl<typename T1, typename... TRest> dfa ER _ReadAll(cx U1*& cur, cx U1* end, T1& first, TRest&... rest)
{
    ifep(ReadType(cur, end, first));
    ret _ReadAll(cur, end, rest...);
}
tpl<typename... TArgs> dfa ER Read(cx U1*& cur, cx U1* end, TArgs&... args)
{
    ret _ReadAll(cur, end, args...);
}

tpl1 dfa ER WriteType(std::vector<U1>& buf, SI& curI, cx T1& val)
{
    static_assert(std::is_trivially_copyable<T1>::value, "Unsupported type for serial::WriteType");
    Req(buf, curI + siz(T1));
    curI += MemCpy(&buf[curI], &val, siz(T1));
    rets;
}
tpl<typename T1, typename... TRest> dfa ER _WriteAll(std::vector<U1>& buf, SI& curI, cx T1& first, cx TRest&... rest)
{
    ifep(WriteType(buf, curI, first));
    ret _WriteAll(buf, curI, rest...);
}
tpl<typename... TArgs> dfa ER Write(std::vector<U1>& buf, SI& curI, cx TArgs&... args)
{
    ret _WriteAll(buf, curI, args...);
}

tpl<typename T1, typename... TArgs> dfa ER Trans(T1&, TArgs&...)
{
    static_assert(NO && siz(T1), "serial::Trans could not deduce the action");
    rete(ErrVal::NA);
}
tpl<typename... TArgs> dfa ER Trans(std::tuple<cx U1*&, cx U1*&>& io, TArgs&... args)
{
    AU& cur = std::get<0>(io);
    AU end = std::get<1>(io);
    ret Read(cur, end, args...);
}
tpl<typename... TArgs> dfa ER Trans(std::tuple<std::vector<U1>&, SI&>& io, TArgs&... args)
{
    AU& buf = std::get<0>(io);
    AU& curI = std::get<1>(io);
    ret Write(buf, curI, args...);
}
tpl<typename... TArgs> dfa ER Trans(TransIo& io, TArgs&... args)
{
    ret (io.isWrite ? Trans(*io.write, args...) : Trans(*io.read, args...));
}

} // namespace serial
