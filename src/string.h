#pragma once

// pre-defined:
#ifdef PROG_SYS_WIN
dfa TEB_* ThdTeb();
#endif

cxex SI STR_EX_LEN = 1;

#ifdef PROG_SYS_WIN
    #define CsstrSetForm sprintf_s
    #define ChstrSetForm swprintf_s
    #define CsstrGetForm sscanf_s
    #define ChstrGetForm swscanf_s
#else
    #define CsstrSetForm sprintf
    #define ChstrSetForm swprintf
    #define CsstrGetForm sscanf
    #define ChstrGetForm swscanf
#endif

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
tpl1 dfa SI StrCpyStrLen(T1* dst, cx T1* src)
{
    T1* p = dst;
    while (*src != '\0')
        *p++ = *src++;
    *p = '\0';
    ret p - dst;
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

tpl1 dfa SA StrCmpCi(cx T1* strA, cx T1* strB)
{
    SI i = 0;
    while (IsSameCi(strA[i], strB[i]) && strA[i] != '\0' && strB[i] != '\0')
        ++i;
    ret SA(strA[i] - strB[i]);
}
tpl1 dfa SA StrCmpCi(cx T1* strA, cx T1* strB, SI len)
{
    SI i = 0;
    while (i < len && IsSameCi(strA[i], strB[i]) && strA[i] != '\0' && strB[i] != '\0')
        ++i;
    if (i == len)
        ret 0;
    ret SA(strA[i] - strB[i]);
}
tpl1 dfa SI StrEnclose(T1* dst, cx T1* src, cx T1* left, cx T1* right)
{
    cx SI leftLen = StrLen(left);
    MemCpyUpdCur(dst, left, leftLen * siz(T1));
    cx SI srcLen = StrLen(src);
    MemCpyUpdCur(dst, src, srcLen * siz(T1));
    cx SI rightLen = StrLen(right);
    MemCpyUpdCur(dst, right, rightLen * siz(T1));
    *dst = '\0';
    ret leftLen + srcLen + rightLen;
}
tpl1 dfa SI StrReplace(T1* dst, cx T1* str, SI i, SI len)
{
    // insert 'str' at 'i' in 'dst' while overwriting 'len' characters
    i += len;
    SI dstLen = StrLen(dst);
    cx SI strLen = StrLen(str) - len;
    T1* dstP = dst + dstLen;
    if (dstP < dst + i)
    {
        dstLen += dst + i - dstP;
        *(dst + dstLen + strLen) = '\0';
    }
    while (dstP >= dst + i)
    {
        *(dstP + strLen) = *dstP;
        --dstP;
    }
    MemCpy(dst + i - len, str, (strLen + len) * siz(T1));
    ret dstLen + strLen;
}
tpl1 dfa SI StrInsert(T1* dst, cx T1* str, SI i)
{
    // insert 'str' at 'i' in 'dst'
    ret StrReplace(dst, str, i, 0);
}
tpl1 dfa SI StrRemove(T1* dst, SI i, SI cnt)
{
    // remove 'cnt' characters from 'dst' starting at 'i'
    AU dstCur = dst + i;
    do
    {
        *dstCur = *(dstCur + cnt);
        if (*dstCur == '\0')
            break;
        ++dstCur;
    } while (YES);
    ret SI(dstCur - dst);
}
tpl1 dfa SI StrSubChar(T1* dst, cx T1* charList)
{
    cx T1* cx dstBase = dst;
    cx SI charListLen = StrLen(charList);
    cx T1* src = dst;
    while (*src != '\0')
    {
        cx T1* charListP = charList;
        while (*charListP != '\0')
        {
            if (*src == *charListP)
            {
                ++src;
                break;
            }
            ++charListP;
        }
        if (charListP == charList + charListLen)
        {
            *dst = *src;
            ++dst;
            ++src;
        }
    }
    *dst = '\0';
    ret dst - dstBase;
}
tpl1 dfa SI StrTrimWspace(T1* dst)
{
    cx T1* src = dst;
    while (IsWspace(*src))
        ++src;
    T1* dstP = dst;
    while (*src != '\0')
    {
        *dstP = *src;
        ++dstP;
        ++src;
    }
    while (dstP > dst && IsWspace(*(dstP - 1)))
        --dstP;
    *dstP = '\0';
    ret dstP - dst;
}
tpl1 dfa cx T1* StrFind(cx T1* main, cx T1& c)
{
    cx T1* p = main;
    while (*p != '\0')
    {
        if (*p == c)
            ret p;
        ++p;
    }
    ret NUL;
}
tpl1 dfa cx T1* StrFindLast(cx T1* main, cx T1& c)
{
    cx T1* p = main + StrLen(main) - 1;
    while (p >= main)
    {
        if (*p == c)
            ret p;
        --p;
    }
    ret NUL;
}
tpl1 dfa SI StrFindI(cx T1* main, cx T1& c)
{
    cx T1* cx p = StrFind(main, c);
    ret (p == NUL) ? -1 : p - main;
}
tpl1 dfa SI StrFindI(cx T1* main, cx T1* sub)
{
    cx T1* cx p = StrFind(main, sub);
    ret (p == NUL) ? -1 : p - main;
}
tpl1 dfa cx T1* StrFindCi(cx T1* main, cx T1* sub)
{
    cx T1* p = main;
    cx SI subLen = StrLen(sub);
    while (*p != '\0')
    {
        if (StrCmpCi(p, sub, subLen) == 0)
            ret p;
        ++p;
    }
    ret NUL;
}
tpl1 dfa cx T1* StrArgSkip(cx T1* str)
{
    cx T1* p = str;
    if (*p == '\"')
    {
        ++p;
        while (*p != '\0' && *p != '\"')
            ++p;
        if (*p == '\"')
            ++p;
    }
    else
    {
        while (*p != '\0' && *p != ' ')
            ++p;
    }
    while (*p == ' ')
        ++p;
    ret p;
}
tpl1 dfa BO StrHasPre(cx T1* main, cx T1* sub)
{
    cx T1* p1 = main;
    cx T1* p2 = sub;
    while (*p1 != '\0' && *p2 != '\0' && *p1 == *p2)
    {
        ++p1;
        ++p2;
    }
    ret *p2 == '\0';
}

tpl1 dfa SI StrToLowcase(T1* str)
{
    cx T1* cx strBase = str;
    while (*str != '\0')
    {
        *str = ToLowcase<T1>(*str);
        ++str;
    }
    ret SI(str - strBase);
}
tpl1 dfa SI StrToLowcaseHun(T1* str)
{
    cx T1* cx strBase = str;
    while (*str != '\0')
    {
        *str = ToLowcase<T1>(*str);
        // clang-format off
        switch (*str)
        {
        case 0xC1: *str = 0xE1; break;
        case 0xC9: *str = 0xE9; break;
        case 0xCD: *str = 0xED; break;
        case 0xD3: *str = 0xF3; break;
        case 0xD6: *str = 0xF6; break;
        case 0xDA: *str = 0xFA; break;
        case 0xDC: *str = 0xFC; break;
        case 0x150: ifcx (siz(T1) > 1) *str = 0x151; break;
        case 0x170: ifcx (siz(T1) > 1) *str = 0x171; break;
        default: break;
        }
        // clang-format on
        ++str;
    }
    ret SI(str - strBase);
}
tpl1 dfa SI StrToUpcase(T1* str)
{
    cx T1* cx strBase = str;
    while (*str != '\0')
    {
        *str = ToUpcase<T1>(*str);
        ++str;
    }
    ret SI(str - strBase);
}

tpl1 dfa SI StrFindReplace(T1* dst, cx T1* src, cx T1* strReplace, cx T1* strFind)
{
    // replace all 'strFind' with 'strReplace' in 'src' and store in 'dst'
    cx SI strFindLen = StrLen(strFind);
    cx SI strReplaceLen = StrLen(strReplace);
    cx T1* srcP = src;
    T1* dstP = dst;
    while (*srcP != '\0')
    {
        if ((*srcP == *strFind) && StrCmp(srcP, strFind, strFindLen) == 0)
        {
            MemCpyUpdCur(dstP, strReplace, strReplaceLen * siz(T1));
            srcP += strFindLen;
        }
        else
        {
            *dstP = *srcP;
            ++srcP;
            ++dstP;
        }
    }
    *dstP = '\0';
    ret dstP - dst;
}
tpl1 dfa SI StrFindReplaceMulti(T1* dst, cx T1* src, std::span<cx T1*> strReplaceList, std::span<cx T1*> strFindList)
{
    // replace all 'strFindList' elements with corresponding 'strReplaceList' in 'src' and store in 'dst'
    // uses 1 pass in total, index 0 of list has highest priority
    cx AU listLen = Min<SI>(strReplaceList.size(), strFindList.size());
    std::vector<SI> strLens(listLen * 2);
    cx AU strFindLens = strLens.data();
    cx AU strReplaceLens = strLens.data() + listLen;
    ite (i, i < listLen)
    {
        strFindLens[i] = StrLen(strFindList[i]);
        strReplaceLens[i] = StrLen(strReplaceList[i]);
    }
    cx T1* srcP = src;
    T1* dstP = dst;
    while (*srcP != '\0')
    {
        ite (i, i < listLen)
        {
            if ((*srcP == *strFindList[i]) && StrCmp(srcP, strFindList[i], strFindLens[i]) == 0)
            {
                MemCpyUpdCur(dstP, strReplaceList[i], strReplaceLens[i] * siz(T1));
                srcP += strFindLens[i];
                jsrc(jReplaced);
            }
        }
        *dstP = *srcP;
        ++srcP;
        ++dstP;
        jdst(jReplaced);
    }
    *dstP = '\0';
    ret dstP - dst;
}

tpl1 dfa SI StrWordWrap(std::vector<cx T1*>& lineDivs, cx T1* str, SI lineMax, T1 wrapVal = ' ')
{
    lineDivs.clear();
    AU strCur = str;
    AU prevDiv = strCur;
    lineDivs.emplace_back(prevDiv);
    SI longestLine = 0;

    while (*strCur != '\0')
    {
        if (strCur - prevDiv >= lineMax)
        {
            AU strCurSeek = strCur;
            do
                --strCurSeek;
            while (strCurSeek != prevDiv && *strCurSeek != wrapVal);
            if (strCurSeek != prevDiv)
            {
                ++strCurSeek;
                strCur = strCurSeek;
            }

            longestLine = Max(longestLine, strCur - prevDiv);
            prevDiv = strCur;
            lineDivs.emplace_back(prevDiv);
        }
        ++strCur;
    }
    ifl (*str != '\0')
    {
        longestLine = Max(longestLine, strCur - prevDiv);
        prevDiv = strCur;
        lineDivs.emplace_back(prevDiv);
    }

    ret longestLine;
}

tpl<SI N> class StrObfuscated
{
  private:
    std::array<U1, N> m_buf;

  public:
    dfa NT Encode()
    {
        MemObfuscate(m_buf.data(), N * siz(U1));
    }
    dfa NT Decode()
    {
        MemUnobfuscate(m_buf.data(), N * siz(U1));
    }
    dfa NT Clr()
    {
        MemSet0Force(m_buf.data(), N * siz(U1));
    }

  public:
    dfa cx CS* Csstr(BO doDecode = NO)
    {
        if (doDecode)
            tx->Decode();
        ret (cx CS*)m_buf.data();
    }
    dfa cx CH* Chstr(BO doDecode = NO)
    {
        if (doDecode)
            tx->Decode();
        ret (cx CH*)m_buf.data();
    }

  public:
    dfa std::string CodeGen(cx CH* strToEncode) cx
    {
        cx AU strSize = (StrLen(strToEncode) + STR_EX_LEN) * siz(CH);
        std::string code;
        CS intStr[8];
        code += "StrObfuscated<";
        IntToStr(intStr, strSize);
        code += intStr;
        code += "> customName({";
        ite (i, i < strSize)
        {
            IntToStr(intStr, ByteObfuscate(*(((U1*)strToEncode) + i), U1(i)));
            code += intStr;
            ifl (i + 1 < strSize)
                code += ", ";
        }
        code += "});\n";
        ret code;
    }

  public:
    dfa cxex StrObfuscated()
    {
    }
    dfa cxex StrObfuscated(cx std::array<U1, N>& dat) : m_buf(dat)
    {
    }
    dfa cxex ~StrObfuscated()
    {
        tx->Clr();
    }
};

tpl1 class StrArgList
{
  private:
    U1* m_buf;
    SI m_argCnt;
    T1** m_argVal;

  public:
    dfa SI Cnt() cx
    {
        ret m_argCnt;
    }
    dfa cx T1* Val(SI i) cx
    {
        ifu (i >= m_argCnt)
            ret NUL;
        ret m_argVal[i];
    }
    dfa NT Clr()
    {
        if (m_buf == NUL)
            ret;
        MemDel(m_buf);
        m_buf = NUL;
        m_argCnt = 0;
        m_argVal = NUL;
    }
    dfa ER Set(cx T1* argStr)
    {
        tx->Clr();

        SI argCntGuess = 0;
        BO wasSpace = YES;
        AU cur = argStr;
        while (*cur != '\0')
        {
            cx BO isSpace = (*cur++ == ' ');
            argCntGuess += (isSpace ^ wasSpace);
            wasSpace = isSpace;
        }
        argCntGuess = DivCeil(argCntGuess, SI(2));
        cx AU argStrSize = SI(cur - argStr + STR_EX_LEN) * siz(T1);
        cx AU bufStrSize = AlignBit(argStrSize, siz(T1*));
        cx AU bufAdrSize = argCntGuess * siz(T1*);
        m_buf = (U1*)MemNew(bufStrSize + bufAdrSize);
        MemCpy(m_buf, argStr, argStrSize);
        m_argVal = (T1**)(m_buf + bufStrSize);

        m_argCnt = 0;
        wasSpace = YES;
        AU curBuf = (T1*)m_buf;
        BO isInQuote = NO;
        while (*curBuf != '\0')
        {
            BO isSpace = (*curBuf == ' ');
            if (isSpace ^ wasSpace)
            {
                if (wasSpace)
                {
                    cx BO isQuote = (*curBuf == '\"');
                    m_argVal[m_argCnt++] = curBuf + isQuote;
                    isInQuote = isQuote;
                }
                else // isSpace
                {
                    if (isInQuote)
                    {
                        cx BO isQuotePrev = (*(curBuf - 1) == '\"');
                        if (isQuotePrev)
                        {
                            *(curBuf - 1) = '\0';
                            isInQuote = NO;
                        }
                        else
                        {
                            isSpace = NO;
                        }
                    }
                    else
                    {
                        *curBuf = '\0';
                    }
                }
            }
            wasSpace = isSpace;
            ++curBuf;
        }
        if (isInQuote)
            *(curBuf - 1) = '\0';

        rets;
    }

  public:
    dfa StrArgList() : m_buf(NUL), m_argCnt(0), m_argVal(NUL)
    {
    }
    dfa StrArgList(cx T1* argStr) : m_buf(NUL), m_argCnt(0), m_argVal(NUL)
    {
        tx->Set(argStr);
    }
    dfa ~StrArgList()
    {
        tx->Clr();
    }
};

#ifdef PROG_SYS_WIN

class EnvvarCache
{
  private:
    std::vector<CH> m_strBuf;
    std::unordered_map<FNV1A64, cx CH*> m_varCache;

  public:
    dfa ER CacheState()
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
    dfa cx CH* ValGet(FNV1A64 hash) cx
    {
        cx AU& it = m_varCache.find(hash);
        ifu (it == m_varCache.end())
            ret NUL;
        ret it->second;
    }
    dfa cx CH* ValGet(cx CS* name) cx
    {
        FNV1A64 hash = FNV1A64_INIT_VAL;
        while (*name != '\0')
            _HashFnv1a64_1(hash, ToLowcase(*name++));
        ret tx->ValGet(hash);
    }

  public:
    dfa EnvvarCache()
    {
    }
};

#endif
