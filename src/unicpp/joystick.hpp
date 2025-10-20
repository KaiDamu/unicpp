#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "time.hpp"
    #include "vector.hpp"
    #include "input.hpp"
#endif

#ifdef PROG_SYS_WIN

cxex U1 _GAMP_LT_DEADZONE = 0x10;
cxex U1 _GAMP_RT_DEADZONE = 0x10;
cxex S2 _GAMP_LS_DEADZONE = 0x1000;
cxex S2 _GAMP_RS_DEADZONE = 0x1000;

cxex U1 _GAMP_LT_THRESHOLD = 0x40;
cxex U1 _GAMP_RT_THRESHOLD = 0x40;
cxex S2 _GAMP_LS_THRESHOLD = 0x2000;
cxex S2 _GAMP_RS_THRESHOLD = 0x2000;

cxex TmMain _GAMP_KEY_STATE_UPD_THRESHOLD = 0.1;

extern U1 g_gampKeyState[INPUT_KEY_CNT];
extern BO g_gampKeyStateTake[INPUT_KEY_CNT];
extern Pos2<S2> g_gampStickPosRaw[2]; // 0: left stick, 1: right stick

struct GampStickState
{
    Pos2<F4> posRaw; // raw position of the stick, range: [-1.0, 1.0]
    F4 len;          // length of the stick movement, range: [0.0, 1.0]
    F4 angle;        // angle of the stick movement, range: [0.0, 2*PI)
    dfa GampStickState();
};

dfa ER _GampStateUpd();

dfa BO GampKeyIsDown(InputKey key);

dfa BO GampKeyIsDownTake(InputKey key, BO isDownReq = NO);

dfa ER GampStickStateGet(GampStickState& left, GampStickState& right);

dfa ER GampVibeSet(F4 left, F4 right);

#endif

#include "joystick.ipp"
