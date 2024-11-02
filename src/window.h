#pragma once

class Win
{
  private:
    struct Cache
    {
        HWND winHdl;
        HDC curDC;
        HDC memDC;
        HBITMAP bmp;
        GA pixels;
        Size2<SI> size;
        Pos2<SI> ofs;

        dfa Cache() : winHdl(NUL), curDC(NUL), memDC(NUL), bmp(NUL), pixels(NUL), size(0, 0), ofs(0, 0)
        {
        }
        dfa ~Cache()
        {
            ifu (bmp != NUL)
                DeleteObject(bmp);
            ifu (memDC != NUL)
                DeleteDC(memDC);
            ifu (curDC != NUL)
                ReleaseDC(winHdl, curDC);
        }
    };

  private:
    HWND m_hdl;
    std::unique_ptr<Cache> m_cache;

  public:
    enum class SelType : U1
    {
        NONE = 0,
        FOCUS = 1,
        TITLE_CLASS = 2,
        CON = 3,
    };

  public:
    dfa HD Hdl() cx
    {
        ret HD(m_hdl);
    }
    dfa ER Sel(SelType selType, CXGA param1 = NUL, CXGA param2 = NUL)
    {
        switch (selType)
        {
        case SelType::NONE: {
            m_hdl = NUL;
            rets;
        }
        case SelType::FOCUS: {
            m_hdl = GetForegroundWindow();
            ifu (m_hdl == NUL)
                rete(ERR_WIN);
            rets;
        }
        case SelType::TITLE_CLASS: {
            cx AU titleStr = (cx CH*)param1;
            cx AU classStr = (cx CH*)param2;
            m_hdl = FindWindowW(classStr, titleStr);
            ifu (m_hdl == NUL)
                rete(ERR_WIN);
            rets;
        }
        case SelType::CON: {
            m_hdl = GetConsoleWindow();
            ifu (m_hdl == NUL)
                rete(ERR_WIN);
            rets;
        }
        default: {
            rete(ERR_NO_SUPPORT);
        }
        }
    }
    dfa wstring TitleStrGet()
    {
        cx AU strLen = SI(GetWindowTextLengthW(m_hdl));
        if (strLen == 0)
            ret wstring(L"");
        cx AU strLenx = strLen + STR_EX_LEN;
        vector<CH> str(strLenx);
        ifu (GetWindowTextW(m_hdl, str.data(), strLenx) == 0)
            ret wstring(L"");
        ret wstring(str.data());
    }
    dfa ER TitleStrSet(cx CH* str)
    {
        ifu (SetWindowTextW(m_hdl, str) == 0)
            rete(ERR_WIN);
        rets;
    }
    dfa wstring ClassStrGet()
    {
        cx AU strLen = SI(GetClassNameW(m_hdl, NUL, 0));
        if (strLen == 0)
            ret wstring(L"");
        cx AU strLenx = strLen + STR_EX_LEN;
        vector<CH> str(strLenx);
        ifu (GetClassNameW(m_hdl, str.data(), strLenx) == 0)
            ret wstring(L"");
        ret wstring(str.data());
    }
    dfa ER RectInnerSizeGet(Size2<SI>& size)
    {
        ife (ProcDpiAwareSet())
            retep;
        RECT rect_;
        ifu (GetClientRect(m_hdl, &rect_) == 0)
            rete(ERR_WIN);
        size.w = SI(rect_.right) - SI(rect_.left);
        size.h = SI(rect_.bottom) - SI(rect_.top);
        rets;
    }
    dfa ER RectInnerGet(Rect2<SI>& rect)
    {
        ife (tx->RectInnerSizeGet(rect.size))
            retep;
        POINT pt = {0, 0};
        ifu (ClientToScreen(m_hdl, &pt) == 0)
            rete(ERR_WIN);
        rect.pos.x = SI(pt.x);
        rect.pos.y = SI(pt.y);
        rets;
    }
    dfa ER RectOuterGet(Rect2<SI>& rect)
    {
        ife (ProcDpiAwareSet())
            retep;
        RECT rect_;
        ifu (GetWindowRect(m_hdl, &rect_) == 0)
            rete(ERR_WIN);
        rect.pos.x = SI(rect_.left);
        rect.pos.y = SI(rect_.top);
        rect.size.w = SI(rect_.right) - SI(rect_.left);
        rect.size.h = SI(rect_.bottom) - SI(rect_.top);
        rets;
    }
    tpl1 dfa ER ColGridGet(ColGrid<T1>& colGrid)
    {
        ifu (m_cache == NUL)
        {
            m_cache = std::make_unique<Cache>();

            m_cache->curDC = GetWindowDC(m_hdl);
            ifu (m_cache->curDC == NUL)
                rete(ERR_WIN);

            m_cache->memDC = CreateCompatibleDC(m_cache->curDC);
            ifu (m_cache->memDC == NUL)
                rete(ERR_WIN);
        }

        Rect2<SI> rectInner;
        ife (tx->RectInnerGet(rectInner))
            retep;

        ifu (rectInner.size != m_cache->size)
        {
            m_cache->size = rectInner.size;

            Rect2<SI> rectOuter;
            ife (tx->RectOuterGet(rectOuter))
                retep;
            m_cache->ofs = rectInner.pos - rectOuter.pos;

            if (m_cache->bmp != NUL)
                DeleteObject(m_cache->bmp);

            BITMAPINFO bmpInfo = {};
            bmpInfo.bmiHeader.biSize = siz(BITMAPINFOHEADER);
            bmpInfo.bmiHeader.biWidth = S4(rectInner.size.w);
            bmpInfo.bmiHeader.biHeight = -S4(rectInner.size.h);
            bmpInfo.bmiHeader.biPlanes = 1;
            bmpInfo.bmiHeader.biBitCount = sizb(ColRgb);
            bmpInfo.bmiHeader.biCompression = BI_RGB;

            m_cache->bmp = CreateDIBSection(m_cache->curDC, &bmpInfo, DIB_RGB_COLORS, &m_cache->pixels, NUL, 0);
            ifu (m_cache->bmp == NUL || m_cache->pixels == NUL)
                rete(ERR_WIN);

            cx AU selResult = SelectObject(m_cache->memDC, m_cache->bmp);
            ifu (selResult == NUL || selResult == HGDI_ERROR)
                rete(ERR_WIN);
        }

        ifu (BitBlt(m_cache->memDC, 0, 0, rectInner.size.w, rectInner.size.h, m_cache->curDC, m_cache->ofs.x, m_cache->ofs.y, SRCCOPY) == 0)
            rete(ERR_WIN);

        colGrid.size = rectInner.size;
        if (colGrid.pixels.size() != rectInner.size.Area())
            colGrid.pixels.resize(rectInner.size.Area());

        cx AU rowSize = AlignBit(rectInner.size.w * siz(ColRgb), siz(U4));
        cx AU curInBase = (U1*)m_cache->pixels;
        AU curOut = colGrid.pixels.data();

        ite (i, i < rectInner.size.h)
        {
            AU curIn = curInBase + i * rowSize;
            cx AU curInEnd = curIn + rectInner.size.w * siz(ColRgb);
            while (curIn != curInEnd)
            {
                ColRgb pixel;
                pixel.b = *curIn++;
                pixel.g = *curIn++;
                pixel.r = *curIn++;
                ToType<T1, ColRgb>(*curOut++, pixel);
            }
        }

        rets;
    }
    dfa BO IsValid()
    {
        ret (m_hdl != NUL) && (IsWindow(m_hdl) != 0);
    }
    dfa ER Focus()
    {
        ifu (SetForegroundWindow(m_hdl) == 0)
            rete(ERR_WIN);
        rets;
    }

  public:
    dfa Win(SelType selType = SelType::NONE, CXGA param1 = NUL, CXGA param2 = NUL) : m_hdl(NUL)
    {
        tx->Sel(selType, param1, param2);
    }
    dfa ~Win()
    {
    }
};
