#pragma once

#include "typeex.hpp"
#include <string>

dfa S4 F4ToS4Fast(F4 val);

dfa F8 U8ToF8(U8 val);
dfa F8 S8ToF8(S8 val);

dfa F8 U4ToF8(U4 val);
dfa F8 S4ToF8(S4 val);
dfa F8 F4ToF8(F4 val);

dfa U4 F8ToU4(F8 val);
dfa S4 F8ToS4(F8 val);
dfa F4 F8ToF4(F8 val);

dfa F4 U4ToF4(U4 val);
dfa F4 S4ToF4(S4 val);

tpl2 dfa SI StrToInt(T1& dst, cx T2* src);
tpl2 dfa SI StrToInt(T1& dst, cx T2* src, SI len);
tpl2 dfa T1 StrToInt(cx T2* src);
tpl2 dfa SI StrToFloat(T1& dst, cx T2* src);

tpl2 dfa SI IntToStr(T1* dst, T2 src);
tpl2 dfa SI IntToStrBase16(T1* dst, T2 src, SI padToLen = siz(T2) * 2, T1 padVal = '0');

tpl0 dfa NT ToType(U1& dst, cx std::string& src);
tpl0 dfa NT ToType(F4& dst, cx CS* cx& src);
tpl0 dfa NT ToType(F4& dst, cx std::string& src);
tpl0 dfa NT ToType(BO& dst, cx std::string& src);
tpl0 dfa NT ToType(SI& dst, cx std::string& src);
tpl0 dfa NT ToType(std::string& dst, cx std::string& src);
tpl0 dfa NT ToType(std::wstring& dst, cx std::string& src);

#include "typeconvert.ipp"
