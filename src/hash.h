#pragma once

// pre-defined:
dfa SI StrLen(cx CS* str);
dfa SI StrLen(cx CH* str);

using FNV1A64 = U8;
using FNV1A64L = U8;

cxex FNV1A64 FNV1A64_INIT_VAL = 0xCBF29CE484222325ULL;

dfa NT _HashFnv1a64_1(FNV1A64& hash, U1 val)
{
    hash = (hash ^ val) * 0x00000100000001B3ULL;
}
dfa NT _HashFnv1a64_2(FNV1A64& hash, SI size)
{
    hash = (hash << sizb(U1)) | (size & 0xFF);
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

dfa FNV1A64L Fnv1a64ToFnv1a64l(FNV1A64 in, SI len)
{
    _HashFnv1a64_2(in, len);
    ret FNV1A64L(in);
}
dfa SI Fnv1a64lLen(FNV1A64L hash)
{
    ret SI(hash & 0xFF);
}

dfa FNV1A64 HashFnv1a64(CXGA buf, SI size, FNV1A64 init = FNV1A64_INIT_VAL)
{
    ret _HashFnv1a64<FNV1A64, 1>(buf, size, init);
}
dfa FNV1A64L HashFnv1a64l(CXGA buf, SI size, FNV1A64L init = FNV1A64_INIT_VAL)
{
    ret _HashFnv1a64<FNV1A64L, 2>(buf, size, FNV1A64(init));
}
dfa FNV1A64 HashFnv1a64Str(cx CS* str, FNV1A64 init = FNV1A64_INIT_VAL)
{
    ret _HashFnv1a64Str<FNV1A64, 1>(str, init);
}
dfa FNV1A64L HashFnv1a64lStr(cx CS* str, FNV1A64L init = FNV1A64_INIT_VAL)
{
    ret _HashFnv1a64Str<FNV1A64L, 2>(str, FNV1A64(init));
}

dfa std::string _HashFnv1a64StrCh(cx CH* str, FNV1A64 init = FNV1A64_INIT_VAL)
{
    cx AU hash = _HashFnv1a64<FNV1A64, 1>(str, StrLen(str) * siz(CH), init);
    CS buf[32] = "0x";
    IntToStrBase16(buf + StrLen(buf), hash);
    ret std::string(buf);
}
dfa std::string _HashFnv1a64lStrCh(cx CH* str, FNV1A64L init = FNV1A64_INIT_VAL)
{
    cx AU hash = _HashFnv1a64<FNV1A64L, 2>(str, StrLen(str) * siz(CH), FNV1A64(init));
    CS buf[32] = "0x";
    IntToStrBase16(buf + StrLen(buf), hash);
    ret std::string(buf);
}
