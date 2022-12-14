#pragma once

#ifdef PROG_COMPILER_MSVC
	struct FILE_POSITION_INFORMATION {
		LARGE_INTEGER CurrentByteOffset;
	};
	struct FILE_STREAM_INFORMATION {
		ULONG NextEntryOffset;
		ULONG StreamNameLength;
		LARGE_INTEGER StreamSize;
		LARGE_INTEGER StreamAllocationSize;
		WCHAR StreamName[1];
	};
#endif
	