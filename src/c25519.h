#pragma once

struct C25519Key : DatBlockSecure<32>
{
};
struct C25519Seed : DatBlockSecure<32>
{
    dfa NT Gen()
    {
        RandCrypt(tx->Dat(), SIZE);
    }
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

cxex Fp25519 g_fp25519One = {1};
cxex Fp25519 g_ed25519BasePtOrder = {0xed, 0xd3, 0xf5, 0x5c, 0x1a, 0x63, 0x12, 0x58, 0xd6, 0x9c, 0xf7, 0xa2, 0xde, 0xf9, 0xde, 0x14,
                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10};

///  C25519 operations  ///

dfa NT C25519LsDouble(Fp25519& x3, Fp25519& z3, cx Fp25519& x1, cx Fp25519& z1)
{
    Fp25519 x1sq, z1sq, x1z1, a;
    Fp25519Mul(x1sq, x1, x1);
    Fp25519Mul(z1sq, z1, z1);
    Fp25519Mul(x1z1, x1, z1);
    Fp25519Sub(a, x1sq, z1sq);
    Fp25519Mul(x3, a, a);
    Fp25519MulScalar(a, x1z1, 486662);
    Fp25519Add(a, x1sq, a);
    Fp25519Add(a, z1sq, a);
    Fp25519Mul(x1sq, x1z1, a);
    Fp25519MulScalar(z3, x1sq, 4);
}
dfa NT C25519LsDiffAdd(Fp25519& x5, Fp25519& z5, cx Fp25519& x1, cx Fp25519& z1, cx Fp25519& x2, cx Fp25519& z2, cx Fp25519& x3, cx Fp25519& z3)
{
    Fp25519 a, b, cb, da;
    Fp25519Add(a, x2, z2);
    Fp25519Sub(b, x3, z3);
    Fp25519Mul(da, a, b);
    Fp25519Sub(b, x2, z2);
    Fp25519Add(a, x3, z3);
    Fp25519Mul(cb, a, b);
    Fp25519Add(a, da, cb);
    Fp25519Mul(b, a, a);
    Fp25519Mul(x5, z1, b);
    Fp25519Sub(a, da, cb);
    Fp25519Mul(b, a, a);
    Fp25519Mul(z5, x1, b);
}
dfa NT C25519Clamp(Fp25519& scalar)
{
    scalar[0] &= 0xf8;
    scalar[31] &= 0x7f;
    scalar[31] |= 0x40;
}
dfa NT C25519MulScalar(Fp25519& scalar, cx Fp25519& q, cx Fp25519& e)
{
    Fp25519 xm = q;
    Fp25519 zm = {1};
    Fp25519 xm1 = {1};
    Fp25519 zm1 = {};
    for (SI i = 253; i >= 0; --i)
    {
        cx AU bit = BO((e[i >> 3] >> (i & 7)) & 1);
        C25519LsDiffAdd(xm1, zm1, q, g_fp25519One, xm, zm, xm1, zm1);
        C25519LsDouble(xm, zm, xm, zm);
        Fp25519 xms, zms;
        C25519LsDiffAdd(xms, zms, xm1, zm1, xm, zm, q, g_fp25519One);
        BigSelCt<Fp25519::SIZE>(xm1.Dat(), xm1.Dat(), xm.Dat(), bit);
        BigSelCt<Fp25519::SIZE>(zm1.Dat(), zm1.Dat(), zm.Dat(), bit);
        BigSelCt<Fp25519::SIZE>(xm.Dat(), xm.Dat(), xms.Dat(), bit);
        BigSelCt<Fp25519::SIZE>(zm.Dat(), zm.Dat(), zms.Dat(), bit);
    }
    Fp25519InvSpecial(zm1, zm);
    Fp25519Mul(scalar, zm1, xm);
    Fp25519ReduceFull(scalar);
}
dfa NT C25519HashToScalar(Fp25519& scalar, Fp25519* rest, cx C25519Seed& seed)
{
    Sha512Hash hash;
    HashSha512(hash, seed.Dat(), seed.SIZE);
    MemCpy(scalar.Dat(), hash.Dat(), scalar.SIZE);
    C25519Clamp(scalar);
    if (rest != NUL)
        MemCpy(rest->Dat(), hash.Dat() + scalar.SIZE, rest->SIZE);
}

///  Ed25519 operations  ///

dfa NT Ed25519FromAffine(Ed25519Pt& projPt, cx Fp25519& x, cx Fp25519& y)
{
    projPt.x = x;
    projPt.y = y;
    Fp25519SetU4(projPt.z, 1);
    Fp25519Mul(projPt.t, x, y);
}
dfa NT Ed25519ToAffine(Fp25519& x, Fp25519& y, cx Ed25519Pt& projPt)
{
    Fp25519 z1;
    Fp25519InvSpecial(z1, projPt.z);
    Fp25519Mul(x, projPt.x, z1);
    Fp25519Mul(y, projPt.y, z1);
    Fp25519ReduceFull(x);
    Fp25519ReduceFull(y);
}
dfa NT Ed25519Compress(Fp25519& encoded, cx Fp25519& x, cx Fp25519& y)
{
    AU tmp = x;
    Fp25519ReduceFull(tmp);
    encoded = y;
    Fp25519ReduceFull(encoded);
    encoded[31] |= U1((tmp[0] & 1) << 7);
}
dfa BO Ed25519TryDecompress(Fp25519& x, Fp25519& y, cx Fp25519& encoded)
{
    y = encoded;
    y[31] &= 127;
    Fp25519 v[3];
    Fp25519Mul(v[2], y, y);
    cxex Fp25519 ed25519D = {0xa3, 0x78, 0x59, 0x13, 0xca, 0x4d, 0xeb, 0x75, 0xab, 0xd8, 0x41, 0x41, 0x4d, 0x0a, 0x70, 0x00,
                             0x98, 0xe8, 0x79, 0x77, 0x79, 0x40, 0xc7, 0x8c, 0x73, 0xfe, 0x6f, 0x2b, 0xee, 0x6c, 0x03, 0x52};
    Fp25519Mul(v[1], v[2], ed25519D);
    Fp25519Add(v[0], v[1], g_fp25519One);
    Fp25519InvSpecial(v[1], v[0]);
    Fp25519Sub(v[0], v[2], g_fp25519One);
    Fp25519Mul(v[2], v[0], v[1]);
    Fp25519ModSqrt(v[0], v[2]);
    Fp25519Neg(v[1], v[0]);
    BigSelCt<Fp25519::SIZE>(x.Dat(), v[0].Dat(), v[1].Dat(), (v[0][0] ^ U1(encoded[31] >> 7)) & 1);
    Fp25519Mul(v[0], x, x);
    Fp25519ReduceFull(v[0]);
    Fp25519ReduceFull(v[2]);
    ret Fp25519EquCt(v[0], v[2]);
}
dfa NT Ed25519ProjPack(Fp25519& encoded, cx Ed25519Pt& projPt)
{
    Fp25519 x, y;
    Ed25519ToAffine(x, y, projPt);
    Ed25519Compress(encoded, x, y);
}
dfa BO Ed25519ProjUnpack(Ed25519Pt& projPt, cx Fp25519& encoded)
{
    Fp25519 x, y;
    cx AU isOk = Ed25519TryDecompress(x, y, encoded);
    Ed25519FromAffine(projPt, x, y);
    ret isOk;
}
dfa NT Ed25519PtAdd(Ed25519Pt& r, cx Ed25519Pt& p1, cx Ed25519Pt& p2)
{
    Fp25519 v[8];
    Fp25519Sub(v[2], p1.y, p1.x);
    Fp25519Sub(v[3], p2.y, p2.x);
    Fp25519Mul(v[0], v[2], v[3]);
    Fp25519Add(v[2], p1.y, p1.x);
    Fp25519Add(v[3], p2.y, p2.x);
    Fp25519Mul(v[1], v[2], v[3]);
    Fp25519Mul(v[3], p1.t, p2.t);
    cxex Fp25519 ed25519K = {0x59, 0xf1, 0xb2, 0x26, 0x94, 0x9b, 0xd6, 0xeb, 0x56, 0xb1, 0x83, 0x82, 0x9a, 0x14, 0xe0, 0x00,
                             0x30, 0xd1, 0xf3, 0xee, 0xf2, 0x80, 0x8e, 0x19, 0xe7, 0xfc, 0xdf, 0x56, 0xdc, 0xd9, 0x06, 0x24};
    Fp25519Mul(v[2], v[3], ed25519K);
    Fp25519Mul(v[3], p1.z, p2.z);
    Fp25519Add(v[3], v[3], v[3]);
    Fp25519Sub(v[4], v[1], v[0]);
    Fp25519Sub(v[5], v[3], v[2]);
    Fp25519Add(v[6], v[3], v[2]);
    Fp25519Add(v[7], v[1], v[0]);
    Fp25519Mul(r.x, v[4], v[5]);
    Fp25519Mul(r.y, v[6], v[7]);
    Fp25519Mul(r.t, v[4], v[7]);
    Fp25519Mul(r.z, v[5], v[6]);
}
dfa NT Ed25519PtDouble(Ed25519Pt& r, cx Ed25519Pt& p)
{
    Fp25519 v[7];
    Fp25519Mul(v[0], p.x, p.x);
    Fp25519Mul(v[1], p.y, p.y);
    Fp25519Mul(v[2], p.z, p.z);
    Fp25519Add(v[2], v[2], v[2]);
    Fp25519Add(v[4], p.x, p.y);
    Fp25519Mul(v[3], v[4], v[4]);
    Fp25519Sub(v[3], v[3], v[0]);
    Fp25519Sub(v[3], v[3], v[1]);
    Fp25519Sub(v[5], v[1], v[0]);
    Fp25519Sub(v[4], v[5], v[2]);
    Fp25519Neg(v[6], v[1]);
    Fp25519Sub(v[6], v[6], v[0]);
    Fp25519Mul(r.x, v[3], v[4]);
    Fp25519Mul(r.y, v[5], v[6]);
    Fp25519Mul(r.t, v[3], v[6]);
    Fp25519Mul(r.z, v[4], v[5]);
}
dfa NT Ed25519MulScalar(Ed25519Pt& r, cx Ed25519Pt& p, cx Fp25519& scalar)
{
    cxex Ed25519Pt ed25519Neutral = {.x = {}, .y = {1, 0}, .t = {}, .z = {1, 0}};
    Ed25519Pt tmp = ed25519Neutral;
    for (SI i = 255; i >= 0; --i)
    {
        cx AU bit = BO((scalar[i >> 3] >> (i & 7)) & 1);
        Ed25519PtDouble(tmp, tmp);
        Ed25519Pt s;
        Ed25519PtAdd(s, tmp, p);
        BigSelCt<Fp25519::SIZE>(tmp.x.Dat(), tmp.x.Dat(), s.x.Dat(), bit);
        BigSelCt<Fp25519::SIZE>(tmp.y.Dat(), tmp.y.Dat(), s.y.Dat(), bit);
        BigSelCt<Fp25519::SIZE>(tmp.z.Dat(), tmp.z.Dat(), s.z.Dat(), bit);
        BigSelCt<Fp25519::SIZE>(tmp.t.Dat(), tmp.t.Dat(), s.t.Dat(), bit);
    }
    r = tmp;
}
dfa NT Ed25519MulScalarBase(Fp25519& encoded, cx Fp25519& scalar)
{
    cxex Ed25519Pt ed25519Base = {.x = {0x1a, 0xd5, 0x25, 0x8f, 0x60, 0x2d, 0x56, 0xc9, 0xb2, 0xa7, 0x25, 0x95, 0x60, 0xc7, 0x2c, 0x69,
                                        0x5c, 0xdc, 0xd6, 0xfd, 0x31, 0xe2, 0xa4, 0xc0, 0xfe, 0x53, 0x6e, 0xcd, 0xd3, 0x36, 0x69, 0x21},
                                  .y = {0x58, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
                                        0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66},
                                  .t = {0xa3, 0xdd, 0xb7, 0xa5, 0xb3, 0x8a, 0xde, 0x6d, 0xf5, 0x52, 0x51, 0x77, 0x80, 0x9f, 0xf0, 0x20,
                                        0x7d, 0xe3, 0xab, 0x64, 0x8e, 0x4e, 0xea, 0x66, 0x65, 0x76, 0x8b, 0xd7, 0x0f, 0x5f, 0x87, 0x67},
                                  .z = {1, 0}};
    Ed25519Pt p;
    Ed25519MulScalar(p, ed25519Base, scalar);
    Ed25519ProjPack(encoded, p);
}
dfa NT Ed25519HashWithPre(Fp25519& scalar, cx U1* pre, SI preSize, cx U1* msg, SI msgSize)
{
    Sha512Block block;
    MemCpy(block.Dat(), pre, preSize);
    Sha512Hasher hasher;
    hasher.Begin();
    if (msgSize < block.SIZE && msgSize + preSize < block.SIZE)
    {
        MemCpy(block.Dat() + preSize, msg, msgSize);
        hasher.End(block.Dat(), msgSize + preSize);
    }
    else
    {
        MemCpy(block.Dat() + preSize, msg, block.SIZE - preSize);
        hasher.Block(block);
        SI i;
        for (i = block.SIZE - preSize; i + block.SIZE <= msgSize; i += block.SIZE)
            hasher.Block(*(Sha512Block*)(msg + i));
        hasher.End(msg + i, msgSize + preSize);
    }
    Sha512Hash hash;
    hasher.Get(hash);
    BigModByBuf<Fp25519::SIZE>(scalar.Dat(), hash.Dat(), Sha512Hash::SIZE, g_ed25519BasePtOrder.Dat());
}
dfa NT Ed25519HashWithRA(Fp25519& scalar, cx Ed25519Sign::Part& r, cx Ed25519VeriKey& veriKey, cx U1* msg, SI msgSize)
{
    U1 pre[r.SIZE + veriKey.SIZE];
    MemCpy(pre, r.Dat(), r.SIZE);
    MemCpy(pre + r.SIZE, veriKey.Dat(), veriKey.SIZE);
    Ed25519HashWithPre(scalar, pre, r.SIZE + veriKey.SIZE, msg, msgSize);
}

///  X25519 for asymmetric key exchanges  ///

dfa NT X25519KeyGen(X25519PrivKey& privKey, X25519PubKey& pubKey, cx X25519Seed& seed)
{
    Fp25519 seedHash;
    C25519HashToScalar(seedHash, NUL, seed);
    MemCpy(privKey.Dat(), seedHash.Dat(), privKey.SIZE);
    cx Fp25519 c25519BaseX = {9};
    C25519MulScalar(AsType<Fp25519>(pubKey), c25519BaseX, AsType<Fp25519>(privKey));
}
dfa NT X25519Shared(X25519SharedKeyRaw& sharedKeyRaw, cx X25519PrivKey& privKey, cx X25519PubKey& pubKeyPeer)
{
    X25519PrivKey privKeyClamp = privKey;
    C25519Clamp(AsType<Fp25519>(privKeyClamp));
    C25519MulScalar(AsType<Fp25519>(sharedKeyRaw), AsType<cx Fp25519>(pubKeyPeer), AsType<Fp25519>(privKeyClamp));
}
dfa NT X25519Derive(X25519SharedKey& sharedKey, cx X25519SharedKeyRaw& sharedKeyRaw, cx X25519PubKey& pubKeySrv, cx X25519PubKey& pubKeyCli, CXGA msg, SI msgSize)
{
    cx AU datSize = SI(X25519SharedKeyRaw::SIZE + X25519PubKey::SIZE * 2 + msgSize);
    std::vector<U1> dat(datSize);
    AU datCur = dat.data();
    MemCpy(datCur, sharedKeyRaw.Dat(), sharedKeyRaw.SIZE);
    datCur += sharedKeyRaw.SIZE;
    MemCpy(datCur, pubKeySrv.Dat(), pubKeySrv.SIZE);
    datCur += pubKeySrv.SIZE;
    MemCpy(datCur, pubKeyCli.Dat(), pubKeyCli.SIZE);
    datCur += pubKeyCli.SIZE;
    if (msgSize > 0)
    {
        MemCpy(datCur, msg, msgSize);
        datCur += msgSize;
    }
    Sha512Hash hash;
    HashSha512(hash, dat.data(), dat.size());
    MemCpy(sharedKey.Dat(), hash.Dat(), hash.SIZE);
    MemSet0Force(dat.data(), dat.size());
}

///  Ed25519 for digital signatures  ///

dfa NT Ed25519KeyGen(Ed25519SignKey& signKey, Ed25519VeriKey& veriKey, cx Ed25519Seed& seed)
{
    Fp25519 seedHash;
    C25519HashToScalar(seedHash, NUL, seed);
    Ed25519MulScalarBase(AsType<Fp25519>(veriKey), seedHash);
    signKey.seed = seed;
    signKey.veriKey = veriKey;
}
dfa NT Ed25519SignMsg(Ed25519Sign& sign, cx Ed25519SignKey& signKey, CXGA msg, SI msgSize)
{
    cx AU msg_ = (cx U1*)msg;
    Fp25519 seedHash;
    Fp25519 seedHashNoClamp;
    C25519HashToScalar(seedHash, &seedHashNoClamp, signKey.seed);
    U1 pre[seedHashNoClamp.SIZE];
    MemCpy(pre, seedHashNoClamp.Dat(), seedHashNoClamp.SIZE);
    Fp25519 k, z, e, s;
    Ed25519HashWithPre(k, pre, seedHashNoClamp.SIZE, msg_, msgSize);
    Ed25519MulScalarBase(AsType<Fp25519>(sign.r), k);
    Ed25519HashWithRA(z, sign.r, signKey.veriKey, msg_, msgSize);
    BigModByBuf<Fp25519::SIZE>(e.Dat(), seedHash.Dat(), seedHash.SIZE, g_ed25519BasePtOrder.Dat());
    BigModMul<Fp25519::SIZE>(s.Dat(), z.Dat(), e.Dat(), g_ed25519BasePtOrder.Dat());
    BigModAdd<Fp25519::SIZE>(s.Dat(), k.Dat(), g_ed25519BasePtOrder.Dat());
    MemCpy(sign.s.Dat(), s.Dat(), s.SIZE);
}
dfa ER Ed25519VeriMsg(cx Ed25519Sign& sign, cx Ed25519VeriKey& veriKey, CXGA msg, SI msgSize)
{
    BO isOk = YES;
    Fp25519 z, ls, rs;
    Ed25519HashWithRA(z, sign.r, veriKey, (cx U1*)msg, msgSize);
    Ed25519MulScalarBase(ls, AsType<cx Fp25519>(sign.s));
    Ed25519Pt p, q;
    isOk &= Ed25519ProjUnpack(p, AsType<cx Fp25519>(veriKey));
    Ed25519MulScalar(p, p, z);
    isOk &= Ed25519ProjUnpack(q, AsType<cx Fp25519>(sign.r));
    Ed25519PtAdd(p, p, q);
    Ed25519ProjPack(rs, p);
    isOk &= Fp25519EquCt(ls, rs);
    if (!isOk)
        rete(ErrVal::NO_VALID);
    rets;
}
