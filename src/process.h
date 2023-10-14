#pragma once

dfa HANDLE _ProcHdlGetNt() {
	ret reinterpret_cast<HANDLE>(-1);
}

dfa NT ProcCurCrash() {
	volatile U1 x = U1(clock());
	volatile U1 y = *reinterpret_cast<U1*>(UA(x ^ x));
	y = y / (x ^ x);
}
dfa NT ProcCurExit(U4 retVal) {
	RtlExitUserProcess(retVal);
	ProcCurCrash();
}
dfa NT ProcCurExit() {
	ProcCurExit(U4(ErrLastGet()));
}
dfa BO ProcCurIsElevated() {
	SID_IDENTIFIER_AUTHORITY sia = {SECURITY_NT_AUTHORITY};
	PSID sid;
	ifu (AllocateAndInitializeSid(&sia, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &sid) == 0) ret NO;
	BOOL isElevated;
	ifu (CheckTokenMembership(NUL, sid, &isElevated) == 0) isElevated = NO;
	FreeSid(sid);
	ret BO(isElevated);
}
dfa SI ProcFilePath(CH* path) {
	path[0] = '\0';
	cx SI r = GetModuleFileNameW(NUL, path, PATH_LEN_MAX);
	ifu (r == PATH_LEN_MAX) ret r - 1;
	ret r;
}
dfa SI ProcEnvvarGet(CH* val, cx CH* envvar, SI valLenMax) {
	ifl (valLenMax > 0) val[0] = '\0';
	cx SI r = GetEnvironmentVariableW(envvar, val, DWORD(valLenMax));
	ifu (r >= valLenMax && valLenMax > 0) val[0] = '\0';
	ret r;
}
dfa ER ProcCurEnvvarSet(cx CH* val, cx CH* envvar) {
	ifu (SetEnvironmentVariableW(envvar, val) == 0) rete(ERR_PROC);
	rets;
}
dfa ER ProcCurEnvvarClear(cx CH* envvar) {
	ifu (SetEnvironmentVariableW(envvar, NUL) == 0) rete(ERR_PROC);
	rets;
}
dfa cx CH* ProcCurArgFullGet() {
	ret GetCommandLineW();
}

dfa ER ProcNewFile(cx CH* path) {
	SHELLEXECUTEINFOW info = {};
	info.cbSize = siz(info);
	info.fMask = SEE_MASK_NOCLOSEPROCESS;
	info.lpVerb = L"open";
	info.lpFile = path;
	info.nShow = SW_SHOW;
	ifu (ShellExecuteExW(&info) == NO || UA(info.hInstApp) <= 32 || info.hProcess == NUL) rete(ERR_PROC);
	rets;
}

class Proc {
private:
	PROCESS_INFORMATION m_info;
	wstring m_args;
public:
	dfa NT __Drop() {
		m_info.hProcess = NUL;
	}
public:
	dfa HANDLE Hdl() const {
		ret m_info.hProcess;
	}
	dfa ER RetVal(U4& out) const {
		out = -1;
		ifu (m_info.hProcess == NUL) rete(ERR_NO_INIT);
		DWORD r;
		cx BOOL result = GetExitCodeProcess(m_info.hProcess, &r);
		ifu (result == 0) rete(ERR_PROC);
		ifu (r == STILL_ACTIVE) rete(ERR_YES_ACTIVE);
		out = r;
		rets;
	}
	dfa BO IsActive() const {
		ifu (m_info.hProcess == NUL) ret NO;
		DWORD r;
		cx BOOL result = GetExitCodeProcess(m_info.hProcess, &r);
		ifu (result == 0) ret NO;
		if (r == STILL_ACTIVE) ret YES;
		ret NO;
	}
	dfa ER Close() {
		if (m_info.hProcess == NUL) rets;
		if (tx->IsActive()) rete(ERR_YES_ACTIVE);
		ifu (m_info.hThread != NUL && CloseHandle(m_info.hThread) == 0) rete(ERR_THD);
		ifu (m_info.hProcess != NUL && CloseHandle(m_info.hProcess) == 0) rete(ERR_PROC);
		MemSetVal(&m_info, 0, siz(m_info));
		m_args = L"";
		rets;
	}
	dfa ER Wait() const {
		if (tx->IsActive() == NO) rets;
		ifu (WaitForSingleObject(m_info.hProcess, INFINITE) != WAIT_OBJECT_0) rete(ERR_PROC);
		rets;
	}
	dfa ER Start(cx CH* path, cx CH* args, cx CH* workPath) {
		ifu (tx->IsActive()) rete(ERR_YES_ACTIVE);
		ife (tx->Close()) retep;
		m_args = L"\"";
		m_args += path;
		m_args += L"\"";
		if (args != NUL && args[0] != '\0') {
			m_args += L" ";
			m_args += args;
		}
		STARTUPINFOW tmp = {};
		MemSetVal(&tmp, 0, siz(tmp));
		tmp.cb = siz(tmp);
		cx BOOL result = CreateProcessW(path, const_cast<CH*>(m_args.c_str()), NUL, NUL, NO, 0, NUL, workPath, &tmp, &m_info);
		ifu (result == 0) {
			MemSetVal(&m_info, 0, siz(m_info));
			m_args = L"";
			rete(ERR_PROC);
		}
		rets;
	}
	dfa ER StartElevated(cx CH* path, cx CH* args, cx CH* workPath) {
		if (ProcCurIsElevated()) ret tx->Start(path, args, workPath);
		ifu (tx->IsActive()) rete(ERR_YES_ACTIVE);
		ife (tx->Close()) retep;
		m_args = args;
		SHELLEXECUTEINFOW sei = {};
		sei.cbSize = siz(sei);
		sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NO_CONSOLE;
		sei.lpVerb = L"runas";
		sei.lpFile = path;
		sei.lpParameters = args;
		sei.lpDirectory = workPath;
		sei.nShow = SW_SHOW;
		ifu (ShellExecuteExW(&sei) == NO || UA(sei.hInstApp) <= 32 || sei.hProcess == NUL) {
			m_args = L"";
			rete(ERR_PROC);
		}
		MemSetVal(&m_info, 0, siz(m_info));
		m_info.hProcess = sei.hProcess;
		rets;
	}
	dfa ER Stop() const {
		if (tx->IsActive() == NO) rets;
		ifu (TerminateProcess(m_info.hProcess, U4(-1)) == 0) rete(ERR_PROC);
		ife (tx->Wait()) retep;
		rets;
	}
public:
	dfa Proc() {
		MemSetVal(&m_info, 0, siz(m_info));
	}
	dfa ~Proc() {
		tx->Stop();
		tx->Close();
	}
};

dfa ER ProcCurRestartElevated() {
	if (ProcCurIsElevated()) rets;
	Proc proc;
	ife (proc.StartElevated(ProcFilePath(), StrArgSkip(ProcCurArgFullGet()), ProcWorkPath())) retep;
	proc.__Drop();
	ProcCurExit(0);
	rets;
}

constexpr SI PROC_GLOBAL_STR_LEN_MAX = PATH_LEN_MAX;

class ProcGlobalStr {
private:
	HANDLE m_hdl;
	CH m_str[PROC_GLOBAL_STR_LEN_MAX + 1];
private:
	dfa NT Init() {
		m_hdl = NUL;
		m_str[0] = '\0';
	}
public:
	dfa NT __Drop() {
		tx->Init();
	}
public:
	dfa BO IsOpenCur() const {
		ret m_hdl != NUL;
	}
	dfa cx CH* Str() const {
		ret m_str;
	}
	dfa BO IsOpen(cx CH* str) const {
		SetLastError(0);
		cx HANDLE hdl = CreateMutexW(NUL, NO, str);
		ifu (hdl == NUL) ret NO;
		cx BO r = (GetLastError() == ERROR_ALREADY_EXISTS);
		CloseHandle(hdl);
		ret r;
	}
	dfa ER Close() {
		if (tx->IsOpenCur() == NO) rets;
		ifu (CloseHandle(m_hdl) == 0) rete(ERR_PROC);
		m_hdl = NUL;
		rets;
	}
	dfa ER Open(cx CH* str) {
		ifu (tx->IsOpenCur()) rete(ERR_YES_INIT);
		cx SI strLen = StrLen(str);
		ifu (strLen == 0) rete(ERR_LOW_SIZE);
		ifu (strLen > PROC_GLOBAL_STR_LEN_MAX) rete(ERR_HIGH_SIZE);
		SetLastError(0);
		m_hdl = CreateMutexW(NUL, YES, str);
		ifu (m_hdl == NUL) rete(ERR_PROC);
		ifu (GetLastError() == ERROR_ALREADY_EXISTS) {
			CloseHandle(m_hdl);
			m_hdl = NUL;
			rete(ERR_YES_EXIST);
		}
		MemSet(m_str, str, (strLen + 1) * siz(str[0]));
		rets;
	}
	dfa ER WaitIsClose(cx CH* str) const {
		SetLastError(0);
		cx HANDLE hdl = CreateMutexW(NUL, NO, str);
		ifu (hdl == NUL) rete(ERR_PROC);
		ifu ((GetLastError() == ERROR_ALREADY_EXISTS) && (WaitForSingleObject(hdl, INFINITE) != WAIT_OBJECT_0)) {
			CloseHandle(hdl);
			rete(ERR_PROC);
		}
		CloseHandle(hdl);
		rets;
	}
public:
	dfa ProcGlobalStr() {
		tx->Init();
	}
	dfa ProcGlobalStr(cx CH* str) {
		tx->Init();
		tx->Open(str);
	}
	dfa ~ProcGlobalStr() {
		tx->Close();
	}
};

dfa BO ProcGlobalTake(cx CH* str = NUL) {
	CH path[PATH_LENX_MAX];
	if (str == NUL) {
		ProcFilePath(path);
		PathSanitize(path, path);
	} else {
		StrSet(path, str);
	}
	ProcGlobalStr global;
	ife (global.Open(path)) ret NO;
	global.__Drop();
	ret YES;
}
