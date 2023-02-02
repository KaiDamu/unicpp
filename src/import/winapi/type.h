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
	struct FILE_DIRECTORY_INFORMATION {
		ULONG NextEntryOffset;
		ULONG FileIndex;
		LARGE_INTEGER CreationTime;
		LARGE_INTEGER LastAccessTime;
		LARGE_INTEGER LastWriteTime;
		LARGE_INTEGER ChangeTime;
		LARGE_INTEGER EndOfFile;
		LARGE_INTEGER AllocationSize;
		ULONG FileAttributes;
		ULONG FileNameLength;
		WCHAR FileName[1];
	};
#endif
