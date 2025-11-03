#include "glmain.hpp"

#ifdef PROG_SYS_WIN
    #include "glload.hpp"
    #include "memory.hpp"
    #include "color.hpp"
#endif

namespace gl
{

#ifdef PROG_SYS_WIN

    #ifdef PROG_COMPILER_MSVC
        #pragma comment(lib, "opengl32.lib")
    #endif

dfa NT Ctx::_Clr()
{
    m_hdl = NUL;
    m_dev = NUL;
    m_bufArrHdl = 0;
    m_bufElemArrHdl = 0;
    m_vtxArrHdl = 0;
    MemSet(m_texHdl.data(), 0, m_texHdl.size() * siz(m_texHdl[0]));
    m_progHdl = 0;
    m_viewport = Rect2<SI>(-1, -1, -1, -1);
    m_clrCol = ColRgbaN(-1, -1, -1, -1);
}
dfa HD Ctx::Hdl() cx
{
    ret HD(m_hdl);
}
dfa HD Ctx::Dev() cx
{
    ret HD(m_dev);
}
dfa ER Ctx::New(HD dev, SI verMajor, SI verMinor)
{
    ifu (m_hdl != NUL)
        rete(ErrVal::YES_INIT);

    if (verMajor == 1 && verMinor == 0)
    {
        m_hdl = wglCreateContext(HDC(dev));
        ifu (m_hdl == NUL)
            rete(ErrVal::GL);
        m_dev = HDC(dev);
    }
    else if (verMajor == 4 && verMinor == 6)
    {
        ifu (wglCreateContextAttribsARB == NUL)
            rete(ErrVal::NO_INIT);
        cx int attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,
                            int(verMajor),
                            WGL_CONTEXT_MINOR_VERSION_ARB,
                            int(verMinor),
                            WGL_CONTEXT_FLAGS_ARB,
                            WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                            WGL_CONTEXT_PROFILE_MASK_ARB,
                            WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                            0};
        m_hdl = wglCreateContextAttribsARB(HDC(dev), NUL, attribs);
        ifu (m_hdl == NUL)
            rete(ErrVal::GL);
        m_dev = HDC(dev);
    }
    else
    {
        rete(ErrVal::NO_SUPPORT);
    }

    rets;
}
dfa ER Ctx::Del()
{
    if (m_hdl == NUL)
        rets;
    if (g_ctx != NUL && g_ctx->Hdl() == m_hdl)
        ifep(CtxUseClr());
    ifu (wglDeleteContext(m_hdl) == FALSE)
        rete(ErrVal::GL);
    tx->_Clr();
    rets;
}
dfa ER Ctx::Use()
{
    if ((g_ctx != NUL && g_ctx->Hdl() == m_hdl) || (g_ctx == NUL && m_hdl == NUL))
        rets;
    ifu (wglMakeCurrent(m_dev, m_hdl) == FALSE)
        rete(ErrVal::GL);
    g_ctx = ((m_hdl == NUL) ? NUL : tx);
    rets;
}
dfa Ctx::Ctx()
{
    tx->_Clr();
}
dfa Ctx::~Ctx()
{
    tx->Del(); // error ignored
}

thdlocal Ctx* g_ctx = NUL;

dfa ER CtxUseClr()
{
    Ctx ctxNull;
    ifep(ctxNull.Use());
    rets;
}

dfa SI VarTypeSize(VarType varType)
{
    switch (varType)
    {
    case VarType::U1:
        ret 1;
    case VarType::F4:
        ret 4;
    default:
        ret -1;
    }
}
dfa SI Size2ToLvCnt(cx Size2<SI>& size)
{
    ret LenBin(Max(size.w, size.h));
}

dfa GLuint HdlBase::Hdl() cx
{
    ret m_hdl;
}
dfa HdlBase::HdlBase() : m_hdl(0)
{
}

dfa NT Buf::Del()
{
    if (m_hdl == 0)
        ret;
    glDeleteBuffers(1, &m_hdl);
    m_hdl = 0;
    m_size = 0;
}
dfa ER Buf::New()
{
    ifu (m_hdl != 0)
        rete(ErrVal::YES_INIT);
    glGenBuffers(1, &m_hdl);
    ifu (m_hdl == 0)
        rete(ErrVal::GL);
    rets;
}
dfa Buf::Buf() : m_size(0)
{
}
dfa Buf::~Buf()
{
    tx->Del();
}

dfa NT BufArr::Use() cx
{
    if (g_ctx->m_bufArrHdl == m_hdl)
        ret;
    glBindBuffer(GL_ARRAY_BUFFER, m_hdl);
    g_ctx->m_bufArrHdl = m_hdl;
}

dfa NT BufElemArr::Use() cx
{
    if (g_ctx->m_bufElemArrHdl == m_hdl)
        ret;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdl);
    g_ctx->m_bufElemArrHdl = m_hdl;
}

dfa NT VtxArr::Del()
{
    if (m_hdl == 0)
        ret;
    glDeleteVertexArrays(1, &m_hdl);
    m_hdl = 0;
}
dfa ER VtxArr::New()
{
    ifu (m_hdl != 0)
        rete(ErrVal::YES_INIT);
    glGenVertexArrays(1, &m_hdl);
    ifu (m_hdl == 0)
        rete(ErrVal::GL);
    rets;
}
dfa NT VtxArr::Use() cx
{
    if (g_ctx->m_vtxArrHdl == m_hdl)
        ret;
    glBindVertexArray(m_hdl);
    g_ctx->m_vtxArrHdl = m_hdl;
}
dfa ER VtxArr::AttribClr()
{
    ite (i, i < m_attribs.size())
        glDisableVertexArrayAttrib(m_hdl, i);
    m_attribs.clear();
    rets;
}
dfa NT VtxArr::AttribAdd(GLint cnt, VarType type)
{
    AU& attrib = m_attribs.emplace_back();
    attrib.cnt = cnt;
    attrib.type = type;
}
dfa ER VtxArr::AttribUse(GLuint bufArrHdl)
{
    GLsizei stride = 0;
    ite (i, i < m_attribs.size())
        stride += m_attribs[i].cnt * VarTypeSize(m_attribs[i].type);
    if (bufArrHdl == 0)
        bufArrHdl = g_ctx->m_bufArrHdl;
    glVertexArrayVertexBuffer(m_hdl, 0, bufArrHdl, 0, stride); // binding 0
    UA ofs = 0;
    ite (i, i < m_attribs.size())
    {
        glVertexArrayAttribFormat(m_hdl, i, m_attribs[i].cnt, AsType<VarTypeT>(m_attribs[i].type), GL_FALSE, ofs);
        glVertexArrayAttribBinding(m_hdl, i, 0); // binding 0
        glEnableVertexArrayAttrib(m_hdl, i);
        ofs += m_attribs[i].cnt * VarTypeSize(m_attribs[i].type);
    }
    rets;
}
dfa VtxArr::~VtxArr()
{
    tx->Del();
}

dfa NT Tex::Del()
{
    if (m_hdl == 0)
        ret;
    glDeleteTextures(1, &m_hdl);
    m_hdl = 0;
    m_lvCnt = 0;
}
dfa ER Tex::New(Type type)
{
    ifu (m_hdl != 0)
        rete(ErrVal::YES_INIT);
    glCreateTextures(AsType<TypeT>(type), 1, &m_hdl);
    ifu (m_hdl == 0)
        rete(ErrVal::GL);
    rets;
}
dfa Tex::Tex() : m_lvCnt(0)
{
}
dfa Tex::~Tex()
{
    tx->Del();
}

dfa ER Tex2::New()
{
    ret Tex::New(Type::_2D);
}
dfa NT Tex2::Use(SI slot) cx
{
    if (g_ctx->m_texHdl[slot] == m_hdl)
        ret;
    glBindTextureUnit(slot, m_hdl);
    g_ctx->m_texHdl[slot] = m_hdl;
}
dfa ER Tex2::Alloc(cx Size2<SI>& size, ColForm colForm, SI lvCnt)
{
    ifu (m_hdl == 0)
        rete(ErrVal::NO_INIT);
    if (lvCnt == 0)
        lvCnt = Size2ToLvCnt(size);
    glTextureStorage2D(m_hdl, lvCnt, AsType<ColFormT>(colForm), size.w, size.h);
    m_lvCnt = lvCnt;
    rets;
}
dfa ER Tex2::ParamSetDefa()
{
    ifep(tx->ParamSet(Param::MIN_FILTER, ((m_lvCnt > 1) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR)));
    ifep(tx->ParamSet(Param::MAG_FILTER, GL_LINEAR));
    ifep(tx->ParamSet(Param::WRAP_S, GL_CLAMP_TO_EDGE));
    ifep(tx->ParamSet(Param::WRAP_T, GL_CLAMP_TO_EDGE));
    rets;
}

dfa NT Shader::Del()
{
    if (m_hdl == 0)
        ret;
    glDeleteShader(m_hdl);
    m_hdl = 0;
}
dfa ER Shader::New(cx CS* glsl, Type type)
{
    ifu (m_hdl != 0)
        rete(ErrVal::YES_INIT);
    m_hdl = glCreateShader(AsType<TypeT>(type));
    ifu (m_hdl == 0)
        rete(ErrVal::GL);
    glShaderSource(m_hdl, 1, &glsl, NUL);
    glCompileShader(m_hdl);
    GLint result;
    glGetShaderiv(m_hdl, GL_COMPILE_STATUS, &result);
    ifu (result != GL_TRUE)
    {
        tx->Del();
        rete(ErrVal::GL);
    }
    rets;
}
dfa Shader::~Shader()
{
    tx->Del();
}

dfa NT Prog::Del()
{
    if (m_hdl == 0)
        ret;
    glDeleteProgram(m_hdl);
    m_hdl = 0;
}
dfa ER Prog::New(cx Shader* vtx, cx Shader* frag)
{
    ifu (m_hdl != 0)
        rete(ErrVal::YES_INIT);
    m_hdl = glCreateProgram();
    ifu (m_hdl == 0)
        rete(ErrVal::GL);
    if (vtx != NUL)
        glAttachShader(m_hdl, vtx->Hdl());
    if (frag != NUL)
        glAttachShader(m_hdl, frag->Hdl());
    glLinkProgram(m_hdl);
    GLint result;
    glGetProgramiv(m_hdl, GL_LINK_STATUS, &result);
    ifu (result != GL_TRUE)
    {
        tx->Del();
        rete(ErrVal::GL);
    }
    glValidateProgram(m_hdl);
    glGetProgramiv(m_hdl, GL_VALIDATE_STATUS, &result);
    ifu (result != GL_TRUE)
    {
        tx->Del();
        rete(ErrVal::GL);
    }
    if (vtx != NUL)
        glDetachShader(m_hdl, vtx->Hdl());
    if (frag != NUL)
        glDetachShader(m_hdl, frag->Hdl());
    rets;
}
dfa NT Prog::Use() cx
{
    if (g_ctx->m_progHdl == m_hdl)
        ret;
    glUseProgram(m_hdl);
    g_ctx->m_progHdl = m_hdl;
}
dfa Prog::~Prog()
{
    tx->Del();
}

namespace ctx
{

dfa NT Viewport(cx Rect2<SI>& rect)
{
    if (g_ctx->m_viewport == rect)
        ret;
    glViewport(rect.pos.x, rect.pos.y, rect.size.w, rect.size.h);
    g_ctx->m_viewport = rect;
}

} // namespace ctx

namespace draw
{

dfa NT Clr()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

} // namespace draw

#endif

} // namespace gl
