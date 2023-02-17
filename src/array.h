#pragma once

tpl1 class DatIte {
private:
	T1* m_ptr;
	T1* m_cur;
	SI m_cap;
private:
	dfa NT Init() {
		m_ptr = NUL;
		m_cur = NUL;
		m_cap = 0;
	}
public:
	dfa T1* Ptr() const {
		ret m_ptr;
	}
	dfa T1* Cur() const {
		ret m_cur;
	}
	dfa SI Cap() const {
		ret m_cap;
	}
	dfa SI Pos() const {
		ret m_cur - m_ptr;
	}
	dfa NT Src(T1* ptr, SI cap) {
		m_ptr = ptr;
		m_cur = ptr;
		m_cap = cap;
	}
	dfa NT Src(T1* ptr) {
		tx->Src(ptr, SI_VAL_MAX);
	}
	dfa NT Read(T1& dat) {
		Assert(tx->Pos() < m_cap);
		dat = *m_cur;
		++m_cur;
	}
	dfa NT Read(T1* dat, SI cnt) {
		Assert(tx->Pos() + cnt <= m_cap);
		MemSet(dat, m_cur, cnt * siz(T1));
		m_cur += cnt;
	}
	dfa NT Write(cx T1& dat) {
		Assert(tx->Pos() < m_cap);
		*m_cur = dat;
		++m_cur;
	}
	dfa NT Write(cx T1* dat, SI cnt) {
		Assert(tx->Pos() + cnt <= m_cap);
		MemSet(m_cur, dat, cnt * siz(T1));
		m_cur += cnt;
	}
	dfa NT CurMove(SI cnt) {
		m_cur += cnt;
	}
	dfa NT CurSet(SI pos) {
		m_cur = tx->Ptr() + pos;
	}
	dfa NT CurClr() {
		tx->CurSet(0);
	}
	dfa NT Set(cx T1& dat) {
		tx->CurClr();
		tx->Write(dat);
	}
	dfa NT Set(cx T1* dat, SI cnt) {
		tx->CurClr();
		tx->Write(dat, cnt);
	}
public:
	dfa SI ReadLine(T1* dat) {
		AU curBase = m_cur;
		AU curEnd = m_ptr + m_cap;
		ifu (m_cur == curEnd) ret -1;
		while ((m_cur != curEnd) && (IsEol<T1>(*m_cur) == NO)) *dat++ = *m_cur++;
		cx SI len = m_cur - curBase;
		if (m_cur != curEnd) {
			++m_cur;
			if ((m_cur != curEnd) && (*m_cur == CH_LF) && (*(m_cur - 1) == CH_CR)) ++m_cur;
		}
		ret len;
	}
	dfa SI NextLineLen() const {
		AU cur = m_cur;
		AU curBase = cur;
		AU curEnd = m_ptr + m_cap;
		ifu (cur == curEnd) ret -1;
		while ((cur != curEnd) && (IsEol<T1>(*cur) == NO)) ++cur;
		ret cur - curBase;
	}
	dfa BO HasNextLine() const {
		ret (m_cur != m_ptr + m_cap);
	}
public:
	dfa T1& operator [] (SI i) const {
		Assert(i < m_cap);
		ret m_ptr[i];
	}
public:
	dfa DatIte() {
		tx->Init();
	}
	dfa DatIte(SI cnt) {
		tx->Init();
		tx->New(cnt);
	}
};

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
	dfa NT __Drop() {
		m_ptr.Set(NUL);
	}
	dfa NT __Set(T1* ptr) {
		m_ptr.Set(ptr);
	}
	dfa NT __CapMax() {
		m_cap = SI_VAL_MAX;
	}
public:
	dfa DatIte<T1> ToDatIte() const {
		DatIte<T1> datIte;
		datIte.Src(m_ptr.Get(), m_cap);
		datIte.CurMove(m_cur - m_ptr.Get());
		ret datIte;
	}
public:
	dfa NT Del() {
		ifu (m_ptr.Get() == NUL) ret;
		m_ptr.Del();
		m_cur = NUL;
		m_cap = 0;
	}
	dfa NT New(SI cnt) {
		ifu (m_ptr.Get() != NUL) tx->Del();
		m_ptr.New(cnt);
		m_cur = m_ptr.Get();
		m_cap = cnt;
	}
	dfa T1* Ptr() const {
		ret m_ptr.Get();
	}
	dfa T1* Cur() const {
		ret m_cur;
	}
	dfa SI Cap() const {
		ret m_cap;
	}
	dfa SI Pos() const {
		ret m_cur - m_ptr.Get();
	}
	dfa T1 Read() {
		Assert(tx->Pos() < m_cap);
		T1 dat = *m_cur;
		++m_cur;
		ret dat;
	}
	dfa NT Read(T1& dat) {
		Assert(tx->Pos() < m_cap);
		dat = *m_cur;
		++m_cur;
	}
	dfa NT Read(T1* dat, SI cnt) {
		Assert(tx->Pos() + cnt <= m_cap);
		MemSet(dat, m_cur, cnt * siz(T1));
		m_cur += cnt;
	}
	dfa NT Write(cx T1& dat) {
		Assert(tx->Pos() < m_cap);
		*m_cur = dat;
		++m_cur;
	}
	dfa NT Write(cx T1* dat, SI cnt) {
		Assert(tx->Pos() + cnt <= m_cap);
		MemSet(m_cur, dat, cnt * siz(T1));
		m_cur += cnt;
	}
	dfa NT CurMove(SI cnt) {
		m_cur += cnt;
	}
	dfa NT CurSet(SI pos) {
		m_cur = tx->Ptr() + pos;
	}
	dfa NT CurClr() {
		tx->CurSet(0);
	}
	dfa NT Set(cx T1& dat) {
		tx->CurClr();
		tx->Write(dat);
	}
	dfa NT Set(cx T1* dat, SI cnt) {
		tx->CurClr();
		tx->Write(dat, cnt);
	}
	dfa NT Resize(SI cntAlloc, SI cntCpy) {
		cx AU pos = tx->Pos();
		ifu (m_ptr.Get() == NUL) m_ptr.New(cntAlloc);
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
	dfa T1& operator [] (SI i) const {
		Assert(i < m_cap);
		ret m_ptr[i];
	}
	dfa operator DatIte<T1>() const {
		ret tx->ToDatIte();
	}
public:
	dfa Arr() {
		tx->Init();
	}
	dfa Arr(SI cnt) {
		tx->Init();
		tx->New(cnt);
	}
	dfa ~Arr() {
		tx->Del();
	}
};
