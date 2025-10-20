#pragma once

#include "color.hpp"

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

    dfa QoiHdr();
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

inl cxex U1 QOI_EOF[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

dfa SI _QoiColHash(cx ColRgba& col);

tpl1 dfa ER QoiEncode(std::vector<U1>& out, cx T1* in, cx Size2<SI>& inSize);
tpl1 dfa ER QoiDecode(std::vector<T1>& out, Size2<SI>& outSize, cx U1* in, SI inSize);

tpl1 dfa ER ColGridToFileQoi(cx ColGrid<T1>& colGrid, cx CH* path);
tpl1 dfa ER FileToColGridQoi(ColGrid<T1>& colGrid, cx CH* path);

#include "qoi.ipp"
