#pragma once

#include "fp25519.hpp"

struct C25519Key : DatBlockSecure<32>
{
};
struct C25519Seed : DatBlockSecure<32>
{
    dfa NT Gen();
};

using X25519PrivKey = C25519Key;
using X25519PubKey = C25519Key;
using X25519SharedKeyRaw = C25519Key;
using X25519Seed = C25519Seed;

struct X25519SharedKey : DatBlockSecure<64>
{
};

using Ed25519VeriKey = C25519Key;
using Ed25519Seed = C25519Seed;

struct Ed25519Pt
{
    Fp25519 x, y, t, z;
};
struct Ed25519SignKey
{
    Ed25519Seed seed;
    Ed25519VeriKey veriKey;
};
struct Ed25519Sign
{
    static cxex SI SIZE = 64;
    static cxex SI PART_CNT = 2;

    struct Part : DatBlockSecure<SIZE / PART_CNT>
    {
    };

    Part r;
    Part s;
};

inl cxex Fp25519 g_fp25519One = {1};
inl cxex Fp25519 g_ed25519BasePtOrder = {0xed, 0xd3, 0xf5, 0x5c, 0x1a, 0x63, 0x12, 0x58, 0xd6, 0x9c, 0xf7, 0xa2, 0xde, 0xf9, 0xde, 0x14,
                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10};

///  C25519 operations  ///

dfa NT C25519LsDouble(Fp25519& x3, Fp25519& z3, cx Fp25519& x1, cx Fp25519& z1);
dfa NT C25519LsDiffAdd(Fp25519& x5, Fp25519& z5, cx Fp25519& x1, cx Fp25519& z1, cx Fp25519& x2, cx Fp25519& z2, cx Fp25519& x3, cx Fp25519& z3);
dfa NT C25519Clamp(Fp25519& scalar);
dfa NT C25519MulScalar(Fp25519& scalar, cx Fp25519& q, cx Fp25519& e);
dfa NT C25519HashToScalar(Fp25519& scalar, Fp25519* rest, cx C25519Seed& seed);

///  Ed25519 operations  ///

dfa NT Ed25519FromAffine(Ed25519Pt& projPt, cx Fp25519& x, cx Fp25519& y);
dfa NT Ed25519ToAffine(Fp25519& x, Fp25519& y, cx Ed25519Pt& projPt);
dfa NT Ed25519Compress(Fp25519& encoded, cx Fp25519& x, cx Fp25519& y);
dfa BO Ed25519TryDecompress(Fp25519& x, Fp25519& y, cx Fp25519& encoded);
dfa NT Ed25519ProjPack(Fp25519& encoded, cx Ed25519Pt& projPt);
dfa BO Ed25519ProjUnpack(Ed25519Pt& projPt, cx Fp25519& encoded);
dfa NT Ed25519PtAdd(Ed25519Pt& r, cx Ed25519Pt& p1, cx Ed25519Pt& p2);
dfa NT Ed25519PtDouble(Ed25519Pt& r, cx Ed25519Pt& p);
dfa NT Ed25519MulScalar(Ed25519Pt& r, cx Ed25519Pt& p, cx Fp25519& scalar);
dfa NT Ed25519MulScalarBase(Fp25519& encoded, cx Fp25519& scalar);
dfa NT Ed25519HashWithPre(Fp25519& scalar, cx U1* pre, SI preSize, cx U1* msg, SI msgSize);
dfa NT Ed25519HashWithRA(Fp25519& scalar, cx Ed25519Sign::Part& r, cx Ed25519VeriKey& veriKey, cx U1* msg, SI msgSize);

///  X25519 for asymmetric key exchanges  ///

dfa NT X25519KeyGen(X25519PrivKey& privKey, X25519PubKey& pubKey, cx X25519Seed& seed);
dfa NT X25519Shared(X25519SharedKeyRaw& sharedKeyRaw, cx X25519PrivKey& privKey, cx X25519PubKey& pubKeyPeer);
dfa NT X25519Derive(X25519SharedKey& sharedKey, cx X25519SharedKeyRaw& sharedKeyRaw, cx X25519PubKey& pubKeySrv, cx X25519PubKey& pubKeyCli, CXGA msg, SI msgSize);

///  Ed25519 for digital signatures  ///

dfa NT Ed25519KeyGen(Ed25519SignKey& signKey, Ed25519VeriKey& veriKey, cx Ed25519Seed& seed);
dfa NT Ed25519SignMsg(Ed25519Sign& sign, cx Ed25519SignKey& signKey, CXGA msg, SI msgSize);
dfa ER Ed25519VeriMsg(cx Ed25519Sign& sign, cx Ed25519VeriKey& veriKey, CXGA msg, SI msgSize);

#include "c25519.ipp"
