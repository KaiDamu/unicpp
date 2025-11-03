#include "protobuf.hpp"

#include "debug.hpp"
#include "math.hpp"
#include "memory.hpp"

namespace protobuf
{

dfa cx U1* Ctx::Cur()
{
    ret m_bufCur;
}
dfa U8 Ctx::FieldHash()
{
    ret m_fieldHash;
}
dfa SI Ctx::StackLen()
{
    ret m_stackLen;
}
dfa U4 Ctx::ValU4()
{
    ret m_valU4;
}
dfa U8 Ctx::ValU8()
{
    ret m_valU8;
}
dfa S4 Ctx::ValS4()
{
    ret m_valS4;
}
dfa S8 Ctx::ValS8()
{
    ret m_valS8;
}
dfa F4 Ctx::ValF4()
{
    ret m_valF4;
}
dfa F8 Ctx::ValF8()
{
    ret m_valF8;
}
dfa SI Ctx::ValSI()
{
    ret m_valSI;
}
dfa NT Ctx::StackPush(cx U1* elemEnd, U1 fieldId)
{
    Assert(m_stackLen < CTX_STACK_LEN_MAX);
    m_stack[m_stackLen].elemEnd = elemEnd;
    m_stack[m_stackLen].fieldId = fieldId;
    ++m_stackLen;
    if (m_stackLen >= 2)
        m_stackHashByte[m_stackLen - 2] = fieldId;
}
dfa NT Ctx::StackPop()
{
    Assert(m_stackLen > 0);
    --m_stackLen;
    m_bufCur = m_stack[m_stackLen].elemEnd;
    if (m_stackLen >= 1)
    {
        m_stackHashByte[m_stackLen - 1] = FIELD_ID_NONE;
        if (m_stack[m_stackLen - 1].elemEnd == m_stack[m_stackLen].elemEnd)
            tx->StackPop();
    }
}
dfa NT Ctx::Read()
{
    Assert(m_stackLen > 0);
    cx U1 fieldType = *m_bufCur & 0b111;
    m_fieldId = *m_bufCur >> 3;
    ++m_bufCur;
    switch (fieldType)
    {
    case FIELD_TYPE_VARINT: {
        cx SI readSize = VarintDecode<U8>(m_valU8, m_bufCur);
        m_bufCur += readSize;
        break;
    }
    case FIELD_TYPE_I64: {
        MemCpy(&m_valU8, m_bufCur, siz(m_valU8));
        m_bufCur += siz(m_valU8);
        break;
    }
    case FIELD_TYPE_LEN: {
        cx SI readSize = VarintDecode<U8>(m_valU8, m_bufCur);
        m_bufCur += readSize;
        tx->StackPush(m_bufCur + m_valU8, m_fieldId);
        break;
    }
    case FIELD_TYPE_SGROUP: {
        break;
    }
    case FIELD_TYPE_EGROUP: {
        break;
    }
    case FIELD_TYPE_I32: {
        MemCpy(&m_valU4, m_bufCur, siz(m_valU4));
        m_bufCur += siz(m_valU4);
        break;
    }
    default: {
        Assert(NO);
        break;
    }
    }
    m_fieldHash = (m_stackHash << sizb(m_fieldId)) + ((fieldType == FIELD_TYPE_LEN) ? 0x00 : m_fieldId);
    if (m_bufCur == m_stack[m_stackLen - 1].elemEnd)
        tx->StackPop();
}
dfa NT Ctx::Clr()
{
    m_bufBase = NUL;
    m_bufCur = NUL;
    m_bufEnd = NUL;
    m_fieldId = FIELD_ID_NONE;
    m_stackLen = 0;
    MemSet(m_stackHashByte, FIELD_ID_NONE, CTX_STACK_LEN_MAX);
    m_valU8 = 0;
}
dfa NT Ctx::Init(cx U1* buf, SI bufSize)
{
    tx->Clr();
    m_bufBase = buf;
    m_bufCur = buf;
    m_bufEnd = buf + bufSize;
    ifl (bufSize > 0)
    {
        tx->StackPush(m_bufEnd, FIELD_ID_NONE);
    }
}
dfa NT Ctx::Init()
{
    tx->Clr();
}
dfa Ctx::Ctx()
{
    tx->Init();
}
dfa Ctx::Ctx(cx U1* buf, SI bufSize)
{
    tx->Init(buf, bufSize);
}
dfa Ctx::~Ctx()
{
    tx->Clr();
}

} // namespace protobuf
