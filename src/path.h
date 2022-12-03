#pragma once

constexpr SI PATH_LEN_MAX = 260;

constexpr cx CH* NTPATH_PRE_STR = L"\\??\\";
constexpr SI NTPATH_PRE_STR_LEN = 4;

constexpr CH PATH_DIR_SEPARATOR = '\\';
constexpr CH PATH_EXT_SEPARATOR = '.';
constexpr CH PATH_DRIVE_SEPARATOR = ':';

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

dfa ER PathEnvRelToAbs(CH* out, cx CH* in) {
	out[0] = '\0';
	cx SI size = GetEnvironmentVariableW(L"PATH", NUL, 0);
	ifu (size == 0) rete(ERR_ENV);
	Arr<CH> buf(size);
	SI size2 = GetEnvironmentVariableW(L"PATH", buf.Ptr(), size);
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
			ChstrCpy(str + strLen, in);
			if (PathIsExist(str)) {
				MemCpy(out, str, (strLen + ChstrLen(in) + 1) * siz(CH));
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
		ChstrCpy(s_cache, ProcCurFilePathGet());
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
	cx SI pathLen = ChstrLen(path);
	cx SI dirPathLen = ChstrLen(ProcCurDirPathGet());
	SI i = pathLen;
	while (i > -1) {
		path[i + dirPathLen + 1] = path[i]; // +1 to simulate the extra PATH_DIR_SEPARATOR
		--i;
	}
	path[dirPathLen] = PATH_DIR_SEPARATOR;
	MemCpy(path, ProcCurDirPathGet(), dirPathLen * siz(CH));
	ret YES;
}
dfa BO PathToAbsByWorkPath(CH* path) {
	if (PathIsAbs(path)) ret NO;
	cx SI pathLen = ChstrLen(path);
	cx SI workPathLen = ChstrLen(ProcCurWorkPathGet());
	SI i = pathLen;
	while (i > -1) {
		path[i + workPathLen + 1] = path[i]; // +1 to simulate the extra PATH_DIR_SEPARATOR
		--i;
	}
	path[workPathLen] = PATH_DIR_SEPARATOR;
	MemCpy(path, ProcCurWorkPathGet(), workPathLen * siz(CH));
	ret YES;
}

dfa BO PathToNtpathByDirPath(CH* path) {
	if (PathIsNtpath(path)) ret NO;
	cx SI pathLen = ChstrLen(path);
	cx SI dirPathLen = ChstrLen(ProcCurDirPathGet());
	SI i = pathLen;
	while (i > -1) {
		path[i + dirPathLen + 1 + NTPATH_PRE_STR_LEN] = path[i]; // +1 to simulate the extra PATH_DIR_SEPARATOR
		--i;
	}
	path[dirPathLen + NTPATH_PRE_STR_LEN] = PATH_DIR_SEPARATOR;
	MemCpy(path + NTPATH_PRE_STR_LEN, ProcCurDirPathGet(), dirPathLen * siz(CH));
	MemCpy(path, NTPATH_PRE_STR, NTPATH_PRE_STR_LEN * siz(CH));
	ret YES;
}
