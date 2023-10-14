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
constexpr SI CON_HISTORY_CNT_MAX = 32;

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
		ife (_ConColSet(g_conCol)) retep;
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
	ife (_ConColUpd()) retep;
	cx HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	ifu (hdl == INVALID_HANDLE_VALUE) rete(ERR_NO_EXIST);
	cx AU end = buf + bufLen;
	AU cur = buf;
	ConCol colOld;
	ife (ConColGet(colOld)) retep;
	while (cur != end) {
		AU first = cur;
		AU last = TO(first)(NUL);
		while ((cur + 2 < end) && !((*cur == '&') && (*(cur + 2) == ';'))) ++cur;
		ConCol colNew;
		BO isColNew = NO;
		if (cur + 2 < end) {
			isColNew = YES;
			cx AU code = *(cur + 1);
			switch (code) {
			case '0': colNew = ConCol::BLACK; break;
			case '1': colNew = ConCol::DARK_BLUE; break;
			case '2': colNew = ConCol::DARK_GREEN; break;
			case '3': colNew = ConCol::DARK_CYAN; break;
			case '4': colNew = ConCol::DARK_RED; break;
			case '5': colNew = ConCol::PURPLE; break;
			case '6': colNew = ConCol::BROWN; break;
			case '7': colNew = ConCol::LIGHT_GRAY; break;
			case '8': colNew = ConCol::DARK_GRAY; break;
			case '9': colNew = ConCol::BLUE; break;
			case 'a': colNew = ConCol::GREEN; break;
			case 'b': colNew = ConCol::CYAN; break;
			case 'c': colNew = ConCol::RED; break;
			case 'd': colNew = ConCol::MAGENTA; break;
			case 'e': colNew = ConCol::YELLOW; break;
			case 'f': colNew = ConCol::WHITE; break;
			default:  colNew = ConCol::WHITE; break;
			}
			last = cur;
			cur += 3;
		} else {
			last = end;
			cur = end;
		}
		DWORD result;
		cx AU writeLen = DWORD(last - first);
		if (writeLen > 0) {
			ifu ((WriteConsoleA(hdl, first, writeLen, &result, NUL) == 0) || (result != writeLen)) {
				ife (ConColSet(colOld)) retep;
				rete(ERR_CON);
			}
		}
		if (isColNew) {
			ife (ConColSet(colNew)) retep;
			ife (_ConColUpd()) retep;
		}
	}
	ife (ConColSet(colOld)) retep;
	rets;
}
dfa ER _ConWriteRawCol(ConCol col, cx CS* buf, SI bufLen) {
	ConCol colOld;
	ife (ConColGet(colOld)) retep;
	ife (ConColSet(col)) retep;
	ife (_ConWriteRaw(buf, bufLen)) {
		ConColSet(colOld);
		retep;
	}
	ife (ConColSet(colOld)) retep;
	rets;
}
dfa ER _ConWriteRawAl(cx CS* format, cx AL& args) {
	CS buf[CON_BUF_LEN_MAX + 1];
	cx SI bufLen = vsnprintf(buf, CON_BUF_LEN_MAX, format, args);
	ifu (bufLen < 0 || bufLen >= CON_BUF_LEN_MAX) rete(ERR_CON);
	ife (_ConWriteRaw(buf, bufLen)) retep;
	rets;
}

dfa BO ConIsExist() {
	ret (GetConsoleWindow() != NUL);
}
dfa ER ConTitleSet(cx CH* title) {
	ife (ConReq()) retep;
	ifu (SetConsoleTitleW(title) == 0) rete(ERR_CON);
	rets;
}
dfa SI ConTitleGet(CH* title, SI titleLenMax) {
	cx SI titleLen = GetConsoleTitleW(title, DWORD(titleLenMax));
	ret titleLen;
}
dfa ER ConCreate(cx CH* title = NUL) {
	ifu (AllocConsole() == 0) rete(ERR_CON);
	if (title != NUL) {
		ife (ConTitleSet(title)) retep;
	}
	ife (_ConColSet(g_conCol)) retep;
	ife (_ConBufEmpty()) retep;
	// font (extra)
	cx HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX font = {};
	font.cbSize = siz(font);
	font.dwFontSize.X = 0;
	font.dwFontSize.Y = 18;
	font.FontWeight = FW_NORMAL;
	StrSet(font.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(hdl, NO, &font);
	//
	ife (WinFocusSet(HD(GetConsoleWindow()))) retep;
	rets;
}
dfa ER ConDel() {
	if (ConIsExist() == NO) rets;
	ifu (FreeConsole() == 0) rete(ERR_CON);
	rets;
}
dfa ER ConReq() {
	if (ConIsExist()) rets;
	ife (ConCreate()) retep;
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
	ife (ConReq()) retep;
	AL args;
	AlNew(args, format);
	ife (_ConWriteRawAl(format, args)) {
		AlDel(args);
		retep;
	}
	AlDel(args);
	rets;
}
dfa ER ConWrite(cx CS* format, ...) {
	ife (ConReq()) retep;
	AL args;
	AlNew(args, format);
	ife (_ConWriteRawAl(format, args)) {
		AlDel(args);
		retep;
	}
	AlDel(args);
	ife (_ConWriteRaw("\n", 1)) retep;
	rets;
}
dfa ER ConWriteDbg(cx CS* format, ...) {
	ifdbg (YES) {
		ife (ConReq()) retep;
		ife (_ConWriteRawCol(ConCol::LIGHT_GRAY, "[DBG] ", 6)) retep;
		AL args;
		AlNew(args, format);
		ife (_ConWriteRawAl(format, args)) {
			AlDel(args);
			retep;
		}
		AlDel(args);
		ife (_ConWriteRaw("\n", 1)) retep;
	}
	rets;
}
dfa ER ConWriteInfo(cx CS* format, ...) {
	ife (ConReq()) retep;
	ife (_ConWriteRawCol(ConCol::GREEN, "[INFO] ", 7)) retep;
	AL args;
	AlNew(args, format);
	ife (_ConWriteRawAl(format, args)) {
		AlDel(args);
		retep;
	}
	AlDel(args);
	ife (_ConWriteRaw("\n", 1)) retep;
	rets;
}
dfa ER ConWriteWarn(cx CS* format, ...) {
	ife (ConReq()) retep;
	ife (_ConWriteRawCol(ConCol::YELLOW, "[WARNING] ", 10)) retep;
	AL args;
	AlNew(args, format);
	ife (_ConWriteRawAl(format, args)) {
		AlDel(args);
		retep;
	}
	AlDel(args);
	ife (_ConWriteRaw("\n", 1)) retep;
	rets;
}
dfa ER ConWriteErr(cx CS* format, ...) {
	ife (ConReq()) retep;
	ife (_ConWriteRawCol(ConCol::RED, "[ERROR] ", 8)) retep;
	AL args;
	AlNew(args, format);
	ife (_ConWriteRawAl(format, args)) {
		AlDel(args);
		retep;
	}
	AlDel(args);
	ife (_ConWriteRaw("\n", 1)) retep;
	rets;
}
dfa ER ConReadStr(CS* str, SI strLenxMax, SI& strLen, BO isShow = YES) {
	strLen = -1;
	ifu (strLenxMax < STR_EX_LEN) rete(ERR_LOW_SIZE);
	strLen = 0;
	str[strLen] = '\0';
	ife (ConReq()) retep;
	ife (_ConBufEmpty()) retep;
	cx AU hdl = GetStdHandle(STD_INPUT_HANDLE);
	ifu (hdl == INVALID_HANDLE_VALUE) rete(ERR_CON);
	SI strPos = 0;
	static vector<string>* s_history = NUL;
	if (s_history == NUL) s_history = new vector<string>;
	SI historyPos = -1;
	while (YES) {
		INPUT_RECORD ir;
		DWORD irRead;
		ifu (PeekConsoleInputW(hdl, &ir, 1, &irRead) == 0) rete(ERR_CON);
		if (irRead == 0) {
			ThdWait(1);
			continue;
		}
		ifu (ReadConsoleInputW(hdl, &ir, 1, &irRead) == 0) rete(ERR_CON);
		if (ir.EventType != KEY_EVENT || !ir.Event.KeyEvent.bKeyDown) continue; // only keep key down
		cx AU& vkCode = ir.Event.KeyEvent.wVirtualKeyCode;
		switch (vkCode) {
		case VK_RETURN: {
			if (historyPos != -1) {
				s_history->pop_back();
			}
			if (s_history->size() == 0 || StrCmp(str, (*s_history)[s_history->size() - 1].c_str()) != 0) {
				if (s_history->size() == CON_HISTORY_CNT_MAX) s_history->erase(s_history->begin());
				s_history->push_back(str);
			}
			ife (_ConWriteRaw("\n", 1)) retep;
			rets;
		}
		case VK_ESCAPE: {
			if (strLen < 1) break;
			if (isShow) {
				cx AU writeLen = strLen;
				vector<CS> buf(writeLen);
				MemSetVal(buf.data(), '\b', strPos * siz(CS));
				ife (_ConWriteRaw(buf.data(), strPos)) retep;
				MemSetVal(buf.data(), ' ', buf.size() * siz(CS));
				ife (_ConWriteRaw(buf.data(), buf.size())) retep;
				MemSetVal(buf.data(), '\b', buf.size() * siz(CS));
				ife (_ConWriteRaw(buf.data(), buf.size())) retep;
			}
			strPos = 0;
			strLen = 0;
			str[strLen] = '\0';
			break;
		}
		case VK_BACK: {
			if (strPos < 1) break;
			--strPos;
			strLen = StrRemove(str, strPos, 1);
			if (isShow) {
				ife (_ConWriteRaw("\b", 1)) retep;
				cx AU writeLen = strLen - strPos;
				ife (_ConWriteRaw(str + strPos, writeLen)) retep;
				ife (_ConWriteRaw(" ", 1)) retep;
				vector<CS> buf(writeLen + 1);
				MemSetVal(buf.data(), '\b', buf.size() * siz(CS));
				ife (_ConWriteRaw(buf.data(), buf.size())) retep;
			}
			break;
		}
		case VK_LEFT: {
			if (strPos > 0) {
				--strPos;
				if (isShow) ife (_ConWriteRaw("\b", 1)) retep;
			}
			break;
		}
		case VK_RIGHT: {
			if (strPos < strLen) {
				++strPos;
				if (isShow) ife (_ConWriteRaw(str + strPos - 1, 1)) retep;
			}
			break;
		}
		case VK_UP:
		case VK_DOWN: {
			if (s_history->size() < 1) break;
			if (vkCode == VK_UP) {
				if (historyPos == -1) {
					if (s_history->size() == CON_HISTORY_CNT_MAX) s_history->erase(s_history->begin());
					s_history->push_back(str);
					historyPos = s_history->size() - 2;
				} else if (historyPos > 0) {
					if (historyPos == SI(s_history->size()) - 1) {
						(*s_history)[historyPos] = str;
					}
					--historyPos;
				} else {
					break;
				}
			} else {
				if ((historyPos != -1) && (historyPos < SI(s_history->size()) - 1)) {
					++historyPos;
				} else {
					break;
				}
			}
			if (isShow) {
				cx AU writeLen = strLen;
				vector<CS> buf(writeLen);
				MemSetVal(buf.data(), '\b', strPos * siz(CS));
				ife (_ConWriteRaw(buf.data(), strPos)) retep;
				MemSetVal(buf.data(), ' ', buf.size() * siz(CS));
				ife (_ConWriteRaw(buf.data(), buf.size())) retep;
				MemSetVal(buf.data(), '\b', buf.size() * siz(CS));
				ife (_ConWriteRaw(buf.data(), buf.size())) retep;
			}
			cx AU& strHistory = (*s_history)[historyPos];
			strLen = Min<SI>(strHistory.size(), strLenxMax - STR_EX_LEN);
			MemSet(str, strHistory.c_str(), strLen * siz(CS));
			str[strLen] = '\0';
			strPos = strLen;
			ife (_ConWriteRaw(str, strLen)) retep;
			break;
		}
		default: {
			if (strLen >= strLenxMax - STR_EX_LEN) break;
			cx CS c = CHToCS(CH(ir.Event.KeyEvent.uChar.UnicodeChar));
			if (c == '\0') break;
			cx CS s[2] = {c, '\0'};
			strLen = StrInsert(str, s, strPos);
			if (isShow) {
				cx AU writeLen = strLen - strPos;
				ife (_ConWriteRaw(str + strPos, writeLen)) retep;
				if (writeLen - 1 > 0) {
					vector<CS> buf(writeLen - 1);
					MemSetVal(buf.data(), '\b', buf.size() * siz(CS));
					ife (_ConWriteRaw(buf.data(), buf.size())) retep;
				}
			}
			++strPos;
			break;
		}
		case VK_TAB:
		case VK_SHIFT:
		case VK_CONTROL:
		case VK_MENU: {
			break;
		}
		}
	}
}
dfa ER ConWait() {
	ife (ConReq()) retep;
	ife (_ConBufEmpty()) retep;
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
