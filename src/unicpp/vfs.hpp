#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "path.hpp"
    #include "file.hpp"
    #include "directory.hpp"
    #include "dictionary.hpp"
#endif

#ifdef PROG_SYS_WIN

cxex cx CS* VFS_MAGIC = "VFS\0";
cxex SI VFS_MAGIC_SIZE = 4;

cxex U1 VFS_FORMAT_NEW = 1;
cxex U1 VFS_FORMAT_MAX = 1;

enum class VfsCompress : U4
{
    NONE = 0,
};
enum class VfsEncrypt : U4
{
    NONE = 0,
    OBFUSCATE = 1,
};

struct VfsHdr
{
    CS magic[VFS_MAGIC_SIZE];
    U1 format;
    U4 flags;
    VfsCompress compress;
    VfsEncrypt encrypt;
    SI entryCnt;
    SI datSize;
};
struct VfsEntry
{
    U4 flags;
    U4 attrib;
    SI datOfs;
    SI datSize;
    SI pathLen;
    CH path[PATH_LENX_MAX];
};

struct VfsNewInfo
{
    U4 flags;
    VfsCompress compress;
    VfsEncrypt encrypt;
};

class Vfs
{
  private:
    struct FileEntry
    {
        FileInfoCommon info;
        std::wstring path;
    };

  private:
    MemFile m_file;
    VfsHdr m_hdr;
    DictAvl<std::wstring, VfsEntry> m_entryList;

  private:
    dfa ER ChkInfo(cx VfsNewInfo& info) cx;
    dfa ER ChkHdr(cx VfsHdr& hdr) cx;
    dfa ER ChkEntry(cx VfsEntry& entry) cx;
    dfa SI PathOfs(cx CH* path) cx;

  private:
    dfa ER FileEntryListGet(std::list<FileEntry>& fileEntryList, cx CH* path) cx;
    dfa ER WriteHdr(MemFile& fileDst, cx VfsNewInfo& info, std::list<FileEntry>& fileEntryList) cx;
    dfa ER WriteEntry(MemFile& fileDst, Arr<SI>& datOfsList, cx FileEntry& fileEntry, SI pathOfs) cx;
    dfa ER WriteEntryDat(MemFile& fileDst, Arr<SI>& datOfsList, cx FileEntry& fileEntry, cx VfsNewInfo& info) cx;
    dfa ER ReadHdr(MemFile& fileSrc, VfsHdr& hdr);
    dfa ER ReadEntry(MemFile& fileSrc, VfsEntry& entry);

  private:
    dfa NT Init();

  public:
    dfa ER New(cx CH* dst, cx CH* src, cx VfsNewInfo& info);
    dfa ER Close();
    dfa ER Open(cx CH* path);
    dfa ER Load(cx CH* path, Arr<U1>& buf);

  public:
    dfa Vfs();
    dfa ~Vfs();
};

#endif

#include "vfs.ipp"
