#pragma once

#include <span>
#include <vector>
#include <array>
#include <unordered_map>
#include "hash.hpp"

cxex SI STR_EX_LEN = 1;

#ifdef PROG_SYS_WIN
    #define CsstrSetForm sprintf_s
    #define ChstrSetForm swprintf_s
    #define CsstrGetForm sscanf_s
    #define ChstrGetForm swscanf_s
#else
    #define CsstrSetForm snprintf
    #define ChstrSetForm swprintf
    #define CsstrGetForm sscanf
    #define ChstrGetForm swscanf
#endif

dfa BO _CmpAsc_FNV1A64L(cx FNV1A64L& a, cx FNV1A64L& b);

dfa SI StrLen(cx CS* str);
dfa SI StrLen(cx CH* str);
dfa SI StrLenx(cx CS* str);
dfa SI StrLenx(cx CH* str);
dfa NT StrCpy(CS* dst, cx CS* src);
dfa NT StrCpy(CH* dst, cx CH* src);
dfa NT StrAdd(CS* dst, cx CS* src);
dfa NT StrAdd(CH* dst, cx CH* src);
dfa SA StrCmp(cx CS* strA, cx CS* strB);
dfa SA StrCmp(cx CH* strA, cx CH* strB);
dfa SA StrCmp(cx CS* strA, cx CS* strB, SI len);
dfa SA StrCmp(cx CH* strA, cx CH* strB, SI len);
dfa cx CS* StrFind(cx CS* main, cx CS* sub);
dfa cx CH* StrFind(cx CH* main, cx CH* sub);
dfa cx CS* StrFindFnv1a64l(cx CS* main, FNV1A64L sub);
dfa cx CS* StrFindAnyFnv1a64l(FNV1A64L& found, cx CS* main, cx std::span<cx FNV1A64L>& subList);
dfa SI CsstrToChstr(CH* dst, cx CS* src);
dfa SI ChstrToCsstr(CS* dst, cx CH* src);
dfa NT CsstrToChstrN(CH* dst, cx CS* src, SI len);
dfa NT ChstrToCsstrN(CS* dst, cx CH* src, SI len);

#ifdef PROG_SYS_WIN

class EnvvarCache
{
  private:
    std::vector<CH> m_strBuf;
    std::unordered_map<FNV1A64, cx CH*, StdHasherNoOpe<FNV1A64>> m_varCache;

  public:
    dfa ER CacheState();
    dfa cx CH* ValGet(FNV1A64 hash) cx;
    dfa cx CH* ValGet(cx CS* name) cx;

  public:
    dfa EnvvarCache();
};

#endif

tpl1 dfa SI StrCpyStrLen(T1* dst, cx T1* src);

tpl1 dfa SA StrCmpCi(cx T1* strA, cx T1* strB);
tpl1 dfa SA StrCmpCi(cx T1* strA, cx T1* strB, SI len);
tpl1 dfa SI StrEnclose(T1* dst, cx T1* src, cx T1* left, cx T1* right);
tpl1 dfa SI StrReplace(T1* dst, cx T1* str, SI i, SI len);
tpl1 dfa SI StrInsert(T1* dst, cx T1* str, SI i);
tpl1 dfa SI StrRemove(T1* dst, SI i, SI cnt);
tpl1 dfa SI StrSubChar(T1* dst, cx T1* charList);
tpl1 dfa SI StrTrimWspace(T1* dst);
tpl1 dfa cx T1* StrFind(cx T1* main, cx T1& c);
tpl1 dfa cx T1* StrFindLast(cx T1* main, cx T1& c);
tpl1 dfa SI StrFindI(cx T1* main, cx T1& c);
tpl1 dfa SI StrFindI(cx T1* main, cx T1* sub);
tpl1 dfa cx T1* StrFindCi(cx T1* main, cx T1* sub);
tpl1 dfa cx T1* StrArgSkip(cx T1* str);
tpl1 dfa BO StrHasPre(cx T1* main, cx T1* sub);

tpl1 dfa SI StrToLowcase(T1* str);
tpl1 dfa SI StrToLowcaseHun(T1* str);
tpl1 dfa SI StrToUpcase(T1* str);

tpl1 dfa SI StrFindReplace(T1* dst, cx T1* src, cx T1* strReplace, cx T1* strFind);
tpl1 dfa SI StrFindReplaceMulti(T1* dst, cx T1* src, std::span<cx T1*> strReplaceList, std::span<cx T1*> strFindList);

tpl1 dfa SI StrWordWrap(std::vector<cx T1*>& lineDivs, cx T1* str, SI lineMax, T1 wrapVal = ' ');

tpl<SI N> class StrObfuscated
{
  private:
    std::array<U1, N> m_buf;

  public:
    dfa NT Encode();
    dfa NT Decode();
    dfa NT Clr();

  public:
    dfa cx CS* Csstr(BO doDecode = NO);
    dfa cx CH* Chstr(BO doDecode = NO);

  public:
    dfa std::string CodeGen(cx CH* strToEncode) cx;

  public:
    dfa cxex StrObfuscated();
    dfa cxex StrObfuscated(cx std::array<U1, N>& dat);
    dfa cxex ~StrObfuscated();
};

tpl1 class StrArgList
{
  private:
    U1* m_buf;
    SI m_argCnt;
    T1** m_argVal;

  public:
    dfa SI Cnt() cx;
    dfa cx T1* Val(SI i) cx;
    dfa NT Clr();
    dfa ER Set(cx T1* argStr);

  public:
    dfa StrArgList();
    dfa StrArgList(cx T1* argStr);
    dfa ~StrArgList();
};

#include "string.ipp"
