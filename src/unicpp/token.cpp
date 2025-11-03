#include "token.hpp"

#include "memory.hpp"
#include "process.hpp"

#ifdef PROG_SYS_WIN

dfa cx SID_* SidLocalSystem()
{
    static U1 sidBuf[siz(SID_) + 1 * siz(U4)];
    AU& sid = *(SID_*)sidBuf;
    sid.Revision = 1;
    sid.SubAuthorityCount = 1;
    sid.IdentifierAuthority.Value[0] = 0;
    sid.IdentifierAuthority.Value[1] = 0;
    sid.IdentifierAuthority.Value[2] = 0;
    sid.IdentifierAuthority.Value[3] = 0;
    sid.IdentifierAuthority.Value[4] = 0;
    sid.IdentifierAuthority.Value[5] = 5;
    sid.SubAuthority[0] = 18;
    ret&sid;
}
dfa BO SidIsSame(cx SID_& a, cx SID_& b)
{
    if (a.Revision != b.Revision || a.SubAuthorityCount != b.SubAuthorityCount)
        ret NO;
    if (MemCmp(&a.IdentifierAuthority, &b.IdentifierAuthority, siz(SID_IDENTIFIER_AUTHORITY_)) != 0)
        ret NO;
    ite (i, i < a.SubAuthorityCount)
        if (a.SubAuthority[i] != b.SubAuthority[i])
            ret NO;
    ret YES;
}

dfa HD SecToken::Hdl() cx
{
    ret m_hdl;
}
dfa cx SID_* SecToken::SidUser()
{
    if (m_sidBuf != NUL)
        MemDel(m_sidBuf);
    m_sidBuf = (U1*)MemNew(TOKEN_USER_MAX_SIZE_);
    U4 tmp;
    ifu (NtQueryInformationToken_(m_hdl, TOKEN_INFORMATION_CLASS_::TokenUser, m_sidBuf, TOKEN_USER_MAX_SIZE_, &tmp) != STATUS_SUCCESS)
    {
        MemDel(m_sidBuf);
        m_sidBuf = NUL;
        ret NUL;
    }
    ret((TOKEN_USER_*)m_sidBuf)->User.Sid;
}
dfa ER SecToken::OpenByProc(HD proc, U4 access, U4 attrib)
{
    ifu (m_hdl != NUL)
        rete(ErrVal::YES_INIT);
    cx AU status = NtOpenProcessTokenEx_(proc, access, attrib, &m_hdl);
    ifu (status != STATUS_SUCCESS)
    {
        m_hdl = NUL;
        rete(ErrVal::TOKEN);
    }
    rets;
}
dfa ER SecToken::Close()
{
    if (m_hdl == NUL)
        rets;
    cx AU status = NtClose_(m_hdl);
    ifu (status != STATUS_SUCCESS)
        rete(ErrVal::PROC);
    m_hdl = NUL;
    if (m_sidBuf != NUL)
    {
        MemDel(m_sidBuf);
        m_sidBuf = NUL;
    }
    rets;
}
dfa SecToken::SecToken() : m_hdl(NUL), m_sidBuf(NUL)
{
}
dfa SecToken::~SecToken()
{
    tx->Close();
}

dfa ER ProcSecTokenPriviListGet(std::vector<LUID_AND_ATTRIBUTES_>& out, HD proc)
{
    out.clear();
    SecToken token;
    ife (token.OpenByProc(proc, TOKEN_QUERY, 0))
        retep;
    cxex SI bufPrivisSize = siz(TOKEN_PRIVILEGES_) + 128 * siz(LUID_AND_ATTRIBUTES_); // max count assumed, as of Win11 the max is under 40
    U1 bufPrivis[bufPrivisSize];
    U4 tmp;
    cx AU status = NtQueryInformationToken_(token.Hdl(), TOKEN_INFORMATION_CLASS_::TokenPrivileges, bufPrivis, bufPrivisSize, &tmp);
    ifu (status != STATUS_SUCCESS)
        rete(ErrVal::TOKEN);
    cx AU privis = (TOKEN_PRIVILEGES_*)bufPrivis;
    out.reserve(privis->PrivilegeCount);
    ite (i, i < privis->PrivilegeCount)
        out.push_back(privis->Privileges[i]);
    rets;
}
dfa ER ProcSecTokenPriviEnable(SecTokenPriviId priviId)
{
    U1 tmp;
    ifu (RtlAdjustPrivilege_(U4(priviId), YES, NO, &tmp) != STATUS_SUCCESS)
        rete(ErrVal::TOKEN);
    rets;
}
dfa ER ProcSecTokenPriviEnableAll()
{
    std::vector<LUID_AND_ATTRIBUTES_> privis;
    ife (ProcSecTokenPriviListGet(privis, ProcCurHdl()))
        retep;
    BO isErr = NO;
    for (cx AU& privi : privis)
    {
        if ((privi.Attributes & SE_PRIVILEGE_ENABLED) || (privi.Attributes & SE_PRIVILEGE_REMOVED))
            continue;
        U1 tmp;
        cx AU status = RtlAdjustPrivilege_(privi.Luid.LowPart, YES, NO, &tmp);
        ifu (status != STATUS_SUCCESS)
            isErr = YES;
    }
    ifu (isErr)
        rete(ErrVal::NO_FULL);
    rets;
}
dfa ER ProcSecTokenPriviIsEnabled(BO& isTrue, HD proc, SecTokenPriviId priviId)
{
    isTrue = NO;
    std::vector<LUID_AND_ATTRIBUTES_> privis;
    ife (ProcSecTokenPriviListGet(privis, proc))
        retep;
    for (cx AU& privi : privis)
        if (privi.Luid.LowPart == U4(priviId))
        {
            isTrue = ((privi.Attributes & SE_PRIVILEGE_ENABLED) != 0);
            break;
        }
    rets;
}
dfa ER ProcSecTokenPriviIsExist(BO& isTrue, HD proc, SecTokenPriviId priviId)
{
    isTrue = NO;
    std::vector<LUID_AND_ATTRIBUTES_> privis;
    ife (ProcSecTokenPriviListGet(privis, proc))
        retep;
    for (cx AU& privi : privis)
        if (privi.Luid.LowPart == U4(priviId))
        {
            isTrue = ((privi.Attributes & SE_PRIVILEGE_REMOVED) == 0);
            break;
        }
    rets;
}

#endif
