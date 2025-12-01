#include "clipboard.hpp"

#ifdef PROG_SYS_WIN

    #include "error.hpp"

tpl1 dfa ER ClipbSetColGrid(cx ColGrid<T1>& colGrid)
{
    ifu (OpenClipboard(NUL) == 0)
        rete(ErrVal::CLIPB);
    ifu (EmptyClipboard() == 0)
    {
        CloseClipboard();
        rete(ErrVal::CLIPB);
    }
    BITMAPINFOHEADER bi = {};
    bi.biSize = siz(bi);
    bi.biWidth = LONG(colGrid.size.w);
    bi.biHeight = LONG(colGrid.size.h);
    bi.biPlanes = 1;
    bi.biBitCount = sizb(ColRgb);
    bi.biCompression = BI_RGB;
    cx AU rowSize = AlignBit(colGrid.size.w * siz(ColRgb), siz(U4));
    cx AU imgSize = rowSize * colGrid.size.h;
    cx AU sumSize = siz(bi) + imgSize;
    cx HGLOBAL objGlobal = GlobalAlloc(GMEM_MOVEABLE, sumSize);
    ifu (objGlobal == NUL)
    {
        CloseClipboard();
        rete(ErrVal::MEM_NEW);
    }
    U1* cx pGlobal = (U1*)GlobalLock(objGlobal);
    ifu (pGlobal == NUL)
    {
        GlobalFree(objGlobal);
        CloseClipboard();
        rete(ErrVal::LOCK);
    }
    MemCpy(pGlobal, &bi, siz(bi));
    cx AU pGlobalCol = pGlobal + siz(bi);
    cx AU* curIn = colGrid.pixels.data();
    ite (y, y < colGrid.size.h)
    {
        AU curOut = pGlobalCol + (colGrid.size.h - 1 - y) * rowSize;
        cx AU curInEnd = curIn + colGrid.size.w;
        while (curIn != curInEnd)
        {
            cx AU colRgb = ToType<ColRgb, T1>(*curIn++);
            *curOut++ = colRgb.b;
            *curOut++ = colRgb.g;
            *curOut++ = colRgb.r;
        }
    }
    GlobalUnlock(objGlobal);
    ifu (SetClipboardData(CF_DIB, objGlobal) == NUL)
    {
        GlobalFree(objGlobal);
        CloseClipboard();
        rete(ErrVal::CLIPB);
    }
    CloseClipboard();
    rets;
}
tpl1 dfa ER ClipbGetColGrid(ColGrid<T1>& colGrid)
{
    ifu (OpenClipboard(NUL) == 0)
        rete(ErrVal::CLIPB);

    cx AU clipbDat = GetClipboardData(CF_DIB);
    ifu (clipbDat == NUL)
    {
        CloseClipboard();
        rete(ErrVal::CLIPB);
    }

    cx AU pGlobal = (U1*)GlobalLock(clipbDat);
    ifu (pGlobal == NUL)
    {
        CloseClipboard();
        rete(ErrVal::LOCK);
    }

    cx AU bmpHdr = (BITMAPINFOHEADER*)pGlobal;

    cx AU isFormBgr = (bmpHdr->biBitCount == 24 && bmpHdr->biCompression == BI_RGB);
    cx AU isFormBgra = (bmpHdr->biBitCount == 32 && bmpHdr->biCompression == BI_RGB);
    cx AU isFormMask32 = (bmpHdr->biBitCount == 32 && bmpHdr->biCompression == BI_BITFIELDS);
    ifu (!(isFormBgr || isFormBgra || isFormMask32))
    {
        GlobalUnlock(clipbDat);
        CloseClipboard();
        rete(ErrVal::NO_SUPPORT);
    }

    cx AU bmpSize = Size2(SI(bmpHdr->biWidth), SI(Abs(bmpHdr->biHeight)));
    colGrid.Resize(bmpSize);

    AU pPx = pGlobal + siz(BITMAPINFOHEADER);

    // for the format 'isFormMask32'
    struct CompDat
    {
        U4 mask;
        SI shiftCnt;
        SI bitCnt;

        dfa NT Set(U4 mask)
        {
            tx->mask = mask;
            AU mask_ = mask;
            shiftCnt = SI(0);
            while ((mask_ & 1) == 0)
            {
                mask_ >>= 1;
                ++shiftCnt;
            }
            bitCnt = SI(0);
            while (mask_ != 0)
            {
                mask_ >>= 1;
                ++bitCnt;
            }
        }

        dfa U1 ChGet(U4 val)
        {
            ifu (mask == 0)
                ret 0;
            cx AU raw = (val & mask) >> shiftCnt;
            if (bitCnt == BIT_IN_BYTE)
                ret U1(raw);
            if (bitCnt > BIT_IN_BYTE)
                ret U1(raw >> (bitCnt - BIT_IN_BYTE));
            ret U1(raw << (BIT_IN_BYTE - bitCnt));
        }
    };
    enum : SI
    {
        CH_R,
        CH_G,
        CH_B,
        CH_A,
        CH_CNT,
    };

    std::array<CompDat, CH_CNT> compDats;
    compDats[CH_R].Set(0x00FF0000);
    compDats[CH_G].Set(0x0000FF00);
    compDats[CH_B].Set(0x000000FF);
    compDats[CH_A].Set(0xFF000000);
    BO hasMaskA = NO;

    if (isFormMask32)
    {
        cx AU masks = (U4*)pPx;

        compDats[CH_R].Set(masks[0]);
        compDats[CH_G].Set(masks[1]);
        compDats[CH_B].Set(masks[2]);
        if (bmpHdr->biSize >= siz(BITMAPINFOHEADER) + (4 * siz(U4)))
        {
            compDats[CH_A].Set(masks[3]);
            hasMaskA = YES;
        }

        pPx += (3 + hasMaskA) * siz(U4);
    }

    cx AU isBottomUp = (bmpHdr->biHeight > 0);
    cx AU rowSize = AlignBit(bmpSize.w * (bmpHdr->biBitCount / BIT_IN_BYTE), siz(U4));

    ite (y, y < bmpSize.h)
    {
        cx AU srcY = (isBottomUp ? (bmpSize.h - 1 - y) : y);
        AU srcCur = pPx + srcY * rowSize;

        AU dstCur = colGrid.pixels.data() + y * bmpSize.w;
        cx AU dstEnd = dstCur + bmpSize.w;

        if (isFormBgr)
        {
            while (dstCur != dstEnd)
            {
                ColRgb col;
                col.b = *srcCur++;
                col.g = *srcCur++;
                col.r = *srcCur++;
                ToType(*dstCur++, col);
            }
        }
        else if (isFormBgra)
        {
            while (dstCur != dstEnd)
            {
                ColRgba col;
                col.b = *srcCur++;
                col.g = *srcCur++;
                col.r = *srcCur++;
                col.a = *srcCur++;
                ToType(*dstCur++, col);
            }
        }
        else if (isFormMask32)
        {
            while (dstCur != dstEnd)
            {
                cx AU px = *(U4*)srcCur;
                srcCur += siz(U4);

                ColRgba col;
                col.r = compDats[CH_R].ChGet(px);
                col.g = compDats[CH_G].ChGet(px);
                col.b = compDats[CH_B].ChGet(px);
                col.a = (hasMaskA ? compDats[CH_A].ChGet(px) : 255);
                ToType(*dstCur++, col);
            }
        }
        else
        {
            // should not happen
        }
    }

    GlobalUnlock(clipbDat);
    CloseClipboard();

    rets;
}

#endif
