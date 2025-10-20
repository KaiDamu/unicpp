#include "clipboard.hpp"

#ifdef PROG_SYS_WIN

    #include "memory.hpp"

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
