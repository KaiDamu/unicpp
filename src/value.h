#pragma once

constexpr BO NO = 0;
constexpr BO YES = 1;

constexpr ER ERR_NO = 0;
constexpr ER ERR_YES = 1;

constexpr SI BIT_IN_BYTE = 8;

constexpr SI BYTE_IN_KB = 1024;
constexpr SI BYTE_IN_MB = BYTE_IN_KB * 1024;
constexpr SI BYTE_IN_GB = BYTE_IN_MB * 1024;
constexpr SI BYTE_IN_TB = BYTE_IN_GB * 1024;

constexpr SI USEC_IN_MSEC = 1000;
constexpr SI MSEC_IN_SEC = 1000;
constexpr SI SEC_IN_MINUTE = 60;
constexpr SI MINUTE_IN_HOUR = 60;
constexpr SI HOUR_IN_DAY = 24;
constexpr SI DAY_IN_WEEK = 7;
constexpr SI DAY_IN_MONTH = 30;
constexpr SI DAY_IN_YEAR = 365;

constexpr SI SEC_IN_HOUR = SEC_IN_MINUTE * MINUTE_IN_HOUR;
constexpr SI SEC_IN_DAY = SEC_IN_HOUR * HOUR_IN_DAY;
constexpr SI SEC_IN_WEEK = SEC_IN_DAY * DAY_IN_WEEK;
constexpr SI SEC_IN_MONTH = SEC_IN_DAY * DAY_IN_MONTH;
constexpr SI SEC_IN_YEAR = SEC_IN_DAY * DAY_IN_YEAR;

constexpr U1 U1_VAL_MAX = 0xFF;
constexpr U2 U2_VAL_MAX = 0xFFFF;
constexpr U4 U4_VAL_MAX = 0xFFFFFFFF;
constexpr U8 U8_VAL_MAX = 0xFFFFFFFFFFFFFFFF;
constexpr S1 S1_VAL_MAX = 0x7F;
constexpr S2 S2_VAL_MAX = 0x7FFF;
constexpr S4 S4_VAL_MAX = 0x7FFFFFFF;
constexpr S8 S8_VAL_MAX = 0x7FFFFFFFFFFFFFFF;

constexpr CS CS_VAL_MAX = S1_VAL_MAX;

constexpr SI U1_LEN_MAX = 3;
constexpr SI U2_LEN_MAX = 5;
constexpr SI U4_LEN_MAX = 10;
constexpr SI U8_LEN_MAX = 20;
constexpr SI S1_LEN_MAX = 4;
constexpr SI S2_LEN_MAX = 6;
constexpr SI S4_LEN_MAX = 11;
constexpr SI S8_LEN_MAX = 20;

#ifdef PROG_ADR_SIZE_4
constexpr SI SI_VAL_MAX = S4_VAL_MAX;
constexpr SI SI_LEN_MAX = S4_LEN_MAX;
#endif

#ifdef PROG_ADR_SIZE_8
constexpr SI SI_VAL_MAX = S8_VAL_MAX;
constexpr SI SI_LEN_MAX = S8_LEN_MAX;
#endif

#ifdef PROG_THD_CNT_SINGLE
constexpr BO IS_THD_SUPPORT = NO;
#endif

#ifdef PROG_THD_CNT_MULTI
constexpr BO IS_THD_SUPPORT = YES;
#endif

tpl1 constexpr dfa T1 Pi()
{
    ret static_cast<T1>(3.14159265358979323846);
}
tpl0 constexpr dfa F4 Pi<F4>()
{
    ret 3.14159265358979f;
}
tpl0 constexpr dfa F8 Pi<F8>()
{
    ret 3.14159265358979323846;
}

tpl1 constexpr dfa SI VarintSizeMax()
{
    ret (sizb(T1) + 6) / 7;
}
tpl<typename T1, U1 TBase> constexpr dfa SI VarbaseintSizeMax()
{
    static_assert(TBase > 0 && TBase < 8, "TBase must be in (0, 8)");
    ret (sizb(T1) + (TBase - 1)) / TBase;
}

#define NUL 0
