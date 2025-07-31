#pragma once

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
        FileInfo info;
        std::wstring path;
    };

  private:
    MemFile m_file;
    VfsHdr m_hdr;
    DictAvl<std::wstring, VfsEntry> m_entryList;

  private:
    dfa ER ChkInfo(cx VfsNewInfo& info) cx
    {
        ifu (info.flags != 0)
            rete(ErrVal::NO_SUPPORT);
        ifu (info.compress != VfsCompress::NONE)
            rete(ErrVal::NO_SUPPORT);
        ifu (info.encrypt != VfsEncrypt::NONE && info.encrypt != VfsEncrypt::OBFUSCATE)
            rete(ErrVal::NO_SUPPORT);
        rets;
    }
    dfa ER ChkHdr(cx VfsHdr& hdr) cx
    {
        ifu (MemCmp(hdr.magic, VFS_MAGIC, VFS_MAGIC_SIZE) != 0)
            rete(ErrVal::NO_VALID);
        ifu (hdr.format < VFS_FORMAT_MAX)
            rete(ErrVal::NO_SUPPORT);
        ifu (hdr.flags != 0)
            rete(ErrVal::NO_SUPPORT);
        ifu (hdr.compress != VfsCompress::NONE)
            rete(ErrVal::NO_SUPPORT);
        ifu (hdr.encrypt != VfsEncrypt::NONE && hdr.encrypt != VfsEncrypt::OBFUSCATE)
            rete(ErrVal::NO_SUPPORT);
        rets;
    }
    dfa ER ChkEntry(cx VfsEntry& entry) cx
    {
        ifu (entry.flags != 0)
            rete(ErrVal::NO_SUPPORT);
        ifu (entry.pathLen <= 0)
            rete(ErrVal::NO_VALID);
        ifu (entry.path[entry.pathLen] != '\0')
            rete(ErrVal::NO_VALID);
        rets;
    }
    dfa SI PathOfs(cx CH* path) cx
    {
        CH path_[PATH_LENX_MAX];
        ret PathToAbspath(path_, path) + 1; // +1 for CH_PATH_DIR
    }

  private:
    dfa ER FileEntryListGet(std::list<FileEntry>& fileEntryList, cx CH* path) cx
    {
        ife (DirEnum(
                 path, -1,
                 [](cx FileInfo& fileInfo, GA param1, GA param2) {
                     std::list<FileEntry>& fileEntryList_ = *(std::list<FileEntry>*)param1;
                     FileEntry fileEntry = {};
                     fileEntry.info = fileInfo;
                     fileEntry.path = fileInfo.path;
                     if ((fileEntry.info.attrib & FILE_ATTRIB_DIR) != 0)
                         fileEntry.info.datSize = 0;
                     fileEntryList_.emplace_back(fileEntry);
                     ret YES;
                     unused(param2);
                 },
                 0, &fileEntryList, NUL))
            retep;
        rets;
    }
    dfa ER WriteHdr(MemFile& fileDst, cx VfsNewInfo& info, std::list<FileEntry>& fileEntryList) cx
    {
        cx U4 compress = U4(info.compress);
        cx U4 encrypt = U4(info.encrypt);
        cx U8 entryCnt = U8(fileEntryList.size());
        U8 datSize = 0;
        // ited (fileEntry, fileEntryList) datSize += fileEntry->info.datSize;
        for (cx AU& fileEntry : fileEntryList)
            datSize += fileEntry.info.datSize;
        fileDst.Write(VFS_MAGIC, VFS_MAGIC_SIZE);
        fileDst.Write(&VFS_FORMAT_NEW, siz(U1));
        fileDst.Write(&info.flags, siz(U4));
        fileDst.Write(&compress, siz(U4));
        fileDst.Write(&encrypt, siz(U4));
        fileDst.Write(&entryCnt, siz(U8));
        fileDst.Write(&datSize, siz(U8));
        rets;
    }
    dfa ER WriteEntry(MemFile& fileDst, Arr<SI>& datOfsList, cx FileEntry& fileEntry, SI pathOfs) cx
    {
        cx U8 datSize = U8(fileEntry.info.datSize);
        cx U8 pathLen = U8(fileEntry.path.size() - pathOfs);
        CH path[PATH_LENX_MAX];
        MemObfuscate(path, fileEntry.path.c_str() + pathOfs, pathLen * siz(CH));
        fileDst.Write("\x00\x00\x00\x00", siz(U4));
        fileDst.Write(&fileEntry.info.attrib, siz(U4));
        datOfsList.Write(fileDst.CurGet());
        fileDst.Write("\x00\x00\x00\x00\x00\x00\x00\x00", siz(U8));
        fileDst.Write(&datSize, siz(U8));
        fileDst.Write(&pathLen, siz(U8));
        fileDst.Write(path, pathLen * siz(CH));
        rets;
    }
    dfa ER WriteEntryDat(MemFile& fileDst, Arr<SI>& datOfsList, cx FileEntry& fileEntry, cx VfsNewInfo& info) cx
    {
        cx SI datOfsHdr = datOfsList.Read();
        if (fileEntry.info.datSize == 0)
            rets;
        cx SI datOfsNow = fileDst.CurGet();
        cx U8 datOfsNow_ = U8(datOfsNow);
        fileDst.CurSet(datOfsHdr);
        fileDst.Write(&datOfsNow_, siz(U8));
        fileDst.CurSet(datOfsNow);
        MemFile fileSrc;
        ife (fileSrc.Open(fileEntry.path.c_str(), YES))
            retep;
        if (info.encrypt == VfsEncrypt::OBFUSCATE)
            MemObfuscate(fileSrc.Dat(), fileEntry.info.datSize);
        fileDst.Write(fileSrc.Dat(), fileEntry.info.datSize);
        ife (fileSrc.Close())
            retep;
        rets;
    }
    dfa ER ReadHdr(MemFile& fileSrc, VfsHdr& hdr)
    {
        U4 compress = 0;
        U4 encrypt = 0;
        U8 entryCnt = 0;
        U8 datSize = 0;
        ifu (fileSrc.Read(hdr.magic, VFS_MAGIC_SIZE) != VFS_MAGIC_SIZE)
            rete(ErrVal::FILE);
        ifu (fileSrc.Read(&hdr.format, siz(U1)) != siz(U1))
            rete(ErrVal::FILE);
        ifu (fileSrc.Read(&hdr.flags, siz(U4)) != siz(U4))
            rete(ErrVal::FILE);
        ifu (fileSrc.Read(&compress, siz(U4)) != siz(U4))
            rete(ErrVal::FILE);
        ifu (fileSrc.Read(&encrypt, siz(U4)) != siz(U4))
            rete(ErrVal::FILE);
        ifu (fileSrc.Read(&entryCnt, siz(U8)) != siz(U8))
            rete(ErrVal::FILE);
        ifu (fileSrc.Read(&datSize, siz(U8)) != siz(U8))
            rete(ErrVal::FILE);
        hdr.compress = VfsCompress(compress);
        hdr.encrypt = VfsEncrypt(encrypt);
        hdr.entryCnt = SI(entryCnt);
        hdr.datSize = SI(datSize);
        rets;
    }
    dfa ER ReadEntry(MemFile& fileSrc, VfsEntry& entry)
    {
        U8 datOfs = 0;
        U8 datSize = 0;
        U8 pathLen = 0;
        ifu (fileSrc.Read(&entry.flags, siz(U4)) != siz(U4))
            rete(ErrVal::FILE);
        ifu (fileSrc.Read(&entry.attrib, siz(U4)) != siz(U4))
            rete(ErrVal::FILE);
        ifu (fileSrc.Read(&datOfs, siz(U8)) != siz(U8))
            rete(ErrVal::FILE);
        ifu (fileSrc.Read(&datSize, siz(U8)) != siz(U8))
            rete(ErrVal::FILE);
        ifu (fileSrc.Read(&pathLen, siz(U8)) != siz(U8))
            rete(ErrVal::FILE);
        ifu (fileSrc.Read(entry.path, SI(pathLen) * siz(CH)) != (SI(pathLen) * siz(CH)))
            rete(ErrVal::FILE);
        MemUnobfuscate(entry.path, SI(pathLen) * siz(CH));
        entry.path[SI(pathLen)] = '\0';
        entry.datOfs = SI(datOfs);
        entry.datSize = SI(datSize);
        entry.pathLen = SI(pathLen);
        rets;
    }

  private:
    dfa NT Init()
    {
        MemSet(&m_hdr, 0, siz(m_hdr));
    }

  public:
    dfa ER New(cx CH* dst, cx CH* src, cx VfsNewInfo& info)
    {
        ife (tx->ChkInfo(info))
            retep;
        std::list<FileEntry> fileEntryList;
        ife (tx->FileEntryListGet(fileEntryList, src))
            retep;
        MemFile fileDst;
        ife (fileDst.Open(dst, NO))
            retep;
        ife (tx->WriteHdr(fileDst, info, fileEntryList))
            retep;
        Arr<SI> datOfsList(fileEntryList.size());
        for (cx AU& fileEntry : fileEntryList)
        {
            ife (tx->WriteEntry(fileDst, datOfsList, fileEntry, tx->PathOfs(src)))
                retep;
        }
        datOfsList.CurClr();
        for (cx AU& fileEntry : fileEntryList)
        {
            ife (tx->WriteEntryDat(fileDst, datOfsList, fileEntry, info))
                retep;
        }
        ife (fileDst.Close(YES))
            retep;
        rets;
    }
    dfa ER Close()
    {
        ife (m_file.Close())
            retep;
        MemSet(&m_hdr, 0, siz(m_hdr));
        m_entryList.Clr();
        rets;
    }
    dfa ER Open(cx CH* path)
    {
        ife (tx->Close())
            retep;
        ife (m_file.Open(path))
            retep;
        ife (tx->ReadHdr(m_file, m_hdr))
            retep;
        ife (tx->ChkHdr(m_hdr))
            retep;
        ite (i, i < m_hdr.entryCnt)
        {
            VfsEntry entry = {};
            ife (tx->ReadEntry(m_file, entry))
                retep;
            ife (tx->ChkEntry(entry))
                retep;
            m_entryList.Add(entry.path, entry);
        }
        rets;
    }
    dfa ER Load(cx CH* path, Arr<U1>& buf)
    {
        buf.New(0);
        cx VfsEntry* cx entry = m_entryList[path];
        ifu (entry == NUL)
            rete(ErrVal::NO_EXIST);
        if (entry->datSize == 0)
            rets;
        m_file.CurSet(entry->datOfs);
        buf.New(entry->datSize);
        ifu (m_file.Read(buf.Ptr(), entry->datSize) != entry->datSize)
        {
            buf.New(0);
            rete(ErrVal::FILE);
        }
        if (m_hdr.encrypt == VfsEncrypt::OBFUSCATE)
            MemUnobfuscate(buf.Ptr(), entry->datSize);
        rets;
    }

  public:
    dfa Vfs()
    {
        tx->Init();
    }
    dfa ~Vfs()
    {
        tx->Close();
    }
};

#endif
