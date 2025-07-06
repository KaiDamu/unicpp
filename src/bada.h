#pragma once

/// TODO: "bada.h is in a PoC stage!" ///

#ifdef PROG_SYS_WIN

enum class BadaColDatType : U1
{
    NONE = 0,
    BI_SWITCH_LTR = 1,
    EDGE_FILL = 2,
};
enum class BadaColDatFeatures : U4
{
    RESERVED = 0b0,
};
enum class BadaDatBlockType : U1
{
    NONE = 0,
    GRID_SIZE = 1,
    FPS = 2,
    COL_DAT_TYPE = 3,
    COL_DAT = 4,
    END = 5,
};

cxex U4 BADA_MAGIC = 0x41444142; // "BADA"

/*
   The following information is describing the format of the BADA codec and file format.

   The BADA codec is used to simply and efficiently encode and decode videos only containing shades of black and white.
   Inspired by the music video "Bad Apple!!".

   Although not enforced, it is suggested to use ".bada" as the file extension.

   (see file structure below)

   The 'magic' field must be "BADA".
   The last element of 'datBlocks' must have 'type' set to 'BadaDatBlockType.END'.

   [File structure]
       U4 magic = "BADA"
       DatBlock[] datBlocks

   [File structure structures]
       DatBlock
           U1 type (BadaDatBlockType)
           U1[] dat

   [File structure structure types]
       BadaDatBlockType.NONE
           U1[0] none
       BadaDatBlockType.GRID_SIZE
           U4V w
           U4V h
           U4 cnt
       BadaDatBlockType.FPS
           U4V cnt
       BadaDatBlockType.COL_DAT_TYPE
           U1 type (BadaColDatType)
           U4V features (BadaColDatFeatures)
           U1[] featuresDat
       BadaDatBlockType.COL_DAT
           U4 size
           U1[size] dat
       BadaDatBlockType.END
           U1[0] none

   [Color data types]
       BadaColDatType.NONE
       BadaColDatType.BI_SWITCH_LTR
       BadaColDatType.EDGE_FILL
*/

class BadaCodec
{
  private:
    struct Frame
    {
        BO isLazy;
        BO isBaseWhite;
        BitVec pixelPath;
        std::vector<SI> fillOrigins;

        dfa Frame()
        {
            isLazy = NO;
            isBaseWhite = NO;
        }
    };

  private:
    // ...
    FileMem m_file;
    Size2<SI> m_frameSize;
    SI m_frameCnt;
    std::vector<Frame> m_frames;
    ColGrid<ColV> m_lazyRef;
    SI m_lazyRefFrameI;
    BadaColDatType m_colDatType;
    BadaColDatFeatures m_colDatFeatures;
    SI m_fps;
    // ...
    ThdTaskMgr m_thdTaskMgr;
    // ...
    SI m_cntCur;
    U1 m_colCur;
    // ...
    SI m_colDatSizePos;
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
    struct EncodeParams
    {
        BadaColDatType colDatType;
        BadaColDatFeatures colDatFeatures;
        SI fps;

        dfa EncodeParams()
        {
            colDatType = BadaColDatType::BI_SWITCH_LTR;
            colDatFeatures = BadaColDatFeatures(0);
            fps = 30;
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
        /* // this makes it worse...
        if (IsBetween<F4>(blackRatio, 0.3, 0.7))
        {
            SI blackSide = 0;
            ite (i, i < SI(colGrid.pixels.size()))
                blackSide += ToType<ColV>(colGrid.pixels[i]).v < TO(ColV::v)(0x80) ? 1 : -1;
            ret ((blackSide >= 0) ? ColV(0x00) : ColV(0xFF));
        }
        */
        ret ((blackRatio >= F4(0.5)) ? ColV(0x00) : ColV(0xFF));
    }

  private:
    dfa ER _FrameEncode_EdgeFill(Frame& frame, ColGrid<ColV> colGrid, cx ColV& colBase)
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
            m_thdTaskMgr.TaskAdd(thdTaskList);
        }
        m_thdTaskMgr.WaitAll();

        AU stratMinI = SI(-1);
        AU stratMinSize = 1e9;
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
    dfa NT _FrameToColGrid_EdgeFill(ColGrid<ColV>& colGrid, cx Frame& frame)
    {
        // init
        colGrid.Resize(m_frameSize);

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

  public:
    dfa ER BeginEncode(cx CH* path, cx EncodeParams& params)
    {
        ife (m_thdTaskMgr.Init())
            retep;
        m_colDatType = params.colDatType;
        m_colDatFeatures = params.colDatFeatures;
        m_fps = params.fps;
        ife (m_file.OpenWrite(path))
            retep;
        ife (m_file.WriteVal(BADA_MAGIC))
            retep;
        ife (m_file.WriteVal(BadaDatBlockType::FPS))
            retep;
        ife (m_file.WriteVarint(U4(m_fps)))
            retep;
        ife (m_file.WriteVal(BadaDatBlockType::COL_DAT_TYPE))
            retep;
        ife (m_file.WriteVal(m_colDatType))
            retep;
        ife (m_file.WriteVarint(U4(m_colDatFeatures)))
            retep;
        m_frameSize = Size2<SI>(0, 0);
        m_frameCnt = 0;
        m_frames.clear();
        m_cntCur = 0;
        m_colCur = 0;
        rets;
    }
    dfa ER BeginDecode(cx CH* path)
    {
        ife (m_file.OpenRead(path))
            retep;
        m_frameSize = Size2<SI>(0, 0);
        m_frameCnt = 0;
        m_frames.clear();
        m_colDatType = BadaColDatType::NONE;
        m_colDatFeatures = BadaColDatFeatures(0);
        m_fps = 30;

        ife (m_file.ReadVal(m_varU4))
            retep;
        ifu (m_varU4 != BADA_MAGIC)
            rete(ErrVal::NO_VALID);

        {
            BadaDatBlockType datBlockType = BadaDatBlockType::NONE;
            while (datBlockType != BadaDatBlockType::END)
            {
                ife (m_file.ReadVal(datBlockType))
                    retep;

                switch (datBlockType)
                {
                case BadaDatBlockType::NONE: {
                    break;
                }
                case BadaDatBlockType::GRID_SIZE: {
                    ife (m_file.ReadVarint(m_varU4))
                        retep;
                    m_frameSize.w = m_varU4;
                    ife (m_file.ReadVarint(m_varU4))
                        retep;
                    m_frameSize.h = m_varU4;
                    ife (m_file.ReadVal(m_varU4))
                        retep;
                    m_frameCnt = m_varU4;
                    break;
                }
                case BadaDatBlockType::FPS: {
                    ife (m_file.ReadVarint(m_varU4))
                        retep;
                    m_fps = m_varU4;
                    break;
                }
                case BadaDatBlockType::COL_DAT_TYPE: {
                    ife (m_file.ReadVal(m_colDatType))
                        retep;
                    ife (m_file.ReadVarint(AsType<U4>(m_colDatFeatures)))
                        retep;
                    ifu (U4(m_colDatFeatures) != 0)
                        rete(ErrVal::NO_SUPPORT);
                    break;
                }
                case BadaDatBlockType::COL_DAT: {
                    ife (m_file.ReadVal(m_varU4))
                        retep;
                    m_colDatSize = m_varU4;
                    m_colDatPos = m_file.CurPos();
                    ife (m_file.CurMove(m_colDatSize))
                        retep;
                    break;
                }
                case BadaDatBlockType::END: {
                    break;
                }
                default: {
                    rete(ErrVal::NO_SUPPORT);
                }
                }
            }
        }

        m_frameCur = 0;
        m_cntCur = 0;
        m_colCur = 1;
        ife (m_file.CurSet(m_colDatPos))
            retep;

        if (m_colDatType == BadaColDatType::EDGE_FILL)
        {
            // init
            m_frames.resize(m_frameCnt);

            // lazy
            {
                std::vector<SI> vals;
                m_file.ReadValSeqBox(vals);
                SI frameI = 0;
                BO last = NO;
                for (cx AU& val : vals)
                {
                    ite (i, i < val)
                        m_frames[frameI++].isLazy = last;
                    last = !last;
                }
            }

            // lazy map
            std::vector<Frame*> framesNolazyMap;
            framesNolazyMap.reserve(m_frameCnt);
            {
                for (AU& frame : m_frames)
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
                ife (m_file.ReadVal<U4>(m_varU4))
                    retep;
                cx AU pixelPathDatSize = SI(m_varU4);
                std::vector<U1> pixelPathDat(pixelPathDatSize);
                ife (m_file.Read(pixelPathDat.data(), pixelPathDat.size()))
                    retep;
                frame.pixelPath.AddLast(&pixelPathDatSize, sizb(U4));
                frame.pixelPath.AddLast(pixelPathDat.data(), pixelPathDat.size() * BIT_IN_BYTE);
            }
        }

        rets;
    }
    dfa ER FrameEncode(cx ColGrid<ColV>& colGrid_)
    {
        ColGrid<ColV> colGrid = colGrid_;

        ifu (m_frameCnt == 0)
        {
            m_frameSize = colGrid.size;
            ife (m_file.WriteVal(BadaDatBlockType::COL_DAT))
                retep;
            m_colDatSizePos = m_file.CurPos();
            ife (m_file.WriteVal(U4(0))) // size, will be filled in later
                retep;
        }

        ifu (colGrid.size != m_frameSize)
            rete(ErrVal::NO_VALID);

        if (m_colDatType == BadaColDatType::BI_SWITCH_LTR)
        {
            AU pixel = colGrid.pixels.data();
            cx AU pixelEnd = pixel + colGrid.pixels.size();
            while (pixel != pixelEnd)
            {
                cx AU isWhite = pixel->v >= TO(ColV::v)(0x80);
                ifu (isWhite ^ m_colCur)
                {
                    ife (m_file.WriteVarint(U4(m_cntCur)))
                        retep;
                    m_colCur = isWhite;
                    m_cntCur = 1;
                }
                else
                {
                    ++m_cntCur;
                }
                ++pixel;
            }
        }
        else if (m_colDatType == BadaColDatType::EDGE_FILL)
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
                ife (ColGridVNCmp(diff, colGrid, m_lazyRef, cmpInfo))
                    retep;
                cxex AU LAZY_DIFF_THRESHOLD = F4(0.075);
                if (diff >= LAZY_DIFF_THRESHOLD)
                {
                    for (SI i = m_lazyRefFrameI + 1; i < m_frameCnt - 1; ++i)
                        m_frames[i].isLazy = YES;
                    m_lazyRef = colGrid;
                    m_lazyRefFrameI = m_frameCnt;
                }
            }

            cx AU colBase = tx->FrameBaseCol(colGrid);
            cx AU colBaseAlter = (colBase == ColV(0x00) ? ColV(0xFF) : ColV(0x00));

            Frame frame;
            Frame frameAlter;
            ife (tx->_FrameEncode_EdgeFill(frame, colGrid, colBase))
                retep;
            ife (tx->_FrameEncode_EdgeFill(frameAlter, colGrid, colBaseAlter))
                retep;

            // choose the smaller one
            cxex AU FILL_ORIGIN_SIZE_AVG = F4(2.25);
            cx AU frameCost = frame.pixelPath.SizeByte() + SI(frame.fillOrigins.size() * FILL_ORIGIN_SIZE_AVG);
            cx AU frameAlterCost = frameAlter.pixelPath.SizeByte() + SI(frameAlter.fillOrigins.size() * FILL_ORIGIN_SIZE_AVG);
            if (frameAlterCost < frameCost)
                m_frames.emplace_back(std::move(frameAlter));
            else
                m_frames.emplace_back(std::move(frame));
        }
        else
        {
            rete(ErrVal::NO_SUPPORT);
        }
        // jdst(skipEncode);

        ++m_frameCnt;

        rets;
    }
    dfa ER FrameDecode(ColGrid<ColV>& colGrid)
    {
        colGrid.Resize(m_frameSize);

        if (m_colDatType == BadaColDatType::BI_SWITCH_LTR)
        {
            cx AU pixelCnt = m_frameSize.Area();
            SI pixelI = 0;
            while (pixelI < pixelCnt)
            {
                if (m_cntCur == 0)
                {
                    ifu (m_file.CurPos() >= m_colDatPos + m_colDatSize)
                        rete(ErrVal::NO_EXIST);
                    ife (m_file.ReadVarint(m_varU4))
                        retep;
                    m_cntCur = m_varU4;
                    m_colCur = m_colCur ? 0 : 1;
                }

                cx AU cntToRead = Min(m_cntCur, pixelCnt - pixelI);
                cx AU colSrc = m_colCur ? ColV(0xFF) : ColV(0x00);
                AU pixelCur = colGrid.pixels.data() + pixelI;
                ite (i, i < cntToRead)
                    *(pixelCur + i) = colSrc;
                pixelI += cntToRead;
                m_cntCur -= cntToRead;
            }
        }
        else if (m_colDatType == BadaColDatType::EDGE_FILL)
        {
            // get frame
            cx AU& frame = m_frames[m_frameCur];

            // handle lazy
            if (frame.isLazy)
            {
                // for (AU& pixel : colGrid.pixels)
                // pixel = ColV(0x80);

                // get previous frame index which is not lazy
                AU frameIPrev = m_frameCur;
                while (m_frames[frameIPrev].isLazy)
                    --frameIPrev;
                // copy previous frame
                ColGrid<ColV> colGridPrev;
                tx->_FrameToColGrid_EdgeFill(colGridPrev, m_frames[frameIPrev]);

                // get next frame index which is not lazy
                AU frameINext = m_frameCur;
                while (m_frames[frameINext].isLazy)
                    ++frameINext;
                // copy next frame
                ColGrid<ColV> colGridNext;
                tx->_FrameToColGrid_EdgeFill(colGridNext, m_frames[frameINext]);

                // lerping between previous and next frame
                F4 lerpRatio = F4(m_frameCur - frameIPrev) / F4(frameINext - frameIPrev);
                ColGridLerp(colGrid, colGridPrev, colGridNext, lerpRatio);
            }
            else
            {
                tx->_FrameToColGrid_EdgeFill(colGrid, frame);
            }
        }
        else
        {
            rete(ErrVal::NO_SUPPORT);
        }

        ++m_frameCur;

        rets;
    }
    dfa ER EndEncode()
    {
        if (m_colDatType == BadaColDatType::BI_SWITCH_LTR)
        {
            if (m_cntCur > 0)
            {
                ife (m_file.WriteVarint(U4(m_cntCur)))
                    retep;
            }
        }
        else if (m_colDatType == BadaColDatType::EDGE_FILL)
        {
            // process remaining lazy frames
            for (SI i = m_lazyRefFrameI + 1; i < m_frameCnt - 1; ++i)
                m_frames[i].isLazy = YES;

            // filter lazy
            TO(m_frames) framesNolazy;
            for (cx AU& frame : m_frames)
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
                for (cx AU& frame : m_frames)
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
            ife (m_file.Write(lazyBox.Dat(), lazyBox.SizeByte()))
                retep;

            // base
            ife (m_file.Write(baseColBox.Dat(), baseColBox.SizeByte()))
                retep;

            // fill
            ife (m_file.Write(fillCntBox.Dat(), fillCntBox.SizeByte()))
                retep;
            ife (m_file.Write(fillOriginBox.Dat(), fillOriginBox.SizeByte()))
                retep;

            for (cx AU& frame : framesNolazy)
            {
                // path
                ife (m_file.Write(frame.pixelPath.Dat(), frame.pixelPath.SizeByte()))
                    retep;
            }
        }

        {
            cx AU curPos = m_file.CurPos();
            cx AU colDatSize = curPos - m_colDatSizePos - siz(U4);
            ife (m_file.CurSet(m_colDatSizePos))
                retep;
            ife (m_file.WriteVal(U4(colDatSize)))
                retep;
            ife (m_file.CurSet(curPos))
                retep;
        }

        ife (m_file.WriteVal(BadaDatBlockType::GRID_SIZE))
            retep;
        ife (m_file.WriteVarint(U4(m_frameSize.w)))
            retep;
        ife (m_file.WriteVarint(U4(m_frameSize.h)))
            retep;
        ife (m_file.WriteVal(U4(m_frameCnt)))
            retep;
        ife (m_file.WriteVal(BadaDatBlockType::END))
            retep;

        ife (m_file.Close())
            retep;

        ife (m_thdTaskMgr.Free())
            retep;

        rets;
    }
    dfa ER EndDecode()
    {
        ife (m_file.Close())
            retep;
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
        ret m_frameSize;
    }
    dfa SI FpsGet() cx
    {
        ret m_fps;
    }

  public:
    dfa BadaCodec()
    {
        m_frameSize = Size2<SI>(0, 0);
        m_frameCnt = 0;
        m_lazyRefFrameI = -1;
        m_colDatType = BadaColDatType::NONE;
        m_colDatFeatures = BadaColDatFeatures(0);
        m_fps = 0;
        m_cntCur = 0;
        m_colCur = 0;
        m_colDatSizePos = 0;
    }
};

#endif
