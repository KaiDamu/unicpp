#pragma once

#include "container.hpp"
#include "color.hpp"

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

    static dfa SI GetIOfsByDir(Dir dir, SI w);
    static dfa Dir GetBackDir(Dir dir);
    static dfa U1 DirLenToMoves(SI len);
    static dfa SI MovesToDirLen(U1 moveHash);

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

    tpl1 static dfa NT Build(ColGrid<T1>& colGrid, BitVec& pixelPath, cx T1& colObj, cx T1& colMark, BuildStrat strat);
    tpl1 static dfa NT Draw(ColGrid<T1>& colGrid, cx BitVec& pixelPath, cx T1& col);
};

#include "pixelpath.ipp"
