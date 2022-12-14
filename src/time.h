#pragma once

U8 g_timeMainOfs = 0;
U8 g_timeMainDivU = 0;
F8 g_timeMainDivF = 0.0;

dfa U8 CpuTsc() {
	union {
		U8 val;
		U4 part[2];
	};
	#ifdef PROG_COMPILER_GCC
		__asm__(".byte 0x0f, 0x31" : "=a"(part[0]), "=d"(part[1]));
	#else
		val = __rdtsc();
	#endif
	ret val;
}

dfa ER TimeResClear() {
	ULONG resNew;
	ifu (NtSetTimerResolution(0, FALSE, &resNew) != STATUS_SUCCESS) {
		rete(ERR_TIME_RES);
	}
	rets;
}
dfa ER TimeResSet(S4 ms, S4 us, BO force) {
	cx ULONG resReq = ULONG((ms * 1000 + us) * 10);
	ULONG resNew;
	ifu (NtSetTimerResolution(resReq, TRUE, &resNew) != STATUS_SUCCESS) {
		rete(ERR_TIME_RES);
	}
	ifu (force && (resReq != resNew)) {
		TimeResClear();
		rete(ERR_TIME_RES);
	}
	rets;
}

dfa ER TimeMainInit() {
	LARGE_INTEGER val;
	ifu (QueryPerformanceFrequency(&val) == 0) {
		rete(ERR_TIME);
	}
	cx U8 timeMainDivU = val.QuadPart / 1000;
	cx F8 timeMainDivF = S8ToF8(val.QuadPart) / 1000.0;
	ifu (QueryPerformanceCounter(&val) == 0) {
		rete(ERR_TIME);
	}
	cx U8 timeMainOfs = val.QuadPart;
	g_timeMainOfs = timeMainOfs;
	g_timeMainDivU = timeMainDivU;
	g_timeMainDivF = timeMainDivF;
	rets;
}
dfa U8 TimeMainGetU() {
	LARGE_INTEGER val;
	QueryPerformanceCounter(&val);
	ret (val.QuadPart - g_timeMainOfs) / g_timeMainDivU;
}
dfa F8 TimeMainGetF() {
	LARGE_INTEGER val;
	QueryPerformanceCounter(&val);
	ret U8ToF8(val.QuadPart - g_timeMainOfs) / g_timeMainDivF;
}

dfa U8 TimeUnixGetSecU() {
	ret time(NUL);
}
