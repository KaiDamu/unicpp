#pragma once

#include <string>
#include "type.hpp"

using FNV1A64 = U8;
using FNV1A64L = U8;

cxex FNV1A64 FNV1A64_INIT_VAL = 0xCBF29CE484222325ULL;

dfa NT _HashFnv1a64_1(FNV1A64& hash, U1 val);
dfa NT _HashFnv1a64_2(FNV1A64& hash, SI size);

dfa FNV1A64L Fnv1a64ToFnv1a64l(FNV1A64 in, SI len);
dfa SI Fnv1a64lLen(FNV1A64L hash);

dfa FNV1A64 HashFnv1a64(CXGA buf, SI size, FNV1A64 init = FNV1A64_INIT_VAL);
dfa FNV1A64L HashFnv1a64l(CXGA buf, SI size, FNV1A64L init = FNV1A64_INIT_VAL);
dfa FNV1A64 HashFnv1a64Str(cx CS* str, FNV1A64 init = FNV1A64_INIT_VAL);
dfa FNV1A64L HashFnv1a64lStr(cx CS* str, FNV1A64L init = FNV1A64_INIT_VAL);

dfa std::string _HashFnv1a64StrCh(cx CH* str, FNV1A64 init = FNV1A64_INIT_VAL);
dfa std::string _HashFnv1a64lStrCh(cx CH* str, FNV1A64L init = FNV1A64_INIT_VAL);

tpl1 struct StdHasherNoOpe
{
    dfa size_t operator()(cx T1& val) cx noex;
};

tpl<typename T1, U1 M> dfa T1 _HashFnv1a64(CXGA buf, SI size, FNV1A64 init);
tpl<typename T1, U1 M> dfa T1 _HashFnv1a64Str(cx CS* str, FNV1A64 init);

#include "hash.ipp"
