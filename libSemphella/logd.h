#pragma once
#include "main.h"
#include "string.h"
#include "crypt.h"
#include "VER.h"
#include "files.h"
#include "debug.h"
namespace LogDaemon {
	enum EXCEPTION {
		META_DATA_CHECK_FIALD,
		META_WRITE_EXCEPTED,
		META_READ_EXCEPTED,
		DATA_FILE_EXCEPTED,
		PASSWORD_VERIFY_ERROR,
	};
#define hash getsumV2
#define crypt(pwd,buf,len) fastCrypt(buf,len,pwd)
#define BEGIN_MASX	"LGDLSS"
#ifndef CUSTMON_DATA
#define CUSTMON_DATA	16
#endif
	class API logd {
	public:
		~logd();
		inline logd() {};
		logd(string path,string password="",bool create=false);
		uint64_t getLogSize();
		void SetPassword(string password);
		bool VerifyFile();
		void Load(string path, string password = "", bool create=false);
		string GetLogDescription(uint64_t id);
		void Append(string description);
		void ReadCustmonData(char CDATA[CUSTMON_DATA]);
		void SaveCustmonData(const char CDATA[CUSTMON_DATA]);
		time_t GetLogTime(uint64_t id);
	private:
#pragma pack(push,1)
		struct META_INFO {
			//char begin[16];
			//VER meta_ver;
			bool crypted;
			uint64_t hash_code=0;
			uint64_t SectionSize;
			char CDATA[CUSTMON_DATA] = { 0x00 };
			char a='v';
			uint64_t verify;
			char b = 'e';
		};
		struct Section {
			char begin='\n';
			time_t time_log;
			uint64_t descript_len;
		};
#pragma pack(pop)
		file meta_file;
		file data_file;
		vector<Section> section;
		META_INFO meta;
		string loaded_password;
		uint64_t lp_hash;
		void ReadMeta();
		void WriteMeta();
		uint64_t GetOffSet(uint64_t id);
	};

	extern API VER meta_info;
}