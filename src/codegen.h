#pragma once

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

enum class CodeGenHash : FNV1A64
{
    Begin = 0x9A848D5E6672B3DE, // "begin"
    End = 0xC2F00318F053500A,   // "end"
};

enum class CodeGenOpeId : U4
{
    None = 0,
    UniNtFn = 1,
};

cxex cx CS* _CODE_GEN_PREFIX = "// code-gen ";
cxex SI _CODE_GEN_PREFIX_LEN = 12;

struct CodeGenResult
{
    std::string code[3];
};

struct CodeGenCtx
{
    CodeGenOpeId opeId;
    SI fnCnt;

    dfa CodeGenCtx() : opeId(CodeGenOpeId::None), fnCnt(0)
    {
    }
};

dfa ER _CodeGenBegin1(CodeGenResult& result, CodeGenCtx& ctx)
{
    result.code[2] += "cxex cx FNV1A64 g_uniNtFnHash[UNI_NT_FN_CNT] = {";
    rets;
}
dfa ER _CodeGenEnd1(CodeGenResult& result, CodeGenCtx& ctx)
{
    result.code[2] += "};\r\n";
    CS globalStr[256];
    CsstrSetForm(globalStr, "cxex SI UNI_NT_FN_CNT = %d;\r\n\r\nGAFN g_uniNtFn[UNI_NT_FN_CNT] = {};\r\n", ctx.fnCnt);
    result.code[0] += globalStr;
    rets;
}
dfa ER _CodeGenLine1(std::string& line, CodeGenResult& result, CodeGenCtx& ctx)
{
    cx AU prefix = "using _MDL_";
    cx AU prefixLen = StrLen(prefix);
    ifu ((SI(line.size()) < prefixLen) || (MemCmp(line.c_str(), prefix, prefixLen) != 0))
        rete(ErrVal::NO_VALID);
    cx AU hashStrBegin = line.c_str() + prefixLen;
    cx AU hashStrEnd = StrFind(hashStrBegin, "_T = ");
    ifu (hashStrEnd == NUL)
        rete(ErrVal::NO_VALID);
    std::vector<CS> hashStr(hashStrEnd - hashStrBegin + STR_EX_LEN);
    ifu (hashStr.data() == NUL)
        rete(ErrVal::MEM);
    MemCpy(hashStr.data(), hashStrBegin, hashStr.size() * siz(CS));
    hashStr.back() = '\0';
    AU hashStrCur = hashStr.data();
    while (*hashStrCur != ' ')
    {
        *hashStrCur = ToLowcase(*hashStrCur);
        *hashStrCur = (*hashStrCur == '_') ? '.' : *hashStrCur;
        ++hashStrCur;
    }
    *hashStrCur++ = '!';
    cx AU hash = HashFnv1a64Str(hashStr.data());
    CS hashAsStr[32] = "0x";
    IntToStrBase16(hashAsStr + 2, hash);
    if (ctx.fnCnt > 0)
        result.code[2] += ", ";
    result.code[2] += hashAsStr;
    CS defineStr[256];
    CsstrSetForm(defineStr, "#define %s_ ((%s_T)g_uniNtFn[%d])\r\n", hashStrCur, hashStrCur, ctx.fnCnt);
    result.code[1] += defineStr;
    ++ctx.fnCnt;
    rets;
}

dfa ER CodeGenByFile(std::vector<CodeGenResult>& results, cx CH* path)
{
    results.clear();
    CodeGenResult* resultCur = NUL;
    CodeGenCtx ctx;
    MemFile file;
    ife (file.Open(path, YES))
        retep;
    std::string line;
    while (file.ReadLine(line) > 0)
    {
        if ((line.size() >= _CODE_GEN_PREFIX_LEN) && (MemCmp(line.c_str(), _CODE_GEN_PREFIX, _CODE_GEN_PREFIX_LEN) == 0))
        {
            cx AU cmd = line.c_str() + _CODE_GEN_PREFIX_LEN;
            StrArgList<CS> argList;
            ife (argList.Set(cmd))
                retep;
            ifu (argList.Cnt() < 1)
                rete(ErrVal::NO_VALID);
            cx AU argVal0Hash = CodeGenHash(HashFnv1a64Str(argList.Val(0)));
            switch (argVal0Hash)
            {
            case CodeGenHash::Begin: {
                ifu (argList.Cnt() < 2)
                    rete(ErrVal::NO_VALID);
                cx AU opeId = CodeGenOpeId(StrToInt<U4>(argList.Val(1)));
                ifu (opeId == CodeGenOpeId::None || ctx.opeId != CodeGenOpeId::None)
                    rete(ErrVal::NO_VALID);
                ctx.opeId = opeId;
                results.emplace_back();
                resultCur = &results.back();
                switch (opeId)
                {
                case CodeGenOpeId::UniNtFn:
                    ife (_CodeGenBegin1(*resultCur, ctx))
                        retep;
                    break;
                default:
                    break;
                }
                break;
            }
            case CodeGenHash::End: {
                ifu (argList.Cnt() < 2)
                    rete(ErrVal::NO_VALID);
                cx AU opeId = CodeGenOpeId(StrToInt<U4>(argList.Val(1)));
                ifu (opeId == CodeGenOpeId::None || ctx.opeId != opeId)
                    rete(ErrVal::NO_VALID);
                ctx.opeId = CodeGenOpeId::None;
                switch (opeId)
                {
                case CodeGenOpeId::UniNtFn:
                    ife (_CodeGenEnd1(*resultCur, ctx))
                        retep;
                    break;
                default:
                    break;
                }
                resultCur = NUL;
                break;
            }
            default: {
                rete(ErrVal::NO_VALID);
            }
            }
        }
        else if (ctx.opeId != CodeGenOpeId::None)
        {
            switch (ctx.opeId)
            {
            case CodeGenOpeId::UniNtFn:
                ife (_CodeGenLine1(line, *resultCur, ctx))
                    retep;
                break;
            default:
                rete(ErrVal::NO_VALID);
            }
        }
    }
    ife (file.Close(NO))
        retep;
    rets;
}
