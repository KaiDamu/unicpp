#include "serial.hpp"

namespace serial
{

dfa ER _ReadAll(cx U1*& cur, cx U1* end)
{
    rets;
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

dfa ER _WriteAll(std::vector<U1>& buf, SI& curI)
{
    rets;
}

dfa TransIo::TransIo()
{
}
dfa TransIo::TransIo(std::tuple<cx U1*&, cx U1*&>* read) : isWrite(NO), read(read), write(NUL)
{
}
dfa TransIo::TransIo(std::tuple<std::vector<U1>&, SI&>* write) : isWrite(YES), read(NUL), write(write)
{
}

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
    GetTypeU<TO(val.val)> tmp;
    ifep(VarintDecode(tmp, readSize, cur, end));
    cur += readSize;
    val.val = ZigzagDecode(tmp);
    rets;
}

tpl0 dfa ER WriteType(std::vector<U1>& buf, SI& curI, cx std::string& val)
{
    Req(buf, curI + VarintSizeMax<SI>() + SI(val.size()));
    curI += VarintEncode(&buf[curI], GetTypeU<SI>(val.size()));
    curI += MemCpy(&buf[curI], val.data(), val.size());
    rets;
}
tpl0 dfa ER WriteType(std::vector<U1>& buf, SI& curI, cx ZzVarint& val)
{
    Req(buf, curI + VarintSizeMax<TO(val.val)>());
    curI += VarintEncode(&buf[curI], ZigzagEncode(val.val));
    rets;
}

} // namespace serial
