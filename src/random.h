#pragma once

cxex SI RAND_CTX_VAL_CNT = 624;

class RandCtx
{
  private:
    SI m_i;
    std::vector<U4> m_val;
    U4 m_seed;
    BO m_isPrep;

  private:
    dfa NT Init(U4 seed)
    {
        m_i = RAND_CTX_VAL_CNT;
        m_seed = seed;
        m_isPrep = NO;
    }
    dfa NT Prep()
    {
        m_val.resize(RAND_CTX_VAL_CNT);
        m_val[0] = m_seed;
        for (m_i = 1; m_i < RAND_CTX_VAL_CNT; ++m_i)
            m_val[m_i] = m_val[m_i - 1] * 6069;
        m_isPrep = YES;
    }
    dfa NT Roll(U4 i11, U4 i12, U4 i21, U4 i22)
    {
        cx U4 val = (m_val[i11] & 0x80000000) | (m_val[i12] & 0x7FFFFFFF);
        m_val[i21] = m_val[i22] ^ (val >> 1) ^ ((val & 0x01) * 0x9908B0DF);
    }
    dfa NT Gen()
    {
        ifu (!m_isPrep)
            tx->Prep();
        U4 i = 0;
        while (i < 227)
        {
            tx->Roll(i, i + 1, i, i + 397);
            ++i;
        }
        while (i < RAND_CTX_VAL_CNT - 1)
        {
            tx->Roll(i, i + 1, i, i - 227);
            ++i;
        }
        tx->Roll(RAND_CTX_VAL_CNT - 1, 0, RAND_CTX_VAL_CNT - 1, 396);
        m_i = 0;
    }
    dfa U4 Next()
    {
        ifu (m_i == RAND_CTX_VAL_CNT)
            tx->Gen();
        U4 r = m_val[m_i++];
        r ^= r >> 11;
        r ^= (r << 7) & 0x9D2C5680;
        r ^= (r << 15) & 0xEFC60000;
        r ^= r >> 18;
        ret r;
    }

  public:
    dfa F8 RandNormal()
    {
        ret U4ToF8(tx->Next()) * 2.32830643654e-10;
    }
    dfa U4 RandU4()
    {
        ret tx->Next();
    }
    dfa U4 RandU4(U4 min, U4 max)
    {
        ret min + F8ToU4(U4ToF8(max - min + 1) * tx->RandNormal());
    }
    dfa S4 RandS4(S4 min, S4 max)
    {
        ret min + F8ToS4(S4ToF8(max - min + 1) * tx->RandNormal());
    }
    dfa F4 RandF4(F4 min, F4 max)
    {
        ret min + F8ToF4(F4ToF8(max - min) * tx->RandNormal());
    }
    dfa F8 RandF8(F8 min, F8 max)
    {
        ret min + ((max - min) * tx->RandNormal());
    }

  public:
    dfa RandCtx()
    {
        tx->Init(4357);
    }
    dfa RandCtx(U4 seed)
    {
        tx->Init(seed);
    }
};

thdlocal RandCtx g_randCtx(U4(TimeUnix() + CpuTsc()));

dfa F8 RandNormal()
{
    ret g_randCtx.RandNormal();
}
dfa U4 RandU4()
{
    ret g_randCtx.RandU4();
}
dfa U4 RandU4(U4 min, U4 max)
{
    ret g_randCtx.RandU4(min, max);
}
dfa S4 RandS4(S4 min, S4 max)
{
    ret g_randCtx.RandS4(min, max);
}
dfa F4 RandF4(F4 min, F4 max)
{
    ret g_randCtx.RandF4(min, max);
}
dfa F8 RandF8(F8 min, F8 max)
{
    ret g_randCtx.RandF8(min, max);
}

tpl1 dfa T1 RandMag()
{
    ret T1(F8(T1(1) << RandU4(0, sizb(T1) - 1)) * RandF8(0.5, 2.0));
}

dfa NT RandCrypt(GA buf, SI size)
{
    // WARNING: currently not cryptographically secure, but difficult to predict and fast to generate
    AU cur = (U1*)buf;
    while (size != 0)
    {
        cx AU v0 = U8(CpuTsc());
        cx AU v1 = U8(TimeUnix()) ^ 0xAAAAAAAAAAAAAAAA;
        cx AU v2 = v0 ^ 0x5555555555555555;
        cx AU v3 = (v1 + v2) ^ ~RotL(v1 - v2, sizb(U4));
        cx AU v4 = v3 * (v0 | 1);
        cx AU v5 = (U8(RandU4()) << sizb(U4)) | U8(RandU4());
        cx AU v6 = v4 ^ v5;
        cx AU sizeCpy = Min(size, siz(U8));
        MemCpyUpdCur(cur, &v6, sizeCpy);
        size -= sizeCpy;
    }
}

tpl1 dfa NT MixRand(T1* buf, SI cnt)
{
    ite (i, i < cnt)
        Swap<T1>(buf[i], buf[SI(RandU4(0, U4(cnt - 1)))]);
}
