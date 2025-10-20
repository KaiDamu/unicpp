#include "aes.hpp"

dfa NT _AesXorIv(AesBlock* restx dst, cx AesBlock* restx iv)
{
    dst->u8[0] = dst->u8[0] ^ iv->u8[0];
    dst->u8[1] = dst->u8[1] ^ iv->u8[1];
}

dfa NT _AesRoundFirst(AesBlock* restx dst, cx AesBlock* restx src, cx AesBlock* restx rkey)
{
    dst->u8[0] = src->u8[0] ^ rkey->u8[0];
    dst->u8[1] = src->u8[1] ^ rkey->u8[1];
}
dfa NT _AesRoundMid(AesBlock* restx dst, cx AesBlock* restx src, cx AesBlock* restx rkey)
{
    dst->u1[0] = g_aesGfm2S[src->u1[0]] ^ g_aesGfm3S[src->u1[5]] ^ g_aesSbox[src->u1[10]] ^ g_aesSbox[src->u1[15]];
    dst->u1[1] = g_aesSbox[src->u1[0]] ^ g_aesGfm2S[src->u1[5]] ^ g_aesGfm3S[src->u1[10]] ^ g_aesSbox[src->u1[15]];
    dst->u1[2] = g_aesSbox[src->u1[0]] ^ g_aesSbox[src->u1[5]] ^ g_aesGfm2S[src->u1[10]] ^ g_aesGfm3S[src->u1[15]];
    dst->u1[3] = g_aesGfm3S[src->u1[0]] ^ g_aesSbox[src->u1[5]] ^ g_aesSbox[src->u1[10]] ^ g_aesGfm2S[src->u1[15]];
    dst->u1[4] = g_aesGfm2S[src->u1[4]] ^ g_aesGfm3S[src->u1[9]] ^ g_aesSbox[src->u1[14]] ^ g_aesSbox[src->u1[3]];
    dst->u1[5] = g_aesSbox[src->u1[4]] ^ g_aesGfm2S[src->u1[9]] ^ g_aesGfm3S[src->u1[14]] ^ g_aesSbox[src->u1[3]];
    dst->u1[6] = g_aesSbox[src->u1[4]] ^ g_aesSbox[src->u1[9]] ^ g_aesGfm2S[src->u1[14]] ^ g_aesGfm3S[src->u1[3]];
    dst->u1[7] = g_aesGfm3S[src->u1[4]] ^ g_aesSbox[src->u1[9]] ^ g_aesSbox[src->u1[14]] ^ g_aesGfm2S[src->u1[3]];
    dst->u1[8] = g_aesGfm2S[src->u1[8]] ^ g_aesGfm3S[src->u1[13]] ^ g_aesSbox[src->u1[2]] ^ g_aesSbox[src->u1[7]];
    dst->u1[9] = g_aesSbox[src->u1[8]] ^ g_aesGfm2S[src->u1[13]] ^ g_aesGfm3S[src->u1[2]] ^ g_aesSbox[src->u1[7]];
    dst->u1[10] = g_aesSbox[src->u1[8]] ^ g_aesSbox[src->u1[13]] ^ g_aesGfm2S[src->u1[2]] ^ g_aesGfm3S[src->u1[7]];
    dst->u1[11] = g_aesGfm3S[src->u1[8]] ^ g_aesSbox[src->u1[13]] ^ g_aesSbox[src->u1[2]] ^ g_aesGfm2S[src->u1[7]];
    dst->u1[12] = g_aesGfm2S[src->u1[12]] ^ g_aesGfm3S[src->u1[1]] ^ g_aesSbox[src->u1[6]] ^ g_aesSbox[src->u1[11]];
    dst->u1[13] = g_aesSbox[src->u1[12]] ^ g_aesGfm2S[src->u1[1]] ^ g_aesGfm3S[src->u1[6]] ^ g_aesSbox[src->u1[11]];
    dst->u1[14] = g_aesSbox[src->u1[12]] ^ g_aesSbox[src->u1[1]] ^ g_aesGfm2S[src->u1[6]] ^ g_aesGfm3S[src->u1[11]];
    dst->u1[15] = g_aesGfm3S[src->u1[12]] ^ g_aesSbox[src->u1[1]] ^ g_aesSbox[src->u1[6]] ^ g_aesGfm2S[src->u1[11]];
    dst->u8[0] ^= rkey->u8[0];
    dst->u8[1] ^= rkey->u8[1];
}
dfa NT _AesRoundLast(AesBlock* restx dst, cx AesBlock* restx src, cx AesBlock* restx rkey)
{
    dst->u1[0] = g_aesSbox[src->u1[0]];
    dst->u1[1] = g_aesSbox[src->u1[5]];
    dst->u1[2] = g_aesSbox[src->u1[10]];
    dst->u1[3] = g_aesSbox[src->u1[15]];
    dst->u1[4] = g_aesSbox[src->u1[4]];
    dst->u1[5] = g_aesSbox[src->u1[9]];
    dst->u1[6] = g_aesSbox[src->u1[14]];
    dst->u1[7] = g_aesSbox[src->u1[3]];
    dst->u1[8] = g_aesSbox[src->u1[8]];
    dst->u1[9] = g_aesSbox[src->u1[13]];
    dst->u1[10] = g_aesSbox[src->u1[2]];
    dst->u1[11] = g_aesSbox[src->u1[7]];
    dst->u1[12] = g_aesSbox[src->u1[12]];
    dst->u1[13] = g_aesSbox[src->u1[1]];
    dst->u1[14] = g_aesSbox[src->u1[6]];
    dst->u1[15] = g_aesSbox[src->u1[11]];
    dst->u8[0] ^= rkey->u8[0];
    dst->u8[1] ^= rkey->u8[1];
}
dfa NT _AesRoundInvFirst(AesBlock* restx dst, cx AesBlock* restx src, cx AesBlock* restx rkey)
{
    dst->u8[0] = src->u8[0] ^ rkey->u8[0];
    dst->u8[1] = src->u8[1] ^ rkey->u8[1];
}
dfa NT _AesRoundInvMid(AesBlock* restx dst, cx AesBlock* restx src, cx AesBlock* restx rkey)
{
    AesBlock tmp;
    tmp.u8[0] = src->u8[0] ^ rkey->u8[0];
    tmp.u8[1] = src->u8[1] ^ rkey->u8[1];
    dst->u1[0] = g_aesSboxInv[g_aesGfm14[tmp.u1[0]] ^ g_aesGfm11[tmp.u1[1]] ^ g_aesGfm13[tmp.u1[2]] ^ g_aesGfm9[tmp.u1[3]]];
    dst->u1[5] = g_aesSboxInv[g_aesGfm9[tmp.u1[0]] ^ g_aesGfm14[tmp.u1[1]] ^ g_aesGfm11[tmp.u1[2]] ^ g_aesGfm13[tmp.u1[3]]];
    dst->u1[10] = g_aesSboxInv[g_aesGfm13[tmp.u1[0]] ^ g_aesGfm9[tmp.u1[1]] ^ g_aesGfm14[tmp.u1[2]] ^ g_aesGfm11[tmp.u1[3]]];
    dst->u1[15] = g_aesSboxInv[g_aesGfm11[tmp.u1[0]] ^ g_aesGfm13[tmp.u1[1]] ^ g_aesGfm9[tmp.u1[2]] ^ g_aesGfm14[tmp.u1[3]]];
    dst->u1[4] = g_aesSboxInv[g_aesGfm14[tmp.u1[4]] ^ g_aesGfm11[tmp.u1[5]] ^ g_aesGfm13[tmp.u1[6]] ^ g_aesGfm9[tmp.u1[7]]];
    dst->u1[9] = g_aesSboxInv[g_aesGfm9[tmp.u1[4]] ^ g_aesGfm14[tmp.u1[5]] ^ g_aesGfm11[tmp.u1[6]] ^ g_aesGfm13[tmp.u1[7]]];
    dst->u1[14] = g_aesSboxInv[g_aesGfm13[tmp.u1[4]] ^ g_aesGfm9[tmp.u1[5]] ^ g_aesGfm14[tmp.u1[6]] ^ g_aesGfm11[tmp.u1[7]]];
    dst->u1[3] = g_aesSboxInv[g_aesGfm11[tmp.u1[4]] ^ g_aesGfm13[tmp.u1[5]] ^ g_aesGfm9[tmp.u1[6]] ^ g_aesGfm14[tmp.u1[7]]];
    dst->u1[8] = g_aesSboxInv[g_aesGfm14[tmp.u1[8]] ^ g_aesGfm11[tmp.u1[9]] ^ g_aesGfm13[tmp.u1[10]] ^ g_aesGfm9[tmp.u1[11]]];
    dst->u1[13] = g_aesSboxInv[g_aesGfm9[tmp.u1[8]] ^ g_aesGfm14[tmp.u1[9]] ^ g_aesGfm11[tmp.u1[10]] ^ g_aesGfm13[tmp.u1[11]]];
    dst->u1[2] = g_aesSboxInv[g_aesGfm13[tmp.u1[8]] ^ g_aesGfm9[tmp.u1[9]] ^ g_aesGfm14[tmp.u1[10]] ^ g_aesGfm11[tmp.u1[11]]];
    dst->u1[7] = g_aesSboxInv[g_aesGfm11[tmp.u1[8]] ^ g_aesGfm13[tmp.u1[9]] ^ g_aesGfm9[tmp.u1[10]] ^ g_aesGfm14[tmp.u1[11]]];
    dst->u1[12] = g_aesSboxInv[g_aesGfm14[tmp.u1[12]] ^ g_aesGfm11[tmp.u1[13]] ^ g_aesGfm13[tmp.u1[14]] ^ g_aesGfm9[tmp.u1[15]]];
    dst->u1[1] = g_aesSboxInv[g_aesGfm9[tmp.u1[12]] ^ g_aesGfm14[tmp.u1[13]] ^ g_aesGfm11[tmp.u1[14]] ^ g_aesGfm13[tmp.u1[15]]];
    dst->u1[6] = g_aesSboxInv[g_aesGfm13[tmp.u1[12]] ^ g_aesGfm9[tmp.u1[13]] ^ g_aesGfm14[tmp.u1[14]] ^ g_aesGfm11[tmp.u1[15]]];
    dst->u1[11] = g_aesSboxInv[g_aesGfm11[tmp.u1[12]] ^ g_aesGfm13[tmp.u1[13]] ^ g_aesGfm9[tmp.u1[14]] ^ g_aesGfm14[tmp.u1[15]]];
}
dfa NT _AesRoundInvLast(AesBlock* restx dst, cx AesBlock* restx src, cx AesBlock* restx rkey)
{
    dst->u8[0] = src->u8[0] ^ rkey->u8[0];
    dst->u8[1] = src->u8[1] ^ rkey->u8[1];
    dst->u1[0] = g_aesSboxInv[dst->u1[0]];
    dst->u1[4] = g_aesSboxInv[dst->u1[4]];
    dst->u1[8] = g_aesSboxInv[dst->u1[8]];
    dst->u1[12] = g_aesSboxInv[dst->u1[12]];
    U1 tmp = dst->u1[1];
    dst->u1[1] = g_aesSboxInv[dst->u1[13]];
    dst->u1[13] = g_aesSboxInv[dst->u1[9]];
    dst->u1[9] = g_aesSboxInv[dst->u1[5]];
    dst->u1[5] = g_aesSboxInv[tmp];
    tmp = dst->u1[2];
    dst->u1[2] = g_aesSboxInv[dst->u1[10]];
    dst->u1[10] = g_aesSboxInv[tmp];
    tmp = dst->u1[3];
    dst->u1[3] = g_aesSboxInv[dst->u1[7]];
    dst->u1[7] = g_aesSboxInv[dst->u1[11]];
    dst->u1[11] = g_aesSboxInv[dst->u1[15]];
    dst->u1[15] = g_aesSboxInv[tmp];
    tmp = dst->u1[6];
    dst->u1[6] = g_aesSboxInv[dst->u1[14]];
    dst->u1[14] = g_aesSboxInv[tmp];
}

dfa ER Aes128EcbEncrypt(std::vector<AesBlock>& dst, cx std::span<cx U1>& src, Aes128Ctx& ctx)
{
    ret _AesEncrypt<4, AesMode::ECB>(dst, src, ctx);
}
dfa ER Aes128EcbDecrypt(std::vector<U1>& dst, cx std::span<cx AesBlock>& src, Aes128Ctx& ctx)
{
    ret _AesDecrypt<4, AesMode::ECB>(dst, src, ctx);
}
dfa ER Aes128CbcEncrypt(std::vector<AesBlock>& dst, cx std::span<cx U1>& src, Aes128Ctx& ctx)
{
    ret _AesEncrypt<4, AesMode::CBC>(dst, src, ctx);
}
dfa ER Aes128CbcDecrypt(std::vector<U1>& dst, cx std::span<cx AesBlock>& src, Aes128Ctx& ctx)
{
    ret _AesDecrypt<4, AesMode::CBC>(dst, src, ctx);
}
dfa ER Aes128CtrEncrypt(std::vector<U1>& dst, cx std::span<cx U1>& src, Aes128Ctx& ctx)
{
    ret _AesEncryptStream<4, AesMode::CTR>(dst, src, ctx);
}
dfa ER Aes128CtrDecrypt(std::vector<U1>& dst, cx std::span<cx U1>& src, Aes128Ctx& ctx)
{
    ret _AesDecryptStream<4, AesMode::CTR>(dst, src, ctx);
}
