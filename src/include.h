#pragma once

#include <stdio.h>
#include <time.h>

#include <type_traits>
#include <new>
#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <unordered_map>
#include <algorithm>

using std::list;
using std::string;
using std::string_view;
using std::unordered_map;
using std::vector;
using std::wstring;

#include "import/stdc/type.h"
#include "import/stdc/value.h"
#include "import/stdc/function.h"

#ifdef PROG_SYS_WIN
    #define WINVER 0x0A00 // Windows 10
    #ifdef PROG_COMPILER_MSVC
        #define _WIN32_WINNT 0x0A00 // Windows 10
    #endif

    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <winternl.h>
    #include <shellapi.h>
    #include <shellscalingapi.h>
    #include <shlobj.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>

    #include "import/winapi/type.h"
    #include "import/winapi/value.h"
    #include "import/winapi/function.h"
#endif

#include "keyword.h" // ...
#include "debug.h"   // ...
#include "type.h"    // ...

#include "pre/process.h" // ...
#include "pre/console.h" // ...

#include "value.h" // ...
#include "error.h" // ...
#include "math.h"  // ...
#include "vector.h"
#include "matrix.h"      // ...
#include "char.h"        // ...
#include "typeconvert.h" // ...
#include "tuple.h"       // ...
#include "time.h"        // ...
#include "memory.h"      // ...
#include "pointer.h"     // ...
#include "array.h"       // ...
#include "random.h"      // ...
#include "string.h"      // ...
#include "path.h"        // ...
#include "color.h"
#include "process.h"    // to rework x2
#include "stack.h"      // ...
#include "list.h"       // ...
#include "queue.h"      // to rework x2
#include "event.h"      // to rework x2
#include "thread.h"     // to rework x2
#include "file.h"       // to rework x2
#include "dictionary.h" // to rework x2
#include "pipe.h"       // to rework x2
#include "directory.h"  // to rework x2
#include "sort.h"       // to rework x2
#include "test.h"       // to rework x2
#include "arg.h"        // to rework x2
#include "python.h"     // to rework x2
#include "protobuf.h"   // to rework x2
#include "aes.h"        // ...
#include "rsa.h"        // ...
#include "sha256.h"     // ...
#include "md5.h"        // ...
#include "registry.h"   // to rework x2
#include "screen.h"     // ...
#include "window.h"     // to rework x2
#include "console.h"    // to rework x2
#include "network.h"    // ...
#include "clipboard.h"  // to rework x2
#include "vfs.h"        // ...
#ifdef INCLUDE_KEYB
    #include "keyboard.h" // to rework x2
#endif
#include "cursor.h"
#include "joystick.h" // ...
#include "opengl.h"   // ...
#include "sound.h"    // ...
#include "log.h"      // ...
#ifdef INCLUDE_DUMB
    #include "dumb.h" // to rework x2
#endif

#include "main.h" // to rework x2
