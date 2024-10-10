#pragma once

#ifdef PROG_BUILD_TYPE_DBG
    #define Assert(cond)                                                                                                                                 \
        {                                                                                                                                                \
            ifu ((cond) == NO)                                                                                                                           \
            {                                                                                                                                            \
                ConWriteErr("Assertion failed!\n\tFile: %s\n\tLine: %u\n\tFunction: %s\n\tDid not happen: %s", __FILE__, __LINE__, __FUNCTION__, #cond); \
                ConWait();                                                                                                                               \
            }                                                                                                                                            \
        }
#else
    #define Assert(cond)
#endif
