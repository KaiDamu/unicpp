#include "string.hpp"

#include "memory.hpp"
#include "sort.hpp"
#include "thread.hpp"

dfa BO _CmpAsc_FNV1A64L(cx FNV1A64L& a, cx FNV1A64L& b)
{
    ret Fnv1a64lLen(a) < Fnv1a64lLen(b);
}

dfa SI StrLen(cx CS* str)
{
    ret __builtin_strlen(str);
}
dfa SI StrLen(cx CH* str)
{
    ret wcslen(str);
}
dfa SI StrLenx(cx CS* str)
{
    ret StrLen(str) + STR_EX_LEN;
}
dfa SI StrLenx(cx CH* str)
{
    ret StrLen(str) + STR_EX_LEN;
}
dfa NT StrCpy(CS* dst, cx CS* src)
{
#ifdef PROG_COMPILER_GCC
    __builtin_strcpy(dst, src);
#else
    strcpy(dst, src);
#endif
}
dfa NT StrCpy(CH* dst, cx CH* src)
{
    wcscpy(dst, src);
}
dfa NT StrAdd(CS* dst, cx CS* src)
{
#ifdef PROG_COMPILER_GCC
    __builtin_strcat(dst, src);
#else
    strcat(dst, src);
#endif
}
dfa NT StrAdd(CH* dst, cx CH* src)
{
    wcscat(dst, src);
}
dfa SA StrCmp(cx CS* strA, cx CS* strB)
{
#ifdef PROG_COMPILER_GCC
    ret SA(__builtin_strcmp(strA, strB));
#else
    ret SA(strcmp(strA, strB));
#endif
}
dfa SA StrCmp(cx CH* strA, cx CH* strB)
{
    ret SA(wcscmp(strA, strB));
}
dfa SA StrCmp(cx CS* strA, cx CS* strB, SI len)
{
#ifdef PROG_COMPILER_GCC
    ret SA(__builtin_strncmp(strA, strB, len));
#else
    ret SA(strncmp(strA, strB, len));
#endif
}
dfa SA StrCmp(cx CH* strA, cx CH* strB, SI len)
{
    ret SA(wcsncmp(strA, strB, len));
}
dfa cx CS* StrFind(cx CS* main, cx CS* sub)
{
#ifdef PROG_COMPILER_GCC
    ret __builtin_strstr(main, sub);
#else
    ret strstr(main, sub);
#endif
}
dfa cx CH* StrFind(cx CH* main, cx CH* sub)
{
    ret wcsstr(main, sub);
}
dfa cx CS* StrFindFnv1a64l(cx CS* main, FNV1A64L sub)
{
    cx AU mainLen = StrLen(main);
    cx AU subLen = Fnv1a64lLen(sub);
    cx AU loopCnt = mainLen - subLen + SI(1);
    ite (i, i < loopCnt)
        if (HashFnv1a64l(main + i, subLen * siz(CS)) == sub)
            ret main + i;
    ret NUL;
}
dfa cx CS* StrFindAnyFnv1a64l(FNV1A64L& found, cx CS* main, cx std::span<cx FNV1A64L>& subList)
{
    found = FNV1A64L(0);
    ifu (subList.size() == 0)
        ret NUL;
    cx AU mainLen = StrLen(main);
    std::vector<FNV1A64L> subList_(subList.size());
    MemCpy(subList_.data(), subList.data(), subList.size() * siz(FNV1A64L));
    SortQuick<FNV1A64L, _CmpAsc_FNV1A64L>(subList_.data(), subList_.size());
    cx AU loopCnt = mainLen - Fnv1a64lLen(subList_[0]) + SI(1);
    ite (i, i < loopCnt)
    {
        FNV1A64 hash = FNV1A64_INIT_VAL;
        SI subLenPrev = 0;
        ite (j, j < SI(subList_.size()))
        {
            cx AU subLen = Fnv1a64lLen(subList_[j]);
            if (subLen > mainLen - i)
                break;
            cx AU subLenDiff = subLen - subLenPrev;
            hash = HashFnv1a64(main + i + subLenPrev, subLenDiff * siz(CS), hash);
            cx AU hashL = Fnv1a64ToFnv1a64l(hash, subLen);
            if (hashL == subList_[j])
            {
                found = subList_[j];
                ret main + i;
            }
            subLenPrev = subLen;
        }
    }
    ret NUL;
}
dfa SI CsstrToChstr(CH* dst, cx CS* src)
{
    cx CH* cx dstBase = dst;
    while (*src != '\0')
        *dst++ = *src++;
    *dst = '\0';
    ret SI(dst - dstBase);
}
dfa SI ChstrToCsstr(CS* dst, cx CH* src)
{
    cx CS* cx dstBase = dst;
    while (*src != '\0')
    {
        if (*src > CH(CS_VAL_MAX))
            *dst = CH_NA;
        else
            *dst = CS(*src);
        ++dst;
        ++src;
    }
    *dst = '\0';
    ret SI(dst - dstBase);
}
dfa NT CsstrToChstrN(CH* dst, cx CS* src, SI len)
{
    cx AU end = src + len;
    while (src != end)
        *dst++ = *src++;
    *dst = '\0';
}
dfa NT ChstrToCsstrN(CS* dst, cx CH* src, SI len)
{
    cx AU end = src + len;
    while (src != end)
    {
        if (*src > CH(CS_VAL_MAX))
            *dst = CH_NA;
        else
            *dst = CS(*src);
        ++dst;
        ++src;
    }
    *dst = '\0';
}

#ifdef PROG_SYS_WIN

dfa ER EnvvarCache::CacheState()
{
    m_strBuf.clear();
    m_varCache.clear();
    cx AU envBase = (cx CH*)(ThdTeb()->ProcessEnvironmentBlock->ProcessParameters->Environment);
    AU envCur = envBase;
    while (*envCur != '\0')
        envCur += StrLen(envCur) + STR_EX_LEN;
    cx AU envEnd = envCur;
    cx AU envLenx = SI(envEnd - envBase);
    m_strBuf.resize(envLenx);
    cx AU bufCurBase = m_strBuf.data();
    AU bufCur = bufCurBase;
    envCur = envBase;
    while (envCur != envEnd)
    {
        FNV1A64 hash = FNV1A64_INIT_VAL;
        while (*envCur != '=')
            _HashFnv1a64_1(hash, ToLowcase(U1(*envCur++)));
        ++envCur;
        cx AU ofsLenx = StrCpyStrLen(bufCur, envCur) + STR_EX_LEN;
        m_varCache[hash] = bufCur;
        bufCur += ofsLenx;
        envCur += ofsLenx;
    }
    m_strBuf.resize(bufCur - bufCurBase);
    cx AU bufCurBaseDiff = SI(m_strBuf.data() - bufCurBase);
    if (bufCurBaseDiff != 0)
        for (AU& it : m_varCache)
            it.second += bufCurBaseDiff;
    rets;
}
dfa cx CH* EnvvarCache::ValGet(FNV1A64 hash) cx
{
    cx AU& it = m_varCache.find(hash);
    ifu (it == m_varCache.end())
        ret NUL;
    ret it->second;
}
dfa cx CH* EnvvarCache::ValGet(cx CS* name) cx
{
    FNV1A64 hash = FNV1A64_INIT_VAL;
    while (*name != '\0')
        _HashFnv1a64_1(hash, ToLowcase(*name++));
    ret tx->ValGet(hash);
}
dfa EnvvarCache::EnvvarCache()
{
}

#endif
