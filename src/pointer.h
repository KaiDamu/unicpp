#pragma once

tpl1 class Ptr {
private:
	T1* m_ptr;
public:
	dfa NT Alloc(SI cnt) {
		Assert(m_ptr == NUL);
		m_ptr = new T1[cnt];
	}
	dfa NT Dealloc() {
		Assert(m_ptr != NUL);
		delete[] m_ptr;
		m_ptr = NUL;
	}
	dfa NT Resize(SI cntAlloc, SI cntCpy) {
		Assert(m_ptr != NUL);
		Assert(cntAlloc >= cntCpy);
		m_ptr = reinterpret_cast<T1*>(MemResize(m_ptr, cntAlloc * siz(T1), cntCpy * siz(T1)));
		new(m_ptr + cntCpy) T1[cntAlloc - cntCpy]();
	}
	dfa NT Val(T1* val) {
		m_ptr = val;
	}
	dfa T1* Val() const {
		ret m_ptr;
	}
public:
	dfa T1& operator [] (SI i) {
		ret m_ptr[i];
	}
public:
	dfa Ptr() {
		m_ptr = NUL;
	}
	dfa Ptr(SI cnt) {
		m_ptr = NUL;
		tx->Alloc(cnt);
	}
};
