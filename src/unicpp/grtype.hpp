#pragma once

#include <vector>
#include <span>
#include "typeex.hpp"

namespace gr
{

struct VtxLayout
{
    struct Attrib
    {
        using NameT = U2;
        enum class Name : NameT
        {
            NONE = 0,
        };

        Name name;
        SI compCnt;
        VarType varType;
        SI ofs;
        BO isNormalized;

        dfa Attrib();
    };

    std::vector<Attrib> attribs;
    SI stride;

    dfa VtxLayout();
};

using IndiceT = SI4;
using MaterialIT = SI;

struct MeshDat
{
    struct Part
    {
        std::vector<IndiceT> indices;
        MaterialIT materialI;

        dfa Part();
    };

    std::vector<U1> vtxDat;
    std::vector<Part> parts;
    VtxLayout vtxLayout;

    tpl1 dfa NT VtxDatAdd(cx std::span<T1>& vtxDat);
    dfa NT VtxAttribAdd(SI compCnt, VarType varType, SI padAfter = 0, BO isNormalized = YES, VtxLayout::Attrib::Name name = VtxLayout::Attrib::Name::NONE);
    dfa NT PartAdd(SI materialI);
    dfa NT IndicesAdd(cx std::span<IndiceT>& indices);
};

} // namespace gr

#include "grtype.ipp"
