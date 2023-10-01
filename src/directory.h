#pragma once

constexpr U4 FILE_ATTRIB_DIR = 0x00000001;
constexpr U4 FILE_ATTRIB_HIDE = 0x00000002;
constexpr U4 FILE_ATTRIB_READ = 0x00000004;
constexpr U4 FILE_ATTRIB_SYS = 0x00000008;
constexpr U4 FILE_ATTRIB_TMP = 0x00000010;
constexpr U4 FILE_ATTRIB_ARCHIVE = 0x00000020;
constexpr U4 FILE_ATTRIB_COMPRESS = 0x00000040;

constexpr U4 DIR_ENUM_FLAG_POST = 0x00000001;

class FileInfo {
public:
	U8 createDate;
	U8 accessDate;
	U8 writeDate;
	U8 changeDate;
	SI datSize;
	SI allocSize;
	U4 index;
	U4 attrib;
	SI pathLen;
	cx CH* path;
	SI relLen;
	cx CH* rel;
	SI nameLen;
	cx CH* name;
	SI extLen;
	cx CH* ext;
	SI bufSize;
	CH* buf;
public:
	dfa NT Save() {
		if (buf != NUL) delete[] buf;
		bufSize = (pathLen + 1) * siz(CH);
		buf = new CH[bufSize / siz(CH)];
		cx SI bufOfs = buf - path;
		MemSet(buf, path, pathLen * siz(CH));
		buf[pathLen] = '\0';
		ifl (path != NUL) path += bufOfs;
		ifl (rel != NUL) rel += bufOfs;
		ifl (name != NUL) name += bufOfs;
		ifl (ext != NUL) ext += bufOfs;
	}
public:
	dfa FileInfo() {
		MemSetVal(tx, 0, siz(*tx));
	}
	dfa ~FileInfo() {
		if (buf != NUL) delete[] buf;
	}
};

using DirEnumCallbFnType = BO(*)(cx FileInfo& fileInfo, GA param1, GA param2); // ret: YES to continue, NO to stop

dfa ER _DirEnum(CH* path, SI pathLen, SI depth, DirEnumCallbFnType callb, U4 flags, GA param1, GA param2, SI pathLenBase) {
	if (depth == 0) rets;
	--depth;
	FileNt dir;
	ife (dir.Open(path, FILE_LIST_DIRECTORY | SYNCHRONIZE, 0, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, FILE_OPEN, FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT)) retep;
	Arr<U1> buf(BYTE_IN_KB * 4); // maybe this should be configurable
	IO_STATUS_BLOCK isb;
	U4 queryFlags = 0;
	jdst(retry);
	cx NTSTATUS status = NtQueryDirectoryFileEx(dir.Hdl(), NUL, NUL, NUL, &isb, buf.Ptr(), ULONG(buf.Cap()), FileDirectoryInformation, queryFlags, NUL);
	ifu (status == STATUS_BUFFER_OVERFLOW) {
		buf.New(buf.Cap() * 2);
		queryFlags |= SL_RESTART_SCAN;
		jsrc(retry);
	}
	queryFlags &= ~SL_RESTART_SCAN;
	if (status == STATUS_NO_MORE_FILES) rets;
	if (status != STATUS_SUCCESS) rete(ERR_DIR);
	buf.CurClr();
	while (YES) {
		cx FILE_DIRECTORY_INFORMATION*cx info = (FILE_DIRECTORY_INFORMATION*)(buf.Cur());
		cx BO isEx = ((info->FileNameLength == (1 * siz(CH))) && (MemCmp(info->FileName, L".", 1 * siz(CH)) == 0)) || ((info->FileNameLength == (2 * siz(CH))) && (MemCmp(info->FileName, L"..", 2 * siz(CH)) == 0));
		if (!isEx) {
			MemSet(path + pathLen, info->FileName, info->FileNameLength);
			pathLen += info->FileNameLength / siz(CH);
			path[pathLen] = '\0';
			FileInfo fileInfo;
			fileInfo.createDate = LdapToUnix(info->CreationTime.QuadPart);
			fileInfo.accessDate = LdapToUnix(info->LastAccessTime.QuadPart);
			fileInfo.writeDate = LdapToUnix(info->LastWriteTime.QuadPart);
			fileInfo.changeDate = LdapToUnix(info->ChangeTime.QuadPart);
			fileInfo.datSize = SI(info->EndOfFile.QuadPart);
			fileInfo.allocSize = SI(info->AllocationSize.QuadPart);
			fileInfo.index = U8(info->FileIndex);
			fileInfo.attrib = 0;
			if (info->FileAttributes & FILE_ATTRIBUTE_DIRECTORY) fileInfo.attrib |= FILE_ATTRIB_DIR;
			if (info->FileAttributes & FILE_ATTRIBUTE_HIDDEN) fileInfo.attrib |= FILE_ATTRIB_HIDE;
			if (info->FileAttributes & FILE_ATTRIBUTE_READONLY) fileInfo.attrib |= FILE_ATTRIB_READ;
			if (info->FileAttributes & FILE_ATTRIBUTE_SYSTEM) fileInfo.attrib |= FILE_ATTRIB_SYS;
			if (info->FileAttributes & FILE_ATTRIBUTE_TEMPORARY) fileInfo.attrib |= FILE_ATTRIB_TMP;
			if (info->FileAttributes & FILE_ATTRIBUTE_ARCHIVE) fileInfo.attrib |= FILE_ATTRIB_ARCHIVE;
			if (info->FileAttributes & FILE_ATTRIBUTE_COMPRESSED) fileInfo.attrib |= FILE_ATTRIB_COMPRESS;
			fileInfo.pathLen = pathLen - STR_NTPATH_PRE_LEN;
			fileInfo.path = path + STR_NTPATH_PRE_LEN;
			fileInfo.relLen = fileInfo.pathLen - pathLenBase;
			fileInfo.rel = fileInfo.path + pathLenBase;
			fileInfo.nameLen = info->FileNameLength / siz(CH);
			fileInfo.name = path + pathLen - fileInfo.nameLen;
			fileInfo.extLen = 0;
			fileInfo.ext = NUL;
			for (SI i = fileInfo.nameLen - 1; i >= 0; --i) {
				if (fileInfo.name[i] == CH_PATH_EXT) {
					fileInfo.extLen = fileInfo.nameLen - i - 1;
					fileInfo.ext = fileInfo.name + i + 1;
					break;
				}
			}
			if ((flags & DIR_ENUM_FLAG_POST) == 0) ifu (callb(fileInfo, param1, param2) == NO) rets;
			if (fileInfo.attrib & FILE_ATTRIB_DIR) {
				path[pathLen++] = CH_PATH_DIR;
				path[pathLen] = '\0';
				ife (_DirEnum(path, pathLen, depth, callb, flags, param1, param2, pathLenBase)) retep;
				path[--pathLen] = '\0';
			}
			if ((flags & DIR_ENUM_FLAG_POST) != 0) ifu (callb(fileInfo, param1, param2) == NO) rets;
			pathLen -= info->FileNameLength / siz(CH);
			path[pathLen] = '\0';
		}
		if (info->NextEntryOffset == 0) break;
		buf.CurMove(info->NextEntryOffset);
	}
	buf.New(buf.Cap() * 2);
	jsrc(retry);
}

dfa ER DirEnum(cx CH* path, SI depth, DirEnumCallbFnType callb, U4 flags, GA param1, GA param2) {
	CH path2[PATH_LEN_MAX];
	CH path3[PATH_LEN_MAX];
	PathToAbs(path2, path);
	cx SI pathLen = StrEnclose(path3, path2, STR_NTPATH_PRE, STR_PATH_DIR);
	ret _DirEnum(path3, pathLen, depth, callb, flags, param1, param2, pathLen - STR_NTPATH_PRE_LEN);
}
dfa ER DirEnum(cx CH* path, SI depth, DirEnumCallbFnType callb, U4 flags) {
	ret DirEnum(path, depth, callb, flags, NUL, NUL);
}

dfa ER _DirDel(cx CH* path) {
	ifu (RemoveDirectoryW(path) == 0) rete(ERR_DIR);
	rets;
}

dfa ER DirNew(cx CH* path) {
	ifu (CreateDirectoryW(path, NUL) == 0) {
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			if (PathIsDir(path)) rets;
			rete(ERR_DIR);
		}
		CH path_[PATH_LEN_MAX];
		StrSet(path_, path);
		CH* pathSep = (CH*)StrFindLast(path_, CH_PATH_DIR);
		ifu (pathSep == NUL) rete(ERR_DIR);
		*pathSep = '\0';
		ife (DirNew(path_)) retep;
		ifu (CreateDirectoryW(path, NUL) == 0) rete(ERR_DIR);
	}
	rets;
}
dfa ER DirCpy(cx CH* dst, cx CH* src, BO isReplace = YES) {
	CH dst_[PATH_LEN_MAX];
	PathToAbs(dst_, dst);
	ife (DirNew(dst_)) retep;
	struct Param {
		CH* dst;
		BO isReplace;
		ErrVal err;
	};
	Param param = {dst_, isReplace, ERR_NONE};
	ife (DirEnum(src, -1, [](cx FileInfo& fileInfo, GA param1, GA param2) {
		unused(param2);
		CH path[PATH_LEN_MAX];
		StrSet(path, ((Param*)param1)->dst);
		StrAdd(path, fileInfo.rel - 1); // hack: -1 for CH_PATH_DIR
		if (fileInfo.attrib & FILE_ATTRIB_DIR) {
			ife (DirNew(path)) {
				((Param*)param1)->err = ErrLastGet();
				ret NO;
			}
		} else {
			ife (FileCpy(path, fileInfo.path, ((Param*)param1)->isReplace)) {
				((Param*)param1)->err = ErrLastGet();
				ret NO;
			}
		}
		ret YES;
	}, 0, (GA)&param, NUL)) retep;
	ifu (param.err != ERR_NONE) rete(param.err);
	rets;
}
dfa ER DirDel(cx CH* path) {
	CH path_[PATH_LEN_MAX];
	PathToAbs(path_, path);
	struct Param {
		ErrVal err;
	};
	Param param = {ERR_NONE};
	ife (DirEnum(path_, -1, [](cx FileInfo& fileInfo, GA param1, GA param2) {
		unused(param2);
		if (fileInfo.attrib & FILE_ATTRIB_DIR) {
			ife (_DirDel(fileInfo.path)) {
				((Param*)param1)->err = ErrLastGet();
				ret NO;
			}
		} else {
			ife (FileDel(fileInfo.path)) {
				((Param*)param1)->err = ErrLastGet();
				ret NO;
			}
		}
		ret YES;
	}, DIR_ENUM_FLAG_POST, (GA)&param, NUL)) retep;
	ife (_DirDel(path_)) retep;
	ifu (param.err != ERR_NONE) rete(param.err);
	rets;
}
dfa ER DirMove(cx CH* dst, cx CH* src, BO isReplace = YES) {
	ife (FileMove(dst, src, isReplace)) {
		ife (DirCpy(dst, src, isReplace)) retep;
		ife (DirDel(src)) retep;
	}
	rets;
}

dfa ER DirList(list<FileInfo>& files, cx CH* path, SI depth = -1) {
	files.clear();
	cx DirEnumCallbFnType callb = [](cx FileInfo& fileInfo, GA param1, GA param2) {
		unused(param2);
		((list<FileInfo>*)param1)->emplace_back(fileInfo);
		((list<FileInfo>*)param1)->back().Save();
		ret YES;
	};
	ife (DirEnum(path, depth, callb, 0, &files, NUL)) retep;
	rets;
}
