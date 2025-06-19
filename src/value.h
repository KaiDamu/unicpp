#pragma once

cxex BO NO = 0;
cxex BO YES = 1;

cxex ER ERR_NO = 0;
cxex ER ERR_YES = 1;

cxex SI BIT_IN_BYTE = 8;

cxex SI BYTE_IN_KB = 1024;
cxex SI BYTE_IN_MB = BYTE_IN_KB * 1024;
cxex SI BYTE_IN_GB = BYTE_IN_MB * 1024;
cxex SI BYTE_IN_TB = BYTE_IN_GB * 1024;

cxex SI USEC_IN_MSEC = 1000;
cxex SI MSEC_IN_SEC = 1000;
cxex SI SEC_IN_MINUTE = 60;
cxex SI MINUTE_IN_HOUR = 60;
cxex SI HOUR_IN_DAY = 24;
cxex SI DAY_IN_WEEK = 7;
cxex SI DAY_IN_MONTH = 30;
cxex SI DAY_IN_YEAR = 365;

cxex SI SEC_IN_HOUR = SEC_IN_MINUTE * MINUTE_IN_HOUR;
cxex SI SEC_IN_DAY = SEC_IN_HOUR * HOUR_IN_DAY;
cxex SI SEC_IN_WEEK = SEC_IN_DAY * DAY_IN_WEEK;
cxex SI SEC_IN_MONTH = SEC_IN_DAY * DAY_IN_MONTH;
cxex SI SEC_IN_YEAR = SEC_IN_DAY * DAY_IN_YEAR;

cxex U1 U1_VAL_MAX = 0xFF;
cxex U2 U2_VAL_MAX = 0xFFFF;
cxex U4 U4_VAL_MAX = 0xFFFFFFFF;
cxex U8 U8_VAL_MAX = 0xFFFFFFFFFFFFFFFF;
cxex S1 S1_VAL_MAX = 0x7F;
cxex S2 S2_VAL_MAX = 0x7FFF;
cxex S4 S4_VAL_MAX = 0x7FFFFFFF;
cxex S8 S8_VAL_MAX = 0x7FFFFFFFFFFFFFFF;

cxex CS CS_VAL_MAX = S1_VAL_MAX;

cxex SI U1_LEN_MAX = 3;
cxex SI U2_LEN_MAX = 5;
cxex SI U4_LEN_MAX = 10;
cxex SI U8_LEN_MAX = 20;
cxex SI S1_LEN_MAX = 4;
cxex SI S2_LEN_MAX = 6;
cxex SI S4_LEN_MAX = 11;
cxex SI S8_LEN_MAX = 20;

#ifdef PROG_ADR_SIZE_4
cxex SI SI_VAL_MAX = S4_VAL_MAX;
cxex SI SI_LEN_MAX = S4_LEN_MAX;
#endif

#ifdef PROG_ADR_SIZE_8
cxex SI SI_VAL_MAX = S8_VAL_MAX;
cxex SI SI_LEN_MAX = S8_LEN_MAX;
#endif

#ifdef PROG_THD_CNT_SINGLE
cxex BO IS_THD_SUPPORT = NO;
#endif

#ifdef PROG_THD_CNT_MULTI
cxex BO IS_THD_SUPPORT = YES;
#endif

tpl1 cxex dfa T1 Pi()
{
    ret static_cast<T1>(3.14159265358979323846);
}
tpl0 cxex dfa F4 Pi<F4>()
{
    ret 3.14159265358979f;
}
tpl0 cxex dfa F8 Pi<F8>()
{
    ret 3.14159265358979323846;
}
tpl1 cxex dfa T1 Tau()
{
    ret Pi<T1>() + Pi<T1>();
}

tpl1 cxex dfa SI VarintSizeMax()
{
    ret (sizb(T1) + 6) / 7;
}
tpl<typename T1, U1 TBase> cxex dfa SI VarbaseintSizeMax()
{
    static_assert(TBase > 0 && TBase < 8, "TBase must be in (0, 8)");
    ret (sizb(T1) + (TBase - 1)) / TBase;
}

#define NUL 0
