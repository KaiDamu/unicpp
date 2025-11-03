#include "windowcapture.hpp"

#ifdef PROG_SYS_WIN

    #include "screen.hpp"

tpl1 dfa ER WinCapture::_ColGridGetGdi(ColGrid<T1>& colGrid, Mode captureMode)
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
        case Mode::GDI_WIN: {
            cx AU winHdl = HWND(m_win->Hdl());
            cache->curDC = GetWindowDC(winHdl);
            cache->winHdl = winHdl;
            break;
        }
        case Mode::GDI_SCN:
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
    case Mode::GDI_WIN:
        ifep(m_win->InnerRectGet(rectInner));
        break;
    case Mode::GDI_SCN:
        ifep(m_win->InnerRectGet(rectInner));
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
        case Mode::GDI_WIN:
            ifep(m_win->OuterRectGet(rectOuter));
            cache->ofs = rectInner.pos - rectOuter.pos;
            break;
        case Mode::GDI_SCN:
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
tpl1 dfa ER WinCapture::_ColGridGetDxgi(ColGrid<T1>& colGrid, Mode captureMode)
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
    ifep(ScnRectGet(rectScn));

    ifu (rectScn.size != cache->size)
    {
        cache->size = rectScn.size;

        // TODO: implement, process screen resolution change...
    }

    Rect2<SI> rectWin;
    ifep(m_win->InnerRectGet(rectWin));

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

tpl1 dfa ER WinCapture::ColGridGet(ColGrid<T1>& colGrid, Mode captureMode)
{
    switch (captureMode)
    {
    case Mode::GDI_WIN:
        ret tx->_ColGridGetGdi(colGrid, captureMode);
    case Mode::GDI_SCN:
        ret tx->_ColGridGetGdi(colGrid, captureMode);
    case Mode::DXGI_SCN:
        ret tx->_ColGridGetDxgi(colGrid, captureMode);
    default:
        break;
    }
    rete(ErrVal::NO_SUPPORT);
}

#endif
