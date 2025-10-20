#pragma once

#include "file.hpp"
#include "vector.hpp"
#include "container.hpp"
#include "thread.hpp"
#include "pixelpath.hpp"

/// TODO: "bada.h is in a PoC stage!" ///

/*
   The BADA codec is used to simply and efficiently encode/decode videos only containing shades of black and white.
   It's also a perfect fit for storing animations and even character sets on single-color displays.

   Inspired by the music video "Bad Apple!!".
   A size comparison between different codecs on this video:
   [#4] 161.611.776 bytes - <lossless> Raw bit format
   [#3]   6.072.392 bytes - < lossy  > HEVC/H.265 grayscale (2 color), standard compression
   [#2]   3.156.114 bytes - < lossy  > HEVC/H.265 grayscale (2 color), very strong compression
   [#1]   3.060.870 bytes - <lossless> BADA format

   Although not enforced, it is suggested to use ".bada" as the file extension.
*/

namespace Bada
{

using SubCodecTypeT = U1;
enum class SubCodecType : SubCodecTypeT
{
    NONE = 0,
    RAW_BIT,
    CPY_FRAME,
    BI_SWITCH,
    EDGE_FILL,
    DIFF_PATH,
};

using SubCodecExTypeT = U1;
enum class SubCodecExType : SubCodecExTypeT
{
    NONE = 0,
    RAW_BIT,
    CPY_FRAME,
    BI_SWITCH_X_BLACK,
    BI_SWITCH_X_WHITE,
    BI_SWITCH_Y_BLACK,
    BI_SWITCH_Y_WHITE,
    EDGE_FILL_BLACK,
    EDGE_FILL_WHITE,
    DIFF_PATH_BLACK,
    DIFF_PATH_WHITE,
    CNT,
};

struct Meta
{
    Size2<SI> frameSize;
    SI frameCnt;
    SI fps;

    dfa NT Clr();

    dfa Meta();
};

struct Hdr
{
    using MagicT = U4;

    static cxex MagicT MAGIC = 0x2520DABA; // 'BADA2025' in hex

    MagicT magic; // must be set before writing a valid header
    Meta meta;

    dfa NT Write(MemFile& file) cx;
    dfa ER Read(MemFile& file);

    dfa NT Clr();

    dfa Hdr();
};

struct SubCodecCpyFrame
{
};
struct SubCodecBiSwitch
{
    SI cntCur;
    U1 colCurIsWhite;

    dfa NT Clr();

    dfa SubCodecBiSwitch();
};
struct SubCodecEdgeFill
{
    struct Frame
    {
        BO isBaseWhite;
        BitVec pixelPath;
        std::vector<SI> fillOrigins;

        dfa Frame();
    };

    dfa NT Clr();
};
struct SubCodecDiffPath
{
};

class Xcoder
{
  protected:
    MemFile m_file;
    Hdr m_hdr;
};

#ifdef PROG_SYS_WIN

struct EncoderSubCodecCpyFrame : public SubCodecCpyFrame
{
    struct CompactFrame
    {
        std::vector<U1> bits;
        FNV1A64 hash;
        SI filePos;

        dfa NT Clr();

        dfa CompactFrame();
    };

    DCircularBuf<CompactFrame> recentFrames;

    dfa NT Clr();

    dfa EncoderSubCodecCpyFrame();
};
struct EncoderSubCodecBiSwitch : public SubCodecBiSwitch
{
    dfa NT _FrameColDatGenByX_BiSwitch(BitVec& bitVec, cx ColGrid<ColV>& colGrid, U1 colCurIsWhiteInit);
    dfa NT _FrameColDatGenByY_BiSwitch(BitVec& bitVec, cx ColGrid<ColV>& colGrid, U1 colCurIsWhiteInit);
};
struct EncoderSubCodecEdgeFill : public SubCodecEdgeFill
{
    dfa ER _FrameEncode_EdgeFill(Frame& frame, ColGrid<ColV> colGrid, cx ColV& colBase, ThdTaskMgr& thdTaskMgr);
};
struct EncoderSubCodecDiffPath : public SubCodecDiffPath
{
};
class Encoder : public Xcoder
{
  private:
    EncoderSubCodecCpyFrame m_subCodecCpyFrame;
    EncoderSubCodecBiSwitch m_subCodecBiSwitch;
    EncoderSubCodecEdgeFill m_subCodecEdgeFill;
    EncoderSubCodecDiffPath m_subCodecDiffPath;

  private:
    ThdTaskMgr m_thdTaskMgr;

  public:
    struct Params
    {
        SI fps;
        SI recentFrameCnt;

        dfa Params();
    };

  public:
    dfa ColV FrameBaseCol(cx ColGrid<ColV>& colGrid);

  public:
    dfa ER Begin(cx CH* path, cx Params& params);
    dfa ER FrameWrite(cx ColGrid<ColV>& colGrid_);
    dfa ER End();

  public:
    dfa Encoder();
};

#endif

struct DecoderSubCodecCpyFrame : public SubCodecCpyFrame
{
};
struct DecoderSubCodecBiSwitch : public SubCodecBiSwitch
{
    dfa NT _CntListToColGrid_ByX_BiSwitch(ColGrid<ColV>& colGrid, cx std::vector<SI>& cntList, BO colCurIsWhiteInit);
    dfa NT _CntListToColGrid_ByY_BiSwitch(ColGrid<ColV>& colGrid, cx std::vector<SI>& cntList, BO colCurIsWhiteInit);
};
struct DecoderSubCodecEdgeFill : public SubCodecEdgeFill
{
    dfa NT _FrameToColGrid_EdgeFill(ColGrid<ColV>& colGrid, cx Frame& frame, cx Size2<SI>& frameSize);
};
struct DecoderSubCodecDiffPath : public SubCodecDiffPath
{
};
class Decoder : public Xcoder
{
  private:
    DecoderSubCodecCpyFrame m_subCodecCpyFrame;
    DecoderSubCodecBiSwitch m_subCodecBiSwitch;
    DecoderSubCodecEdgeFill m_subCodecEdgeFill;
    DecoderSubCodecDiffPath m_subCodecDiffPath;

  private:
    SI m_frameCur;

  public:
    dfa ER Begin(cx CH* path);
    dfa ER FrameRead(ColGrid<ColV>& colGrid);
    dfa ER End();

  public:
    dfa BO HasNextFrame() cx;

  public:
    dfa Size2<SI> FrameSizeGet() cx;
    dfa SI FpsGet() cx;

  public:
    dfa Decoder();
};

} // namespace Bada

#include "bada.ipp"
