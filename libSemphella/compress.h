#pragma once
#include "main.h"
#include "files.h"
#define MAX_FILENAME_LEN	128
#define MAX_FILE_SIZE(size) 

#pragma pack(push,1)
struct block_keep {

};
struct keep_data_f {

};
struct head_f {
	char _define[8] = "ECX";
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
	Compress(string filename, vector<string> collect,const configure_t cfg=def_cfg);
	Compress(string filename,bool create=false, const configure_t cfg = def_cfg);
	Compress();
	~Compress();
	vector<string> FileList();
	void Import(string filename,bool create=false, const configure_t cfg = def_cfg);
	void ReadBuffer(string filename, uint64_t begin, uint64_t length, char *buff);
	void AddFile(string filename);
	void AddFile(vector<string>filename);
private:
	struct file_t {
		file_f infile;
		vector<uint64_t> bcc;
	};
	head_f head;
	vector<file_t> fat;
	vector<block_f> bat;
	keep_data_f keep;
	string basic_target_name;
	file index;
	file block;
	bool inited = false;
protected:
	uint64_t AddBuffers(const char *buff);
	void InitFileIO(char *mode);
	void UpdateIDFile();
	uint64_t Search(uint64_t id);
	void ReadBuff(uint64_t hash, char *ptr);
};