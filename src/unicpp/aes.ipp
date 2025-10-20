#include "aes.hpp"

#include "random.hpp"

tpl<SI keySize4> dfa NT AesCtx<keySize4>::Init(cx U1* key, cx AesBlock* iv)
{
    if (key != NUL)
        _AesKeyExpand<keySize4>(rkey, key);
    if (iv == NUL)
        tx->iv.Clr();
    else
        tx->iv = *iv;
    streamBlock.Clr();
    streamBlockPos = AesBlock::SIZE;
    state = ((key == NUL) ? State::NO_INIT : State::YES_INIT);
}
tpl<SI keySize4> dfa AesCtx<keySize4>::AesCtx(cx U1* key, cx AesBlock* iv) : state(State::NO_INIT)
{
    tx->Init(key, iv);
}

tpl<SI keySize4> cxex dfa SI _AesKeyRoundCnt()
{
    ret keySize4 + 6;
}
tpl<SI keySize4> dfa NT _AesKeyExpand(AesBlock* restx rkey, cx U1* restx key)
{
    cxex U1 rcx[11] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
    MemCpy(rkey, key, keySize4 * siz(U4));
    for (SI i = keySize4; i < ((_AesKeyRoundCnt<keySize4>() + 1) * AES_COL_SIZE); ++i)
    {
        AU tmp = rkey->u4[i - 1];
        AU p = (U1*)&tmp;
        if (i % keySize4 == 0)
        {
            tmp = RotR(tmp, 8);
            p[0] = g_aesSbox[p[0]] ^ rcx[i / keySize4];
            p[1] = g_aesSbox[p[1]];
            p[2] = g_aesSbox[p[2]];
            p[3] = g_aesSbox[p[3]];
        }
        else if (i % keySize4 == 4)
        {
            ifcx (keySize4 == 8)
            {
                p[0] = g_aesSbox[p[0]];
                p[1] = g_aesSbox[p[1]];
                p[2] = g_aesSbox[p[2]];
                p[3] = g_aesSbox[p[3]];
            }
        }
        rkey->u4[i] = rkey->u4[i - keySize4] ^ tmp;
    }
}

tpl<SI keySize4> dfa NT _AesBlockWrite(AesBlock* restx dst, cx AesBlock* restx src, cx AesBlock* restx rkey)
{
    AesBlock tmp;
    _AesRoundFirst(dst, src, rkey);
    for (SI round = 1; round < (_AesKeyRoundCnt<keySize4>() - 1); round += 2)
    {
        _AesRoundMid(&tmp, dst, rkey + round);
        _AesRoundMid(dst, &tmp, rkey + (round + 1));
    }
    _AesRoundMid(&tmp, dst, rkey + (_AesKeyRoundCnt<keySize4>() - 1));
    _AesRoundLast(dst, &tmp, rkey + _AesKeyRoundCnt<keySize4>());
}
tpl<SI keySize4> dfa NT _AesBlockRead(AesBlock* restx dst, cx AesBlock* restx src, cx AesBlock* restx rkey)
{
    AesBlock tmp;
    _AesRoundInvLast(dst, src, rkey + _AesKeyRoundCnt<keySize4>());
    for (SI round = (_AesKeyRoundCnt<keySize4>() - 1); round > 1; round -= 2)
    {
        _AesRoundInvMid(&tmp, dst, rkey + round);
        _AesRoundInvMid(dst, &tmp, rkey + (round - 1));
    }
    _AesRoundInvMid(&tmp, dst, (rkey + 1));
    _AesRoundInvFirst(dst, &tmp, rkey);
}

tpl<SI keySize4, AesMode mode> dfa ER _AesEncrypt(std::vector<AesBlock>& dst, cx std::span<cx U1>& src, AesCtx<keySize4>& ctx)
{
    dst.clear();

    ifcx (mode != AesMode::ECB && mode != AesMode::CBC)
        rete(ErrVal::NO_SUPPORT);

    ifu (ctx.state != AesCtx<keySize4>::State::YES_INIT)
        rete(ErrVal::NO_VALID);

    cx AU srcSize = SI(src.size_bytes());
    cx AU blockCntPre = SI(mode == AesMode::CBC);
    cx AU blockCntPad = SI(1);
    cx AU blockCnt = blockCntPre + (srcSize / AesBlock::SIZE) + blockCntPad;
    dst.resize(blockCnt);
    std::vector<AesBlock> src_(blockCnt - blockCntPre);
    MemCpy(src_.data(), src.data(), srcSize);

    // iv scheme is automatically prepend mode
    ifcx (mode == AesMode::CBC)
    {
        if (ctx.iv.u8[0] == 0 && ctx.iv.u8[1] == 0)
            RandCrypt(ctx.iv.u1, AesBlock::SIZE);
        dst[0] = ctx.iv;
    }

    // pad scheme is automatically PKCS#7
    cx AU padSize = (blockCnt - blockCntPre) * AesBlock::SIZE - srcSize;
    MemSet((U1*)src_.data() + srcSize, U1(padSize), padSize);

    for (SI i = blockCntPre; i < blockCnt; ++i)
    {
        ifcx (mode == AesMode::CBC)
            _AesXorIv(&src_[i - blockCntPre], &ctx.iv);
        _AesBlockWrite<keySize4>(&dst[i], &src_[i - blockCntPre], ctx.rkey);
        ifcx (mode == AesMode::CBC)
            ctx.iv = dst[i];
    }

    ctx.state = AesCtx<keySize4>::State::DONE;

    rets;
}
tpl<SI keySize4, AesMode mode> dfa ER _AesDecrypt(std::vector<U1>& dst, cx std::span<cx AesBlock>& src, AesCtx<keySize4>& ctx)
{
    dst.clear();

    ifcx (mode != AesMode::ECB && mode != AesMode::CBC)
        rete(ErrVal::NO_SUPPORT);

    ifu (ctx.state != AesCtx<keySize4>::State::YES_INIT)
        rete(ErrVal::NO_VALID);

    ifu (src.size() == 0)
        rete(ErrVal::NO_VALID);

    cx AU blockCntPre = SI(mode == AesMode::CBC);
    cx AU blockCnt = SI(src.size());
    std::vector<AesBlock> dst_(blockCnt - blockCntPre);

    // iv scheme is automatically prepend mode
    ifcx (mode == AesMode::CBC)
        ctx.iv = src[0];

    for (SI i = blockCntPre; i < blockCnt; ++i)
    {
        _AesBlockRead<keySize4>(&dst_[i - blockCntPre], &src[i], ctx.rkey);
        ifcx (mode == AesMode::CBC)
        {
            _AesXorIv(&dst_[i - blockCntPre], &ctx.iv);
            ctx.iv = src[i];
        }
    }

    // pad scheme is automatically PKCS#7
    AU padCur = &dst_.back().u1[AesBlock::SIZE - 1];
    cx AU padSize = *padCur;
    ifu (padSize > AesBlock::SIZE || padSize == 0)
        rete(ErrVal::NO_VALID);
    ite (i, i < padSize)
        ifu (*padCur-- != padSize)
            rete(ErrVal::NO_VALID);

    cx AU srcSizeNoPreNoPad = SI(src.size_bytes()) - (blockCntPre * AesBlock::SIZE) - padSize;
    dst.resize(srcSizeNoPreNoPad);
    MemCpy(dst.data(), dst_.data(), srcSizeNoPreNoPad);

    ctx.state = AesCtx<keySize4>::State::DONE;

    rets;
}
tpl<SI keySize4, AesMode mode> dfa ER _AesEncryptStream(std::vector<U1>& dst, cx std::span<cx U1>& src, AesCtx<keySize4>& ctx)
{
    dst.clear();

    ifcx (mode != AesMode::CTR)
        rete(ErrVal::NO_SUPPORT);

    ifu (ctx.state != AesCtx<keySize4>::State::YES_INIT && ctx.state != AesCtx<keySize4>::State::STREAM)
        rete(ErrVal::NO_VALID);

    cx AU srcSize = SI(src.size_bytes());
    cx AU dstSizePre = ((ctx.state == AesCtx<keySize4>::State::YES_INIT) ? AesBlock::SIZE : 0);
    cx AU dstSize = dstSizePre + srcSize;
    dst.resize(dstSize);

    // iv scheme is automatically prepend mode
    if (ctx.state == AesCtx<keySize4>::State::YES_INIT)
    {
        ctx.state = AesCtx<keySize4>::State::STREAM;
        MemCpy(dst.data(), &ctx.iv, AesBlock::SIZE);
    }

    ite (i, i < srcSize)
    {
        if (ctx.streamBlockPos == AesBlock::SIZE)
        {
            _AesBlockWrite<keySize4>(&ctx.streamBlock, &ctx.iv, ctx.rkey);
            ctx.streamBlockPos = 0;
            for (SI j = AesBlock::SIZE - 1; j >= 0; --j)
            {
                if (ctx.iv.u1[j] == 0xFF)
                {
                    ctx.iv.u1[j] = 0x00;
                    continue;
                }
                ++ctx.iv.u1[j];
                break;
            }
        }
        dst[dstSizePre + i] = src[i] ^ ctx.streamBlock.u1[ctx.streamBlockPos++];
    }

    rets;
}
tpl<SI keySize4, AesMode mode> dfa ER _AesDecryptStream(std::vector<U1>& dst, cx std::span<cx U1>& src, AesCtx<keySize4>& ctx)
{
    dst.clear();

    ifcx (mode != AesMode::CTR)
        rete(ErrVal::NO_SUPPORT);

    ifu (ctx.state != AesCtx<keySize4>::State::YES_INIT && ctx.state != AesCtx<keySize4>::State::STREAM)
        rete(ErrVal::NO_VALID);

    cx AU srcSize = SI(src.size_bytes());
    cx AU srcSizePre = ((ctx.state == AesCtx<keySize4>::State::YES_INIT) ? AesBlock::SIZE : SI(0));
    cx AU dstSize = srcSize - srcSizePre;
    ifu (dstSize < 0)
        rete(ErrVal::NO_VALID);
    dst.resize(dstSize);

    // iv scheme is automatically prepend mode
    if (ctx.state == AesCtx<keySize4>::State::YES_INIT)
    {
        ctx.state = AesCtx<keySize4>::State::STREAM;
        MemCpy(&ctx.iv, src.data(), AesBlock::SIZE);
    }

    ite (i, i < dstSize)
    {
        if (ctx.streamBlockPos == AesBlock::SIZE)
        {
            _AesBlockWrite<keySize4>(&ctx.streamBlock, &ctx.iv, ctx.rkey);
            ctx.streamBlockPos = 0;
            for (SI j = AesBlock::SIZE - 1; j >= 0; --j)
            {
                if (ctx.iv.u1[j] == 0xFF)
                {
                    ctx.iv.u1[j] = 0x00;
                    continue;
                }
                ++ctx.iv.u1[j];
                break;
            }
        }
        dst[i] = src[srcSizePre + i] ^ ctx.streamBlock.u1[ctx.streamBlockPos++];
    }

    rets;
}
