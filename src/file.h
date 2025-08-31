#pragma once

// pre-defined:
dfa ER DirNew(cx CH* path);

#ifdef PROG_SYS_ESP32
cxex SI FILE_READ_SIZE_LOW = BYTE_IN_KB / 16;
#else
cxex SI FILE_READ_SIZE_LOW = BYTE_IN_KB * 4;
#endif
cxex SI FILE_READ_SIZE_MED = FILE_READ_SIZE_LOW * 64;
cxex SI FILE_READ_SIZE_HIGH = FILE_READ_SIZE_MED * 64;

#ifdef PROG_SYS_WIN
    #define FILE_HDL_NONE INVALID_HANDLE_VALUE
#else
    #define FILE_HDL_NONE NUL
#endif

dfa BO FileIsExist(cx CH* path, BO acceptFile = YES, BO acceptDir = YES)
{
#ifdef PROG_SYS_WIN
    CH path_[PATH_LENX_MAX];
    cx AU pathLen = PathToNtpath(path_, path);
    UNICODE_STRING_ str(path_, pathLen);
    OBJECT_ATTRIBUTES_ oa(str);
    FILE_BASIC_INFORMATION_ info;
    if (NtQueryAttributesFile_(&oa, &info) != STATUS_SUCCESS)
        ret NO;
    if (info.FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
        ret NO;
    if (!acceptFile && !(info.FileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        ret NO;
    if (!acceptDir && (info.FileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        ret NO;
    ret YES;
#else
    std::filesystem::path path_(path);
    if (!std::filesystem::exists(path_))
        ret NO;
    if (acceptFile && std::filesystem::is_regular_file(path_))
        ret YES;
    if (acceptDir && std::filesystem::is_directory(path_))
        ret YES;
    ret NO;
#endif
}

class File
{
  private:
#ifdef PROG_SYS_WIN
    HD m_hdl;
#else
    FILE* m_hdl;
#endif

  public:
#ifdef PROG_SYS_WIN
    dfa ER _OpenWin(cx CH* path, U4 access, U4 share, U4 openMode, U4 attrib)
    {
        ifu (tx->IsOpen())
            rete(ErrVal::YES_INIT);
        m_hdl = CreateFileW(path, access, share, NUL, openMode, attrib, NUL);
        ifu (!tx->IsOpen())
            rete(ErrVal::FILE);
        rets;
    }
    dfa ER _OpenNt(cx CH* path, U4 access, U4 share, U4 openMode, U4 attrib = FILE_ATTRIBUTE_NORMAL, SI allocSize = 0, U4 options = FILE_SYNCHRONOUS_IO_NONALERT, HD hdlRoot = NUL)
    {
        ifu (m_hdl != FILE_HDL_NONE)
            rete(ErrVal::YES_INIT);
        CH path_[PATH_LENX_MAX];
        cx AU pathLen = ((hdlRoot == NUL) ? PathToNtpath(path_, path) : StrCpyStrLen(path_, path));
        UNICODE_STRING_ us(path_, pathLen);
        OBJECT_ATTRIBUTES_ oa(us);
        oa.RootDirectory = hdlRoot;
        IO_STATUS_BLOCK_ isb;
        LARGE_INTEGER_ allocSize_(allocSize);
        cx AU status = NtCreateFile_(&m_hdl, access, &oa, &isb, &allocSize_, attrib, share, openMode, options, NUL, 0);
        ifu (status != STATUS_SUCCESS)
        {
            m_hdl = FILE_HDL_NONE;
            rete(ErrVal::FILE);
        }
        rets;
    }
#else
    dfa ER _OpenStd(cx CH* path, cx CS* openMode)
    {
        ifu (tx->IsOpen())
            rete(ErrVal::YES_INIT);
        CS path_[PATH_LENX_MAX];
        ChstrToCsstr(path_, path);
        m_hdl = fopen(path_, openMode);
        ifu (!tx->IsOpen())
            rete(ErrVal::FILE);
        rets;
    }
#endif

  public:
    dfa HD Hdl() cx
    {
        ret HD(m_hdl);
    }
    dfa BO IsOpen() cx
    {
        ret m_hdl != FILE_HDL_NONE;
    }

  public:
    dfa ER CurMove(SI ofs) cx
    {
        ifu (!tx->IsOpen())
            rete(ErrVal::NO_INIT);
#ifdef PROG_SYS_WIN
        LARGE_INTEGER tmp = {};
        tmp.QuadPart = ofs;
        ifu (SetFilePointerEx(m_hdl, tmp, NUL, FILE_CURRENT) == 0)
            rete(ErrVal::FILE);
#else
        ifu (fseek(m_hdl, S4(ofs), SEEK_CUR) != 0)
            rete(ErrVal::FILE);
#endif
        rets;
    }
    dfa ER CurSet(SI pos) cx
    {
        ifu (!tx->IsOpen())
            rete(ErrVal::NO_INIT);
#ifdef PROG_SYS_WIN
        LARGE_INTEGER tmp = {};
        tmp.QuadPart = pos;
        ifu (SetFilePointerEx(m_hdl, tmp, NUL, FILE_BEGIN) == 0)
            rete(ErrVal::FILE);
#else
        ifu (fseek(m_hdl, S4(pos), SEEK_SET) != 0)
            rete(ErrVal::FILE);
#endif
        rets;
    }
    dfa ER CurGet(SI& pos) cx
    {
        pos = 0;
        ifu (!tx->IsOpen())
            rete(ErrVal::NO_INIT);
#ifdef PROG_SYS_WIN
        LARGE_INTEGER tmp = {};
        ifu (SetFilePointerEx(m_hdl, tmp, &tmp, FILE_CURRENT) == 0)
            rete(ErrVal::FILE);
        pos = tmp.QuadPart;
#else
        cx AU tmp = ftell(m_hdl);
        ifu (tmp < 0)
            rete(ErrVal::FILE);
        pos = tmp;
#endif
        rets;
    }
    dfa ER OpenRead(cx CH* path)
    {
#ifdef PROG_SYS_WIN
        ret tx->_OpenNt(path, FILE_READ_DATA | SYNCHRONIZE, FILE_SHARE_READ, FILE_OPEN);
#else
        ret tx->_OpenStd(path, "rb");
#endif
    }
    dfa ER OpenWrite(cx CH* path)
    {
#ifdef PROG_SYS_WIN
        ret tx->_OpenNt(path, FILE_WRITE_DATA | SYNCHRONIZE, 0, FILE_OVERWRITE_IF);
#else
        ret tx->_OpenStd(path, "wb");
#endif
    }
    dfa ER OpenReadWrite(cx CH* path)
    {
#ifdef PROG_SYS_WIN
        ret tx->_OpenNt(path, FILE_READ_DATA | FILE_WRITE_DATA | SYNCHRONIZE, 0, FILE_OPEN_IF);
#else
        ife (tx->_OpenStd(path, "r+b"))
        {
            if (ErrLastGet() != ErrVal::FILE)
                retep;
            ret tx->_OpenStd(path, "w+b");
        }
        rets;
#endif
    }
    dfa ER Close()
    {
        if (!tx->IsOpen())
            rets;
#ifdef PROG_SYS_WIN
        ifu (NtClose_(m_hdl) != STATUS_SUCCESS)
            rete(ErrVal::FILE);
#else
        ifu (fclose(m_hdl) != 0)
            rete(ErrVal::FILE);
#endif
        m_hdl = FILE_HDL_NONE;
        rets;
    }
    dfa ER Read(GA buf, SI size, SI& result) cx
    {
        result = 0;
        ifu (!tx->IsOpen())
            rete(ErrVal::NO_INIT);
#ifdef PROG_SYS_WIN
        DWORD tmp;
        ifu (ReadFile(m_hdl, buf, DWORD(size), &tmp, NUL) == 0)
            rete(ErrVal::FILE);
#else
        cx AU tmp = fread(buf, 1, size, m_hdl);
        ifu (ferror(m_hdl))
            rete(ErrVal::FILE);
#endif
        result = tmp;
        rets;
    }
    dfa ER Read(GA buf, SI size) cx
    {
        SI result;
        ife (tx->Read(buf, size, result))
            retep;
        ifu (size != result)
            rete(ErrVal::NO_FULL);
        rets;
    }
    dfa ER Write(CXGA buf, SI size) cx
    {
        ifu (!tx->IsOpen())
            rete(ErrVal::NO_INIT);
#ifdef PROG_SYS_WIN
        DWORD result;
        ifu (WriteFile(m_hdl, buf, DWORD(size), &result, NUL) == 0)
            rete(ErrVal::FILE);
#else
        cx AU result = fwrite(buf, 1, size, m_hdl);
        ifu (ferror(m_hdl))
            rete(ErrVal::FILE);
#endif
        ifu (size != SI(result))
            rete(ErrVal::NO_FULL);
        rets;
    }
    dfa ER SizeGet(SI& size) cx
    {
        size = 0;
        ifu (!tx->IsOpen())
            rete(ErrVal::NO_INIT);
#ifdef PROG_SYS_WIN
        LARGE_INTEGER tmp;
        ifu (GetFileSizeEx(m_hdl, &tmp) == 0)
            rete(ErrVal::FILE);
        size = tmp.QuadPart;
#else
        SI curPos;
        ifep(tx->CurGet(curPos));
        ifu (fseek(m_hdl, 0, SEEK_END) != 0)
            rete(ErrVal::FILE);
        SI endPos;
        ifep(tx->CurGet(endPos));
        ifep(tx->CurSet(curPos));
        size = endPos;
#endif
        rets;
    }
    dfa ER SizeSet(SI size) cx
    {
#ifdef PROG_SYS_WIN
        SI curPos;
        ifep(tx->CurGet(curPos));
        ifep(tx->CurSet(size));
        ifu (SetEndOfFile(m_hdl) == 0)
            rete(ErrVal::FILE);
        if (size > curPos)
            ifep(tx->CurSet(curPos));
#else
        unimp;
#endif
        rets;
    }
    dfa ER TimeGet(TmUnix& modTime) cx
    {
        modTime = 0;
        ifu (!tx->IsOpen())
            rete(ErrVal::NO_INIT);
#ifdef PROG_SYS_WIN
        FILETIME tmp;
        ifu (GetFileTime(m_hdl, NUL, NUL, &tmp) == 0)
            rete(ErrVal::FILE);
        modTime = LdapToUnix(U8(tmp.dwLowDateTime) | (U8(tmp.dwHighDateTime) << sizb(U4)));
#else
        unimp;
#endif
        rets;
    }
    dfa ER Flush() cx
    {
        ifu (!tx->IsOpen())
            rete(ErrVal::NO_INIT);
#ifdef PROG_SYS_WIN
        ifu (FlushFileBuffers(m_hdl) == 0)
            rete(ErrVal::FILE);
#else
        ifu (fflush(m_hdl) != 0)
            rete(ErrVal::FILE);
#endif
        rets;
    }

  public:
    dfa File() : m_hdl(FILE_HDL_NONE)
    {
    }
    dfa ~File()
    {
        tx->Close();
    }
};

dfa ER BufToFile(cx CH* path, CXGA buf, SI size)
{
    File file;
    ifep(file.OpenWrite(path));
    ifep(file.Write(buf, size));
    ifep(file.Close());
    rets;
}
tpl1 dfa ER FileToBuf(std::vector<T1>& buf, cx CH* path, CXGA bufAppend = NUL, SI sizeAppend = 0)
{
    File file;
    ifep(file.OpenRead(path));
    SI size;
    ifep(file.SizeGet(size));
    buf.resize(size + sizeAppend);
    ifep(file.Read(buf.data(), size));
    if (sizeAppend != 0)
        MemCpy(buf.data() + size, bufAppend, sizeAppend);
    ifep(file.Close());
    rets;
}

#ifdef PROG_SYS_ESP32
cxex F8 FILE_MEM_RESIZE_MUL_DEFA = 1.25;
cxex SI FILE_MEM_RESIZE_ADD_DEFA = BYTE_IN_KB / 4;
#else
cxex F8 FILE_MEM_RESIZE_MUL_DEFA = 1.5;
cxex SI FILE_MEM_RESIZE_ADD_DEFA = BYTE_IN_KB;
#endif

class MemFile
{
  private:
    std::wstring m_path;
    std::vector<U1> m_buf;
    U1* m_cur;
    U1* m_end;

  public:
    dfa HD Hdl() cx
    {
        ret FILE_HDL_NONE;
    }
    dfa BO IsOpen() cx
    {
        ret m_cur != NUL;
    }
    dfa cx CH* Path() cx
    {
        ret m_path.c_str();
    }
    dfa SI Size() cx
    {
        ifu (!tx->IsOpen())
            ret 0;
        ret SI(m_end - m_buf.data());
    }
    dfa U1* Dat()
    {
        ret m_buf.data();
    }
    dfa U1* Cur()
    {
        ret m_cur;
    }
    dfa U1* End()
    {
        ret m_end;
    }

  public:
    dfa NT CurMove(SI ofs)
    {
        m_cur += ofs;
    }
    dfa NT CurSet(SI pos)
    {
        m_cur = m_buf.data() + pos;
    }
    dfa NT CurGet(SI& pos)
    {
        pos = SI(m_cur - m_buf.data());
    }
    dfa SI CurGet()
    {
        ret SI(m_cur - m_buf.data());
    }
    dfa ER Open(cx CH* path = NUL, BO mustExist = YES)
    {
        ifu (tx->IsOpen())
            rete(ErrVal::YES_INIT);
        if (path == NUL)
        {
            jdst(jmp1);
            m_buf.resize(FILE_MEM_RESIZE_ADD_DEFA);
            m_cur = m_buf.data();
            m_end = m_cur;
            rets;
        }
        ife (FileToBuf(m_buf, path))
        {
            if (!mustExist && !FileIsExist(path, YES, YES))
            {
                m_path = path;
                jsrc(jmp1);
            }
            rete(ErrVal::FILE);
        }
        m_path = path;
        m_cur = m_buf.data();
        m_end = m_cur + m_buf.size();
        rets;
    }
    dfa ER Close(BO doSave = NO, cx CH* pathOverride = NUL)
    {
        if (!tx->IsOpen())
            rets;
        if (doSave)
        {
            cx CH* path = ((pathOverride == NUL) ? m_path.c_str() : pathOverride);
            ife (BufToFile(path, m_buf.data(), SI(m_end - m_buf.data())))
                retep;
        }
        m_path = L"";
        m_buf.clear();
        m_buf.shrink_to_fit();
        m_cur = NUL;
        m_end = NUL;
        rets;
    }
    dfa SI Read(GA buf, SI size)
    {
        ifu (m_cur + size > m_end)
            size = m_end - m_cur;
        MemCpy(buf, m_cur, size);
        m_cur += size;
        ret size;
    }
    tpl1 dfa SI ReadVal(T1& val)
    {
        ret tx->Read((GA)&val, siz(T1));
    }
    tpl1 dfa SI ReadVarint(T1& val)
    {
        ifl (m_end - m_cur > (VarintSizeMax<T1>()))
        {
            cx AU size = VarintDecode<T1>(val, m_cur);
            m_cur += size;
            ret size;
        }
        U1 buf[VarintSizeMax<T1>()];
        SI size = 0;
        do
        {
            ifu (m_cur == m_end)
            {
                val = 0;
                ret 0;
            }
            *(buf + size) = *m_cur;
            ++m_cur;
            ++size;
        } while (VarintIsIncomplete(buf, size));
        ret VarintDecode<T1>(val, buf);
    }
    tpl1 dfa SI ReadVarbaseint(T1& val, U1 base)
    {
        ifl (m_end - m_cur > (VarbaseintSizeMax<T1, 1>()))
        {
            cx AU size = VarbaseintDecode<T1>(val, m_cur, base);
            m_cur += size;
            ret size;
        }
        U1 buf[VarbaseintSizeMax<T1, 1>()];
        SI size = 0;
        do
        {
            ifu (m_cur == m_end)
            {
                val = 0;
                ret 0;
            }
            *(buf + size) = *m_cur;
            ++m_cur;
            ++size;
        } while (VarbaseintIsIncomplete(buf, size, base));
        ret VarbaseintDecode<T1>(val, buf, base);
    }
    tpl1 dfa SI ReadValSeqBox(std::vector<T1>& vals)
    {
        vals.clear();
        SI datSize = 0;
        cx AU readSize = tx->ReadVarbaseint(datSize, 7);
        ifu (readSize == 0)
            ret 0;
        cx AU fullSize = readSize + datSize;
        tx->CurMove(-readSize);
        BitVec box;
        box.Reserve(fullSize * BIT_IN_BYTE);
        box._LenSet(fullSize * BIT_IN_BYTE);
        MemCpy(box._Dat(), m_cur, fullSize);
        m_cur += fullSize;
        ValSeqBoxDecode(vals, box);
        ret fullSize;
    }
    dfa SI ReadLine(std::string& str)
    {
        cx AU curBase = m_cur;
        while (m_cur < m_end)
        {
            if (*m_cur == '\r' || *m_cur == '\n')
            {
                cx AU lineLen = SI(m_cur - curBase);
                str.assign((CS*)curBase, lineLen);
                m_cur += (1 + ((*m_cur == '\r') && (m_cur + 1 < m_end) && (*(m_cur + 1) == '\n')));
                ret lineLen + 1;
            }
            ++m_cur;
        }
        if (m_cur > curBase)
        {
            cx AU lineLen = SI(m_cur - curBase);
            str.assign((CS*)curBase, lineLen);
            ret lineLen + 1;
        }
        str.clear();
        ret 0;
    }
    dfa NT Write(CXGA buf, SI size)
    {
        if (m_cur + size > m_end)
        {
            ifu (m_cur + size > m_buf.data() + m_buf.size())
            {
                cx AU posI = SI(m_cur - m_buf.data());
                cx AU sizeNew = Max(SI(F8(m_buf.size()) * FILE_MEM_RESIZE_MUL_DEFA) + FILE_MEM_RESIZE_ADD_DEFA, posI + size);
                m_buf.resize(sizeNew);
                m_cur = m_buf.data() + posI;
            }
            m_end = m_cur + size;
        }
        MemCpyUpdCur(m_cur, buf, size);
    }
    tpl1 dfa NT WriteVal(cx T1& val)
    {
        tx->Write((CXGA)&val, siz(T1));
    }
    tpl1 dfa NT WriteVarint(cx T1& val)
    {
        U1 buf[VarintSizeMax<T1>()];
        cx AU size = VarintEncode<T1>(buf, val);
        tx->Write(buf, size);
    }
    dfa NT WriteCsstr(cx CS* str)
    {
        tx->Write(str, StrLen(str) * siz(CS));
    }
    dfa ER SizeGet(SI& size) cx
    {
        size = 0;
        ifu (!tx->IsOpen())
            rete(ErrVal::NO_INIT);
        size = SI(m_end - m_buf.data());
        rets;
    }
    dfa ER SizeSet(SI size)
    {
        ifu (!tx->IsOpen())
            rete(ErrVal::NO_INIT);
        ifu (size < 0)
            rete(ErrVal::LOW_SIZE);
        cx AU sizeNow = SI(m_end - m_buf.data());
        cx AU sizeDiff = size - sizeNow;
        if (sizeDiff > 0)
        {
            if (size > SI(m_buf.size()))
            {
                cx AU posI = SI(m_cur - m_buf.data());
                m_buf.resize(size);
                m_cur = m_buf.data() + posI;
                m_end = m_buf.data() + sizeNow;
            }
            MemSet(m_end, 0, sizeDiff);
            m_end += sizeDiff;
        }
        else
        {
            m_end += sizeDiff;
            if (m_cur > m_end)
                m_cur = m_end;
        }
        rets;
    }
    dfa ER TimeGet(TmUnix& modTime) cx
    {
        rete(ErrVal::NO_SUPPORT);
    }
    dfa ER Flush() cx
    {
        rete(ErrVal::NO_SUPPORT);
    }

  public:
    dfa MemFile() : m_cur(NUL), m_end(NUL)
    {
    }
    dfa ~MemFile()
    {
        tx->Close();
    }
};

#ifdef PROG_SYS_WIN

dfa ER FileCpy(cx CH* dst, cx CH* src, BO isReplace = YES)
{
    SI tryCnt = 0;
    jdst(retry);
    cx AU result = CopyFileW(src, dst, !isReplace);
    ifu (result == 0)
    {
        ++tryCnt;
        ifu (tryCnt == 2)
            rete(ErrVal::FILE);
        if (GetLastError() == ERROR_PATH_NOT_FOUND && FileIsExist(src, YES, YES))
        {
            CH dirPath[PATH_LENX_MAX];
            PathToAbspath(dirPath, dst);
            PathDirUp(dirPath);
            ife (DirNew(dirPath))
                retep;
            jsrc(retry);
        }
        else
        {
            rete(ErrVal::FILE);
        }
    }
    rets;
}
dfa ER FileDel(cx CH* path)
{
    ifu (DeleteFileW(path) == 0)
        rete(ErrVal::FILE);
    rets;
}
dfa ER FileMove(cx CH* dst, cx CH* src, BO isReplace = YES)
{
    SI tryCnt = 0;
    jdst(retry);
    cx AU result = MoveFileExW(src, dst, MOVEFILE_COPY_ALLOWED | (isReplace ? MOVEFILE_REPLACE_EXISTING : 0));
    ifu (result == 0)
    {
        ++tryCnt;
        ifu (tryCnt == 2)
            rete(ErrVal::FILE);
        if (GetLastError() == ERROR_PATH_NOT_FOUND && FileIsExist(src, YES, YES))
        {
            CH dirPath[PATH_LENX_MAX];
            PathToAbspath(dirPath, dst);
            PathDirUp(dirPath);
            ife (DirNew(dirPath))
                retep;
            jsrc(retry);
        }
        else
        {
            rete(ErrVal::FILE);
        }
    }
    rets;
}

dfa ER FileTimeGet(TmUnix& time, cx CH* path)
{
    time = 0;
    WIN32_FILE_ATTRIBUTE_DATA info = {};
    ifu (GetFileAttributesExW(path, GetFileExInfoStandard, &info) == 0)
        rete(ErrVal::FILE);
    time = LdapToUnix(U8(info.ftLastWriteTime.dwLowDateTime) | (U8(info.ftLastWriteTime.dwHighDateTime) << sizb(U4)));
    rets;
}
dfa ER FileSizeSet(cx CH* path, SI size)
{
    cx AU hdl = CreateFileW(path, GENERIC_WRITE, 0, NUL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NUL);
    ifu (hdl == FILE_HDL_NONE)
        rete(ErrVal::FILE);
    LARGE_INTEGER tmp = {};
    tmp.QuadPart = size;
    ifu (SetFilePointerEx(hdl, tmp, NUL, FILE_BEGIN) == 0)
    {
        NtClose_(hdl);
        rete(ErrVal::FILE);
    }
    ifu (SetEndOfFile(hdl) == 0)
    {
        NtClose_(hdl);
        rete(ErrVal::FILE);
    }
    NtClose_(hdl);
    rets;
}

#endif
