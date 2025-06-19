#pragma once

inline FILE_INFORMATION_CLASS ToNative(const FILE_INFORMATION_CLASS_& val)
{
    return static_cast<FILE_INFORMATION_CLASS>(val);
}
