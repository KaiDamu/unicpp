#include "grtype.hpp"

namespace gr
{

tpl1 dfa NT MeshDat::VtxDatAdd(cx std::span<T1>& vtxDat)
{
    cx AU srcDat = reinterpret_cast<cx U1*>(vtxDat.data());
    cx AU srcSize = vtxDat.size_bytes();
    cx AU sizeCur = tx->vtxDat.size();
    cx AU sizeReq = sizeCur + srcSize;
    ifu (sizeReq > tx->vtxDat.capacity())
        tx->vtxDat.reserve(Max(sizeReq, tx->vtxDat.capacity() * 2));
    tx->vtxDat.resize(sizeReq);
    MemCpy(tx->vtxDat.data() + sizeCur, srcDat, srcSize);
}

} // namespace gr
