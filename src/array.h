#pragma once

tpl1 class Arr {
private:
	Ptr<T1> m_ptr;
	T1* m_cur;
	SI m_cap;
private:
	dfa NT Init() {
		m_cur = NUL;
		m_cap = 0;
	}
public:
	dfa NT __SetPtr(T1* ptr) {
		m_ptr.Val(ptr);
	}
	dfa NT __NoCap() {
		m_cap = SI_VAL_MAX;
	}
	dfa NT __DropPtr() {
		m_ptr.Val(NUL);
	}
public:
	dfa T1* Ptr() const {
		ret m_ptr.Val();
	}
	dfa T1* Cur() const {
		ret m_cur;
	}
	dfa SI Cap() const {
		ret m_cap;
	}
	dfa SI Pos() const {
		ret m_cur - m_ptr.Val();
	}
	dfa NT CurMove(SI cnt) {
		m_cur += cnt;
	}
	dfa NT CurSet(SI pos) {
		m_cur = m_ptr.Val() + pos;
	}
	dfa NT CurRewind() {
		tx->CurSet(0);
	}
	dfa NT Read(T1& dat) {
		Assert(tx->Pos() < m_cap);
		dat = *m_cur;
		++m_cur;
	}
	dfa NT Read(T1* dat, SI cnt) {
		Assert(tx->Pos() + cnt <= m_cap);
		MemCpy(dat, m_cur, cnt * siz(T1));
		m_cur += cnt;
	}
	dfa NT Add(cx T1& dat) {
		Assert(tx->Pos() < m_cap);
		*m_cur = dat;
		++m_cur;
	}
	dfa NT Add(cx T1* dat, SI cnt) {
		Assert(tx->Pos() + cnt <= m_cap);
		MemCpy(m_cur, dat, cnt * siz(T1));
		m_cur += cnt;
	}
	dfa NT Set(cx T1& dat) {
		tx->CurRewind();
		tx->Add(dat);
	}
	dfa NT Set(cx T1* dat, SI cnt) {
		tx->CurRewind();
		tx->Add(dat, cnt);
	}
	dfa NT Dealloc() {
		ifu (m_ptr.Val() == NUL) ret;
		m_ptr.Dealloc();
		m_cur = NUL;
		m_cap = 0;
	}
	dfa NT Alloc(SI cnt) {
		ifu (m_ptr.Val() != NUL) tx->Dealloc();
		m_ptr.Alloc(cnt);
		m_cur = m_ptr.Val();
		m_cap = cnt;
	}
	dfa NT Resize(SI cntAlloc, SI cntCpy) {
		cx SI pos = tx->Pos();
		ifu (m_ptr.Val() == NUL) m_ptr.Alloc(cntAlloc);
		else m_ptr.Resize(cntAlloc, cntCpy);
		tx->CurSet(Min<SI>(pos, cntAlloc));
		m_cap = cntAlloc;
	}
	dfa NT Req(SI cntReq, SI cntAlloc, SI cntCpy) {
		ifu (m_cap < cntReq) tx->Resize(cntAlloc, cntCpy);
	}
	dfa NT Req(SI cntReq, SI cntAlloc) {
		tx->Req(cntReq, cntAlloc, tx->Pos());
	}
	dfa NT Req(SI cntReq) {
		tx->Req(cntReq, cntReq, tx->Pos());
	}
public:
	dfa T1& operator [] (SI i) {
		Assert(i < m_cap);
		ret m_ptr[i];
	}
public:
	dfa Arr() {
		tx->Init();
	}
	dfa Arr(SI cap) {
		tx->Init();
		tx->Alloc(cap);
	}
	dfa ~Arr() {
		tx->Dealloc();
	}
};
