#pragma once

using ErrVal = U4;

enum : ErrVal {
	ERR_NONE = 0,
	//
	ERR_MEM,
	ERR_TIME,
	ERR_PROC,
	ERR_THD,
	ERR_EVT,
	ERR_DRV,
	ERR_PATH,
	ERR_FILE,
	ERR_DIR,
	ERR_PIPE,
	ERR_CLIPB,
	ERR_ENV,
	ERR_TOKEN,
	ERR_CUR,
	ERR_HOOK,
	ERR_LOCK,
	ERR_WIN,
	ERR_CON,
	ERR_REG,
	ERR_KEYB,
	ERR_MOUS,
	ERR_NET,
	ERR_SCN,
	//
	ERR_MEM_NEW,
	ERR_TIME_RES,
	//
	ERR_NO_SUPPORT,
	ERR_NO_VALID,
	ERR_NO_EXIST,
	ERR_NO_INIT,
	ERR_NO_FULL,
	//
	ERR_YES_EXIST,
	ERR_YES_INIT,
	ERR_YES_ACTIVE,
	//
	ERR_LOW_SIZE,
	ERR_LOW_WAIT,
	//
	ERR_HIGH_SIZE,
	ERR_HIGH_WAIT,
	//
	ERR_NA,
};

thdlocal ErrVal g_errLastVal = ERR_NONE;

dfa NT ErrLastSet(ErrVal val) {
	g_errLastVal = val;
}
dfa ErrVal ErrLastGet() {
	ret g_errLastVal;
}
