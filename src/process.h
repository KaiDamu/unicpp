#pragma once

dfa HANDLE ProcCurNtHdlGet() {
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
	ProcCurExit(U4(ErrLastValGet()));
}

class Proc {
private:
	PROCESS_INFORMATION m_info;
	Str m_args;
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
		ifu (CloseHandle(m_info.hThread) == 0) rete(ERR_THD);
		ifu (CloseHandle(m_info.hProcess) == 0) rete(ERR_PROC);
		MemSet(&m_info, 0, siz(m_info));
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
		ife (tx->Close()) retepass;
		m_args = args;
		STARTUPINFOW tmp;
		MemSet(&tmp, 0, siz(tmp));
		tmp.cb = siz(tmp);
		cx BOOL result = CreateProcessW(path, const_cast<CH*>(m_args.Val()), NUL, NUL, NO, 0, NUL, workPath, &tmp, &m_info);
		ifu (result == 0) {
			MemSet(&m_info, 0, siz(m_info));
			m_args = L"";
			rete(ERR_PROC);
		}
		rets;
	}
	dfa ER Stop() const {
		if (tx->IsActive() == NO) rets;
		ifu (TerminateProcess(m_info.hProcess, U4(-1)) == 0) rete(ERR_PROC);
		ife (tx->Wait()) retepass;
		rets;
	}
public:
	dfa Proc() {
		MemSet(&m_info, 0, siz(m_info));
	}
	dfa ~Proc() {
		tx->Stop();
		tx->Close();
	}
};
