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
        MemSet(m_dat.data(), 0, BitToByteSize(m_len));
        m_len = 0;
        m_free = m_dat.size() * BIT_IN_BYTE;
        m_cur = 0;
    }
    dfa NT Reserve(SI size)
    {
        ifl (size <= m_len + m_free)
            ret;
        m_dat.resize(BitToByteSize(size), 0);
        m_free = size - m_len;
    }
    dfa NT Reserve(SI sizeMin, SI sizeNew)
    {
        ifl (sizeMin <= m_len + m_free)
            ret;
        tx->Reserve((sizeNew == -1) ? SI(F8(sizeMin) * 1.5) : sizeNew);
    }

  public:
    dfa NT Set(CXGA buf, SI pos, SI size)
    {
        cx AU buf_ = (cx U1*)buf;

        tx->Reserve(BitToByteSize(pos + size) * BIT_IN_BYTE, -1);

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
                dstByte &= U1(~(((1 << bitCnt) - 1) << dstBitOfs));
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
            bufSize = VarbaseintEncode(buf, vals[i], U1(encodingBestI));
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
            in.ReadVarbasetruncint(val, U1(encodingBestI), valMaxLenb);
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
