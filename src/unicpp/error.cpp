#include "error.hpp"

thdlocal ErrVal g_errLastVal = ErrVal::NONE;

dfa NT ErrLastSet(ErrVal val)
{
    g_errLastVal = val;
}
dfa ErrVal ErrLastGet()
{
    ret g_errLastVal;
}
