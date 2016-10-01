#pragma once
#include "main.h"
#include "files.h"
#define PROTOCO_VER		0xBF
#define MAX_FILENAME_LEN	128
#define MAX_DESCRIPTION_LEN	4096
#define MAX_FILE_SIZE(size) 
#define PREDEF_HEAD		"ELCOMP"
#define EXT_FORMAT_VER	0xA0
#define BLK_FORMAT_VER	0xA1

API extern const int ext_format[];
API extern const int pro_format[];
API extern const int blk_format[];

API bool ExtFormatCompact(int ver);
API bool BlkFormatCompact(int ver);
API bool ProFormatCompact(int ver);

#pragma pack(push,1)
struct block_keep {
	char _define[8] = PREDEF_HEAD;
	int _ver = BLK_FORMAT_VER;
};

struct keep_data_f {
	char description[MAX_DESCRIPTION_LEN];
	int _ver = EXT_FORMAT_VER;
	uint64_t password_lize=0;
};

struct head_f {
	char _define[8] = PREDEF_HEAD;
	int _ver = PROTOCO_VER;
	uint64_t filesize;
	int block_len;
	uint64_t blocksize;
	uint64_t keep_data_len = sizeof(keep_data_f);
};

struct file_f {
	uint64_t file_size;
	uint64_t block_size;
	char name[MAX_FILENAME_LEN];
};

struct block_f
{
	uint64_t id;
	uint64_t hash;
};
#pragma pack(pop)

struct configure_t {
	int block_len;
};
extern const configure_t def_cfg;

class API Compress {
public:
	enum VERIFY_MODE {
		LINK_TAG,
		BLK_DATA,
		LINK_AND_DATA,
	};
	Compress(string filename, vector<string> collect,const configure_t cfg=def_cfg);
	Compress(string filename,bool create=false, const configure_t cfg = def_cfg);
	Compress();
	~Compress();
	vector<string> FileList();
	void Import(string filename,bool create=false, const configure_t cfg = def_cfg);
	void ReadBuffer(string filename, uint64_t begin, uint64_t length, char *buff);
	void AddFile(string filename);
	void AddFile(vector<string>filename);
	bool VerifyFileHead();
	bool Verify(VERIFY_MODE mode,int mpsize=-1);
private:
	struct file_t {
		file_f infile;
		vector<uint64_t> bcc;
	};
	head_f head;
	vector<file_t> fat;
	vector<block_f> bat;
	keep_data_f keep;
	block_keep blk_keep;
	string basic_target_name;
	file index;
	file block;
	bool inited = false;
protected:
	uint64_t AddBuffers(const char *buff);
	void InitFileIO(char *mode);
	void UpdateIDFile();
	uint64_t Search(uint64_t id);
	void IDReadBuff(uint64_t id, char *ptr);
	inline void ReadBuff(uint64_t hash, char *ptr)
	{
		uint64_t id = Search(hash);
		if (id == -1)
			return;
		IDReadBuff(id, ptr);
	}
	bool VerifyLink(int mpsize=-1);
	bool VerifyBlock(int mpsize = -1);
};