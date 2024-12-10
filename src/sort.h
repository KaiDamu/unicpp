#pragma once

tpl1 dfa BO _CmpAsc(cx T1& a, cx T1& b)
{
    ret a < b;
}
tpl1 dfa BO _CmpDesc(cx T1& a, cx T1& b)
{
    ret a > b;
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
                Swap<T1>(*ptrA, *ptrB);
            }
            ++ptrB;
        }
        ++ptrA;
        Swap<T1>(*ptrA, *high);
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

dfa NT SortRadixAscF4(F4* buf, SI cnt)
{
    // @Credit: Based on the approach from "kspalaiologos" (Kamila Szewczyk)

    AU buf1 = (U4*)buf;

    cxex AU BLOCK_CNT = SI(0x100);
    U4 dat[BLOCK_CNT * 3];
    MemSet(dat, 0, BLOCK_CNT * 3 * siz(*dat));
    AU b1 = dat;
    AU b2 = dat + BLOCK_CNT;
    AU si = dat + BLOCK_CNT * 2;

    AU p = buf1;
    AU pEnd = buf1 + cnt;
    while (p != pEnd)
        *p++ ^= 0x80000000;

    cx AU bucketScan = [&]() {
        *si = 0;
        ite (i, i < BLOCK_CNT - 1)
            si[i + 1] = si[i] + b1[i];
    };

    AU buf2 = TO(buf1)(calloc(cnt, siz(*buf1)));

    {
        U4 key = 0;

        SI i = 0;
        if (cnt & 1)
            ++b1[ByteIVal<0>(buf1[i++])];
        while (i < cnt)
            ++b1[ByteIVal<0>(buf1[i++])], ++b2[ByteIVal<0>(buf1[i++])];
        for (i = 0; i < BLOCK_CNT; ++i)
            b1[i] += b2[i];
        MemSet(b2, 0, BLOCK_CNT * siz(*b2));
        bucketScan();
        for (i = cnt - 1; i > -1; --i)
            key = ByteIVal<0>(buf1[i]), buf2[si[key] + --b1[key]] = buf1[i];

        i = 0;
        if (cnt & 1)
            ++b1[ByteIVal<1>(buf2[i++])];
        while (i < cnt)
            ++b1[ByteIVal<1>(buf2[i++])], ++b2[ByteIVal<1>(buf2[i++])];
        for (i = 0; i < BLOCK_CNT; ++i)
            b1[i] += b2[i];
        MemSet(b2, 0, BLOCK_CNT * siz(*b2));
        bucketScan();
        for (i = cnt - 1; i > -1; --i)
            key = ByteIVal<1>(buf2[i]), buf1[si[key] + --b1[key]] = buf2[i];

        i = 0;
        if (cnt & 1)
            ++b1[ByteIVal<2>(buf1[i++])];
        while (i < cnt)
            ++b1[ByteIVal<2>(buf1[i++])], ++b2[ByteIVal<2>(buf1[i++])];
        for (i = 0; i < BLOCK_CNT; ++i)
            b1[i] += b2[i];
        MemSet(b2, 0, BLOCK_CNT * siz(*b2));
        bucketScan();
        for (i = cnt - 1; i > -1; --i)
            key = ByteIVal<2>(buf1[i]), buf2[si[key] + --b1[key]] = buf1[i];

        i = 0;
        if (cnt & 1)
            ++b1[ByteIVal<3>(buf2[i++])];
        while (i < cnt)
            ++b1[ByteIVal<3>(buf2[i++])], ++b2[ByteIVal<3>(buf2[i++])];
        for (i = 0; i < BLOCK_CNT; ++i)
            b1[i] += b2[i];
        // MemSet(b2, 0, BLOCK_CNT * siz(*b2));
        bucketScan();
        for (i = cnt - 1; i > -1; --i)
            key = ByteIVal<3>(buf2[i]), buf1[si[key] + --b1[key]] = buf2[i];
    }

    free(buf2);

    SI key = 0;
    pEnd = (p = buf1) + cnt;
    while (p != pEnd)
        key += (((*p++ ^= 0x80000000) & 0x80000000) != 0);
    cx SI ofs = key - 1;
    cx SI iEnd = key >> 1;
    ite (i, i < iEnd)
        Swap(buf1[i], buf1[ofs - i]);
}
