#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "macro-off.hpp"
    #include <shlobj.h>
    #include "macro-on.hpp"
    #include "unint.hpp"
    #include "array.hpp"
#endif

#ifdef PROG_SYS_WIN

class RegKey
{
  private:
    HD m_hdl;

  private:
    dfa ER _Get(ArrSbo<U1, 512>& buf, cx CH* name) cx;

  public:
    dfa HD Hdl() cx;
    dfa ER GetU4(U4& val, cx CH* name) cx;
    dfa ER GetStr(std::wstring& val, cx CH* name) cx;
    dfa ER SetU4(cx CH* name, U4 val) cx;
    dfa ER SetStr(cx CH* name, cx CH* val) cx;
    dfa ER Open(cx CH* path, U4 access, U4 options = 0, HD hdlRoot = NUL);
    dfa ER Close();

  public:
    dfa RegKey();
    dfa ~RegKey();
};

dfa BO RegKeyIsExist(cx CH* path);
dfa BO RegKeyIsExist(cx CH* pathBase, cx std::span<FNV1A64L>& subKeyList, HD hdlRoot = NUL);

dfa ER RegKeyGetStr(std::wstring& val, cx CH* path, cx CH* name);

dfa cx CH* _RegKeySplitSub(cx CH* path, HKEY& key);

dfa ER RegDirCreate(cx CH* path);
dfa ER RegDirCpy(cx CH* dst, cx CH* src);
dfa ER RegDirDel(cx CH* path);
dfa ER RegDirMove(cx CH* dst, cx CH* src);

dfa ER RegValSetStr(cx CH* path, cx CH* val);
dfa SI RegValGetStr(cx CH* path, CH* val, SI valLenMax);
dfa ER RegValSetU4(cx CH* path, U4 val);
dfa SI RegValGetU4(cx CH* path, U4& val);
dfa ER RegValDel(cx CH* path);

dfa ER RegSetExtOpenPath(cx CH* ext, cx CH* prog, cx CH* extName);

#endif

#include "registry.ipp"
