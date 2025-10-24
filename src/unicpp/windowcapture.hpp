#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include <memory>
    #include "window.hpp"
    #include "color.hpp"
    #include "macro-off.hpp"
    #include <d3d11.h>
    #include <dxgi1_2.h>
    #include <wrl/client.h>
    #include "macro-on.hpp"
#endif

#ifdef PROG_SYS_WIN

class WinCapture
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
    Win* m_win;
    std::unique_ptr<Cache> m_cache;

  public:
    using ModeT = U1;
    enum class Mode : ModeT
    {
        GDI_WIN = 0,
        GDI_SCN = 1,
        DXGI_SCN = 2,
    };

  public:
    dfa Win* WinGet();
    dfa cx Win* WinGet() cx;
    dfa NT WinSet(Win* win);

  private:
    tpl1 dfa ER _ColGridGetGdi(ColGrid<T1>& colGrid, Mode captureMode);
    tpl1 dfa ER _ColGridGetDxgi(ColGrid<T1>& colGrid, Mode captureMode);

  public:
    tpl1 dfa ER ColGridGet(ColGrid<T1>& colGrid, Mode captureMode = Mode::GDI_WIN);

  public:
    dfa WinCapture(Win* win = NUL);
    dfa ~WinCapture();
};

#endif

#include "windowcapture.ipp"
