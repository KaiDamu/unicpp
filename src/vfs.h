#pragma once

constexpr cx CS* VFS_MAGIC = "VFS\0";
constexpr SI VFS_MAGIC_SIZE = 4;

constexpr U1 VFS_FORMAT_CREATE = 1;
constexpr U1 VFS_FORMAT_SUPPORT = 1;

constexpr U4 VFS_COMPRESS_NONE = 0;

constexpr U4 VFS_ENCRYPT_NONE = 0;
constexpr U4 VFS_ENCRYPT_OBFUSCATE = 1;

struct VfsHdr {
	CS magic[VFS_MAGIC_SIZE];
	U1 format;
	U4 flags;
	U4 compress;
	U4 encrypt;
	U4 entryCnt;
	U8 datSize;
};

struct VfsEntry {
	U4 flags;
	U4 attrib;
	U8 datSize;
	U8 datOfs;
	U8 pathLen;
	CH path[PATH_LEN_MAX];
};

/* Virtual File System (VFS) format
{VfsHdr}
	CS[4] magic = "VFS\0"
	U1 format = 1
	U4 flags
	U4 compress
		0 = none
	U4 encrypt
		0 = none
		1 = ByteObfuscate
	U4 entryCnt
	U8 datSize
{VfsEntry}
	U4 flags
	U4 attrib
	U8 datSize
	U8 datOfs
	U8 pathLen
	CHO[pathLen] path
[".vfs"]
	VfsHdr hdr
	VfsEntry[hdr.entryCnt] entryList
	U1[hdr.datSize] dat
*/
class Vfs {
private:
	FileMem m_file;
	DictAvl<Str, VfsEntry> m_fileList;
	VfsHdr m_hdr;
public:
	dfa ER Create(cx CH* dst, cx CH* src, U4 flags = 0, U4 compress = 0, U4 encrypt = 0) {
		// Get entry list
		struct Entry {
			FileInfo info;
			Str path;
		};
		DList<Entry> entryList;
		ife (DirEnum(src, -1, [](cx FileInfo& fileInfo, GA param1, GA param2) {
			unused(param2);
			DList<Entry>& entryList_ = *(DList<Entry>*)param1;
			Entry& entry = *entryList_.AddLast();
			entry.info = fileInfo;
			entry.path = fileInfo.path;
			ret YES;
		}, 0, &entryList, NUL)) retep;
		// Get entry count
		cx U4 entryCnt = U4(entryList.Len());
		// Get data size
		U8 datSize = 0;
		for (Entry* entry = entryList.GetFirst(); entry != NUL; entry = entryList.GetNext()) {
			datSize += entry->info.datSize;
		}
		// Create VFS file
		FileMem fileDst;
		ife (fileDst.OpenWrite(dst)) retep;
		// Write header
		ife (fileDst.Write(VFS_MAGIC, VFS_MAGIC_SIZE)) retep; // magic
		ife (fileDst.Write(&VFS_FORMAT_CREATE, siz(U1))) retep; // format
		ife (fileDst.Write(&flags, siz(U4))) retep; // flags
		ife (fileDst.Write(&compress, siz(U4))) retep; // compress
		ife (fileDst.Write(&encrypt, siz(U4))) retep; // encrypt
		ife (fileDst.Write(&entryCnt, siz(U4))) retep; // entryCnt
		ife (fileDst.Write(&datSize, siz(U8))) retep; // datSize
		// Get path offset
		CH src_[PATH_LEN_MAX];
		StrSet(src_, src);
		PathToAbsByWorkPath(src_);
		cx SI pathOfs = StrLen(src_) + 1; // +1 for PATH_DIR_SEPARATOR
		// Write entries
		Arr<U8> datOfsList(entryCnt);
		for (Entry* entry = entryList.GetFirst(); entry != NUL; entry = entryList.GetNext()) {
			// Write entry
			ife (fileDst.Write("\x00\x00\x00\x00", siz(U4))) retep; // flags
			ife (fileDst.Write(&entry->info.attrib, siz(U4))) retep; // attrib
			ife (fileDst.Write(&entry->info.datSize, siz(U8))) retep; // datSize
			datOfsList.Write(fileDst.CurPos());
			ife (fileDst.Write("\x00\x00\x00\x00\x00\x00\x00\x00", siz(U8))) retep; // datOfs
			CH path[PATH_LEN_MAX];
			StrSet(path, entry->path.Get() + pathOfs);
			cx U8 pathLen = entry->path.Len() + 1 - pathOfs;
			MemObfuscate(path, path, pathLen * siz(CH));
			ife (fileDst.Write(&pathLen, siz(U8))) retep; // pathLen
			ife (fileDst.Write(path, pathLen * siz(CH))) retep; // path
		}
		// Write data
		datOfsList.CurClr();
		FileMem fileSrc;
		for (Entry* entry = entryList.GetFirst(); entry != NUL; entry = entryList.GetNext()) {
			// Update data offset
			cx U8 datOfsNow = fileDst.CurPos();
			U8 datOfsHdr;
			datOfsList.Read(datOfsHdr);
			ife (fileDst.CurSet(datOfsHdr)) retep;
			ife (fileDst.Write(&datOfsNow, siz(U8))) retep;
			ife (fileDst.CurSet(datOfsNow)) retep;
			// Skip directory
			if (entry->info.attrib & FILE_ATTRIB_DIR) continue;
			// Open file
			ife (fileSrc.OpenRead(entry->path.Get())) retep;
			// Compress
			if (compress == VFS_COMPRESS_NONE);
			else rete(ERR_NO_SUPPORT);
			// Encrypt
			if (encrypt == VFS_ENCRYPT_OBFUSCATE) MemObfuscate(fileSrc._Dat(), fileSrc._Dat(), entry->info.datSize);
			else if (encrypt == VFS_ENCRYPT_NONE);
			else rete(ERR_NO_SUPPORT);
			// Write data
			ife (fileDst.Write(fileSrc._Dat(), entry->info.datSize)) retep;
			// Close file
			ife (fileSrc.Close()) retep;
		}
		// Close VFS file
		ife (fileDst.Close()) retep;
		// Done
		rets;
	}
	dfa ER Close() {
		ife (m_file.Close()) retep;
		m_fileList.Free();
		MemSet(&m_hdr, U1(0), siz(m_hdr));
		rets;
	}
	dfa ER Open(cx CH* path) {
		// Close VFS file
		ife (tx->Close()) retep;
		// Open VFS file
		ife (m_file.OpenRead(path)) retep;
		// Read header
		ife (m_file.Read(m_hdr.magic, VFS_MAGIC_SIZE)) retep;
		ife (m_file.Read(&m_hdr.format, siz(U1))) retep;
		ife (m_file.Read(&m_hdr.flags, siz(U4))) retep;
		ife (m_file.Read(&m_hdr.compress, siz(U4))) retep;
		ife (m_file.Read(&m_hdr.encrypt, siz(U4))) retep;
		ife (m_file.Read(&m_hdr.entryCnt, siz(U4))) retep;
		ife (m_file.Read(&m_hdr.datSize, siz(U8))) retep;
		// Check magic
		ifu (MemCmp(m_hdr.magic, VFS_MAGIC, VFS_MAGIC_SIZE) != 0) rete(ERR_NO_VALID);
		// Check format
		ifu (m_hdr.format < VFS_FORMAT_SUPPORT) rete(ERR_NO_SUPPORT);
		// Check flags
		ifu (m_hdr.flags != 0) rete(ERR_NO_SUPPORT);
		// Check compress
		ifu (m_hdr.compress != VFS_COMPRESS_NONE) rete(ERR_NO_SUPPORT);
		// Check encrypt
		ifu (m_hdr.encrypt != VFS_ENCRYPT_NONE && m_hdr.encrypt != VFS_ENCRYPT_OBFUSCATE) rete(ERR_NO_SUPPORT);
		// Read entries
		ite (i, i < m_hdr.entryCnt) {
			// Read entry
			VfsEntry entry = {};
			ife (m_file.Read(&entry.flags, siz(U4))) retep;
			ife (m_file.Read(&entry.attrib, siz(U4))) retep;
			ife (m_file.Read(&entry.datSize, siz(U8))) retep;
			ife (m_file.Read(&entry.datOfs, siz(U8))) retep;
			ife (m_file.Read(&entry.pathLen, siz(U8))) retep;
			ife (m_file.Read(entry.path, entry.pathLen * siz(entry.path[0]))) retep;
			MemUnobfuscate(entry.path, entry.path, entry.pathLen * siz(entry.path[0]));
			// Check flags
			ifu (entry.flags != 0) rete(ERR_NO_SUPPORT);
			// Check path
			ifu (entry.pathLen == 0 || entry.path[entry.pathLen - 1] != '\0') rete(ERR_NO_VALID);
			// Add entry
			m_fileList.Add(entry.path, entry);
		}
		// Done
		rets;
	}
	dfa ER Load(cx CH* path, Arr<U1>& buf) {
		cx VfsEntry* entry = m_fileList[path];
		ifu (entry == NUL) rete(ERR_NO_EXIST);
		ife (m_file.CurSet(entry->datOfs)) retep;
		buf.New(entry->datSize);
		if (entry->attrib & FILE_ATTRIB_DIR) rets;
		ife (m_file.Read(buf.Ptr(), entry->datSize)) retep;
		if (m_hdr.encrypt == VFS_ENCRYPT_OBFUSCATE) MemUnobfuscate(buf.Ptr(), buf.Ptr(), entry->datSize);
		rets;
	}
public:
	dfa Vfs() {
		tx->Close();
	}
	dfa ~Vfs() {
		tx->Close();
	}
};
