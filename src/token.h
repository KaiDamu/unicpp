#pragma once

#ifdef PROG_SYS_WIN

enum class TokenPriviId : U4
{
    None_ = 0,
    CreateToken_ = 2,                     // Required To Create A Primary Token.
    Assignprimarytoken_ = 3,              // Required To Assign The Primary Token Of A Process.
    LockMemory_ = 4,                      // Required To Lock Physical Pages In Memory.
    IncreaseQuota_ = 5,                   // Required To Increase The Quota Assigned To A Process.
    MachineAccount_ = 6,                  // Required To Create A Computer Account.
    Tcb_ = 7,                             // Required To Act As Part Of The Trusted Computer Base.
    Security_ = 8,                        // Required To Perform A Number Of Security-Related Functions, Such As Controlling And Viewing Audit Messages. // Security Operator.
    TakeOwnership_ = 9,                   // Required To Take Ownership Of An Object Without Being Granted Discretionary Access.
    LoadDriver_ = 10,                     // Required To Load Or Unload A Device Driver.
    SystemProfile_ = 11,                  // Required To Gather Profiling Information For The Entire System.
    Systemtime_ = 12,                     // Required To Modify The System Time.
    ProfSingleProcess_ = 13,              // Required To Gather Profiling Information For A Single Process.
    IncBasePriority_ = 14,                // Required To Increase The Base Priority Of A Process.
    CreatePagefile_ = 15,                 // Required To Create A Paging File.
    CreatePermanent_ = 16,                // Required To Create A Permanent Object.
    Backup_ = 17,                         // Required To Perform Backup Operations. This Privilege Causes The System To Grant All Read Access Control To Any File.
    Restore_ = 18,                        // Required To Perform Restore Operations. This Privilege Causes The System To Grant All Write Access Control To Any File.
    Shutdown_ = 19,                       // Required To Shut Down A Local System.
    Debug_ = 20,                          // Required To Debug And Adjust Memory Of Any Process, Ignoring The Dacl For The Process.
    Audit_ = 21,                          // Required To Generate Audit-Log Entries.
    SystemEnvironment_ = 22,              // Required To Modify Uefi Variables Of Systems That Use This Type Of Memory To Store Configuration Information.
    ChangeNotify_ = 23,                   // Required To Receive Notifications Of Changes To Files Or Directories And Skip All Traversal Access Checks. It Is Enabled By Default For All Users.
    RemoteShutdown_ = 24,                 // Required To Shut Down A System Using A Network Request.
    Undock_ = 25,                         // Required To Undock A Laptop.
    SyncAgent_ = 26,                      // Required For A Domain Controller To Use The Lightweight Directory Access Protocol = Ldap) Directory Synchronization Services.
    EnableDelegation_ = 27,               // Required To Mark User And Computer Accounts As Trusted For Delegation.
    ManageVolume_ = 28,                   // Required To Enable Volume Management Privileges.
    Impersonate_ = 29,                    // Required To Impersonate A Client After Authentication.
    CreateGlobal_ = 30,                   // Required To Create Named File Mapping Objects In The Global Namespace During Terminal Services Sessions. It Is Enabled By Default For All Administrators.
    TrustedCredmanAccess_ = 31,           // Required To Access Credential Manager As A Trusted Caller.
    Relabel_ = 32,                        // Required To Modify The Mandatory Integrity Level Of An Object.
    IncWorkingSet_ = 33,                  // Required To Allocate More Memory For Applications That Run In The Context Of Users.
    TimeZone_ = 34,                       // Required To Adjust The Time Zone Associated With The Computer's Internal Clock.
    CreateSymbolicLink_ = 35,             // Required To Create A Symbolic Link.
    DelegateSessionUserImpersonate_ = 36, // Required To Obtain An Impersonation Token For Another User In The Same Session.
};

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

class Token
{
  private:
    HD m_hdl;
    U1* m_sidBuf;

  public:
    dfa HD Hdl() cx
    {
        ret m_hdl;
    }
    dfa cx SID_* SidUser()
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
    dfa ER OpenByProc(HD proc, U4 access, U4 attrib)
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
    dfa ER Close()
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
    dfa Token() : m_hdl(NUL), m_sidBuf(NUL)
    {
    }
    dfa ~Token()
    {
        tx->Close();
    }
};

dfa ER ProcTokenPriviListGet(std::vector<LUID_AND_ATTRIBUTES_>& out, HD proc)
{
    out.clear();
    Token token;
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
dfa ER ProcTokenPriviEnableAll()
{
    std::vector<LUID_AND_ATTRIBUTES_> privis;
    ife (ProcTokenPriviListGet(privis, ProcCurHdl()))
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
dfa ER ProcTokenPriviIsEnabled(BO& isTrue, HD proc, TokenPriviId priviId)
{
    isTrue = NO;
    std::vector<LUID_AND_ATTRIBUTES_> privis;
    ife (ProcTokenPriviListGet(privis, proc))
        retep;
    for (cx AU& privi : privis)
        if (privi.Luid.LowPart == U4(priviId))
        {
            isTrue = ((privi.Attributes & SE_PRIVILEGE_ENABLED) != 0);
            break;
        }
    rets;
}
dfa ER ProcTokenPriviIsExist(BO& isTrue, HD proc, TokenPriviId priviId)
{
    isTrue = NO;
    std::vector<LUID_AND_ATTRIBUTES_> privis;
    ife (ProcTokenPriviListGet(privis, proc))
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
