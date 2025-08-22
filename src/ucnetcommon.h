#pragma once

#ifdef PROG_SYS_WIN

namespace Ucnet
{

using ProtoVerT = U2;
cxex ProtoVerT PROTO_VER = 5;

using ErrTypeT = U2;
enum class ErrType : ErrTypeT
{
    NONE = 0,
};

using SessionIdT = U8;
using RoomIdT = U8;

using PriviLvT = U1;
enum class PriviLv : PriviLvT
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

namespace Sec
{

using ChecksumModeT = U1;
enum class ChecksumMode : ChecksumModeT
{
    NONE = 0,
    FAST,
    SECURE,
};

using EncryptModeT = U1;
enum class EncryptMode : EncryptModeT
{
    NONE = 0,
    FAST,
    SECURE,
};

using SrvAuthModeT = U1;
enum class SrvAuthMode : SrvAuthModeT
{
    NONE = 0,
    FAST,
    SECURE,
};

} // namespace Sec

} // namespace Ucnet

#endif
