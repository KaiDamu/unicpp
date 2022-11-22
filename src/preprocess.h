#pragma once

#ifdef PROG_COMPILER_GCC
#else
	#ifdef PROG_COMPILER_MSVC
	#else
		#error "PROG_COMPILER is not defined! (define: *_GCC or *_MSVC)"
	#endif
#endif

#ifdef PROG_BUILD_TYPE_DBG
#else
	#ifdef PROG_BUILD_TYPE_RELEASE
	#else
		#error "PROG_BUILD_TYPE is not defined! (define: *_DBG or *_RELEASE)"
	#endif
#endif

#ifdef PROG_ADR_SIZE_4
#else
	#ifdef PROG_ADR_SIZE_8
	#else
		#error "PROG_ADR_SIZE is not defined! (define: *_4 or *_8)"
	#endif
#endif

#ifdef PROG_SYS_LINUX
#else
	#ifdef PROG_SYS_WIN
	#else
		#error "PROG_SYS is not defined! (define: *_LINUX or *_WIN)"
	#endif
#endif

#ifdef PROG_THD_CNT_SINGLE
#else
	#ifdef PROG_THD_CNT_MULTI
	#else
		#error "PROG_THD_CNT is not defined! (define: *_SINGLE or *_MULTI)"
	#endif
#endif

#ifdef PROG_COMPILER_MSVC
	#pragma warning(disable:4996) // _CRT_SECURE_NO_WARNINGS
#endif
