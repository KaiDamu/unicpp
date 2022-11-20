#pragma once

#define ret return
#define cx const
#define tx this
#define inl inline
#define tpl template
#define tpl1 tpl<typename T1>
#define tpl2 tpl<typename T1, typename T2>
#define tpl3 tpl<typename T1, typename T2, typename T3>
#define siz(_) SI(sizeof(_))
#define siz4(_) S4(sizeof(_))
#define siz8(_) S8(sizeof(_))
#define sizbit(_) SI(siz(_) * BIT_IN_BYTE)
#define sizbit4(_) S4(siz(_) * BIT_IN_BYTE)
#define sizbit8(_) S8(siz(_) * BIT_IN_BYTE)
#define nooptimize __attribute__((optimize("O0")))
#define falltru __attribute__((fallthrough))

#define unused(var) static_cast<NT>(var)

#ifdef PROG_COMPILER_GCC
	#define ifu(cond) if (__builtin_expect((cond), NO))
	#define ifl(cond) if (__builtin_expect((cond), YES))
#else
	#define ifu(cond) if (cond)
	#define ifl(cond) if (cond)
#endif

#define ifs(cond) ifl ((cond) == ERR_NO)
#define ife(cond) ifu ((cond) == ERR_YES)

#define jsrc(name) goto name
#define jdst(name) name:
#define ite(i, cond) for (SI i = 0; (cond); ++i)

#define rets ret ERR_NO
#define rete(errVal) { ErrLastValSet(errVal); ret ERR_YES; }
#define retepass ret ERR_YES

#define class_setget(type, var, fn) dfa NT fn(type val) { var = val; } dfa type fn() const { ret var; }

#ifdef PROG_BUILD_TYPE_DBG
	#define ifdbg(cond) if (cond)
	#define ifndbg(cond) if (NO)
#else
	#define ifdbg(cond) if (NO)
	#define ifndbg(cond) if (cond)
#endif

#ifdef PROG_DEFA_FN_ATTRIB
	#define dfa PROG_DEFA_FN_ATTRIB
#else
	#define dfa inl
#endif

#ifdef PROG_THD_CNT_SINGLE
	#define thdlocal
#else
	#define thdlocal thread_local
#endif
