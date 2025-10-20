#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include <string>
    #include <memory>
    #include "macro-off.hpp"
    #include <windows.h>
    #include <d3d11.h>
    #include <dxgi1_2.h>
    #include <wrl/client.h>
    #include "macro-on.hpp"
    #include "container.hpp"
    #include "color.hpp"
#endif

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

        dfa Cache();
        dfa ~Cache();
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
    dfa HD Hdl() cx;
    dfa ER Sel(SelType selType, CXGA param1 = NUL, CXGA param2 = NUL);
    dfa std::wstring TitleStrGet();
    dfa ER TitleStrSet(cx CH* str);
    dfa std::wstring ClassStrGet();
    dfa ER RectOuterGet(Rect2<SI>& rect);
    dfa ER RectOuterSet(cx Rect2<SI>& rect);
    dfa ER RectInnerSizeGet(Size2<SI>& size);
    dfa ER RectInnerGet(Rect2<SI>& rect);
    dfa ER RectInnerSet(cx Rect2<SI>& rect);
    dfa BO IsValid();
    dfa BO IsFocused();
    dfa ER Focus();

  private:
    tpl1 dfa ER _ColGridGetGdi(ColGrid<T1>& colGrid, CaptureMode captureMode);
    tpl1 dfa ER _ColGridGetDxgi(ColGrid<T1>& colGrid, CaptureMode captureMode);

  public:
    tpl1 dfa ER ColGridGet(ColGrid<T1>& colGrid, CaptureMode captureMode = CaptureMode::GDI_WIN);

  public:
    dfa Win(SelType selType = SelType::NONE, CXGA param1 = NUL, CXGA param2 = NUL);
    dfa ~Win();
};

#endif

#include "window.ipp"
