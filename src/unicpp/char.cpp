#include "char.hpp"

dfa CS CHToCS(CH val)
{
    if (val > CS_VAL_MAX)
        ret CS(CH_NA);
    ret CS(val);
}
