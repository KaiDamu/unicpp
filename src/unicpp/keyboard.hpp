#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "string.hpp"
    #include "queue.hpp"
    #include "lock.hpp"
    #include "thread.hpp"
    #include "input.hpp"
#endif

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

extern BO g_keybThdIsValid;
extern Thd g_keybThd;
extern volatile U1 g_keybKeyState[INPUT_KEY_CNT];
extern volatile BO g_keybKeyStateTake[INPUT_KEY_CNT];
extern DQueue<KeybKeyEvt> g_keybKeyEvtQueue;
extern ThdLockFast g_keybKeyEvtQueueLock;
extern S4 g_keybHookThdDelay;

dfa LRESULT CALLBACK _KeybHookCallb(int code, WPARAM wp, LPARAM lp);

dfa S4 _KeybHookThd(GA code);

dfa U1 _KeybKeyState(SI i);

dfa ER KeybInit();
dfa ER KeybFree();

dfa BO KeybKeyIsDown(InputKey key);
dfa BO KeybKeyIsDownTake(InputKey key, BO isDownReq = NO);
dfa BO KeybKeyEvtGet(KeybKeyEvt& keybKeyEvt);

dfa ER KeybKeyWait(InputKey key, BO isSlow = NO, TmMain checkRate = INPUT_KEY_CHECK_RATE_DEFA);

dfa ER KeybKeyPressDown(InputKey key);
dfa ER KeybKeyPressUp(InputKey key);
dfa ER KeybKeyPress(InputKey key, TmMain hold = INPUT_KEY_HOLD_DEFA, TmMain delay = INPUT_KEY_DELAY_DEFA);

#endif

#include "keyboard.ipp"
