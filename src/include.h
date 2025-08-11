#pragma once

#include <stdio.h>
#include <stdarg.h>
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
    #include <xinput.h>

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
    #include "import/winapi/util.h"
#endif

#include "keyword.h"
#include "debug.h"
#include "type.h"
#include "value.h"

#ifdef PROG_SYS_WIN
    #include "import/winapi/unint.h"
#endif

#include "pre/process.h"
#include "pre/console.h"

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
#include "hash.h"
#include "sort.h"
#include "string.h"
#include "path.h"
#include "stack.h"
#include "list.h"
#include "queue.h"
#include "event.h"
#include "token.h"
#include "thread.h"
#include "system.h"
#include "process.h"
#include "file.h"
#include "dictionary.h"
#include "pipe.h"
#include "directory.h"
#include "test.h"
#include "color.h"
#include "arg.h"
#include "python.h"
#include "protobuf.h"
#include "sha.h"
#include "aes.h"
#include "rsa.h"
#include "md5.h"
#include "qoi.h"
#include "registry.h"
#include "screen.h"
#include "window.h"
#include "console.h"
#include "network.h"
#include "clipboard.h"
#include "sandbox.h"
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
#include "hunic.h"
#include "dumb.h"
#include "codegen.h"

#include "main.h"

#ifdef PROG_SYS_WIN
    #include "import/winapi/unintimpl.h"
#endif
