#pragma once

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
		ife (tx->Read(buf, size, result)) retepass;
		ifu (size != result) rete(ERR_NO_FULL);
		rets;
	}
	dfa ER Write(CXGA buf, SI size) {
		SI result;
		ife (tx->Write(buf, size, result)) retepass;
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
		ChstrCpy(path_, path);
		PathToNtpathByDirPath(path_);
		UNICODE_STRING us;
		us.Length = ChstrLen(path_) * siz(CH);
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
		ifu ((size >> sizbit(ULONG)) > 0) rete(ERR_HIGH_SIZE);
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
