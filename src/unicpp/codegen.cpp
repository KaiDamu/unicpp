#include "codegen.hpp"

#include "memory.hpp"
#include "file.hpp"

dfa CodeGenCtx::CodeGenCtx() : opeId(CodeGenOpeId::None), fnCnt(0), hashKey(0)
{
}

dfa ER _CodeGenBegin1(CodeGenResult& result, CodeGenCtx& ctx)
{
    ctx.fnCnt = 0;
    ctx.hashKey = 0;
    result.code[2] += "cxex cx FNV1A64 g_uniNtFnHash[UNI_NT_FN_CNT] = {";
    rets;
}
dfa ER _CodeGenEnd1(CodeGenResult& result, CodeGenCtx& ctx)
{
    result.code[2] += "};\r\n";
    CS globalStr[256];
    CsstrSetForm(globalStr, 256, "cxex SI UNI_NT_FN_CNT = %d;\r\n\r\nGAFN g_uniNtFn[UNI_NT_FN_CNT] = {};\r\n", int(ctx.fnCnt));
    result.code[0] += globalStr;
    rets;
}
dfa ER _CodeGenCmd1(CodeGenResult& result, CodeGenCtx& ctx, cx StrArgList<CS>& argList, CodeGenHash cmdHash)
{
    switch (cmdHash)
    {
    case CodeGenHash::Hashkey: {
        ifu (argList.Cnt() != 2)
            rete(ErrVal::NO_VALID);
        ctx.hashKey = StrToInt<U8>(argList.Val(1));
        break;
    }
    default: {
        rete(ErrVal::NO_VALID);
    }
    }
    rets;
}
dfa ER _CodeGenLine1(CodeGenResult& result, CodeGenCtx& ctx, cx std::string& line)
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
    AU hash = HashFnv1a64Str(hashStr.data());
    hash ^= ctx.hashKey;
    CS hashAsStr[32] = "0x";
    IntToStrBase16(hashAsStr + 2, hash);
    if (ctx.fnCnt > 0)
        result.code[2] += ", ";
    result.code[2] += hashAsStr;
    CS defineStr[256];
    CsstrSetForm(defineStr, 256, "#define %s_ reinterpret_cast<%s_T>(g_uniNtFn[%d])\r\n", hashStrCur, hashStrCur, int(ctx.fnCnt));
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
    ifep(file.Open(path, YES));
    std::string line;
    while (file.ReadLine(line) > 0)
    {
        if ((line.size() >= _CODE_GEN_PREFIX_LEN) && (MemCmp(line.c_str(), _CODE_GEN_PREFIX, _CODE_GEN_PREFIX_LEN) == 0))
        {
            cx AU cmd = line.c_str() + _CODE_GEN_PREFIX_LEN;
            StrArgList<CS> argList;
            ifep(argList.Set(cmd));
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
                    ifep(_CodeGenBegin1(*resultCur, ctx));
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
                    ifep(_CodeGenEnd1(*resultCur, ctx));
                    break;
                default:
                    break;
                }
                resultCur = NUL;
                break;
            }
            default: {
                ifu (ctx.opeId == CodeGenOpeId::None)
                    rete(ErrVal::NO_VALID);
                switch (ctx.opeId)
                {
                case CodeGenOpeId::UniNtFn:
                    ifep(_CodeGenCmd1(*resultCur, ctx, argList, argVal0Hash));
                    break;
                default:
                    break;
                }
                break;
            }
            }
        }
        else if (ctx.opeId != CodeGenOpeId::None)
        {
            switch (ctx.opeId)
            {
            case CodeGenOpeId::UniNtFn:
                ifep(_CodeGenLine1(*resultCur, ctx, line));
                break;
            default:
                rete(ErrVal::NO_VALID);
            }
        }
    }
    ifep(file.Close(NO));
    rets;
}
