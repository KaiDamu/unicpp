#pragma once

dfa NT MemSet(GA ptr, U1 val, SI size) {
	#ifdef PROG_COMPILER_GCC
		__builtin_memset(ptr, val, size);
	#else
		memset(ptr, val, size);
	#endif
}
dfa NT MemCpy(GA dst, CXGA src, SI size) {
	#ifdef PROG_COMPILER_GCC
		__builtin_memcpy(dst, src, size);
	#else
		memcpy(dst, src, size);
	#endif
}
dfa S1 MemCmp(CXGA ptr1, CXGA ptr2, SI size) {
	ret __builtin_memcmp(ptr1, ptr2, size);
}

dfa SI MemPageSizeGet() {
	SYSTEM_INFO info;
	info.dwPageSize = 0;
	GetSystemInfo(&info);
	ret SI(info.dwPageSize);
}

dfa NT MemObfuscate(GA dst, CXGA src, SI size) {
	U1* _dst = (U1*)dst;
	cx U1* _src = (cx U1*)src;
	ite (i, i < size) {
		*_dst = ObfuscateByte(*_src, i);
		++_dst;
		++_src;
	}
}
dfa NT MemUnobfuscate(GA dst, CXGA src, SI size) {
	U1* _dst = (U1*)dst;
	cx U1* _src = (cx U1*)src;
	ite (i, i < size) {
		*_dst = UnobfuscateByte(*_src, i);
		++_dst;
		++_src;
	}
}

dfa ER MemAllocSys(GA* ptr, SI size) {
	GA ptr_ = NUL;
	SIZE_T size_ = size;
	ifu (NtAllocateVirtualMemory(ProcCurNtHdlGet(), &ptr_, 0, &size_, MEM_COMMIT, PAGE_READWRITE) != STATUS_SUCCESS) {
		*ptr = NUL;
		rete(ERR_MEM_ALLOC);
	}
	*ptr = ptr_;
	rets;
}
dfa ER MemDeallocSys(GA ptr) {
	GA ptr_ = ptr;
	SIZE_T size_ = 0;
	ifu (NtFreeVirtualMemory(ProcCurNtHdlGet(), &ptr_, &size_, MEM_RELEASE) != STATUS_SUCCESS) {
		rete(ERR_MEM_ALLOC);
	}
	rets;
}
dfa GA MemAlloc(SI size) {
	ret malloc(size);
}
dfa NT MemDealloc(GA ptr) {
	free(ptr);
}

dfa GA MemResize(GA ptr, SI sizeAlloc, SI sizeCpy) {
	GA r = MemAlloc(sizeAlloc);
	MemCpy(r, ptr, sizeCpy);
	MemDealloc(ptr);
	ret r;
}

dfa GA operator new (UA size) {
	ret MemAlloc(size);
}
dfa GA operator new[](UA size) {
	ret MemAlloc(size);
}
dfa GA operator new (UA size, GA ptr) {
	ret ptr;
	unused(size);
}
dfa GA operator new[](UA size, GA ptr) {
	ret ptr;
	unused(size);
}
dfa NT operator delete (GA ptr) {
	MemDealloc(ptr);
}
dfa NT operator delete[](GA ptr) {
	MemDealloc(ptr);
}
dfa NT operator delete (GA ptr, UA size) {
	unused(size);
	MemDealloc(ptr);
}
dfa NT operator delete[](GA ptr, UA size) {
	unused(size);
	MemDealloc(ptr);
}

class MemPoolTmp {
private:
	U1** m_listPtr;
	SI m_listPtrLen;
	SI m_sizeFree;
	U1* m_curPtr;
private:
	dfa NT _Init() {
		m_listPtr = NUL;
		m_listPtrLen = 0;
		m_sizeFree = 0;
		m_curPtr = NUL;
	}
public:
	dfa GA Alloc(SI size) {
		do {
			ifl (m_sizeFree >= size) {
				m_sizeFree -= size;
				m_curPtr += size;
				ret m_curPtr - size;
			}
			m_sizeFree = 1 << m_listPtrLen;
			m_listPtr[m_listPtrLen] = new U1[m_sizeFree];
			m_curPtr = m_listPtr[m_listPtrLen];
			++m_listPtrLen;
		} while (YES);
	}
	dfa NT DeallocAll() {
		while (m_listPtrLen > 0) {
			delete[] m_listPtr[m_listPtrLen - 1];
			--m_listPtrLen;
		}
		m_sizeFree = 0;
		m_curPtr = NUL;
	}
public:
	dfa MemPoolTmp() {
		tx->_Init();
		m_listPtr = new U1 * [sizb(SI)];
		MemSet(m_listPtr, 0, sizb(SI) * sizeof(U1*));
	}
	dfa ~MemPoolTmp() {
		tx->DeallocAll();
		delete[] m_listPtr;
	}
};
