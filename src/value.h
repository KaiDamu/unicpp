#pragma once

constexpr BO NO = 0;
constexpr BO YES = 1;

constexpr ER ERR_NO = 0;
constexpr ER ERR_YES = 1;

constexpr SI BIT_IN_BYTE = 8;
constexpr SI SEC_IN_HOUR = 3600;

#ifdef PROG_ADR_SIZE_4
	constexpr SI SI_VAL_MAX = 0x7FFFFFFF;
#endif

#ifdef PROG_ADR_SIZE_8
	constexpr SI SI_VAL_MAX = 0x7FFFFFFFFFFFFFFF;
#endif

#define NUL 0
