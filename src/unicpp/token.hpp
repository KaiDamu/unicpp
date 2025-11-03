#pragma once

#include "unint.hpp"

#ifdef PROG_SYS_WIN

    #include <vector>

enum class SecTokenPriviId : U4
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

dfa cx SID_* SidLocalSystem();
dfa BO SidIsSame(cx SID_& a, cx SID_& b);

class SecToken
{
  private:
    HD m_hdl;
    U1* m_sidBuf;

  public:
    dfa HD Hdl() cx;
    dfa cx SID_* SidUser();
    dfa ER OpenByProc(HD proc, U4 access, U4 attrib);
    dfa ER Close();
    dfa SecToken();
    dfa ~SecToken();
};

dfa ER ProcSecTokenPriviListGet(std::vector<LUID_AND_ATTRIBUTES_>& out, HD proc);
dfa ER ProcSecTokenPriviEnable(SecTokenPriviId priviId);
dfa ER ProcSecTokenPriviEnableAll();
dfa ER ProcSecTokenPriviIsEnabled(BO& isTrue, HD proc, SecTokenPriviId priviId);
dfa ER ProcSecTokenPriviIsExist(BO& isTrue, HD proc, SecTokenPriviId priviId);

#endif

#include "token.ipp"
