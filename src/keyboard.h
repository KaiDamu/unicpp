#pragma once

#ifdef PROG_THD_CNT_SINGLE
	#error "PROG_THD_CNT_SINGLE with INCLUDE_KEYB is not supported!"
#endif

constexpr SI KEYB_KEY_CNT = 256;
constexpr SI KEYB_KEY_STR_LEN_MAX = 4;

constexpr U1 KEYB_HOOK_THD_CODE_ERR_NO = 0;
constexpr U1 KEYB_HOOK_THD_CODE_ERR_YES = 1;
constexpr U1 KEYB_HOOK_THD_CODE_WAIT = 2;

enum CodeVk : U4 {
	MOUS_KEY_LEFT = 0x01,
	MOUS_KEY_RIGHT = 0x02,
	KEYB_KEY_CANCEL = 0x03,
	MOUS_KEY_MID = 0x04,
	MOUS_KEY_EX1 = 0x05,
	MOUS_KEY_EX2 = 0x06,
	KEYB_KEY_BACK = 0x08,
	KEYB_KEY_TAB = 0x09,
	KEYB_KEY_CLEAR = 0x0C,
	KEYB_KEY_ENTER = 0x0D,
	KEYB_KEY_SHIFT = 0x10,
	KEYB_KEY_CTRL = 0x11,
	KEYB_KEY_ALT = 0x12,
	KEYB_KEY_PAUSE = 0x13,
	KEYB_KEY_CAPSLOCK = 0x14,
	KEYB_KEY_ESC = 0x1B,
	KEYB_KEY_SPACE = 0x20,
	KEYB_KEY_PAGEUP = 0x21,
	KEYB_KEY_PAGEDOWN = 0x22,
	KEYB_KEY_END = 0x23,
	KEYB_KEY_HOME = 0x24,
	KEYB_KEY_LEFT = 0x25,
	KEYB_KEY_UP = 0x26,
	KEYB_KEY_RIGHT = 0x27,
	KEYB_KEY_DOWN = 0x28,
	KEYB_KEY_SELECT = 0x29,
	KEYB_KEY_PRINT = 0x2A,
	KEYB_KEY_EXE = 0x2B,
	KEYB_KEY_SNAPSHOT = 0x2C,
	KEYB_KEY_INSERT = 0x2D,
	KEYB_KEY_DEL = 0x2E,
	KEYB_KEY_HELP = 0x2F,
	KEYB_KEY_0 = 0x30,
	KEYB_KEY_1 = 0x31,
	KEYB_KEY_2 = 0x32,
	KEYB_KEY_3 = 0x33,
	KEYB_KEY_4 = 0x34,
	KEYB_KEY_5 = 0x35,
	KEYB_KEY_6 = 0x36,
	KEYB_KEY_7 = 0x37,
	KEYB_KEY_8 = 0x38,
	KEYB_KEY_9 = 0x39,
	KEYB_KEY_A = 0x41,
	KEYB_KEY_B = 0x42,
	KEYB_KEY_C = 0x43,
	KEYB_KEY_D = 0x44,
	KEYB_KEY_E = 0x45,
	KEYB_KEY_F = 0x46,
	KEYB_KEY_G = 0x47,
	KEYB_KEY_H = 0x48,
	KEYB_KEY_I = 0x49,
	KEYB_KEY_J = 0x4A,
	KEYB_KEY_K = 0x4B,
	KEYB_KEY_L = 0x4C,
	KEYB_KEY_M = 0x4D,
	KEYB_KEY_N = 0x4E,
	KEYB_KEY_O = 0x4F,
	KEYB_KEY_P = 0x50,
	KEYB_KEY_Q = 0x51,
	KEYB_KEY_R = 0x52,
	KEYB_KEY_S = 0x53,
	KEYB_KEY_T = 0x54,
	KEYB_KEY_U = 0x55,
	KEYB_KEY_V = 0x56,
	KEYB_KEY_W = 0x57,
	KEYB_KEY_X = 0x58,
	KEYB_KEY_Y = 0x59,
	KEYB_KEY_Z = 0x5A,
	KEYB_KEY_LWIN = 0x5B,
	KEYB_KEY_RWIN = 0x5C,
	KEYB_KEY_APPS = 0x5D,
	KEYB_KEY_SLEEP = 0x5F,
	KEYB_KEY_NUM0 = 0x60,
	KEYB_KEY_NUM1 = 0x61,
	KEYB_KEY_NUM2 = 0x62,
	KEYB_KEY_NUM3 = 0x63,
	KEYB_KEY_NUM4 = 0x64,
	KEYB_KEY_NUM5 = 0x65,
	KEYB_KEY_NUM6 = 0x66,
	KEYB_KEY_NUM7 = 0x67,
	KEYB_KEY_NUM8 = 0x68,
	KEYB_KEY_NUM9 = 0x69,
	KEYB_KEY_MUL = 0x6A,
	KEYB_KEY_ADD = 0x6B,
	KEYB_KEY_SEPA = 0x6C,
	KEYB_KEY_SUB = 0x6D,
	KEYB_KEY_DEC = 0x6E,
	KEYB_KEY_DIV = 0x6F,
	KEYB_KEY_F1 = 0x70,
	KEYB_KEY_F2 = 0x71,
	KEYB_KEY_F3 = 0x72,
	KEYB_KEY_F4 = 0x73,
	KEYB_KEY_F5 = 0x74,
	KEYB_KEY_F6 = 0x75,
	KEYB_KEY_F7 = 0x76,
	KEYB_KEY_F8 = 0x77,
	KEYB_KEY_F9 = 0x78,
	KEYB_KEY_F10 = 0x79,
	KEYB_KEY_F11 = 0x7A,
	KEYB_KEY_F12 = 0x7B,
	KEYB_KEY_F13 = 0x7C,
	KEYB_KEY_F14 = 0x7D,
	KEYB_KEY_F15 = 0x7E,
	KEYB_KEY_F16 = 0x7F,
	KEYB_KEY_F17 = 0x80,
	KEYB_KEY_F18 = 0x81,
	KEYB_KEY_F19 = 0x82,
	KEYB_KEY_F20 = 0x83,
	KEYB_KEY_F21 = 0x84,
	KEYB_KEY_F22 = 0x85,
	KEYB_KEY_F23 = 0x86,
	KEYB_KEY_F24 = 0x87,
	KEYB_KEY_NUMLOCK = 0x90,
	KEYB_KEY_SCROLLLOCK = 0x91,
	KEYB_KEY_LSHIFT = 0xA0,
	KEYB_KEY_RSHIFT = 0xA1,
	KEYB_KEY_LCTRL = 0xA2,
	KEYB_KEY_RCTRL = 0xA3,
	KEYB_KEY_LALT = 0xA4,
	KEYB_KEY_RALT = 0xA5,
	KEYB_KEY_BROWSER_PREV = 0xA6,
	KEYB_KEY_BROWSER_NEXT = 0xA7,
	KEYB_KEY_BROWSER_REFRESH = 0xA8,
	KEYB_KEY_BROWSER_STOP = 0xA9,
	KEYB_KEY_BROWSER_SRCH = 0xAA,
	KEYB_KEY_BROWSER_FAV = 0xAB,
	KEYB_KEY_BROWSER_HOME = 0xAC,
	KEYB_KEY_VOL_MUTE = 0xAD,
	KEYB_KEY_VOL_DOWN = 0xAE,
	KEYB_KEY_VOL_UP = 0xAF,
	KEYB_KEY_MEDIA_NEXT = 0xB0,
	KEYB_KEY_MEDIA_PREV = 0xB1,
	KEYB_KEY_MEDIA_STOP = 0xB2,
	KEYB_KEY_MEDIA_PLAY = 0xB3,
	KEYB_KEY_START_MAIL = 0xB4,
	KEYB_KEY_START_MEDIA = 0xB5,
	KEYB_KEY_START_APP1 = 0xB6,
	KEYB_KEY_START_APP2 = 0xB7,
	KEYB_KEY_OEM_1 = 0xBA,
	KEYB_KEY_OEM_PLUS = 0xBB,
	KEYB_KEY_OEM_COMMA = 0xBC,
	KEYB_KEY_OEM_MINUS = 0xBD,
	KEYB_KEY_OEM_PERIOD = 0xBE,
	KEYB_KEY_OEM_2 = 0xBF,
	KEYB_KEY_OEM_3 = 0xC0,
	KEYB_KEY_OEM_4 = 0xDB,
	KEYB_KEY_OEM_5 = 0xDC,
	KEYB_KEY_OEM_6 = 0xDD,
	KEYB_KEY_OEM_7 = 0xDE,
	KEYB_KEY_OEM_8 = 0xDF,
	KEYB_KEY_OEM_102 = 0xE2,
	KEYB_KEY_PROCKEY = 0xE5,
	KEYB_KEY_PACKET = 0xE7,
	KEYB_KEY_ATTN = 0xF6,
	KEYB_KEY_CRSEL = 0xF7,
	KEYB_KEY_EXSEL = 0xF8,
	KEYB_KEY_DELEOF = 0xF9,
	KEYB_KEY_PLAY = 0xFA,
	KEYB_KEY_ZOOM = 0xFB,
	KEYB_KEY_PA1 = 0xFD,
	KEYB_KEY_OEM_CLEAR = 0xFE,
};

struct KeybKeyEvt {
	U8 infoDate;
	U4 codeVk;
	U4 codeScan;
	BO isDown;
	CH str[KEYB_KEY_STR_LEN_MAX + 1];
};

Thd g_keybThd;
U1 g_keybKeyState[KEYB_KEY_CNT] = {};
DQueue<KeybKeyEvt> g_keybKeyEvtQueue;
ThdLock g_keybKeyEvtQueueLock;
S4 g_keybHookThdDelay = 0;

dfa LRESULT CALLBACK _KeybHookCallb(int code, WPARAM wp, LPARAM lp) {
	if (NO) {
		jdst(next);
		ret CallNextHookEx(NUL, code, wp, lp);
	}
	if (code != HC_ACTION) jsrc(next);
	BO isDown;
	switch (wp) {
	case WM_KEYUP:
	case WM_SYSKEYUP:
		isDown = NO;
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		isDown = YES;
		break;
	default:
		jsrc(next);
	}
	cx KBDLLHOOKSTRUCT* info = reinterpret_cast<KBDLLHOOKSTRUCT*>(lp);
	ifu (info == NUL) jsrc(next);
	cx U4 codeVk = info->vkCode;
	cx U4 codeScan = info->scanCode;
	cx U8 infoDate = U8(info->time);
	//cx BO isExt = (info->flags & LLKHF_EXTENDED) != 0;
	//cx BO isInjectedLow = (info->flags & LLKHF_LOWER_IL_INJECTED) != 0;
	//cx BO isInjected = (info->flags & LLKHF_INJECTED) != 0;
	//cx BO isAltDown = (info->flags & LLKHF_ALTDOWN) != 0;
	//cx BO isUp = (info->flags & LLKHF_UP) != 0;
	ifu (codeVk >= KEYB_KEY_CNT) jsrc(next);
	g_keybKeyState[codeVk] = (g_keybKeyState[codeVk] & (~0x80)) | (isDown * 0x80);
	if (isDown) g_keybKeyState[codeVk] ^= 0x01;
	switch (codeVk) {
	case KEYB_KEY_LSHIFT: g_keybKeyState[KEYB_KEY_SHIFT] = g_keybKeyState[KEYB_KEY_LSHIFT]; break;
	case KEYB_KEY_RSHIFT: g_keybKeyState[KEYB_KEY_SHIFT] = g_keybKeyState[KEYB_KEY_RSHIFT]; break;
	case KEYB_KEY_LCTRL: g_keybKeyState[KEYB_KEY_CTRL] = g_keybKeyState[KEYB_KEY_LCTRL]; break;
	case KEYB_KEY_RCTRL: g_keybKeyState[KEYB_KEY_CTRL] = g_keybKeyState[KEYB_KEY_RCTRL]; break;
	case KEYB_KEY_LALT: g_keybKeyState[KEYB_KEY_ALT] = g_keybKeyState[KEYB_KEY_LALT]; break;
	case KEYB_KEY_RALT: g_keybKeyState[KEYB_KEY_ALT] = g_keybKeyState[KEYB_KEY_RALT]; break;
	default: break;
	}
	CH buf[KEYB_KEY_STR_LEN_MAX + 1];
	SI bufSize = ToUnicode(codeVk, codeScan, (BYTE*)g_keybKeyState, buf, KEYB_KEY_STR_LEN_MAX + 1, 0);
	if (bufSize < 0) bufSize = 0;
	buf[bufSize] = '\0';
	KeybKeyEvt keybKey = {};
	keybKey.infoDate = infoDate;
	keybKey.codeVk = codeVk;
	keybKey.codeScan = codeScan;
	keybKey.isDown = isDown;
	MemCpy(keybKey.str, buf, (bufSize + 1) * siz(buf[0]));
	g_keybKeyEvtQueueLock.Lock();
	g_keybKeyEvtQueue.Add(keybKey);
	g_keybKeyEvtQueueLock.Unlock();
	g_keybHookThdDelay = 0;
	jsrc(next);
}

dfa DWORD WINAPI _KeybHookThd(LPVOID code) {
	cx HHOOK hook = SetWindowsHookExW(WH_KEYBOARD_LL, _KeybHookCallb, GetModuleHandleW(NUL), 0);
	ifu (hook == NUL) {
		*reinterpret_cast<U1*>(code) = KEYB_HOOK_THD_CODE_ERR_YES;
		ret 1;
	}
	*reinterpret_cast<U1*>(code) = KEYB_HOOK_THD_CODE_ERR_NO;
	ite (i, i < KEYB_KEY_CNT) {
		g_keybKeyState[i] = U1(GetKeyState(i) & 0xFF);
	}
	g_keybHookThdDelay = 0;
	MSG msg;
	while (YES) {
		if (PeekMessageW(&msg, NUL, 0, 0, PM_REMOVE) == 0) {
			++g_keybHookThdDelay;
			if (g_keybHookThdDelay <= 0) continue;
			cx S4 delay = Clamp<S4>(g_keybHookThdDelay / 100 - 500, 1, 100); // maybe this should be configurable
			ThdWait(delay);
		} else {
			ifu (msg.message == WM_QUIT) break;
		}
	}
	UnhookWindowsHookEx(hook);
	ret 0;
}

dfa ER KeybInit() {
	volatile AU code = KEYB_HOOK_THD_CODE_WAIT;
	ife (g_keybThd.Start(_KeybHookThd, const_cast<U1*>(&code))) retepass;
	while (code == KEYB_HOOK_THD_CODE_WAIT);
	ifu (code == KEYB_HOOK_THD_CODE_ERR_YES) rete(ERR_HOOK);
	rets;
}
dfa ER KeybFree() {
	ifu (PostThreadMessageW(g_keybThd.Id(), WM_QUIT, 0, 0) == 0) rete(ERR_THD);
	ife (g_keybThd.Wait()) retepass;
	ife (g_keybThd.Close()) retepass;
	g_keybKeyEvtQueueLock.Lock();
	g_keybKeyEvtQueue.Free();
	g_keybKeyEvtQueueLock.Unlock();
	rets;
}

dfa BO KeybKeyIsDown(U4 codeVk) {
	ifu (codeVk >= KEYB_KEY_CNT) ret NO;
	ret BO(g_keybKeyState[codeVk] & 0x80);
}
dfa BO KeybKeyEvtGet(KeybKeyEvt& keybKeyEvt) {
	g_keybKeyEvtQueueLock.Lock();
	if (g_keybKeyEvtQueue.IsEmpty()) {
		g_keybKeyEvtQueueLock.Unlock();
		ret NO;
	}
	g_keybKeyEvtQueue.Get(keybKeyEvt);
	g_keybKeyEvtQueueLock.Unlock();
	ret YES;
}
