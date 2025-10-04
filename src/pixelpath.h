#pragma once

/// TODO: "pixelpath.h is in a PoC stage!" ///

/*
    [Pixel Path]
    Losslessly encoded pixel position data.
    Best suited for contiguous lines of pixels, like lines, arches, rectangles, circles, etc.

    - the format is not byte-aligned, but bit-aligned
    PixelPathFormat
        PathDat[] paths
        SinglePxDat singlePxs
    PathDat
        U4V posOfs
        U4V movePxCnt
        1B[] moveDat
    SinglePxDat
        U4V cntOfSinglePx
        U4V[cntOfSinglePx] posOfs_2

    // 'posOfs' offsets 'posMain'
    // 'posMain' initial value is '-1'
    // when 'posOfs' is '0', that marks there are no more paths
*/

class PixelPath
{
  private:
    enum class Dir : U1
    {
        UL = 0b000,
        U = 0b001,
        UR = 0b010,
        L = 0b011,
        R = 0b100,
        DL = 0b101,
        D = 0b110,
        DR = 0b111,
        CNT = 8,
    };

    static cxex SI DIR_SIZEB = 3;
    static cxex SI DIR_LEN_VAR = 15;

    static dfa SI GetIOfsByDir(Dir dir, SI w)
    {
        switch (dir)
        {
        case Dir::UL:
            ret -w - 1;
        case Dir::U:
            ret -w;
        case Dir::UR:
            ret -w + 1;
        case Dir::L:
            ret -1;
        case Dir::R:
            ret 1;
        case Dir::DL:
            ret w - 1;
        case Dir::D:
            ret w;
        case Dir::DR:
            ret w + 1;
        default:
            ret 0;
        }
    }
    static dfa Dir GetBackDir(Dir dir)
    {
        cx Dir backDir[] = {
            Dir::DR, // UL
            Dir::D,  // U
            Dir::DL, // UR
            Dir::R,  // L
            Dir::L,  // R
            Dir::UR, // DL
            Dir::U,  // D
            Dir::UL  // DR
        };
        ret backDir[SI(dir)];
    }
    static dfa U1 DirLenToMoves(SI len)
    {
        cxex U1 tab[] = {0b00, 0b00, 0b01, 0b10, 0b0101, 0b1001, 0b0110, 0b1010, 0b010101, 0b100101, 0b011001, 0b101001, 0b010110, 0b100110, 0b011010, 0b11101010};
        ret tab[len > 15 ? 15 : len];
    }
    static dfa SI MovesToDirLen(U1 moveHash)
    {
        cxex U1 tab[] = {0, 2, 3, 0, 4, 6, 0, 5, 7, 0, 0, 0, 0, 8, 12, 0, 10, 14, 0, 0, 0, 0, 9, 13, 0, 11, DIR_LEN_VAR};
        ret tab[moveHash];
    }

  public:
    enum class BuildStrat : U1
    {
        PLUS = 0,
        FORWARD1 = 1,
        FORWARD2 = 2,
        RAND1 = 3,
        RAND2 = 4,
        RAND3 = 5,
        RAND4 = 6,
        CNT = 7,
    };

    tpl1 static dfa NT Build(ColGrid<T1>& colGrid, BitVec& pixelPath, cx T1& colObj, cx T1& colMark, BuildStrat strat)
    {
        struct DirLen
        {
            Dir dir;
            SI len;

            dfa DirLen(Dir dir = Dir::CNT, SI len = 0) noex : dir(dir), len(len)
            {
            }
        };
        struct SinglePx
        {
            SI ofs;

            dfa SinglePx(SI ofs = 0) noex : ofs(ofs)
            {
            }
        };

        pixelPath.Clr();
        SI pxCurILastPaths = -1;
        SI pxCurILastSinglePxs = -1;
        std::vector<Dir> moveBufDir;
        std::vector<DirLen> moveBufDirLen;
        BitVec moveBuf;
        std::vector<SinglePx> singlePxs;

        U1 buf[VarintSizeMax<U4>()];
        SI bufDatSize;

        cx AU pxBase = colGrid.pixels.data();
        cx AU pxEnd = pxBase + colGrid.pixels.size();
        AU pxCurMain = pxBase;
        while (pxCurMain != pxEnd)
        {
            if (*pxCurMain == colObj)
            {
                AU pxCur = pxCurMain;
                cx AU pxCurI = SI(pxCur - pxBase);

                *pxCur = colMark;
                moveBufDir.clear();
                AU forwardDir = Dir(0);

                while (YES)
                {
                    cx AU getStratDirMap = [](Dir* stratDirMap, BuildStrat strat_, Dir forwardDir_) -> NT {
                        switch (strat_)
                        {
                        case BuildStrat::PLUS:
                            stratDirMap[0] = Dir::D;
                            stratDirMap[1] = Dir::R;
                            stratDirMap[2] = Dir::L;
                            stratDirMap[3] = Dir::U;
                            stratDirMap[4] = Dir::UL;
                            stratDirMap[5] = Dir::UR;
                            stratDirMap[6] = Dir::DL;
                            stratDirMap[7] = Dir::DR;
                            break;
                        case BuildStrat::FORWARD1: {
                            SI len = 0;
                            stratDirMap[len++] = forwardDir_;
                            if (Dir::DR != forwardDir_)
                                stratDirMap[len++] = Dir::DR;
                            if (Dir::D != forwardDir_)
                                stratDirMap[len++] = Dir::D;
                            if (Dir::DL != forwardDir_)
                                stratDirMap[len++] = Dir::DL;
                            if (Dir::R != forwardDir_)
                                stratDirMap[len++] = Dir::R;
                            if (Dir::L != forwardDir_)
                                stratDirMap[len++] = Dir::L;
                            if (Dir::UR != forwardDir_)
                                stratDirMap[len++] = Dir::UR;
                            if (Dir::U != forwardDir_)
                                stratDirMap[len++] = Dir::U;
                            if (Dir::UL != forwardDir_)
                                stratDirMap[len++] = Dir::UL;
                            break;
                        }
                        case BuildStrat::FORWARD2: {
                            SI len = 0;
                            stratDirMap[len++] = forwardDir_;
                            if (Dir::UL != forwardDir_)
                                stratDirMap[len++] = Dir::UL;
                            if (Dir::D != forwardDir_)
                                stratDirMap[len++] = Dir::D;
                            if (Dir::UR != forwardDir_)
                                stratDirMap[len++] = Dir::UR;
                            if (Dir::DL != forwardDir_)
                                stratDirMap[len++] = Dir::DL;
                            if (Dir::L != forwardDir_)
                                stratDirMap[len++] = Dir::L;
                            if (Dir::R != forwardDir_)
                                stratDirMap[len++] = Dir::R;
                            if (Dir::U != forwardDir_)
                                stratDirMap[len++] = Dir::U;
                            if (Dir::DR != forwardDir_)
                                stratDirMap[len++] = Dir::DR;
                            break;
                        }
                        case BuildStrat::RAND1:
                            stratDirMap[0] = Dir::L;
                            stratDirMap[1] = Dir::DL;
                            stratDirMap[2] = Dir::D;
                            stratDirMap[3] = Dir::R;
                            stratDirMap[4] = Dir::U;
                            stratDirMap[5] = Dir::DR;
                            stratDirMap[6] = Dir::UL;
                            stratDirMap[7] = Dir::UR;
                            break;
                        case BuildStrat::RAND2:
                            stratDirMap[0] = Dir::U;
                            stratDirMap[1] = Dir::L;
                            stratDirMap[2] = Dir::R;
                            stratDirMap[3] = Dir::D;
                            stratDirMap[4] = Dir::DR;
                            stratDirMap[5] = Dir::UL;
                            stratDirMap[6] = Dir::UR;
                            stratDirMap[7] = Dir::DL;
                            break;
                        case BuildStrat::RAND3:
                            stratDirMap[0] = Dir::D;
                            stratDirMap[1] = Dir::DL;
                            stratDirMap[2] = Dir::DR;
                            stratDirMap[3] = Dir::L;
                            stratDirMap[4] = Dir::R;
                            stratDirMap[5] = Dir::U;
                            stratDirMap[6] = Dir::UL;
                            stratDirMap[7] = Dir::UR;
                            break;
                        case BuildStrat::RAND4:
                            stratDirMap[0] = Dir::R;
                            stratDirMap[1] = Dir::U;
                            stratDirMap[2] = Dir::D;
                            stratDirMap[3] = Dir::L;
                            stratDirMap[4] = Dir::UR;
                            stratDirMap[5] = Dir::UL;
                            stratDirMap[6] = Dir::DR;
                            stratDirMap[7] = Dir::DL;
                            break;
                        default:
                            break;
                        }
                    };
                    Dir stratDirMap[SI(Dir::CNT)];
                    getStratDirMap(stratDirMap, strat, forwardDir);
                    ite (i, i < SI(Dir::CNT))
                    {
                        cx AU dir = stratDirMap[i];
                        cx AU iOfs = GetIOfsByDir(dir, colGrid.size.w);
                        AU pxTest = pxCur + iOfs;
                        ifu ((pxTest < pxBase) || (pxTest >= pxEnd))
                            continue;

                        if (*pxTest != colObj)
                            continue;

                        pxCur = pxTest;
                        moveBufDir.emplace_back(dir);
                        *pxCur = colMark;
                        forwardDir = dir;
                        jsrc(validStep);
                    }
                    break;
                    jdst(validStep);
                }

                cx AU isMultiPxMode = (moveBufDir.size() > 0);

                ifl (isMultiPxMode)
                {
                    moveBufDirLen.clear();
                    moveBufDirLen.reserve(moveBufDir.size());
                    AU dirPrev = moveBufDir[0];
                    SI dirCnt = 1;
                    for (SI i = 1; i < SI(moveBufDir.size()); ++i)
                    {
                        if (moveBufDir[i] == dirPrev)
                        {
                            ++dirCnt;
                            continue;
                        }
                        moveBufDirLen.emplace_back(dirPrev, U4(dirCnt));
                        dirPrev = moveBufDir[i];
                        dirCnt = 1;
                    }
                    moveBufDirLen.emplace_back(dirPrev, U4(dirCnt));

                    moveBuf.Clr();
                    for (cx AU& dirLen : moveBufDirLen)
                    {
                        moveBuf.AddLast(&dirLen.dir, DIR_SIZEB);
                        if (dirLen.len > 1)
                        {
                            cx AU backDir = GetBackDir(dirLen.dir);
                            AU encodeStrVal = DirLenToMoves(dirLen.len);
                            do
                            {
                                switch (encodeStrVal & 0b11)
                                {
                                case 0b01:
                                    moveBuf.AddLast(&dirLen.dir, DIR_SIZEB);
                                    break;
                                case 0b10:
                                    moveBuf.AddLast(&backDir, DIR_SIZEB);
                                    break;
                                case 0b11: {
                                    bufDatSize = VarintEncode(buf, AsTypeU(dirLen.len));
                                    moveBuf.AddLast(buf, bufDatSize * BIT_IN_BYTE);
                                    break;
                                }
                                default:
                                    jsrc(double_break_1);
                                }
                                encodeStrVal >>= 2;
                            } while (YES);
                            jdst(double_break_1);
                        }
                    }

                    cx AU posOfs = pxCurI - pxCurILastPaths;
                    pxCurILastPaths = pxCurI;

                    bufDatSize = VarintEncode(buf, U4(posOfs));
                    pixelPath.AddLast(buf, bufDatSize * BIT_IN_BYTE);

                    bufDatSize = VarintEncode(buf, U4(moveBufDir.size()));
                    pixelPath.AddLast(buf, bufDatSize * BIT_IN_BYTE);

                    pixelPath.AddLast(moveBuf.Dat(), moveBuf.SizeBit());
                }
                else
                {
                    cx AU posOfs = pxCurI - pxCurILastSinglePxs;
                    pxCurILastSinglePxs = pxCurI;

                    singlePxs.emplace_back(posOfs);
                }
            }

            ++pxCurMain;
        }

        // (posOfs == 0) marks end of paths
        bufDatSize = VarintEncode(buf, U4(0));
        pixelPath.AddLast(buf, bufDatSize * BIT_IN_BYTE);

        bufDatSize = VarintEncode(buf, U4(singlePxs.size()));
        pixelPath.AddLast(buf, bufDatSize * BIT_IN_BYTE);
        for (cx AU& singlePx : singlePxs)
        {
            bufDatSize = VarintEncode(buf, U4(singlePx.ofs));
            pixelPath.AddLast(buf, bufDatSize * BIT_IN_BYTE);
        }

        pixelPath.PadToByte();
    }
    tpl1 static dfa NT Draw(ColGrid<T1>& colGrid, cx BitVec& pixelPath, cx T1& col)
    {
        U4 varU4;
        SI posMain = -1;
        AU pxPathCurb = 0;

        while (YES)
        {
            pxPathCurb += pixelPath.GetVarintAt(varU4, pxPathCurb) * BIT_IN_BYTE;
            cx AU posOfs = SI(varU4);

            ifu (posOfs == 0)
                break;

            posMain += posOfs;
            AU pxCur = colGrid.pixels.data() + posMain;
            *pxCur = col;

            pxPathCurb += pixelPath.GetVarintAt(varU4, pxPathCurb) * BIT_IN_BYTE;
            cx AU moveCntEnd = SI(varU4);
            AU moveCntCur = SI(0);

            while (moveCntCur != moveCntEnd)
            {
                U1 varU1;
                pixelPath.GetAt(&varU1, DIR_SIZEB, pxPathCurb);
                pxPathCurb += DIR_SIZEB;
                cx AU dir = Dir(varU1);
                cx AU backDir = GetBackDir(dir);
                cx AU iOfs = GetIOfsByDir(Dir(dir), colGrid.size.w);

                U1 moveHash = 0;
                U1 moveHashMul = 1;
                AU moveCnt = SI(1);
                while (moveCntCur + moveCnt != moveCntEnd)
                {
                    pixelPath.GetAt(&varU1, DIR_SIZEB, pxPathCurb);
                    cx AU dirNext = Dir(varU1);
                    if (dirNext != dir && dirNext != backDir)
                        break;
                    pxPathCurb += DIR_SIZEB;

                    moveHash += U1(((dirNext == dir) ? 1 : 2) * moveHashMul);
                    moveHashMul *= 3;
                    moveCnt = MovesToDirLen(moveHash);

                    ifu (moveCnt == DIR_LEN_VAR)
                    {
                        pxPathCurb += pixelPath.GetVarintAt(varU4, pxPathCurb) * BIT_IN_BYTE;
                        moveCnt = SI(varU4);
                    }
                }

                ite (i, i < moveCnt)
                {
                    cx AU pxDst = pxCur + iOfs;
                    pxCur = pxDst;
                    *pxCur = col;
                }
                moveCntCur += moveCnt;
            }
        }

        posMain = -1;
        pxPathCurb += pixelPath.GetVarintAt(varU4, pxPathCurb) * BIT_IN_BYTE;
        cx AU singlePxCnt = SI(varU4);
        ite (i, i < singlePxCnt)
        {
            pxPathCurb += pixelPath.GetVarintAt(varU4, pxPathCurb) * BIT_IN_BYTE;
            cx AU posOfs = SI(varU4);
            posMain += posOfs;
            cx AU pxCur = colGrid.pixels.data() + posMain;
            *pxCur = col;
        }
    }
};
