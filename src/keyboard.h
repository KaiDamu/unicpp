#pragma once

#ifdef PROG_THD_CNT_SINGLE
    #error "PROG_THD_CNT_SINGLE with UCPP_INCLUDE_KEYB is not supported!"
#endif

constexpr SI KEYB_KEY_STR_LEN_MAX = 4;

constexpr U1 KEYB_HOOK_THD_CODE_ERR_NO = 0;
constexpr U1 KEYB_HOOK_THD_CODE_ERR_YES = 1;
constexpr U1 KEYB_HOOK_THD_CODE_WAIT = 2;

enum class InputKey : U1
{
    NONE = 0x00,
    MOUS_LEFT = 0x01,
    MOUS_RIGHT = 0x02,
    CANCEL = 0x03,
    MOUS_MID = 0x04,
    MOUS_EX1 = 0x05,
    MOUS_EX2 = 0x06,
    BACK = 0x08,
    TAB = 0x09,
    CLEAR = 0x0C,
    ENTER = 0x0D,
    SHIFT = 0x10,
    CTRL = 0x11,
    ALT = 0x12,
    PAUSE = 0x13,
    CAPSLOCK = 0x14,
    ESC = 0x1B,
    SPACE = 0x20,
    PAGEUP = 0x21,
    PAGEDOWN = 0x22,
    END = 0x23,
    HOME = 0x24,
    LEFT = 0x25,
    UP = 0x26,
    RIGHT = 0x27,
    DOWN = 0x28,
    SELECT = 0x29,
    PRINT = 0x2A,
    EXE = 0x2B,
    SNAPSHOT = 0x2C,
    INSERT = 0x2D,
    DEL = 0x2E,
    HELP = 0x2F,
    _0 = 0x30,
    _1 = 0x31,
    _2 = 0x32,
    _3 = 0x33,
    _4 = 0x34,
    _5 = 0x35,
    _6 = 0x36,
    _7 = 0x37,
    _8 = 0x38,
    _9 = 0x39,
    A = 0x41,
    B = 0x42,
    C = 0x43,
    D = 0x44,
    E = 0x45,
    F = 0x46,
    G = 0x47,
    H = 0x48,
    I = 0x49,
    J = 0x4A,
    K = 0x4B,
    L = 0x4C,
    M = 0x4D,
    N = 0x4E,
    O = 0x4F,
    P = 0x50,
    Q = 0x51,
    R = 0x52,
    S = 0x53,
    T = 0x54,
    U = 0x55,
    V = 0x56,
    W = 0x57,
    X = 0x58,
    Y = 0x59,
    Z = 0x5A,
    LWIN = 0x5B,
    RWIN = 0x5C,
    APPS = 0x5D,
    SLEEP = 0x5F,
    NUM0 = 0x60,
    NUM1 = 0x61,
    NUM2 = 0x62,
    NUM3 = 0x63,
    NUM4 = 0x64,
    NUM5 = 0x65,
    NUM6 = 0x66,
    NUM7 = 0x67,
    NUM8 = 0x68,
    NUM9 = 0x69,
    MUL = 0x6A,
    ADD = 0x6B,
    SEPA = 0x6C,
    SUB = 0x6D,
    DEC = 0x6E,
    DIV = 0x6F,
    F1 = 0x70,
    F2 = 0x71,
    F3 = 0x72,
    F4 = 0x73,
    F5 = 0x74,
    F6 = 0x75,
    F7 = 0x76,
    F8 = 0x77,
    F9 = 0x78,
    F10 = 0x79,
    F11 = 0x7A,
    F12 = 0x7B,
    F13 = 0x7C,
    F14 = 0x7D,
    F15 = 0x7E,
    F16 = 0x7F,
    F17 = 0x80,
    F18 = 0x81,
    F19 = 0x82,
    F20 = 0x83,
    F21 = 0x84,
    F22 = 0x85,
    F23 = 0x86,
    F24 = 0x87,
    NUMLOCK = 0x90,
    SCROLLLOCK = 0x91,
    LSHIFT = 0xA0,
    RSHIFT = 0xA1,
    LCTRL = 0xA2,
    RCTRL = 0xA3,
    LALT = 0xA4,
    RALT = 0xA5,
    BROWSER_PREV = 0xA6,
    BROWSER_NEXT = 0xA7,
    BROWSER_REFRESH = 0xA8,
    BROWSER_STOP = 0xA9,
    BROWSER_SRCH = 0xAA,
    BROWSER_FAV = 0xAB,
    BROWSER_HOME = 0xAC,
    VOL_MUTE = 0xAD,
    VOL_DOWN = 0xAE,
    VOL_UP = 0xAF,
    MEDIA_NEXT = 0xB0,
    MEDIA_PREV = 0xB1,
    MEDIA_STOP = 0xB2,
    MEDIA_PLAY = 0xB3,
    START_MAIL = 0xB4,
    START_MEDIA = 0xB5,
    START_APP1 = 0xB6,
    START_APP2 = 0xB7,
    OEM_1 = 0xBA,
    OEM_PLUS = 0xBB,
    OEM_COMMA = 0xBC,
    OEM_MINUS = 0xBD,
    OEM_PERIOD = 0xBE,
    OEM_2 = 0xBF,
    OEM_3 = 0xC0,
    OEM_4 = 0xDB,
    OEM_5 = 0xDC,
    OEM_6 = 0xDD,
    OEM_7 = 0xDE,
    OEM_8 = 0xDF,
    OEM_102 = 0xE2,
    PROCKEY = 0xE5,
    PACKET = 0xE7,
    ATTN = 0xF6,
    CRSEL = 0xF7,
    EXSEL = 0xF8,
    DELEOF = 0xF9,
    PLAY = 0xFA,
    ZOOM = 0xFB,
    PA1 = 0xFD,
    OEM_CLEAR = 0xFE,
};

constexpr SI INPUT_KEY_CNT = 256;

// operator for InputKey to int
dfa int operator+(InputKey key)
{
    ret (int)key;
}

struct KeybKeyEvt
{
    U8 infoDate;
    U4 codeVk;
    U4 codeScan;
    BO isDown;
    CH str[KEYB_KEY_STR_LEN_MAX + 1];
};

Thd g_keybThd;
volatile U1 g_keybKeyState[INPUT_KEY_CNT] = {};
volatile BO g_keybKeyStateTake[INPUT_KEY_CNT] = {};
DQueue<KeybKeyEvt> g_keybKeyEvtQueue;
ThdLock g_keybKeyEvtQueueLock;
S4 g_keybHookThdDelay = 0;

dfa LRESULT CALLBACK _KeybHookCallb(int code, WPARAM wp, LPARAM lp)
{
    if (NO)
    {
        jdst(next);
        ret CallNextHookEx(NUL, code, wp, lp);
    }
    if (code != HC_ACTION)
        jsrc(next);
    BO isDown;
    switch (wp)
    {
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
    ifu (info == NUL)
        jsrc(next);
    cx U4 codeVk = info->vkCode;
    cx U4 codeScan = info->scanCode;
    cx U8 infoDate = U8(info->time);
    // cx BO isExt = (info->flags & LLKHF_EXTENDED) != 0;
    // cx BO isInjectedLow = (info->flags & LLKHF_LOWER_IL_INJECTED) != 0;
    // cx BO isInjected = (info->flags & LLKHF_INJECTED) != 0;
    // cx BO isAltDown = (info->flags & LLKHF_ALTDOWN) != 0;
    // cx BO isUp = (info->flags & LLKHF_UP) != 0;
    ifu (codeVk >= INPUT_KEY_CNT)
        jsrc(next);
    cx AU wasDown = BO(g_keybKeyState[codeVk] & 0x80);
    g_keybKeyState[codeVk] = (g_keybKeyState[codeVk] & (~0x80)) | (isDown * 0x80);
    if (!wasDown && isDown)
    {
        g_keybKeyState[codeVk] = g_keybKeyState[codeVk] ^ 0x01;
        g_keybKeyStateTake[codeVk] = YES;
    }
    switch ((InputKey)codeVk)
    {
    case InputKey::LSHIFT:
        g_keybKeyState[(U1)InputKey::SHIFT] = g_keybKeyState[(U1)InputKey::LSHIFT];
        break;
    case InputKey::RSHIFT:
        g_keybKeyState[(U1)InputKey::SHIFT] = g_keybKeyState[(U1)InputKey::RSHIFT];
        break;
    case InputKey::LCTRL:
        g_keybKeyState[(U1)InputKey::CTRL] = g_keybKeyState[(U1)InputKey::LCTRL];
        break;
    case InputKey::RCTRL:
        g_keybKeyState[(U1)InputKey::CTRL] = g_keybKeyState[(U1)InputKey::RCTRL];
        break;
    case InputKey::LALT:
        g_keybKeyState[(U1)InputKey::ALT] = g_keybKeyState[(U1)InputKey::LALT];
        break;
    case InputKey::RALT:
        g_keybKeyState[(U1)InputKey::ALT] = g_keybKeyState[(U1)InputKey::RALT];
        break;
    default:
        break;
    }
    CH buf[KEYB_KEY_STR_LEN_MAX + 1];
    SI bufSize = ToUnicode(codeVk, codeScan, (BYTE*)g_keybKeyState, buf, KEYB_KEY_STR_LEN_MAX + 1, 0);
    if (bufSize < 0)
        bufSize = 0;
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

dfa DWORD WINAPI _KeybHookThd(LPVOID code)
{
    cx HHOOK hook = SetWindowsHookExW(WH_KEYBOARD_LL, _KeybHookCallb, GetModuleHandleW(NUL), 0);
    ifu (hook == NUL)
    {
        *reinterpret_cast<U1*>(code) = KEYB_HOOK_THD_CODE_ERR_YES;
        ret 1;
    }
    *reinterpret_cast<U1*>(code) = KEYB_HOOK_THD_CODE_ERR_NO;
    ite (i, i < INPUT_KEY_CNT)
    {
        g_keybKeyState[i] = U1(GetKeyState(int(i)) & 0xFF);
        g_keybKeyStateTake[i] = NO;
    }
    g_keybHookThdDelay = 0;
    MSG msg;
    while (YES)
    {
        if (PeekMessageW(&msg, NUL, 0, 0, PM_REMOVE) == 0)
        {
            ++g_keybHookThdDelay;
            if (g_keybHookThdDelay <= 0)
                continue;
            cx AU delay = Clamp<TmMain>(TmMain(g_keybHookThdDelay) / 100 - 500, 1, 100); // maybe this should be configurable
            ThdWait(delay);
        }
        else
        {
            ifu (msg.message == WM_QUIT)
                break;
        }
    }
    UnhookWindowsHookEx(hook);
    ret 0;
}

dfa ER KeybInit()
{
    volatile AU code = KEYB_HOOK_THD_CODE_WAIT;
    ife (g_keybThd.Start(_KeybHookThd, const_cast<U1*>(&code)))
        retep;
    while (code == KEYB_HOOK_THD_CODE_WAIT)
        ;
    ifu (code == KEYB_HOOK_THD_CODE_ERR_YES)
        rete(ERR_HOOK);
    rets;
}
dfa ER KeybFree()
{
    ifu (PostThreadMessageW(g_keybThd.Id(), WM_QUIT, 0, 0) == 0)
        rete(ERR_THD);
    ife (g_keybThd.Wait())
        retep;
    ife (g_keybThd.Close())
        retep;
    g_keybKeyEvtQueueLock.Lock();
    g_keybKeyEvtQueue.Clr();
    g_keybKeyEvtQueueLock.Unlock();
    rets;
}

dfa U4 InputKeyToCodeVk(InputKey key)
{
    ret U4(key);
}

dfa BO KeybKeyIsDown(InputKey key)
{
    cx AU codeVk = InputKeyToCodeVk(key);
    ifu (codeVk >= INPUT_KEY_CNT)
        ret NO;
    ret BO(g_keybKeyState[codeVk] & 0x80);
}
dfa BO KeybKeyIsDownTake(InputKey key, BO isDownReq = NO)
{
    cx AU codeVk = InputKeyToCodeVk(key);
    ifu (codeVk >= INPUT_KEY_CNT)
        ret NO;
    if (isDownReq && !BO(g_keybKeyState[codeVk] & 0x80))
    {
        ret NO;
    }
    if (g_keybKeyStateTake[codeVk] == NO)
    {
        ret NO;
    }
    g_keybKeyStateTake[codeVk] = NO;
    ret YES;
}
dfa BO KeybKeyEvtGet(KeybKeyEvt& keybKeyEvt)
{
    g_keybKeyEvtQueueLock.Lock();
    if (g_keybKeyEvtQueue.IsEmpty())
    {
        g_keybKeyEvtQueueLock.Unlock();
        ret NO;
    }
    g_keybKeyEvtQueue.Get(keybKeyEvt);
    g_keybKeyEvtQueueLock.Unlock();
    ret YES;
}

dfa ER KeybKeyWait(InputKey key, BO isSlow = NO, TmMain checkRate = 1)
{
    if (isSlow == YES)
        while (KeybKeyIsDown(key) == YES)
            ThdWait(checkRate);
    while (KeybKeyIsDown(key) == NO)
        ThdWait(checkRate);
    if (isSlow == YES)
        while (KeybKeyIsDown(key) == YES)
            ThdWait(checkRate);
    rets;
}

dfa ER KeybKeyPressDown(InputKey key)
{
    AU codeVk = InputKeyToCodeVk(key);
    INPUT ip = {};
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = TO(ip.ki.wVk)(codeVk);
    ip.ki.wScan = MapVirtualKeyW(codeVk, MAPVK_VK_TO_VSC);
    ip.ki.dwExtraInfo = GetMessageExtraInfo();
    ifu (SendInput(1, &ip, siz(ip)) != 1)
        rete(ERR_KEYB);
    rets;
}
dfa ER KeybKeyPressUp(InputKey key)
{
    AU codeVk = InputKeyToCodeVk(key);
    INPUT ip = {};
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = TO(ip.ki.wVk)(codeVk);
    ip.ki.wScan = MapVirtualKeyW(codeVk, MAPVK_VK_TO_VSC);
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    ip.ki.dwExtraInfo = GetMessageExtraInfo();
    ifu (SendInput(1, &ip, siz(ip)) != 1)
        rete(ERR_KEYB);
    rets;
}
dfa ER KeybKeyPress(InputKey key, TmMain hold = 50, TmMain delay = 40)
{
    ife (KeybKeyPressDown(key))
        retep;
    ThdWait(hold);
    ife (KeybKeyPressUp(key))
        retep;
    ThdWait(delay);
    rets;
}
