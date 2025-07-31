#pragma once

#ifdef PROG_SYS_WIN

class RegKey
{
  private:
    HD m_hdl;

  private:
    dfa ER _Get(ArrSbo<U1, 512>& buf, cx CH* name) cx
    {
        UNICODE_STRING_ str(name);
        jdst(retry);
        U4 resultSize = 0;
        ifu (NtQueryValueKey_(m_hdl, &str, KEY_VALUE_INFORMATION_CLASS_::KeyValuePartialInformation, buf.Dat(), U4(buf.Size()), &resultSize) != STATUS_SUCCESS)
        {
            if (resultSize > buf.Size())
            {
                buf.Resize(resultSize / siz(U1), 0);
                jsrc(retry);
            }
            rete(ErrVal::REG);
        }
        rets;
    }

  public:
    dfa ER GetU4(U4& val, cx CH* name) cx
    {
        val = 0;
        ArrSbo<U1, 512> buf;
        ife (tx->_Get(buf, name))
            retep;
        cx AU& info = *(KEY_VALUE_PARTIAL_INFORMATION_*)buf.Dat();
        ifu ((info.Type != REG_DWORD_LITTLE_ENDIAN && info.Type != REG_DWORD_BIG_ENDIAN) || (info.DataLength != siz(U4)))
            rete(ErrVal::REG);
        val = *(U4*)info.Data;
        if (info.Type == REG_DWORD_BIG_ENDIAN)
            val = RevByte(val);
        rets;
    }
    dfa ER GetStr(std::wstring& val, cx CH* name) cx
    {
        val.clear();
        ArrSbo<U1, 512> buf;
        ife (tx->_Get(buf, name))
            retep;
        cx AU& info = *(KEY_VALUE_PARTIAL_INFORMATION_*)buf.Dat();
        ifu (info.Type != REG_SZ)
            rete(ErrVal::REG);
        AU len = SI(info.DataLength) / siz(CH);
        if (len > 0 && ((CH*)info.Data)[len - 1] == '\0')
            --len;
        val.assign((CH*)info.Data, len);
        rets;
    }
    dfa ER SetU4(cx CH* name, U4 val) cx
    {
        UNICODE_STRING_ str(name);
        ifu (NtSetValueKey_(m_hdl, &str, 0, REG_DWORD_LITTLE_ENDIAN, &val, siz(val)) != STATUS_SUCCESS)
            rete(ErrVal::REG);
        rets;
    }
    dfa ER SetStr(cx CH* name, cx CH* val) cx
    {
        UNICODE_STRING_ str(name);
        ifu (NtSetValueKey_(m_hdl, &str, 0, REG_SZ, val, U4(StrLenx(val) * siz(val[0]))) != STATUS_SUCCESS)
            rete(ErrVal::REG);
        rets;
    }
    dfa ER Open(cx CH* path, U4 access, U4 options)
    {
        ifu (m_hdl != NUL)
            rete(ErrVal::YES_INIT);
        UNICODE_STRING_ str(path);
        OBJECT_ATTRIBUTES_ oa;
        oa.ObjectName = &str;
        oa.Attributes = OBJ_CASE_INSENSITIVE;
        ifu (NtOpenKeyEx_(&m_hdl, access, &oa, options) != STATUS_SUCCESS)
            rete(ErrVal::REG);
        rets;
    }
    dfa ER Close()
    {
        if (m_hdl == NUL)
            rets;
        ifu (NtClose_(m_hdl) != STATUS_SUCCESS)
            rete(ErrVal::REG);
        m_hdl = NUL;
        rets;
    }

  public:
    dfa RegKey() : m_hdl(NUL)
    {
    }
    dfa ~RegKey()
    {
        tx->Close();
    }
};

dfa BO RegKeyIsExist(cx CH* path)
{
    RegKey regKey;
    ife (regKey.Open(path, 0, 0))
        ret NO;
    regKey.Close(); // error ignored
    ret YES;
}

dfa ER RegKeyGetStr(std::wstring& val, cx CH* path, cx CH* name)
{
    val.clear();
    RegKey regKey;
    ife (regKey.Open(path, KEY_READ, 0))
        retep;
    ife (regKey.GetStr(val, name))
        retep;
    ife (regKey.Close())
        retep;
    rets;
}

dfa cx CH* _RegKeySplitSub(cx CH* path, HKEY& key)
{
    ifu (StrCmp(path, L"HKEY_", 5) != 0)
        ret NUL;
    path += 5;
    if (StrCmp(path, L"CLASSES_ROOT", 12) == 0)
    {
        key = HKEY_CLASSES_ROOT;
        path += 12;
    }
    else if (StrCmp(path, L"CURRENT_USER", 12) == 0)
    {
        key = HKEY_CURRENT_USER;
        path += 12;
    }
    else if (StrCmp(path, L"LOCAL_MACHINE", 13) == 0)
    {
        key = HKEY_LOCAL_MACHINE;
        path += 13;
    }
    else if (StrCmp(path, L"USERS", 5) == 0)
    {
        key = HKEY_USERS;
        path += 5;
    }
    else if (StrCmp(path, L"CURRENT_CONFIG", 14) == 0)
    {
        key = HKEY_CURRENT_CONFIG;
        path += 14;
    }
    else
    {
        ret NUL;
    }
    ifu (*path != '\\')
        ret NUL;
    ret path + 1;
}

dfa ER RegDirCreate(cx CH* path)
{
    HKEY key;
    cx CH* subKey = _RegKeySplitSub(path, key);
    ifu (subKey == NUL)
        rete(ErrVal::REG);
    HKEY subKeyHdl = NUL;
    ifu (RegCreateKeyExW(key, subKey, 0, NUL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NUL, &subKeyHdl, NUL) != ERROR_SUCCESS)
        rete(ErrVal::REG);
    ifu (RegCloseKey(subKeyHdl) != ERROR_SUCCESS)
        rete(ErrVal::REG);
    rets;
}
dfa ER RegDirCpy(cx CH* dst, cx CH* src)
{
    HKEY keySrc;
    cx CH* subKeySrc = _RegKeySplitSub(src, keySrc);
    ifu (subKeySrc == NUL)
        rete(ErrVal::REG);
    HKEY keyDst;
    cx CH* subKeyDst = _RegKeySplitSub(dst, keyDst);
    ifu (subKeyDst == NUL)
        rete(ErrVal::REG);
    ifu (RegCopyTreeW(keySrc, subKeySrc, keyDst) != ERROR_SUCCESS)
        rete(ErrVal::REG);
    rets;
}
dfa ER RegDirDel(cx CH* path)
{
    HKEY key;
    cx CH* subKey = _RegKeySplitSub(path, key);
    ifu (subKey == NUL)
        rete(ErrVal::REG);
    ifu (RegDeleteTreeW(key, subKey) != ERROR_SUCCESS)
        rete(ErrVal::REG);
    rets;
}
dfa ER RegDirMove(cx CH* dst, cx CH* src)
{
    ife (RegDirCpy(dst, src))
        retep;
    ife (RegDirDel(src))
        retep;
    rets;
}

dfa ER RegValSetStr(cx CH* path, cx CH* val)
{
    CH path_[PATH_LEN_MAX];
    StrCpy(path_, path);
    CH* name = (CH*)StrFindLast(path_, L'\\');
    ifu (name == NUL)
        rete(ErrVal::REG);
    *name++ = '\0';
    HKEY key;
    cx CH* subKey = _RegKeySplitSub(path_, key);
    ifu (subKey == NUL)
        rete(ErrVal::REG);
    HKEY subKeyHdl;
    ifu (RegOpenKeyExW(key, subKey, 0, KEY_WRITE, &subKeyHdl) != ERROR_SUCCESS)
        rete(ErrVal::REG);
    ifu (RegSetValueExW(subKeyHdl, name, 0, REG_SZ, (BYTE*)val, DWORD((StrLen(val) + 1) * siz(val[0]))) != ERROR_SUCCESS)
        rete(ErrVal::REG);
    ifu (RegCloseKey(subKeyHdl) != ERROR_SUCCESS)
        rete(ErrVal::REG);
    rets;
}
dfa SI RegValGetStr(cx CH* path, CH* val, SI valLenMax)
{
    CH path_[PATH_LEN_MAX];
    StrCpy(path_, path);
    CH* name = (CH*)StrFindLast(path_, L'\\');
    ifu (name == NUL)
        ret -1;
    *name++ = '\0';
    HKEY key;
    cx CH* subKey = _RegKeySplitSub(path_, key);
    ifu (subKey == NUL)
        ret -1;
    HKEY subKeyHdl;
    ifu (RegOpenKeyExW(key, subKey, 0, KEY_READ, &subKeyHdl) != ERROR_SUCCESS)
        ret -1;
    DWORD type;
    DWORD valLen = DWORD(valLenMax * siz(val[0]));
    ifu (RegQueryValueExW(subKeyHdl, name, NUL, &type, (BYTE*)val, &valLen) != ERROR_SUCCESS)
        ret -1;
    ifu (RegCloseKey(subKeyHdl) != ERROR_SUCCESS)
        ret -1;
    ifu (type != REG_SZ)
        ret -1;
    ret SI(valLen) / siz(val[0]);
}
dfa ER RegValSetU4(cx CH* path, U4 val)
{
    CH path_[PATH_LEN_MAX];
    StrCpy(path_, path);
    CH* name = (CH*)StrFindLast(path_, L'\\');
    ifu (name == NUL)
        rete(ErrVal::REG);
    *name++ = '\0';
    HKEY key;
    cx CH* subKey = _RegKeySplitSub(path_, key);
    ifu (subKey == NUL)
        rete(ErrVal::REG);
    HKEY subKeyHdl;
    ifu (RegOpenKeyExW(key, subKey, 0, KEY_WRITE, &subKeyHdl) != ERROR_SUCCESS)
        rete(ErrVal::REG);
    ifu (RegSetValueExW(subKeyHdl, name, 0, REG_DWORD, (BYTE*)&val, siz(val)) != ERROR_SUCCESS)
        rete(ErrVal::REG);
    ifu (RegCloseKey(subKeyHdl) != ERROR_SUCCESS)
        rete(ErrVal::REG);
    rets;
}
dfa SI RegValGetU4(cx CH* path, U4& val)
{
    CH path_[PATH_LEN_MAX];
    StrCpy(path_, path);
    CH* name = (CH*)StrFindLast(path_, L'\\');
    ifu (name == NUL)
        ret -1;
    *name++ = '\0';
    HKEY key;
    cx CH* subKey = _RegKeySplitSub(path_, key);
    ifu (subKey == NUL)
        ret -1;
    HKEY subKeyHdl;
    ifu (RegOpenKeyExW(key, subKey, 0, KEY_READ, &subKeyHdl) != ERROR_SUCCESS)
        ret -1;
    DWORD type;
    DWORD valLen = siz(val);
    ifu (RegQueryValueExW(subKeyHdl, name, NUL, &type, (BYTE*)&val, &valLen) != ERROR_SUCCESS)
        ret -1;
    ifu (RegCloseKey(subKeyHdl) != ERROR_SUCCESS)
        ret -1;
    ifu (type != REG_DWORD)
        ret -1;
    ret 1;
}
dfa ER RegValDel(cx CH* path)
{
    HKEY key;
    cx CH* subKey = _RegKeySplitSub(path, key);
    ifu (subKey == NUL)
        rete(ErrVal::REG);
    ifu (RegDeleteValueW(key, subKey) != ERROR_SUCCESS)
        rete(ErrVal::REG);
    rets;
}

dfa ER RegSetExtOpenPath(cx CH* ext, cx CH* prog, cx CH* extName)
{
    CH name[PATH_LEN_MAX];
    StrCpy(name, L"def.");
    StrAdd(name, ext);
    CH path[PATH_LEN_MAX];
    StrCpy(path, L"HKEY_CLASSES_ROOT\\.");
    StrAdd(path, ext);
    ife (RegDirCreate(path))
        retep;
    StrAdd(path, L"\\");
    ife (RegValSetStr(path, name))
        retep;
    StrCpy(path, L"HKEY_CLASSES_ROOT\\");
    StrAdd(path, name);
    ife (RegDirCreate(path))
        retep;
    StrAdd(path, L"\\");
    ife (RegValSetStr(path, extName))
        retep;
    StrCpy(path, L"HKEY_CLASSES_ROOT\\");
    StrAdd(path, name);
    StrAdd(path, L"\\shell\\open\\command");
    ife (RegDirCreate(path))
        retep;
    StrAdd(path, L"\\");
    CH cmd[PATH_LEN_MAX + 10];
    StrCpy(cmd, L"\"");
    StrAdd(cmd, prog);
    StrAdd(cmd, L"\" \"%1\"");
    ife (RegValSetStr(path, cmd))
        retep;
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NUL, NUL);
    rets;
}

#endif
