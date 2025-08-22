#pragma once

#ifdef PROG_SYS_WIN

cxex U1 KEYB_HOOK_THD_CODE_ERR_NO = 0;
cxex U1 KEYB_HOOK_THD_CODE_ERR_YES = 1;
cxex U1 KEYB_HOOK_THD_CODE_WAIT = 2;

cxex SI KEYB_KEY_STR_LEN_MAX = 4;
cxex SI KEYB_KEY_STR_LENX_MAX = KEYB_KEY_STR_LEN_MAX + STR_EX_LEN;

struct KeybKeyEvt
{
    U8 infoDate;
    U4 codeVk;
    U4 codeScan;
    BO isDown;
    CH str[KEYB_KEY_STR_LENX_MAX];
};

BO g_keybThdIsValid = NO;
Thd g_keybThd;
volatile U1 g_keybKeyState[INPUT_KEY_CNT];
volatile BO g_keybKeyStateTake[INPUT_KEY_CNT];
DQueue<KeybKeyEvt> g_keybKeyEvtQueue;
ThdLockFast g_keybKeyEvtQueueLock;
S4 g_keybHookThdDelay;

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
    CH buf[KEYB_KEY_STR_LENX_MAX];
    SI bufSize = ToUnicode(codeVk, codeScan, const_cast<BYTE*>(g_keybKeyState), buf, KEYB_KEY_STR_LENX_MAX, 0);
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

dfa S4 _KeybHookThd(GA code)
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

dfa U1 _KeybKeyState(SI i)
{
    ret g_keybThdIsValid ? g_keybKeyState[i] : U1(GetKeyState(int(i)) & 0xFF);
}

dfa ER KeybInit()
{
    ifu (g_keybThdIsValid)
        rets;
    volatile AU code = KEYB_HOOK_THD_CODE_WAIT;
    ife (g_keybThd.New(_KeybHookThd, const_cast<U1*>(&code)))
        retep;
    while (code == KEYB_HOOK_THD_CODE_WAIT)
        NtYieldExecution_();
    ifu (code == KEYB_HOOK_THD_CODE_ERR_YES)
        rete(ErrVal::HOOK);
    g_keybThdIsValid = YES;
    rets;
}
dfa ER KeybFree()
{
    ifu (!g_keybThdIsValid)
        rets;
    ifu (PostThreadMessageW(DWORD(UA(g_keybThd.Id())), WM_QUIT, 0, 0) == 0)
        rete(ErrVal::THD);
    ife (g_keybThd.Wait())
        retep;
    ife (g_keybThd.Close())
        retep;
    g_keybKeyEvtQueueLock.Lock();
    g_keybKeyEvtQueue.Clr();
    g_keybKeyEvtQueueLock.Unlock();
    g_keybThdIsValid = NO;
    rets;
}

dfa BO KeybKeyIsDown(InputKey key)
{
    cx AU codeVk = InputKeyToCodeVk(key);
    ifu (codeVk >= INPUT_KEY_CNT)
        ret NO;
    ret BO(_KeybKeyState(codeVk) & 0x80);
}
dfa BO KeybKeyIsDownTake(InputKey key, BO isDownReq = NO)
{
    cx AU codeVk = InputKeyToCodeVk(key);
    ifu (codeVk >= INPUT_KEY_CNT)
        ret NO;
    if (isDownReq && !BO(_KeybKeyState(codeVk) & 0x80))
        ret NO;
    ifu (!g_keybThdIsValid)
        ret NO;
    if (g_keybKeyStateTake[codeVk] == NO)
        ret NO;
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

dfa ER KeybKeyWait(InputKey key, BO isSlow = NO, TmMain checkRate = INPUT_KEY_CHECK_RATE_DEFA)
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
    ip.ki.wScan = U2(MapVirtualKeyW(codeVk, MAPVK_VK_TO_VSC));
    ip.ki.dwExtraInfo = GetMessageExtraInfo();
    ifu (SendInput(1, &ip, siz(ip)) != 1)
        rete(ErrVal::KEYB);
    rets;
}
dfa ER KeybKeyPressUp(InputKey key)
{
    AU codeVk = InputKeyToCodeVk(key);
    INPUT ip = {};
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = TO(ip.ki.wVk)(codeVk);
    ip.ki.wScan = U2(MapVirtualKeyW(codeVk, MAPVK_VK_TO_VSC));
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    ip.ki.dwExtraInfo = GetMessageExtraInfo();
    ifu (SendInput(1, &ip, siz(ip)) != 1)
        rete(ErrVal::KEYB);
    rets;
}
dfa ER KeybKeyPress(InputKey key, TmMain hold = INPUT_KEY_HOLD_DEFA, TmMain delay = INPUT_KEY_DELAY_DEFA)
{
    ife (KeybKeyPressDown(key))
        retep;
    ThdWait(hold);
    ife (KeybKeyPressUp(key))
        retep;
    ThdWait(delay);
    rets;
}

#endif
