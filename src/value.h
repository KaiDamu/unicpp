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

constexpr SI SEC_IN_HOUR = 3600;

constexpr CS CS_VAL_MAX = 0x7F;

constexpr SI U1_LEN_MAX = 3;
constexpr SI U2_LEN_MAX = 5;
constexpr SI U4_LEN_MAX = 10;
constexpr SI U8_LEN_MAX = 20;

constexpr SI S1_LEN_MAX = 4;
constexpr SI S2_LEN_MAX = 6;
constexpr SI S4_LEN_MAX = 11;
constexpr SI S8_LEN_MAX = 20;

#ifdef PROG_ADR_SIZE_4
	constexpr SI SI_VAL_MAX = 0x7FFFFFFF;
	constexpr SI SI_LEN_MAX = S4_LEN_MAX;
#endif

#ifdef PROG_ADR_SIZE_8
	constexpr SI SI_VAL_MAX = 0x7FFFFFFFFFFFFFFF;
	constexpr SI SI_LEN_MAX = S8_LEN_MAX;
#endif

#define NUL 0
