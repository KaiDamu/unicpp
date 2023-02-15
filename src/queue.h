#pragma once

tpl1 class DQueue {
private:
	struct DQueueElem {
		DQueueElem* prev;
		DQueueElem* next;
		T1 dat;
	};
private:
	DQueueElem* m_first;
	DQueueElem* m_last;
	SI m_len;
public:
	dfa BO IsEmpty() const {
		ret m_first == NUL;
	}
	dfa SI Len() const {
		ret m_len;
	}
	dfa NT Add(cx T1& dat) {
		DQueueElem* elem = new DQueueElem;
		elem->prev = m_last;
		elem->next = NUL;
		elem->dat = dat;
		ifu (m_last == NUL) m_first = elem;
		else m_last->next = elem;
		m_last = elem;
		++m_len;
	}
	dfa T1& Get(T1& dat) {
		Assert(tx->IsEmpty() == NO);
		DQueueElem* elem = m_first;
		m_first = elem->next;
		ifu (m_first == NUL) m_last = NUL;
		else m_first->prev = NUL;
		dat = elem->dat;
		delete elem;
		--m_len;
		ret dat;
	}
	dfa NT Clr() {
		DQueueElem* elem;
		while ((elem = m_first) != NUL) {
			m_first = elem->next;
			delete elem;
		}
		m_first = NUL;
		m_last = NUL;
		m_len = 0;
	}
public:
	dfa DQueue() {
		m_first = NUL;
		m_last = NUL;
		m_len = 0;
	}
	dfa ~DQueue() {
		tx->Clr();
	}
};
