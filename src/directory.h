#pragma once

constexpr U4 FILE_ATTRIB_DIR = 0x00000001;
constexpr U4 FILE_ATTRIB_HIDE = 0x00000002;
constexpr U4 FILE_ATTRIB_READ = 0x00000004;
constexpr U4 FILE_ATTRIB_SYS = 0x00000008;
constexpr U4 FILE_ATTRIB_TMP = 0x00000010;
constexpr U4 FILE_ATTRIB_ARCHIVE = 0x00000020;
constexpr U4 FILE_ATTRIB_COMPRESS = 0x00000040;

constexpr U4 DIR_ENUM_FLAG_POST = 0x00000001;

struct FileInfo {
	U8 createDate;
	U8 accessDate;
	U8 writeDate;
	U8 changeDate;
	U8 datSize;
	U8 allocSize;
	U4 index;
	U4 attrib;
	SI pathLen;
	cx CH* path;
	SI relLen;
	cx CH* rel;
	SI nameLen;
	cx CH* name;
};

using DirEnumCallbFnType = BO(*)(cx FileInfo& fileInfo, GA param1, GA param2); // ret: YES to continue, NO to stop

dfa ER _DirEnum(CH* path, SI pathLen, SI depth, DirEnumCallbFnType callb, U4 flags, GA param1, GA param2, SI pathLenBase) {
	if (depth == 0) rets;
	--depth;
	FileNt dir;
	ife (dir.Open(path, FILE_LIST_DIRECTORY | SYNCHRONIZE, 0, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, FILE_OPEN, FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT)) retepass;
	Arr<U1> buf(BYTE_IN_KB * 4); // maybe this should be configurable
	IO_STATUS_BLOCK isb;
	U4 queryFlags = 0;
	jdst(retry);
	cx NTSTATUS status = NtQueryDirectoryFileEx(dir.Hdl(), NUL, NUL, NUL, &isb, buf.Ptr(), buf.Cap(), FileDirectoryInformation, queryFlags, NUL);
	ifu (status == STATUS_BUFFER_OVERFLOW) {
		buf.Alloc(buf.Cap() * 2);
		queryFlags |= SL_RESTART_SCAN;
		jsrc(retry);
	}
	queryFlags &= ~SL_RESTART_SCAN;
	if (status == STATUS_NO_MORE_FILES) rets;
	if (status != STATUS_SUCCESS) rete(ERR_DIR);
	buf.CurRewind();
	while (YES) {
		cx FILE_DIRECTORY_INFORMATION*cx info = (FILE_DIRECTORY_INFORMATION*)(buf.Cur());
		cx BO isEx = ((info->FileNameLength == (1 * siz(CH))) && (MemCmp(info->FileName, L".", 1 * siz(CH)) == 0)) || ((info->FileNameLength == (2 * siz(CH))) && (MemCmp(info->FileName, L"..", 2 * siz(CH)) == 0));
		if (!isEx) {
			MemCpy(path + pathLen, info->FileName, info->FileNameLength);
			pathLen += info->FileNameLength / siz(CH);
			path[pathLen] = '\0';
			FileInfo fileInfo;
			fileInfo.createDate = TimeLdapToUnix(info->CreationTime.QuadPart);
			fileInfo.accessDate = TimeLdapToUnix(info->LastAccessTime.QuadPart);
			fileInfo.writeDate = TimeLdapToUnix(info->LastWriteTime.QuadPart);
			fileInfo.changeDate = TimeLdapToUnix(info->ChangeTime.QuadPart);
			fileInfo.datSize = info->EndOfFile.QuadPart;
			fileInfo.allocSize = info->AllocationSize.QuadPart;
			fileInfo.index = U8(info->FileIndex);
			fileInfo.attrib = 0;
			if (info->FileAttributes & FILE_ATTRIBUTE_DIRECTORY) fileInfo.attrib |= FILE_ATTRIB_DIR;
			if (info->FileAttributes & FILE_ATTRIBUTE_HIDDEN) fileInfo.attrib |= FILE_ATTRIB_HIDE;
			if (info->FileAttributes & FILE_ATTRIBUTE_READONLY) fileInfo.attrib |= FILE_ATTRIB_READ;
			if (info->FileAttributes & FILE_ATTRIBUTE_SYSTEM) fileInfo.attrib |= FILE_ATTRIB_SYS;
			if (info->FileAttributes & FILE_ATTRIBUTE_TEMPORARY) fileInfo.attrib |= FILE_ATTRIB_TMP;
			if (info->FileAttributes & FILE_ATTRIBUTE_ARCHIVE) fileInfo.attrib |= FILE_ATTRIB_ARCHIVE;
			if (info->FileAttributes & FILE_ATTRIBUTE_COMPRESSED) fileInfo.attrib |= FILE_ATTRIB_COMPRESS;
			fileInfo.pathLen = pathLen - NTPATH_PRE_STR_LEN;
			fileInfo.path = path + NTPATH_PRE_STR_LEN;
			fileInfo.relLen = fileInfo.pathLen - pathLenBase;
			fileInfo.rel = fileInfo.path + pathLenBase;
			fileInfo.nameLen = info->FileNameLength / siz(CH);
			fileInfo.name = path + pathLen - fileInfo.nameLen;
			if ((flags & DIR_ENUM_FLAG_POST) == 0) ifu (callb(fileInfo, param1, param2) == NO) rets;
			if (fileInfo.attrib & FILE_ATTRIB_DIR) {
				path[pathLen++] = PATH_DIR_SEPARATOR;
				path[pathLen] = '\0';
				ife (_DirEnum(path, pathLen, depth, callb, flags, param1, param2, pathLenBase)) retepass;
				path[--pathLen] = '\0';
			}
			if ((flags & DIR_ENUM_FLAG_POST) != 0) ifu (callb(fileInfo, param1, param2) == NO) rets;
			pathLen -= info->FileNameLength / siz(CH);
			path[pathLen] = '\0';
		}
		if (info->NextEntryOffset == 0) break;
		buf.CurMove(info->NextEntryOffset);
	}
	buf.Alloc(buf.Cap() * 2);
	jsrc(retry);
}

dfa ER DirEnum(cx CH* path, SI depth, DirEnumCallbFnType callb, U4 flags, GA param1, GA param2) {
	CH path2[PATH_LEN_MAX];
	CH path3[PATH_LEN_MAX];
	StrCpy(path2, path);
	PathToAbsByWorkPath(path2);
	cx SI pathLen = StrEnclose(path3, path2, NTPATH_PRE_STR, PATH_DIR_SEPARATOR_STR);
	ret _DirEnum(path3, pathLen, depth, callb, flags, param1, param2, pathLen - NTPATH_PRE_STR_LEN);
}
dfa ER DirEnum(cx CH* path, SI depth, DirEnumCallbFnType callb, U4 flags) {
	ret DirEnum(path, depth, callb, flags, NUL, NUL);
}

dfa ER _DirDel(cx CH* path) {
	ifu (RemoveDirectoryW(path) == 0) rete(ERR_DIR);
	rets;
}

dfa ER DirCreate(cx CH* path) {
	ifu (CreateDirectoryW(path, NUL) == 0) {
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			if (PathIsDir(path)) rets;
			rete(ERR_DIR);
		}
		CH path_[PATH_LEN_MAX];
		StrCpy(path_, path);
		CH* pathSep = (CH*)StrFindLast(path_, PATH_DIR_SEPARATOR);
		ifu (pathSep == NUL) rete(ERR_DIR);
		*pathSep = '\0';
		ife (DirCreate(path_)) retepass;
		ifu (CreateDirectoryW(path, NUL) == 0) rete(ERR_DIR);
	}
	rets;
}
dfa ER DirCpy(cx CH* dst, cx CH* src, BO isReplace = YES) {
	CH dst_[PATH_LEN_MAX];
	StrCpy(dst_, dst);
	PathToAbsByWorkPath(dst_);
	ife (DirCreate(dst_)) retepass;
	struct Param {
		CH* dst;
		BO isReplace;
		ErrVal err;
	};
	Param param = {dst_, isReplace, ERR_NONE};
	ife (DirEnum(src, -1, [](cx FileInfo& fileInfo, GA param1, GA param2) {
		unused(param2);
		CH path[PATH_LEN_MAX];
		StrCpy(path, ((Param*)param1)->dst);
		StrAdd(path, fileInfo.rel - 1); // hack: -1 for PATH_DIR_SEPARATOR
		if (fileInfo.attrib & FILE_ATTRIB_DIR) {
			ife (DirCreate(path)) {
				((Param*)param1)->err = ErrLastValGet();
				ret NO;
			}
		} else {
			ife (FileCpy(path, fileInfo.path, ((Param*)param1)->isReplace)) {
				((Param*)param1)->err = ErrLastValGet();
				ret NO;
			}
		}
		ret YES;
	}, 0, (GA)&param, NUL)) retepass;
	ifu (param.err != ERR_NONE) rete(param.err);
	rets;
}
dfa ER DirDel(cx CH* path) {
	CH path_[PATH_LEN_MAX];
	StrCpy(path_, path);
	PathToAbsByWorkPath(path_);
	struct Param {
		ErrVal err;
	};
	Param param = {ERR_NONE};
	ife (DirEnum(path_, -1, [](cx FileInfo& fileInfo, GA param1, GA param2) {
		unused(param2);
		if (fileInfo.attrib & FILE_ATTRIB_DIR) {
			ife (_DirDel(fileInfo.path)) {
				((Param*)param1)->err = ErrLastValGet();
				ret NO;
			}
		} else {
			ife (FileDel(fileInfo.path)) {
				((Param*)param1)->err = ErrLastValGet();
				ret NO;
			}
		}
		ret YES;
	}, DIR_ENUM_FLAG_POST, (GA)&param, NUL)) retepass;
	ife (_DirDel(path_)) retepass;
	ifu (param.err != ERR_NONE) rete(param.err);
	rets;
}
dfa ER DirMove(cx CH* dst, cx CH* src, BO isReplace = YES) {
	ife (FileMove(dst, src, isReplace)) {
		ife (DirCpy(dst, src, isReplace)) retepass;
		ife (DirDel(src)) retepass;
	}
	rets;
}
