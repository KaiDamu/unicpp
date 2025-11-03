#pragma once

#include <span>
#include <vector>
#include <unordered_map>
#include "vector.hpp"
#include "lock.hpp"
#include "array.hpp"

tpl1 struct Line2
{
    Pos2<T1> a;
    Pos2<T1> b;

    dfa T1 Len() cx;

    dfa Line2() noex;
    dfa Line2(cx Pos2<T1>& a, cx Pos2<T1>& b) noex;
    dfa Line2(T1 x1, T1 y1, T1 x2, T1 y2) noex;
};

tpl1 struct Rect2
{
    Pos2<T1> pos;
    Size2<T1> size;

    dfa T1 XEnd() cx;
    dfa T1 YEnd() cx;
    dfa BO IsIn(cx Pos2<T1>& pos) cx;

    dfa Rect2();
    dfa Rect2(cx Pos2<T1>& pos, cx Size2<T1>& size);
    dfa Rect2(T1 x, T1 y, T1 w, T1 h);

    dfa BO operator==(cx Rect2<T1>& other) cx;
    dfa BO operator!=(cx Rect2<T1>& other) cx;
};

tpl1 struct Triangle2
{
    Pos2<T1> a;
    Pos2<T1> b;
    Pos2<T1> c;

    dfa Triangle2();
    dfa Triangle2(cx Pos2<T1>& a, cx Pos2<T1>& b, cx Pos2<T1>& c);
    dfa Triangle2(T1 x1, T1 y1, T1 x2, T1 y2, T1 x3, T1 y3);
};

tpl1 struct Circle2
{
    Pos2<T1> center;
    T1 radius;

    dfa Circle2();
    dfa Circle2(cx Pos2<T1>& center, T1 radius);
    dfa Circle2(T1 x, T1 y, T1 radius);
};

class BitVec
{
  private:
    std::vector<U1> m_dat;
    SI m_lenb;  // length in bits
    SI m_freeb; // free bits at the end
    SI m_curb;  // cursor position in bits

  public:
    dfa U1* _Dat();
    dfa NT _LenbSet(SI lenb);

  public:
    dfa NT Clr();
    dfa NT Reserve(SI sizeb);
    dfa NT Reserve(SI sizebMin, SI sizebNew);

  public:
    dfa NT SetAt(CXGA buf, SI sizeb, SI posb);
    dfa NT GetAt(GA buf, SI sizeb, SI posb) cx;
    dfa NT AddLast(CXGA buf, SI sizeb);
    tpl1 dfa SI GetVarintAt(T1& val, SI posb) cx;
    dfa NT Read(GA buf, SI sizeb);
    tpl1 dfa SI ReadVarbint(T1& val, U1 base);
    tpl1 dfa SI ReadVarbtint(T1& val, U1 base, SI sizebMax);
    dfa NT PadToByte();

  public:
    dfa cx U1* Dat() cx;
    dfa SI SizeBit() cx;
    dfa SI SizeByte() cx;

  public:
    dfa BitVec();
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

tpl<typename T1, ValSeqBoxMode TMode> dfa NT ValSeqBoxEncode(BitVec& out, cx std::span<cx T1>& vals);
tpl<typename T1, ValSeqBoxMode TMode> dfa NT ValSeqBoxDecode(std::vector<T1>& vals, BitVec& in);

tpl<typename T1, SI TCap> class CircularBuf
{
  private:
    std::array<T1, TCap> m_buf;
    SI m_first;
    SI m_end;
    SI m_len;

  private:
    dfa SI _NextI(SI i) cx;
    dfa SI _PrevI(SI i) cx;

  public:
    dfa SI Len() cx;
    dfa BO IsEmpty() cx;
    dfa BO IsFull() cx;

  public:
    dfa T1& NewFirst();
    dfa T1& NewLast();
    dfa NT DelFirst();
    dfa NT DelLast();

  public:
    dfa T1& operator[](SI i);
    dfa cx T1& operator[](SI i) cx;

  public:
    dfa CircularBuf();
};
tpl1 class DCircularBuf
{
  private:
    std::vector<T1> m_buf;
    SI m_first;
    SI m_end;
    SI m_len;

  private:
    dfa SI _NextI(SI i) cx;
    dfa SI _PrevI(SI i) cx;

  public:
    dfa SI Len() cx;
    dfa BO IsEmpty() cx;
    dfa BO IsFull() cx;

  public:
    dfa T1& NewFirst();
    dfa T1& NewLast();
    dfa NT DelFirst();
    dfa NT DelLast();

  public:
    dfa NT Clr();
    dfa NT New(SI cap);
    dfa NT Del();

  public:
    dfa T1& operator[](SI i);
    dfa cx T1& operator[](SI i) cx;

  public:
    dfa DCircularBuf();
};

tpl1 Rect2<T1> Rect2FitRatio(cx Rect2<T1>& rect, F4 ratio);
tpl1 Rect2<T1> Rect2Center(cx Rect2<T1>& rect, cx Rect2<T1>& rectOuter);
tpl1 Rect2<T1> Rect2FitRatioCenter(cx Rect2<T1>& rect, F4 ratio);

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

    dfa U1* Dat() noex;
    dfa cx U1* Dat() cx noex;
    dfa NT Clr() noex;
    dfa U1& operator[](SI i) noex;
    dfa cx U1& operator[](SI i) cx noex;
    dfa BO operator==(cx DatBlock& other) cx noex;
    dfa BO operator!=(cx DatBlock& other) cx noex;

    dfa NT operator+(S4) cx = delete;
    dfa NT operator-(S4) cx = delete;
};

tpl<SI TSize, SI TAlign = alignof(U8)> struct alignas(TAlign) DatBlockSecure : DatBlock<TSize, TAlign>
{
    dfa NT Clr() noex;
    dfa ~DatBlockSecure() noex;
};

// a varint which is serialized using zigzag encoding
struct ZzVarint
{
    S8 val;

    dfa ZzVarint(S8 val = 0) noex;
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

        dfa Ref(GA ptrRaw = NUL, IdT id = IdT()) noex;
    };

  private:
    mutable ThdLockFast m_lock;
    ArrSFree<T1> m_bufStable;
    std::unordered_map<IdT, T1*> m_mapId;
    std::unordered_map<IdSecT, T1*> m_mapIdSec;

  private:
    dfa NT _IdGenUnique(IdT& id) cx;
    dfa NT _ClrDel(BO doSkipDel);

  public:
    dfa SI Cnt() cx;
    dfa NT List(std::vector<Ref>& refList) cx;
    dfa NT Del();
    dfa NT New(SI cap);
    dfa NT Clr();
    dfa NT ElemDel(cx Ref& ref);
    tpl<typename... TArgs> dfa ER ElemNew(Ref& ref, TArgs&&... args);
    dfa ER IdSecSet(T1* ptr, cx IdSecT& idSec);
    dfa NT Let(T1* ptr) cx;
    dfa T1* Get(cx Ref& ref) cx;
    dfa T1* PeekUns(cx Ref& ref) cx;
};

tpl1 class MthdObjListAu
{
  private:
    T1*& m_ptr;
    MthdObjList<T1>& m_objList;

  public:
    dfa MthdObjListAu(T1*& ptr, MthdObjList<T1>& objList, cx MthdObjList<T1>::Ref& ref);
    dfa ~MthdObjListAu();
};

#include "container.ipp"
