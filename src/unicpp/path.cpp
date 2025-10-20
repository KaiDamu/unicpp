#include "path.hpp"

#include "memory.hpp"
#include "array.hpp"
#include "process.hpp"
#include "string.hpp"
#include "file.hpp"

dfa cx CH* PathExtPtr(cx CH* path)
{
    cx CH* last = NUL;
    while (*path != '\0')
    {
        if (*path == CH_PATH_DIR || *path == ' ' || *path == CH_PATH_EXT)
            last = path;
        ++path;
    }
    if (last == NUL)
        ret path;
    if (*last == CH_PATH_DIR || *last == ' ')
        ret path;
    ret last + 1;
}
dfa cx CH* PathNamePtr(cx CH* path, SI& nameoLen)
{
    cx CH* cx pathBase = path;
    cx CH* lastDir = NUL;
    cx CH* lastExt = NUL;
    while (*path != '\0')
    {
        if (*path == CH_PATH_DIR)
            lastDir = path;
        else if (*path == CH_PATH_EXT)
            lastExt = path;
        else if (*path == ' ')
            lastExt = NUL;
        ++path;
    }
    cx CH* cx namePtr = (lastDir == NUL) ? pathBase : (lastDir + 1);
    nameoLen = (lastExt == NUL) ? (path - namePtr) : (lastExt - namePtr);
    ret namePtr;
}
dfa cx CH* PathNamePtr(cx CH* path)
{
    SI nameLen;
    ret PathNamePtr(path, nameLen);
}

dfa SI PathNameo(CH* dst, cx CH* src)
{
    SI nameLen;
    cx CH* cx namePtr = PathNamePtr(src, nameLen);
    MemCpy(dst, namePtr, nameLen * siz(CH));
    dst[nameLen] = '\0';
    ret nameLen;
}

dfa SI PathDirUp(CH* dst, BO* isRoot)
{
    cx AU last = const_cast<CH*>(StrFindLast(dst, CH_PATH_DIR));
    ifu (last == NUL)
    {
        if (isRoot != NUL)
            *isRoot = YES;
        ret StrLen(dst);
    }
    *last = '\0';
    if (isRoot != NUL)
        *isRoot = NO;
    ret last - dst;
}

dfa BO PathIsAbspath(cx CH* path)
{
    ret (path[0] != '\0' && path[1] == CH_PATH_DRIVE);
}
dfa BO PathIsNtpath(cx CH* path)
{
    ret StrHasPre(path, STR_NTPATH_PRE);
}

#ifdef PROG_SYS_WIN

dfa SI PathEnvRelToAbs(CH* dst, cx CH* src)
{
    dst[0] = '\0';
    cx SI size = GetEnvironmentVariableW(L"PATH", NUL, 0);
    ifu (size == 0)
        ret 0;
    Arr<CH> buf(size);
    SI size2 = GetEnvironmentVariableW(L"PATH", buf.Ptr(), DWORD(size));
    if (size2 + 1 == size)
        ++size2; // weird Windows fix
    ifu (size != size2)
        ret 0;
    buf[size - 1] = ';';
    CH str[PATH_LENX_MAX];
    SI strLen = 0;
    ite (i, i < size)
    {
        cx CH c = buf[i];
        str[strLen++] = c;
        ifu (c == ';')
        {
            ifu (strLen < 2)
            {
                strLen = 0;
                continue;
            }
            if (str[strLen - 2] == '\\')
            {
                str[strLen - 1] = '\0';
                --strLen;
            }
            else
            {
                str[strLen - 1] = '\\';
                str[strLen] = '\0';
            }
            StrCpy(str + strLen, src);
            if (FileIsExist(str, YES, YES))
            {
                cx SI dstLen = strLen + StrLen(src);
                MemCpy(dst, str, (dstLen + STR_EX_LEN) * siz(CH));
                ret dstLen;
            }
            strLen = 0;
        }
    }
    ret 0;
}

dfa SI PathToAbspath(CH* path)
{
    cx AU pathLen = StrLen(path);
    if (PathIsAbspath(path))
        ret pathLen;
    cx AU workPathLen = ProcWorkPathLen();
    cxex SI CH_PATH_DIR_ADD = 1;
    for (SI i = pathLen; i > -1; --i)
        path[i + workPathLen + CH_PATH_DIR_ADD] = path[i];
    ProcWorkPathGet(path);
    path[workPathLen] = CH_PATH_DIR;
    ret workPathLen + CH_PATH_DIR_ADD + pathLen;
}
dfa SI PathToAbspath(CH* dst, cx CH* src)
{
    if (PathIsAbspath(src))
        ret StrCpyStrLen(dst, src);
    cx AU workPathLen = ProcWorkPathGet(dst);
    dst[workPathLen] = CH_PATH_DIR;
    cxex SI CH_PATH_DIR_ADD = 1;
    cx AU srcLen = StrCpyStrLen(dst + workPathLen + CH_PATH_DIR_ADD, src);
    ret workPathLen + CH_PATH_DIR_ADD + srcLen;
}
dfa SI PathToNtpath(CH* path)
{
    if (PathIsNtpath(path))
        ret StrLen(path);
    cx AU pathLen = PathToAbspath(path);
    for (SI i = pathLen; i > -1; --i)
        path[i + STR_NTPATH_PRE_LEN] = path[i];
    MemCpy(path, STR_NTPATH_PRE, STR_NTPATH_PRE_SIZE);
    ret pathLen + STR_NTPATH_PRE_LEN;
}
dfa SI PathToNtpath(CH* dst, cx CH* src)
{
    if (PathIsNtpath(src))
        ret StrCpyStrLen(dst, src);
    cx AU pathLen = PathToAbspath(dst + STR_NTPATH_PRE_LEN, src);
    MemCpy(dst, STR_NTPATH_PRE, STR_NTPATH_PRE_SIZE);
    ret pathLen + STR_NTPATH_PRE_LEN;
}

dfa SI PathSanitize(CH* dst, cx CH* src)
{
    cx CH* base = dst;
    while (*src != '\0')
    {
        if (*src == '\\' || *src == '/' || *src == ':' || *src == '*' || *src == '?' || *src == '\"' || *src == '<' || *src == '>' || *src == '|')
            *dst = CH_PATH_SANITIZE;
        else
            *dst = *src;
        ++dst;
        ++src;
    }
    *dst = '\0';
    ret dst - base;
}

dfa SI PathEnvvarResolve(CH* path)
{
    CH* p = path;
    while (*p != '\0')
    {
        if (*p == CH_PATH_ENVVAR)
        {
            CH* cx pFirst = p;
            do
            {
                ++p;
            } while (*p != CH_PATH_ENVVAR && *p != CH_PATH_DIR && *p != '\0');
            if (*p == CH_PATH_ENVVAR)
            {
                CH buf[PATH_LENX_MAX];
                *p = '\0';
                cx SI bufLen = ProcEnvvarGet(buf, pFirst + 1, PATH_LENX_MAX);
                *p = CH_PATH_ENVVAR;
                if (bufLen > 0 && bufLen < PATH_LENX_MAX)
                {
                    StrReplace(pFirst, buf, 0, SI(p - pFirst) + 1);
                    p = pFirst + bufLen;
                }
                else
                {
                    ++p;
                }
            }
            else if (*p != '\0')
            {
                ++p;
            }
        }
        else
        {
            ++p;
        }
    }
    ret p - path;
}
dfa SI PathEnvvarResolve(CH* dst, cx CH* src)
{
    StrCpy(dst, src);
    ret PathEnvvarResolve(dst);
}

#endif
