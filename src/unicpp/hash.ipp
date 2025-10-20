#include "hash.hpp"

tpl1 dfa size_t StdHasherNoOpe<T1>::operator()(cx T1& val) cx noex
{
    ret size_t(val);
}

tpl<typename T1, U1 M> dfa T1 _HashFnv1a64(CXGA buf, SI size, FNV1A64 init)
{
    AU cur = (cx U1*)buf;
    cx AU end = cur + size;
    FNV1A64& hash = init;
    while (cur != end)
        _HashFnv1a64_1(hash, *cur++);
    ifcx (M == 2)
        _HashFnv1a64_2(hash, size);
    ret T1(hash);
}
tpl<typename T1, U1 M> dfa T1 _HashFnv1a64Str(cx CS* str, FNV1A64 init)
{
    AU cur = (cx U1*)str;
    FNV1A64& hash = init;
    while (*cur != '\0')
        _HashFnv1a64_1(hash, *cur++);
    ifcx (M == 2)
        _HashFnv1a64_2(hash, cur - (cx U1*)str);
    ret T1(hash);
}
