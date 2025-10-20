#pragma once

#include <span>
#include <vector>
#include "unint.hpp"
#include "container.hpp"

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

dfa BO FileIsExist(cx CH* path, BO acceptFile = YES, BO acceptDir = YES);

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
    dfa ER _OpenWin(cx CH* path, U4 access, U4 share, U4 openMode, U4 attrib);
    dfa ER _OpenNt(cx CH* path, U4 access, U4 share, U4 openMode, U4 attrib = FILE_ATTRIBUTE_NORMAL, SI allocSize = 0, U4 options = FILE_SYNCHRONOUS_IO_NONALERT, HD hdlRoot = NUL);
#else
    dfa ER _OpenStd(cx CH* path, cx CS* openMode);
#endif

  public:
    dfa HD Hdl() cx;
    dfa BO IsOpen() cx;

  public:
    dfa ER CurMove(SI ofs) cx;
    dfa ER CurSet(SI pos) cx;
    dfa ER CurGet(SI& pos) cx;
    dfa ER OpenRead(cx CH* path);
    dfa ER OpenWrite(cx CH* path);
    dfa ER OpenReadWrite(cx CH* path);
    dfa ER Close();
    dfa ER Read(GA buf, SI size, SI& result) cx;
    dfa ER Read(GA buf, SI size) cx;
    dfa ER Write(CXGA buf, SI size) cx;
    dfa ER SizeGet(SI& size) cx;
    dfa ER SizeSet(SI size) cx;
    dfa ER TimeGet(TmUnix& modTime) cx;
    dfa ER Flush() cx;

  public:
    dfa File();
    dfa ~File();
};

dfa ER BufToFile(cx CH* path, CXGA buf, SI size);
tpl1 dfa ER FileToBuf(std::vector<T1>& buf, cx CH* path, CXGA bufAppend = NUL, SI sizeAppend = 0);

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
    dfa HD Hdl() cx;
    dfa BO IsOpen() cx;
    dfa cx CH* Path() cx;
    dfa SI Size() cx;
    dfa U1* Dat();
    dfa U1* Cur();
    dfa U1* End();

  public:
    dfa NT CurMove(SI ofs);
    dfa NT CurSet(SI pos);
    dfa NT CurGet(SI& pos);
    dfa SI CurGet();
    dfa ER Open(cx CH* path = NUL, BO mustExist = YES, BO doClr = NO);
    dfa ER Close(BO doSave = NO, cx CH* pathOverride = NUL);
    dfa SI Read(GA buf, SI size);
    tpl1 dfa SI Read(cx std::span<T1>& buf);
    tpl<typename TRead, typename TDst> dfa ER ReadVal(TDst& val);
    tpl1 dfa SI ReadVarint(T1& val);
    tpl1 dfa SI ReadVarbaseint(T1& val, U1 base);
    tpl<typename T1, ValSeqBoxMode TMode> dfa SI ReadValSeqBox(std::vector<T1>& vals);
    dfa SI ReadLine(std::string& str);
    dfa NT Write(CXGA buf, SI size);
    tpl1 dfa NT Write(cx std::span<cx T1>& buf);
    tpl1 dfa NT WriteVal(cx T1& val);
    tpl1 dfa NT WriteVarint(cx T1& val);
    dfa NT WriteCsstr(cx CS* str);
    dfa ER SizeGet(SI& size) cx;
    dfa ER SizeSet(SI size);
    dfa ER TimeGet(TmUnix& modTime) cx;
    dfa ER Flush() cx;

  public:
    dfa MemFile();
    dfa ~MemFile();
};

#ifdef PROG_SYS_WIN

dfa ER FileCpy(cx CH* dst, cx CH* src, BO isReplace = YES);
dfa ER FileDel(cx CH* path);
dfa ER FileMove(cx CH* dst, cx CH* src, BO isReplace = YES);

dfa ER FileTimeGet(TmUnix& time, cx CH* path);
dfa ER FileSizeSet(cx CH* path, SI size);

#endif

#include "file.ipp"
