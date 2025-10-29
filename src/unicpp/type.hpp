#pragma once

#include "keyword.hpp"

using U1 = unsigned char;
using U2 = unsigned short;
using U4 = unsigned int;
using U8 = unsigned long long;
using S1 = signed char;
using S2 = signed short;
using S4 = signed int;
using S8 = signed long long;
using F4 = float;
using F8 = double;

using CS = char;
using CH = wchar_t;
using BO = bool;
using ER = bool;

using GA = void*;
using CXGA = cx void*;
using GAFN = GA (*)();

using NT = void;

#define AU auto
#define TO decltype

#ifdef PROG_ADR_SIZE_4
using UA = U4;
using SA = S4;
using FA = F4;

using SI = S4;

static_assert(siz(GA) == 4, "\"GA\" type size is not 4 byte! (while PROG_ADR_SIZE_4 is defined)");
#endif

#ifdef PROG_ADR_SIZE_8
using UA = U8;
using SA = S8;
using FA = F8;

using SI = S8;

static_assert(siz(GA) == 8, "\"GA\" type size is not 8 byte! (while PROG_ADR_SIZE_8 is defined)");
#endif

using ID = UA;
using HD = GA;

static_assert(siz(U1) == 1, "\"U1\" type size is not 1 byte!");
static_assert(siz(U2) == 2, "\"U2\" type size is not 2 byte!");
static_assert(siz(U4) == 4, "\"U4\" type size is not 4 byte!");
static_assert(siz(U8) == 8, "\"U8\" type size is not 8 byte!");
static_assert(siz(S1) == 1, "\"S1\" type size is not 1 byte!");
static_assert(siz(S2) == 2, "\"S2\" type size is not 2 byte!");
static_assert(siz(S4) == 4, "\"S4\" type size is not 4 byte!");
static_assert(siz(S8) == 8, "\"S8\" type size is not 8 byte!");
static_assert(siz(F4) == 4, "\"F4\" type size is not 4 byte!");
static_assert(siz(F8) == 8, "\"F8\" type size is not 8 byte!");

static_assert(siz(CS) == 1, "\"CS\" type size is not 1 byte! (assumed)");
#ifdef PROG_SYS_LINUX
static_assert(siz(CH) == 4, "\"CH\" type size is not 4 byte! (assumed)");
#else
static_assert(siz(CH) == 2, "\"CH\" type size is not 2 byte! (assumed)");
#endif
static_assert(siz(BO) == 1, "\"BO\" type size is not 1 byte! (assumed)");

#if IS_U16_SUPPORT
using U16 = unsigned __int128;
static_assert(siz(U16) == 16, "\"U16\" type size is not 16 byte!");
#endif

#if IS_S16_SUPPORT
using S16 = signed __int128;
static_assert(siz(S16) == 16, "\"S16\" type size is not 16 byte!");
#endif

#include "type.ipp"
