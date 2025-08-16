#pragma once

namespace Serial
{

tpl1 dfa ER ReadType(cx U1*& cur, cx U1* end, T1& val)
{
    static_assert(std::is_trivially_copyable<T1>::value, "Unsupported type for Serial::ReadType");
    ifu (SI(end - cur) < siz(T1))
        rete(ErrVal::NO_VALID);
    MemCpyUpdCurSrc(&val, cur, siz(T1));
    rets;
}
dfa ER _ReadAll(cx U1*& cur, cx U1* end)
{
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

dfa NT Req(std::vector<U1>& buf, SI reqSize)
{
    ifu (SI(buf.size()) < reqSize)
    {
        AU newSize = SI(buf.size() + 1) << 1;
        while (newSize < reqSize)
            newSize <<= 1;
        buf.resize(newSize);
    }
}

tpl1 dfa ER WriteType(std::vector<U1>& buf, SI& curI, cx T1& val)
{
    static_assert(std::is_trivially_copyable<T1>::value, "Unsupported type for Serial::WriteType");
    Req(buf, curI + siz(T1));
    MemCpy(&buf[curI], &val, siz(T1));
    curI += siz(T1);
    rets;
}
dfa ER _WriteAll(std::vector<U1>& buf, SI& curI)
{
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

struct TransIo
{
    BO isWrite;
    std::tuple<cx U1*&, cx U1*&>* read;
    std::tuple<std::vector<U1>&, SI&>* write;

    dfa TransIo()
    {
    }
    dfa TransIo(std::tuple<cx U1*&, cx U1*&>* read) : isWrite(NO), read(read), write(NUL)
    {
    }
    dfa TransIo(std::tuple<std::vector<U1>&, SI&>* write) : isWrite(YES), read(NUL), write(write)
    {
    }
};

tpl<typename T1, typename... TArgs> dfa ER Trans(T1&, TArgs&...)
{
    static_assert(NO, "Serial::Trans could not deduce the action");
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

} // namespace Serial

namespace Serial
{

tpl0 dfa ER ReadType(cx U1*& cur, cx U1* end, std::string& val)
{
    SI strLen, readSize;
    ifep(VarintDecode(strLen, readSize, cur, end));
    cur += readSize;
    ifu (SI(end - cur) < strLen)
        rete(ErrVal::NO_VALID);
    val.assign((cx CS*)cur, strLen);
    cur += strLen;
    rets;
}
tpl0 dfa ER ReadType(cx U1*& cur, cx U1* end, ZzVarint& val)
{
    SI readSize;
    ifep(VarintDecode(val.val, readSize, cur, end));
    cur += readSize;
    val.val = ZigzagDecode(val.val);
    rets;
}

tpl0 dfa ER WriteType(std::vector<U1>& buf, SI& curI, cx std::string& val)
{
    Req(buf, curI + VarintSizeMax<SI>() + SI(val.size()));
    curI += VarintEncode(&buf[curI], SI(val.size()));
    MemCpy(&buf[curI], val.data(), val.size());
    curI += val.size();
    rets;
}
tpl0 dfa ER WriteType(std::vector<U1>& buf, SI& curI, cx ZzVarint& val)
{
    Req(buf, curI + VarintSizeMax<TO(val.val)>());
    curI += VarintEncode(&buf[curI], ZigzagEncode(val.val));
    rets;
}

} // namespace Serial
