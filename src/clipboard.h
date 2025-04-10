#pragma once

#ifdef PROG_SYS_WIN

dfa ER ClipbSetText(cx CH* text)
{
    ifu (OpenClipboard(NUL) == 0)
        rete(ErrVal::CLIPB);
    ifu (EmptyClipboard() == 0)
    {
        CloseClipboard();
        rete(ErrVal::CLIPB);
    }
    cx SI textSize = (StrLen(text) + 1) * siz(text[0]);
    cx HGLOBAL objGlobal = GlobalAlloc(GMEM_MOVEABLE, textSize);
    ifu (objGlobal == NUL)
    {
        CloseClipboard();
        rete(ErrVal::MEM_NEW);
    }
    CH* cx pGlobal = (CH*)GlobalLock(objGlobal);
    ifu (pGlobal == NUL)
    {
        GlobalFree(objGlobal);
        CloseClipboard();
        rete(ErrVal::LOCK);
    }
    MemCpy(pGlobal, text, textSize);
    GlobalUnlock(objGlobal);
    ifu (SetClipboardData(CF_UNICODETEXT, objGlobal) == NUL)
    {
        GlobalFree(objGlobal);
        CloseClipboard();
        rete(ErrVal::CLIPB);
    }
    CloseClipboard();
    rets;
}
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
dfa ER ClipbGetText(std::wstring& text)
{
    text.clear();
    if (IsClipboardFormatAvailable(CF_UNICODETEXT) == NO)
        rets;
    ifu (OpenClipboard(NUL) == 0)
        rete(ErrVal::CLIPB);
    cx HGLOBAL objGlobal = GetClipboardData(CF_UNICODETEXT);
    ifu (objGlobal == NUL)
    {
        CloseClipboard();
        rete(ErrVal::CLIPB);
    }
    CH* cx pGlobal = (CH*)GlobalLock(objGlobal);
    ifu (pGlobal == NUL)
    {
        CloseClipboard();
        rete(ErrVal::LOCK);
    }
    text = pGlobal;
    GlobalUnlock(objGlobal);
    CloseClipboard();
    rets;
}

#endif
