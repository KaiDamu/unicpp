#include "grtype.hpp"

namespace gr
{

dfa VtxLayout::Attrib::Attrib() : name(Name::NONE), compCnt(0), varType(VarType::NT), ofs(0), isNormalized(NO)
{
}

dfa VtxLayout::VtxLayout() : stride(0)
{
}

dfa MeshDat::Part::Part() : materialI(-1)
{
}

dfa NT MeshDat::VtxAttribAdd(SI compCnt, VarType varType, SI padAfter, BO isNormalized, VtxLayout::Attrib::Name name)
{
    AU& attrib = tx->vtxLayout.attribs.emplace_back();
    attrib.name = name;
    attrib.compCnt = compCnt;
    attrib.varType = varType;
    attrib.ofs = tx->vtxLayout.stride;
    attrib.isNormalized = isNormalized;
    tx->vtxLayout.stride += compCnt * VarTypeSize(varType) + padAfter;
}
dfa NT MeshDat::PartAdd(SI materialI)
{
    AU& part = tx->parts.emplace_back();
    part.materialI = materialI;
}
dfa NT MeshDat::IndicesAdd(cx std::span<IndiceT>& indices)
{
    AU& part = tx->parts.back();
    part.indices.insert(part.indices.end(), indices.begin(), indices.end());
}

} // namespace gr
