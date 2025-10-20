#pragma once

#include "type.hpp"

cxex SI PATH_LEN_MAX = 260;

#include "string.hpp"

cxex SI PATH_LENX_MAX = PATH_LEN_MAX + STR_EX_LEN;

cxex CH CH_PATH_DIR = '\\';
cxex CH CH_PATH_EXT = '.';
cxex CH CH_PATH_DRIVE = ':';

cxex cx CH* STR_PATH_DIR = L"\\";
cxex cx CH* STR_PATH_EXT = L".";
cxex cx CH* STR_PATH_DRIVE = L":";

cxex CH CH_PATH_SANITIZE = '_';
cxex CH CH_PATH_ENVVAR = '%';

cxex cx CH* STR_NTPATH_PRE = L"\\??\\";
cxex SI STR_NTPATH_PRE_LEN = 4;
cxex SI STR_NTPATH_PRE_SIZE = STR_NTPATH_PRE_LEN * siz(CH);

dfa cx CH* PathExtPtr(cx CH* path);
dfa cx CH* PathNamePtr(cx CH* path, SI& nameoLen);
dfa cx CH* PathNamePtr(cx CH* path);

dfa SI PathNameo(CH* dst, cx CH* src);

dfa SI PathDirUp(CH* dst, BO* isRoot = NUL);

dfa BO PathIsAbspath(cx CH* path);
dfa BO PathIsNtpath(cx CH* path);

#ifdef PROG_SYS_WIN

dfa SI PathEnvRelToAbs(CH* dst, cx CH* src);

dfa SI PathToAbspath(CH* path);
dfa SI PathToAbspath(CH* dst, cx CH* src);
dfa SI PathToNtpath(CH* path);
dfa SI PathToNtpath(CH* dst, cx CH* src);

dfa SI PathSanitize(CH* dst, cx CH* src);

dfa SI PathEnvvarResolve(CH* path);
dfa SI PathEnvvarResolve(CH* dst, cx CH* src);

#endif

#include "path.ipp"
