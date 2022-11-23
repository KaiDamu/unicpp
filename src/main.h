#pragma once

dfa ER MainInit() {
	#ifdef PROG_SYS_WIN
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS); // hint for performance, error not handled
	#endif
	TimeResSet(1, 0, NO); // hint for performance, error not handled
	ife (TimeMainInit()) rete(ERR_TIME);
	rets;
}
dfa ER MainFree() {
	TimeResClear(); // hint for performance, error not handled
	rets;
}

dfa ER Main();

dfa ER _Main() {
	ife (MainInit()) retepass;
	ife (Main()) retepass;
	ife (MainFree()) retepass;
	rets;
}

int main() {
	ifdbg (YES) {
		ConWriteInfo("Start of program");
	}
	ErrVal errVal = ERR_NONE;
	ife (_Main()) errVal = ErrLastValGet();
	ifdbg (YES) {
		cx F8 t = TimeMainGetF();
		ifu (errVal != ERR_NONE) ConWriteErr("Main error code : %u", errVal);
		ConWriteInfo("Main execution time : %.3f ms", t);
		ConWriteInfo("End of program");
		ConPause();
	}
	ProcCurExit(U4(errVal));
	ret int(errVal);
}
