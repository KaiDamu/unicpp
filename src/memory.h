#pragma once

dfa ER MemDelSys(GA ptr) {
	GA ptr_ = ptr;
	SIZE_T size_ = 0;
	ifu (NtFreeVirtualMemory(_ProcHdlGetNt(), &ptr_, &size_, MEM_RELEASE) != STATUS_SUCCESS) {
		rete(ERR_MEM);
	}
	rets;
}
dfa ER MemNewSys(GA* ptr, SI size) {
	GA ptr_ = NUL;
	SIZE_T size_ = size;
	ifu (NtAllocateVirtualMemory(_ProcHdlGetNt(), &ptr_, 0, &size_, MEM_COMMIT, PAGE_READWRITE) != STATUS_SUCCESS) {
		*ptr = NUL;
		rete(ERR_MEM_NEW);
	}
	*ptr = ptr_;
	rets;
}
dfa NT MemDel(GA ptr) {
	free(ptr);
}
dfa GA MemNew(SI size) {
	ret malloc(size);
}

dfa NT MemSetVal(GA dst, U1 val, SI size) {
	#ifdef PROG_COMPILER_GCC
		__builtin_memset(dst, val, size);
	#else
		memset(dst, val, size);
	#endif
}
dfa NT MemSet(GA dst, CXGA src, SI size) {
	#ifdef PROG_COMPILER_GCC
		__builtin_memcpy(dst, src, size);
	#else
		memcpy(dst, src, size);
	#endif
}
dfa SA MemCmp(CXGA ptr1, CXGA ptr2, SI size) {
	ret SA(__builtin_memcmp(ptr1, ptr2, size));
}

dfa GA MemResize(GA ptr, SI sizeAlloc, SI sizeCpy) {
	GA r = MemNew(sizeAlloc);
	MemSet(r, ptr, sizeCpy);
	MemDel(ptr);
	ret r;
}

dfa SI MemPageSize() {
	SYSTEM_INFO info = {};
	info.dwPageSize = 0;
	GetSystemInfo(&info);
	ret SI(info.dwPageSize);
}

dfa NT MemObfuscate(GA dst, CXGA src, SI size) {
	U1* dst_ = (U1*)dst;
	cx U1* src_ = (cx U1*)src;
	ite (i, i < size) dst_[i] = ByteObfuscate(src_[i], U1(i));
}
dfa NT MemUnobfuscate(GA dst, CXGA src, SI size) {
	U1* dst_ = (U1*)dst;
	cx U1* src_ = (cx U1*)src;
	ite (i, i < size) dst_[i] = ByteUnobfuscate(src_[i], U1(i));
}
dfa NT MemObfuscate(GA dst, SI size) {
	U1* dst_ = (U1*)dst;
	ite (i, i < size) dst_[i] = ByteObfuscate(dst_[i], U1(i));
}
dfa NT MemUnobfuscate(GA dst, SI size) {
	U1* dst_ = (U1*)dst;
	ite (i, i < size) dst_[i] = ByteUnobfuscate(dst_[i], U1(i));
}

GA operator new (size_t size) {
	ret MemNew(size);
}
GA operator new[](size_t size) {
	ret MemNew(size);
}
/*
	GA operator new (size_t size, GA ptr) {
		ret ptr;
		unused(size);
	}
	GA operator new[](size_t size, GA ptr) {
		ret ptr;
		unused(size);
	}
*/
NT operator delete (GA ptr) noexcept {
	MemDel(ptr);
}
NT operator delete[](GA ptr) noexcept {
	MemDel(ptr);
}
NT operator delete (GA ptr, size_t size) noexcept {
	unused(size);
	MemDel(ptr);
}
NT operator delete[](GA ptr, size_t size) noexcept {
	unused(size);
	MemDel(ptr);
}

class MemPoolTmp {
private:
	U1** m_listPtr;
	SI m_listPtrLen;
	SI m_sizeFree;
	U1* m_curPtr;
private:
	dfa NT Init() {
		m_listPtr = NUL;
		m_listPtrLen = 0;
		m_sizeFree = 0;
		m_curPtr = NUL;
	}
public:
	dfa GA New(SI size) {
		do {
			ifl (m_sizeFree >= size) {
				m_sizeFree -= size;
				m_curPtr += size;
				ret m_curPtr - size;
			}
			m_sizeFree = SI(1) << m_listPtrLen;
			m_listPtr[m_listPtrLen] = new U1[m_sizeFree];
			m_curPtr = m_listPtr[m_listPtrLen];
			++m_listPtrLen;
		} while (YES);
	}
	dfa NT DelAll() {
		while (m_listPtrLen > 0) {
			delete[] m_listPtr[m_listPtrLen - 1];
			--m_listPtrLen;
		}
		m_sizeFree = 0;
		m_curPtr = NUL;
	}
public:
	dfa MemPoolTmp() {
		tx->Init();
		m_listPtr = new U1*[sizb(SI)];
		MemSetVal(m_listPtr, 0, sizb(SI) * siz(U1*));
	}
	dfa ~MemPoolTmp() {
		tx->DelAll();
		delete[] m_listPtr;
	}
};
