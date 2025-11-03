#include "file.hpp"

tpl1 dfa ER FileToBuf(std::vector<T1>& buf, cx CH* path, CXGA bufAppend, SI sizeAppend)
{
    buf.clear();
    File file;
    ifep(file.OpenRead(path));
    SI size;
    ifep(file.SizeGet(size));
    buf.resize(size + sizeAppend);
    ifep(file.Read(buf.data(), size));
    if (sizeAppend != 0)
        MemCpy(buf.data() + size, bufAppend, sizeAppend);
    ifep(file.Close());
    rets;
}
tpl1 dfa SI MemFile::Read(cx std::span<T1>& buf)
{
    ret tx->Read(buf.data(), buf.size_bytes());
}
tpl<typename TRead, typename TDst> dfa ER MemFile::ReadVal(TDst& val)
{
    ifcx (IsTypeSame<TRead, TDst>)
    {
        ifu (tx->Read((GA)&val, siz(val)) != siz(val))
            rete(ErrVal::FILE);
    }
    else
    {
        TRead tmp;
        ifu (tx->Read((GA)&tmp, siz(tmp)) != siz(tmp))
            rete(ErrVal::FILE);
        val = TDst(tmp);
    }
    rets;
}
tpl1 dfa SI MemFile::ReadVarint(T1& val)
{
    ifl (m_end - m_cur > (VarintSizeMax<T1>()))
    {
        cx AU size = VarintDecode<T1>(val, m_cur);
        m_cur += size;
        ret size;
    }
    U1 buf[VarintSizeMax<T1>()];
    SI size = 0;
    do
    {
        ifu (m_cur == m_end)
        {
            val = 0;
            ret 0;
        }
        *(buf + size) = *m_cur;
        ++m_cur;
        ++size;
    } while (VarintIsIncomplete(buf, size));
    ret VarintDecode<T1>(val, buf);
}
tpl1 dfa SI MemFile::ReadVarbint(T1& val, U1 base)
{
    ifl (m_end - m_cur > (VarbintSizeMax<T1, 1>()))
    {
        cx AU size = VarbintDecode<T1>(val, m_cur, base);
        m_cur += size;
        ret size;
    }
    U1 buf[VarbintSizeMax<T1, 1>()];
    SI size = 0;
    do
    {
        ifu (m_cur == m_end)
        {
            val = 0;
            ret 0;
        }
        *(buf + size) = *m_cur;
        ++m_cur;
        ++size;
    } while (VarbintIsIncomplete(buf, size, base));
    ret VarbintDecode<T1>(val, buf, base);
}
tpl<typename T1, ValSeqBoxMode TMode> dfa SI MemFile::ReadValSeqBox(std::vector<T1>& vals)
{
    vals.clear();
    SI datSize = 0;
    cx AU readSize = tx->ReadVarbint(datSize, 7);
    ifu (readSize == 0)
        ret 0;
    cx AU fullSize = readSize + datSize;
    tx->CurMove(-readSize);
    BitVec box;
    box.Reserve(fullSize * BIT_IN_BYTE);
    box._LenbSet(fullSize * BIT_IN_BYTE);
    MemCpy(box._Dat(), m_cur, fullSize);
    m_cur += fullSize;
    ValSeqBoxDecode<T1, TMode>(vals, box);
    ret fullSize;
}
tpl1 dfa NT MemFile::Write(cx std::span<cx T1>& buf)
{
    tx->Write(buf.data(), buf.size_bytes());
}
tpl1 dfa NT MemFile::WriteVal(cx T1& val)
{
    tx->Write((CXGA)&val, siz(T1));
}
tpl1 dfa NT MemFile::WriteVarint(cx T1& val)
{
    U1 buf[VarintSizeMax<T1>()];
    cx AU size = VarintEncode<T1>(buf, val);
    tx->Write(buf, size);
}
