#pragma once

/// TODO: "bada.h is in a PoC stage!" ///

/*
   The following information is describing the format of the BADA codec and file format.

   The BADA codec is used to simply and efficiently encode and decode videos only containing shades of black and white.
   Inspired by the music video "Bad Apple!!".

   Although not enforced, it is suggested to use ".bada" as the file extension.

   (see file structure below)

   The 'magic' field must be "BADA".
   The last element of 'datBlocks' must have 'type' set to 'DatBlockType.END'.

   [File structure]
       U4 magic = "BADA"
       DatBlock[] datBlocks

   [File structure structures]
       DatBlock
           U1 type (DatBlockType)
           U1[] dat

   [File structure structure types]
       DatBlockType.NONE
           U1[0] none
       DatBlockType.GRID_SIZE
           U4V w
           U4V h
           U4 cnt
       DatBlockType.FPS
           U4V cnt
       DatBlockType.COL_DAT_TYPE
           U1 type (SubCodecType)
           U4V features (ColDatFeatures)
           U1[] featuresDat
       DatBlockType.COL_DAT
           U4 size
           U1[size] dat
       DatBlockType.END
           U1[0] none

   [Color data types]
       SubCodecType.NONE
       SubCodecType.BI_SWITCH
       SubCodecType.EDGE_FILL
*/

namespace Bada
{

using SubCodecTypeT = U1;
enum class SubCodecType : SubCodecTypeT
{
    NONE = 0,
    BI_SWITCH = 1,
    EDGE_FILL = 2,
};

using ColDatFeaturesT = U4;
enum class ColDatFeatures : ColDatFeaturesT
{
    RESERVED = 0b0,
};

using DatBlockTypeT = U1;
enum class DatBlockType : DatBlockTypeT
{
    NONE = 0,
    GRID_SIZE = 1,
    FPS = 2,
    COL_DAT_TYPE = 3,
    COL_DAT = 4,
    END = 5,
};

using MagicT = U4;
cxex MagicT MAGIC = 0x41444142; // "BADA"

struct Meta
{
    Size2<SI> frameSize;
    SI fps;

    dfa Meta() : frameSize(0, 0), fps(0)
    {
    }
};

struct SubCodecBiSwitch
{
    SI cntCur;
    U1 colCur;

    dfa SubCodecBiSwitch() : cntCur(0), colCur(0)
    {
    }
};
struct SubCodecEdgeFill
{
    struct Frame
    {
        BO isLazy;
        BO isBaseWhite;
        BitVec pixelPath;
        std::vector<SI> fillOrigins;

        dfa Frame() : isLazy(NO), isBaseWhite(NO)
        {
        }
    };

    std::vector<Frame> frames;
};

class Xcoder
{
  protected:
    MemFile m_file;
    SI m_frameCnt;
    SubCodecType m_subCodecType;
    ColDatFeatures m_colDatFeatures;
    Meta m_meta;

  protected:
    dfa Xcoder()
    {
        m_frameCnt = 0;
        m_subCodecType = SubCodecType::NONE;
        m_colDatFeatures = ColDatFeatures(0);
    }
};

#ifdef PROG_SYS_WIN

struct EncoderSubCodecBiSwitch : public SubCodecBiSwitch
{
    dfa NT _FrameColDatGenByX_BiSwitch(BitVec& bitVec, cx ColGrid<ColV>& colGrid)
    {
        // TODO: merge with _FrameColDatGenByY_BiSwitch

        cntCur = 0;
        colCur = 0;

        std::vector<SI> cntList;

        AU pixel = colGrid.pixels.data();
        cx AU pixelEnd = pixel + colGrid.pixels.size();
        while (pixel != pixelEnd)
        {
            cx AU isWhite = pixel->v >= TO(ColV::v)(0x80);
            ifu (U1(isWhite) ^ colCur)
            {
                cntList.emplace_back(cntCur);
                colCur = isWhite;
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

        ValSeqBoxEncode(bitVec, cntList.data(), cntList.size());
    }
    dfa NT _FrameColDatGenByY_BiSwitch(BitVec& bitVec, cx ColGrid<ColV>& colGrid)
    {
        // TODO: merge with _FrameColDatGenByX_BiSwitch

        cntCur = 0;
        colCur = 0;

        std::vector<SI> cntList;

        ite (x, x < colGrid.size.w)
            ite (y, y < colGrid.size.h)
            {
                cx AU& pixel = colGrid.Pixel(Pos2<SI>(x, y));
                cx AU isWhite = pixel.v >= TO(ColV::v)(0x80);
                ifu (U1(isWhite) ^ colCur)
                {
                    cntList.emplace_back(cntCur);
                    colCur = isWhite;
                    cntCur = 1;
                }
                else
                {
                    ++cntCur;
                }
            }

        if (cntCur > 0)
            cntList.emplace_back(cntCur);

        ValSeqBoxEncode(bitVec, cntList.data(), cntList.size());
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
        AU isTie = NO;
        ite (i, i < SI(PixelPath::BuildStrat::CNT))
        {
            if (stratSizes[i] < stratMinSize)
            {
                stratMinI = i;
                stratMinSize = stratSizes[i];
                isTie = NO;
            }
            else if (stratSizes[i] == stratMinSize)
            {
                isTie = YES;
            }
        }
        // if (!isTie)
        // g_stratWins[stratMinI] += 1;
        // ite (i, i < SI(PixelPath::BuildStrat::CNT))
        // ConWriteRaw("%d ", g_stratWins[i]);
        // ConWriteRaw("\n");
        unused(isTie);
        cx AU& pixelPathBest = pixelPath[stratMinI];

        // add data to frame
        frame.isLazy = NO;
        frame.isBaseWhite = (colBase != ColV(0x00));
        frame.pixelPath = std::move(pixelPathBest);
        frame.fillOrigins = std::move(fillOriginsFiltered);

        rets;
    }
};
class Encoder : public Xcoder
{
  private:
    EncoderSubCodecBiSwitch m_subCodecBiSwitch;
    EncoderSubCodecEdgeFill m_subCodecEdgeFill;

  private:
    ColGrid<ColV> m_lazyRef;
    SI m_lazyRefFrameI;
    ThdTaskMgr m_thdTaskMgr;
    SI m_colDatSizePos;

  public:
    struct Params
    {
        SubCodecType colDatType;
        ColDatFeatures colDatFeatures;
        SI fps;

        dfa Params() : colDatType(SubCodecType::BI_SWITCH), colDatFeatures(ColDatFeatures(0)), fps(30)
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
        m_subCodecType = params.colDatType;
        m_colDatFeatures = params.colDatFeatures;
        m_meta.fps = params.fps;
        ifep(m_file.Open(path, NO, YES));
        m_file.WriteVal(MAGIC);
        m_file.WriteVal(DatBlockType::FPS);
        m_file.WriteVarint(U4(m_meta.fps));
        m_file.WriteVal(DatBlockType::COL_DAT_TYPE);
        m_file.WriteVal(m_subCodecType);
        m_file.WriteVarint(U4(m_colDatFeatures));
        m_meta.frameSize = Size2<SI>(0, 0);
        m_frameCnt = 0;
        m_subCodecEdgeFill.frames.clear();
        rets;
    }
    dfa ER FrameWrite(cx ColGrid<ColV>& colGrid_)
    {
        ColGrid<ColV> colGrid = colGrid_;

        ifu (m_frameCnt == 0)
        {
            m_meta.frameSize = colGrid.size;
            m_file.WriteVal(DatBlockType::COL_DAT);
            m_colDatSizePos = m_file.CurGet();
            m_file.WriteVal(U4(0)); // size, will be filled in later
        }

        ifu (colGrid.size != m_meta.frameSize)
            rete(ErrVal::NO_VALID);

        if (m_subCodecType == SubCodecType::BI_SWITCH)
        {
            BitVec frameColDatByX, frameColDatByY;
            m_subCodecBiSwitch._FrameColDatGenByX_BiSwitch(frameColDatByX, colGrid);
            m_subCodecBiSwitch._FrameColDatGenByY_BiSwitch(frameColDatByY, colGrid);

            cx AU isByY = (frameColDatByY.SizeByte() < frameColDatByX.SizeByte());

            m_file.WriteVal(U1(isByY));
            if (isByY)
                m_file.Write(frameColDatByY.Dat(), frameColDatByY.SizeByte());
            else
                m_file.Write(frameColDatByX.Dat(), frameColDatByX.SizeByte());
        }
        else if (m_subCodecType == SubCodecType::EDGE_FILL)
        {
            if (m_lazyRefFrameI == -1)
            {
                m_lazyRef = colGrid;
                m_lazyRefFrameI = m_frameCnt;
            }
            else
            {
                ColGridVNCmpInfo cmpInfo;
                cmpInfo.mode = ColGridVNCmpInfo::Mode::CHUNK_MAX;
                cmpInfo.longChunkCntSuggest = 10;
                F4 diff;
                ifep(ColGridVNCmp(diff, colGrid, m_lazyRef, cmpInfo));
                cxex AU LAZY_DIFF_THRESHOLD = F4(0.075);
                if (diff >= LAZY_DIFF_THRESHOLD)
                {
                    for (SI i = m_lazyRefFrameI + 1; i < m_frameCnt - 1; ++i)
                        m_subCodecEdgeFill.frames[i].isLazy = YES;
                    m_lazyRef = colGrid;
                    m_lazyRefFrameI = m_frameCnt;
                }
            }

            cx AU colBase = tx->FrameBaseCol(colGrid);
            cx AU colBaseAlter = (colBase == ColV(0x00) ? ColV(0xFF) : ColV(0x00));

            SubCodecEdgeFill::Frame frame;
            SubCodecEdgeFill::Frame frameAlter;
            ifep(m_subCodecEdgeFill._FrameEncode_EdgeFill(frame, colGrid, colBase, m_thdTaskMgr));
            ifep(m_subCodecEdgeFill._FrameEncode_EdgeFill(frameAlter, colGrid, colBaseAlter, m_thdTaskMgr));

            // choose the smaller one
            cxex AU FILL_ORIGIN_SIZE_AVG = F4(2.25);
            cx AU frameCost = frame.pixelPath.SizeByte() + SI(F4(frame.fillOrigins.size()) * FILL_ORIGIN_SIZE_AVG);
            cx AU frameAlterCost = frameAlter.pixelPath.SizeByte() + SI(F4(frameAlter.fillOrigins.size()) * FILL_ORIGIN_SIZE_AVG);
            if (frameAlterCost < frameCost)
                m_subCodecEdgeFill.frames.emplace_back(std::move(frameAlter));
            else
                m_subCodecEdgeFill.frames.emplace_back(std::move(frame));
        }
        else
        {
            rete(ErrVal::NO_SUPPORT);
        }
        // jdst(skipEncode);

        ++m_frameCnt;

        rets;
    }
    dfa ER End()
    {
        if (m_subCodecType == SubCodecType::BI_SWITCH)
        {
            ;
        }
        else if (m_subCodecType == SubCodecType::EDGE_FILL)
        {
            // process remaining lazy frames
            for (SI i = m_lazyRefFrameI + 1; i < m_frameCnt - 1; ++i)
                m_subCodecEdgeFill.frames[i].isLazy = YES;

            // filter lazy
            TO(m_subCodecEdgeFill.frames) framesNolazy;
            for (cx AU& frame : m_subCodecEdgeFill.frames)
            {
                if (!frame.isLazy)
                    framesNolazy.emplace_back(frame);
            }

            // simplify lazy
            BitVec lazyBox;
            {
                std::vector<SI> vals;
                SI cnt = 0;
                BO last = NO;
                for (cx AU& frame : m_subCodecEdgeFill.frames)
                {
                    if (frame.isLazy == last)
                    {
                        ++cnt;
                    }
                    else
                    {
                        vals.emplace_back(cnt);
                        cnt = 1;
                        last = frame.isLazy;
                    }
                }
                vals.emplace_back(cnt);
                ValSeqBoxEncode(lazyBox, vals.data(), vals.size());
            }

            // simplify base
            BitVec baseColBox;
            {
                std::vector<SI> baseColCnts;
                SI cnt = 0;
                BO last = NO;
                for (cx AU& frame : framesNolazy)
                {
                    if (frame.isBaseWhite == last)
                    {
                        ++cnt;
                    }
                    else
                    {
                        baseColCnts.emplace_back(cnt);
                        cnt = 1;
                        last = frame.isBaseWhite;
                    }
                }
                baseColCnts.emplace_back(cnt);
                ValSeqBoxEncode(baseColBox, baseColCnts.data(), baseColCnts.size());
            }

            // simplify fill
            BitVec fillCntBox;
            BitVec fillOriginBox;
            {
                std::vector<SI> fillCnts;
                std::vector<SI> fillOriginDiffs;
                fillCnts.reserve(framesNolazy.size());
                fillOriginDiffs.reserve(framesNolazy.size() * 4);
                for (cx AU& frame : framesNolazy)
                {
                    fillCnts.emplace_back(frame.fillOrigins.size());

                    SI fillOriginLast = -1;
                    for (cx AU& fillOrigin : frame.fillOrigins)
                    {
                        cx AU fillOriginDiff = fillOrigin - fillOriginLast;
                        fillOriginDiffs.emplace_back(fillOriginDiff);
                        fillOriginLast = fillOrigin;
                    }
                }
                ValSeqBoxEncode(fillCntBox, fillCnts.data(), fillCnts.size());
                ValSeqBoxEncode(fillOriginBox, fillOriginDiffs.data(), fillOriginDiffs.size());
            }

            // lazy
            m_file.Write(lazyBox.Dat(), lazyBox.SizeByte());

            // base
            m_file.Write(baseColBox.Dat(), baseColBox.SizeByte());

            // fill
            m_file.Write(fillCntBox.Dat(), fillCntBox.SizeByte());
            m_file.Write(fillOriginBox.Dat(), fillOriginBox.SizeByte());

            for (cx AU& frame : framesNolazy)
            {
                // path
                m_file.Write(frame.pixelPath.Dat(), frame.pixelPath.SizeByte());
            }
        }

        {
            cx AU curPos = m_file.CurGet();
            cx AU colDatSize = curPos - m_colDatSizePos - siz(U4);
            m_file.CurSet(m_colDatSizePos);
            m_file.WriteVal(U4(colDatSize));
            m_file.CurSet(curPos);
        }

        m_file.WriteVal(DatBlockType::GRID_SIZE);
        m_file.WriteVarint(U4(m_meta.frameSize.w));
        m_file.WriteVarint(U4(m_meta.frameSize.h));
        m_file.WriteVal(U4(m_frameCnt));
        m_file.WriteVal(DatBlockType::END);

        ifep(m_file.Close(YES));

        ifep(m_thdTaskMgr.Free());

        rets;
    }

  public:
    dfa Encoder()
    {
        m_lazyRefFrameI = -1;
        m_colDatSizePos = 0;
    }
};

#endif

struct DecoderSubCodecBiSwitch : public SubCodecBiSwitch
{
    dfa NT _CntListToColGrid_ByX_BiSwitch(ColGrid<ColV>& colGrid, cx std::vector<SI>& cntList)
    {
        // TODO: merge with _CntListToColGrid_ByY_BiSwitch

        SI cntListI = 0;

        cntCur = 0;
        colCur = 1;

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
                colCur = colCur ? 0 : 1;
            }

            cx AU cntToRead = Min(cntCur, pixelCnt - pixelI);
            cx AU colSrc = colCur ? ColV(0xFF) : ColV(0x00);
            AU pixelCur = colGrid.pixels.data() + pixelI;
            ite (i, i < cntToRead)
                *(pixelCur + i) = colSrc;
            pixelI += cntToRead;
            cntCur -= cntToRead;
        }
    }
    dfa NT _CntListToColGrid_ByY_BiSwitch(ColGrid<ColV>& colGrid, cx std::vector<SI>& cntList)
    {
        // TODO: merge with _CntListToColGrid_ByX_BiSwitch

        SI cntListI = 0;

        cntCur = 0;
        colCur = 1;

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
                    colCur ^= 1;
                    ifu (cntCur == 0) // this happens if the first pixel of the frame is not the default color
                        jsrc(nextCol);
                }
                colGrid.Pixel(Pos2<SI>(x, y)) = ColV(0 - colCur);
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
    DecoderSubCodecBiSwitch m_subCodecBiSwitch;
    DecoderSubCodecEdgeFill m_subCodecEdgeFill;

  private:
    SI m_colDatPos;
    SI m_colDatSize;
    SI m_frameCur;
    union {
        U1 m_varU1;
        U2 m_varU2;
        U4 m_varU4;
        U8 m_varU8;
    };

  public:
    dfa ER Begin(cx CH* path)
    {
        ifep(m_file.Open(path, YES));
        m_meta.frameSize = Size2<SI>(0, 0);
        m_frameCnt = 0;
        m_subCodecEdgeFill.frames.clear();
        m_subCodecType = SubCodecType::NONE;
        m_colDatFeatures = ColDatFeatures(0);
        m_meta.fps = 30;

        ifu (m_file.ReadVal(m_varU4) != siz(m_varU4))
            rete(ErrVal::FILE);
        ifu (m_varU4 != MAGIC)
            rete(ErrVal::NO_VALID);

        {
            DatBlockType datBlockType = DatBlockType::NONE;
            while (datBlockType != DatBlockType::END)
            {
                ifu (m_file.ReadVal(datBlockType) != siz(datBlockType))
                    rete(ErrVal::FILE);

                switch (datBlockType)
                {
                case DatBlockType::NONE: {
                    break;
                }
                case DatBlockType::GRID_SIZE: {
                    ifu (m_file.ReadVarint(m_varU4) == 0)
                        rete(ErrVal::FILE);
                    m_meta.frameSize.w = m_varU4;
                    ifu (m_file.ReadVarint(m_varU4) == 0)
                        rete(ErrVal::FILE);
                    m_meta.frameSize.h = m_varU4;
                    ifu (m_file.ReadVal(m_varU4) != siz(m_varU4))
                        rete(ErrVal::FILE);
                    m_frameCnt = m_varU4;
                    break;
                }
                case DatBlockType::FPS: {
                    ifu (m_file.ReadVarint(m_varU4) == 0)
                        rete(ErrVal::FILE);
                    m_meta.fps = m_varU4;
                    break;
                }
                case DatBlockType::COL_DAT_TYPE: {
                    ifu (m_file.ReadVal(m_subCodecType) != siz(m_subCodecType))
                        rete(ErrVal::FILE);
                    ifu (m_file.ReadVarint(AsType<U4>(m_colDatFeatures)) == 0)
                        rete(ErrVal::FILE);
                    ifu (U4(m_colDatFeatures) != 0)
                        rete(ErrVal::NO_SUPPORT);
                    break;
                }
                case DatBlockType::COL_DAT: {
                    ifu (m_file.ReadVal(m_varU4) != siz(m_varU4))
                        rete(ErrVal::FILE);
                    m_colDatSize = m_varU4;
                    m_colDatPos = m_file.CurGet();
                    m_file.CurMove(m_colDatSize);
                    break;
                }
                case DatBlockType::END: {
                    break;
                }
                default: {
                    rete(ErrVal::NO_SUPPORT);
                }
                }
            }
        }

        m_frameCur = 0;
        m_file.CurSet(m_colDatPos);

        if (m_subCodecType == SubCodecType::EDGE_FILL)
        {
            // init
            m_subCodecEdgeFill.frames.resize(m_frameCnt);

            // lazy
            {
                std::vector<SI> vals;
                m_file.ReadValSeqBox(vals);
                SI frameI = 0;
                BO last = NO;
                for (cx AU& val : vals)
                {
                    ite (i, i < val)
                        m_subCodecEdgeFill.frames[frameI++].isLazy = last;
                    last = !last;
                }
            }

            // lazy map
            std::vector<SubCodecEdgeFill::Frame*> framesNolazyMap;
            framesNolazyMap.reserve(m_frameCnt);
            {
                for (AU& frame : m_subCodecEdgeFill.frames)
                {
                    if (!frame.isLazy)
                        framesNolazyMap.emplace_back(&frame);
                }
            }

            // base
            {
                std::vector<SI> baseColCnts;
                m_file.ReadValSeqBox(baseColCnts);
                SI frameI = 0;
                BO baseCur = NO;
                for (cx AU& baseColCnt : baseColCnts)
                {
                    ite (i, i < baseColCnt)
                        framesNolazyMap[frameI++]->isBaseWhite = baseCur;
                    baseCur = !baseCur;
                }
            }

            // fill
            {
                std::vector<SI> fillCnts;
                std::vector<SI> fillOriginDiffs;
                m_file.ReadValSeqBox(fillCnts);
                m_file.ReadValSeqBox(fillOriginDiffs);
                SI frameI = 0;
                SI fillOriginDiffIOfs = 0;
                for (cx AU& fillCnt : fillCnts)
                {
                    framesNolazyMap[frameI]->fillOrigins.reserve(fillCnt);
                    SI fillOriginMain = -1;
                    ite (i, i < fillCnt)
                    {
                        fillOriginMain += fillOriginDiffs[i + fillOriginDiffIOfs];
                        framesNolazyMap[frameI]->fillOrigins.emplace_back(fillOriginMain);
                    }
                    fillOriginDiffIOfs += fillCnt;
                    ++frameI;
                }
            }

            ite (i, i < SI(framesNolazyMap.size()))
            {
                AU& frame = *(framesNolazyMap[i]);

                // path
                ifu (m_file.ReadVal<U4>(m_varU4) != siz(m_varU4))
                    rete(ErrVal::FILE);
                cx AU pixelPathDatSize = SI(m_varU4);
                std::vector<U1> pixelPathDat(pixelPathDatSize);
                ifu (m_file.Read(pixelPathDat.data(), pixelPathDat.size()) != SI(pixelPathDat.size()))
                    rete(ErrVal::FILE);
                frame.pixelPath.AddLast(&pixelPathDatSize, sizb(U4));
                frame.pixelPath.AddLast(pixelPathDat.data(), pixelPathDat.size() * BIT_IN_BYTE);
            }
        }

        rets;
    }
    dfa ER FrameRead(ColGrid<ColV>& colGrid)
    {
        colGrid.Resize(m_meta.frameSize);

        if (m_subCodecType == SubCodecType::BI_SWITCH)
        {
            BO isByY;
            ifu (m_file.ReadVal(isByY) != siz(isByY))
                rete(ErrVal::FILE);

            std::vector<SI> cntList;
            ifu (m_file.ReadValSeqBox(cntList) < 1)
                rete(ErrVal::FILE);

            if (isByY)
                m_subCodecBiSwitch._CntListToColGrid_ByY_BiSwitch(colGrid, cntList);
            else
                m_subCodecBiSwitch._CntListToColGrid_ByX_BiSwitch(colGrid, cntList);
        }
        else if (m_subCodecType == SubCodecType::EDGE_FILL)
        {
            // get frame
            cx AU& frame = m_subCodecEdgeFill.frames[m_frameCur];

            // handle lazy
            if (frame.isLazy)
            {
                // for (AU& pixel : colGrid.pixels)
                // pixel = ColV(0x80);

                // get previous frame index which is not lazy
                AU frameIPrev = m_frameCur;
                while (m_subCodecEdgeFill.frames[frameIPrev].isLazy)
                    --frameIPrev;
                // copy previous frame
                ColGrid<ColV> colGridPrev;
                m_subCodecEdgeFill._FrameToColGrid_EdgeFill(colGridPrev, m_subCodecEdgeFill.frames[frameIPrev], m_meta.frameSize);

                // get next frame index which is not lazy
                AU frameINext = m_frameCur;
                while (m_subCodecEdgeFill.frames[frameINext].isLazy)
                    ++frameINext;
                // copy next frame
                ColGrid<ColV> colGridNext;
                m_subCodecEdgeFill._FrameToColGrid_EdgeFill(colGridNext, m_subCodecEdgeFill.frames[frameINext], m_meta.frameSize);

                // lerping between previous and next frame
                F4 lerpRatio = F4(m_frameCur - frameIPrev) / F4(frameINext - frameIPrev);
                ColGridLerp(colGrid, colGridPrev, colGridNext, lerpRatio);
            }
            else
            {
                m_subCodecEdgeFill._FrameToColGrid_EdgeFill(colGrid, frame, m_meta.frameSize);
            }
        }
        else
        {
            rete(ErrVal::NO_SUPPORT);
        }

        ++m_frameCur;

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
        ret m_frameCur < m_frameCnt;
    }

  public:
    dfa Size2<SI> FrameSizeGet() cx
    {
        ret m_meta.frameSize;
    }
    dfa SI FpsGet() cx
    {
        ret m_meta.fps;
    }

  public:
    dfa Decoder()
    {
        m_colDatPos = 0;
        m_colDatSize = 0;
        m_frameCur = 0;
        m_varU8 = 0;
    }
};

} // namespace Bada
