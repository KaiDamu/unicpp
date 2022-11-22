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
