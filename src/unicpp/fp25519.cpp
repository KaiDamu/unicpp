#include "fp25519.hpp"

#include "bigmath.hpp"

dfa NT Fp25519SetU4(Fp25519& dst, U4 val)
{
    MemSet(dst.Dat(), 0, dst.SIZE);
    dst.u4[0] = val;
}
dfa NT Fp25519ReducePartial(Fp25519& dst, U4 carry)
{
    dst[31] &= 127;
    carry = (carry >> 7) * FP_25519_PRIME;
    ite (i, i < Fp25519::SIZE)
    {
        carry += dst[i];
        dst[i] = U1(carry);
        carry >>= 8;
    }
}
dfa NT Fp25519ReduceFull(Fp25519& dst)
{
    Fp25519 tmp;
    U2 carry = (dst[31] >> 7) * FP_25519_PRIME;
    dst[31] &= 127;
    ite (i, i < Fp25519::SIZE)
    {
        carry += dst[i];
        dst[i] = U1(carry);
        carry >>= 8;
    }
    carry = FP_25519_PRIME;
    ite (i, i + 1 < Fp25519::SIZE)
    {
        carry += dst[i];
        tmp[i] = U1(carry);
        carry >>= 8;
    }
    carry += U2(U4(dst[31]) - 128);
    tmp[31] = U1(carry);
    BigSelCt<Fp25519::SIZE>(dst.Dat(), tmp.Dat(), dst.Dat(), (carry >> 15) & 1);
}
dfa NT Fp25519Add(Fp25519& dst, cx Fp25519& a, cx Fp25519& b)
{
    U4 carry = 0;
    ite (i, i < Fp25519::SIZE)
    {
        carry >>= 8;
        carry += U4(a[i]) + U4(b[i]);
        dst[i] = U1(carry);
    }
    Fp25519ReducePartial(dst, carry);
}
dfa NT Fp25519Sub(Fp25519& dst, cx Fp25519& a, cx Fp25519& b)
{
    U4 carry = 218;
    ite (i, i + 1 < Fp25519::SIZE)
    {
        carry += 65280 + U4(a[i]) - U4(b[i]);
        dst[i] = U1(carry);
        carry >>= 8;
    }
    carry += U4(a[31]) - U4(b[31]);
    dst[31] = U1(carry);
    Fp25519ReducePartial(dst, carry);
}
dfa NT Fp25519Neg(Fp25519& dst, cx Fp25519& a)
{
    U4 carry = 218;
    ite (i, i + 1 < Fp25519::SIZE)
    {
        carry += 65280 - U4(a[i]);
        dst[i] = U1(carry);
        carry >>= 8;
    }
    carry -= U4(a[31]);
    dst[31] = U1(carry);
    Fp25519ReducePartial(dst, carry);
}
dfa NT Fp25519Mul(Fp25519& dst, cx Fp25519& a, cx Fp25519& b)
{
    U4 carry = 0;
    ite (i, i < Fp25519::SIZE)
    {
        carry >>= 8;
        SI j = 0;
        for (; j <= i; ++j)
            carry += U4(a[j]) * U4(b[i - j]);
        for (; j < Fp25519::SIZE; ++j)
            carry += U4(a[j]) * U4(b[i + Fp25519::SIZE - j]) * 38;
        dst[i] = U1(carry);
    }
    Fp25519ReducePartial(dst, carry);
}
dfa NT Fp25519MulScalar(Fp25519& dst, cx Fp25519& a, U4 b)
{
    U4 carry = 0;
    ite (i, i < Fp25519::SIZE)
    {
        carry >>= 8;
        carry += b * U4(a[i]);
        dst[i] = U1(carry);
    }
    Fp25519ReducePartial(dst, carry);
}
dfa NT Fp25519Exp252M3(Fp25519& dst, cx Fp25519& base)
{
    Fp25519Mul(dst, base, base);
    Fp25519 s;
    Fp25519Mul(s, dst, base);
    ite (i, i < 248)
    {
        Fp25519Mul(dst, s, s);
        Fp25519Mul(s, dst, base);
    }
    Fp25519Mul(dst, s, s);
    Fp25519Mul(s, dst, dst);
    Fp25519Mul(dst, s, base);
}
dfa NT Fp25519ModSqrt(Fp25519& dst, cx Fp25519& a)
{
    Fp25519 x;
    Fp25519MulScalar(x, a, 2);
    Fp25519 v;
    Fp25519Exp252M3(v, x);
    Fp25519 y;
    Fp25519Mul(y, v, v);
    Fp25519 i;
    Fp25519Mul(i, x, y);
    Fp25519SetU4(y, 1);
    Fp25519Sub(i, i, y);
    Fp25519Mul(x, v, a);
    Fp25519Mul(dst, x, i);
}
dfa NT Fp25519InvSpecial(Fp25519& dst, cx Fp25519& a)
{
    Fp25519 s;
    Fp25519Mul(s, a, a);
    Fp25519Mul(dst, s, a);
    ite (i, i < 248)
    {
        Fp25519Mul(s, dst, dst);
        Fp25519Mul(dst, s, a);
    }
    Fp25519Mul(s, dst, dst);
    Fp25519Mul(dst, s, s);
    Fp25519Mul(s, dst, a);
    Fp25519Mul(dst, s, s);
    Fp25519Mul(s, dst, dst);
    Fp25519Mul(dst, s, a);
    Fp25519Mul(s, dst, dst);
    Fp25519Mul(dst, s, a);
}

dfa BO Fp25519EquCt(cx Fp25519& a, cx Fp25519& b)
{
    U1 sum = 0;
    ite (i, i < Fp25519::SIZE)
        sum |= U1(a[i] ^ b[i]);
    sum |= (sum >> 4);
    sum |= (sum >> 2);
    sum |= (sum >> 1);
    ret BO((sum ^ 1) & 1);
}
