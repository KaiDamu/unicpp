#include "sort.hpp"

#include "memory.hpp"
#include "math.hpp"

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
