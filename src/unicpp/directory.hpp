#pragma once

#include <vector>
#include <string>
#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "macro-off.hpp"
    #include <windows.h>
    #include <winternl.h>
    #include "macro-on.hpp"
    #include "time.hpp"
#endif

#ifdef PROG_SYS_WIN

struct FileInfoCommon
{
    std::wstring path;
    SI pathRelOfs;
    SI sizeDat;
    SI sizeDisk;
    U4 attrib;
    TmUnix tmCreate;
    TmUnix tmAccess;
    TmUnix tmWrite;
    TmUnix tmChange;
};

// return YES to continue, NO to stop
using DirFileEnumCallbFnT = BO (*)(cx FileInfoCommon& fileInfo, GA param1, GA param2);

dfa ER _DirFileEnum(cx CH* path, SI depth, DirFileEnumCallbFnT callb, GA param1, GA param2, BO inPostOrder, HD hdlRoot, CH* pathFull, CH* pathFullEnd, SI pathRelOfs);
dfa ER DirFileEnum(cx CH* path, SI depth, DirFileEnumCallbFnT callb, GA param1, GA param2, BO inPostOrder);

dfa ER DirFileList(std::vector<FileInfoCommon>& fileInfoList, cx CH* path, SI depth = -1);

dfa ER _DirDel(cx CH* path);

dfa ER DirNew(cx CH* path);
dfa ER DirCpy(cx CH* dst, cx CH* src, BO isReplace = YES);
dfa ER DirDel(cx CH* path);
dfa ER DirMove(cx CH* dst, cx CH* src, BO isReplace = YES);

#endif

#include "directory.ipp"
