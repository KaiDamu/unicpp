#include "typeconvert.hpp"

dfa S4 F4ToS4Fast(F4 val)
{
    S4 r;
#ifdef PROG_COMPILER_GCC
    __asm__("fistpl %0" : "=m"(r) : "t"(val) : "st");
#else
    r = S4(val);
#endif
    ret r;
}

dfa F8 U8ToF8(U8 val)
{
    ret F8(val);
}
dfa F8 S8ToF8(S8 val)
{
    ret F8(val);
}

dfa F8 U4ToF8(U4 val)
{
    ret F8(val);
}
dfa F8 S4ToF8(S4 val)
{
    ret F8(val);
}
dfa F8 F4ToF8(F4 val)
{
    ret F8(val);
}

dfa U4 F8ToU4(F8 val)
{
    ret U4(val);
}
dfa S4 F8ToS4(F8 val)
{
    ret S4(val);
}
dfa F4 F8ToF4(F8 val)
{
    ret F4(val);
}

dfa F4 U4ToF4(U4 val)
{
    ret F4(val);
}
dfa F4 S4ToF4(S4 val)
{
    ret F4(val);
}

tpl0 dfa NT ToType(U1& dst, cx std::string& src)
{
    StrToInt(dst, src.c_str(), SI(src.size()));
}
tpl0 dfa NT ToType(F4& dst, cx CS* cx& src)
{
    StrToFloat(dst, src);
}
tpl0 dfa NT ToType(F4& dst, cx std::string& src)
{
    StrToFloat(dst, src.c_str());
}
tpl0 dfa NT ToType(BO& dst, cx std::string& src)
{
    dst = ((src.size() != 0) && (src[0] != '0' || src.size() > 1));
}
tpl0 dfa NT ToType(SI& dst, cx std::string& src)
{
    StrToInt(dst, src.c_str(), SI(src.size()));
}
tpl0 dfa NT ToType(std::string& dst, cx std::string& src)
{
    dst = src;
}
tpl0 dfa NT ToType(std::wstring& dst, cx std::string& src)
{
    dst = std::wstring(src.begin(), src.end());
}
