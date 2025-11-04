#include "typeex.hpp"

dfa SI VarTypeSize(VarType varType)
{
    switch (varType)
    {
    case VarType::NT:
        ret 0;
    case VarType::U1:
    case VarType::S1:
    case VarType::SI1:
        ret 1;
    case VarType::U2:
    case VarType::S2:
    case VarType::SI2:
        ret 2;
    case VarType::U4:
    case VarType::S4:
    case VarType::F4:
    case VarType::SI4:
        ret 4;
    case VarType::U8:
    case VarType::S8:
    case VarType::F8:
    case VarType::SI8:
        ret 8;
    case VarType::U16:
    case VarType::S16:
        ret 16;
    case VarType::GA:
    case VarType::UA:
    case VarType::SA:
    case VarType::FA:
    case VarType::SI:
        ret siz(GA);
    case VarType::CS:
        ret siz(CS);
    case VarType::CH:
        ret siz(CH);
    case VarType::BO:
        ret siz(BO);
    case VarType::ER:
        ret siz(ER);
    case VarType::ID:
        ret siz(ID);
    case VarType::HD:
        ret siz(HD);
    default:
        ret -1;
    }
}
