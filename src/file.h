#pragma once

dfa ER FileCpy(cx CH* dst, cx CH* src, BO isReplace = YES) {
	ifu (CopyFileW(src, dst, !isReplace) == 0) rete(ERR_FILE);
	rets;
}
dfa ER FileDel(cx CH* path) {
	ifu (DeleteFileW(path) == 0) rete(ERR_FILE);
	rets;
}
dfa ER FileMove(cx CH* dst, cx CH* src, BO isReplace = YES) {
	ifu (MoveFileExW(src, dst, MOVEFILE_COPY_ALLOWED | (isReplace ? MOVEFILE_REPLACE_EXISTING : 0)) == 0) rete(ERR_FILE);
	rets;
}

class FileWin {
private:
	HANDLE m_hdl;
public:
	dfa HANDLE Hdl() {
		ret m_hdl;
	}
	dfa BO IsOpen() {
		ret m_hdl != INVALID_HANDLE_VALUE;
	}
	dfa ER CurMove(SI cnt) {
		ifu (tx->IsOpen() == NO) rete(ERR_NO_INIT);
		LARGE_INTEGER tmp;
		tmp.QuadPart = cnt;
		if (SetFilePointerEx(m_hdl, tmp, NUL, FILE_CURRENT) == 0) rete(ERR_FILE);
		rets;
	}
	dfa ER CurSet(SI pos) {
		ifu (tx->IsOpen() == NO) rete(ERR_NO_INIT);
		LARGE_INTEGER tmp;
		tmp.QuadPart = pos;
		if (SetFilePointerEx(m_hdl, tmp, NUL, FILE_BEGIN) == 0) rete(ERR_FILE);
		rets;
	}
	dfa ER Open(cx CH* path, DWORD access, DWORD share, DWORD openMode, DWORD attrib) {
		ifu (tx->IsOpen() == YES) rete(ERR_YES_INIT);
		m_hdl = CreateFileW(path, access, share, NUL, openMode, attrib, NUL);
		ifu (tx->IsOpen() == NO) rete(ERR_FILE);
		rets;
	}
	dfa ER OpenRead(cx CH* path) {
		ret tx->Open(path, FILE_READ_DATA, FILE_SHARE_READ, OPEN_EXISTING, 0);
	}
	dfa ER OpenWrite(cx CH* path) {
		ret tx->Open(path, FILE_WRITE_DATA, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL);
	}
	dfa ER OpenReadWrite(cx CH* path) {
		ret tx->Open(path, FILE_READ_DATA | FILE_WRITE_DATA, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL);
	}
	dfa ER Close() {
		if (tx->IsOpen() == NO) rets;
		ifu (CloseHandle(m_hdl) == 0) rete(ERR_FILE);
		m_hdl = INVALID_HANDLE_VALUE;
		rets;
	}
	dfa ER Read(GA buf, SI size, SI& result) {
		result = 0;
		ifu (tx->IsOpen() == NO) rete(ERR_NO_INIT);
		DWORD tmp;
		ifu (ReadFile(m_hdl, buf, size, &tmp, NUL) == 0) rete(ERR_FILE);
		result = tmp;
		rets;
	}
	dfa ER Write(CXGA buf, SI size, SI& result) {
		result = 0;
		ifu (tx->IsOpen() == NO) rete(ERR_NO_INIT);
		DWORD tmp;
		ifu (WriteFile(m_hdl, buf, size, &tmp, NUL) == 0) rete(ERR_FILE);
		result = tmp;
		rets;
	}
	dfa ER Read(GA buf, SI size) {
		SI result;
		ife (tx->Read(buf, size, result)) retep;
		ifu (size != result) rete(ERR_NO_FULL);
		rets;
	}
	dfa ER Write(CXGA buf, SI size) {
		SI result;
		ife (tx->Write(buf, size, result)) retep;
		ifu (size != result) rete(ERR_NO_FULL);
		rets;
	}
	dfa ER SizeGet(SI& size) {
		size = 0;
		ifu (tx->IsOpen() == NO) rete(ERR_NO_INIT);
		LARGE_INTEGER tmp;
		ifu (GetFileSizeEx(m_hdl, &tmp) == 0) rete(ERR_FILE);
		size = tmp.QuadPart;
		rets;
	}
public:
	dfa FileWin() {
		m_hdl = INVALID_HANDLE_VALUE;
	}
	dfa ~FileWin() {
		tx->Close();
	}
};

class FileNt {
private:
	HANDLE m_hdl;
public:
	dfa HANDLE Hdl() {
		ret m_hdl;
	}
	dfa BO IsOpen() {
		ret m_hdl != INVALID_HANDLE_VALUE;
	}
	/*dfa ER CurMove(SI cnt) {
		rete(ERR_UNKNOWN);
	}*/
	dfa ER CurSet(SI pos) {
		ifu (tx->IsOpen() == NO) rete(ERR_NO_INIT);
		IO_STATUS_BLOCK iosb;
		FILE_POSITION_INFORMATION info;
		info.CurrentByteOffset.QuadPart = pos;
		cx NTSTATUS status = NtSetInformationFile(m_hdl, &iosb, &info, siz(FILE_POSITION_INFORMATION), FilePositionInformation);
		ifu (status != STATUS_SUCCESS) rete(ERR_FILE);
		rets;
	}
	dfa ER Open(cx CH* path, ACCESS_MASK access, SI allocSize, ULONG attrib, ULONG share, ULONG openMode, ULONG flags) {
		ifu (tx->IsOpen() == YES) rete(ERR_YES_INIT);
		CH path_[PATH_LEN_MAX];
		StrCpy(path_, path);
		PathToNtpathByDirPath(path_);
		UNICODE_STRING us;
		us.Length = StrLen(path_) * siz(CH);
		us.MaximumLength = us.Length;
		us.Buffer = path_;
		OBJECT_ATTRIBUTES objAttrib;
		objAttrib.Length = siz(OBJECT_ATTRIBUTES);
		objAttrib.RootDirectory = NUL;
		objAttrib.ObjectName = &us;
		objAttrib.Attributes = 0;
		objAttrib.SecurityDescriptor = NUL;
		objAttrib.SecurityQualityOfService = NUL;
		IO_STATUS_BLOCK iosb;
		LARGE_INTEGER allocSize_;
		allocSize_.QuadPart = allocSize;
		cx NTSTATUS status = NtCreateFile(&m_hdl, access, &objAttrib, &iosb, &allocSize_, attrib, share, openMode, flags, NUL, 0);
		ifu (status != STATUS_SUCCESS) {
			m_hdl = INVALID_HANDLE_VALUE;
			rete(ERR_FILE);
		}
		rets;
	}
	dfa ER OpenRead(cx CH* path) {
		ret tx->Open(path, FILE_READ_DATA | SYNCHRONIZE, 0, 0, FILE_SHARE_READ, FILE_OPEN, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);
	}
	dfa ER OpenWrite(cx CH* path) {
		ret tx->Open(path, FILE_WRITE_DATA | SYNCHRONIZE, 0, FILE_ATTRIBUTE_NORMAL, 0, FILE_OVERWRITE_IF, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);
	}
	dfa ER OpenReadWrite(cx CH* path) {
		ret tx->Open(path, FILE_READ_DATA | FILE_WRITE_DATA | SYNCHRONIZE, 0, FILE_ATTRIBUTE_NORMAL, 0, FILE_OPEN_IF, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);
	}
	dfa ER Close() {
		if (tx->IsOpen() == NO) rets;
		ifu (NtClose(m_hdl) != STATUS_SUCCESS) rete(ERR_FILE);
		m_hdl = INVALID_HANDLE_VALUE;
		rets;
	}
	dfa ER Read(GA buf, SI size, SI& result) {
		result = 0;
		ifu (tx->IsOpen() == NO) rete(ERR_NO_INIT);
		ifu ((size >> sizb(ULONG)) > 0) rete(ERR_HIGH_SIZE);
		IO_STATUS_BLOCK iosb;
		cx NTSTATUS status = NtReadFile(m_hdl, NUL, NUL, NUL, &iosb, buf, size, NUL, NUL);
		ifu (status != STATUS_SUCCESS) rete(ERR_FILE);
		result = iosb.Information;
		rets;
	}
	/*dfa ER Write(CXGA buf, SI size, SI& result) {
		rete(ERR_UNKNOWN);
	}*/
	dfa ER SizeGet(SI& size) {
		size = 0;
		ifu (tx->IsOpen() == NO) rete(ERR_NO_INIT);
		IO_STATUS_BLOCK iosb;
		SI infoCnt = 3;
		jdst(again);
		++infoCnt;
		Arr<FILE_STREAM_INFORMATION> info(infoCnt);
		cx NTSTATUS status = NtQueryInformationFile(m_hdl, &iosb, info.Ptr(), infoCnt * siz(FILE_STREAM_INFORMATION), FileStreamInformation);
		ifu (status == STATUS_BUFFER_OVERFLOW) jsrc(again);
		ifu (status != STATUS_SUCCESS) rete(ERR_FILE);
		size = info[0].StreamSize.QuadPart;
		rets;
	}
public:
	dfa FileNt() {
		m_hdl = INVALID_HANDLE_VALUE;
	}
	dfa ~FileNt() {
		tx->Close();
	}
};

class File {
private:
	#ifdef PROG_SYS_WIN
		FileWin m_file;
	#endif
public:
	dfa BO IsOpen() {
		ret m_file.IsOpen();
	}
	dfa ER CurMove(SI cnt) {
		ret m_file.CurMove(cnt);
	}
	dfa ER CurSet(SI pos) {
		ret m_file.CurSet(pos);
	}
	dfa ER OpenRead(cx CH* path) {
		ret m_file.OpenRead(path);
	}
	dfa ER OpenWrite(cx CH* path) {
		ret m_file.OpenWrite(path);
	}
	dfa ER OpenReadWrite(cx CH* path) {
		ret m_file.OpenReadWrite(path);
	}
	dfa ER Close() {
		ret m_file.Close();
	}
	dfa ER Read(GA buf, SI size, SI& result) {
		ret m_file.Read(buf, size, result);
	}
	dfa ER Write(CXGA buf, SI size, SI& result) {
		ret m_file.Write(buf, size, result);
	}
	dfa ER Read(GA buf, SI size) {
		ret m_file.Read(buf, size);
	}
	dfa ER Write(CXGA buf, SI size) {
		ret m_file.Write(buf, size);
	}
	dfa ER SizeGet(SI& size) {
		ret m_file.SizeGet(size);
	}
public:
	dfa File() {
		;
	}
	dfa ~File() {
		;
	}
};

constexpr F8 FILE_MEM_RESIZE_MUL_DEFA = 1.5;
constexpr SI FILE_MEM_RESIZE_ADD_DEFA = 1024;

class FileMem {
private:
	File m_file;
	Arr<U1> m_dat;
	SI m_filePos;
	SI m_filePosOfs;
	BO m_isOpen;
	BO m_isRead;
	BO m_isWrite;
	BO m_isWriteDirect;
	F8 m_resizeMul;
	SI m_resizeAdd;
public:
	dfa U1* _Dat() const {
		ret m_dat.Ptr();
	}
public:
	dfa BO IsOpen() const {
		ret m_isOpen;
	}
	dfa SI CurPos() const {
		ret m_filePos + m_filePosOfs;
	}
	dfa ER CurMove(SI cnt) {
		ifu (m_filePos + m_filePosOfs + cnt < 0) rete(ERR_LOW_SIZE);
		ifu (m_filePos + m_filePosOfs + cnt > m_dat.Pos()) rete(ERR_HIGH_SIZE);
		if (m_isWriteDirect) m_filePosOfs += cnt;
		else m_filePos += cnt;
		rets;
	}
	dfa ER CurSet(SI pos) {
		ifu (pos < 0) rete(ERR_LOW_SIZE);
		ifu (pos > m_dat.Pos()) rete(ERR_HIGH_SIZE);
		if (m_isWriteDirect) m_filePosOfs = pos - m_filePos;
		else m_filePos = pos;
		rets;
	}
	dfa ER OpenRead(cx CH* path) {
		ifu (tx->IsOpen()) rete(ERR_YES_INIT);
		ife (m_file.OpenRead(path)) retep;
		SI size = 0;
		ife (m_file.SizeGet(size)) retep;
		m_dat.Alloc(size);
		ife (m_file.Read(m_dat.Ptr(), size)) retep;
		m_dat.CurSet(size);
		ife (m_file.Close()) retep;
		m_filePos = 0;
		m_filePosOfs = 0;
		m_isOpen = YES;
		m_isRead = YES;
		m_isWrite = NO;
		m_isWriteDirect = NO;
		m_resizeMul = 0.0;
		m_resizeAdd = 0;
		rets;
	}
	dfa ER OpenWrite(cx CH* path, BO isWriteDirect = NO, F8 resizeMul = FILE_MEM_RESIZE_MUL_DEFA, SI resizeAdd = FILE_MEM_RESIZE_ADD_DEFA) {
		ifu (tx->IsOpen()) rete(ERR_YES_INIT);
		ife (m_file.OpenWrite(path)) retep;
		SI size = 0;
		ife (m_file.SizeGet(size)) retep;
		m_dat.Alloc(SI(F8(size) * resizeMul + F8(resizeAdd)));
		MemSet(m_dat.Ptr(), 0, size);
		m_dat.CurSet(size);
		m_filePos = 0;
		m_filePosOfs = 0;
		m_isOpen = YES;
		m_isRead = NO;
		m_isWrite = YES;
		m_isWriteDirect = isWriteDirect;
		m_resizeMul = resizeMul;
		m_resizeAdd = resizeAdd;
		rets;
	}
	dfa ER OpenReadWrite(cx CH* path, BO isWriteDirect = NO, F8 resizeMul = FILE_MEM_RESIZE_MUL_DEFA, SI resizeAdd = FILE_MEM_RESIZE_ADD_DEFA) {
		ifu (tx->IsOpen()) rete(ERR_YES_INIT);
		ife (m_file.OpenReadWrite(path)) retep;
		SI size = 0;
		ife (m_file.SizeGet(size)) retep;
		m_dat.Alloc(SI(F8(size) * resizeMul + F8(resizeAdd)));
		ife (m_file.Read(m_dat.Ptr(), size)) retep;
		m_dat.CurSet(size);
		m_filePos = 0;
		m_filePosOfs = 0;
		m_isOpen = YES;
		m_isRead = YES;
		m_isWrite = YES;
		m_isWriteDirect = isWriteDirect;
		m_resizeMul = resizeMul;
		m_resizeAdd = resizeAdd;
		rets;
	}
	dfa ER Close() {
		ifu (!tx->IsOpen()) rets;
		if (m_isWrite) {
			if (!m_isWriteDirect) {
				ife (m_file.CurSet(0)) retep;
				ife (m_file.Write(m_dat.Ptr(), m_dat.Pos())) retep;
			}
			ife (m_file.Close()) retep;
		}
		m_dat.Dealloc();
		m_filePos = 0;
		m_filePosOfs = 0;
		m_isOpen = NO;
		m_isRead = NO;
		m_isWrite = NO;
		m_isWriteDirect = NO;
		m_resizeMul = 0.0;
		m_resizeAdd = 0;
		rets;
	}
	dfa ER Read(GA buf, SI size, SI& result) {
		result = 0;
		ifu (m_filePos + m_filePosOfs + size > m_dat.Pos()) rete(ERR_HIGH_SIZE);
		MemCpy(buf, m_dat.Ptr() + m_filePos + m_filePosOfs, size);
		if (m_isWriteDirect) m_filePosOfs += size;
		else m_filePos += size;
		result = size;
		rets;
	}
	dfa ER Write(CXGA buf, SI size, SI& result) {
		result = 0;
		ifu (!m_isWrite) rete(ERR_NO_INIT);
		if (m_isWriteDirect) {
			if (m_filePosOfs != 0) {
				ife (m_file.CurMove(m_filePosOfs)) retep;
				m_filePos += m_filePosOfs;
				m_filePosOfs = 0;
			}
			ife (m_file.Write(buf, size)) retep;
		}
		ifu (m_filePos + size > m_dat.Cap()) {
			cx SI newCap = SI(F8(m_dat.Cap()) * m_resizeMul) + m_resizeAdd + size;
			m_dat.Req(newCap, newCap, m_dat.Pos());
		}
		MemCpy(m_dat.Ptr() + m_filePos, buf, size);
		m_filePos += size;
		if (m_filePos > m_dat.Pos()) m_dat.CurSet(m_filePos);
		result = size;
		rets;
	}
	dfa ER Read(GA buf, SI size) {
		SI result;
		ife (tx->Read(buf, size, result)) retep;
		ifu (size != result) rete(ERR_NO_FULL);
		rets;
	}
	dfa BO ReadLine(SStr& str) {
		U1* ptrBase = m_dat.Ptr() + m_filePos;
		U1* ptr = ptrBase;
		U1* end = m_dat.Ptr() + m_dat.Pos();
		while (ptr < end) {
			if (*ptr == '\r' || *ptr == '\n') {
				str.Set((CS*)ptrBase, ptr - ptrBase);
				m_filePos += ptr - ptrBase;
				if ((*ptr == '\r') && (ptr + 1 < end) && (*(ptr + 1) == '\n')) ++m_filePos;
				++m_filePos;
				ret YES;
			}
			++ptr;
		}
		if (ptr > ptrBase) {
			str.Set((CS*)ptrBase, ptr - ptrBase);
			m_filePos += ptr - ptrBase;
			ret YES;
		}
		ret NO;
	}
	dfa ER Write(CXGA buf, SI size) {
		SI result;
		ife (tx->Write(buf, size, result)) retep;
		ifu (size != result) rete(ERR_NO_FULL);
		rets;
	}
	dfa ER SizeGet(SI& size) {
		size = 0;
		ifu (tx->IsOpen() == NO) rete(ERR_NO_INIT);
		size = m_dat.Pos();
		rets;
	}
public:
	dfa FileMem() {
		m_filePos = 0;
		m_filePosOfs = 0;
		m_isOpen = NO;
		m_isRead = NO;
		m_isWrite = NO;
		m_isWriteDirect = NO;
		m_resizeMul = 0.0;
		m_resizeAdd = 0;
	}
	dfa ~FileMem() {
		tx->Close();
	}
};
