#include "dictionary.hpp"

#include "memory.hpp"
#include "file.hpp"

dfa ER SVarBlock::_Get(std::string& val, cx CS* var) cx
{
    cx AU& it = m_vars.find(HashFnv1a64Str(var));
    ifu (it == m_vars.end())
        rete(ErrVal::NO_EXIST);
    val = it->second;
    rets;
}
dfa NT SVarBlock::Clr()
{
    m_vars.clear();
}
dfa NT SVarBlock::Set(cx CS* var, cx std::string& val)
{
    m_vars[HashFnv1a64Str(var)] = val;
}
dfa std::string SVarBlock::Get(cx CS* var) cx
{
    std::string str;
    tx->_Get(str, var);
    ret str;
}
dfa SI SVarBlock::LoadMemCfg(DatIte<U1> dat)
{
    DatIte<U1> dat_ = dat;
    Arr<CS> buf;
    Arr<CS> buf2;
    Arr<CS> hdr;
    SI hdrLen = 0;
    while (dat_.HasNextLine())
    {
        buf.Req(dat_.NextLineLen() + 1);
        cx SI readLen = dat_.ReadLine((U1*)buf.Ptr());
        buf[readLen] = '\0';
        AU p = const_cast<CS*>(StrFind(buf.Ptr(), '='));
        if (p == NUL)
        {
            p = const_cast<CS*>(StrFind(buf.Ptr(), '['));
            if (p == NUL)
                continue;
            AU p2 = const_cast<CS*>(StrFind(p, ']'));
            if (p2 == NUL)
                continue;
            ++p;
            *p2 = '\0';
            hdrLen = StrTrimWspace(p);
            hdr.Req(hdrLen + 1);
            MemCpy(hdr.Ptr(), p, (hdrLen + 1) * siz(CS));
        }
        else
        {
            *p = '\0';
            CS* var = buf.Ptr();
            cx SI varLen = StrTrimWspace(var);
            if (var[0] == '\0' || var[0] == '#' || var[0] == ';')
                continue;
            CS* val = p + 1;
            StrTrimWspace(val);
            if (hdrLen == 0)
            {
                tx->Set(var, val);
            }
            else
            {
                buf2.Req(hdrLen + 1 + varLen + 1);
                MemCpy(buf2.Ptr(), hdr.Ptr(), hdrLen * siz(CS));
                buf2[hdrLen] = '.';
                MemCpy(buf2.Ptr() + hdrLen + 1, var, (varLen + 1) * siz(CS));
                tx->Set(buf2.Ptr(), val);
            }
        }
    }
    ret dat_.Pos() - dat.Pos();
}
dfa ER SVarBlock::LoadFileCfg(cx CH* path)
{
    std::vector<U1> fileDat;
    ife (FileToBuf(fileDat, path))
        retep;
    DatIte<U1> datIte;
    datIte.Src(fileDat.data(), fileDat.size());
    cx AU len = tx->LoadMemCfg(datIte);
    unused(len);
    rets;
}
dfa SVarBlock::SVarBlock()
{
}
dfa SVarBlock::~SVarBlock()
{
}
