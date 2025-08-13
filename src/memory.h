#pragma once

#ifdef PROG_COMPILER_GCC
    #define RetAdr() GA(__builtin_return_address(0))
#else
    #ifdef PROG_COMPILER_MSVC
        #pragma intrinsic(_ReturnAddress)
        #define RetAdr() GA(_ReturnAddress())
    #else
        #define RetAdr() GA(0)
    #endif
#endif

__declspec(noinline) GA AdrOfNextOpe()
{
    ret RetAdr();
}

#ifdef PROG_SYS_WIN

dfa ER MemDelSys(GA ptr, HD proc = ProcCurHdl())
{
    UA size = 0;
    ifu (NtFreeVirtualMemory_(proc, &ptr, &size, MEM_RELEASE) != STATUS_SUCCESS)
        rete(ErrVal::MEM);
    rets;
}
dfa ER MemNewSys(GA* ptr, SI size, HD proc = ProcCurHdl(), U4 prot = PAGE_READWRITE)
{
    *ptr = NUL;
    GA ptr_ = NUL;
    ifu (NtAllocateVirtualMemory_(proc, &ptr_, 0, (UA*)&size, MEM_RESERVE | MEM_COMMIT, prot) != STATUS_SUCCESS)
        rete(ErrVal::MEM_NEW);
    *ptr = ptr_;
    rets;
}

#endif

dfa NT MemDel(GA ptr)
{
    free(ptr);
}
dfa GA MemNew(SI size)
{
    ret malloc(size);
}

dfa NT MemSet(GA dst, U1 val, SI size)
{
#ifdef PROG_COMPILER_GCC
    __builtin_memset(dst, val, size);
#else
    memset(dst, val, size);
#endif
}
dfa NT MemCpy(GA dst, CXGA src, SI size)
{
#ifdef PROG_COMPILER_GCC
    __builtin_memcpy(dst, src, size);
#else
    memcpy(dst, src, size);
#endif
}
dfa NT MemMove(GA dst, CXGA src, SI size)
{
#ifdef PROG_COMPILER_GCC
    __builtin_memmove(dst, src, size);
#else
    memmove(dst, src, size);
#endif
}
dfa SA MemCmp(CXGA ptr1, CXGA ptr2, SI size)
{
    ret SA(__builtin_memcmp(ptr1, ptr2, size));
}

dfa GA MemResize(GA ptr, SI sizeAlloc, SI sizeCpy)
{
    GA r = MemNew(sizeAlloc);
    MemCpy(r, ptr, sizeCpy);
    MemDel(ptr);
    ret r;
}

dfa NT MemSet0Force(GA dst, SI size)
{
    ifu (size < 1)
        ret;
    __stosb((U1*)dst, 0, size);
    cx volatile U1 tmp = AsType<volatile U1>(((U1*)dst)[size >> 1]);
    unused(tmp);
}
dfa NT MemCpyNocall(GA dst, CXGA src, SI size)
{
    AU dst_ = (U1*)dst;
    AU src_ = (cx U1*)src;
    cx AU end = src_ + size;
    while (src_ != end)
        *dst_++ = *src_++;
}
tpl1 dfa NT MemCpyValBe(T1& dst, CXGA src)
{
    static_assert(IsTypeU<T1> || IsTypeS<T1>, "MemCpyValBe: T1 must be an integer type");
    cxex AU sizT1 = siz(T1);
    static_assert(sizT1 > 0 && sizT1 <= siz(U8), "MemCpyValBe: T1 size must be between 1 and 8 bytes");
    AU p = (cx U1*)src;
    dst = *p++;
    ifcx (sizT1 >= 2)
        dst = (dst << 8) | *p++;
    ifcx (sizT1 >= 3)
        dst = (dst << 8) | *p++;
    ifcx (sizT1 >= 4)
        dst = (dst << 8) | *p++;
    ifcx (sizT1 >= 5)
        dst = (dst << 8) | *p++;
    ifcx (sizT1 >= 6)
        dst = (dst << 8) | *p++;
    ifcx (sizT1 >= 7)
        dst = (dst << 8) | *p++;
    ifcx (sizT1 >= 8)
        dst = (dst << 8) | *p++;
}
tpl1 dfa NT MemCpyValBe(GA dst, T1 src)
{
    static_assert(IsTypeU<T1> || IsTypeS<T1>, "MemCpyValBe: T1 must be an integer type");
    cxex AU sizT1 = siz(T1);
    static_assert(sizT1 > 0 && sizT1 <= siz(U8), "MemCpyValBe: T1 size must be between 1 and 8 bytes");
    AU p = (U1*)dst;
    p += sizT1 - 1;
    *(p--) = src;
    ifcx (sizT1 < 2)
        ret;
    *(p--) = (src >>= 8);
    ifcx (sizT1 < 3)
        ret;
    *(p--) = (src >>= 8);
    *(p--) = (src >>= 8);
    ifcx (sizT1 < 5)
        ret;
    *(p--) = (src >>= 8);
    *(p--) = (src >>= 8);
    *(p--) = (src >>= 8);
    *(p--) = (src >>= 8);
}
tpl1 dfa NT MemCpyUpdCur(T1*& dstCur, CXGA src, SI size)
{
    MemCpy(dstCur, src, size);
    AsType<UA>(dstCur) += size;
}
tpl1 dfa NT MemCpyUpdCurSrc(GA dst, cx T1*& srcCur, SI size)
{
    MemCpy(dst, srcCur, size);
    AsType<UA>(srcCur) += size;
}

#ifdef PROG_SYS_WIN

dfa ER MemCpySys(GA dst, CXGA src, SI size, HD procDst, HD procSrc)
{
    cx AU size_ = UA(size);
    UA sizeResult = 0;
    if (procSrc == ProcCurHdl())
    {
        if (procDst == ProcCurHdl())
        {
            MemCpy(dst, src, size);
            rets;
        }
        else
        {
            ifu ((NtWriteVirtualMemory_(procDst, dst, src, size_, &sizeResult) != STATUS_SUCCESS) || (sizeResult != size_))
                rete(ErrVal::MEM);
            rets;
        }
    }
    else
    {
        if (procDst == ProcCurHdl())
        {
            ifu ((NtReadVirtualMemory_(procSrc, src, dst, size_, &sizeResult) != STATUS_SUCCESS) || (sizeResult != size_))
                rete(ErrVal::MEM);
            rets;
        }
        else
        {
            GA buf = MemNew(size);
            ifu ((NtReadVirtualMemory_(procSrc, src, buf, size_, &sizeResult) != STATUS_SUCCESS) || (sizeResult != size_))
            {
                MemDel(buf);
                rete(ErrVal::MEM);
            }
            ifu ((NtWriteVirtualMemory_(procDst, dst, buf, size_, &sizeResult) != STATUS_SUCCESS) || (sizeResult != size_))
            {
                MemDel(buf);
                rete(ErrVal::MEM);
            }
            MemDel(buf);
            rets;
        }
    }
}

dfa SI MemPageSize()
{
    SYSTEM_INFO info = {};
    info.dwPageSize = 0;
    GetSystemInfo(&info);
    ret SI(info.dwPageSize);
}

#endif

dfa NT MemObfuscate(GA dst, CXGA src, SI size)
{
    U1* dst_ = (U1*)dst;
    cx U1* src_ = (cx U1*)src;
    ite (i, i < size)
        dst_[i] = ByteObfuscate(src_[i], U1(i));
}
dfa NT MemUnobfuscate(GA dst, CXGA src, SI size)
{
    U1* dst_ = (U1*)dst;
    cx U1* src_ = (cx U1*)src;
    ite (i, i < size)
        dst_[i] = ByteUnobfuscate(src_[i], U1(i));
}
dfa NT MemObfuscate(GA dst, SI size)
{
    U1* dst_ = (U1*)dst;
    ite (i, i < size)
        dst_[i] = ByteObfuscate(dst_[i], U1(i));
}
dfa NT MemUnobfuscate(GA dst, SI size)
{
    U1* dst_ = (U1*)dst;
    ite (i, i < size)
        dst_[i] = ByteUnobfuscate(dst_[i], U1(i));
}

GA operator new(size_t size)
{
    ret MemNew(size);
}
GA operator new[](size_t size)
{
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
NT operator delete(GA ptr) noex
{
    MemDel(ptr);
}
NT operator delete[](GA ptr) noex
{
    MemDel(ptr);
}
NT operator delete(GA ptr, size_t size) noex
{
    unused(size);
    MemDel(ptr);
}
NT operator delete[](GA ptr, size_t size) noex
{
    unused(size);
    MemDel(ptr);
}

class MemPoolTmp
{
  private:
    U1** m_listPtr;
    SI m_listPtrLen;
    SI m_sizeFree;
    U1* m_curPtr;

  private:
    dfa NT Init()
    {
        m_listPtr = NUL;
        m_listPtrLen = 0;
        m_sizeFree = 0;
        m_curPtr = NUL;
    }

  public:
    dfa GA New(SI size)
    {
        do
        {
            ifl (m_sizeFree >= size)
            {
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
    dfa NT DelAll()
    {
        while (m_listPtrLen > 0)
        {
            delete[] m_listPtr[m_listPtrLen - 1];
            --m_listPtrLen;
        }
        m_sizeFree = 0;
        m_curPtr = NUL;
    }

  public:
    dfa MemPoolTmp()
    {
        tx->Init();
        m_listPtr = new U1*[sizb(SI)];
        MemSet(m_listPtr, 0, sizb(SI) * siz(U1*));
    }
    dfa ~MemPoolTmp()
    {
        tx->DelAll();
        delete[] m_listPtr;
    }
};
