#include "glmain.hpp"

namespace gl
{

#ifdef PROG_SYS_WIN

tpl1 dfa NT ColToCompType(ColComp& colComp, VarType& varType)
{
    ifcx (IsTypeSame<T1, ColRgb>)
    {
        colComp = ColComp::RGB;
        varType = VarType::U1;
    }
    else ifcx (IsTypeSame<T1, ColRgbN>)
    {
        colComp = ColComp::RGB;
        varType = VarType::F4;
    }
    else
    {
        static_assert(!siz(T1), "Unsupported color type");
    }
}

tpl1 dfa ER Buf::Alloc(cx std::span<T1>& buf)
{
    ifu (m_hdl == 0)
        rete(ErrVal::NO_INIT);
    if (buf.size() != 0 || m_size != 0)
    {
        glNamedBufferData(m_hdl, buf.size_bytes(), buf.data(), GL_DYNAMIC_DRAW); // we use GL_DYNAMIC_DRAW as a reasonable default
        m_size = buf.size_bytes();
    }
    rets;
}

tpl1 dfa ER Tex2::Set(cx ColGrid<T1>& colGrid, BO doLvUpd, cx Pos2<SI>& pos)
{
    ifu (m_hdl == 0)
        rete(ErrVal::NO_INIT);
    ColComp colComp;
    VarType varType;
    ColToCompType<T1>(colComp, varType);
    glTextureSubImage2D(m_hdl, 0, pos.x, pos.y, colGrid.size.w, colGrid.size.h, AsType<ColCompT>(colComp), AsType<VarTypeT>(varType), colGrid.pixels.data());
    if (m_lvCnt > 1 && doLvUpd)
        glGenerateTextureMipmap(m_hdl);
    rets;
}
tpl1 dfa ER Tex2::ParamSet(Param name, cx T1& val)
{
    ifu (m_hdl == 0)
        rete(ErrVal::NO_INIT);
    ifcx (IsTypeSame<T1, GLint>)
        glTextureParameteri(m_hdl, AsType<ParamT>(name), val);
    else
        static_assert(!siz(T1), "Unsupported param type");
    rets;
}

tpl1 dfa ER Prog::UniformSet(cx CS* name, cx T1& val) cx
{
    ifu (m_hdl != g_ctx->m_progHdl)
        rete(ErrVal::NO_INIT);
    cx AU loc = glGetUniformLocation(m_hdl, name);
    ifu (loc == -1)
        rete(ErrVal::NO_VALID);
    ifcx (IsTypeSame<T1, GLint>)
        glUniform1i(loc, val);
    else
        static_assert(!siz(T1), "Unsupported uniform type");
    rets;
}

namespace ctx
{

tpl1 dfa NT ClrCol(cx T1& col)
{
    ColRgbaN col_;
    ToType(col_, col);
    if (g_ctx->m_clrCol == col_)
        ret;
    glClearColor(col_.r, col_.g, col_.b, col_.a);
    g_ctx->m_clrCol = col_;
}

} // namespace ctx

namespace draw
{

tpl1 dfa NT ClrCol(cx T1& col)
{
    ctx::ClrCol(col);
    draw::Clr();
}

} // namespace draw

#endif

} // namespace gl
