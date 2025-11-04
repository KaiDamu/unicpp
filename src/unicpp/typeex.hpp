#pragma once

#include "value.hpp"

using VarTypeT = U2;
enum class VarType : VarTypeT
{
    NT = 0,
    U1,
    U2,
    U4,
    U8,
    U16,
    S1,
    S2,
    S4,
    S8,
    S16,
    F4,
    F8,
    CS,
    CH,
    BO,
    ER,
    GA,
    UA,
    SA,
    FA,
    SI,
    SI1,
    SI2,
    SI4,
    SI8,
    ID,
    HD,
};

dfa SI VarTypeSize(VarType varType);

#include "typeex.ipp"
