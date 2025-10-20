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

#endif
