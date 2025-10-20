#pragma once

#include "hash.hpp"
#include "string.hpp"

enum class CodeGenHash : FNV1A64
{
    Begin = 0x9A848D5E6672B3DE,   // "begin"
    End = 0xC2F00318F053500A,     // "end"
    Hashkey = 0x92FB1E886EE90AB8, // "hashkey"
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
    U8 hashKey;

    dfa CodeGenCtx();
};

dfa ER _CodeGenBegin1(CodeGenResult& result, CodeGenCtx& ctx);
dfa ER _CodeGenEnd1(CodeGenResult& result, CodeGenCtx& ctx);
dfa ER _CodeGenCmd1(CodeGenResult& result, CodeGenCtx& ctx, cx StrArgList<CS>& argList, CodeGenHash cmdHash);
dfa ER _CodeGenLine1(CodeGenResult& result, CodeGenCtx& ctx, cx std::string& line);

dfa ER CodeGenByFile(std::vector<CodeGenResult>& results, cx CH* path);

#include "codegen.ipp"
