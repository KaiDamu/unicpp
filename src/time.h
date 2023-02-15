#pragma once

U8 g_timeMainOfs = 0;
U8 g_timeMainDivU = 0;
F8 g_timeMainDivF = 0.0;

dfa U8 LdapToUnix(U8 ldap) {
	ifu (ldap < 116444736000000000) ret 0;
	ret ldap / 10000000 - 11644473600;
}
dfa U8 UnixToLdap(U8 unix) {
	ret unix * 10000000 + 116444736000000000;
}

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

dfa ER TimeResClr() {
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
		TimeResClr();
		rete(ERR_TIME_RES);
	}
	rets;
}

dfa ER _TimeMainInit() {
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
dfa U8 TimeMainU() {
	LARGE_INTEGER val;
	QueryPerformanceCounter(&val);
	ret (val.QuadPart - g_timeMainOfs) / g_timeMainDivU;
}
dfa F8 TimeMainF() {
	LARGE_INTEGER val;
	QueryPerformanceCounter(&val);
	ret U8ToF8(val.QuadPart - g_timeMainOfs) / g_timeMainDivF;
}

dfa U8 TimeUnix() {
	ret time(NUL);
}
