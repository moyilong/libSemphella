#pragma once
#include "stdafx.h"
#define level 6
#define level_compact 6
#define DESIGN_KEEP_SIZE	4096
#define EXT_SIZE	sizeof(uint64_t)
#undef PRELOAD_HEAD
#define CLASS_TYPED
#include "dll.h"
#pragma pack(push,1)
struct  HEAD {
	char account_level = level;
	char algrthom;
	uint64_t sum;
	//uint64_t matrix_sum;
	char ext[EXT_SIZE];
	uint64_t password_sum;
	uint64_t bs = bs;

	//Program Funcation
	inline bool check();
	void init();
	void reset_ext(bool faild_test = false);
	inline HEAD()
	{
		init();
	}
};

struct EXT {
	const char start_bit = 0xB0;
	uint64_t length;
	uint64_t checksum;
	const char stop_bit = 0xB1;
	char buff[DESIGN_KEEP_SIZE - sizeof(uint64_t) * 2];
	const char stop_all = 0xBA;
};
#pragma pack(pop)
#define EXT_SUPPORT	0
#define EXT_FHANDLE	1
#define EXT_EXTABLE	2
#define EXT_ENDFLAG	EXT_SIZE-1

#define APOLL_SIZE	xsize
#define APOLL_IDMAX	xsize

#define ext_support_lab	(char)'S'
#define ext_end_lab	(char)'E'

#define DEFAULT_FHANDLE 0
#define DEFAULT_ALGRTHOM_TYPE	10

extern EXT ex;

#define cp2 debug

inline bool HEAD::check()
{
	cp2 << "Head Level:" << (int)account_level << endl;
	cp2 << "Algrthom Type:" << (int)algrthom << endl;
	cp2 << "File Handle Type:" << (int)ext[EXT_FHANDLE] << endl;
	cp2 << "File Check Sum:" << sum << endl;
	cp2 << "Password Check Sum:" << password_sum << endl;
	cp2 << "Block Length:" << bs << endl;
	cp2 << "Extenision Table Size:" << EXT_SIZE << endl;
	if (account_level > level || account_level < level_compact)
	{
		cout << "Error: HEAD Protoco Check Fail d!" << endl;
		cout << "Unsupported Level:" << (int)account_level << endl;
		cout << "Compact of:" << level_compact << " max  " << level << endl;
		return false;
	}
	if (ext[EXT_SUPPORT] != ext_support_lab || ext[EXT_ENDFLAG] != ext_end_lab)
	{
		cout << "Warring: HEAD Protoco Extend Table Check Faild!" << endl;
		cout << " Convert to Default Status!" << endl;
		cout << hex << "CheckLab:" << (int)(char)ext_support_lab << " != " << (int)(char)ext[EXT_SUPPORT] << endl << oct;
		reset_ext(true);
	}
	return true;
}
inline void HEAD::reset_ext(bool faild_test)
{
	cp2 << "Resetting Extension Table..." << endl;
	memset(ext, 0, sizeof(ext));
	ext[EXT_FHANDLE] = DEFAULT_FHANDLE;
	ext[EXT_SUPPORT] = ext_support_lab;
	ext[EXT_EXTABLE] = 1;
	ext[EXT_ENDFLAG] = ext_end_lab;
	if (faild_test)
		ext[EXT_EXTABLE] = 0;
}
inline void HEAD::init()
{
	account_level = level;
	algrthom = 0;
	sum = 0;
	password_sum = 0;
	bs = 0;
	reset_ext();
}
