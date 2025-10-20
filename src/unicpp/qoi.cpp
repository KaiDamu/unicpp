#include "qoi.hpp"

dfa QoiHdr::QoiHdr() : size(0, 0), chCnt(0), colSpace(QoiColSpace::SRGB)
{
}

dfa SI _QoiColHash(cx ColRgba& col)
{
    ret (col.r * 3 + col.g * 5 + col.b * 7 + col.a * 11) & 0x3F;
}
