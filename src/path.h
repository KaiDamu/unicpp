#pragma once

constexpr SI PATH_LEN_MAX = 260;

constexpr cx CH* NTPATH_PRE_STR = L"\\??\\";
constexpr SI NTPATH_PRE_STR_LEN = 4;

constexpr CH PATH_DIR_SEPARATOR = '\\';
constexpr CH PATH_EXT_SEPARATOR = '.';
constexpr CH PATH_DRIVE_SEPARATOR = ':';

constexpr cx CH* PATH_DIR_SEPARATOR_STR = L"\\";
constexpr cx CH* PATH_EXT_SEPARATOR_STR = L".";
constexpr cx CH* PATH_DRIVE_SEPARATOR_STR = L":";

dfa cx CH* PathExtPtr(cx CH* path) {
	cx CH* last = NUL;
	while (*path != '\0') {
		if (*path == PATH_DIR_SEPARATOR || *path == PATH_EXT_SEPARATOR) last = path;
		++path;
	}
	if (last == NUL) ret path;
	if (*last == PATH_DIR_SEPARATOR) ret path;
	ret last + 1;
}

dfa BO PathDirUp(CH* path) {
	CH* last = NUL;
	while (*path != '\0') {
		if (*path == PATH_DIR_SEPARATOR) last = path;
		++path;
	}
	if (last == NUL) ret NO;
	*last = '\0';
	ret YES;
}

dfa BO PathIsAbs(cx CH* path) {
	ret (path[0] != '\0') && (path[1] == PATH_DRIVE_SEPARATOR);
}
dfa BO PathIsNtpath(cx CH* path) {
	ret (MemCmp(path, NTPATH_PRE_STR, NTPATH_PRE_STR_LEN * siz(CH)) == 0);
}

dfa BO PathIsExist(cx CH* path) {
	ret GetFileAttributesW(path) != INVALID_FILE_ATTRIBUTES;
}
dfa BO PathIsDir(cx CH* path) {
	cx DWORD attrib = GetFileAttributesW(path);
	if (attrib == INVALID_FILE_ATTRIBUTES) ret NO;
	ret (attrib & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

dfa ER PathEnvRelToAbs(CH* out, cx CH* in) {
	out[0] = '\0';
	cx SI size = GetEnvironmentVariableW(L"PATH", NUL, 0);
	ifu (size == 0) rete(ERR_ENV);
	Arr<CH> buf(size);
	SI size2 = GetEnvironmentVariableW(L"PATH", buf.Ptr(), DWORD(size));
	if (size2 + 1 == size) ++size2; // weird Windows fix
	ifu (size != size2) rete(ERR_ENV);
	buf[size - 1] = ';';
	CH str[PATH_LEN_MAX];
	SI strLen = 0;
	ite (i, i < size) {
		cx CH c = buf[i];
		str[strLen++] = c;
		ifu (c == ';') {
			ifu (strLen < 2) {
				strLen = 0;
				continue;
			}
			if (str[strLen - 2] == '\\') {
				str[strLen - 1] = '\0';
				--strLen;
			} else {
				str[strLen - 1] = '\\';
				str[strLen] = '\0';
			}
			StrSet(str + strLen, in);
			if (PathIsExist(str)) {
				MemSet(out, str, (strLen + StrLen(in) + 1) * siz(CH));
				rets;
			}
			strLen = 0;
		}
	}
	rete(ERR_NO_EXIST);
}

dfa cx CH* ProcCurFilePathGet() {
	static CH s_cache[PATH_LEN_MAX] = {};
	ifu (s_cache[0] == '\0') {
		cx DWORD result = GetModuleFileNameW(NUL, s_cache, PATH_LEN_MAX);
		Assert(result > 0 && result < PATH_LEN_MAX);
		#ifdef PROG_BUILD_TYPE_RELEASE
			unused(result);
		#endif
	}
	ret s_cache;
}
dfa cx CH* ProcCurDirPathGet() {
	static CH s_cache[PATH_LEN_MAX] = {};
	ifu (s_cache[0] == '\0') {
		StrSet(s_cache, ProcCurFilePathGet());
		PathDirUp(s_cache);
	}
	ret s_cache;
}
dfa cx CH* ProcCurWorkPathGet() {
	static CH s_cache[PATH_LEN_MAX] = {};
	ifu (s_cache[0] == '\0') {
		cx DWORD result = GetCurrentDirectoryW(PATH_LEN_MAX, s_cache);
		Assert(result > 0 && result < PATH_LEN_MAX);
		#ifdef PROG_BUILD_TYPE_RELEASE
			unused(result);
		#endif
	}
	ret s_cache;
}

dfa BO PathToAbsByDirPath(CH* path) {
	if (PathIsAbs(path)) ret NO;
	cx SI pathLen = StrLen(path);
	cx SI dirPathLen = StrLen(ProcCurDirPathGet());
	SI i = pathLen;
	while (i > -1) {
		path[i + dirPathLen + 1] = path[i]; // +1 to simulate the extra PATH_DIR_SEPARATOR
		--i;
	}
	path[dirPathLen] = PATH_DIR_SEPARATOR;
	MemSet(path, ProcCurDirPathGet(), dirPathLen * siz(CH));
	ret YES;
}
dfa BO PathToAbsByWorkPath(CH* path) {
	if (PathIsAbs(path)) ret NO;
	cx SI pathLen = StrLen(path);
	cx SI workPathLen = StrLen(ProcCurWorkPathGet());
	SI i = pathLen;
	while (i > -1) {
		path[i + workPathLen + 1] = path[i]; // +1 to simulate the extra PATH_DIR_SEPARATOR
		--i;
	}
	path[workPathLen] = PATH_DIR_SEPARATOR;
	MemSet(path, ProcCurWorkPathGet(), workPathLen * siz(CH));
	ret YES;
}

dfa BO PathToNtpathByDirPath(CH* path) {
	if (PathIsNtpath(path)) ret NO;
	cx SI pathLen = StrLen(path);
	cx SI dirPathLen = StrLen(ProcCurDirPathGet());
	SI i = pathLen;
	while (i > -1) {
		path[i + dirPathLen + 1 + NTPATH_PRE_STR_LEN] = path[i]; // +1 to simulate the extra PATH_DIR_SEPARATOR
		--i;
	}
	path[dirPathLen + NTPATH_PRE_STR_LEN] = PATH_DIR_SEPARATOR;
	MemSet(path + NTPATH_PRE_STR_LEN, ProcCurDirPathGet(), dirPathLen * siz(CH));
	MemSet(path, NTPATH_PRE_STR, NTPATH_PRE_STR_LEN * siz(CH));
	ret YES;
}

constexpr CH PATH_SANITIZE_CHAR = '_';

dfa NT PathSanitize(CH* dst, cx CH* src) {
	while (*src != '\0') {
		if (*src == '\\' || *src == '/' || *src == ':' || *src == '*' || *src == '?' || *src == '\"' || *src == '<' || *src == '>' || *src == '|') *dst = PATH_SANITIZE_CHAR;
		else *dst = *src;
		++dst;
		++src;
	}
	*dst = '\0';
}

SI ProcCurEnvvarGet(CH*, cx CH*, SI);

constexpr CH PATH_ENVVAR_CHAR = '%';

dfa SI PathEnvvarResolve(CH* path) {
	CH* p = path;
	while (*p != '\0') {
		if (*p == PATH_ENVVAR_CHAR) {
			CH*cx pFirst = p;
			do {
				++p;
			} while (*p != PATH_ENVVAR_CHAR && *p != PATH_DIR_SEPARATOR && *p != '\0');
			if (*p == PATH_ENVVAR_CHAR) {
				CH buf[PATH_LEN_MAX + 1];
				*p = '\0';
				cx SI bufLen = ProcCurEnvvarGet(buf, pFirst + 1, PATH_LEN_MAX + 1);
				*p = PATH_ENVVAR_CHAR;
				if (bufLen > 0 && bufLen < PATH_LEN_MAX + 1) {
					StrReplace(pFirst, buf, 0, SI(p - pFirst) + 1);
					p = pFirst + bufLen;
				} else {
					++p;
				}
			} else if (*p != '\0') {
				++p;
			}
		} else {
			++p;
		}
	}
	ret p - path;
}
dfa SI PathEnvvarResolve(CH* dst, cx CH* src) {
	StrSet(dst, src);
	ret PathEnvvarResolve(dst);
}
