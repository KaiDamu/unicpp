#include "sha.hpp"

#include "memory.hpp"

dfa NT Sha512Hasher::Begin()
{
    cxex U8 init[8] = {0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1, 0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179};
    MemCpy(m_hash.u1, init, Sha512Hash::SIZE);
}
dfa NT Sha512Hasher::Block(cx Sha512Block& bufBlock)
{
    AU bufBlock_ = bufBlock.Dat();
    U8 b[Sha512Block::SIZE / siz(U8)];
    ite (i, i < Sha512Block::SIZE / siz(U8))
    {
        MemCpyValBe(b[i], bufBlock_);
        bufBlock_ += siz(U8);
    }
    U8 h[Sha512Hash::SIZE / siz(U8)];
    MemCpy(h, m_hash.u8, Sha512Hash::SIZE);
    ite (i, i < 80)
    {
        cx AU v1 = b[(i + 1) & 15];
        cx AU v2 = b[(i + 14) & 15];
        cx AU v3 = RotR(v1, 1) ^ RotR(v1, 8) ^ (v1 >> 7);
        cx AU v4 = RotR(v2, 19) ^ RotR(v2, 61) ^ (v2 >> 6);
        cx AU v5 = RotR(h[0], 28) ^ RotR(h[0], 34) ^ RotR(h[0], 39);
        cx AU v6 = RotR(h[4], 14) ^ RotR(h[4], 18) ^ RotR(h[4], 41);
        cx AU v7 = (h[4] & h[5]) ^ ((~h[4]) & h[6]);
        cx AU v8 = h[7] + v6 + v7 + g_sha512Round[i] + b[i & 15];
        cx AU v9 = (h[0] & h[1]) ^ (h[0] & h[2]) ^ (h[1] & h[2]);
        h[7] = h[6];
        h[6] = h[5];
        h[5] = h[4];
        h[4] = h[3] + v8;
        h[3] = h[2];
        h[2] = h[1];
        h[1] = h[0];
        h[0] = v5 + v8 + v9;
        b[i & 15] += b[(i + 9) & 15] + v3 + v4;
    }
    ite (i, i < Sha512Hash::SIZE / siz(U8))
        m_hash.u8[i] += h[i];
}
dfa NT Sha512Hasher::End(CXGA buf, SI sizeSum)
{
    // WARNING: if the remaining bytes == Sha512Block::SIZE, call Block() first, then End() will correctly process 0 bytes
    cx AU sizeLast = sizeSum & (Sha512Block::SIZE - 1);
    Sha512Block bufBlock;
    MemCpy(bufBlock.Dat(), buf, sizeLast);
    MemSet(bufBlock.Dat() + sizeLast, 0, Sha512Block::SIZE - sizeLast);
    bufBlock.Dat()[sizeLast] = 0x80;
    if (sizeLast > 111)
    {
        tx->Block(bufBlock);
        MemSet(bufBlock.Dat(), 0, Sha512Block::SIZE);
    }
    MemCpyValBe(bufBlock.Dat() + Sha512Block::SIZE - siz(U8), sizeSum * BIT_IN_BYTE);
    tx->Block(bufBlock);
}
dfa NT Sha512Hasher::Get(Sha512Hash& hash) cx
{
    AU hash_ = (U1*)&hash;
    ite (i, i < Sha512Hash::SIZE / siz(U8))
    {
        MemCpyValBe(hash_, m_hash.u8[i]);
        hash_ += siz(U8);
    }
}

dfa NT HashSha512(Sha512Hash& hash, CXGA buf, SI size)
{
    AU buf_ = (cx U1*)buf;
    Sha512Hasher hasher;
    SI i = 0;
    hasher.Begin();
    while (size - i >= Sha512Block::SIZE)
    {
        hasher.Block(*(cx Sha512Block*)(buf_ + i));
        i += Sha512Block::SIZE;
    }
    hasher.End(buf_ + i, size);
    hasher.Get(hash);
}
