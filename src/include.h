#pragma once

#include <stdio.h>
#include <time.h>
#ifdef PROG_SYS_WIN
	#define WINVER 0x0A00 // Windows 10
	#ifdef PROG_COMPILER_MSVC
		#define _WIN32_WINNT 0x0A00 // Windows 10
	#endif

	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <winternl.h>
	#include <shellapi.h>
	#include <shlobj.h>

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
#include "error.h"
#include "math.h"
#include "typeconvert.h"
#include "vector.h"
#include "matrix.h"
#include "char.h"
#include "tuple.h"
#include "time.h"
#include "memory.h"
#include "pointer.h"
#include "array.h"
#include "random.h"
#include "string.h"
#include "path.h"
#include "process.h" // to rework
#include "stack.h"
#include "list.h"
#include "queue.h" // to rework
#include "event.h" // to rework
#include "thread.h" // to rework
#include "file.h" // to rework
#include "dictionary.h" // to rework
#include "pipe.h" // to rework
#include "directory.h" // to rework
#include "sort.h" // to rework
#include "test.h" // to rework
#include "arg.h" // to rework
#include "python.h" // to rework
#include "protobuf.h" // to rework
#include "aes.h"
#include "rsa.h"
#include "sha256.h"
#include "md5.h"
#include "registry.h" // to rework
#include "screen.h"
#include "window.h" // to rework
#include "console.h" // to rework
#include "network.h"
#include "clipboard.h" // to rework
#include "vfs.h"
#ifdef INCLUDE_KEYB
	#include "keyboard.h" // to rework
#endif
#include "cursor.h" // to rework
#include "joystick.h"
#include "opengl.h"
#include "sound.h"
#include "log.h"
#ifdef INCLUDE_DUMB
	#include "dumb.h" // to rework
#endif

#include "main.h" // to rework
