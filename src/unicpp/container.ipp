#include "container.hpp"

#include "error.hpp"
#include "math.hpp"
#include "memory.hpp"
#include "random.hpp"

tpl1 dfa T1 Line2<T1>::Len() cx
{
    ret Dist<T1>(a, b);
}

tpl1 dfa Line2<T1>::Line2() noex
{
}
tpl1 dfa Line2<T1>::Line2(cx Pos2<T1>& a, cx Pos2<T1>& b) noex : a(a), b(b)
{
}
tpl1 dfa Line2<T1>::Line2(T1 x1, T1 y1, T1 x2, T1 y2) noex : a(x1, y1), b(x2, y2)
{
}

tpl1 dfa T1 Rect2<T1>::XEnd() cx
{
    ret pos.x + size.w;
}
tpl1 dfa T1 Rect2<T1>::YEnd() cx
{
    ret pos.y + size.h;
}
tpl1 dfa BO Rect2<T1>::IsIn(cx Pos2<T1>& pos) cx
{
    ret pos.x >= tx->pos.x && pos.x < XEnd() && pos.y >= tx->pos.y && pos.y < YEnd();
}

tpl1 dfa Rect2<T1>::Rect2()
{
}
tpl1 dfa Rect2<T1>::Rect2(cx Pos2<T1>& pos, cx Size2<T1>& size) : pos(pos), size(size)
{
}
tpl1 dfa Rect2<T1>::Rect2(T1 x, T1 y, T1 w, T1 h) : pos(x, y), size(w, h)
{
}

tpl1 dfa Triangle2<T1>::Triangle2()
{
}
tpl1 dfa Triangle2<T1>::Triangle2(cx Pos2<T1>& a, cx Pos2<T1>& b, cx Pos2<T1>& c) : a(a), b(b), c(c)
{
}
tpl1 dfa Triangle2<T1>::Triangle2(T1 x1, T1 y1, T1 x2, T1 y2, T1 x3, T1 y3) : a(x1, y1), b(x2, y2), c(x3, y3)
{
}

tpl1 dfa Circle2<T1>::Circle2()
{
}
tpl1 dfa Circle2<T1>::Circle2(cx Pos2<T1>& center, T1 radius) : center(center), radius(radius)
{
}
tpl1 dfa Circle2<T1>::Circle2(T1 x, T1 y, T1 radius) : center(x, y), radius(radius)
{
}

tpl1 dfa SI BitVec::GetVarintAt(T1& val, SI posb) cx
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
tpl1 dfa SI BitVec::ReadVarbaseint(T1& val, U1 base)
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
tpl1 dfa SI BitVec::ReadVarbasetruncint(T1& val, U1 base, SI sizebMax)
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

tpl<typename T1, SI TCap> dfa SI CircularBuf<T1, TCap>::_NextI(SI i) cx
{
    ret (i + 1) % TCap;
}
tpl<typename T1, SI TCap> dfa SI CircularBuf<T1, TCap>::_PrevI(SI i) cx
{
    ret (i + (TCap - 1)) % TCap;
}
tpl<typename T1, SI TCap> dfa SI CircularBuf<T1, TCap>::Len() cx
{
    ret m_len;
}
tpl<typename T1, SI TCap> dfa BO CircularBuf<T1, TCap>::IsEmpty() cx
{
    ret m_len == 0;
}
tpl<typename T1, SI TCap> dfa BO CircularBuf<T1, TCap>::IsFull() cx
{
    ret m_len == TCap;
}
tpl<typename T1, SI TCap> dfa T1& CircularBuf<T1, TCap>::NewFirst()
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
tpl<typename T1, SI TCap> dfa T1& CircularBuf<T1, TCap>::NewLast()
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
tpl<typename T1, SI TCap> dfa NT CircularBuf<T1, TCap>::DelFirst()
{
    Assert(!tx->IsEmpty());
    m_first = tx->_NextI(m_first);
    --m_len;
}
tpl<typename T1, SI TCap> dfa NT CircularBuf<T1, TCap>::DelLast()
{
    Assert(!tx->IsEmpty());
    m_end = tx->_PrevI(m_end);
    --m_len;
}
tpl<typename T1, SI TCap> dfa T1& CircularBuf<T1, TCap>::operator[](SI i)
{
    Assert(i < m_len);
    ret m_buf[(m_first + i) % TCap];
}
tpl<typename T1, SI TCap> dfa cx T1& CircularBuf<T1, TCap>::operator[](SI i) cx
{
    Assert(i < m_len);
    ret m_buf[(m_first + i) % TCap];
}
tpl<typename T1, SI TCap> dfa CircularBuf<T1, TCap>::CircularBuf()
{
    m_first = 0;
    m_end = 0;
    m_len = 0;
}

tpl1 dfa SI DCircularBuf<T1>::_NextI(SI i) cx
{
    ret (i + 1) % m_buf.size();
}
tpl1 dfa SI DCircularBuf<T1>::_PrevI(SI i) cx
{
    ret (i + (m_buf.size() - 1)) % m_buf.size();
}
tpl1 dfa SI DCircularBuf<T1>::Len() cx
{
    ret m_len;
}
tpl1 dfa BO DCircularBuf<T1>::IsEmpty() cx
{
    ret m_len == 0;
}
tpl1 dfa BO DCircularBuf<T1>::IsFull() cx
{
    ret m_len == SI(m_buf.size());
}
tpl1 dfa T1& DCircularBuf<T1>::NewFirst()
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
tpl1 dfa T1& DCircularBuf<T1>::NewLast()
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
tpl1 dfa NT DCircularBuf<T1>::DelFirst()
{
    Assert(!tx->IsEmpty());
    m_first = tx->_NextI(m_first);
    --m_len;
}
tpl1 dfa NT DCircularBuf<T1>::DelLast()
{
    Assert(!tx->IsEmpty());
    m_end = tx->_PrevI(m_end);
    --m_len;
}
tpl1 dfa NT DCircularBuf<T1>::Clr()
{
    m_first = 0;
    m_end = 0;
    m_len = 0;
}
tpl1 dfa NT DCircularBuf<T1>::New(SI cap)
{
    m_buf.resize(cap);
    tx->Clr();
}
tpl1 dfa NT DCircularBuf<T1>::Del()
{
    m_buf.clear();
    tx->Clr();
}
tpl1 dfa T1& DCircularBuf<T1>::operator[](SI i)
{
    Assert(i < m_len);
    ret m_buf[(m_first + i) % m_buf.size()];
}
tpl1 dfa cx T1& DCircularBuf<T1>::operator[](SI i) cx
{
    Assert(i < m_len);
    ret m_buf[(m_first + i) % m_buf.size()];
}
tpl1 dfa DCircularBuf<T1>::DCircularBuf()
{
    tx->Clr();
}

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

tpl<SI TSize, SI TAlign> dfa U1* DatBlock<TSize, TAlign>::Dat() noex
{
    ret u1;
}
tpl<SI TSize, SI TAlign> dfa cx U1* DatBlock<TSize, TAlign>::Dat() cx noex
{
    ret u1;
}
tpl<SI TSize, SI TAlign> dfa NT DatBlock<TSize, TAlign>::Clr() noex
{
    MemSet(u1, 0, TSize);
}
tpl<SI TSize, SI TAlign> dfa U1& DatBlock<TSize, TAlign>::operator[](SI i) noex
{
    ret u1[i];
}
tpl<SI TSize, SI TAlign> dfa cx U1& DatBlock<TSize, TAlign>::operator[](SI i) cx noex
{
    ret u1[i];
}
tpl<SI TSize, SI TAlign> dfa BO DatBlock<TSize, TAlign>::operator==(cx DatBlock& other) cx noex
{
    ret MemCmp(u1, other.u1, TSize) == 0;
}
tpl<SI TSize, SI TAlign> dfa BO DatBlock<TSize, TAlign>::operator!=(cx DatBlock& other) cx noex
{
    ret !(*tx == other);
}

tpl<SI TSize, SI TAlign> dfa NT DatBlockSecure<TSize, TAlign>::Clr() noex
{
    MemSet0Force(tx->u1, TSize);
}
tpl<SI TSize, SI TAlign> dfa DatBlockSecure<TSize, TAlign>::~DatBlockSecure() noex
{
    tx->Clr();
}

tpl1 dfa MthdObjList<T1>::Ref::Ref(GA ptrRaw, IdT id) noex : ptrRaw(ptrRaw), id(id)
{
}

tpl1 dfa NT MthdObjList<T1>::_IdGenUnique(IdT& id) cx
{
    do
        RandCrypt(&id, siz(id));
    while ((id == IdT()) || (m_mapId.find(id) != m_mapId.end()));
}
tpl1 dfa NT MthdObjList<T1>::_ClrDel(BO doSkipDel)
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
tpl1 dfa SI MthdObjList<T1>::Cnt() cx
{
    ThdLockFastAu lock(m_lock);
    ret m_bufStable.Cnt();
}
tpl1 dfa NT MthdObjList<T1>::List(std::vector<Ref>& refList) cx
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
tpl1 dfa NT MthdObjList<T1>::Del()
{
    ThdLockFastAu lock(m_lock);
    tx->_ClrDel(NO);
}
tpl1 dfa NT MthdObjList<T1>::New(SI cap)
{
    ThdLockFastAu lock(m_lock);
    tx->_ClrDel(NO);
    m_bufStable.New(cap);
}
tpl1 dfa NT MthdObjList<T1>::Clr()
{
    ThdLockFastAu lock(m_lock);
    tx->_ClrDel(YES);
}
tpl1 dfa NT MthdObjList<T1>::ElemDel(cx Ref& ref)
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
tpl1 tpl<typename... TArgs> dfa ER MthdObjList<T1>::ElemNew(Ref& ref, TArgs&&... args)
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
tpl1 dfa ER MthdObjList<T1>::IdSecSet(T1* ptr, cx IdSecT& idSec)
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
tpl1 dfa NT MthdObjList<T1>::Let(T1* ptr) cx
{
    TypeTraits<T1>::Unlock(*ptr);
}
tpl1 dfa T1* MthdObjList<T1>::Get(cx Ref& ref) cx
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
tpl1 dfa T1* MthdObjList<T1>::PeekUns(cx Ref& ref) cx
{
    cx AU elemPtr = (T1*)ref.ptrRaw;
    ifu (TypeTraits<T1>::id(*elemPtr) != ref.id) // reference is invalid
        ret NUL;
    ret elemPtr;
}

tpl1 dfa MthdObjListAu<T1>::MthdObjListAu(T1*& ptr, MthdObjList<T1>& objList, cx MthdObjList<T1>::Ref& ref) : m_ptr(ptr), m_objList(objList)
{
    ptr = m_objList.Get(ref);
}
tpl1 dfa MthdObjListAu<T1>::~MthdObjListAu()
{
    ifu (m_ptr == NUL)
        ret;
    m_objList.Let(m_ptr);
}
