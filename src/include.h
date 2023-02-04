#pragma once

#include <stdio.h>
#include <time.h>
#ifdef PROG_SYS_WIN
	#define WINVER 0x0A00 // Windows 10
	#ifdef PROG_COMPILER_MSVC
		#define _WIN32_WINNT 0x0A00 // Windows 10
	#endif

	#include <windows.h>
	#include <winternl.h>

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
#include "typeconvert.h"
#include "math.h"
#include "vector.h"
#include "matrix.h"
#include "char.h"
#include "tuple.h"
#include "dictionary.h"
#include "time.h"
#include "random.h"
#include "memory.h"
#include "pointer.h"
#include "array.h"
#include "string.h"
#include "path.h"
#include "process.h"
#include "stack.h"
#include "list.h"
#include "queue.h"
#include "event.h"
#include "thread.h"
#include "console.h"
#include "file.h"
#include "pipe.h"
#include "directory.h"
#include "sort.h"
#include "test.h"
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
#include "network.h"
#include "clipboard.h"
#ifdef INCLUDE_KEYB
	#include "keyboard.h"
#endif
#include "cursor.h"
#include "joystick.h"
#include "opengl.h"
#include "sound.h"
#include "log.h"
#ifdef INCLUDE_DUMB
	#include "dumb.h"
#endif

#include "main.h"
