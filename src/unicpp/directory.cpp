#include "directory.hpp"

#ifdef PROG_SYS_WIN

    #include "unint.hpp"
    #include "file.hpp"
    #include "array.hpp"
    #include "memory.hpp"

dfa ER _DirFileEnum(cx CH* path, SI depth, DirFileEnumCallbFnT callb, GA param1, GA param2, BO inPostOrder, HD hdlRoot, CH* pathFull, CH* pathFullEnd, SI pathRelOfs)
{
    if (depth == 0)
        rets;
    --depth;
    File dir;
    ifep(dir._OpenNt(path, FILE_LIST_DIRECTORY | SYNCHRONIZE, FILE_SHARE_READ, FILE_OPEN, 0, 0, FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, hdlRoot));
    IO_STATUS_BLOCK_ isb;
    ArrDFast<U1> buf(4 * BYTE_IN_KB - 64);
    cxex SI END_SIZE = siz(CH); // reserve space for null-terminator
    jdst(nextRead);
    cx AU status = NtQueryDirectoryFile_(dir.Hdl(), NUL, NUL, NUL, &isb, buf.Dat(), U4(buf.Size() - END_SIZE), FILE_INFORMATION_CLASS_::FileDirectoryInformation, NO, NUL, NO);
    if (status != STATUS_SUCCESS)
    {
        ifl (status == STATUS_NO_MORE_FILES)
            rets;
        rete(ErrVal::DIR);
    }
    cx AU resultSize = SI(isb.Information);
    cx AU infoBase = (FILE_DIRECTORY_INFORMATION_*)buf.Dat();
    AU infoCur = infoBase;
    while (YES)
    {
        AU& info = *infoCur;
        cx AU isSpecialA = (info.FileNameLength == siz(U2) && info.FileName[0] == '.');
        cx AU isSpecialB = (info.FileNameLength == siz(U4) && info.FileName[0] == '.' && info.FileName[1] == '.');
        if (!(isSpecialA || isSpecialB))
        {
            cx AU fileNameLen = SI(info.FileNameLength) / siz(CH);
            *pathFullEnd++ = L'\\';
            AsType<UA>(pathFullEnd) += MemCpy(pathFullEnd, info.FileName, info.FileNameLength);
            FileInfoCommon entry;
            entry.path.assign(pathFull, pathFullEnd);
            entry.pathRelOfs = pathRelOfs;
            entry.sizeDat = SI(info.EndOfFile.QuadPart);
            entry.sizeDisk = SI(info.AllocationSize.QuadPart);
            entry.attrib = info.FileAttributes;
            entry.tmCreate = LdapToUnix(info.CreationTime.QuadPart);
            entry.tmAccess = LdapToUnix(info.LastAccessTime.QuadPart);
            entry.tmWrite = LdapToUnix(info.LastWriteTime.QuadPart);
            entry.tmChange = LdapToUnix(info.ChangeTime.QuadPart);
            if (!inPostOrder)
                ifu (callb(entry, param1, param2) == NO)
                    rete(ErrVal::DIR_NO_ENUM);
            if (info.FileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                cx AU chSave = info.FileName[fileNameLen]; // safe since we reserved END_SIZE
                info.FileName[fileNameLen] = '\0';
                ifep(_DirFileEnum(info.FileName, depth, callb, param1, param2, inPostOrder, dir.Hdl(), pathFull, pathFullEnd, pathRelOfs));
                info.FileName[fileNameLen] = chSave;
            }
            if (inPostOrder)
                ifu (callb(entry, param1, param2) == NO)
                    rete(ErrVal::DIR_NO_ENUM);
            pathFullEnd -= fileNameLen + 1;
        }
        if (info.NextEntryOffset == 0)
            break;
        infoCur = (FILE_DIRECTORY_INFORMATION_*)((U1*)infoCur + info.NextEntryOffset);
    }
    if (resultSize > buf.Size() / 2)
        buf.ReNew(buf.Cnt() * 2);
    jsrc(nextRead);
}
dfa ER DirFileEnum(cx CH* path, SI depth, DirFileEnumCallbFnT callb, GA param1, GA param2, BO inPostOrder)
{
    CH path_[PATH_LENX_MAX];
    cx AU pathLen = PathToNtpath(path_, path);
    CH pathFull[PATH_LENX_MAX];
    MemCpy(pathFull, path_ + STR_NTPATH_PRE_LEN, (pathLen - STR_NTPATH_PRE_LEN) * siz(CH));
    CH* pathFullEnd = pathFull + pathLen - STR_NTPATH_PRE_LEN;
    cxex SI CH_PATH_DIR_CNT = 1;
    ife (_DirFileEnum(path_, depth, callb, param1, param2, inPostOrder, NUL, pathFull, pathFullEnd, SI(pathFullEnd - pathFull) + CH_PATH_DIR_CNT))
    {
        cx AU err = ErrLastGet();
        if (err != ErrVal::DIR_NO_ENUM)
            rete(err);
    }
    rets;
}

dfa ER DirFileList(std::vector<FileInfoCommon>& fileInfoList, cx CH* path, SI depth)
{
    fileInfoList.clear();
    cx DirFileEnumCallbFnT callb = [](cx FileInfoCommon& fileInfo, GA param1, GA param2) -> BO {
        ((std::vector<FileInfoCommon>*)param1)->emplace_back(fileInfo);
        unused(param2);
        ret YES;
    };
    ifep(DirFileEnum(path, depth, callb, &fileInfoList, NUL, NO));
    rets;
}

dfa ER _DirDel(cx CH* path)
{
    ifu (RemoveDirectoryW(path) == 0)
        rete(ErrVal::DIR);
    rets;
}

dfa ER DirNew(cx CH* path)
{
    ifu (CreateDirectoryW(path, NUL) == 0)
    {
        if (GetLastError() == ERROR_ALREADY_EXISTS)
        {
            if (FileIsExist(path, NO, YES))
                rets;
            rete(ErrVal::DIR);
        }
        CH path_[PATH_LENX_MAX];
        StrCpy(path_, path);
        AU pathSep = const_cast<CH*>(StrFindLast(path_, CH_PATH_DIR));
        ifu (pathSep == NUL)
            rete(ErrVal::DIR);
        *pathSep = '\0';
        ifep(DirNew(path_));
        ifu (CreateDirectoryW(path, NUL) == 0)
            rete(ErrVal::DIR);
    }
    rets;
}
dfa ER DirCpy(cx CH* dst, cx CH* src, BO isReplace)
{
    CH dst_[PATH_LENX_MAX];
    PathToAbspath(dst_, dst);
    ifep(DirNew(dst_));
    struct Param
    {
        CH* dst;
        BO isReplace;
        ErrVal err;
    };
    Param param = {dst_, isReplace, ErrVal::NONE};
    cx DirFileEnumCallbFnT callb = [](cx FileInfoCommon& fileInfo, GA param1, GA param2) -> BO {
        unused(param2);
        CH path[PATH_LENX_MAX];
        StrCpy(path, ((Param*)param1)->dst);
        cxex SI CH_PATH_DIR_CNT = 1;
        StrAdd(path, fileInfo.path.c_str() + fileInfo.pathRelOfs - CH_PATH_DIR_CNT);
        if (fileInfo.attrib & FILE_ATTRIBUTE_DIRECTORY)
        {
            ife (DirNew(path))
            {
                ((Param*)param1)->err = ErrLastGet();
                ret NO;
            }
        }
        else
        {
            ife (FileCpy(path, fileInfo.path.c_str(), ((Param*)param1)->isReplace))
            {
                ((Param*)param1)->err = ErrLastGet();
                ret NO;
            }
        }
        ret YES;
    };
    ifep(DirFileEnum(src, -1, callb, (GA)&param, NUL, NO));
    ifu (param.err != ErrVal::NONE)
        rete(param.err);
    rets;
}
dfa ER DirDel(cx CH* path)
{
    CH path_[PATH_LENX_MAX];
    PathToAbspath(path_, path);
    struct Param
    {
        ErrVal err;
    };
    Param param = {ErrVal::NONE};
    cx DirFileEnumCallbFnT callb = [](cx FileInfoCommon& fileInfo, GA param1, GA param2) -> BO {
        unused(param2);
        if (fileInfo.attrib & FILE_ATTRIBUTE_DIRECTORY)
        {
            ife (_DirDel(fileInfo.path.c_str()))
            {
                ((Param*)param1)->err = ErrLastGet();
                ret NO;
            }
        }
        else
        {
            ife (FileDel(fileInfo.path.c_str()))
            {
                ((Param*)param1)->err = ErrLastGet();
                ret NO;
            }
        }
        ret YES;
    };
    ifep(DirFileEnum(path_, -1, callb, (GA)&param, NUL, YES));
    ifep(_DirDel(path_));
    ifu (param.err != ErrVal::NONE)
        rete(param.err);
    rets;
}
dfa ER DirMove(cx CH* dst, cx CH* src, BO isReplace)
{
    ife (FileMove(dst, src, isReplace))
    {
        ifep(DirCpy(dst, src, isReplace));
        ifep(DirDel(src));
    }
    rets;
}

#endif
