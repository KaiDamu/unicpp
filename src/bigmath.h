#pragma once

tpl<SI size> dfa BO BigIsNeg(cx U1* val)
{
    ifcx (size < 1)
        ret NO;
    ret BO(val[size - 1] & 0x80);
}
tpl<SI size> dfa SI BigHiBitI(cx U1* val)
{
    for (SI i = size - 1; i != -1; --i)
        if (val[i])
            ret i * BIT_IN_BYTE + LenBin(val[i]);
    ret -1;
}
tpl<SI size> dfa BO BigIsHi(cx U1* main, cx U1* cmp)
{
    for (SI i = size - 1; i != -1; --i)
        if (main[i] != cmp[i])
            ret main[i] > cmp[i];
    ret NO;
}
tpl<SI size> dfa NT BigCpy(U1* dst, cx U1* src)
{
    MemCpy(dst, src, size);
}
tpl<SI size> dfa NT BigSel(U1* dst, cx U1* ifNo, cx U1* ifYes, BO cond)
{
    MemCpy(dst, (cond ? ifYes : ifNo), size);
}
tpl<SI size> dfa NT BigAdd(U1* dst, cx U1* src)
{
    U2 carry = 0;
    ite (i, i < size)
    {
        carry += U2(dst[i]) + U2(src[i]);
        dst[i] = U1(carry);
        carry >>= BIT_IN_BYTE;
    }
}
tpl<SI size> dfa U1 BigSub(U1* dst, cx U1* src)
{
    S2 carry = 0;
    ite (i, i < size)
    {
        cx AU diff = S2(dst[i]) - S2(src[i]) - carry;
        dst[i] = U1(diff & 0xFF);
        carry = (diff < 0);
    }
    ret U1(carry);
}
tpl<SI size> dfa NT BigSubIfPos(U1* dst, cx U1* src)
{
    std::array<U1, size> tmp;
    S2 carry = 0;
    ite (i, i < size)
    {
        cx AU diff = S2(dst[i]) - S2(src[i]) - carry;
        tmp[i] = U1(diff & 0xFF);
        carry = (diff < 0);
    }
    if (carry != 0)
        ret;
    BigCpy<size>(dst, tmp.data());
}
tpl<SI size> dfa U1 BigShiftL(U1* dst, SI cnt)
{
    if (cnt <= 0)
        ret 0;
    cx AU cntByte = SI(cnt / BIT_IN_BYTE);
    cx AU cntBit = U4(cnt % BIT_IN_BYTE);
    std::array<U1, size> tmp;
    BigCpy<size>(tmp.data(), dst);
    U1 carry = 0;
    if (cntByte >= size)
    {
        carry = ((cntBit == 0) ? 0 : (tmp[size - 1] >> (BIT_IN_BYTE - cntBit)));
        MemSet(dst, 0, size);
        ret carry;
    }
    if (cntBit == 0)
    {
        for (SI i = size; i-- > 0;)
        {
            cx AU srcI = i - cntByte;
            dst[i] = ((srcI >= 0) ? tmp[srcI] : 0);
        }
        ret 0;
    }
    for (SI i = size; i-- > 0;)
    {
        cx AU srcI = i - cntByte;
        U1 low = 0;
        if (srcI >= 0)
            low = tmp[srcI];
        U1 high = 0;
        if (srcI - 1 >= 0)
            high = tmp[srcI - 1];
        dst[i] = U1((low << cntBit) | (high >> (BIT_IN_BYTE - cntBit)));
    }
    carry = U1(tmp[size - 1] >> (BIT_IN_BYTE - cntBit));
    ret carry;
}
tpl<SI size> dfa NT BigModAdd(U1* dst, cx U1* add, cx U1* mod)
{
    BigAdd<size>(dst, add);
    BigSubIfPos<size>(dst, mod);
}
tpl<SI size> dfa NT BigModMul(U1* dst, cx U1* mulA, cx U1* mulB, cx U1* mod)
{
    MemSet(dst, 0, size);
    cx AU bitCnt = Max(BigHiBitI<size>(mulB), BigHiBitI<size>(mod));
    for (SI i = bitCnt; i >= 0; --i)
    {
        cx AU bit = U1((mulB[i >> 3] >> (i & 7)) & 1);
        BigShiftL<size>(dst, 1);
        BigSubIfPos<size>(dst, mod);
        U1 tmp[size];
        BigCpy<size>(tmp, dst);
        BigModAdd<size>(tmp, mulA, mod);
        BigSel<size>(dst, dst, tmp, bit);
    }
}
tpl<SI size> dfa NT BigModByBuf(U1* dst, cx U1* buf, SI bufSize, cx U1* mod)
{
    cx AU prelSum = Min(BigHiBitI<size>(mod) - 1, bufSize << 3);
    cx AU prelByteCnt = prelSum >> 3;
    cx AU prelBitCnt = prelSum & 7;
    MemSet(dst, 0, size);
    ite (i, i < prelByteCnt)
        dst[i] = buf[bufSize - prelByteCnt + i];
    if (prelBitCnt > 0)
    {
        BigShiftL<size>(dst, prelBitCnt);
        dst[0] |= buf[bufSize - prelByteCnt - 1] >> (8 - prelBitCnt);
    }
    cx AU remBitCnt = (bufSize << 3) - prelSum;
    for (SI i = remBitCnt - 1; i >= 0; --i)
    {
        cx AU bit = U1((buf[i >> 3] >> (i & 7)) & 1);
        BigShiftL<size>(dst, 1);
        dst[0] |= bit;
        BigSubIfPos<size>(dst, mod);
    }
}

tpl<SI size> dfa NT BigSelCt(U1* dst, cx U1* ifNo, cx U1* ifYes, BO cond)
{
    U1 mask = -cond;
    ite (i, i < size)
        dst[i] = ifNo[i] ^ (mask & (ifYes[i] ^ ifNo[i]));
}
