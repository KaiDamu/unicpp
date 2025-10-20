#include "memory.hpp"

tpl1 dfa NT MemFreeAt(T1* obj) noex
{
    ifcx (!std::is_trivially_destructible_v<T1>)
        obj->~T1();
}
tpl<typename T1, typename... TArgs> dfa T1* MemInitAt(T1* obj, TArgs&&... args)
{
    ret ::new (obj) T1(std::forward<TArgs>(args)...);
}

tpl1 dfa NT MemDelNoInit(GA ptr) noex
{
    ::operator delete[](ptr, std::align_val_t(alignof(T1)));
}
tpl1 dfa T1* MemNewNoInit(SI cnt)
{
    ret (T1*)(::operator new[](cnt * siz(T1), std::align_val_t(alignof(T1))));
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
    *(p--) = U1(src);
    ifcx (sizT1 < 2)
        ret;
    *(p--) = U1(src >>= 8);
    ifcx (sizT1 < 3)
        ret;
    *(p--) = U1(src >>= 8);
    *(p--) = U1(src >>= 8);
    ifcx (sizT1 < 5)
        ret;
    *(p--) = U1(src >>= 8);
    *(p--) = U1(src >>= 8);
    *(p--) = U1(src >>= 8);
    *(p--) = U1(src >>= 8);
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
