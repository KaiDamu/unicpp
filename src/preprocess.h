#pragma once

#if !defined(PROG_COMPILER_GCC) && !defined(PROG_COMPILER_MSVC)
    // try detecting compiler
    #if defined(__GNUC__)
        #define PROG_COMPILER_GCC
    #elif defined(_MSC_VER)
        #define PROG_COMPILER_MSVC
    #endif

    #if !defined(PROG_COMPILER_GCC) && !defined(PROG_COMPILER_MSVC)
        #error "PROG_COMPILER is not defined! Define either PROG_COMPILER_GCC or PROG_COMPILER_MSVC."
    #endif
#endif

#if !defined(PROG_BUILD_TYPE_DBG) && !defined(PROG_BUILD_TYPE_RELEASE)
    // default to release build type
    #define PROG_BUILD_TYPE_RELEASE

    #if !defined(PROG_BUILD_TYPE_DBG) && !defined(PROG_BUILD_TYPE_RELEASE)
        #error "PROG_BUILD_TYPE is not defined! Define either PROG_BUILD_TYPE_DBG or PROG_BUILD_TYPE_RELEASE."
    #endif
#endif

#if !defined(PROG_ADR_SIZE_4) && !defined(PROG_ADR_SIZE_8)
    // try detecting address size
    #if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) || defined(_M_ARM64)
        #define PROG_ADR_SIZE_8
    #elif defined(__i386__) || defined(_M_IX86) || defined(__arm__) || defined(_M_ARM)
        #define PROG_ADR_SIZE_4
    #endif

    #if !defined(PROG_ADR_SIZE_4) && !defined(PROG_ADR_SIZE_8)
        #error "PROG_ADR_SIZE is not defined! Define either PROG_ADR_SIZE_4 or PROG_ADR_SIZE_8."
    #endif
#endif

#if !defined(PROG_SYS_LINUX) && !defined(PROG_SYS_WIN)
    // try detecting system
    #if defined(__linux__)
        #define PROG_SYS_LINUX
    #elif defined(_WIN32) || defined(_WIN64)
        #define PROG_SYS_WIN
    #endif

    #if !defined(PROG_SYS_LINUX) && !defined(PROG_SYS_WIN)
        #error "PROG_SYS is not defined! Define either PROG_SYS_LINUX or PROG_SYS_WIN."
    #endif
#endif

#if !defined(PROG_THD_CNT_SINGLE) && !defined(PROG_THD_CNT_MULTI)
    // default to multiple thread count
    #define PROG_THD_CNT_MULTI

    #if !defined(PROG_THD_CNT_SINGLE) && !defined(PROG_THD_CNT_MULTI)
        #error "PROG_THD_CNT is not defined! Define either PROG_THD_CNT_SINGLE or PROG_THD_CNT_MULTI."
    #endif
#endif

#ifdef PROG_COMPILER_MSVC
    #pragma warning(disable : 4996) // _CRT_SECURE_NO_WARNINGS
#endif
