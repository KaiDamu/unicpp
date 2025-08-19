#pragma once

#ifdef PROG_SYS_WIN

namespace Ucnet
{

using TProtoVer = U2;
cxex TProtoVer PROTO_VER = 5;

using TErrType = U2;
enum class ErrType : TErrType
{
    NONE = 0,
};

using TSessionId = U8;
using TRoomId = U8;

using TPriviLv = U1;
enum class PriviLv : TPriviLv
{
    NONE = 0,  // absolutely no actions allowed
    INIT,      // can only do initialization specific actions
    NO_USER,   // can do some actions that do not require user authentication
    USER,      // can do standard registered user actions, encryption is required from this level
    HIGH_USER, // allows the user to do some administrative actions on their own account
    MODERATOR, // can influence other users, but not the system
    ADMIN,     // can do almost everything, except core system actions
    SYSTEM,    // all actions are allowed
};

struct Sec
{
    using TChecksumMode = U1;
    using TEncryptMode = U1;
    using TSrvAuthMode = U1;

    enum class ChecksumMode : TChecksumMode
    {
        NONE = 0,
        FAST,
        SECURE,
    };
    enum class EncryptMode : TEncryptMode
    {
        NONE = 0,
        FAST,
        SECURE,
    };
    enum class SrvAuthMode : TSrvAuthMode
    {
        NONE = 0,
        FAST,
        SECURE,
    };
};

} // namespace Ucnet

#endif
