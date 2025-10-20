#include "container.hpp"

#include "memory.hpp"
#include "math.hpp"

dfa U1* BitVec::_Dat()
{
    ret m_dat.data();
}
dfa NT BitVec::_LenbSet(SI lenb)
{
    cx AU delta = lenb - m_lenb;
    m_lenb = lenb;
    m_freeb -= delta;
}
dfa NT BitVec::Clr()
{
    MemSet(m_dat.data(), 0, BitToByteSize(m_lenb));
    m_lenb = 0;
    m_freeb = m_dat.size() * BIT_IN_BYTE;
    m_curb = 0;
}
dfa NT BitVec::Reserve(SI sizeb)
{
    ifl (sizeb <= m_lenb + m_freeb)
        ret;
    m_dat.resize(BitToByteSize(sizeb), 0);
    m_freeb = sizeb - m_lenb;
}
dfa NT BitVec::Reserve(SI sizebMin, SI sizebNew)
{
    ifl (sizebMin <= m_lenb + m_freeb)
        ret;
    tx->Reserve((sizebNew == -1) ? SI(F8(sizebMin) * 1.5) : sizebNew);
}
dfa NT BitVec::SetAt(CXGA buf, SI sizeb, SI posb)
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
dfa NT BitVec::GetAt(GA buf, SI sizeb, SI posb) cx
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
dfa NT BitVec::AddLast(CXGA buf, SI sizeb)
{
    tx->SetAt(buf, sizeb, m_lenb);
}
dfa NT BitVec::Read(GA buf, SI sizeb)
{
    tx->GetAt(buf, sizeb, m_curb);
    m_curb += sizeb;
}
dfa NT BitVec::PadToByte()
{
    cx AU zero = U1(0);
    tx->SetAt(&zero, (BIT_IN_BYTE - (m_lenb % BIT_IN_BYTE)) % BIT_IN_BYTE, m_lenb);
}
dfa cx U1* BitVec::Dat() cx
{
    ret m_dat.data();
}
dfa SI BitVec::SizeBit() cx
{
    ret m_lenb;
}
dfa SI BitVec::SizeByte() cx
{
    ret BitToByteSize(m_lenb);
}
dfa BitVec::BitVec() : m_lenb(0), m_freeb(0), m_curb(0)
{
}

dfa ZzVarint::ZzVarint(S8 val) noex : val(val)
{
}
