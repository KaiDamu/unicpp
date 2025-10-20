#pragma once

#include "type.hpp"

cxex U1 PRB_FIELD_TYPE_VARINT = 0;
cxex U1 PRB_FIELD_TYPE_I64 = 1;
cxex U1 PRB_FIELD_TYPE_LEN = 2;
cxex U1 PRB_FIELD_TYPE_SGROUP = 3;
cxex U1 PRB_FIELD_TYPE_EGROUP = 4;
cxex U1 PRB_FIELD_TYPE_I32 = 5;

cxex U1 PRB_FIELD_ID_NONE = 0;

cxex SI PRB_CTX_STACK_LEN_MAX = 8;

class PrbCtx
{
  private:
    struct PrbCtxStackElem
    {
        cx U1* elemEnd;
        U1 fieldId;
    };

  private:
    cx U1* m_bufBase;
    cx U1* m_bufCur;
    cx U1* m_bufEnd;
    U1 m_fieldId;
    U8 m_fieldHash;
    SI m_stackLen;
    PrbCtxStackElem m_stack[PRB_CTX_STACK_LEN_MAX];
    union {
        U8 m_stackHash;
        U1 m_stackHashByte[PRB_CTX_STACK_LEN_MAX];
    };
    union {
        U4 m_valU4;
        U8 m_valU8;
        S4 m_valS4;
        S8 m_valS8;
        F4 m_valF4;
        F8 m_valF8;
        SI m_valSI;
    };

  public:
    dfa cx U1* Cur();
    dfa U8 FieldHash();
    dfa SI StackLen();
    dfa U4 ValU4();
    dfa U8 ValU8();
    dfa S4 ValS4();
    dfa S8 ValS8();
    dfa F4 ValF4();
    dfa F8 ValF8();
    dfa SI ValSI();
    dfa NT StackPush(cx U1* elemEnd, U1 fieldId);
    dfa NT StackPop();
    dfa NT Read();
    dfa NT Clr();
    dfa NT Init(cx U1* buf, SI bufSize);
    dfa NT Init();

  public:
    dfa PrbCtx();
    dfa PrbCtx(cx U1* buf, SI bufSize);
    dfa ~PrbCtx();
};

#include "protobuf.ipp"
