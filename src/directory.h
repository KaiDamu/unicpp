#pragma once

constexpr U4 FILE_ATTRIB_DIR = 0x00000001;
constexpr U4 FILE_ATTRIB_HIDE = 0x00000002;
constexpr U4 FILE_ATTRIB_READ = 0x00000004;
constexpr U4 FILE_ATTRIB_SYS = 0x00000008;
constexpr U4 FILE_ATTRIB_TMP = 0x00000010;
constexpr U4 FILE_ATTRIB_ARCHIVE = 0x00000020;
constexpr U4 FILE_ATTRIB_COMPRESS = 0x00000040;

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
};

using DirEnumCallbFnType = BO(*)(cx FileInfo& fileInfo, GA param1, GA param2); // ret: YES to continue, NO to stop

dfa ER _DirEnum(CH* path, SI pathLen, SI depth, DirEnumCallbFnType callb, GA param1, GA param2) {
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
			fileInfo.createDate = info->CreationTime.QuadPart;
			fileInfo.accessDate = info->LastAccessTime.QuadPart;
			fileInfo.writeDate = info->LastWriteTime.QuadPart;
			fileInfo.changeDate = info->ChangeTime.QuadPart;
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
			ifu (callb(fileInfo, param1, param2) == NO) rets;
			if (fileInfo.attrib & FILE_ATTRIB_DIR) {
				path[pathLen++] = PATH_DIR_SEPARATOR;
				path[pathLen] = '\0';
				ife (_DirEnum(path, pathLen, depth, callb, param1, param2)) retepass;
				path[--pathLen] = '\0';
			}
			pathLen -= info->FileNameLength / siz(CH);
			path[pathLen] = '\0';
		}
		if (info->NextEntryOffset == 0) break;
		buf.CurMove(info->NextEntryOffset);
	}
	buf.Alloc(buf.Cap() * 2);
	jsrc(retry);
}

dfa ER DirEnum(cx CH* path, SI depth, DirEnumCallbFnType callb, GA param1, GA param2) {
	CH path_[PATH_LEN_MAX];
	cx SI pathLen = ChstrEnclose(path_, path, NTPATH_PRE_STR, PATH_DIR_SEPARATOR_STR);
	ret _DirEnum(path_, pathLen, depth, callb, param1, param2);
}
