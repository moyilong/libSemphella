#pragma once
#include <libSemphella/main.h>
#ifndef __LINUX__
#ifdef LIBERT2_EXPORTS
#define E2API	__declspec(dllexport)
#else
#define E2API __declspec(dllimport)
#endif
#else
#define E2API
#endif

namespace ERT2 {
#define null NULL
	typedef void(*algorthim_type)(char *data,const uint64_t len,const bool decode);
	typedef uint64_t(*hash_type)(const char *data, uint64_t len);
	typedef void(*procd)(const string password);
	struct E2API algorthim {
		algorthim_type algr = null;
		hash_type hash = null;
		procd pwd_init = null;
		int algr_id;
		string algr_name;
		algorthim(const int _id,const string name,algorthim_type _algr, hash_type _hash, procd _pwd = null);
	};
#define AlgorthimReg(id,name,algr,hash,pwd) \
	extern ERT2::algorthim __algr_registry_##id(id,name,algr,hash,pwd);
#define ERT2_VER	0x1F00
	struct E2API configure {
		bool crypt_filename;
		string padding_password;
		uint64_t password_hash;
		void SetPaswsword(const string password);
		int algrid = 0;
		void SetAlgrID(const int id);
		int block_size;
	};
	bool CreatePackage(const vector<string>filelist, const string output,const configure cfg);
	bool ExtrectFile(const uint64_t fid, const string fname);
}