#include "file.hpp"

#include <filesystem>
#include "directory.hpp"

dfa BO FileIsExist(cx CH* path, BO acceptFile, BO acceptDir)
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

#ifdef PROG_SYS_WIN
dfa ER File::_OpenWin(cx CH* path, U4 access, U4 share, U4 openMode, U4 attrib)
{
    ifu (tx->IsOpen())
        rete(ErrVal::YES_INIT);
    m_hdl = CreateFileW(path, access, share, NUL, openMode, attrib, NUL);
    ifu (!tx->IsOpen())
        rete(ErrVal::FILE);
    rets;
}
dfa ER File::_OpenNt(cx CH* path, U4 access, U4 share, U4 openMode, U4 attrib, SI allocSize, U4 options, HD hdlRoot)
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
dfa ER File::_OpenStd(cx CH* path, cx CS* openMode)
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
dfa HD File::Hdl() cx
{
    ret HD(m_hdl);
}
dfa BO File::IsOpen() cx
{
    ret m_hdl != FILE_HDL_NONE;
}
dfa ER File::CurMove(SI ofs) cx
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
dfa ER File::CurSet(SI pos) cx
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
dfa ER File::CurGet(SI& pos) cx
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
dfa ER File::OpenRead(cx CH* path)
{
#ifdef PROG_SYS_WIN
    ret tx->_OpenNt(path, FILE_READ_DATA | SYNCHRONIZE, FILE_SHARE_READ, FILE_OPEN);
#else
    ret tx->_OpenStd(path, "rb");
#endif
}
dfa ER File::OpenWrite(cx CH* path)
{
#ifdef PROG_SYS_WIN
    ret tx->_OpenNt(path, FILE_WRITE_DATA | SYNCHRONIZE, 0, FILE_OVERWRITE_IF);
#else
    ret tx->_OpenStd(path, "wb");
#endif
}
dfa ER File::OpenReadWrite(cx CH* path)
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
dfa ER File::Close()
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
dfa ER File::Read(GA buf, SI size, SI& result) cx
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
dfa ER File::Read(GA buf, SI size) cx
{
    SI result;
    ife (tx->Read(buf, size, result))
        retep;
    ifu (size != result)
        rete(ErrVal::NO_FULL);
    rets;
}
dfa ER File::Write(CXGA buf, SI size) cx
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
dfa ER File::SizeGet(SI& size) cx
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
dfa ER File::SizeSet(SI size) cx
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
dfa ER File::TimeGet(TmUnix& modTime) cx
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
dfa ER File::Flush() cx
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
dfa File::File() : m_hdl(FILE_HDL_NONE)
{
}
dfa File::~File()
{
    tx->Close();
}

dfa ER BufToFile(cx CH* path, CXGA buf, SI size)
{
    File file;
    ifep(file.OpenWrite(path));
    ifep(file.Write(buf, size));
    ifep(file.Close());
    rets;
}

dfa HD MemFile::Hdl() cx
{
    ret FILE_HDL_NONE;
}
dfa BO MemFile::IsOpen() cx
{
    ret m_cur != NUL;
}
dfa cx CH* MemFile::Path() cx
{
    ret m_path.c_str();
}
dfa SI MemFile::Size() cx
{
    ifu (!tx->IsOpen())
        ret 0;
    ret SI(m_end - m_buf.data());
}
dfa U1* MemFile::Dat()
{
    ret m_buf.data();
}
dfa U1* MemFile::Cur()
{
    ret m_cur;
}
dfa U1* MemFile::End()
{
    ret m_end;
}
dfa NT MemFile::CurMove(SI ofs)
{
    m_cur += ofs;
}
dfa NT MemFile::CurSet(SI pos)
{
    m_cur = m_buf.data() + pos;
}
dfa NT MemFile::CurGet(SI& pos)
{
    pos = SI(m_cur - m_buf.data());
}
dfa SI MemFile::CurGet()
{
    ret SI(m_cur - m_buf.data());
}
dfa ER MemFile::Open(cx CH* path, BO mustExist, BO doClr)
{
    ifu (tx->IsOpen())
        rete(ErrVal::YES_INIT);
    if (path == NUL)
    {
        jdst(jmp1);
        if (path == NUL)
            m_path.clear();
        else
            m_path = path;
        m_buf.resize(FILE_MEM_RESIZE_ADD_DEFA);
        m_cur = m_buf.data();
        m_end = m_cur;
        rets;
    }
    if (doClr)
    {
        ifu (mustExist && !FileIsExist(path, YES, YES))
            rete(ErrVal::FILE);
        jsrc(jmp1);
    }
    else
    {
        ife (FileToBuf(m_buf, path))
        {
            ifu (mustExist && !FileIsExist(path, YES, YES))
                rete(ErrVal::FILE);
            jsrc(jmp1);
        }
    }
    m_path = path;
    m_cur = m_buf.data();
    m_end = m_cur + m_buf.size();
    rets;
}
dfa ER MemFile::Close(BO doSave, cx CH* pathOverride)
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
dfa SI MemFile::Read(GA buf, SI size)
{
    ifu (m_cur + size > m_end)
        size = m_end - m_cur;
    m_cur += MemCpy(buf, m_cur, size);
    ret size;
}
dfa SI MemFile::ReadLine(std::string& str)
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
dfa NT MemFile::Write(CXGA buf, SI size)
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
    m_cur += MemCpy(m_cur, buf, size);
}
dfa NT MemFile::WriteCsstr(cx CS* str)
{
    tx->Write(str, StrLen(str) * siz(CS));
}
dfa ER MemFile::SizeGet(SI& size) cx
{
    size = 0;
    ifu (!tx->IsOpen())
        rete(ErrVal::NO_INIT);
    size = SI(m_end - m_buf.data());
    rets;
}
dfa ER MemFile::SizeSet(SI size)
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
dfa ER MemFile::TimeGet(TmUnix& modTime) cx
{
    rete(ErrVal::NO_SUPPORT);
}
dfa ER MemFile::Flush() cx
{
    rete(ErrVal::NO_SUPPORT);
}
dfa MemFile::MemFile() : m_cur(NUL), m_end(NUL)
{
}
dfa MemFile::~MemFile()
{
    tx->Close();
}

#ifdef PROG_SYS_WIN

dfa ER FileCpy(cx CH* dst, cx CH* src, BO isReplace)
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
dfa ER FileMove(cx CH* dst, cx CH* src, BO isReplace)
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
