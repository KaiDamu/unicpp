#pragma once

#define TestPerf(name, code) { \
	F8 t = 0.0; \
	volatile SI n = 1; \
	do { \
		t = TimeMainF(); \
		for (SI i = 0; i < n; ++i) { \
			code; \
		} \
		t = TimeMainF() - t; \
		n = n * 2; \
	} while (t < 250.0); \
	n = Max<U8>(U8(F8(n / 2) * (1000.0 / t)), 1); \
	t = TimeMainF(); \
	for (SI i = 0; i < n; ++i) { \
		; \
	} \
	cx F8 tEmpty = TimeMainF() - t; \
	t = TimeMainF(); \
	for (SI i = 0; i < n; ++i) { \
		code; \
	} \
	cx F8 tFull = TimeMainF() - t; \
	cx F8 tPayload = tFull - tEmpty; \
	cx F8 perf = F8(n) / (tPayload / 1000.0); \
	if (perf < 1000.0) { ConWriteInfo("<Performance> \"%s\" : %.4f / sec", name, perf); } \
	else if (perf < 1000000.0) { ConWriteInfo("<Performance> \"%s\" : %.4f K / sec", name, perf / 1000.0); } \
	else { ConWriteInfo("<Performance> \"%s\" : %.4f M / sec", name, perf / 1000000.0); } \
}
