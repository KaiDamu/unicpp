#pragma once

enum class QoiColSpace : U1
{
    SRGB = 0,
    LINEAR = 1,
};

struct QoiHdr
{
    static cxex SI SIZE = 14;

    Size2<SI> size;
    SI chCnt;
    QoiColSpace colSpace;

    dfa QoiHdr() : size(0, 0), chCnt(0), colSpace(QoiColSpace::SRGB)
    {
    }
};

cxex U4 QOI_MAGIC = 0x716F6966; // "qoif" in ASCII
cxex SI QOI_PIXEL_CNT_MAX = 400000000;

cxex U1 QOI_OP_INDEX = 0x00; // 00xxxxxx
cxex U1 QOI_OP_DIFF = 0x40;  // 01xxxxxx
cxex U1 QOI_OP_LUMA = 0x80;  // 10xxxxxx
cxex U1 QOI_OP_RUN = 0xC0;   // 11xxxxxx
cxex U1 QOI_OP_MASK = 0xC0;  // 11000000
cxex U1 QOI_OP_RGB = 0xFE;   // 11111110
cxex U1 QOI_OP_RGBA = 0xFF;  // 11111111

cxex U1 QOI_EOF[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

dfa SI _QoiColHash(cx ColRgba& col)
{
    ret (col.r * 3 + col.g * 5 + col.b * 7 + col.a * 11) & 0x3F;
}

tpl1 dfa ER QoiEncode(std::vector<U1>& out, cx T1* in, cx Size2<SI>& inSize)
{
    out.clear();
    out.shrink_to_fit();

    QoiHdr hdr;
    hdr.size = inSize;
    hdr.chCnt = 4;                    // input is later auto-converted to RGBA
    hdr.colSpace = QoiColSpace::SRGB; // currently auto-set to SRGB

    ifu (!in || !hdr.size.Area() || hdr.chCnt != 4 || U1(hdr.colSpace) > 1 || (hdr.size.h >= QOI_PIXEL_CNT_MAX / hdr.size.w))
        rete(ErrVal::NO_VALID);

    std::vector<U1> outTmp;
    cx AU outSizeMax = QoiHdr::SIZE + hdr.size.Area() * (hdr.chCnt + 1) + siz(QOI_EOF);
    outTmp.resize(outSizeMax);
    cx AU outBase = outTmp.data();
    AU outCur = outBase;

    cx AU QoiWriteU4 = [](U1*& outCur_, U4 val) -> NT {
        val = RevByte(val);
        MemCpyUpdCur(outCur_, &val, siz(val));
    };

    QoiWriteU4(outCur, QOI_MAGIC);
    QoiWriteU4(outCur, U4(hdr.size.w));
    QoiWriteU4(outCur, U4(hdr.size.h));
    *(outCur++) = U1(hdr.chCnt);
    *(outCur++) = U1(hdr.colSpace);

    ColRgba index[64];
    MemSet(index, 0, siz(index));
    ColRgba pixelPrev(0xFF000000);
    ColRgba pixel = pixelPrev;

    AU inCur = in;
    cx AU inEnd = in + hdr.size.Area();
    while (inCur != inEnd)
    {
        ifcx (IsTypeSame<T1, ColRgba>)
            pixel.val = inCur->val;
        else
            ToType<ColRgba, T1>(pixel, *inCur);

        if (pixel.val == pixelPrev.val)
        {
            SI run = 0;
            do
            {
                ++run;
                ifu (run == 62)
                {
                    *(outCur++) = QOI_OP_RUN | (run - 1);
                    run = 0;
                }
                ++inCur;
                ifu (inCur == inEnd)
                    break;
                ifcx (IsTypeSame<T1, ColRgba>)
                    pixel.val = inCur->val;
                else
                    ToType<ColRgba, T1>(pixel, *inCur);
            } while (pixel.val == pixelPrev.val);
            if (run > 0)
                *(outCur++) = QOI_OP_RUN | (run - 1);
        }
        else
        {
            cx AU indexPos = _QoiColHash(pixel);

            if (index[indexPos].val == pixel.val)
            {
                *(outCur++) = QOI_OP_INDEX | indexPos;
            }
            else
            {
                index[indexPos] = pixel;

                ifl (pixel.a == pixelPrev.a)
                {
                    cx AU vr = S1(pixel.r - pixelPrev.r);
                    cx AU vg = S1(pixel.g - pixelPrev.g);
                    cx AU vb = S1(pixel.b - pixelPrev.b);

                    if (IsBetween<S1>(vr, -2, 1) && IsBetween<S1>(vg, -2, 1) && IsBetween<S1>(vb, -2, 1))
                    {
                        *(outCur++) = QOI_OP_DIFF | (vr + 2) << 4 | (vg + 2) << 2 | (vb + 2);
                    }
                    else
                    {
                        cx AU vgr = S1(vr - vg);
                        cx AU vgb = S1(vb - vg);

                        if (IsBetween<S1>(vgr, -8, 7) && IsBetween<S1>(vgb, -8, 7) && IsBetween<S1>(vg, -32, 31))
                        {
                            *(outCur++) = QOI_OP_LUMA | (vg + 32);
                            *(outCur++) = (vgr + 8) << 4 | (vgb + 8);
                        }
                        else
                        {
                            *(outCur++) = QOI_OP_RGB;
                            *(outCur++) = pixel.r;
                            *(outCur++) = pixel.g;
                            *(outCur++) = pixel.b;
                        }
                    }
                }
                else
                {
                    *(outCur++) = QOI_OP_RGBA;
                    *(outCur++) = pixel.r;
                    *(outCur++) = pixel.g;
                    *(outCur++) = pixel.b;
                    *(outCur++) = pixel.a;
                }
            }

            pixelPrev.val = pixel.val;
            ++inCur;
        }
    }

    MemCpyUpdCur(outCur, QOI_EOF, siz(QOI_EOF));

    cx AU outSize = SI(outCur - outBase);
    out.resize(outSize);
    MemCpy(out.data(), outBase, outSize);

    rets;
}
tpl1 dfa ER QoiDecode(std::vector<T1>& out, Size2<SI>& outSize, cx U1* in, SI inSize)
{
    out.clear();
    out.shrink_to_fit();
    outSize = Size2<SI>(0, 0);

    ifu (!in || inSize < QoiHdr::SIZE + siz(QOI_EOF))
        rete(ErrVal::NO_VALID);

    cx AU QoiReadU4 = [](cx U1*& inCur) -> U4 {
        U4 val;
        MemCpy(&val, inCur, siz(val));
        inCur += siz(val);
        ret RevByte(val);
    };

    AU inCur = in;
    cx AU hdrMagic = QoiReadU4(inCur);
    QoiHdr hdr;
    hdr.size.w = SI(QoiReadU4(inCur));
    hdr.size.h = SI(QoiReadU4(inCur));
    hdr.chCnt = SI(*(inCur++));
    hdr.colSpace = QoiColSpace(*(inCur++));

    ifu (hdrMagic != QOI_MAGIC || !hdr.size.Area() || !IsBetween<SI>(hdr.chCnt, 3, 4) || U1(hdr.colSpace) > 1)
        rete(ErrVal::NO_VALID);
    ifu (hdr.size.h >= QOI_PIXEL_CNT_MAX / hdr.size.w)
        rete(ErrVal::HIGH_SIZE);

    outSize = hdr.size;
    out.resize(outSize.Area());
    cx AU outBase = out.data();

    ColRgba index[64];
    MemSet(index, 0, siz(index));
    ColRgba pixel(0xFF000000);

    AU outCur = outBase;
    cx AU outEnd = outBase + outSize.Area();
    while (outCur != outEnd)
    {
        cx AU b1 = *(inCur++);

        if ((b1 & QOI_OP_MASK) == QOI_OP_INDEX)
        {
            pixel = index[b1];
        }
        else if ((b1 & QOI_OP_MASK) == QOI_OP_DIFF)
        {
            pixel.r += ((b1 >> 4) & 0x03) - 2;
            pixel.g += ((b1 >> 2) & 0x03) - 2;
            pixel.b += (b1 & 0x03) - 2;
        }
        else if ((b1 & QOI_OP_MASK) == QOI_OP_LUMA)
        {
            cx AU b2 = *(inCur++);
            cx AU vg = S1(S1(b1 & U1(0x3F)) - S1(32));
            pixel.r += vg - 8 + ((b2 >> 4) & 0x0F);
            pixel.g += vg;
            pixel.b += vg - 8 + (b2 & 0x0F);
        }
        else // QOI_OP_RUN or QOI_OP_RGB or QOI_OP_RGBA
        {
            if (b1 < QOI_OP_RGB)
            {
                cx AU run = SI(b1 & 0x3F);
                ite (i, i < run)
                {
                    ifcx (IsTypeSame<T1, ColRgba>)
                        outCur->val = pixel.val;
                    else
                        ToType<T1, ColRgba>(*outCur, pixel);
                    ++outCur;
                }
            }
            else
            {
                pixel.r = *(inCur++);
                pixel.g = *(inCur++);
                pixel.b = *(inCur++);
                if (b1 == QOI_OP_RGBA)
                    pixel.a = *(inCur++);
            }
        }

        index[_QoiColHash(pixel)] = pixel;

        ifcx (IsTypeSame<T1, ColRgba>)
            outCur->val = pixel.val;
        else
            ToType<T1, ColRgba>(*outCur, pixel);
        ++outCur;
    }

    rets;
}

tpl1 dfa ER ColGridToFileQoi(cx ColGrid<T1>& colGrid, cx CH* path)
{
    File file;
    ife (file.OpenWrite(path))
        retep;
    std::vector<U1> out;
    cx T1* in = &colGrid.pixels[0];
    Size2<SI> inSize = colGrid.size;
    ife (QoiEncode(out, in, inSize))
        retep;
    ife (file.Write(out.data(), out.size()))
        retep;
    ife (file.Close())
        retep;
    rets;
}
tpl1 dfa ER FileToColGridQoi(ColGrid<T1>& colGrid, cx CH* path)
{
    File file;
    ife (file.OpenRead(path))
        retep;
    std::vector<T1>& out = colGrid.pixels;
    Size2<SI>& outSize = colGrid.size;
    SI inSize;
    ife (file.SizeGet(inSize))
        retep;
    std::vector<U1> in;
    in.resize(inSize);
    ife (file.Read(in.data(), inSize))
        retep;
    ife (QoiDecode(out, outSize, in.data(), inSize))
        retep;
    ife (file.Close())
        retep;
    rets;
}
