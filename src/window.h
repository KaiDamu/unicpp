#pragma once

#ifdef PROG_SYS_WIN

class Win
{
  private:
    struct Cache
    {
        struct Gdi
        {
            BO isValid;
            HWND winHdl;
            HDC curDC;
            HDC memDC;
            HBITMAP bmp;
            GA pixels;
            Size2<SI> size;
            Pos2<SI> ofs;
        };
        struct Dxgi
        {
            BO isValid;
            Microsoft::WRL::ComPtr<ID3D11Device> dev;
            Microsoft::WRL::ComPtr<ID3D11DeviceContext> devCtx;
            Microsoft::WRL::ComPtr<IDXGIOutputDuplication> outCpy;
            Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
            Size2<SI> size;
        };

        Gdi gdi;
        Dxgi dxgi;

        dfa Cache()
        {
            gdi.isValid = NO;
            gdi.winHdl = NUL;
            gdi.curDC = NUL;
            gdi.memDC = NUL;
            gdi.bmp = NUL;
            gdi.pixels = NUL;
            gdi.size = Size2<SI>(0, 0);
            gdi.ofs = Pos2<SI>(0, 0);

            dxgi.isValid = NO;
            dxgi.dev.Reset();
            dxgi.devCtx.Reset();
            dxgi.outCpy.Reset();
            dxgi.tex.Reset();
            dxgi.size = Size2<SI>(0, 0);
        }
        dfa ~Cache()
        {
            ifu (gdi.isValid)
            {
                if (gdi.bmp != NUL)
                    DeleteObject(gdi.bmp);
                if (gdi.memDC != NUL)
                    DeleteDC(gdi.memDC);
                if (gdi.curDC != NUL)
                    ReleaseDC(gdi.winHdl, gdi.curDC);
            }
            ifu (dxgi.isValid)
            {
                if (dxgi.tex != NUL)
                    dxgi.tex.Reset();
                if (dxgi.outCpy != NUL)
                    dxgi.outCpy.Reset();
                if (dxgi.devCtx != NUL)
                    dxgi.devCtx.Reset();
                if (dxgi.dev != NUL)
                    dxgi.dev.Reset();
            }
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
        DESKTOP = 4,
    };
    enum class CaptureMode : U1
    {
        GDI_WIN = 0,
        GDI_SCN = 1,
        DXGI_SCN = 2,
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
                rete(ErrVal::WIN);
            rets;
        }
        case SelType::TITLE_CLASS: {
            cx AU titleStr = (cx CH*)param1;
            cx AU classStr = (cx CH*)param2;
            m_hdl = FindWindowW(classStr, titleStr);
            ifu (m_hdl == NUL)
                rete(ErrVal::WIN);
            rets;
        }
        case SelType::CON: {
            m_hdl = GetConsoleWindow();
            ifu (m_hdl == NUL)
                rete(ErrVal::WIN);
            rets;
        }
        case SelType::DESKTOP: {
            m_hdl = GetDesktopWindow();
            ifu (m_hdl == NUL)
                rete(ErrVal::WIN);
            rets;
        }
        default: {
            rete(ErrVal::NO_SUPPORT);
        }
        }
    }
    dfa std::wstring TitleStrGet()
    {
        cx AU strLen = SI(GetWindowTextLengthW(m_hdl));
        if (strLen == 0)
            ret std::wstring(L"");
        cx AU strLenx = strLen + STR_EX_LEN;
        std::vector<CH> str(strLenx);
        ifu (GetWindowTextW(m_hdl, str.data(), int(strLenx)) == 0)
            ret std::wstring(L"");
        ret std::wstring(str.data());
    }
    dfa ER TitleStrSet(cx CH* str)
    {
        ifu (SetWindowTextW(m_hdl, str) == 0)
            rete(ErrVal::WIN);
        rets;
    }
    dfa std::wstring ClassStrGet()
    {
        cx AU strLen = SI(GetClassNameW(m_hdl, NUL, 0));
        if (strLen == 0)
            ret std::wstring(L"");
        cx AU strLenx = strLen + STR_EX_LEN;
        std::vector<CH> str(strLenx);
        ifu (GetClassNameW(m_hdl, str.data(), int(strLenx)) == 0)
            ret std::wstring(L"");
        ret std::wstring(str.data());
    }
    dfa ER RectOuterGet(Rect2<SI>& rect)
    {
        ife (ProcDpiAwareSet())
            retep;
        RECT rect_;
        ifu (GetWindowRect(m_hdl, &rect_) == 0)
            rete(ErrVal::WIN);
        rect.pos.x = SI(rect_.left);
        rect.pos.y = SI(rect_.top);
        rect.size.w = SI(rect_.right) - SI(rect_.left);
        rect.size.h = SI(rect_.bottom) - SI(rect_.top);
        rets;
    }
    dfa ER RectOuterSet(cx Rect2<SI>& rect)
    {
        ife (ProcDpiAwareSet())
            retep;
        cx AU doSetPos = (rect.pos.x != -1) && (rect.pos.y != -1);
        cx AU doSetSize = (rect.size.w != -1) && (rect.size.h != -1);
        if (doSetPos)
        {
            if (doSetSize)
            {
                ifu (MoveWindow(m_hdl, int(rect.pos.x), int(rect.pos.y), int(rect.size.w), int(rect.size.h), TRUE) == 0)
                    rete(ErrVal::WIN);
            }
            else
            {
                ifu (SetWindowPos(m_hdl, NUL, int(rect.pos.x), int(rect.pos.y), 0, 0, SWP_NOSIZE | SWP_NOZORDER) == 0)
                    rete(ErrVal::WIN);
            }
        }
        else
        {
            if (doSetSize)
            {
                ifu (SetWindowPos(m_hdl, NUL, 0, 0, int(rect.size.w), int(rect.size.h), SWP_NOMOVE | SWP_NOZORDER) == 0)
                    rete(ErrVal::WIN);
            }
            else
            {
                rets;
            }
        }
        rets;
    }
    dfa ER RectInnerSizeGet(Size2<SI>& size)
    {
        ife (ProcDpiAwareSet())
            retep;
        RECT rect_;
        ifu (GetClientRect(m_hdl, &rect_) == 0)
            rete(ErrVal::WIN);
        size.w = SI(rect_.right) - SI(rect_.left);
        size.h = SI(rect_.bottom) - SI(rect_.top);
        rets;
    }
    dfa ER RectInnerGet(Rect2<SI>& rect)
    {
        ife (tx->RectInnerSizeGet(rect.size))
            retep;

        // if (IsNearZero(FractionPart(ScnDpiMulGet())))
        if (YES)
        {
            POINT pt = {0, 0};
            ifu (ClientToScreen(m_hdl, &pt) == 0)
                rete(ErrVal::WIN);
            rect.pos.x = SI(pt.x);
            rect.pos.y = SI(pt.y);
        }
        else
        {
            // we have to do it manually to avoid rounding errors

            Rect2<SI> rectOuter;
            ife (tx->RectOuterGet(rectOuter))
                retep;

            RECT rectOfs = {0, 0, LONG(rect.size.w), LONG(rect.size.h)};
            ifu (AdjustWindowRectEx(&rectOfs, GetWindowLongW(m_hdl, GWL_STYLE), FALSE, GetWindowLongW(m_hdl, GWL_EXSTYLE)) == 0)
                rete(ErrVal::WIN);

            Pos2<SI> posBase;
            posBase.x = rectOuter.pos.x - SI(rectOfs.left);
            posBase.y = rectOuter.pos.y - SI(rectOfs.top);

            cx SI ofsEx = 0; // correct value varied between 0 and 6 while testing...

            rect.pos.x = posBase.x + ofsEx;
            rect.pos.y = posBase.y + ofsEx;
        }

        rets;
    }
    dfa ER RectInnerSet(cx Rect2<SI>& rect)
    {
        Rect2<SI> rectOuterCur;
        Rect2<SI> rectInnerCur;
        ifep(tx->RectOuterGet(rectOuterCur));
        ifep(tx->RectInnerGet(rectInnerCur));

        Rect2<SI> rectOuterDst;
        rectOuterDst = rect;
        rectOuterDst.pos -= rectInnerCur.pos - rectOuterCur.pos;
        rectOuterDst.size += rectOuterCur.size - rectInnerCur.size;
        ifep(tx->RectOuterSet(rectOuterDst));

        rets;
    }
    dfa BO IsValid()
    {
        ret (m_hdl != NUL) && (IsWindow(m_hdl) != 0);
    }
    dfa BO IsFocused()
    {
        ret m_hdl == GetForegroundWindow();
    }
    dfa ER Focus()
    {
        ifu (SetForegroundWindow(m_hdl) == 0)
            rete(ErrVal::WIN);
        rets;
    }

  private:
    tpl1 dfa ER _ColGridGetGdi(ColGrid<T1>& colGrid, CaptureMode captureMode)
    {
        Cache::Gdi* cache;
        ifu (m_cache == NUL || m_cache->gdi.isValid == NO)
        {
            if (m_cache == NUL)
                m_cache = std::make_unique<Cache>();
            cache = &m_cache->gdi;
            cache->isValid = YES;

            switch (captureMode)
            {
            case CaptureMode::GDI_WIN:
                cache->curDC = GetWindowDC(m_hdl);
                cache->winHdl = m_hdl;
                break;
            case CaptureMode::GDI_SCN:
                cache->curDC = GetDC(NUL);
                cache->winHdl = NUL;
                break;
            default:
                cache->curDC = NUL;
                cache->winHdl = NUL;
                break;
            }
            ifu (cache->curDC == NUL)
                rete(ErrVal::WIN);

            cache->memDC = CreateCompatibleDC(cache->curDC);
            ifu (cache->memDC == NUL)
                rete(ErrVal::WIN);
        }
        cache = &m_cache->gdi;

        Rect2<SI> rectInner;
        switch (captureMode)
        {
        case CaptureMode::GDI_WIN:
            ife (tx->RectInnerGet(rectInner))
                retep;
            break;
        case CaptureMode::GDI_SCN:
            ife (tx->RectInnerGet(rectInner))
                retep;
            break;
        default:
            rectInner = Rect2<SI>(0, 0, 0, 0);
            break;
        }

        ifu (rectInner.size != cache->size)
        {
            cache->size = rectInner.size;

            Rect2<SI> rectOuter;
            switch (captureMode)
            {
            case CaptureMode::GDI_WIN:
                ife (tx->RectOuterGet(rectOuter))
                    retep;
                cache->ofs = rectInner.pos - rectOuter.pos;
                break;
            case CaptureMode::GDI_SCN:
                cache->ofs = rectInner.pos;
                break;
            default:
                cache->ofs = Pos2<SI>(0, 0);
                break;
            }

            if (cache->bmp != NUL)
                DeleteObject(cache->bmp);

            BITMAPINFO bmpInfo = {};
            bmpInfo.bmiHeader.biSize = siz(BITMAPINFOHEADER);
            bmpInfo.bmiHeader.biWidth = S4(rectInner.size.w);
            bmpInfo.bmiHeader.biHeight = -S4(rectInner.size.h);
            bmpInfo.bmiHeader.biPlanes = 1;
            bmpInfo.bmiHeader.biBitCount = sizb(ColRgb);
            bmpInfo.bmiHeader.biCompression = BI_RGB;

            cache->bmp = CreateDIBSection(cache->curDC, &bmpInfo, DIB_RGB_COLORS, &cache->pixels, NUL, 0);
            ifu (cache->bmp == NUL || cache->pixels == NUL)
                rete(ErrVal::WIN);

            cx AU selResult = SelectObject(cache->memDC, cache->bmp);
            ifu (selResult == NUL || selResult == HGDI_ERROR)
                rete(ErrVal::WIN);
        }

        ifu (BitBlt(cache->memDC, 0, 0, rectInner.size.w, rectInner.size.h, cache->curDC, cache->ofs.x, cache->ofs.y, SRCCOPY) == 0)
            rete(ErrVal::WIN);

        colGrid.Resize(rectInner.size);

        cx AU rowSize = AlignBit(rectInner.size.w * siz(ColRgb), siz(U4));
        cx AU curInBase = (U1*)cache->pixels;
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
    tpl1 dfa ER _ColGridGetDxgi(ColGrid<T1>& colGrid, CaptureMode captureMode)
    {
        Cache::Dxgi* cache;
        ifu (m_cache == NUL || m_cache->dxgi.isValid == NO)
        {
            if (m_cache == NUL)
                m_cache = std::make_unique<Cache>();
            cache = &m_cache->dxgi;
            cache->isValid = YES;

            HRESULT result = D3D11CreateDevice(NUL, D3D_DRIVER_TYPE_HARDWARE, NUL, 0, NUL, 0, D3D11_SDK_VERSION, &cache->dev, NUL, &cache->devCtx);
            ifu (FAILED(result))
                rete(ErrVal::WIN);

            Microsoft::WRL::ComPtr<IDXGIDevice> dev;
            result = cache->dev.As(&dev);
            ifu (FAILED(result))
                rete(ErrVal::WIN);

            Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
            result = dev->GetAdapter(&adapter);
            ifu (FAILED(result))
                rete(ErrVal::WIN);

            Microsoft::WRL::ComPtr<IDXGIOutput> out;
            result = adapter->EnumOutputs(0, &out);
            ifu (FAILED(result))
                rete(ErrVal::WIN);

            Microsoft::WRL::ComPtr<IDXGIOutput1> out1;
            result = out.As(&out1);
            ifu (FAILED(result))
                rete(ErrVal::WIN);

            result = out1->DuplicateOutput(cache->dev.Get(), &cache->outCpy);
            ifu (FAILED(result))
                rete(ErrVal::WIN);

            DXGI_OUTDUPL_DESC outCpyInfo;
            cache->outCpy->GetDesc(&outCpyInfo);
            cache->size.w = outCpyInfo.ModeDesc.Width;
            cache->size.h = outCpyInfo.ModeDesc.Height;

            D3D11_TEXTURE2D_DESC texInfo = {};
            texInfo.Width = cache->size.w;
            texInfo.Height = cache->size.h;
            texInfo.MipLevels = 1;
            texInfo.ArraySize = 1;
            texInfo.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            texInfo.SampleDesc.Count = 1;
            texInfo.Usage = D3D11_USAGE_STAGING;
            texInfo.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

            result = cache->dev->CreateTexture2D(&texInfo, NUL, &cache->tex);
            ifu (FAILED(result))
                rete(ErrVal::WIN);

            ScnUpdForce(); // force screen update to ensure the first frame is captured correctly
        }
        cache = &m_cache->dxgi;

        Rect2<SI> rectScn;
        ife (ScnRectGet(rectScn))
            retep;

        ifu (rectScn.size != cache->size)
        {
            cache->size = rectScn.size;

            // TODO: implement, process screen resolution change...
        }

        Rect2<SI> rectWin;
        ife (tx->RectInnerGet(rectWin))
            retep;

        Microsoft::WRL::ComPtr<IDXGIResource> desktop;
        DXGI_OUTDUPL_FRAME_INFO frameInfo;
        HRESULT result = cache->outCpy->AcquireNextFrame(500, &frameInfo, &desktop); // 500 ms timeout, could be configurable...
        if (FAILED(result))
        {
            ifl (result == DXGI_ERROR_WAIT_TIMEOUT)
            {
                ScnUpdForce();
                result = cache->outCpy->AcquireNextFrame(500, &frameInfo, &desktop); // 500 ms timeout, could be configurable...
                ifu (FAILED(result))
                {
                    cache->outCpy->ReleaseFrame();
                    rete(ErrVal::SCN);
                }
            }
            else
            {
                cache->outCpy->ReleaseFrame();
                rete(ErrVal::SCN);
            }
        }

        Microsoft::WRL::ComPtr<ID3D11Texture2D> desktopTex;
        result = desktop.As(&desktopTex);
        ifu (FAILED(result))
        {
            cache->outCpy->ReleaseFrame();
            rete(ErrVal::SCN);
        }

        cache->devCtx->CopyResource(cache->tex.Get(), desktopTex.Get());
        cache->devCtx->Flush();

        D3D11_MAPPED_SUBRESOURCE texDat;
        result = cache->devCtx->Map(cache->tex.Get(), 0, D3D11_MAP_READ, 0, &texDat);
        ifu (FAILED(result))
        {
            cache->outCpy->ReleaseFrame();
            rete(ErrVal::SCN);
        }

        colGrid.Resize(rectWin.size);

        cx AU rowSize = texDat.RowPitch;
        cx AU curInBase = (U1*)texDat.pData;
        AU curOut = colGrid.pixels.data();

        cx AU noneRowCntPre = Max(rectScn.pos.y - rectWin.pos.y, SI(0));
        if (noneRowCntPre > 0)
        {
            cx AU noneDatCnt = noneRowCntPre * rectWin.size.w;
            MemSet(curOut, 0, noneDatCnt * siz(T1));
            curOut += noneDatCnt;
        }

        cx AU usedRowIFirst = Max(rectWin.pos.y, rectScn.pos.y) - rectScn.pos.y;
        cx AU usedRowIEnd = Min(rectWin.YEnd(), rectScn.YEnd()) - rectScn.pos.y;
        cx AU noneColumnCntPre = Max(rectScn.pos.x - rectWin.pos.x, SI(0));
        cx AU noneColumnCntPost = Max(rectWin.XEnd() - rectScn.XEnd(), SI(0));
        cx AU usedPixelCntPerRow = rectWin.size.w - noneColumnCntPre - noneColumnCntPost;

        for (SI y = usedRowIFirst; y < usedRowIEnd; ++y)
        {
            if (noneColumnCntPre > 0)
            {
                MemSet(curOut, 0, noneColumnCntPre * siz(T1));
                curOut += noneColumnCntPre;
            }

            AU curIn = (curInBase + y * rowSize) + (Max(rectWin.pos.x - rectScn.pos.x, SI(0)) * siz(ColRgba));
            cx AU curInEnd = curIn + usedPixelCntPerRow * siz(ColRgba);

            while (curIn != curInEnd)
            {
                ColRgb pixel;
                pixel.b = *curIn++;
                pixel.g = *curIn++;
                pixel.r = *curIn++;
                ++curIn; // skip alpha channel
                ToType<T1, ColRgb>(*curOut++, pixel);
            }

            if (noneColumnCntPost > 0)
            {
                MemSet(curOut, 0, noneColumnCntPost * siz(T1));
                curOut += noneColumnCntPost;
            }
        }

        cx AU noneRowCntPost = Max(rectWin.YEnd() - rectScn.YEnd(), SI(0));
        if (noneRowCntPost > 0)
        {
            cx AU noneDatCnt = noneRowCntPost * rectWin.size.w;
            MemSet(curOut, 0, noneDatCnt * siz(T1));
            curOut += noneDatCnt;
        }

        cache->devCtx->Unmap(cache->tex.Get(), 0);
        cache->outCpy->ReleaseFrame();

        rets;
    }

  public:
    tpl1 dfa ER ColGridGet(ColGrid<T1>& colGrid, CaptureMode captureMode = CaptureMode::GDI_WIN)
    {
        switch (captureMode)
        {
        case CaptureMode::GDI_WIN:
            ret tx->_ColGridGetGdi(colGrid, captureMode);
        case CaptureMode::GDI_SCN:
            ret tx->_ColGridGetGdi(colGrid, captureMode);
        case CaptureMode::DXGI_SCN:
            ret tx->_ColGridGetDxgi(colGrid, captureMode);
        default:
            break;
        }
        rete(ErrVal::NO_SUPPORT);
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

#endif
