#pragma once

tpl1 struct Line2
{
    Pos2<T1> a;
    Pos2<T1> b;

    dfa T1 Len() cx
    {
        ret Dist<T1>(a, b);
    }

    dfa Line2()
    {
    }
    dfa Line2(cx Pos2<T1>& a, cx Pos2<T1>& b) : a(a), b(b)
    {
    }
    dfa Line2(T1 x1, T1 y1, T1 x2, T1 y2) : a(x1, y1), b(x2, y2)
    {
    }
};

tpl1 struct Rect2
{
    Pos2<T1> pos;
    Size2<T1> size;

    dfa T1 XEnd() cx
    {
        ret pos.x + size.w;
    }
    dfa T1 YEnd() cx
    {
        ret pos.y + size.h;
    }
    dfa BO IsIn(cx Pos2<T1>& pos) cx
    {
        ret pos.x >= tx->pos.x && pos.x < XEnd() && pos.y >= tx->pos.y && pos.y < YEnd();
    }

    dfa Rect2()
    {
    }
    dfa Rect2(cx Pos2<T1>& pos, cx Size2<T1>& size) : pos(pos), size(size)
    {
    }
    dfa Rect2(T1 x, T1 y, T1 w, T1 h) : pos(x, y), size(w, h)
    {
    }
};

tpl1 struct Triangle2
{
    Pos2<T1> a;
    Pos2<T1> b;
    Pos2<T1> c;

    dfa Triangle2()
    {
    }
    dfa Triangle2(cx Pos2<T1>& a, cx Pos2<T1>& b, cx Pos2<T1>& c) : a(a), b(b), c(c)
    {
    }
    dfa Triangle2(T1 x1, T1 y1, T1 x2, T1 y2, T1 x3, T1 y3) : a(x1, y1), b(x2, y2), c(x3, y3)
    {
    }
};

tpl1 struct Circle2
{
    Pos2<T1> center;
    T1 radius;

    dfa Circle2()
    {
    }
    dfa Circle2(cx Pos2<T1>& center, T1 radius) : center(center), radius(radius)
    {
    }
    dfa Circle2(T1 x, T1 y, T1 radius) : center(x, y), radius(radius)
    {
    }
};

class BitVec
{
  private:
    std::vector<U1> m_dat;
    SI m_len;  // length in bits
    SI m_free; // free bits at the end
    SI m_cur;  // cursor position in bits

  public:
    dfa U1* _Dat()
    {
        ret m_dat.data();
    }
    dfa NT _LenSet(SI len)
    {
        cx AU delta = len - m_len;
        m_len = len;
        m_free -= delta;
    }

  public:
    dfa NT Clr()
    {
        MemSet(m_dat.data(), 0, m_dat.size());
        m_len = 0;
        m_free = m_dat.size() * BIT_IN_BYTE;
        m_cur = 0;
    }
    dfa NT Reserve(SI size, SI newSize = -1)
    {
        ifl (size <= m_len + m_free)
            ret;
        if (newSize == -1)
            newSize = SI(size * 1.5);
        m_dat.resize(BitToByteSize(newSize), 0);
        m_free = newSize - m_len;
    }

  public:
    dfa NT Set(CXGA buf, SI pos, SI size)
    {
        cx AU buf_ = (cx U1*)buf;

        tx->Reserve(BitToByteSize(pos + size) * BIT_IN_BYTE);

        if ((pos % BIT_IN_BYTE) == 0 && (size % BIT_IN_BYTE) == 0)
        {
            MemCpy(m_dat.data() + (pos / BIT_IN_BYTE), buf_, size / BIT_IN_BYTE);
        }
        else
        {
            AU dstBitPos = pos;
            AU srcBitPos = SI(0);
            AU bitsRemain = size;

            while (bitsRemain > 0)
            {
                cx SI dstByteI = dstBitPos / BIT_IN_BYTE;
                cx SI dstBitOfs = dstBitPos % BIT_IN_BYTE;
                cx SI srcByteI = srcBitPos / BIT_IN_BYTE;
                cx SI srcBitOfs = srcBitPos % BIT_IN_BYTE;
                cx SI bitCnt = Min(bitsRemain, BIT_IN_BYTE - dstBitOfs, BIT_IN_BYTE - srcBitOfs);
                cx U1 srcByte = buf_[srcByteI];
                cx U1 srcBits = (srcByte >> srcBitOfs) & ((1 << bitCnt) - 1);

                U1& dstByte = m_dat[dstByteI];
                dstByte &= ~(((1 << bitCnt) - 1) << dstBitOfs);
                dstByte |= srcBits << dstBitOfs;

                dstBitPos += bitCnt;
                srcBitPos += bitCnt;
                bitsRemain -= bitCnt;
            }
        }

        m_len = Max(m_len, pos + size);
        m_free = m_dat.size() * BIT_IN_BYTE - m_len;
    }
    dfa NT Get(GA buf, SI pos, SI size) cx
    {
        AU buf_ = (U1*)buf;

        if ((pos % BIT_IN_BYTE) == 0 && (size % BIT_IN_BYTE) == 0)
        {
            MemCpy(buf_, m_dat.data() + (pos / BIT_IN_BYTE), size / BIT_IN_BYTE);
        }
        else
        {
            MemSet(buf_, 0, BitToByteSize(size));

            AU srcBitPos = pos;
            AU dstBitPos = SI(0);
            AU bitsRemain = size;

            while (bitsRemain > 0)
            {
                cx SI srcByteI = srcBitPos / BIT_IN_BYTE;
                cx SI srcBitOfs = srcBitPos % BIT_IN_BYTE;
                cx SI dstByteI = dstBitPos / BIT_IN_BYTE;
                cx SI dstBitOfs = dstBitPos % BIT_IN_BYTE;
                cx SI bitCnt = Min(bitsRemain, BIT_IN_BYTE - srcBitOfs, BIT_IN_BYTE - dstBitOfs);
                cx U1 srcByte = m_dat[srcByteI];
                cx U1 bits = (srcByte >> srcBitOfs) & ((1 << bitCnt) - 1);

                U1& dstByte = buf_[dstByteI];
                dstByte |= bits << dstBitOfs;

                srcBitPos += bitCnt;
                dstBitPos += bitCnt;
                bitsRemain -= bitCnt;
            }
        }
    }
    dfa NT AddLast(CXGA buf, SI size)
    {
        tx->Set(buf, m_len, size);
    }
    tpl1 dfa SI GetVarint(T1& val, SI pos) cx
    {
        U1 buf[VarintSizeMax<T1>()];
        SI size = 0;
        do
        {
            tx->Get(buf + size, pos + size * BIT_IN_BYTE, BIT_IN_BYTE);
            ++size;
        } while (VarintIsIncomplete(buf, size));
        ret VarintDecode(val, buf);
    }
    dfa NT Read(GA buf, SI size)
    {
        tx->Get(buf, m_cur, size);
        m_cur += size;
    }
    tpl1 dfa SI ReadVarbaseint(T1& val, U1 base)
    {
        U1 buf[VarbaseintSizeMax<T1, 1>()];
        SI size = 0;
        do
        {
            tx->Read(buf + size, BIT_IN_BYTE);
            ++size;
        } while (VarbaseintIsIncomplete(buf, size, base));
        ret VarbaseintDecode(val, buf, base);
    }
    tpl1 dfa SI ReadVarbasetruncint(T1& val, U1 base, SI sizebMax)
    {
        U1 buf[VarbaseintSizeMax<T1, 1>()];
        SI size = 0;
        do
        {
            tx->Read(buf + size, Min(BIT_IN_BYTE, sizebMax - size * BIT_IN_BYTE));
            ++size;
        } while (VarbaseintIsIncomplete(buf, size, base));
        ret VarbaseintDecode(val, buf, base);
    }
    dfa NT PadToByte()
    {
        cx AU zero = U1(0);
        tx->Set(&zero, m_len, (BIT_IN_BYTE - (m_len % BIT_IN_BYTE)) % BIT_IN_BYTE);
    }

  public:
    dfa cx U1* Dat() cx
    {
        ret m_dat.data();
    }
    dfa SI Size() cx
    {
        ret m_len;
    }
    dfa SI SizeByte() cx
    {
        ret BitToByteSize(m_len);
    }

  public:
    dfa BitVec() : m_len(0), m_free(0), m_cur(0)
    {
    }
};

tpl1 dfa NT ValSeqBoxEncode(BitVec& out, cx T1* vals, SI valCnt)
{
    ifu (valCnt < 1)
        ret;

    AU valMax = vals[0];
    for (SI i = 1; i < valCnt; ++i)
    {
        if (vals[i] > valMax)
            valMax = vals[i];
    }
    cx AU valMaxLenb = LenBin(valMax);

    struct Encoding
    {
        SI valMaxLenb;
        SI sizeb;

        dfa Encoding() : valMaxLenb(0), sizeb(0)
        {
        }
    };
    cxex SI ENCODING_CNT = 8;
    Encoding encoding[ENCODING_CNT];

    U1 buf[64];
    SI bufSize;

    encoding[0].valMaxLenb = valMaxLenb;
    encoding[0].sizeb = valMaxLenb * valCnt;

    for (U1 base = 1; base < 8; ++base)
    {
        cx AU encodingValMaxLen = VarbaseintEncode(buf, valMax, base);
        encoding[base].valMaxLenb = ((encodingValMaxLen - SI(1)) * BIT_IN_BYTE) + LenBin<U1>(*(buf + (encodingValMaxLen - SI(1))));

        ite (i, i < valCnt)
            encoding[base].sizeb += Min(VarbaseintEncodeSize(vals[i], base) * BIT_IN_BYTE, encoding[base].valMaxLenb);
    }

    SI encodingBestI = 0;
    for (SI i = 1; i < ENCODING_CNT; ++i)
    {
        if (encoding[i].sizeb < encoding[encodingBestI].sizeb)
            encodingBestI = i;
    }
    AU& encodingBest = encoding[encodingBestI];

    out.Clr();
    out.Reserve(16 * BIT_IN_BYTE + encodingBest.sizeb);

    cx AU zero = U8(0);
    out.AddLast(&zero, sizb(zero));
    bufSize = VarbaseintEncode(buf, valCnt, 5);
    out.AddLast(buf, bufSize * BIT_IN_BYTE);
    out.AddLast(&encodingBest.valMaxLenb, 7);
    out.AddLast(&encodingBestI, 3);

    if (encodingBestI == 0)
    {
        ite (i, i < valCnt)
            out.AddLast(&vals[i], valMaxLenb);
    }
    else
    {
        ite (i, i < valCnt)
        {
            bufSize = VarbaseintEncode(buf, vals[i], encodingBestI);
            out.AddLast(buf, Min(bufSize * BIT_IN_BYTE, encodingBest.valMaxLenb));
        }
    }

    out.PadToByte();

    bufSize = VarbaseintEncode(out._Dat(), out.SizeByte() - siz(U8), 7);
    MemMove(out._Dat() + bufSize, out.Dat() + siz(U8), out.SizeByte() - siz(U8));
    out._LenSet(out.Size() - ((siz(U8) - bufSize) * BIT_IN_BYTE));
}
tpl1 dfa NT ValSeqBoxDecode(std::vector<T1>& vals, BitVec& in)
{
    SI datSize;
    in.ReadVarbaseint(datSize, 7);
    SI valCnt;
    in.ReadVarbaseint(valCnt, 5);
    SI valMaxLenb = 0;
    in.Read(&valMaxLenb, 7);
    SI encodingBestI = 0;
    in.Read(&encodingBestI, 3);

    vals.resize(valCnt);

    if (encodingBestI == 0)
    {
        for (AU& val : vals)
        {
            val = 0;
            in.Read(&val, valMaxLenb);
        }
    }
    else
    {
        for (AU& val : vals)
            in.ReadVarbasetruncint(val, encodingBestI, valMaxLenb);
    }
}

tpl<typename T1, SI TCap> class CircularBuf
{
  private:
    std::array<T1, TCap> m_buf;
    SI m_first;
    SI m_end;
    SI m_len;

  private:
    dfa SI _NextI(SI i) cx
    {
        ret (i + 1) % TCap;
    }
    dfa SI _PrevI(SI i) cx
    {
        ret (i + (TCap - 1)) % TCap;
    }

  public:
    dfa SI Size() cx
    {
        ret m_len;
    }
    dfa BO IsEmpty() cx
    {
        ret m_len == 0;
    }
    dfa BO IsFull() cx
    {
        ret m_len == TCap;
    }

  public:
    dfa T1& NewFirst()
    {
        if (tx->IsFull())
            m_end = tx->_PrevI(m_end);
        else
            ++m_len;
        m_first = tx->_PrevI(m_first);
        T1& val = m_buf[m_first];
        val = T1();
        ret val;
    }
    dfa T1& NewLast()
    {
        if (tx->IsFull())
            m_first = tx->_NextI(m_first);
        else
            ++m_len;
        T1& val = m_buf[m_end];
        m_end = tx->_NextI(m_end);
        val = T1();
        ret val;
    }
    dfa NT DelFirst()
    {
        Assert(!tx->IsEmpty());
        m_first = tx->_NextI(m_first);
        --m_len;
    }
    dfa NT DelLast()
    {
        Assert(!tx->IsEmpty());
        m_end = tx->_PrevI(m_end);
        --m_len;
    }

  public:
    dfa T1& operator[](SI i)
    {
        Assert(i < m_len);
        ret m_buf[(m_first + i) % TCap];
    }
    dfa cx T1& operator[](SI i) cx
    {
        Assert(i < m_len);
        ret m_buf[(m_first + i) % TCap];
    }

  public:
    dfa CircularBuf()
    {
        m_first = 0;
        m_end = 0;
        m_len = 0;
    }
};

tpl1 Rect2<T1> Rect2FitRatio(cx Rect2<T1>& rect, F4 ratio)
{
    if ((F4(rect.size.w) / F4(rect.size.h)) > ratio)
    {
        ret Rect2<T1>({rect.pos.x, rect.pos.y}, {T1(rect.size.h * ratio), rect.size.h});
    }
    else
    {
        ret Rect2<T1>({rect.pos.x, rect.pos.y}, {rect.size.w, T1(rect.size.w / ratio)});
    }
}
tpl1 Rect2<T1> Rect2Center(cx Rect2<T1>& rect, cx Rect2<T1>& rectOuter)
{
    ret Rect2<T1>({rectOuter.pos.x + (rectOuter.size.w - rect.size.w) / 2, rectOuter.pos.y + (rectOuter.size.h - rect.size.h) / 2}, rect.size);
}
tpl1 Rect2<T1> Rect2FitRatioCenter(cx Rect2<T1>& rect, F4 ratio)
{
    if ((F4(rect.size.w) / F4(rect.size.h)) > ratio)
    {
        cx AU w = T1(rect.size.h * ratio);
        ret Rect2<T1>({rect.pos.x + (rect.size.w - w) / 2, rect.pos.y}, {w, rect.size.h});
    }
    else
    {
        cx AU h = T1(rect.size.w / ratio);
        ret Rect2<T1>({rect.pos.x, rect.pos.y + (rect.size.h - h) / 2}, {rect.size.w, h});
    }
}
