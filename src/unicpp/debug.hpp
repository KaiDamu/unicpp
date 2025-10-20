#pragma once

#include "keyword.hpp"

#ifdef PROG_BUILD_TYPE_DBG

dfa ER ConWriteErr(cx CS* form, ...);
dfa ER ConWait();

    #define Assert(cond)                                                                                                                                 \
        {                                                                                                                                                \
            ifu ((cond) == NO)                                                                                                                           \
            {                                                                                                                                            \
                ConWriteErr("Assertion failed!\n\tFile: %s\n\tLine: %u\n\tFunction: %s\n\tDid not happen: %s", __FILE__, __LINE__, __FUNCTION__, #cond); \
                ConWait();                                                                                                                               \
            }                                                                                                                                            \
        }

    #include "console.hpp"

#else

    #define Assert(cond)

#endif

#include "debug.ipp"
