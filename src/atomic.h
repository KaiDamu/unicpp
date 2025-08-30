#pragma once

dfa BO AtomCmpSetU4(volatile U4& dst, U4 cmp, U4 set)
{
#if defined(PROG_SYS_WIN)
    ret (U4(InterlockedCompareExchange((volatile S4*)&dst, set, cmp)) == cmp);
#elif defined(PROG_SYS_ESP32)
    ret esp_cpu_compare_and_set(&dst, cmp, set);
#elif defined(PROG_COMPILER_GCC)
    ret __atomic_compare_exchange_n(&dst, &cmp, set, NO, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#else
    unimp;
    ret NO;
#endif
}
