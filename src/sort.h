#pragma once

tpl1 dfa BO _CmpAsc(cx T1& a, cx T1& b)
{
    ret (b - a) > 0;
}
tpl1 dfa BO _CmpDesc(cx T1& a, cx T1& b)
{
    ret (a - b) > 0;
}

tpl<typename T1, BO Cmp(cx T1&, cx T1&)> dfa NT SortQuick(T1* low, T1* high)
{
    thdlocal static SI s_stackSize = 0;
    thdlocal static T1** s_stack = NUL;
    cx SI stackSizeNew = high - low + 1;
    if (s_stackSize < stackSizeNew)
    {
        s_stackSize = stackSizeNew;
        if (s_stack != NUL)
            delete[] s_stack;
        s_stack = new T1*[s_stackSize];
    }
    cx T1* cx* cx stackBase = s_stack;
    *(s_stack++) = low;
    *(s_stack++) = high;
    do
    {
        high = *(--s_stack);
        low = *(--s_stack);
        cx T1 val = *high;
        T1* ptrA = low - 1;
        T1* ptrB = low;
        while (ptrB < high)
        {
            if (Cmp(*ptrB, val))
            {
                ++ptrA;
                cx T1 tmp = *ptrA;
                *ptrA = *ptrB;
                *ptrB = tmp;
            }
            ++ptrB;
        }
        ++ptrA;
        cx T1 tmp = *ptrA;
        *ptrA = *high;
        *high = tmp;
        if (ptrA - 1 > low)
        {
            *(s_stack++) = low;
            *(s_stack++) = ptrA - 1;
        }
        if (ptrA + 1 < high)
        {
            *(s_stack++) = ptrA + 1;
            *(s_stack++) = high;
        }
    } while (s_stack != stackBase);
}
tpl<typename T1, BO Cmp(cx T1&, cx T1&)> dfa NT SortQuick(T1* buf, SI cnt)
{
    ifu (cnt < 2)
        ret;
    SortQuick<T1, Cmp>(buf, buf + cnt - 1);
}

tpl1 dfa NT SortQuickAsc(T1* buf, SI cnt)
{
    SortQuick<T1, _CmpAsc<T1>>(buf, cnt);
}
tpl1 dfa NT SortQuickDesc(T1* buf, SI cnt)
{
    SortQuick<T1, _CmpDesc<T1>>(buf, cnt);
}
