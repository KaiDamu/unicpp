#pragma once

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

    dfa NT Clr()
    {
        frameSize.Clr();
        frameCnt = 0;
        fps = 0;
    }

    dfa Meta()
    {
        tx->Clr();
    }
};

struct Hdr
{
    using MagicT = U4;

    static cxex MagicT MAGIC = 0x2520DABA; // 'BADA2025' in hex

    MagicT magic; // must be set before writing a valid header
    Meta meta;

    dfa NT Write(MemFile& file) cx
    {
        file.WriteVal(magic);
        file.WriteVal(U2(meta.frameSize.w));
        file.WriteVal(U2(meta.frameSize.h));
        file.WriteVal(U4(meta.frameCnt));
        file.WriteVal(U1(meta.fps));
    }
    dfa ER Read(MemFile& file)
    {
        ifep(file.ReadVal<TO(magic)>(magic));
        ifu (magic != MAGIC)
            rete(ErrVal::NO_VALID);
        ifep(file.ReadVal<U2>(meta.frameSize.w));
        ifep(file.ReadVal<U2>(meta.frameSize.h));
        ifep(file.ReadVal<U4>(meta.frameCnt));
        ifep(file.ReadVal<U1>(meta.fps));
        rets;
    }

    dfa NT Clr()
    {
        magic = 0;
        meta.Clr();
    }

    dfa Hdr()
    {
        tx->Clr();
    }
};

struct SubCodecCpyFrame
{
};
struct SubCodecBiSwitch
{
    SI cntCur;
    U1 colCurIsWhite;

    dfa NT Clr()
    {
        cntCur = 0;
        colCurIsWhite = 0;
    }

    dfa SubCodecBiSwitch()
    {
        tx->Clr();
    }
};
struct SubCodecEdgeFill
{
    struct Frame
    {
        BO isBaseWhite;
        BitVec pixelPath;
        std::vector<SI> fillOrigins;

        dfa Frame() : isBaseWhite(NO)
        {
        }
    };

    dfa NT Clr()
    {
    }
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

        dfa NT Clr()
        {
            bits.clear();
            hash = FNV1A64();
            filePos = -1;
        }

        dfa CompactFrame()
        {
            tx->Clr();
        }
    };

    DCircularBuf<CompactFrame> recentFrames;

    dfa NT Clr()
    {
        recentFrames.Clr();
    }

    dfa EncoderSubCodecCpyFrame()
    {
        tx->Clr();
    }
};
struct EncoderSubCodecBiSwitch : public SubCodecBiSwitch
{
    dfa NT _FrameColDatGenByX_BiSwitch(BitVec& bitVec, cx ColGrid<ColV>& colGrid, U1 colCurIsWhiteInit)
    {
        // TODO: merge with _FrameColDatGenByY_BiSwitch

        cntCur = 0;
        colCurIsWhite = colCurIsWhiteInit;

        std::vector<SI> cntList;

        AU pixel = colGrid.pixels.data();
        cx AU pixelEnd = pixel + colGrid.pixels.size();
        while (pixel != pixelEnd)
        {
            cx AU isWhite = pixel->v >= TO(ColV::v)(0x80);
            ifu (U1(isWhite) ^ colCurIsWhite)
            {
                cntList.emplace_back(cntCur);
                colCurIsWhite = isWhite;
                cntCur = 1;
            }
            else
            {
                ++cntCur;
            }
            ++pixel;
        }

        if (cntCur > 0)
            cntList.emplace_back(cntCur);

        ValSeqBoxEncode<SI, ValSeqBoxMode::DEFA>(bitVec, cntList);
    }
    dfa NT _FrameColDatGenByY_BiSwitch(BitVec& bitVec, cx ColGrid<ColV>& colGrid, U1 colCurIsWhiteInit)
    {
        // TODO: merge with _FrameColDatGenByX_BiSwitch

        cntCur = 0;
        colCurIsWhite = colCurIsWhiteInit;

        std::vector<SI> cntList;

        ite (x, x < colGrid.size.w)
            ite (y, y < colGrid.size.h)
            {
                cx AU& pixel = colGrid.Pixel(Pos2<SI>(x, y));
                cx AU isWhite = pixel.v >= TO(ColV::v)(0x80);
                ifu (U1(isWhite) ^ colCurIsWhite)
                {
                    cntList.emplace_back(cntCur);
                    colCurIsWhite = isWhite;
                    cntCur = 1;
                }
                else
                {
                    ++cntCur;
                }
            }

        if (cntCur > 0)
            cntList.emplace_back(cntCur);

        ValSeqBoxEncode<SI, ValSeqBoxMode::DEFA>(bitVec, cntList);
    }
};
struct EncoderSubCodecEdgeFill : public SubCodecEdgeFill
{
    dfa ER _FrameEncode_EdgeFill(Frame& frame, ColGrid<ColV> colGrid, cx ColV& colBase, ThdTaskMgr& thdTaskMgr)
    {
        cx AU colObj = ((colBase == ColV(0x00)) ? ColV(0xFF) : ColV(0x00));
        cx AU colMarkEdge = ColV(0x10);
        cx AU colMarkPath = ColV(0x20);
        cx AU colMarkFill = ColV(0x30);
        ColGridEdgeMarkAll(colGrid, colObj, colMarkEdge);

        std::vector<SI> fillOrigins;
        std::vector<SI> fillCnts;
        ColGridFloodFillAll(colGrid, fillOrigins, fillCnts, colObj, colMarkFill);

        cxex SI FILL_SUBSTITUTE_SIZE_MAX = 4;
        std::vector<SI> fillOriginsFiltered;
        fillOriginsFiltered.reserve(fillOrigins.size());
        ite (i, i < SI(fillOrigins.size()))
        {
            if (fillCnts[i] > FILL_SUBSTITUTE_SIZE_MAX)
                fillOriginsFiltered.emplace_back(fillOrigins[i]);
            else
                ColGridFloodFillAt(colGrid, fillOrigins[i], colMarkFill, colMarkEdge);
        }

        BitVec pixelPath[SI(PixelPath::BuildStrat::CNT)];
        SI stratSizes[SI(PixelPath::BuildStrat::CNT)];
        {
            std::vector<ThdTask> thdTaskList;
            thdTaskList.resize(SI(PixelPath::BuildStrat::CNT));
            ite (i, i < SI(PixelPath::BuildStrat::CNT))
            {
                thdTaskList[i] = ThdTask([=, &colGrid, &pixelPath, &colMarkEdge, &colMarkPath, &stratSizes]() {
                    ColGrid<ColV> colGridCopy = colGrid;
                    PixelPath::Build(colGridCopy, pixelPath[i], colMarkEdge, colMarkPath, PixelPath::BuildStrat(i));
                    stratSizes[i] = pixelPath[i].SizeByte();
                });
            }
            thdTaskMgr.TaskAdd(thdTaskList);
        }
        thdTaskMgr.WaitAll();

        AU stratMinI = SI(-1);
        AU stratMinSize = SI(1e9);
        ite (i, i < SI(PixelPath::BuildStrat::CNT))
        {
            if (stratSizes[i] < stratMinSize)
            {
                stratMinI = i;
                stratMinSize = stratSizes[i];
            }
            else if (stratSizes[i] == stratMinSize)
            {
                ;
            }
        }
        cx AU& pixelPathBest = pixelPath[stratMinI];

        // add data to frame
        frame.isBaseWhite = (colBase != ColV(0x00));
        frame.pixelPath = std::move(pixelPathBest);
        frame.fillOrigins = std::move(fillOriginsFiltered);

        rets;
    }
};
class Encoder : public Xcoder
{
  private:
    EncoderSubCodecCpyFrame m_subCodecCpyFrame;
    EncoderSubCodecBiSwitch m_subCodecBiSwitch;
    EncoderSubCodecEdgeFill m_subCodecEdgeFill;

  private:
    ThdTaskMgr m_thdTaskMgr;

  public:
    struct Params
    {
        SI fps;
        SI recentFrameCnt;

        dfa Params() : fps(30), recentFrameCnt(32)
        {
        }
    };

  public:
    dfa ColV FrameBaseCol(cx ColGrid<ColV>& colGrid)
    {
        SI blackCnt = 0;
        ite (x, x < colGrid.size.w)
            blackCnt += colGrid.pixels[x].v < TO(ColV::v)(0x80);
        for (SI y = 1; y < colGrid.size.h - 1; ++y)
        {
            blackCnt += colGrid.pixels[y * colGrid.size.w].v < TO(ColV::v)(0x80);
            blackCnt += colGrid.pixels[y * colGrid.size.w + colGrid.size.w - 1].v < TO(ColV::v)(0x80);
        }
        ite (x, x < colGrid.size.w)
            blackCnt += colGrid.pixels[(colGrid.size.h - 1) * colGrid.size.w + x].v < TO(ColV::v)(0x80);
        cx SI allCnt = (colGrid.size.w + colGrid.size.h) * 2 - 4;
        cx AU blackRatio = F4(blackCnt) / F4(allCnt);
        ret ((blackRatio >= F4(0.5)) ? ColV(0x00) : ColV(0xFF));
    }

  public:
    dfa ER Begin(cx CH* path, cx Params& params)
    {
        ifep(m_thdTaskMgr.Init());
        ifep(m_file.Open(path, NO, YES));
        m_hdr.Clr();
        m_hdr.Write(m_file);
        m_hdr.magic = Hdr::MAGIC;
        m_hdr.meta.fps = params.fps;

        m_subCodecCpyFrame.recentFrames.New(params.recentFrameCnt);
        m_subCodecEdgeFill.Clr();

        rets;
    }
    dfa ER FrameWrite(cx ColGrid<ColV>& colGrid_)
    {
        ColGrid<ColV> colGrid = colGrid_;

        ifu (m_hdr.meta.frameCnt == 0)
            m_hdr.meta.frameSize = colGrid.size;

        ifu (colGrid.size != m_hdr.meta.frameSize)
            rete(ErrVal::NO_VALID);

        SI subCodecExBestI = SI(SubCodecExType::NONE);
        std::array<MemFile, SI(SubCodecExType::CNT)> subCodecExDatList;
        ite (i, i < SI(subCodecExDatList.size()))
        {
            AU& subCodecExDat = subCodecExDatList[i];

            switch (SubCodecExType(i))
            {
            case SubCodecExType::CPY_FRAME: {
                AU& compactFrame = m_subCodecCpyFrame.recentFrames.NewFirst();
                cx AU compactSize = BitToByteSize(colGrid.pixels.size());
                compactFrame.bits.resize(compactSize);
                MemSet(compactFrame.bits.data(), 0, compactFrame.bits.size());
                ite (i, i < colGrid.pixels.size())
                {
                    cx AU bit = (colGrid.pixels[i].v != 0);
                    compactFrame.bits[i / BIT_IN_BYTE] |= U1(bit << (i % BIT_IN_BYTE));
                }
                compactFrame.hash = HashFnv1a64(compactFrame.bits.data(), compactFrame.bits.size());
                compactFrame.filePos = m_file.CurGet();

                for (SI i = 1; i < m_subCodecCpyFrame.recentFrames.Len(); ++i)
                {
                    cx AU& recentFrame = m_subCodecCpyFrame.recentFrames[i];
                    if (recentFrame.hash == compactFrame.hash) // NOTE: we are just assuming no hash collision
                    {
                        // ConWriteInfo("Frame copy found at %d, file pos %d", m_hdr.meta.frameCnt + 1, recentFrame.filePos);

                        ifep(subCodecExDat.Open(NUL, NO, YES));
                        subCodecExDat.WriteVarint(AsTypeU(recentFrame.filePos));

                        m_subCodecCpyFrame.recentFrames.DelFirst();
                        break;
                    }
                }
            }
            break;
            case SubCodecExType::BI_SWITCH_X_BLACK:
            case SubCodecExType::BI_SWITCH_X_WHITE:
            case SubCodecExType::BI_SWITCH_Y_BLACK:
            case SubCodecExType::BI_SWITCH_Y_WHITE: {
                cx BO isByY = (SubCodecExType(i) == SubCodecExType::BI_SWITCH_Y_BLACK || SubCodecExType(i) == SubCodecExType::BI_SWITCH_Y_WHITE);
                cx U1 colCurIsWhiteInit = (SubCodecExType(i) == SubCodecExType::BI_SWITCH_X_WHITE || SubCodecExType(i) == SubCodecExType::BI_SWITCH_Y_WHITE);

                cx AU isCorrectCol = (colGrid.pixels[0] != ColV(0x00)) == colCurIsWhiteInit;
                if (isCorrectCol)
                {
                    BitVec frameColDat;
                    if (isByY)
                        m_subCodecBiSwitch._FrameColDatGenByY_BiSwitch(frameColDat, colGrid, colCurIsWhiteInit);
                    else
                        m_subCodecBiSwitch._FrameColDatGenByX_BiSwitch(frameColDat, colGrid, colCurIsWhiteInit);

                    ifep(subCodecExDat.Open(NUL, NO, YES));
                    subCodecExDat.Write(frameColDat.Dat(), frameColDat.SizeByte());
                }
            }
            break;
            case SubCodecExType::EDGE_FILL_BLACK:
            case SubCodecExType::EDGE_FILL_WHITE: {
                cx AU colBase = (SubCodecExType(i) == SubCodecExType::EDGE_FILL_BLACK) ? ColV(0x00) : ColV(0xFF);

                SubCodecEdgeFill::Frame frame;
                ifep(m_subCodecEdgeFill._FrameEncode_EdgeFill(frame, colGrid, colBase, m_thdTaskMgr));

                cx AU& frameCur = frame;

                // simplify fill origins
                BitVec fillOriginBox;
                ValSeqBoxEncode<SI, ValSeqBoxMode::DIFF_ASC>(fillOriginBox, frameCur.fillOrigins);

                // write serialized data of the current frame
                ifep(subCodecExDat.Open(NUL, NO, YES));
                subCodecExDat.Write(fillOriginBox.Dat(), fillOriginBox.SizeByte());
                subCodecExDat.WriteVarint(AsTypeU(frameCur.pixelPath.SizeByte()));
                subCodecExDat.Write(frameCur.pixelPath.Dat(), frameCur.pixelPath.SizeByte());
            }
            break;
            default:
                break;
            }

            if (subCodecExDat.IsOpen())
            {
                if (subCodecExBestI == SI(SubCodecExType::NONE))
                {
                    subCodecExBestI = i;
                }
                else
                {
                    if (subCodecExDat.Size() < subCodecExDatList[subCodecExBestI].Size())
                        subCodecExBestI = i;
                }
            }
        }

        ifu (subCodecExBestI == SI(SubCodecExType::NONE))
            rete(ErrVal::NO_EXIST);

        m_file.WriteVal(SubCodecExTypeT(subCodecExBestI));
        m_file.Write(subCodecExDatList[subCodecExBestI].Dat(), subCodecExDatList[subCodecExBestI].Size());

        ++m_hdr.meta.frameCnt;

        rets;
    }
    dfa ER End()
    {
        m_file.CurSet(0);
        m_hdr.Write(m_file);
        ifep(m_file.Close(YES));

        ifep(m_thdTaskMgr.Free());

        m_subCodecCpyFrame.recentFrames.Del();

        rets;
    }

  public:
    dfa Encoder()
    {
    }
};

#endif

struct DecoderSubCodecCpyFrame : public SubCodecCpyFrame
{
};
struct DecoderSubCodecBiSwitch : public SubCodecBiSwitch
{
    dfa NT _CntListToColGrid_ByX_BiSwitch(ColGrid<ColV>& colGrid, cx std::vector<SI>& cntList, BO colCurIsWhiteInit)
    {
        // TODO: merge with _CntListToColGrid_ByY_BiSwitch

        SI cntListI = 0;

        cntCur = 0;
        colCurIsWhite = colCurIsWhiteInit ? 0 : 1;

        cx AU pixelCnt = colGrid.size.Area();
        SI pixelI = 0;
        while (pixelI < pixelCnt)
        {
            if (cntCur == 0)
            {
                // ifu (m_file.CurGet() >= m_colDatPos + m_colDatSize)
                // rete(ErrVal::NO_EXIST);
                cntCur = cntList[cntListI];
                ++cntListI;
                colCurIsWhite = colCurIsWhite ? 0 : 1;
            }

            cx AU cntToRead = Min(cntCur, pixelCnt - pixelI);
            cx AU colSrc = colCurIsWhite ? ColV(0xFF) : ColV(0x00);
            AU pixelCur = colGrid.pixels.data() + pixelI;
            ite (i, i < cntToRead)
                *(pixelCur + i) = colSrc;
            pixelI += cntToRead;
            cntCur -= cntToRead;
        }
    }
    dfa NT _CntListToColGrid_ByY_BiSwitch(ColGrid<ColV>& colGrid, cx std::vector<SI>& cntList, BO colCurIsWhiteInit)
    {
        // TODO: merge with _CntListToColGrid_ByX_BiSwitch

        SI cntListI = 0;

        cntCur = 0;
        colCurIsWhite = colCurIsWhiteInit ? 0 : 1;

        ite (x, x < colGrid.size.w)
            ite (y, y < colGrid.size.h)
            {
                if (cntCur == 0)
                {
                    // ifu (m_file.CurGet() >= m_colDatPos + m_colDatSize)
                    // rete(ErrVal::NO_EXIST);
                    jdst(nextCol);
                    cntCur = cntList[cntListI];
                    ++cntListI;
                    colCurIsWhite ^= 1;
                    ifu (cntCur == 0) // this happens if the first pixel of the frame is not the default color
                        jsrc(nextCol);
                }
                colGrid.Pixel(Pos2<SI>(x, y)) = ColV(0 - colCurIsWhite);
                --cntCur;
            }
    }
};
struct DecoderSubCodecEdgeFill : public SubCodecEdgeFill
{
    dfa NT _FrameToColGrid_EdgeFill(ColGrid<ColV>& colGrid, cx Frame& frame, cx Size2<SI>& frameSize)
    {
        // init
        colGrid.Resize(frameSize); // TODO: why is this needed here, but not in BI_SWITCH?

        // base
        cx AU colBase = frame.isBaseWhite ? ColV(0xFF) : ColV(0x00);
        MemSet(colGrid.pixels.data(), colBase.v, colGrid.pixels.size() * siz(colBase.v));

        // path
        cx AU colObj = frame.isBaseWhite ? ColV(0x00) : ColV(0xFF);
        PixelPath::Draw(colGrid, frame.pixelPath, colObj);

        // fill
        for (cx AU& fillOrigin : frame.fillOrigins)
            ColGridFloodFillAt(colGrid, fillOrigin, colBase, colObj);
    }
};
class Decoder : public Xcoder
{
  private:
    DecoderSubCodecCpyFrame m_subCodecCpyFrame;
    DecoderSubCodecBiSwitch m_subCodecBiSwitch;
    DecoderSubCodecEdgeFill m_subCodecEdgeFill;

  private:
    SI m_frameCur;

  public:
    dfa ER Begin(cx CH* path)
    {
        ifep(m_file.Open(path, YES));
        m_hdr.Clr();
        ifep(m_hdr.Read(m_file));

        m_subCodecEdgeFill.Clr();

        m_frameCur = 0;

        rets;
    }
    dfa ER FrameRead(ColGrid<ColV>& colGrid)
    {
        colGrid.Resize(m_hdr.meta.frameSize);
        ++m_frameCur;

        SI filePosRestore = -1;

        jdst(cpyFrameRedir);

        SubCodecExType subCodecEx;
        ifep(m_file.ReadVal<U1>(subCodecEx));

        switch (subCodecEx)
        {
        case SubCodecExType::CPY_FRAME: {
            SI filePosFrame;
            ifu (m_file.ReadVarint(filePosFrame) < 1)
                rete(ErrVal::FILE);
            filePosRestore = m_file.CurGet();
            ifu (filePosFrame < 0 || filePosFrame >= filePosRestore)
                rete(ErrVal::NO_EXIST);
            m_file.CurSet(filePosFrame);
            jsrc(cpyFrameRedir);
        }
        break;
        case SubCodecExType::BI_SWITCH_X_BLACK:
        case SubCodecExType::BI_SWITCH_X_WHITE:
        case SubCodecExType::BI_SWITCH_Y_BLACK:
        case SubCodecExType::BI_SWITCH_Y_WHITE: {
            cx BO isByY = (subCodecEx == SubCodecExType::BI_SWITCH_Y_BLACK || subCodecEx == SubCodecExType::BI_SWITCH_Y_WHITE);
            cx U1 colCurIsWhiteInit = (subCodecEx == SubCodecExType::BI_SWITCH_X_WHITE || subCodecEx == SubCodecExType::BI_SWITCH_Y_WHITE);

            std::vector<SI> cntList;
            ifu ((m_file.ReadValSeqBox<SI, ValSeqBoxMode::DEFA>(cntList) < 1))
                rete(ErrVal::FILE);

            if (isByY)
                m_subCodecBiSwitch._CntListToColGrid_ByY_BiSwitch(colGrid, cntList, colCurIsWhiteInit);
            else
                m_subCodecBiSwitch._CntListToColGrid_ByX_BiSwitch(colGrid, cntList, colCurIsWhiteInit);
        }
        break;
        case SubCodecExType::EDGE_FILL_BLACK:
        case SubCodecExType::EDGE_FILL_WHITE: {
            cx AU colBase = (subCodecEx == SubCodecExType::EDGE_FILL_BLACK) ? ColV(0x00) : ColV(0xFF);

            SubCodecEdgeFill::Frame frame;

            // base color
            frame.isBaseWhite = (colBase != ColV(0x00));

            // fill origins
            ifu ((m_file.ReadValSeqBox<SI, ValSeqBoxMode::DIFF_ASC>(frame.fillOrigins) < 1))
                rete(ErrVal::FILE);

            // path
            SI pixelPathDatSize;
            ifu (m_file.ReadVarint(pixelPathDatSize) < 1)
                rete(ErrVal::FILE);
            std::vector<U1> pixelPathDat(pixelPathDatSize);
            ifu (m_file.Read<U1>(pixelPathDat) != SI(pixelPathDat.size()))
                rete(ErrVal::FILE);
            frame.pixelPath.AddLast(pixelPathDat.data(), pixelPathDat.size() * BIT_IN_BYTE);

            m_subCodecEdgeFill._FrameToColGrid_EdgeFill(colGrid, frame, m_hdr.meta.frameSize);
        }
        break;
        default:
            break;
        }

        if (filePosRestore != -1)
            m_file.CurSet(filePosRestore);

        rets;
    }
    dfa ER End()
    {
        ifep(m_file.Close());
        rets;
    }

  public:
    dfa BO HasNextFrame() cx
    {
        ret m_frameCur < m_hdr.meta.frameCnt;
    }

  public:
    dfa Size2<SI> FrameSizeGet() cx
    {
        ret m_hdr.meta.frameSize;
    }
    dfa SI FpsGet() cx
    {
        ret m_hdr.meta.fps;
    }

  public:
    dfa Decoder()
    {
        m_frameCur = 0;
    }
};

} // namespace Bada
