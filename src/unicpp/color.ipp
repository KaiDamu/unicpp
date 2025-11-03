#include "color.hpp"

#include "math.hpp"
#include "file.hpp"

tpl1 dfa T1& ColGrid<T1>::Pixel(cx Pos2<SI>& pos)
{
    ret pixels[pos.y * size.w + pos.x];
}
tpl1 dfa cx T1& ColGrid<T1>::Pixel(cx Pos2<SI>& pos) cx
{
    ret pixels[pos.y * size.w + pos.x];
}
tpl1 dfa NT ColGrid<T1>::Resize(cx Size2<SI>& size)
{
    tx->size = size;
    cx AU pixelCnt = size.Area();
    if (TO(pixelCnt)(pixels.size()) != pixelCnt)
        pixels.resize(pixelCnt);
}
tpl1 dfa ColGrid<T1>::ColGrid()
{
}
tpl1 dfa ColGrid<T1>::ColGrid(cx Size2<SI>& size) : size(size)
{
    pixels.resize(size.Area());
}

tpl2 dfa NT ColGridToType(ColGrid<T1>& dst, cx ColGrid<T2>& src)
{
    dst.size = src.size;
    dst.pixels.resize(src.pixels.size());
    cx AU iEnd = SI(src.pixels.size());
    ite (i, i < iEnd)
    {
        ToType<T1, T2>(dst.pixels[i], src.pixels[i]);
    }
}

tpl1 dfa ER ColGridCmpA1(F4& resultDiff, cx ColGrid<T1>& subGrid, cx ColGrid<T1>& mainGrid, cx ColGridCmpA1Info& info)
{
    static_assert(IsTypeSame<T1, ColVN> || IsTypeSame<T1, ColV>, "T1 must be ColVN");

    resultDiff = 1.0f;

    ifu (subGrid.size != mainGrid.size)
        rete(ErrVal::NO_VALID);

    if (info.mode == ColGridCmpA1Info::Mode::LINEAR)
    {
        AU diffSum = F8(0);
        cx AU iEnd = SI(subGrid.pixels.size());
        ite (i, i < iEnd)
            diffSum += F8(Diff(subGrid.pixels[i].v, mainGrid.pixels[i].v));
        resultDiff = F4(diffSum / F8(subGrid.pixels.size()));
    }
    else if (info.mode == ColGridCmpA1Info::Mode::CHUNK_MAX)
    {
        cx AU chunkSize = Size2ChunkSizeSuggest(subGrid.size, info.longChunkCntSuggest, 0.9f, 0.15f);
        cx AU chunkCnt = Size2ChunkCnt(subGrid.size, chunkSize);
        struct Chunk
        {
            F8 diffSum;
            SI diffCnt;

            dfa Chunk() : diffSum(0), diffCnt(0)
            {
            }
        };
        std::vector<Chunk> chunks(chunkCnt.Area());

        cx AU iEnd = SI(subGrid.pixels.size());
        ite (i, i < iEnd)
        {
            AU& chunk = chunks[Size2ChunkI(subGrid.size, chunkCnt, chunkSize, i)];
            chunk.diffSum += F8(Diff(subGrid.pixels[i].v, mainGrid.pixels[i].v));
            ++chunk.diffCnt;
        }

        AU diffMax = F8(0);
        for (cx AU& chunk : chunks)
        {
            cx AU diffVal = chunk.diffSum / F8(chunk.diffCnt);
            if (diffVal > diffMax)
                diffMax = diffVal;
        }

        resultDiff = F4(diffMax);
    }
    else
    {
        rete(ErrVal::NO_SUPPORT);
    }

    ifcx (IsTypeSame<T1, ColV>)
        resultDiff /= F4(0xFF);

    rets;
}

tpl<BO TUseMask, BO TDoCol, BO TDoEdges, BO TDoSsim> dfa ER ColGridCmpA2(F4& resultDiff, cx ColGridCmpA2Info& info)
{
    resultDiff = 1.0f;

    Size2<SI> gridSize;
    ifcx (TDoCol)
        gridSize = info.gridACol->size;
    else ifcx (TDoEdges)
        gridSize = info.gridAEdges->size;
    else ifcx (TDoSsim)
        gridSize = info.gridASsim->imgCol->size;
    else
        rete(ErrVal::NO_VALID);

    F4 diffCnt = (TUseMask ? F4(0) : F4(gridSize.Area()));
    F4 diffValSum = 0;
    cx AU iEnd = gridSize.Area();
    ite (i, i < iEnd)
    {
        F4 maskVal = 1.0f;
        ifcx (TUseMask)
        {
            maskVal = info.gridMask->pixels[i].v;
            diffCnt += maskVal;
        }

        ifcx (TDoCol)
        {
            cx AU diffValCol = ColHsvNCmp(info.gridACol->pixels[i], info.gridBCol->pixels[i], info.wColH, info.wColS, info.wColV);
            diffValSum += diffValCol * info.wCol * maskVal;
        }

        ifcx (TDoEdges)
        {
            cx AU diffValEdges = Diff(info.gridAEdges->pixels[i].v, info.gridBEdges->pixels[i].v);
            diffValSum += diffValEdges * info.wEdges * maskVal;
        }
    }
    ifcx (TDoSsim)
    {
        F4 diffSsim;
        ifep(ColGridCmpSsim(diffSsim, *info.gridASsim, *info.gridBSsim));
        diffValSum += diffSsim * info.wSsim * diffCnt;
    }

    cx AU wSum = (TDoCol ? info.wCol : 0.0f) + (TDoEdges ? info.wEdges : 0.0f) + (TDoSsim ? info.wSsim : 0.0f);

    if (IsNearZero(diffCnt) || IsNearZero(wSum))
        resultDiff = 1.0f;
    else
        resultDiff = diffValSum / diffCnt / wSum;

    rets;
}

tpl2 dfa ER ColGridLerp(ColGrid<T1>& out, cx ColGrid<T1>& a, cx ColGrid<T1>& b, T2 t)
{
    ifu (a.size != b.size)
        rete(ErrVal::NO_VALID);

    out.Resize(a.size);
    cx AU iEnd = SI(a.pixels.size());
    ite (i, i < iEnd)
        out.pixels[i] = Lerp<T1, T2>(a.pixels[i], b.pixels[i], t);

    rets;
}

tpl1 dfa NT ColGridEdgeMarkAll(ColGrid<T1>& colGrid, cx T1& colObj, cx T1& colMark)
{
    ite (x, x < colGrid.size.w)
    {
        AU& pixel = colGrid.pixels[x];
        if (pixel == colObj)
            pixel = colMark;
    }

    for (SI y = 1; y < colGrid.size.h - 1; ++y)
    {
        BO isIn = NO;

        for (SI x = 0; x < colGrid.size.w; ++x)
        {
            AU& pixel = colGrid.Pixel(Pos2<SI>(x, y));
            if (isIn)
            {
                if (pixel != colObj)
                {
                    colGrid.Pixel(Pos2<SI>(x - 1, y)) = colMark;
                    isIn = NO;
                }
                else
                {
                    cx AU& pixelTop = colGrid.Pixel(Pos2<SI>(x, y - 1));
                    cx AU& pixelBottom = colGrid.Pixel(Pos2<SI>(x, y + 1));
                    if (((pixelTop != colObj) && (pixelTop != colMark)) || ((pixelBottom != colObj) && (pixelBottom != colMark)))
                        pixel = colMark;
                }
            }
            else if (pixel == colObj)
            {
                pixel = colMark;
                isIn = YES;
            }
        }

        if (isIn == YES)
            colGrid.Pixel(Pos2<SI>(colGrid.size.w - 1, y)) = colMark;
    }

    {
        AU* pixels = colGrid.pixels.data() + (colGrid.size.h - 1) * colGrid.size.w;
        ite (x, x < colGrid.size.w)
        {
            AU& pixel = pixels[x];
            if (pixel == colObj)
                pixel = colMark;
        }
    }
}
tpl1 dfa NT ColGridFloodFillAt(ColGrid<T1>& colGrid, SI& fillCnt, SI fillOrigin, cx T1& colObj, cx T1& colMark, std::vector<SI>& stack)
{
    fillCnt = 0;
    stack.clear();
    stack.emplace_back(fillOrigin);

    while (!stack.empty())
    {
        cx AU curI = stack.back();
        stack.pop_back();

        AU& pixel = colGrid.pixels[curI];

        if (pixel != colObj)
            continue;

        pixel = colMark;
        ++fillCnt;

        cx AU testPixel = [&](SI testI) -> NT {
            if (colGrid.pixels[testI] == colObj)
                stack.emplace_back(testI);
        };

        testPixel(curI - 1);
        testPixel(curI + 1);
        testPixel(curI - colGrid.size.w);
        testPixel(curI + colGrid.size.w);
    }
}
tpl1 dfa NT ColGridFloodFillAt(ColGrid<T1>& colGrid, SI fillOrigin, cx T1& colObj, cx T1& colMark)
{
    ifu (colGrid.pixels[fillOrigin] != colObj)
        ret;

    SI fillCnt;
    std::vector<SI> stack;
    stack.reserve(16);

    ColGridFloodFillAt(colGrid, fillCnt, fillOrigin, colObj, colMark, stack);
}
tpl1 dfa NT ColGridFloodFillAll(ColGrid<T1>& colGrid, std::vector<SI>& fillOrigins, std::vector<SI>& fillCnts, cx T1& colObj, cx T1& colMark)
{
    fillOrigins.clear();
    fillCnts.clear();

    SI fillCnt;
    std::vector<SI> stack;
    stack.reserve(16);

    for (SI y = 1; y < colGrid.size.h - 1; ++y)
    {
        for (SI x = 1; x < colGrid.size.w - 1; ++x)
        {
            cx AU i = x + y * colGrid.size.w;
            ifl (colGrid.pixels[i] != colObj)
                continue;

            ColGridFloodFillAt(colGrid, fillCnt, i, colObj, colMark, stack);
            fillOrigins.emplace_back(i);
            fillCnts.emplace_back(fillCnt);
        }
    }
}

#ifdef PROG_SYS_WIN
tpl1 dfa ER ColGridToFileBmp(cx ColGrid<T1>& colGrid, cx CH* path)
{
    MemFile file;
    ifep(file.Open(path, NO));

    cx AU rowSize = AlignBit(colGrid.size.w * siz(ColRgb), siz(U4));
    cx AU imgSize = rowSize * colGrid.size.h;

    BITMAPINFOHEADER infoHdr = {};
    infoHdr.biSize = U4(siz(infoHdr));
    infoHdr.biWidth = S4(colGrid.size.w);
    infoHdr.biHeight = S4(colGrid.size.h);
    infoHdr.biPlanes = 1;
    infoHdr.biBitCount = sizb(ColRgb);
    infoHdr.biCompression = BI_RGB;
    infoHdr.biSizeImage = U4(imgSize);

    BITMAPFILEHEADER fileHdr = {};
    fileHdr.bfType = 0x4D42;
    fileHdr.bfOffBits = U4(siz(fileHdr) + siz(infoHdr));
    fileHdr.bfSize = fileHdr.bfOffBits + U4(imgSize);

    file.Write(&fileHdr, siz(fileHdr));
    file.Write(&infoHdr, siz(infoHdr));

    std::vector<U1> rowDat(rowSize);
    for (SI y = colGrid.size.h - 1; y >= 0; --y)
    {
        AU curIn = colGrid.pixels.data() + (y * colGrid.size.w);
        AU curOut = rowDat.data();

        ite (x, x < colGrid.size.w)
        {
            cx AU colRgb = ToType<ColRgb, T1>(*curIn++);
            *curOut++ = colRgb.b;
            *curOut++ = colRgb.g;
            *curOut++ = colRgb.r;
        }

        cx AU padSize = rowSize - colGrid.size.w * siz(ColRgb);
        MemSet(curOut, 0, padSize);

        file.Write(rowDat.data(), rowDat.size());
    }

    ifep(file.Close(YES));
    rets;
}
tpl1 dfa ER FileToColGridBmp(ColGrid<T1>& colGrid, cx CH* path)
{
    MemFile file;
    ifep(file.Open(path, YES));

    BITMAPFILEHEADER fileHdr = {};
    ifu (file.Read(&fileHdr, siz(fileHdr)) != siz(fileHdr))
        rete(ErrVal::FILE);

    ifu (fileHdr.bfType != 0x4D42)
        rete(ErrVal::FILE);

    BITMAPINFOHEADER infoHdr = {};
    ifu (file.Read(&infoHdr, siz(infoHdr)) != siz(infoHdr))
        rete(ErrVal::FILE);

    ifu (infoHdr.biPlanes != 1 || infoHdr.biBitCount != sizb(ColRgb) || infoHdr.biCompression != BI_RGB)
    {
        rete(ErrVal::NO_SUPPORT);
    }
    cx AU imgW = SI(infoHdr.biWidth);
    cx AU imgH = SI(Abs(infoHdr.biHeight));
    cx AU isTopDown = (infoHdr.biHeight < 0);
    cx AU rowSize = AlignBit(imgW * siz(ColRgb), siz(U4));

    colGrid.size.w = imgW;
    colGrid.size.h = imgH;
    colGrid.pixels.resize(imgW * imgH);

    file.CurSet(SI(fileHdr.bfOffBits));

    std::vector<U1> rowDat(rowSize);
    ite (i, i < imgH)
    {
        ifu (file.Read(rowDat.data(), rowDat.size()) != rowDat.size())
            rete(ErrVal::FILE);

        AU curIn = rowDat.data();
        AU curOut = colGrid.pixels.data() + ((isTopDown ? i : (imgH - 1 - i)) * imgW);

        ite (x, x < imgW)
        {
            ToType<T1, ColRgb>(*curOut++, ColRgb(*(curIn + 2), *(curIn + 1), *curIn));
            curIn += siz(ColRgb);
        }
    }

    ife (file.Close())
    {
        rete(ErrVal::FILE);
    }
    rets;
}
#endif
