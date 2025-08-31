#pragma once

#if !defined(PROG_COMPILER_GCC) && !defined(PROG_COMPILER_MSVC)
    // try detecting compiler
    #if defined(__GNUC__)
        #define PROG_COMPILER_GCC
    #elif defined(_MSC_VER)
        #define PROG_COMPILER_MSVC
    #else
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

#if defined(PROG_ADR_SIZE_4)
    #define PROG_ADR_SIZE 4
#elif defined(PROG_ADR_SIZE_8)
    #define PROG_ADR_SIZE 8
#endif

#if !defined(PROG_ADR_SIZE)
    // try detecting address size
    #if defined(__SIZEOF_POINTER__)
        #define PROG_ADR_SIZE (__SIZEOF_POINTER__)
    #elif defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64) || defined(__aarch64__) || defined(_M_ARM64)
        #define PROG_ADR_SIZE 8
    #elif defined(__i386__) || defined(_M_IX86) || defined(__arm__) || defined(_M_ARM)
        #define PROG_ADR_SIZE 4
    #else
        #error "PROG_ADR_SIZE is not defined! Define it as [PROG_ADR_SIZE <size>], or either PROG_ADR_SIZE_4 or PROG_ADR_SIZE_8."
    #endif
#endif

#if !defined(PROG_ADR_SIZE_4) && !defined(PROG_ADR_SIZE_8)
    // auto set PROG_ADR_SIZE_X
    #if defined(PROG_ADR_SIZE) && (PROG_ADR_SIZE == 8)
        #define PROG_ADR_SIZE_8
    #elif defined(PROG_ADR_SIZE) && (PROG_ADR_SIZE == 4)
        #define PROG_ADR_SIZE_4
    #else
        #error "PROG_ADR_SIZE is not defined as a supported value!"
    #endif
#endif

#if !defined(PROG_CPU_TYPE_X86_32) && !defined(PROG_CPU_TYPE_X86_64) && !defined(PROG_CPU_TYPE_ARM_32) && !defined(PROG_CPU_TYPE_ARM_64) && !defined(PROG_CPU_TYPE_RISCV_32) && \
    !defined(PROG_CPU_TYPE_RISCV_64) && !defined(PROG_CPU_TYPE_XTENSA)
    // try detecting cpu type
    #if defined(__i386__) || defined(_M_IX86)
        #define PROG_CPU_TYPE_X86_32
    #elif defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64)
        #define PROG_CPU_TYPE_X86_64
    #elif defined(__arm__) || defined(_M_ARM)
        #define PROG_CPU_TYPE_ARM_32
    #elif defined(__aarch64__) || defined(_M_ARM64)
        #define PROG_CPU_TYPE_ARM_64
    #elif defined(__riscv) && (__riscv_xlen == 32)
        #define PROG_CPU_TYPE_RISCV_32
    #elif defined(__riscv) && (__riscv_xlen == 64)
        #define PROG_CPU_TYPE_RISCV_64
    #elif defined(__XTENSA__) || defined(__xtensa__)
        #define PROG_CPU_TYPE_XTENSA
    #else
        #error "PROG_CPU_TYPE is not defined! Define it..."
    #endif
#endif

#if !defined(PROG_SYS_LINUX) && !defined(PROG_SYS_WIN) && !defined(PROG_SYS_ESP32)
    // try detecting system
    #if defined(__linux__)
        #define PROG_SYS_LINUX
    #elif defined(_WIN32) || defined(_WIN64)
        #define PROG_SYS_WIN
    #elif defined(ESP_PLATFORM) || defined(__XTENSA__) || defined(__riscv)
        #define PROG_SYS_ESP32
    #else
        #error "PROG_SYS is not defined! Define either PROG_SYS_LINUX, PROG_SYS_WIN or PROG_SYS_ESP32."
    #endif
#endif

#if !defined(PROG_THD_CNT_SINGLE) && !defined(PROG_THD_CNT_MULTI)
    // default to multiple thread count
    #define PROG_THD_CNT_MULTI

    #if !defined(PROG_THD_CNT_SINGLE) && !defined(PROG_THD_CNT_MULTI)
        #error "PROG_THD_CNT is not defined! Define either PROG_THD_CNT_SINGLE or PROG_THD_CNT_MULTI."
    #endif
#endif

#if !defined(UCPP_MAIN_TYPE_NONE) && !defined(UCPP_MAIN_TYPE_BASE) && !defined(UCPP_MAIN_TYPE_STD) && !defined(UCPP_MAIN_TYPE_DLL)
    // default to standard main type
    #define UCPP_MAIN_TYPE_STD

    #if !defined(UCPP_MAIN_TYPE_NONE) && !defined(UCPP_MAIN_TYPE_BASE) && !defined(UCPP_MAIN_TYPE_STD) && !defined(UCPP_MAIN_TYPE_DLL)
        #error "UCPP_MAIN_TYPE is not defined! Define either UCPP_MAIN_TYPE_NONE, UCPP_MAIN_TYPE_BASE, UCPP_MAIN_TYPE_STD, or UCPP_MAIN_TYPE_DLL."
    #endif
#endif

#if defined(PROG_THD_CNT_MULTI)
    #define IS_THD_SUPPORT 1
#else
    #define IS_THD_SUPPORT 0
#endif

#if defined(__SIZEOF_INT128__)
    #define IS_U16_SUPPORT 1
    #define IS_S16_SUPPORT 1
#else
    #define IS_U16_SUPPORT 0
    #define IS_S16_SUPPORT 0
#endif

#if defined(__SSE__) || (defined(PROG_COMPILER_MSVC) && ((PROG_ADR_SIZE >= 8) || _M_IX86_FP >= 1))
    #define IS_SSE_SUPPORT 1
#else
    #define IS_SSE_SUPPORT 0
#endif

#if defined(__SSE2__) || (defined(PROG_COMPILER_MSVC) && ((PROG_ADR_SIZE >= 8) || _M_IX86_FP >= 2))
    #define IS_SSE2_SUPPORT 1
#else
    #define IS_SSE2_SUPPORT 0
#endif

#if defined(__AVX__)
    #define IS_AVX_SUPPORT 1
#else
    #define IS_AVX_SUPPORT 0
#endif

#ifdef PROG_COMPILER_MSVC
    #pragma warning(disable : 4200) // nonstandard extension used: zero-sized array in struct/union
    #pragma warning(disable : 4996) // _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _HAS_CXX20
    #define _HAS_CXX20 1
#endif
