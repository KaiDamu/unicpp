#pragma once

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

U1 g_gampKeyState[INPUT_KEY_CNT];
BO g_gampKeyStateTake[INPUT_KEY_CNT];
Pos2<S2> g_gampStickPosRaw[2]; // 0: left stick, 1: right stick

struct GampStickState
{
    Pos2<F4> posRaw; // raw position of the stick, range: [-1.0, 1.0]
    F4 len;          // length of the stick movement, range: [0.0, 1.0]
    F4 angle;        // angle of the stick movement, range: [0.0, 2*PI)
    dfa GampStickState() : posRaw(0, 0), len(0), angle(0)
    {
    }
};

dfa ER _GampStateUpd()
{
    static TmMain lastUpd = 0.0;
    cx AU timeMain = TimeMain();
    if (timeMain - lastUpd < _GAMP_KEY_STATE_UPD_THRESHOLD)
        rets;
    lastUpd = timeMain;
    XINPUT_STATE state = {};
    cx DWORD result = XInputGetState(0, &state);
    ifu (result != ERROR_SUCCESS)
    {
        ifu (result != ERROR_DEVICE_NOT_CONNECTED)
            rete(ErrVal::GAMP);
        MemSet(g_gampKeyState, 0, siz(g_gampKeyState));
        g_gampStickPosRaw[0] = Pos2<S2>(0, 0);
        g_gampStickPosRaw[1] = Pos2<S2>(0, 0);
        rets;
    }
    static U4 lastPacketNumber = U4(-1);
    if (lastPacketNumber == state.dwPacketNumber)
        rets;
    ifu (lastPacketNumber == U4(-1))
    {
        MemSet(g_gampKeyState, 0, siz(g_gampKeyState));
        MemSet(g_gampKeyStateTake, 0, siz(g_gampKeyStateTake));
    }
    lastPacketNumber = state.dwPacketNumber;
    U1 gampKeyStatePrev[INPUT_KEY_CNT];
    MemCpy(gampKeyStatePrev, g_gampKeyState, siz(g_gampKeyState));
    g_gampKeyState[U1(InputKey::GAMP_UP)] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_DOWN)] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_LEFT)] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_RIGHT)] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_MENU)] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_VIEW)] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_L3)] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_R3)] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_LB)] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_RB)] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_A)] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_B)] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_X)] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_Y)] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_LT)] = (state.Gamepad.bLeftTrigger >= _GAMP_LT_THRESHOLD) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_RT)] = (state.Gamepad.bRightTrigger >= _GAMP_RT_THRESHOLD) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_LS)] = 0x00;
    g_gampKeyState[U1(InputKey::GAMP_RS)] = 0x00;
    g_gampKeyState[U1(InputKey::GAMP_LS_UP)] = (state.Gamepad.sThumbLY > _GAMP_LS_THRESHOLD) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_LS_DOWN)] = (state.Gamepad.sThumbLY < -_GAMP_LS_THRESHOLD) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_LS_LEFT)] = (state.Gamepad.sThumbLX < -_GAMP_LS_THRESHOLD) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_LS_RIGHT)] = (state.Gamepad.sThumbLX > _GAMP_LS_THRESHOLD) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_RS_UP)] = (state.Gamepad.sThumbRY > _GAMP_RS_THRESHOLD) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_RS_DOWN)] = (state.Gamepad.sThumbRY < -_GAMP_RS_THRESHOLD) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_RS_LEFT)] = (state.Gamepad.sThumbRX < -_GAMP_RS_THRESHOLD) ? 0x80 : 0x00;
    g_gampKeyState[U1(InputKey::GAMP_RS_RIGHT)] = (state.Gamepad.sThumbRX > _GAMP_RS_THRESHOLD) ? 0x80 : 0x00;
    ite (i, i < INPUT_KEY_CNT)
    {
        ifl (((g_gampKeyState[i] ^ gampKeyStatePrev[i]) & 0x80) == 0)
            continue;
        if (g_gampKeyState[i] & 0x80)
            g_gampKeyStateTake[i] = YES;
    }
    g_gampStickPosRaw[0].x = state.Gamepad.sThumbLX;
    g_gampStickPosRaw[0].y = state.Gamepad.sThumbLY;
    g_gampStickPosRaw[1].x = state.Gamepad.sThumbRX;
    g_gampStickPosRaw[1].y = state.Gamepad.sThumbRY;
    rets;
}

dfa BO GampKeyIsDown(InputKey key)
{
    cx AU keyVal = U1(key);
    ifu (keyVal >= INPUT_KEY_CNT)
        ret NO;
    ife (_GampStateUpd())
        ret NO;
    ret BO(g_gampKeyState[keyVal] & 0x80);
}

dfa BO GampKeyIsDownTake(InputKey key, BO isDownReq = NO)
{
    Assert(isDownReq == YES); // since there is no thread for asynchronous processing, NO isDownReq is currently not supported
    cx AU keyVal = U1(key);
    ifu (keyVal >= INPUT_KEY_CNT)
        ret NO;
    ife (_GampStateUpd())
        ret NO;
    if (isDownReq && !BO(g_gampKeyState[keyVal] & 0x80))
        ret NO;
    if (g_gampKeyStateTake[keyVal] == NO)
        ret NO;
    g_gampKeyStateTake[keyVal] = NO;
    ret YES;
}

dfa ER GampStickStateGet(GampStickState& left, GampStickState& right)
{
    ife (_GampStateUpd())
        retep;
    left.posRaw.x = F4(g_gampStickPosRaw[0].x) / F4(0x7FFF + (g_gampStickPosRaw[0].x < 0));
    left.posRaw.y = F4(g_gampStickPosRaw[0].y) / F4(0x7FFF + (g_gampStickPosRaw[0].y < 0));
    right.posRaw.x = F4(g_gampStickPosRaw[1].x) / F4(0x7FFF + (g_gampStickPosRaw[1].x < 0));
    right.posRaw.y = F4(g_gampStickPosRaw[1].y) / F4(0x7FFF + (g_gampStickPosRaw[1].y < 0));
    left.len = Dist0(left.posRaw);
    right.len = Dist0(right.posRaw);
    left.angle = RadNorm1(Atan(left.posRaw.x, left.posRaw.y));
    right.angle = RadNorm1(Atan(right.posRaw.x, right.posRaw.y));
    cxex AU ANGLE_RESOLUTION = SI(200);
    static F4* lenMaxAtAngle = NUL;
    ifu (lenMaxAtAngle == NUL)
    {
        lenMaxAtAngle = (F4*)MemNew(2 * ANGLE_RESOLUTION * siz(F4));
        ite (i, i < 2 * ANGLE_RESOLUTION)
            lenMaxAtAngle[i] = 0.975f; // default value
    }
    cx AU leftAngleI = SI(left.angle * F4(ANGLE_RESOLUTION));
    cx AU rightAngleI = SI(right.angle * F4(ANGLE_RESOLUTION));
    lenMaxAtAngle[leftAngleI] = Max<F4>(lenMaxAtAngle[leftAngleI], left.len);
    lenMaxAtAngle[rightAngleI + ANGLE_RESOLUTION] = Max<F4>(lenMaxAtAngle[rightAngleI + ANGLE_RESOLUTION], right.len);
    left.len = Min(left.len + (Max(left.len - F4(1), F4(0))), lenMaxAtAngle[leftAngleI]);
    right.len = Min(right.len + (Max(right.len - F4(1), F4(0))), lenMaxAtAngle[rightAngleI + ANGLE_RESOLUTION]);
    left.len = (left.len < F4(_GAMP_LS_DEADZONE) / F4(0x7FFF)) ? 0 : (left.len / lenMaxAtAngle[leftAngleI]);
    right.len = (right.len < F4(_GAMP_RS_DEADZONE) / F4(0x7FFF)) ? 0 : (right.len / lenMaxAtAngle[rightAngleI + ANGLE_RESOLUTION]);
    left.angle *= Tau<F4>();
    right.angle *= Tau<F4>();
    rets;
}

dfa ER GampVibeSet(F4 left, F4 right)
{
    ifu (!IsBetween<F4>(left, 0, 1) || !IsBetween<F4>(right, 0, 1))
        rete(ErrVal::NO_VALID);
    cx AU curveRaw = AdamEase1CurveRawGet(F4(-0.2));
    cx AU leftMod = AdamEase1(left, curveRaw);
    cx AU rightMod = AdamEase1(right, curveRaw);
    XINPUT_VIBRATION state = {};
    state.wLeftMotorSpeed = U2(leftMod * F4(0xFFFF));
    state.wRightMotorSpeed = U2(rightMod * F4(0xFFFF));
    cx AU result = XInputSetState(0, &state);
    ifu (result != ERROR_SUCCESS)
    {
        ifu (result != ERROR_DEVICE_NOT_CONNECTED)
            rete(ErrVal::NO_SUPPORT);
        rete(ErrVal::NO_EXIST);
    }
    rets;
}

#endif
