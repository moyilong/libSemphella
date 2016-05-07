#include "stdafx.h"
#include "headproto.h"
#include "libERT.h"
#include "algorthim.h"
#include"fhandle.h"
#define cp2 debug
bool HEAD::check()
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
	if (trans_id(algrthom) == -1)
	{
		cout << "Error: Unsupported Algorthim!" << endl;
		return false;
	}
	return true;
}
void HEAD::reset_ext(bool faild_test)
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
HEAD::HEAD()
{
	account_level = level;
	algrthom = 0;
	sum = 0;
	password_sum = 0;
	bs = 0;
	reset_ext();
}