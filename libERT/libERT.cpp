// libERT.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "libERT.h"
#include "algorthim.h"
#include "fhandle.h"
#include "headproto.h"
using namespace LIB_ERTLIB;
EXT ex;
char *ext_data;
void load_ext_data(string filename)
{
	file ext;
	ext.open(filename, "r");
	ex.length = ext.tell_len();
	ext_data = (char*)malloc(ex.length);
	memset(ext_data, 0, sizeof(ext_data));
	ext.read(ext_data, ex.length);
	ex.checksum = getsumV2(ext_data, ex.length);
	debug << "External Was Been Load " << ex.length << "@" << ex.checksum << endl;
	ext.close();
}

LIBERT_API RETURN_STAT LIB_ERTLIB::crypt_to_file(string in, string out, string password, int alg, int fid, string extfil, int bs)
{
	file i, o;
	if (!extfil.empty())
	{
		load_ext_data(extfil);
	}
	i.open(in, "r");
	if (!i.is_open())
	{
		throw "Open " + in + " faild";
	}

	o.open(out, "w");
	if (!o.is_open())
	{
		throw "Open " + out + " faild!";
	}
	HEAD head;
	head.algrthom = alg;
	head.ext[EXT_FHANDLE] = fid;
	head.password_sum = APOLL[trans_id(head.algrthom)].px(password);
	head.bs = bs;
	head.password_sum = APOLL[trans_id(alg)].px(password);
	double old_presend = 0, dlen = i.tell_len();
	uint64_t ulen = 0;
	o.write(&head, 1);
	if (!extfil.empty())
	{
		o.write(&ex, 1);
		o.write(ext_data, ex.length);
	}
	uint64_t mbs, fix;
	i.get_steps(bs, mbs, fix);
	uint64_t sum = 0;
	time_t start = time(0);
	for (uint64_t n = 0; n < mbs; n++)
	{
		get_fhandle(head.ext[EXT_FHANDLE])(head, i, o, sum, head.bs, n*head.bs, false, false);
		double per = (double)((double)n*(double)head.bs) / (double)i.tell_len();
		if (per != old_presend)
		{
			old_presend = per;
			ulen = (n* head.bs) / dZero(time(0) - start);
			ShowProcessBar(per, human_read(ulen, human_read_storage_str, 1024, 10) + "PS");
		}
	}
	ShowProcessBar(1, " END");
	if (fix > 0)
	{
		get_fhandle(head.ext[EXT_FHANDLE])(head, i, o, sum, fix, mbs*head.bs, false, false);
	}
	ShowProcessBar(1, " Finish");
	i.close();
	head.sum = sum;
	head.algrthom = alg;
	head.ext[EXT_FHANDLE] = fid;
	head.account_level = level;
	o.seekp(0);
	head.check();
	o.write(&head, 1);
	o.close();
	return OK;
}

LIBERT_API RETURN_STAT LIB_ERTLIB::decrtpt_to_file(string in, string out, string password, int std_mode)
{
	file i, o;
	i.open(in, "r");
	if (!i.is_open())
	{
		throw "Open " + in + " faild";
	}
	if (!std_mode)
	{
		o.open(out, "w");
		if (!o.is_open())
		{
			throw "Open " + out + " faild!";
		}
	}
	HEAD head;
	i.seekp(0);
	i.read(&head, 1);
	if (!head.check())
		return HEAD_VERIFY_FAILD;
	if (head.password_sum != APOLL[trans_id(head.algrthom)].px(password))
		return PASSWORD_FAILD;
	uint64_t all_ext_len = 0;
	if (head.ext[EXT_EXTABLE] == 1)
	{
		i.read(&ex, 1);
		all_ext_len = sizeof(EXT) + ex.length;
	}
	uint64_t len = i.tell_len() - sizeof(HEAD) - all_ext_len;
	double old_presend = 0, dlen = i.tell_len();
	uint64_t ulen = 0;
	uint64_t mbs, fix;
	mbs = len / head.bs;
	fix = len - (head.bs*mbs);
	uint64_t sum = 0;
	time_t start = time(0);
	for (uint64_t n = 0; n < mbs; n++)
	{
		get_fhandle(head.ext[EXT_FHANDLE])(head, i, o, sum, head.bs, n*head.bs, true, std_mode);
		if (!std_mode)
		{
			double per = (double)((double)n*(double)head.bs) / (double)i.tell_len();
			if (per != old_presend)
			{
				old_presend = per;
				ulen = (n* head.bs) / dZero(time(0) - start);
				ShowProcessBar(per, human_read(ulen, human_read_storage_str, 1024, 10) + "PS");
			}
		}
	}
	if (!std_mode)
		ShowProcessBar(1, " END");
	if (fix > 0)
	{
		get_fhandle(head.ext[EXT_FHANDLE])(head, i, o, sum, fix, mbs*head.bs, true, std_mode);
	}
	if (!std_mode)
		ShowProcessBar(1, " Finish");
	i.close();
	o.close();
	if (sum != head.sum)
		return FILE_VERIFY_FAILD;

	return OK;
}

LIBERT_API RETURN_STAT LIB_ERTLIB::decrypt_to_std(string in, string out, string password)
{
	return decrtpt_to_file(in, out, password, true);
}

LIBERT_API RETURN_STAT LIB_ERTLIB::get_ext_to_file(string in, string out, bool std_mode)
{
	file fi, fo;
	fi.open(in, "r");
	if (!fi.is_open())
	{
		throw "Open " + in + " Faild!";
	}
	if (!std_mode)
	{
		fo.open(out, "w");
		if (!fo.is_open())
		{
			throw "open " + out + " Faild!";
		}
	}
	HEAD head;
	fi.read(&head, 1);
	if (!head.check())
		return HEAD_VERIFY_FAILD;
	if (head.ext[EXT_EXTABLE] != 1)
		return EXT_NOT_EXIST;
	char *buff = (char*)malloc(ex.length);
	fi.read(buff, ex.length);
	if (std_mode)
	{
		cout << buff;
	}
	else {
		fo.write(buff, ex.length);
	}
	return OK;
}

LIBERT_API RETURN_STAT LIB_ERTLIB::get_ext_to_std(string in)
{
	return get_ext_to_file(in, "", true);
}

LIBERT_API HEAD LIB_ERTLIB::get_head(string in)
{
	file fin;
	fin.open(in, "r");
	if (fin.is_open())
		throw "FILE_IO_FAILD";
	HEAD head;
	fin.read(&head, 1);
	if (!head.check())
		throw "HEAD CHECK FAILD!";
	return head;
}

LIBERT_API int LIB_ERTLIB::algor_max()
{
	return xsize;
}
LIBERT_API int LIB_ERTLIB::fhand_max()
{
	return fsize;
}

LIBERT_API void LIB_ERTLIB::get_alg_info(int stor_id, int & id, string & help)
{
	id = APOLL[stor_id].id;
	help = APOLL[stor_id].doc;
}

LIBERT_API int LIB_ERTLIB::get_alg_id(int tid)
{
	return trans_id(tid);
}

LIBERT_API string LIB_ERTLIB::get_api_ver()
{
	return "ERT4.0.1735";
}