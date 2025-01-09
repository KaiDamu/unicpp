#pragma once

#include <stdio.h>
#include <time.h>
#include <emmintrin.h>

#include <type_traits>
#include <new>
#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <span>
#include <array>
#include <memory>
#include <limits>
#include <functional>

#include "import/stdc/type.h"
#include "import/stdc/value.h"
#include "import/stdc/function.h"

#ifdef PROG_SYS_WIN
    #define WINVER 0x0A00 // Windows 10
    #ifdef PROG_COMPILER_MSVC
        #define _WIN32_WINNT 0x0A00 // Windows 10
    #endif

    #define WIN32_LEAN_AND_MEAN
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <windows.h>
    #include <winternl.h>
    #include <shellapi.h>
    #include <shellscalingapi.h>
    #include <shlobj.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>

    #ifdef PROG_COMPILER_GCC
        #ifdef D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS
            #undef D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS
        #endif

        #ifdef D3D10_ERROR_FILE_NOT_FOUND
            #undef D3D10_ERROR_FILE_NOT_FOUND
        #endif
    #endif

    #ifdef PROG_COMPILER_GCC
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wclass-conversion"
    #endif

    #include <wrl/client.h>
    #include <d3d11.h>
    #include <dxgi1_2.h>

    #ifdef PROG_COMPILER_GCC
        #pragma GCC diagnostic pop
    #endif

    #include "import/winapi/type.h"
    #include "import/winapi/value.h"
    #include "import/winapi/function.h"
#endif

#include "keyword.h"
#include "debug.h"
#include "type.h"

#include "pre/process.h"
#include "pre/console.h"

#include "value.h"
#include "typeex.h"
#include "error.h"
#include "math.h"
#include "vector.h"
#include "matrix.h"
#include "char.h"
#include "typeconvert.h"
#include "tuple.h"
#include "time.h"
#include "memory.h"
#include "pointer.h"
#include "array.h"
#include "container.h"
#include "random.h"
#include "string.h"
#include "path.h"
#include "process.h"
#include "stack.h"
#include "list.h"
#include "queue.h"
#include "event.h"
#include "thread.h"
#include "file.h"
#include "dictionary.h"
#include "pipe.h"
#include "directory.h"
#include "sort.h"
#include "test.h"
#include "color.h"
#include "arg.h"
#include "python.h"
#include "protobuf.h"
#include "aes.h"
#include "rsa.h"
#include "sha256.h"
#include "md5.h"
#include "registry.h"
#include "screen.h"
#include "window.h"
#include "console.h"
#include "network.h"
#include "clipboard.h"
#include "vfs.h"
#include "input.h"
#include "keyboard.h"
#include "mouse.h"
#include "joystick.h"
#include "vuser.h"
#include "graphics.h"
#include "opengl.h"
#include "sound.h"
#include "log.h"
#include "pixelpath.h"
#include "bada.h"
#include "dumb.h"

#include "main.h"
