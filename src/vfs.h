#pragma once

constexpr cx CS* VFS_MAGIC = "VFS\0";
constexpr SI VFS_MAGIC_SIZE = 4;

constexpr U1 VFS_FORMAT_NEW = 1;
constexpr U1 VFS_FORMAT_MAX = 1;

enum class VfsCompress : U4 {
	NONE = 0,
};
enum class VfsEncrypt : U4 {
	NONE = 0,
	OBFUSCATE = 1,
};

struct VfsHdr {
	CS magic[VFS_MAGIC_SIZE];
	U1 format;
	U4 flags;
	VfsCompress compress;
	VfsEncrypt encrypt;
	SI entryCnt;
	SI datSize;
};
struct VfsEntry {
	U4 flags;
	U4 attrib;
	SI datOfs;
	SI datSize;
	SI pathLen;
	CH path[PATH_LENX_MAX];
};

struct VfsNewInfo {
	U4 flags;
	VfsCompress compress;
	VfsEncrypt encrypt;
};

class Vfs {
private:
	struct FileEntry {
		FileInfo info;
		Str path;
	};
private:
	FileMem m_file;
	VfsHdr m_hdr;
	DictAvl<Str, VfsEntry> m_entryList;
private:
	dfa ER ChkInfo(cx VfsNewInfo& info) const {
		ifu (info.flags != 0) rete(ERR_NO_SUPPORT);
		ifu (info.compress != VfsCompress::NONE) rete(ERR_NO_SUPPORT);
		ifu (info.encrypt != VfsEncrypt::NONE && info.encrypt != VfsEncrypt::OBFUSCATE) rete(ERR_NO_SUPPORT);
		rets;
	}
	dfa ER ChkHdr(cx VfsHdr& hdr) const {
		ifu (MemCmp(hdr.magic, VFS_MAGIC, VFS_MAGIC_SIZE) != 0) rete(ERR_NO_VALID);
		ifu (hdr.format < VFS_FORMAT_MAX) rete(ERR_NO_SUPPORT);
		ifu (hdr.flags != 0) rete(ERR_NO_SUPPORT);
		ifu (hdr.compress != VfsCompress::NONE) rete(ERR_NO_SUPPORT);
		ifu (hdr.encrypt != VfsEncrypt::NONE && hdr.encrypt != VfsEncrypt::OBFUSCATE) rete(ERR_NO_SUPPORT);
		rets;
	}
	dfa ER ChkEntry(cx VfsEntry& entry) const {
		ifu (entry.flags != 0) rete(ERR_NO_SUPPORT);
		ifu (entry.pathLen <= 0) rete(ERR_NO_VALID);
		ifu (entry.path[entry.pathLen] != '\0') rete(ERR_NO_VALID);
		rets;
	}
	dfa SI PathOfs(cx CH* path) const {
		CH path_[PATH_LENX_MAX];
		ret PathToAbs(path_, path) + 1; // +1 for CH_PATH_DIR
	}
private:
	dfa ER FileEntryListGet(DList<FileEntry>& fileEntryList, cx CH* path) const {
		ife (DirEnum(path, -1, [](cx FileInfo& fileInfo, GA param1, GA param2) {
			DList<FileEntry>& fileEntryList_ = *(DList<FileEntry>*)param1;
			FileEntry& fileEntry = *fileEntryList_.NewLast();
			fileEntry.info = fileInfo;
			fileEntry.path = fileInfo.path;
			if ((fileEntry.info.attrib & FILE_ATTRIB_DIR) != 0) fileEntry.info.datSize = 0;
			ret YES;
			unused(param2);
		}, 0, &fileEntryList, NUL)) retep;
		rets;
	}
	dfa ER WriteHdr(FileMem& fileDst, cx VfsNewInfo& info, DList<FileEntry>& fileEntryList) const {
		cx U4 compress = U4(info.compress);
		cx U4 encrypt = U4(info.encrypt);
		cx U8 entryCnt = U8(fileEntryList.Len());
		U8 datSize = 0;
		ited (fileEntry, fileEntryList) datSize += fileEntry->info.datSize;
		ife (fileDst.Write(VFS_MAGIC, VFS_MAGIC_SIZE)) retep;
		ife (fileDst.Write(&VFS_FORMAT_NEW, siz(U1))) retep;
		ife (fileDst.Write(&info.flags, siz(U4))) retep;
		ife (fileDst.Write(&compress, siz(U4))) retep;
		ife (fileDst.Write(&encrypt, siz(U4))) retep;
		ife (fileDst.Write(&entryCnt, siz(U8))) retep;
		ife (fileDst.Write(&datSize, siz(U8))) retep;
		rets;
	}
	dfa ER WriteEntry(FileMem& fileDst, Arr<SI>& datOfsList, cx FileEntry& fileEntry, SI pathOfs) const {
		cx U8 datSize = U8(fileEntry.info.datSize);
		cx U8 pathLen = U8(fileEntry.path.Len() - pathOfs);
		CH path[PATH_LENX_MAX];
		MemObfuscate(path, fileEntry.path.Get() + pathOfs, pathLen * siz(CH));
		ife (fileDst.Write("\x00\x00\x00\x00", siz(U4))) retep;
		ife (fileDst.Write(&fileEntry.info.attrib, siz(U4))) retep;
		datOfsList.Write(fileDst.CurPos());
		ife (fileDst.Write("\x00\x00\x00\x00\x00\x00\x00\x00", siz(U8))) retep;
		ife (fileDst.Write(&datSize, siz(U8))) retep;
		ife (fileDst.Write(&pathLen, siz(U8))) retep;
		ife (fileDst.Write(path, pathLen * siz(CH))) retep;
		rets;
	}
	dfa ER WriteEntryDat(FileMem& fileDst, Arr<SI>& datOfsList, cx FileEntry& fileEntry, cx VfsNewInfo& info) const {
		cx SI datOfsHdr = datOfsList.Read();
		if (fileEntry.info.datSize == 0) rets;
		cx SI datOfsNow = fileDst.CurPos();
		cx U8 datOfsNow_ = U8(datOfsNow);
		ife (fileDst.CurSet(datOfsHdr)) retep;
		ife (fileDst.Write(&datOfsNow_, siz(U8))) retep;
		ife (fileDst.CurSet(datOfsNow)) retep;
		FileMem fileSrc;
		ife (fileSrc.OpenRead(fileEntry.path.Get())) retep;
		if (info.encrypt == VfsEncrypt::OBFUSCATE) MemObfuscate(fileSrc._Dat(), fileEntry.info.datSize);
		ife (fileDst.Write(fileSrc._Dat(), fileEntry.info.datSize)) retep;
		ife (fileSrc.Close()) retep;
		rets;
	}
	dfa ER ReadHdr(FileMem& fileSrc, VfsHdr& hdr) {
		U4 compress = 0;
		U4 encrypt = 0;
		U8 entryCnt = 0;
		U8 datSize = 0;
		ife (fileSrc.Read(hdr.magic, VFS_MAGIC_SIZE)) retep;
		ife (fileSrc.Read(&hdr.format, siz(U1))) retep;
		ife (fileSrc.Read(&hdr.flags, siz(U4))) retep;
		ife (fileSrc.Read(&compress, siz(U4))) retep;
		ife (fileSrc.Read(&encrypt, siz(U4))) retep;
		ife (fileSrc.Read(&entryCnt, siz(U8))) retep;
		ife (fileSrc.Read(&datSize, siz(U8))) retep;
		hdr.compress = VfsCompress(compress);
		hdr.encrypt = VfsEncrypt(encrypt);
		hdr.entryCnt = SI(entryCnt);
		hdr.datSize = SI(datSize);
		rets;
	}
	dfa ER ReadEntry(FileMem& fileSrc, VfsEntry& entry) {
		U8 datOfs = 0;
		U8 datSize = 0;
		U8 pathLen = 0;
		ife (fileSrc.Read(&entry.flags, siz(U4))) retep;
		ife (fileSrc.Read(&entry.attrib, siz(U4))) retep;
		ife (fileSrc.Read(&datOfs, siz(U8))) retep;
		ife (fileSrc.Read(&datSize, siz(U8))) retep;
		ife (fileSrc.Read(&pathLen, siz(U8))) retep;
		ife (fileSrc.Read(entry.path, SI(pathLen) * siz(CH))) retep;
		MemUnobfuscate(entry.path, SI(pathLen) * siz(CH));
		entry.path[SI(pathLen)] = '\0';
		entry.datOfs = SI(datOfs);
		entry.datSize = SI(datSize);
		entry.pathLen = SI(pathLen);
		rets;
	}
private:
	dfa NT Init() {
		MemSet(&m_hdr, U1(0), siz(m_hdr));
	}
public:
	dfa ER New(cx CH* dst, cx CH* src, cx VfsNewInfo& info) {
		ife (tx->ChkInfo(info)) retep;
		DList<FileEntry> fileEntryList;
		ife (tx->FileEntryListGet(fileEntryList, src)) retep;
		FileMem fileDst;
		ife (fileDst.OpenWrite(dst)) retep;
		ife (tx->WriteHdr(fileDst, info, fileEntryList)) retep;
		Arr<SI> datOfsList(fileEntryList.Len());
		ited (fileEntry, fileEntryList) ife (tx->WriteEntry(fileDst, datOfsList, *fileEntry, tx->PathOfs(src))) retep;
		datOfsList.CurClr();
		ited (fileEntry, fileEntryList) ife (tx->WriteEntryDat(fileDst, datOfsList, *fileEntry, info)) retep;
		ife (fileDst.Close()) retep;
		rets;
	}
	dfa ER Close() {
		ife (m_file.Close()) retep;
		MemSet(&m_hdr, U1(0), siz(m_hdr));
		m_entryList.Clr();
		rets;
	}
	dfa ER Open(cx CH* path) {
		ife (tx->Close()) retep;
		ife (m_file.OpenRead(path)) retep;
		ife (tx->ReadHdr(m_file, m_hdr)) retep;
		ife (tx->ChkHdr(m_hdr)) retep;
		ite (i, i < m_hdr.entryCnt) {
			VfsEntry entry = {};
			ife (tx->ReadEntry(m_file, entry)) retep;
			ife (tx->ChkEntry(entry)) retep;
			m_entryList.Add(entry.path, entry);
		}
		rets;
	}
	dfa ER Load(cx CH* path, Arr<U1>& buf) {
		buf.New(0);
		cx VfsEntry*cx entry = m_entryList[path];
		ifu (entry == NUL) rete(ERR_NO_EXIST);
		if (entry->datSize == 0) rets;
		ife (m_file.CurSet(entry->datOfs)) retep;
		buf.New(entry->datSize);
		ife (m_file.Read(buf.Ptr(), entry->datSize)) {
			buf.New(0);
			retep;
		}
		if (m_hdr.encrypt == VfsEncrypt::OBFUSCATE) MemUnobfuscate(buf.Ptr(), entry->datSize);
		rets;
	}
public:
	dfa Vfs() {
		tx->Init();
	}
	dfa ~Vfs() {
		tx->Close();
	}
};
