#pragma once

using ErrVal = U4;

enum : ErrVal {
	ERR_NONE = 0,
	//
	ERR_TIME,
	ERR_PROC,
	ERR_THD,
	ERR_EVT,
	ERR_FILE,
	ERR_PIPE,
	ERR_ENV,
	ERR_CUR,
	//
	ERR_MEM_ALLOC,
	ERR_TIME_RES,
	//
	ERR_NO_INIT,
	ERR_NO_VALID,
	ERR_NO_SUPPORT,
	ERR_NO_FULL,
	ERR_NO_EXIST,
	//
	ERR_YES_INIT,
	ERR_YES_EXIST,
	ERR_YES_ACTIVE,
	//
	ERR_LOW_SIZE,
	ERR_LOW_WAIT,
	//
	ERR_HIGH_SIZE,
	ERR_HIGH_WAIT,
	//
	ERR_UNKNOWN,
};

thdlocal ErrVal g_errLastVal = ERR_NONE;

dfa NT ErrLastValSet(ErrVal val) {
	g_errLastVal = val;
}
dfa ErrVal ErrLastValGet() {
	ret g_errLastVal;
}
