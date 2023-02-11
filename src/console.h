#pragma once

enum class ConCol : U1 {
	BLACK = 0,
	DARK_BLUE = 1,
	DARK_GREEN = 2,
	DARK_CYAN = 3,
	DARK_RED = 4,
	PURPLE = 5,
	BROWN = 6,
	LIGHT_GRAY = 7,
	DARK_GRAY = 8,
	BLUE = 9,
	GREEN = 10,
	CYAN = 11,
	RED = 12,
	MAGENTA = 13,
	YELLOW = 14,
	WHITE = 15,
};

constexpr SI CON_BUF_LEN_MAX = 1024;

ConCol g_conCol = ConCol::WHITE;
ConCol g_conColReal = g_conCol;

dfa ER _ConColSet(ConCol col) {
	cx HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	ifu (hdl == INVALID_HANDLE_VALUE) rete(ERR_NO_EXIST);
	ifu (SetConsoleTextAttribute(hdl, WORD(col)) == 0) rete(ERR_CON);
	g_conColReal = col;
	rets;
}
dfa ER _ConColGet(ConCol& col) {
	cx HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	ifu (hdl == INVALID_HANDLE_VALUE) rete(ERR_NO_EXIST);
	CONSOLE_SCREEN_BUFFER_INFO info;
	ifu (GetConsoleScreenBufferInfo(hdl, &info) == 0) rete(ERR_CON);
	col = ConCol(info.wAttributes & 0x0F);
	rets;
}
dfa ER _ConColUpd() {
	if (g_conColReal != g_conCol) {
		ife (_ConColSet(g_conCol)) retepass;
	}
	rets;
}
dfa ER _ConBufEmpty() {
	cx HANDLE hdl = GetStdHandle(STD_INPUT_HANDLE);
	ifu (hdl == INVALID_HANDLE_VALUE) rete(ERR_NO_EXIST);
	INPUT_RECORD ir;
	DWORD irRead;
	while (YES) {
		ifu (PeekConsoleInputW(hdl, &ir, 1, &irRead) == 0) rete(ERR_CON);
		if (irRead == 0) break;
		ifu (ReadConsoleInputW(hdl, &ir, 1, &irRead) == 0) rete(ERR_CON);
	}
	rets;
}
dfa ER _ConWriteRaw(cx CS* buf, SI bufLen) {
	ifu (bufLen <= 0) rets;
	ife (_ConColUpd()) retepass;
	cx HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	ifu (hdl == INVALID_HANDLE_VALUE) rete(ERR_NO_EXIST);
	DWORD result;
	ifu (WriteConsoleA(hdl, buf, bufLen, &result, NUL) == 0) rete(ERR_CON);
	ifu (result != bufLen) rete(ERR_CON);
	rets;
}
dfa ER _ConWriteRawCol(ConCol col, cx CS* buf, SI bufLen) {
	ConCol colOld;
	ife (ConColGet(colOld)) retepass;
	ife (ConColSet(col)) retepass;
	ife (_ConWriteRaw(buf, bufLen)) {
		ConColSet(colOld);
		retepass;
	}
	ife (ConColSet(colOld)) retepass;
	rets;
}
dfa ER _ConWriteRawAl(cx CS* format, cx AL& args) {
	CS buf[CON_BUF_LEN_MAX + 1];
	cx SI bufLen = vsnprintf(buf, CON_BUF_LEN_MAX, format, args);
	ifu (bufLen < 0 || bufLen >= CON_BUF_LEN_MAX) rete(ERR_CON);
	ife (_ConWriteRaw(buf, bufLen)) retepass;
	rets;
}

dfa BO ConIsExist() {
	ret (GetConsoleWindow() != NUL);
}
dfa ER ConTitleSet(cx CH* title) {
	ife (ConReq()) retepass;
	ifu (SetConsoleTitleW(title) == 0) rete(ERR_CON);
	rets;
}
dfa SI ConTitleGet(CH* title, SI titleLenMax) {
	cx SI titleLen = GetConsoleTitleW(title, titleLenMax);
	ret titleLen;
}
dfa ER ConCreate(cx CH* title = NUL) {
	ifu (AllocConsole() == 0) rete(ERR_CON);
	if (title != NUL) {
		ife (ConTitleSet(title)) retepass;
	}
	ife (_ConColSet(g_conCol)) retepass;
	ife (_ConBufEmpty()) retepass;
	// font (extra)
	cx HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX font;
	font.cbSize = siz(font);
	font.dwFontSize.X = 0;
	font.dwFontSize.Y = 18;
	font.FontWeight = FW_NORMAL;
	StrCpy(font.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(hdl, NO, &font);
	//
	ife (WinFocusSet(HD(GetConsoleWindow()))) retepass;
	rets;
}
dfa ER ConDel() {
	if (ConIsExist() == NO) rets;
	ifu (FreeConsole() == 0) rete(ERR_CON);
	rets;
}
dfa ER ConReq() {
	if (ConIsExist()) rets;
	ife (ConCreate()) retepass;
	rets;
}
dfa ER ConColSet(ConCol col) {
	g_conCol = col;
	rets;
}
dfa ER ConColGet(ConCol& col) {
	col = g_conCol;
	rets;
}
dfa ER ConWriteRaw(cx CS* format, ...) {
	ife (ConReq()) retepass;
	AL args;
	AlCreate(args, format);
	ife (_ConWriteRawAl(format, args)) {
		AlDel(args);
		retepass;
	}
	AlDel(args);
	rets;
}
dfa ER ConWrite(cx CS* format, ...) {
	ife (ConReq()) retepass;
	AL args;
	AlCreate(args, format);
	ife (_ConWriteRawAl(format, args)) {
		AlDel(args);
		retepass;
	}
	AlDel(args);
	ife (_ConWriteRaw("\n", 1)) retepass;
	rets;
}
dfa ER ConWriteInfo(cx CS* format, ...) {
	ife (ConReq()) retepass;
	ife (_ConWriteRawCol(ConCol::GREEN, "[INFO] ", 7)) retepass;
	AL args;
	AlCreate(args, format);
	ife (_ConWriteRawAl(format, args)) {
		AlDel(args);
		retepass;
	}
	AlDel(args);
	ife (_ConWriteRaw("\n", 1)) retepass;
	rets;
}
dfa ER ConWriteWarn(cx CS* format, ...) {
	ife (ConReq()) retepass;
	ife (_ConWriteRawCol(ConCol::YELLOW, "[WARNING] ", 10)) retepass;
	AL args;
	AlCreate(args, format);
	ife (_ConWriteRawAl(format, args)) {
		AlDel(args);
		retepass;
	}
	AlDel(args);
	ife (_ConWriteRaw("\n", 1)) retepass;
	rets;
}
dfa ER ConWriteErr(cx CS* format, ...) {
	ife (ConReq()) retepass;
	ife (_ConWriteRawCol(ConCol::RED, "[ERROR] ", 8)) retepass;
	AL args;
	AlCreate(args, format);
	ife (_ConWriteRawAl(format, args)) {
		AlDel(args);
		retepass;
	}
	AlDel(args);
	ife (_ConWriteRaw("\n", 1)) retepass;
	rets;
}
dfa SI ConReadStr(CS* str, SI strLenMax, BO isShow = YES) {
	ife (ConReq()) ret -1;
	ife (_ConBufEmpty()) ret -1;
	cx HANDLE hdl = GetStdHandle(STD_INPUT_HANDLE);
	ifu (hdl == INVALID_HANDLE_VALUE) ret -1;
	INPUT_RECORD ir;
	DWORD irRead;
	SI strLen = 0;
	while (YES) {
		ifu (PeekConsoleInputW(hdl, &ir, 1, &irRead) == 0) ret -1;
		if (irRead == 0) {
			ThdWait(1);
			continue;
		}
		ifu (ReadConsoleInputW(hdl, &ir, 1, &irRead) == 0) ret -1;
		if (ir.EventType != KEY_EVENT || !ir.Event.KeyEvent.bKeyDown) continue;
		if (ir.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
			ife (_ConWriteRaw("\n", 1)) ret -1;
			break;
		}
		if (ir.Event.KeyEvent.wVirtualKeyCode == VK_BACK || ir.Event.KeyEvent.wVirtualKeyCode == VK_LEFT) {
			if (strLen > 0) {
				str[--strLen] = '\0';
				if (isShow) ife (_ConWriteRaw("\b \b", 3)) ret -1;
			}
			continue;
		}
		if (ir.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT || ir.Event.KeyEvent.wVirtualKeyCode == VK_UP || ir.Event.KeyEvent.wVirtualKeyCode == VK_DOWN) {
			continue;
		}
		if (strLen < strLenMax - 1) {
			str[strLen++] = ir.Event.KeyEvent.uChar.UnicodeChar;
			if (isShow) ife (ConWriteRaw("%c", ir.Event.KeyEvent.uChar.UnicodeChar)) ret -1;
		}
	}
	str[strLen] = '\0';
	ret strLen;
}
dfa ER ConPause() {
	ife (ConReq()) retepass;
	ife (_ConBufEmpty()) retepass;
	cx HANDLE hdl = GetStdHandle(STD_INPUT_HANDLE);
	ifu (hdl == INVALID_HANDLE_VALUE) rete(ERR_NO_EXIST);
	INPUT_RECORD ir;
	DWORD irRead;
	while (YES) {
		ifu (PeekConsoleInputW(hdl, &ir, 1, &irRead) == 0) rete(ERR_CON);
		if (irRead == 0) {
			ThdWait(1);
			continue;
		}
		ifu (ReadConsoleInputW(hdl, &ir, 1, &irRead) == 0) rete(ERR_CON);
		if (ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown) {
			if (ir.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) break;
		}
	}
	rets;
}
dfa ER ConDatEmpty() {
	if (ConIsExist() == NO) rets;
	cx HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	ifu (hdl == INVALID_HANDLE_VALUE) rete(ERR_NO_EXIST);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	ifu (GetConsoleScreenBufferInfo(hdl, &csbi) == 0) rete(ERR_CON);
	cx DWORD conSize = csbi.dwSize.X * csbi.dwSize.Y;
	cx COORD coord = {0, 0};
	DWORD result = 0;
	ifu (FillConsoleOutputCharacterW(hdl, ' ', conSize, coord, &result) == 0) rete(ERR_CON);
	ifu (GetConsoleScreenBufferInfo(hdl, &csbi) == 0) rete(ERR_CON);
	ifu (FillConsoleOutputAttribute(hdl, csbi.wAttributes, conSize, coord, &result) == 0) rete(ERR_CON);
	ifu (SetConsoleCursorPosition(hdl, coord) == 0) rete(ERR_CON);
	rets;
}
