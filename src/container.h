#pragma once

// pre-defined:
dfa NT RandCrypt(GA buf, SI size);

tpl1 struct Line2
{
    Pos2<T1> a;
    Pos2<T1> b;

    dfa T1 Len() cx
    {
        ret Dist<T1>(a, b);
    }

    dfa Line2() noex
    {
    }
    dfa Line2(cx Pos2<T1>& a, cx Pos2<T1>& b) noex : a(a), b(b)
    {
    }
    dfa Line2(T1 x1, T1 y1, T1 x2, T1 y2) noex : a(x1, y1), b(x2, y2)
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
    SI m_lenb;  // length in bits
    SI m_freeb; // free bits at the end
    SI m_curb;  // cursor position in bits

  public:
    dfa U1* _Dat()
    {
        ret m_dat.data();
    }
    dfa NT _LenbSet(SI lenb)
    {
        cx AU delta = lenb - m_lenb;
        m_lenb = lenb;
        m_freeb -= delta;
    }

  public:
    dfa NT Clr()
    {
        MemSet(m_dat.data(), 0, BitToByteSize(m_lenb));
        m_lenb = 0;
        m_freeb = m_dat.size() * BIT_IN_BYTE;
        m_curb = 0;
    }
    dfa NT Reserve(SI sizeb)
    {
        ifl (sizeb <= m_lenb + m_freeb)
            ret;
        m_dat.resize(BitToByteSize(sizeb), 0);
        m_freeb = sizeb - m_lenb;
    }
    dfa NT Reserve(SI sizebMin, SI sizebNew)
    {
        ifl (sizebMin <= m_lenb + m_freeb)
            ret;
        tx->Reserve((sizebNew == -1) ? SI(F8(sizebMin) * 1.5) : sizebNew);
    }

  public:
    dfa NT SetAt(CXGA buf, SI sizeb, SI posb)
    {
        cx AU buf_ = (cx U1*)buf;

        tx->Reserve(BitToByteSize(posb + sizeb) * BIT_IN_BYTE, -1);

        if ((posb % BIT_IN_BYTE) == 0 && (sizeb % BIT_IN_BYTE) == 0)
        {
            MemCpy(m_dat.data() + (posb / BIT_IN_BYTE), buf_, sizeb / BIT_IN_BYTE);
        }
        else
        {
            AU dstBitPos = posb;
            AU srcBitPos = SI(0);
            AU bitsRemain = sizeb;

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
                dstByte &= U1(~(((1 << bitCnt) - 1) << dstBitOfs));
                dstByte |= srcBits << dstBitOfs;

                dstBitPos += bitCnt;
                srcBitPos += bitCnt;
                bitsRemain -= bitCnt;
            }
        }

        m_lenb = Max(m_lenb, posb + sizeb);
        m_freeb = m_dat.size() * BIT_IN_BYTE - m_lenb;
    }
    dfa NT GetAt(GA buf, SI sizeb, SI posb) cx
    {
        AU buf_ = (U1*)buf;

        if ((posb % BIT_IN_BYTE) == 0 && (sizeb % BIT_IN_BYTE) == 0)
        {
            MemCpy(buf_, m_dat.data() + (posb / BIT_IN_BYTE), sizeb / BIT_IN_BYTE);
        }
        else
        {
            MemSet(buf_, 0, BitToByteSize(sizeb));

            AU srcBitPos = posb;
            AU dstBitPos = SI(0);
            AU bitsRemain = sizeb;

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
    dfa NT AddLast(CXGA buf, SI sizeb)
    {
        tx->SetAt(buf, sizeb, m_lenb);
    }
    tpl1 dfa SI GetVarintAt(T1& val, SI posb) cx
    {
        U1 buf[VarintSizeMax<T1>()];
        SI size = 0;
        do
        {
            tx->GetAt(buf + size, BIT_IN_BYTE, posb + size * BIT_IN_BYTE);
            ++size;
        } while (VarintIsIncomplete(buf, size));
        ret VarintDecode(val, buf);
    }
    dfa NT Read(GA buf, SI sizeb)
    {
        tx->GetAt(buf, sizeb, m_curb);
        m_curb += sizeb;
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
        tx->SetAt(&zero, (BIT_IN_BYTE - (m_lenb % BIT_IN_BYTE)) % BIT_IN_BYTE, m_lenb);
    }

  public:
    dfa cx U1* Dat() cx
    {
        ret m_dat.data();
    }
    dfa SI SizeBit() cx
    {
        ret m_lenb;
    }
    dfa SI SizeByte() cx
    {
        ret BitToByteSize(m_lenb);
    }

  public:
    dfa BitVec() : m_lenb(0), m_freeb(0), m_curb(0)
    {
    }
};

using ValSeqBoxModeT = U1;
enum class ValSeqBoxMode : ValSeqBoxModeT
{
    DEFA,      // keep values as they are
    DIFF,      // store value differences, general case (can be negative)
    DIFF_ASC,  // store value differences, optimal for ascending ordered values
    DIFF2,     // store value differences of differences, general case (can be negative)
    DIFF2_ASC, // store value differences of differences, optimal for ascending ordered DIFFERENCE values
};

tpl<typename T1, ValSeqBoxMode TMode> dfa NT ValSeqBoxEncode(BitVec& out, cx std::span<cx T1>& vals)
{
    std::vector<T1> valsMod;
    std::span<T1> valsModSpan;
    ifcx (TMode == ValSeqBoxMode::DIFF || TMode == ValSeqBoxMode::DIFF_ASC)
    {
        valsMod.resize(vals.size());
        T1 valPrev = 0;
        ite (i, i < SI(vals.size()))
        {
            ifcx (TMode == ValSeqBoxMode::DIFF_ASC) // if ascending order is guaranteed, diff is always non-negative
                valsMod[i] = vals[i] - valPrev;
            else
                valsMod[i] = ZigzagEncode(vals[i] - valPrev);
            valPrev = vals[i];
        }
        valsModSpan = valsMod;
    }
    ifcx (TMode == ValSeqBoxMode::DIFF2 || TMode == ValSeqBoxMode::DIFF2_ASC)
    {
        valsMod.resize(vals.size());
        T1 valPrev = 0;
        T1 glide = 0;
        ite (i, i < SI(vals.size()))
        {
            cx AU glideDiff = (vals[i] - valPrev) - glide;
            ifcx (TMode == ValSeqBoxMode::DIFF2_ASC) // if ascending order is guaranteed, diff is always non-negative
                valsMod[i] = glideDiff;
            else
                valsMod[i] = ZigzagEncode(glideDiff);
            glide += glideDiff;
            valPrev = vals[i];
        }
        valsModSpan = valsMod;
    }

    cx std::span<cx T1>& valsProc_ = (TMode == ValSeqBoxMode::DEFA) ? vals : valsModSpan;
    cx std::span<cx GetTypeU<T1>>& valsProc = AsType<cx std::span<cx GetTypeU<T1>>>(valsProc_);

    GetTypeU<T1> valMax = ((valsProc.size() > 0) ? valsProc[0] : GetTypeU<T1>(0));
    for (SI i = 1; i < SI(valsProc.size()); ++i)
        if (valsProc[i] > valMax)
            valMax = valsProc[i];
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
    encoding[0].sizeb = valMaxLenb * SI(valsProc.size());

    for (U1 base = 1; base < 8; ++base)
    {
        cx AU encodingValMaxLen = VarbaseintEncode(buf, valMax, base);
        encoding[base].valMaxLenb = ((encodingValMaxLen - SI(1)) * BIT_IN_BYTE) + LenBin<U1>(*(buf + (encodingValMaxLen - SI(1))));

        ite (i, i < SI(valsProc.size()))
            encoding[base].sizeb += Min(VarbaseintEncodeSize(valsProc[i], base) * BIT_IN_BYTE, encoding[base].valMaxLenb);
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
    bufSize = VarbaseintEncode(buf, GetTypeU<SI>(valsProc.size()), 5);
    out.AddLast(buf, bufSize * BIT_IN_BYTE);
    out.AddLast(&encodingBest.valMaxLenb, 7);
    out.AddLast(&encodingBestI, 3);

    if (encodingBestI == 0)
    {
        ite (i, i < SI(valsProc.size()))
            out.AddLast(&valsProc[i], valMaxLenb);
    }
    else
    {
        ite (i, i < SI(valsProc.size()))
        {
            bufSize = VarbaseintEncode(buf, valsProc[i], U1(encodingBestI));
            out.AddLast(buf, Min(bufSize * BIT_IN_BYTE, encodingBest.valMaxLenb));
        }
    }

    out.PadToByte();

    bufSize = VarbaseintEncode(out._Dat(), AsTypeU(out.SizeByte() - siz(U8)), 7);
    MemMove(out._Dat() + bufSize, out.Dat() + siz(U8), out.SizeByte() - siz(U8));
    out._LenbSet(out.SizeBit() - ((siz(U8) - bufSize) * BIT_IN_BYTE));
}
tpl<typename T1, ValSeqBoxMode TMode> dfa NT ValSeqBoxDecode(std::vector<T1>& vals, BitVec& in)
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
            in.ReadVarbasetruncint(val, U1(encodingBestI), valMaxLenb);
    }

    ifcx (TMode == ValSeqBoxMode::DIFF || TMode == ValSeqBoxMode::DIFF_ASC)
    {
        T1 valPrev = 0;
        ite (i, i < SI(vals.size()))
        {
            ifcx (TMode == ValSeqBoxMode::DIFF_ASC) // if ascending order is guaranteed, diff is always non-negative
                vals[i] = valPrev + vals[i];
            else
                vals[i] = valPrev + ZigzagDecode(AsTypeU(vals[i]));
            valPrev = vals[i];
        }
    }
    ifcx (TMode == ValSeqBoxMode::DIFF2 || TMode == ValSeqBoxMode::DIFF2_ASC)
    {
        T1 valPrev = 0;
        T1 glide = 0;
        ite (i, i < SI(vals.size()))
        {
            T1 glideDiff;
            ifcx (TMode == ValSeqBoxMode::DIFF2_ASC) // if ascending order is guaranteed, diff is always non-negative
                glideDiff = vals[i];
            else
                glideDiff = ZigzagDecode(AsTypeU(vals[i]));
            glide += glideDiff;
            vals[i] = valPrev + glide;
            valPrev = vals[i];
        }
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
    dfa SI Len() cx
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
tpl<typename T1> class DCircularBuf
{
  private:
    std::vector<T1> m_buf;
    SI m_first;
    SI m_end;
    SI m_len;

  private:
    dfa SI _NextI(SI i) cx
    {
        ret (i + 1) % m_buf.size();
    }
    dfa SI _PrevI(SI i) cx
    {
        ret (i + (m_buf.size() - 1)) % m_buf.size();
    }

  public:
    dfa SI Len() cx
    {
        ret m_len;
    }
    dfa BO IsEmpty() cx
    {
        ret m_len == 0;
    }
    dfa BO IsFull() cx
    {
        ret m_len == SI(m_buf.size());
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
    dfa NT Clr()
    {
        m_first = 0;
        m_end = 0;
        m_len = 0;
    }
    dfa NT New(SI cap)
    {
        m_buf.resize(cap);
        tx->Clr();
    }
    dfa NT Del()
    {
        m_buf.clear();
        tx->Clr();
    }

  public:
    dfa T1& operator[](SI i)
    {
        Assert(i < m_len);
        ret m_buf[(m_first + i) % m_buf.size()];
    }
    dfa cx T1& operator[](SI i) cx
    {
        Assert(i < m_len);
        ret m_buf[(m_first + i) % m_buf.size()];
    }

  public:
    dfa DCircularBuf()
    {
        tx->Clr();
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

tpl<SI TSize, SI TAlign = alignof(U8)> struct alignas(TAlign) DatBlock
{
    static_assert(TSize >= siz(U8), "DatBlock size must be at least 8 bytes");
    static_assert(TSize % TAlign == 0, "DatBlock size must be a multiple of alignment");

    static cxex SI SIZE = TSize;
    static cxex SI ALIGN = TAlign;

    union {
        U1 u1[TSize / siz(U1)];
        U2 u2[TSize / siz(U2)];
        U4 u4[TSize / siz(U4)];
        U8 u8[TSize / siz(U8)];
    };

    dfa U1* Dat() noex
    {
        ret u1;
    }
    dfa cx U1* Dat() cx noex
    {
        ret u1;
    }
    dfa NT Clr() noex
    {
        MemSet(u1, 0, TSize);
    }
    dfa U1& operator[](SI i) noex
    {
        ret u1[i];
    }
    dfa cx U1& operator[](SI i) cx noex
    {
        ret u1[i];
    }
    dfa BO operator==(cx DatBlock& other) cx noex
    {
        ret MemCmp(u1, other.u1, TSize) == 0;
    }
    dfa BO operator!=(cx DatBlock& other) cx noex
    {
        ret !(*tx == other);
    }

    dfa NT operator+(S4) cx = delete;
    dfa NT operator-(S4) cx = delete;
};

tpl<SI TSize, SI TAlign = alignof(U8)> struct alignas(TAlign) DatBlockSecure : DatBlock<TSize, TAlign>
{
    dfa NT Clr() noex
    {
        MemSet0Force(tx->u1, TSize);
    }
    dfa ~DatBlockSecure() noex
    {
        tx->Clr();
    }
};

// a varint which is serialized using zigzag encoding
struct ZzVarint
{
    S8 val;

    dfa ZzVarint(S8 val = 0) noex : val(val)
    {
    }
};

tpl1 class MthdObjList
{
  private:
    using LockT = TypeTraits<T1>::LockT;
    using IdT = TypeTraits<T1>::IdT;
    using IdSecT = TypeTraits<T1>::IdSecT;
    static_assert(IsTypeSame<LockT, ThdLockFast>, "LockT must be ThdLockFast");
    static_assert(siz(IdT) >= siz(U8), "IdT must be at least 8 bytes");

  public:
    struct Ref
    {
        GA ptrRaw;
        IdT id;

        dfa Ref(GA ptrRaw = NUL, IdT id = IdT()) noex : ptrRaw(ptrRaw), id(id)
        {
        }
    };

  private:
    mutable ThdLockFast m_lock;
    ArrSFree<T1> m_bufStable;
    std::unordered_map<IdT, T1*> m_mapId;
    std::unordered_map<IdSecT, T1*> m_mapIdSec;

  private:
    dfa NT _IdGenUnique(IdT& id) cx
    {
        do
            RandCrypt(&id, siz(id));
        while ((id == IdT()) || (m_mapId.find(id) != m_mapId.end()));
    }
    dfa NT _ClrDel(BO doSkipDel)
    {
        cx AU cnt = m_bufStable.Cnt();
        std::vector<T1*> elemList(cnt);
        ite (i, i < cnt)
        {
            AU& elem = m_bufStable[i];
            elemList[i] = &elem;
            TypeTraits<T1>::Lock(*elemList[i]);
            TypeTraits<T1>::id(elem) = IdT(); // reset id
        }
        m_mapId.clear();
        m_mapIdSec.clear();
        m_bufStable.Clr();
        if (!doSkipDel)
            m_bufStable.Del();
        if (doSkipDel)
            ite (i, i < cnt)
                TypeTraits<T1>::Unlock(*elemList[i]);
    }

  public:
    dfa SI Cnt() cx
    {
        ThdLockFastAu lock(m_lock);
        ret m_bufStable.Cnt();
    }
    dfa NT List(std::vector<Ref>& refList) cx
    {
        refList.clear();
        ThdLockFastAu lock(m_lock);
        cx AU cnt = m_bufStable.Cnt();
        refList.reserve(cnt);
        ite (i, i < cnt)
        {
            AU& elem = m_bufStable[i];
            refList.emplace_back(const_cast<T1*>(&elem), TypeTraits<T1>::id(elem));
        }
    }
    dfa NT Del()
    {
        ThdLockFastAu lock(m_lock);
        tx->_ClrDel(NO);
    }
    dfa NT New(SI cap)
    {
        ThdLockFastAu lock(m_lock);
        tx->_ClrDel(NO);
        m_bufStable.New(cap);
    }
    dfa NT Clr()
    {
        ThdLockFastAu lock(m_lock);
        tx->_ClrDel(YES);
    }
    dfa NT ElemDel(cx Ref& ref)
    {
        ThdLockFastAu lock(m_lock);
        ThdLockFastAu lockElem(TypeTraits<T1>::lock(*(T1*)ref.ptrRaw));
        cx AU elemPtr = (T1*)ref.ptrRaw;
        ifu (TypeTraits<T1>::id(*elemPtr) != ref.id) // reference is invalid
            ret;
        m_mapId.erase(ref.id);
        if (TypeTraits<T1>::HasIdSec(*elemPtr))
            m_mapIdSec.erase(TypeTraits<T1>::idSec(*elemPtr));
        m_bufStable.ElemDel(elemPtr);
        TypeTraits<T1>::id(*elemPtr) = IdT(); // reset id
        ret;
    }
    tpl<typename... TArgs> dfa ER ElemNew(Ref& ref, TArgs&&... args)
    {
        ref = Ref();
        ThdLockFastAu lock(m_lock);
        AU elemPtr = m_bufStable.ElemNew(std::forward<TArgs>(args)...);
        ifu (elemPtr == NUL) // out of free elements
            rete(ErrVal::HIGH_SIZE);
        ref.ptrRaw = elemPtr;
        tx->_IdGenUnique(ref.id);
        TypeTraits<T1>::id(*elemPtr) = ref.id;
        m_mapId[ref.id] = elemPtr;
        rets;
    }
    dfa ER IdSecSet(T1* ptr, cx IdSecT& idSec)
    {
        ThdLockFastAu lock(m_lock);
        cx AU& it = m_mapIdSec.find(idSec);
        ifu (it != m_mapIdSec.end())
        {
            if (it->second == ptr)
                rets;
            rete(ErrVal::YES_EXIST);
        }
        if (TypeTraits<T1>::HasIdSec(*ptr))
            m_mapIdSec.erase(TypeTraits<T1>::idSec(*ptr));
        TypeTraits<T1>::idSec(*ptr) = idSec;
        m_mapIdSec[idSec] = ptr;
        rets;
    }
    dfa NT Let(T1* ptr) cx
    {
        TypeTraits<T1>::Unlock(*ptr);
    }
    dfa T1* Get(cx Ref& ref) cx
    {
        TypeTraits<T1>::Lock(*(T1*)ref.ptrRaw);
        cx AU elemPtr = (T1*)ref.ptrRaw;
        ifu (TypeTraits<T1>::id(*elemPtr) != ref.id) // reference is invalid
        {
            TypeTraits<T1>::Unlock(*(T1*)ref.ptrRaw);
            ret NUL;
        }
        ret elemPtr;
    }
    dfa T1* PeekUns(cx Ref& ref) cx
    {
        cx AU elemPtr = (T1*)ref.ptrRaw;
        ifu (TypeTraits<T1>::id(*elemPtr) != ref.id) // reference is invalid
            ret NUL;
        ret elemPtr;
    }
};

tpl1 class MthdObjListAu
{
  private:
    T1*& m_ptr;
    MthdObjList<T1>& m_objList;

  public:
    dfa MthdObjListAu(T1*& ptr, MthdObjList<T1>& objList, cx MthdObjList<T1>::Ref& ref) : m_ptr(ptr), m_objList(objList)
    {
        ptr = m_objList.Get(ref);
    }
    dfa ~MthdObjListAu()
    {
        ifu (m_ptr == NUL)
            ret;
        m_objList.Let(m_ptr);
    }
};
