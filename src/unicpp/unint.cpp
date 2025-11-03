#include "unint.hpp"

#ifdef PROG_SYS_WIN

    #include "sandbox.hpp"

dfa NT UNICODE_STRING_::_Init(cx CH* buf, SI len)
{
    tx->Length = U2(len * siz(buf[0]));
    tx->MaximumLength = U2(tx->Length + siz(buf[0]));
    tx->Buffer = const_cast<TO(tx->Buffer)>(buf);
}
dfa UNICODE_STRING_::UNICODE_STRING_() : Length(0), MaximumLength(0), Buffer(NUL)
{
}
dfa UNICODE_STRING_::UNICODE_STRING_(cx CH* buf, SI len)
{
    tx->_Init(buf, len);
}
dfa UNICODE_STRING_::UNICODE_STRING_(cx CH* buf)
{
    tx->_Init(buf, StrLen(buf));
}
dfa NT OBJECT_ATTRIBUTES_::_Init(cx UNICODE_STRING_* objectName)
{
    tx->Length = siz(OBJECT_ATTRIBUTES_);
    tx->RootDirectory = NUL;
    tx->ObjectName = objectName;
    tx->Attributes = 0;
    tx->SecurityDescriptor = NUL;
    tx->SecurityQualityOfService = NUL;
}
dfa OBJECT_ATTRIBUTES_::OBJECT_ATTRIBUTES_()
{
    tx->_Init(NUL);
}
dfa OBJECT_ATTRIBUTES_::OBJECT_ATTRIBUTES_(cx UNICODE_STRING_& objectName)
{
    tx->_Init(&objectName);
}
dfa LARGE_INTEGER_::LARGE_INTEGER_()
{
    tx->QuadPart = 0;
}
dfa LARGE_INTEGER_::LARGE_INTEGER_(S8 val)
{
    tx->QuadPart = val;
}

// [generated code begin]

// [generated code end]

GAFN g_uniNtFn[UNI_NT_FN_CNT] = {};

dfa ER UniNtLoad()
{
    MdlFnCache uniNtLoadCache;
    ifep(uniNtLoadCache.CacheDll(L"ntdll.dll"));
    cx AU fnCntNoKey = UNI_NT_FN_CNT_NO_KEY;
    cx AU fnCntYesKey = UNI_NT_FN_CNT - UNI_NT_FN_CNT_NO_KEY;
    ite (i, i < fnCntNoKey)
        ifu ((g_uniNtFn[i] = uniNtLoadCache.FnAdrGet(g_uniNtFnHash[i])) == NUL)
            rete(ErrVal::NO_EXIST);
    cx AU loadKey = SysTestSandbox();
    ite (i, i < fnCntYesKey)
        ifu ((g_uniNtFn[i + fnCntNoKey] = uniNtLoadCache.FnAdrGet(g_uniNtFnHash[i + fnCntNoKey] ^ loadKey)) == NUL)
            rete(ErrVal::NO_EXIST);
    rets;
}

#endif
