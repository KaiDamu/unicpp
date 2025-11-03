#pragma once

#include <vector>
#include "type.hpp"
#include "container.hpp"
#include "string.hpp"

namespace serial
{

tpl1 dfa ER ReadType(cx U1*& cur, cx U1* end, T1& val);
dfa ER _ReadAll(cx U1*& cur, cx U1* end);
tpl<typename T1, typename... TRest> dfa ER _ReadAll(cx U1*& cur, cx U1* end, T1& first, TRest&... rest);
tpl<typename... TArgs> dfa ER Read(cx U1*& cur, cx U1* end, TArgs&... args);

dfa NT Req(std::vector<U1>& buf, SI reqSize);

tpl1 dfa ER WriteType(std::vector<U1>& buf, SI& curI, cx T1& val);
dfa ER _WriteAll(std::vector<U1>& buf, SI& curI);
tpl<typename T1, typename... TRest> dfa ER _WriteAll(std::vector<U1>& buf, SI& curI, cx T1& first, cx TRest&... rest);
tpl<typename... TArgs> dfa ER Write(std::vector<U1>& buf, SI& curI, cx TArgs&... args);

struct TransIo
{
    BO isWrite;
    std::tuple<cx U1*&, cx U1*&>* read;
    std::tuple<std::vector<U1>&, SI&>* write;

    dfa TransIo();
    dfa TransIo(std::tuple<cx U1*&, cx U1*&>* read);
    dfa TransIo(std::tuple<std::vector<U1>&, SI&>* write);
};

tpl<typename T1, typename... TArgs> dfa ER Trans(T1&, TArgs&...);
tpl<typename... TArgs> dfa ER Trans(std::tuple<cx U1*&, cx U1*&>& io, TArgs&... args);
tpl<typename... TArgs> dfa ER Trans(std::tuple<std::vector<U1>&, SI&>& io, TArgs&... args);
tpl<typename... TArgs> dfa ER Trans(TransIo& io, TArgs&... args);

} // namespace serial

namespace serial
{

tpl0 dfa ER ReadType(cx U1*& cur, cx U1* end, std::string& val);
tpl0 dfa ER ReadType(cx U1*& cur, cx U1* end, ZzVarint& val);

tpl0 dfa ER WriteType(std::vector<U1>& buf, SI& curI, cx std::string& val);
tpl0 dfa ER WriteType(std::vector<U1>& buf, SI& curI, cx ZzVarint& val);

} // namespace serial

#include "serial.ipp"
