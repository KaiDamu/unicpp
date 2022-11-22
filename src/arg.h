#pragma once

SI g_argCnt = 0;
CH** g_argVal = NUL;

dfa NT _ArgInit() {
	ifl (g_argVal != NUL) ret;
	int tmp = 0;
	g_argVal = CommandLineToArgvW(GetCommandLineW(), &tmp);
	g_argCnt = tmp;
	Assert(g_argVal != NUL);
}
dfa SI ArgCnt() {
	_ArgInit();
	ret g_argCnt;
}
dfa CH* ArgVal(SI i) {
	_ArgInit();
	Assert(i < g_argCnt);
	ret g_argVal[i];
}
