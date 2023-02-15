#pragma once

constexpr U1 PRB_FIELD_TYPE_VARINT = 0;
constexpr U1 PRB_FIELD_TYPE_I64 = 1;
constexpr U1 PRB_FIELD_TYPE_LEN = 2;
constexpr U1 PRB_FIELD_TYPE_SGROUP = 3;
constexpr U1 PRB_FIELD_TYPE_EGROUP = 4;
constexpr U1 PRB_FIELD_TYPE_I32 = 5;

constexpr U1 PRB_FIELD_ID_NONE = 0;

constexpr SI PRB_CTX_STACK_LEN_MAX = 8;

class PrbCtx {
private:
	struct PrbCtxStackElem {
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
	dfa cx U1* Cur() {
		ret m_bufCur;
	}
	dfa U8 FieldHash() {
		ret m_fieldHash;
	}
	dfa SI StackLen() {
		ret m_stackLen;
	}
	dfa U4 ValU4() {
		ret m_valU4;
	}
	dfa U8 ValU8() {
		ret m_valU8;
	}
	dfa S4 ValS4() {
		ret m_valS4;
	}
	dfa S8 ValS8() {
		ret m_valS8;
	}
	dfa F4 ValF4() {
		ret m_valF4;
	}
	dfa F8 ValF8() {
		ret m_valF8;
	}
	dfa SI ValSI() {
		ret m_valSI;
	}
	dfa NT StackPush(cx U1* elemEnd, U1 fieldId) {
		Assert(m_stackLen < PRB_CTX_STACK_LEN_MAX);
		m_stack[m_stackLen].elemEnd = elemEnd;
		m_stack[m_stackLen].fieldId = fieldId;
		++m_stackLen;
		if (m_stackLen >= 2) m_stackHashByte[m_stackLen - 2] = fieldId;
	}
	dfa NT StackPop() {
		Assert(m_stackLen > 0);
		--m_stackLen;
		m_bufCur = m_stack[m_stackLen].elemEnd;
		if (m_stackLen >= 1) {
			m_stackHashByte[m_stackLen - 1] = PRB_FIELD_ID_NONE;
			if (m_stack[m_stackLen - 1].elemEnd == m_stack[m_stackLen].elemEnd) tx->StackPop();
		}
	}
	dfa NT Read() {
		Assert(m_stackLen > 0);
		cx U1 fieldType = *m_bufCur & 0b111;
		m_fieldId = *m_bufCur >> 3;
		++m_bufCur;
		switch (fieldType) {
		case PRB_FIELD_TYPE_VARINT:
		{
			cx SI readSize = VarintDecode<U8>(m_valU8, m_bufCur);
			m_bufCur += readSize;
			break;
		}
		case PRB_FIELD_TYPE_I64:
		{
			MemSet(&m_valU8, m_bufCur, siz(m_valU8));
			m_bufCur += siz(m_valU8);
			break;
		}
		case PRB_FIELD_TYPE_LEN:
		{
			cx SI readSize = VarintDecode<U8>(m_valU8, m_bufCur);
			m_bufCur += readSize;
			tx->StackPush(m_bufCur + m_valU8, m_fieldId);
			break;
		}
		case PRB_FIELD_TYPE_SGROUP:
		{
			break;
		}
		case PRB_FIELD_TYPE_EGROUP:
		{
			break;
		}
		case PRB_FIELD_TYPE_I32:
		{
			MemSet(&m_valU4, m_bufCur, siz(m_valU4));
			m_bufCur += siz(m_valU4);
			break;
		}
		default:
		{
			Assert(NO);
			break;
		}
		}
		m_fieldHash = (m_stackHash << sizb(m_fieldId)) + ((fieldType == PRB_FIELD_TYPE_LEN) ? 0x00 : m_fieldId);
		if (m_bufCur == m_stack[m_stackLen - 1].elemEnd) tx->StackPop();
	}
	dfa NT Clr() {
		m_bufBase = NUL;
		m_bufCur = NUL;
		m_bufEnd = NUL;
		m_fieldId = PRB_FIELD_ID_NONE;
		m_stackLen = 0;
		MemSet(m_stackHashByte, PRB_FIELD_ID_NONE, PRB_CTX_STACK_LEN_MAX);
		m_valU8 = 0;
	}
	dfa NT Init(cx U1* buf, SI bufSize) {
		tx->Clr();
		m_bufBase = buf;
		m_bufCur = buf;
		m_bufEnd = buf + bufSize;
		ifl (bufSize > 0) {
			tx->StackPush(m_bufEnd, PRB_FIELD_ID_NONE);
		}
	}
	dfa NT Init() {
		tx->Clr();
	}
public:
	dfa PrbCtx() {
		tx->Init();
	}
	dfa PrbCtx(cx U1* buf, SI bufSize) {
		tx->Init(buf, bufSize);
	}
	dfa ~PrbCtx() {
		tx->Clr();
	}
};
