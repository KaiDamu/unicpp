#pragma once

#include "container.hpp"

cxex U4 FP_25519_PRIME = 19;

struct Fp25519 : DatBlock<32>
{
};

dfa NT Fp25519SetU4(Fp25519& dst, U4 val);
dfa NT Fp25519ReducePartial(Fp25519& dst, U4 carry);
dfa NT Fp25519ReduceFull(Fp25519& dst);
dfa NT Fp25519Add(Fp25519& dst, cx Fp25519& a, cx Fp25519& b);
dfa NT Fp25519Sub(Fp25519& dst, cx Fp25519& a, cx Fp25519& b);
dfa NT Fp25519Neg(Fp25519& dst, cx Fp25519& a);
dfa NT Fp25519Mul(Fp25519& dst, cx Fp25519& a, cx Fp25519& b);
dfa NT Fp25519MulScalar(Fp25519& dst, cx Fp25519& a, U4 b);
dfa NT Fp25519Exp252M3(Fp25519& dst, cx Fp25519& base);
dfa NT Fp25519ModSqrt(Fp25519& dst, cx Fp25519& a);
dfa NT Fp25519InvSpecial(Fp25519& dst, cx Fp25519& a);

dfa BO Fp25519EquCt(cx Fp25519& a, cx Fp25519& b);

#include "fp25519.ipp"
