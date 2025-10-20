#pragma once

#include "type.hpp"

struct MainInitCtx
{
#ifndef PROG_SYS_WIN
    SI argc;
    CS** argv;
#endif
};

dfa ER MainInit(MainInitCtx& ctx);
dfa ER MainFree();

#ifndef UCPP_MAIN_TYPE_NONE

dfa ER Main();

dfa ER _Main(MainInitCtx& ctx);

    #if defined(UCPP_MAIN_TYPE_BASE)
extern "C" void _start()
    #elif defined(UCPP_MAIN_TYPE_STD)
        #if defined(PROG_SYS_WIN)
int main() // define UCPP_MAIN_TYPE_NONE if you're using your own main function
        #elif defined(PROG_SYS_ESP32)
extern "C" void app_main() // define UCPP_MAIN_TYPE_NONE if you're using your own main function
        #else
int main(int argc, char** argv) // define UCPP_MAIN_TYPE_NONE if you're using your own main function
        #endif
    #else
inline int _main_unused()
    #endif
    ;

#endif

#ifdef UCPP_MAIN_TYPE_DLL
    #error "UCPP_MAIN_TYPE_DLL is not yet supported!"
#endif

#include "main.ipp"
