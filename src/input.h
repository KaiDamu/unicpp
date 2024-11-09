#pragma once

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
enum class InputKeyType : U1
{
    NA = 0,
    KEYB = 1,
    MOUS = 2,
};

// operator for InputKey to int
dfa int operator+(InputKey key)
{
    ret (int)key;
}

dfa U4 InputKeyToCodeVk(InputKey key)
{
    ret U4(key);
}
dfa InputKeyType InputKeyTypeGet(InputKey key)
{
    switch (key)
    {
    case InputKey::NONE:
        ret InputKeyType::NA;
    case InputKey::MOUS_LEFT:
    case InputKey::MOUS_RIGHT:
    case InputKey::MOUS_MID:
    case InputKey::MOUS_EX1:
    case InputKey::MOUS_EX2:
        ret InputKeyType::MOUS;
    default:
        ret InputKeyType::KEYB;
    }
}

constexpr SI INPUT_KEY_CNT = 256;

constexpr TmMain INPUT_KEY_HOLD_DEFA = 50;
constexpr TmMain INPUT_KEY_DELAY_DEFA = 40;
constexpr TmMain INPUT_KEY_CHECK_RATE_DEFA = 1;