#pragma once

cxex SI FILE_READ_SIZE_LOW = BYTE_IN_KB * 4;
cxex SI FILE_READ_SIZE_MED = BYTE_IN_KB * 256;
cxex SI FILE_READ_SIZE_HIGH = BYTE_IN_KB * 16384;

class FileStd
{
  private:
    FILE* m_hdl;

  public:
    dfa FILE* Hdl()
    {
        ret m_hdl;
    }
    dfa BO IsOpen()
    {
        ret m_hdl != NUL;
    }
    dfa ER CurMove(SI cnt)
    {
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        ifu (fseek(m_hdl, cnt, SEEK_CUR) != 0)
            rete(ErrVal::FILE);
        rets;
    }
    dfa ER CurSet(SI pos)
    {
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        ifu (fseek(m_hdl, pos, SEEK_SET) != 0)
            rete(ErrVal::FILE);
        rets;
    }
    dfa ER CurGet(SI& pos)
    {
        pos = 0;
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        cx SI tmp = SI(ftell(m_hdl));
        ifu (tmp < 0)
            rete(ErrVal::FILE);
        pos = tmp;
        rets;
    }
    dfa ER OpenRead(cx CH* path)
    {
        ifu (tx->IsOpen() == YES)
            rete(ErrVal::YES_INIT);
#ifdef PROG_SYS_WIN
        m_hdl = _wfopen(path, L"rb");
#else
        CS path_[PATH_LENX_MAX];
        ChstrToCsstr(path_, path);
        m_hdl = fopen(path_, "rb");
#endif
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::FILE);
        rets;
    }
    dfa ER OpenWrite(cx CH* path)
    {
        ifu (tx->IsOpen() == YES)
            rete(ErrVal::YES_INIT);
#ifdef PROG_SYS_WIN
        m_hdl = _wfopen(path, L"wb");
#else
        CS path_[PATH_LENX_MAX];
        ChstrToCsstr(path_, path);
        m_hdl = fopen(path_, "wb");
#endif
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::FILE);
        rets;
    }
    dfa ER OpenReadWrite(cx CH* path)
    {
        ifu (tx->IsOpen() == YES)
            rete(ErrVal::YES_INIT);
#ifdef PROG_SYS_WIN
        m_hdl = _wfopen(path, L"rb+");
#else
        CS path_[PATH_LENX_MAX];
        ChstrToCsstr(path_, path);
        m_hdl = fopen(path_, "rb+");
#endif
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::FILE);
        rets;
    }
    dfa ER Close()
    {
        if (tx->IsOpen() == NO)
            rets;
        ifu (fclose(m_hdl) != 0)
            rete(ErrVal::FILE);
        m_hdl = NUL;
        rets;
    }
    dfa ER Read(GA buf, SI size, SI& result)
    {
        result = 0;
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        SI tmp = SI(fread(buf, 1, size, m_hdl));
        ifu (ferror(m_hdl))
            rete(ErrVal::FILE);
        result = tmp;
        rets;
    }
    dfa ER Write(CXGA buf, SI size, SI& result)
    {
        result = 0;
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        SI tmp = SI(fwrite(buf, 1, size, m_hdl));
        ifu (ferror(m_hdl))
            rete(ErrVal::FILE);
        result = tmp;
        rets;
    }
    dfa ER Read(GA buf, SI size)
    {
        SI result;
        ife (tx->Read(buf, size, result))
            retep;
        ifu (size != result)
            rete(ErrVal::NO_FULL);
        rets;
    }
    dfa ER Write(CXGA buf, SI size)
    {
        SI result;
        ife (tx->Write(buf, size, result))
            retep;
        ifu (size != result)
            rete(ErrVal::NO_FULL);
        rets;
    }
    dfa ER SizeGet(SI& size)
    {
        size = 0;
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        SI tmp = SI(ftell(m_hdl));
        ifu (tmp < 0)
            rete(ErrVal::FILE);
        ifu (fseek(m_hdl, 0, SEEK_END) != 0)
            rete(ErrVal::FILE);
        size = SI(ftell(m_hdl));
        ifu (size < 0)
            rete(ErrVal::FILE);
        ifu (fseek(m_hdl, tmp, SEEK_SET) != 0)
            rete(ErrVal::FILE);
        rets;
    }
    dfa ER SizeSet(SI size)
    {
        unimp;
        rete(ErrVal::NO_SUPPORT);
    }
    dfa ER TimeGet(TmUnix& time)
    {
        time = 0;
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        struct stat st;
        ifu (fstat(fileno(m_hdl), &st) != 0)
            rete(ErrVal::FILE);
        time = TmUnix(st.st_mtime);
        rets;
    }
    dfa ER Flush()
    {
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        ifu (fflush(m_hdl) != 0)
            rete(ErrVal::FILE);
        rets;
    }

  public:
    dfa FileStd()
    {
        m_hdl = NUL;
    }
    dfa ~FileStd()
    {
        tx->Close();
    }
};

#ifdef PROG_SYS_WIN

dfa ER DirNew(cx CH* path); // referenced here for FileMove

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
        if (GetLastError() == ERROR_PATH_NOT_FOUND && PathIsExist(src))
        {
            CH dirPath[PATH_LENX_MAX];
            PathToAbs(dirPath, dst);
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
        if (GetLastError() == ERROR_PATH_NOT_FOUND && PathIsExist(src))
        {
            CH dirPath[PATH_LENX_MAX];
            PathToAbs(dirPath, dst);
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
    ifu (hdl == INVALID_HANDLE_VALUE)
        rete(ErrVal::FILE);
    LARGE_INTEGER tmp = {};
    tmp.QuadPart = size;
    ifu (SetFilePointerEx(hdl, tmp, NUL, FILE_BEGIN) == 0)
    {
        CloseHandle(hdl);
        rete(ErrVal::FILE);
    }
    ifu (SetEndOfFile(hdl) == 0)
    {
        CloseHandle(hdl);
        rete(ErrVal::FILE);
    }
    CloseHandle(hdl);
    rets;
}

class FileWin
{
  private:
    HANDLE m_hdl;

  public:
    dfa HANDLE Hdl()
    {
        ret m_hdl;
    }
    dfa BO IsOpen()
    {
        ret m_hdl != INVALID_HANDLE_VALUE;
    }
    dfa ER CurMove(SI cnt)
    {
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        LARGE_INTEGER tmp = {};
        tmp.QuadPart = cnt;
        if (SetFilePointerEx(m_hdl, tmp, NUL, FILE_CURRENT) == 0)
            rete(ErrVal::FILE);
        rets;
    }
    dfa ER CurSet(SI pos)
    {
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        LARGE_INTEGER tmp = {};
        tmp.QuadPart = pos;
        ifu (SetFilePointerEx(m_hdl, tmp, NUL, FILE_BEGIN) == 0)
            rete(ErrVal::FILE);
        rets;
    }
    dfa ER CurGet(SI& pos)
    {
        pos = 0;
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        LARGE_INTEGER tmp = {};
        ifu (SetFilePointerEx(m_hdl, tmp, &tmp, FILE_CURRENT) == 0)
            rete(ErrVal::FILE);
        pos = tmp.QuadPart;
        rets;
    }
    dfa ER Open(cx CH* path, DWORD access, DWORD share, DWORD openMode, DWORD attrib)
    {
        ifu (tx->IsOpen() == YES)
            rete(ErrVal::YES_INIT);
        m_hdl = CreateFileW(path, access, share, NUL, openMode, attrib, NUL);
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::FILE);
        rets;
    }
    dfa ER OpenRead(cx CH* path)
    {
        ret tx->Open(path, FILE_READ_DATA, FILE_SHARE_READ, OPEN_EXISTING, 0);
    }
    dfa ER OpenWrite(cx CH* path)
    {
        ret tx->Open(path, FILE_WRITE_DATA, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL);
    }
    dfa ER OpenReadWrite(cx CH* path)
    {
        ret tx->Open(path, FILE_READ_DATA | FILE_WRITE_DATA, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL);
    }
    dfa ER Close()
    {
        if (tx->IsOpen() == NO)
            rets;
        ifu (CloseHandle(m_hdl) == 0)
            rete(ErrVal::FILE);
        m_hdl = INVALID_HANDLE_VALUE;
        rets;
    }
    dfa ER Read(GA buf, SI size, SI& result)
    {
        result = 0;
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        DWORD tmp;
        ifu (ReadFile(m_hdl, buf, DWORD(size), &tmp, NUL) == 0)
            rete(ErrVal::FILE);
        result = tmp;
        rets;
    }
    dfa ER Write(CXGA buf, SI size, SI& result)
    {
        result = 0;
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        DWORD tmp;
        ifu (WriteFile(m_hdl, buf, DWORD(size), &tmp, NUL) == 0)
            rete(ErrVal::FILE);
        result = tmp;
        rets;
    }
    dfa ER Read(GA buf, SI size)
    {
        SI result;
        ife (tx->Read(buf, size, result))
            retep;
        ifu (size != result)
            rete(ErrVal::NO_FULL);
        rets;
    }
    dfa ER Write(CXGA buf, SI size)
    {
        SI result;
        ife (tx->Write(buf, size, result))
            retep;
        ifu (size != result)
            rete(ErrVal::NO_FULL);
        rets;
    }
    dfa ER SizeGet(SI& size)
    {
        size = 0;
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        LARGE_INTEGER tmp;
        ifu (GetFileSizeEx(m_hdl, &tmp) == 0)
            rete(ErrVal::FILE);
        size = tmp.QuadPart;
        rets;
    }
    dfa ER SizeSet(SI size)
    {
        SI curPos;
        ife (tx->CurGet(curPos))
            retep;
        ife (tx->CurSet(size))
            retep;
        ifu (SetEndOfFile(m_hdl) == 0)
            rete(ErrVal::FILE);
        if (size > curPos)
            ife (tx->CurSet(curPos))
                retep;
        rets;
    }
    dfa ER TimeGet(TmUnix& time)
    {
        time = 0;
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        FILETIME tmp;
        ifu (GetFileTime(m_hdl, NUL, NUL, &tmp) == 0)
            rete(ErrVal::FILE);
        time = LdapToUnix(U8(tmp.dwLowDateTime) | (U8(tmp.dwHighDateTime) << sizb(U4)));
        rets;
    }
    dfa ER Flush()
    {
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        ifu (FlushFileBuffers(m_hdl) == 0)
            rete(ErrVal::FILE);
        rets;
    }

  public:
    dfa FileWin()
    {
        m_hdl = INVALID_HANDLE_VALUE;
    }
    dfa ~FileWin()
    {
        tx->Close();
    }
};

class FileNt
{
  private:
    HANDLE m_hdl;

  public:
    dfa HANDLE Hdl()
    {
        ret m_hdl;
    }
    dfa BO IsOpen()
    {
        ret m_hdl != INVALID_HANDLE_VALUE;
    }
    /*dfa ER CurMove(SI cnt) {
        rete(ErrVal::NA);
    }*/
    dfa ER CurSet(SI pos)
    {
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        IO_STATUS_BLOCK iosb;
        FILE_POSITION_INFORMATION info = {};
        info.CurrentByteOffset.QuadPart = pos;
        cx NTSTATUS status = NtSetInformationFile(m_hdl, &iosb, &info, siz(FILE_POSITION_INFORMATION), ToNative(FILE_INFORMATION_CLASS_::FilePositionInformation));
        ifu (status != STATUS_SUCCESS)
            rete(ErrVal::FILE);
        rets;
    }
    dfa ER Open(cx CH* path, ACCESS_MASK access, SI allocSize, ULONG attrib, ULONG share, ULONG openMode, ULONG flags)
    {
        ifu (tx->IsOpen() == YES)
            rete(ErrVal::YES_INIT);
        CH path_[PATH_LEN_MAX];
        cx SI pathLen = PathToNtpath(path_, path);
        UNICODE_STRING us = {};
        us.Length = USHORT(pathLen * siz(CH));
        us.MaximumLength = us.Length;
        us.Buffer = path_;
        OBJECT_ATTRIBUTES objAttrib = {};
        objAttrib.Length = siz(OBJECT_ATTRIBUTES);
        objAttrib.RootDirectory = NUL;
        objAttrib.ObjectName = &us;
        objAttrib.Attributes = 0;
        objAttrib.SecurityDescriptor = NUL;
        objAttrib.SecurityQualityOfService = NUL;
        IO_STATUS_BLOCK iosb;
        LARGE_INTEGER allocSize_ = {};
        allocSize_.QuadPart = allocSize;
        cx NTSTATUS status = NtCreateFile(&m_hdl, access, &objAttrib, &iosb, &allocSize_, attrib, share, openMode, flags, NUL, 0);
        ifu (status != STATUS_SUCCESS)
        {
            m_hdl = INVALID_HANDLE_VALUE;
            rete(ErrVal::FILE);
        }
        rets;
    }
    dfa ER OpenRead(cx CH* path)
    {
        ret tx->Open(path, FILE_READ_DATA | SYNCHRONIZE, 0, 0, FILE_SHARE_READ, FILE_OPEN, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);
    }
    dfa ER OpenWrite(cx CH* path)
    {
        ret tx->Open(path, FILE_WRITE_DATA | SYNCHRONIZE, 0, FILE_ATTRIBUTE_NORMAL, 0, FILE_OVERWRITE_IF, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);
    }
    dfa ER OpenReadWrite(cx CH* path)
    {
        ret tx->Open(path, FILE_READ_DATA | FILE_WRITE_DATA | SYNCHRONIZE, 0, FILE_ATTRIBUTE_NORMAL, 0, FILE_OPEN_IF, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);
    }
    dfa ER Close()
    {
        if (tx->IsOpen() == NO)
            rets;
        ifu (NtClose(m_hdl) != STATUS_SUCCESS)
            rete(ErrVal::FILE);
        m_hdl = INVALID_HANDLE_VALUE;
        rets;
    }
    dfa ER Read(GA buf, SI size, SI& result)
    {
        result = 0;
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        ifu ((size >> sizb(ULONG)) > 0)
            rete(ErrVal::HIGH_SIZE);
        IO_STATUS_BLOCK iosb;
        cx NTSTATUS status = NtReadFile(m_hdl, NUL, NUL, NUL, &iosb, buf, ULONG(size), NUL, NUL);
        ifu (status != STATUS_SUCCESS)
            rete(ErrVal::FILE);
        result = iosb.Information;
        rets;
    }
    /*dfa ER Write(CXGA buf, SI size, SI& result) {
        rete(ErrVal::NA);
    }*/
    dfa ER SizeGet(SI& size)
    {
        size = 0;
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        IO_STATUS_BLOCK iosb;
        SI infoCnt = 3;
        jdst(again);
        ++infoCnt;
        Arr<FILE_STREAM_INFORMATION> info(infoCnt);
        cx NTSTATUS status = NtQueryInformationFile(m_hdl, &iosb, info.Ptr(), ULONG(infoCnt * siz(FILE_STREAM_INFORMATION)), ToNative(FILE_INFORMATION_CLASS_::FileStreamInformation));
        ifu (status == STATUS_BUFFER_OVERFLOW)
            jsrc(again);
        ifu (status != STATUS_SUCCESS)
            rete(ErrVal::FILE);
        size = info[0].StreamSize.QuadPart;
        rets;
    }

  public:
    dfa FileNt()
    {
        m_hdl = INVALID_HANDLE_VALUE;
    }
    dfa ~FileNt()
    {
        tx->Close();
    }
};

#endif

class File
{
  private:
#ifdef PROG_SYS_WIN
    FileWin m_file;
#else
    FileStd m_file;
#endif

  public:
    dfa BO IsOpen()
    {
        ret m_file.IsOpen();
    }
    dfa ER CurMove(SI cnt)
    {
        ret m_file.CurMove(cnt);
    }
    dfa ER CurSet(SI pos)
    {
        ret m_file.CurSet(pos);
    }
    dfa ER CurGet(SI& pos)
    {
        ret m_file.CurGet(pos);
    }
    dfa ER OpenRead(cx CH* path)
    {
        ret m_file.OpenRead(path);
    }
    dfa ER OpenWrite(cx CH* path)
    {
        ret m_file.OpenWrite(path);
    }
    dfa ER OpenReadWrite(cx CH* path)
    {
        ret m_file.OpenReadWrite(path);
    }
    dfa ER Close()
    {
        ret m_file.Close();
    }
    dfa ER Read(GA buf, SI size, SI& result)
    {
        ret m_file.Read(buf, size, result);
    }
    dfa ER Write(CXGA buf, SI size, SI& result)
    {
        ret m_file.Write(buf, size, result);
    }
    dfa ER Read(GA buf, SI size)
    {
        ret m_file.Read(buf, size);
    }
    dfa ER Write(CXGA buf, SI size)
    {
        ret m_file.Write(buf, size);
    }
    dfa ER SizeGet(SI& size)
    {
        ret m_file.SizeGet(size);
    }
    dfa ER SizeSet(SI size)
    {
        ret m_file.SizeSet(size);
    }
    dfa ER TimeGet(TmUnix& time)
    {
        ret m_file.TimeGet(time);
    }
    dfa ER Flush()
    {
        ret m_file.Flush();
    }

  public:
    dfa File()
    {
        ;
    }
    dfa ~File()
    {
        ;
    }
};

cxex F8 FILE_MEM_RESIZE_MUL_DEFA = 1.5;
cxex SI FILE_MEM_RESIZE_ADD_DEFA = 1024;

class FileMem
{
  private:
    File m_file;
    Arr<U1> m_dat;
    SI m_filePos;
    SI m_filePosOfs;
    BO m_isOpen;
    BO m_isRead;
    BO m_isWrite;
    BO m_isWriteDirect;
    F8 m_resizeMul;
    SI m_resizeAdd;

  public:
    dfa U1* _Dat() cx
    {
        ret m_dat.Ptr();
    }

  public:
    dfa BO IsOpen() cx
    {
        ret m_isOpen;
    }
    dfa SI CurPos() cx
    {
        ret m_filePos + m_filePosOfs;
    }
    dfa ER CurMove(SI cnt)
    {
        ifu (m_filePos + m_filePosOfs + cnt < 0)
            rete(ErrVal::LOW_SIZE);
        ifu (m_filePos + m_filePosOfs + cnt > m_dat.Pos())
            rete(ErrVal::HIGH_SIZE);
        if (m_isWriteDirect)
            m_filePosOfs += cnt;
        else
            m_filePos += cnt;
        rets;
    }
    dfa ER CurSet(SI pos)
    {
        ifu (pos < 0)
            rete(ErrVal::LOW_SIZE);
        ifu (pos > m_dat.Pos())
            rete(ErrVal::HIGH_SIZE);
        if (m_isWriteDirect)
            m_filePosOfs = pos - m_filePos;
        else
            m_filePos = pos;
        rets;
    }
    dfa ER OpenRead(cx CH* path)
    {
        ifu (tx->IsOpen())
            rete(ErrVal::YES_INIT);
        ife (m_file.OpenRead(path))
            retep;
        SI size = 0;
        ife (m_file.SizeGet(size))
            retep;
        m_dat.New(size);
        ife (m_file.Read(m_dat.Ptr(), size))
            retep;
        m_dat.CurSet(size);
        // ife (m_file.Close()) retep;
        m_filePos = 0;
        m_filePosOfs = 0;
        m_isOpen = YES;
        m_isRead = YES;
        m_isWrite = NO;
        m_isWriteDirect = NO;
        m_resizeMul = 0.0;
        m_resizeAdd = 0;
        rets;
    }
    dfa ER OpenWrite(cx CH* path, BO isWriteDirect = NO, F8 resizeMul = FILE_MEM_RESIZE_MUL_DEFA, SI resizeAdd = FILE_MEM_RESIZE_ADD_DEFA)
    {
        ifu (tx->IsOpen())
            rete(ErrVal::YES_INIT);
        ife (m_file.OpenWrite(path))
            retep;
        SI size = 0;
        ife (m_file.SizeGet(size))
            retep;
        m_dat.New(SI(F8(size) * resizeMul + F8(resizeAdd)));
        MemSet(m_dat.Ptr(), 0, size);
        m_dat.CurSet(size);
        m_filePos = 0;
        m_filePosOfs = 0;
        m_isOpen = YES;
        m_isRead = NO;
        m_isWrite = YES;
        m_isWriteDirect = isWriteDirect;
        m_resizeMul = resizeMul;
        m_resizeAdd = resizeAdd;
        rets;
    }
    dfa ER OpenReadWrite(cx CH* path, BO isWriteDirect = NO, F8 resizeMul = FILE_MEM_RESIZE_MUL_DEFA, SI resizeAdd = FILE_MEM_RESIZE_ADD_DEFA)
    {
        ifu (tx->IsOpen())
            rete(ErrVal::YES_INIT);
        SI size = 0;
        if (path == NUL)
        {
            ifu (isWriteDirect)
                rete(ErrVal::NO_VALID);
            m_dat.New(SI(F8(size) * resizeMul + F8(resizeAdd)));
        }
        else
        {
            ife (m_file.OpenReadWrite(path))
                retep;
            ife (m_file.SizeGet(size))
                retep;
            m_dat.New(SI(F8(size) * resizeMul + F8(resizeAdd)));
            ife (m_file.Read(m_dat.Ptr(), size))
                retep;
        }
        m_dat.CurSet(size);
        m_filePos = 0;
        m_filePosOfs = 0;
        m_isOpen = YES;
        m_isRead = YES;
        m_isWrite = YES;
        m_isWriteDirect = isWriteDirect;
        m_resizeMul = resizeMul;
        m_resizeAdd = resizeAdd;
        rets;
    }
    dfa ER Close()
    {
        ifu (!tx->IsOpen())
            rets;
        if (m_isWrite && !m_isWriteDirect && m_file.IsOpen())
        {
            ife (m_file.CurSet(0))
                retep;
            ife (m_file.Write(m_dat.Ptr(), m_dat.Pos()))
                retep;
        }
        if (m_file.IsOpen())
            ife (m_file.Close())
                retep;
        m_dat.Del();
        m_filePos = 0;
        m_filePosOfs = 0;
        m_isOpen = NO;
        m_isRead = NO;
        m_isWrite = NO;
        m_isWriteDirect = NO;
        m_resizeMul = 0.0;
        m_resizeAdd = 0;
        rets;
    }
    dfa ER Read(GA buf, SI size, SI& result)
    {
        result = 0;
        ifu (m_filePos + m_filePosOfs + size > m_dat.Pos())
            rete(ErrVal::HIGH_SIZE);
        MemCpy(buf, m_dat.Ptr() + m_filePos + m_filePosOfs, size);
        if (m_isWriteDirect)
            m_filePosOfs += size;
        else
            m_filePos += size;
        result = size;
        rets;
    }
    dfa ER Write(CXGA buf, SI size, SI& result)
    {
        result = 0;
        ifu (!m_isWrite)
            rete(ErrVal::NO_INIT);
        if (m_isWriteDirect)
        {
            if (m_filePosOfs != 0)
            {
                ife (m_file.CurMove(m_filePosOfs))
                    retep;
                m_filePos += m_filePosOfs;
                m_filePosOfs = 0;
            }
            ife (m_file.Write(buf, size))
                retep;
        }
        ifu (m_filePos + size > m_dat.Cap())
        {
            cx SI newCap = SI(F8(m_dat.Cap()) * m_resizeMul) + m_resizeAdd + size;
            m_dat.Req(newCap, newCap, m_dat.Pos());
        }
        MemCpy(m_dat.Ptr() + m_filePos, buf, size);
        m_filePos += size;
        if (m_filePos > m_dat.Pos())
            m_dat.CurSet(m_filePos);
        result = size;
        rets;
    }
    dfa ER Read(GA buf, SI size)
    {
        SI result;
        ife (tx->Read(buf, size, result))
            retep;
        ifu (size != result)
            rete(ErrVal::NO_FULL);
        rets;
    }
    tpl1 dfa ER ReadVal(T1& val)
    {
        ret tx->Read((GA)&val, siz(T1));
    }
    tpl1 dfa ER ReadVarint(T1& val)
    {
        cx U1* buf = m_dat.Ptr() + m_filePos + m_filePosOfs;
        cx AU size = VarintDecode<T1>(val, buf);
        if (m_isWriteDirect)
            m_filePosOfs += size;
        else
            m_filePos += size;
        rets;
    }
    tpl1 dfa SI ReadVarbaseint(T1& val, U1 base)
    {
        U1 buf[VarbaseintSizeMax<T1, 1>()];
        SI size = 0;
        do
        {
            ife (tx->Read(buf + size, 1))
                ret 0;
            ++size;
        } while (VarbaseintIsIncomplete(buf, size, base));
        ret VarbaseintDecode(val, buf, base);
    }
    tpl1 dfa NT ReadValSeqBox(std::vector<T1>& vals)
    {
        SI datSize = 0;
        cx AU readSize = tx->ReadVarbaseint(datSize, 7);
        cx AU fullSize = readSize + datSize;
        tx->CurMove(-readSize);
        BitVec box;
        box.Reserve(fullSize * BIT_IN_BYTE);
        box._LenSet(fullSize * BIT_IN_BYTE);
        MemCpy(box._Dat(), tx->_Dat() + tx->CurPos(), fullSize);
        if (m_isWriteDirect)
            m_filePosOfs += fullSize;
        else
            m_filePos += fullSize;
        ValSeqBoxDecode(vals, box);
    }
    dfa BO ReadLine(std::string& str)
    {
        U1* ptrBase = m_dat.Ptr() + m_filePos;
        U1* ptr = ptrBase;
        U1* end = m_dat.Ptr() + m_dat.Pos();
        while (ptr < end)
        {
            if (*ptr == '\r' || *ptr == '\n')
            {
                str.assign((CS*)ptrBase, SI(ptr - ptrBase));
                m_filePos += ptr - ptrBase;
                if ((*ptr == '\r') && (ptr + 1 < end) && (*(ptr + 1) == '\n'))
                    ++m_filePos;
                ++m_filePos;
                ret YES;
            }
            ++ptr;
        }
        if (ptr > ptrBase)
        {
            str.assign((CS*)ptrBase, SI(ptr - ptrBase));
            m_filePos += ptr - ptrBase;
            ret YES;
        }
        ret NO;
    }
    dfa ER Write(CXGA buf, SI size)
    {
        SI result;
        ife (tx->Write(buf, size, result))
            retep;
        ifu (size != result)
            rete(ErrVal::NO_FULL);
        rets;
    }
    tpl1 dfa ER WriteVal(T1 val)
    {
        ret tx->Write((CXGA)&val, siz(T1));
    }
    tpl1 dfa ER WriteVarint(T1 val)
    {
        U1 buf[VarintSizeMax<T1>()];
        cx AU size = VarintEncode<T1>(buf, val);
        ret tx->Write(buf, size);
    }
    dfa ER SizeGet(SI& size)
    {
        size = 0;
        ifu (tx->IsOpen() == NO)
            rete(ErrVal::NO_INIT);
        size = m_dat.Pos();
        rets;
    }
    dfa SI SizeGet()
    {
        SI size;
        ife (tx->SizeGet(size))
            size = -1;
        ret size;
    }
    dfa ER SizeSet(SI size)
    { // TODO: TO TEST!
        ifu (!m_isWrite)
            rete(ErrVal::NO_INIT);
        if (m_isWriteDirect)
        {
            if (m_filePosOfs != 0)
            {
                ife (m_file.CurMove(m_filePosOfs))
                    retep;
                m_filePos += m_filePosOfs;
                m_filePosOfs = 0;
            }
            ife (m_file.SizeSet(size))
                retep;
        }
        cx AU isExtend = size > m_dat.Pos();
        if (isExtend)
        {
            ifu (size > m_dat.Cap())
            {
                cx SI newCap = SI(F8(size) * m_resizeMul) + m_resizeAdd;
                m_dat.Req(newCap, newCap, m_dat.Pos());
            }
            MemSet(m_dat.Ptr() + m_dat.Pos(), 0, size - m_dat.Pos());
        }
        m_filePos = Min(m_filePos, size);
        m_dat.CurSet(m_filePos); // set virtual size
        rets;
    }
    dfa ER TimeGet(TmUnix& time)
    {
        ret m_file.TimeGet(time);
    }
    dfa ER Flush()
    {
        ifu (!tx->IsOpen())
            rets;
        if (m_isWrite)
        {
            if (!m_isWriteDirect)
            { // to optimize
                SI curPos;
                ife (m_file.CurGet(curPos))
                    retep;
                ife (m_file.CurSet(0))
                    retep;
                ife (m_file.Write(m_dat.Ptr(), m_dat.Pos()))
                    retep;
                ife (m_file.CurSet(curPos))
                    retep;
            }
            ife (m_file.Flush())
                retep;
        }
        rets;
    }

  public:
    dfa FileMem()
    {
        m_filePos = 0;
        m_filePosOfs = 0;
        m_isOpen = NO;
        m_isRead = NO;
        m_isWrite = NO;
        m_isWriteDirect = NO;
        m_resizeMul = 0.0;
        m_resizeAdd = 0;
    }
    dfa ~FileMem()
    {
        tx->Close();
    }
};

dfa ER FileToDatIte(DatIte<U1>& datIte, cx CH* path)
{
    File file;
    ife (file.OpenRead(path))
        retep;
    SI size;
    ife (file.SizeGet(size))
        retep;
    Ptr<U1> buf(size);
    ife (file.Read(buf.Get(), size))
        retep;
    datIte.Src(buf.Get(), size);
    ife (file.Close())
        retep;
    rets;
}
tpl1 dfa ER FileToArr(Arr<T1>& arr, cx CH* path, cx CS* append = NUL)
{
    File file;
    ife (file.OpenRead(path))
        retep;
    SI size;
    ife (file.SizeGet(size))
        retep;
    SI sizeAppend = 0;
    if (append != NUL)
        sizeAppend = StrLen(append) + 1;
    arr.New(size + sizeAppend);
    ife (file.Read(arr.Ptr(), size))
        retep;
    ife (file.Close())
        retep;
    if (sizeAppend != 0)
        MemCpy(arr.Ptr() + size, append, sizeAppend);
    rets;
}

dfa ER ArrToFile(cx CH* path, cx Arr<U1>& arr, BO isFull = NO)
{
    File file;
    ife (file.OpenWrite(path))
        retep;
    cx SI size = isFull ? arr.Cap() : arr.Pos();
    ife (file.Write(arr.Ptr(), size))
        retep;
    ife (file.Close())
        retep;
    rets;
}
