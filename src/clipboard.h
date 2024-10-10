#pragma once

dfa ER ClipbSetText(cx CH* text)
{
    ifu (OpenClipboard(NUL) == 0)
        rete(ERR_CLIPB);
    ifu (EmptyClipboard() == 0)
    {
        CloseClipboard();
        rete(ERR_CLIPB);
    }
    cx SI textSize = (StrLen(text) + 1) * siz(text[0]);
    cx HGLOBAL textGlobalObj = GlobalAlloc(GMEM_MOVEABLE, textSize);
    ifu (textGlobalObj == NUL)
    {
        CloseClipboard();
        rete(ERR_MEM_NEW);
    }
    CH* cx textGlobal = (CH*)GlobalLock(textGlobalObj);
    ifu (textGlobal == NUL)
    {
        GlobalFree(textGlobalObj);
        CloseClipboard();
        rete(ERR_LOCK);
    }
    MemSet(textGlobal, text, textSize);
    GlobalUnlock(textGlobalObj);
    ifu (SetClipboardData(CF_UNICODETEXT, textGlobalObj) == NUL)
    {
        GlobalFree(textGlobalObj);
        CloseClipboard();
        rete(ERR_CLIPB);
    }
    CloseClipboard();
    rets;
}
dfa SI ClipbGetText(CH* text, SI textLenMax)
{
    if (IsClipboardFormatAvailable(CF_UNICODETEXT) == NO)
        ret 0;
    ifu (OpenClipboard(NUL) == 0)
        ret -1;
    cx HGLOBAL textGlobalObj = GetClipboardData(CF_UNICODETEXT);
    ifu (textGlobalObj == NUL)
    {
        CloseClipboard();
        ret -1;
    }
    CH* cx textGlobal = (CH*)GlobalLock(textGlobalObj);
    ifu (textGlobal == NUL)
    {
        CloseClipboard();
        ret -1;
    }
    cx SI textSize = (StrLen(textGlobal) + 1) * siz(textGlobal[0]);
    cx SI textLen = textSize / siz(textGlobal[0]);
    ifu (textLen > textLenMax)
    {
        CloseClipboard();
        ret textLen;
    }
    MemSet(text, textGlobal, textSize);
    GlobalUnlock(textGlobalObj);
    CloseClipboard();
    ret textLen - 1;
}
