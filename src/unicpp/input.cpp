#include "input.hpp"

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
