#pragma once

constexpr SI PATH_LEN_MAX = 260;
constexpr SI PATH_LENX_MAX = PATH_LEN_MAX + STR_EX_LEN;

constexpr CH CH_PATH_DIR = '\\';
constexpr CH CH_PATH_EXT = '.';
constexpr CH CH_PATH_DRIVE = ':';

constexpr cx CH* STR_PATH_DIR = L"\\";
constexpr cx CH* STR_PATH_EXT = L".";
constexpr cx CH* STR_PATH_DRIVE = L":";

constexpr CH CH_PATH_SANITIZE = '_';
constexpr CH CH_PATH_ENVVAR = '%';

constexpr cx CH* STR_NTPATH_PRE = L"\\??\\";
constexpr SI STR_NTPATH_PRE_LEN = 4;

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

dfa SI PathDirUp(CH* dst, BO* isRoot = NUL)
{
    CH* last = (CH*)StrFindLast(dst, CH_PATH_DIR);
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

dfa BO PathIsAbs(cx CH* path)
{
    ret (path[0] != '\0' && path[1] == CH_PATH_DRIVE);
}
dfa BO PathIsNtpath(cx CH* path)
{
    ret StrIsFirst(path, STR_NTPATH_PRE);
}

dfa BO PathIsExist(cx CH* path)
{
    ret GetFileAttributesW(path) != INVALID_FILE_ATTRIBUTES;
}
dfa BO PathIsDir(cx CH* path)
{
    cx DWORD attrib = GetFileAttributesW(path);
    if (attrib == INVALID_FILE_ATTRIBUTES)
        ret NO;
    ret (attrib & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

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
    CH str[PATH_LEN_MAX];
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
            if (PathIsExist(str))
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

dfa cx CH* ProcFilePath()
{
    static CH s_cache[PATH_LEN_MAX] = {};
    ifu (s_cache[0] == '\0')
    {
        cx DWORD result = GetModuleFileNameW(NUL, s_cache, PATH_LEN_MAX);
        Assert(result > 0 && result < PATH_LEN_MAX);
#ifdef PROG_BUILD_TYPE_RELEASE
        unused(result);
#endif
    }
    ret s_cache;
}
dfa cx CH* ProcDirPath()
{
    static CH s_cache[PATH_LEN_MAX] = {};
    ifu (s_cache[0] == '\0')
    {
        StrCpy(s_cache, ProcFilePath());
        PathDirUp(s_cache);
    }
    ret s_cache;
}
dfa cx CH* ProcWorkPath()
{
    static CH s_cache[PATH_LEN_MAX] = {};
    ifu (s_cache[0] == '\0')
    {
        cx DWORD result = GetCurrentDirectoryW(PATH_LEN_MAX, s_cache);
        Assert(result > 0 && result < PATH_LEN_MAX);
#ifdef PROG_BUILD_TYPE_RELEASE
        unused(result);
#endif
    }
    ret s_cache;
}

dfa SI PathToAbs(CH* path)
{
    if (PathIsAbs(path))
        ret StrLen(path);
    cx SI pathLen = StrLen(path);
    cx SI workPathLen = StrLen(ProcWorkPath());
    SI i = pathLen;
    while (i > -1)
    {
        path[i + workPathLen + 1] = path[i]; // +1 to simulate the extra CH_PATH_DIR
        --i;
    }
    path[workPathLen] = CH_PATH_DIR;
    MemCpy(path, ProcWorkPath(), workPathLen * siz(CH));
    ret pathLen + workPathLen + 1; // +1 for the CH_PATH_DIR
}
dfa SI PathToAbs(CH* dst, cx CH* src)
{
    if (PathIsAbs(src))
        ret StrSetLenGet(dst, src);
    cx SI workPathLen = StrSetLenGet(dst, ProcWorkPath());
    dst[workPathLen] = CH_PATH_DIR;
    ret StrSetLenGet(dst + workPathLen + 1, src) + workPathLen + 1; // +1 for the CH_PATH_DIR
}

dfa SI PathToNtpath(CH* path)
{
    if (PathIsNtpath(path))
        ret StrLen(path);
    cx SI pathLen = PathToAbs(path);
    SI i = pathLen;
    while (i > -1)
    {
        path[i + STR_NTPATH_PRE_LEN] = path[i];
        --i;
    }
    MemCpy(path, STR_NTPATH_PRE, STR_NTPATH_PRE_LEN * siz(CH));
    ret pathLen + STR_NTPATH_PRE_LEN;
}
dfa SI PathToNtpath(CH* dst, cx CH* src)
{
    if (PathIsNtpath(src))
        ret StrSetLenGet(dst, src);
    cx SI pathLen = PathToAbs(dst + STR_NTPATH_PRE_LEN, src);
    MemCpy(dst, STR_NTPATH_PRE, STR_NTPATH_PRE_LEN * siz(CH));
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

dfa SI ProcEnvvarGet(CH*, cx CH*, SI);

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
                CH buf[PATH_LEN_MAX + 1];
                *p = '\0';
                cx SI bufLen = ProcEnvvarGet(buf, pFirst + 1, PATH_LEN_MAX + 1);
                *p = CH_PATH_ENVVAR;
                if (bufLen > 0 && bufLen < PATH_LEN_MAX + 1)
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
