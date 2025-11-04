#pragma once

#include "type.hpp"
#ifdef PROG_SYS_WIN
    #include "glload.hpp"
    #include "color.hpp"
    #include "grtype.hpp"
#endif

namespace gl
{

#ifdef PROG_SYS_WIN

class Ctx
{
  public: // public by current design
    HGLRC m_hdl;
    HDC m_dev;
    GLuint m_bufArrHdl;
    GLuint m_bufElemArrHdl;
    GLuint m_vtxArrHdl;
    std::array<GLuint, 16> m_texHdl;
    GLuint m_progHdl;
    Rect2<SI> m_viewport;
    ColRgbaN m_clrCol;

  private:
    dfa NT _Clr();

  public:
    dfa HD Hdl() cx;
    dfa HD Dev() cx;
    dfa ER New(HD dev, SI verMajor = 4, SI verMinor = 6);
    dfa ER Del();
    dfa ER Use();

  public:
    dfa Ctx();
    dfa ~Ctx();
};

extern thdlocal Ctx* g_ctx;

dfa ER CtxUseClr();

using VarTypeT = GLenum;
enum class VarType : VarTypeT
{
    NONE = 0,
    U1 = GL_UNSIGNED_BYTE,
    F4 = GL_FLOAT,
};

using ColFormT = GLenum;
enum class ColForm : ColFormT
{
    RGB8 = GL_RGB8,
    RGBA8 = GL_RGBA8,
};

using ColCompT = GLenum;
enum class ColComp : ColCompT
{
    RGB = GL_RGB,
    RGBA = GL_RGBA,
};

dfa gl::VarType VarTypeGet(::VarType varType);
tpl1 dfa NT ColToCompType(ColComp& colComp, VarType& varType);
dfa SI Size2ToLvCnt(cx Size2<SI>& size);

class HdlBase
{
  protected:
    GLuint m_hdl;

  public:
    dfa GLuint Hdl() cx;

  public:
    dfa HdlBase();
};

class Buf : public HdlBase
{
  private:
    SI m_size;

  public:
    dfa NT Del();
    dfa ER New();
    tpl1 dfa ER Alloc(cx std::span<T1>& buf);

  public:
    dfa Buf();
    dfa ~Buf();
};
class BufArr : public Buf
{
  public:
    dfa NT Use() cx;
};
class BufElemArr : public Buf
{
  public:
    dfa NT Use() cx;
};

class VtxArr : public HdlBase
{
  private:
    SI m_attribCnt;

  public:
    dfa NT Del();
    dfa ER New();
    dfa NT Use() cx;
    dfa ER VtxLayoutSet(cx gr::VtxLayout& vtxLayout, GLuint bufArrHdl = 0);

  public:
    dfa VtxArr();
    dfa ~VtxArr();
};

class Tex : public HdlBase
{
  public:
    using TypeT = GLenum;
    enum class Type : TypeT
    {
        _2D = GL_TEXTURE_2D,
    };

    using ParamT = GLenum;
    enum class Param : ParamT
    {
        BASE_LEVEL = GL_TEXTURE_BASE_LEVEL,
        CMP_FUNC = GL_TEXTURE_COMPARE_FUNC,
        CMP_MODE = GL_TEXTURE_COMPARE_MODE,
        LOD_BIAS = GL_TEXTURE_LOD_BIAS,
        MIN_FILTER = GL_TEXTURE_MIN_FILTER,
        MAG_FILTER = GL_TEXTURE_MAG_FILTER,
        MIN_LOD = GL_TEXTURE_MIN_LOD,
        MAX_LOD = GL_TEXTURE_MAX_LOD,
        MAX_LEVEL = GL_TEXTURE_MAX_LEVEL,
        SWIZZLE_R = GL_TEXTURE_SWIZZLE_R,
        SWIZZLE_G = GL_TEXTURE_SWIZZLE_G,
        SWIZZLE_B = GL_TEXTURE_SWIZZLE_B,
        SWIZZLE_A = GL_TEXTURE_SWIZZLE_A,
        WRAP_R = GL_TEXTURE_WRAP_R,
        WRAP_S = GL_TEXTURE_WRAP_S,
        WRAP_T = GL_TEXTURE_WRAP_T,
    };

  protected:
    SI m_lvCnt;

  public:
    dfa NT Del();

  protected:
    dfa ER New(Type type);

  public:
    dfa Tex();
    dfa ~Tex();
};
class Tex2 : public Tex
{
  public:
    dfa ER New();
    dfa NT Use(SI slot = 0) cx;
    dfa ER Alloc(cx Size2<SI>& size, ColForm colForm, SI lvCnt = 0);
    tpl1 dfa ER Set(cx ColGrid<T1>& colGrid, BO doLvUpd = YES, cx Pos2<SI>& pos = Pos2<SI>(0, 0));
    tpl1 dfa ER ParamSet(Param name, cx T1& val);
    dfa ER ParamSetDefa();
};

class Shader : public HdlBase
{
  public:
    using TypeT = GLenum;
    enum class Type : TypeT
    {
        VTX = GL_VERTEX_SHADER,
        FRAG = GL_FRAGMENT_SHADER,
    };

  public:
    dfa NT Del();
    dfa ER New(cx CS* glsl, Type type);

  public:
    dfa ~Shader();
};

class Prog : public HdlBase
{
  public:
    dfa NT Del();
    dfa ER New(cx Shader* vtx, cx Shader* frag);
    dfa NT Use() cx;
    tpl1 dfa ER UniformSet(cx CS* name, cx T1& val) cx;

  public:
    dfa ~Prog();
};

class MeshGpu
{
  private:
    BufArr m_vbo;
    BufElemArr m_ebo;
    VtxArr m_vao;

  public:
    dfa NT Del();
    dfa ER New();
    dfa NT Use() cx;
    dfa ER Alloc(cx gr::MeshDat& meshDat);
};

namespace ctx
{

dfa NT Viewport(cx Rect2<SI>& rect);
tpl1 dfa NT ClrCol(cx T1& col);

} // namespace ctx

namespace draw
{

dfa NT Clr();
tpl1 dfa NT ClrCol(cx T1& col);

} // namespace draw

#endif

} // namespace gl

#include "glmain.ipp"
