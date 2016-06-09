// libERT.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "libERT.h"
#include "algorthim.h"
#include "fhandle.h"
#include "headproto.h"
EXT ex;
char *ext_data;
int count_set = omp_get_num_procs();
void load_ext_data(string filename)
{
	file ext;
	ext.open(filename, "r");
	if (!ext.is_open())
		exit(-1);
	ex.length = ext.tell_len();
	ext_data = (char*)malloc(ex.length);
	memset(ext_data, 0, sizeof(ext_data));
	ext.read(ext_data, ex.length);
	ex.checksum = getsumV2(ext_data, ex.length);
	debug << "External Was Been Load " << ex.length << "@" << ex.checksum << endl;
	ext.close();
}

LIBERT_API RETURN_STAT crypt_to_file(string in, string out, string password, int alg, int fid, string extfil, int bs)
{
	decryptmode = false;
	file i, o;
	if (!extfil.empty())
	{
		load_ext_data(extfil);
	}
	i.open(in, "r");
	if (!i.is_open())
	{
		cout << "Open File " << in << " Faild!" << endl;
		return FILE_IO_FAILD;
	}

	o.open(out, "w");
	if (!o.is_open())
	{
		cout << "Open File " << out << " Faild!" << endl;
		return FILE_IO_FAILD;
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
		char xbuff[sizeof(EXT)];
		memcpy(xbuff, &ex, sizeof(EXT));
		o.write(xbuff, sizeof(EXT));
		char *buff = (char*)malloc(ex.length);
		memcpy(buff, ext_data, ex.length);
		mask(buff, ex.length);
		o.write(buff, ex.length);
	}
	uint64_t mbs, fix;
	i.get_steps(bs, mbs, fix);
	uint64_t sum = 0;
	time_t start = time(0);
	char str_buff[MAX_BUFF_SIZE];
	double per = 0;
	for (uint64_t n = 0; n < mbs;)
	{
		int count = mbs - n;
		if (count > count_set)
			count = count_set;
		get_fhandle(head.ext[EXT_FHANDLE])(head, i, o, sum, head.bs, n*head.bs, false, false,count);
		per = (double)((double)n*(double)head.bs) / (double)i.tell_len();
		if (per != old_presend)
		{
			old_presend = per;
			ulen = (n* head.bs) / dZero(time(0) - start);
			sprintf(str_buff, "%sPS", human_read_storage_str(ulen).data());
			ShowProcessBar(per, str_buff);
		}
		n += count;
	}
	ShowProcessBar(1, " END");
	if (fix > 0)
		get_fhandle(head.ext[EXT_FHANDLE])(head, i, o, sum, fix, mbs*head.bs, false, false,1);
	ShowProcessBar(1, " Finish");
	head.sum = sum;
	head.algrthom = alg;
	head.ext[EXT_FHANDLE] = fid;
	head.account_level = level;
	o.seekp(0);
	head.check();
	o.write(&head, 1);
	cout << endl << endl;
	debug << "WriteHead:" << getsumV2((char*)&head, sizeof(head)) << endl;
	cout << "SysteDecode:" << head.sum << "=>" << sum << endl;
	i.close();
	o.close();
	return OK;
}

LIBERT_API RETURN_STAT decrtpt_to_file(string in, string out, string password, int std_mode)
{
	decryptmode = true;
	file i, o;
	i.open(in, "r");
	if (!i.is_open())
	{
		debug << "Open File " << in << " Faild!" << endl;
		return FILE_IO_FAILD;
	}
	if (!std_mode)
	{
		o.open(out, "w");
		if (!o.is_open())
		{
			debug << "Open File " << out << " Faild!" << endl;
			return FILE_IO_FAILD;
		}
	}
	HEAD head;
	i.seekp(0);
	char hbuff[sizeof(HEAD)];
	i.read(hbuff, sizeof(HEAD));
	memcpy(&head, hbuff, sizeof(HEAD));
	display_dump((char*)&head, sizeof(HEAD));
	debug << "ReadHead:" << getsumV2((char*)&head, sizeof(head)) << endl;
	if (!head.check())
		return HEAD_VERIFY_FAILD;
	if (head.password_sum != APOLL[trans_id(head.algrthom)].px(password))
		return PASSWORD_FAILD;
	uint64_t all_ext_len = 0;
	if (head.ext[EXT_EXTABLE] == 1)
	{
		char ebuff[sizeof(EXT)];
		i.read(ebuff, sizeof(EXT));
		memcpy(&ex, ebuff, sizeof(EXT));
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
	for (uint64_t n = 0; n < mbs; )
	{
		int count = mbs - n;
		if (count > count_set)
			count = count_set;
		get_fhandle(head.ext[EXT_FHANDLE])(head, i, o, sum, head.bs, n*head.bs, true, std_mode,count);
		if (!std_mode)
		{
			double per = (double)((double)n*(double)head.bs) / (double)i.tell_len();
			if (per != old_presend)
			{
				old_presend = per;
				ulen = (n* head.bs) / dZero(time(0) - start);
				ShowProcessBar(per, human_read_storage_str(ulen) + "PS");
			}
		}
		n += count;
	}
	if (!std_mode)
		ShowProcessBar(1, " END");
	if (fix > 0)
	{
		get_fhandle(head.ext[EXT_FHANDLE])(head, i, o, sum, fix, mbs*head.bs, true, std_mode,1);
	}
	if (!std_mode)
		ShowProcessBar(1, " Finish");
	i.close();
	o.close();
	if (!std_mode)
	{
		cout << "SysteDecode:" << head.sum << "=>" << sum << endl;
	}
	if (sum != head.sum)
		return FILE_VERIFY_FAILD;
	if (!std_mode)
		cout << endl << endl;
	return OK;
}

LIBERT_API RETURN_STAT decrypt_to_std(string in, string out, string password)
{
	return decrtpt_to_file(in, out, password, true);
}

LIBERT_API RETURN_STAT get_ext_to_file(string in, string out, bool std_mode)
{
	decryptmode = true;
	file fi, fo;
	fi.open(in, "r");
	if (!fi.is_open())
	{
		cout << "Open " << in << " Faild!" << endl;
		return FILE_IO_FAILD;
	}
	if (!std_mode)
	{
		fo.open(out, "w");
		if (!fo.is_open())
		{
			cout << "open " << out << " Faild!" << endl;
			return FILE_IO_FAILD;
		}
	}
	HEAD head;
	fi.read(&head, 1);
	if (!head.check())
		return HEAD_VERIFY_FAILD;
	if (head.ext[EXT_EXTABLE] != 1)
		return EXT_NOT_EXIST;
	ext_data = (char*)malloc(ex.length);
	fi.read(ext_data, ex.length);
	mask(ext_data, ex.length);
	if (std_mode)
	{
		cout << ext_data;
	}
	else {
		fo.write(ext_data, ex.length);
	}

	return OK;
}

LIBERT_API RETURN_STAT get_ext_to_std(string in)
{
	return get_ext_to_file(in, "", true);
}

LIBERT_API HEAD get_head(string in)
{
	file fin;
	fin.open(in, "r");
	if (fin.is_open())
		throw "FILE_IO_FAILD";
	HEAD head;
	fin.read(&head, 1);
	if (!head.check())
		throw "FILE_TO_LOAD_HEAD";
	return head;
}

LIBERT_API int algor_max()
{
	return xsize;
}
LIBERT_API int fhand_max()
{
	return fsize;
}

LIBERT_API void get_alg_info(int stor_id, int & id, string & help)
{
	id = APOLL[stor_id].id;
	help = APOLL[stor_id].doc;
}

LIBERT_API int get_alg_id(int tid)
{
	return trans_id(tid);
}

LIBERT_API string get_api_ver()
{
	return "ERT4.1.1850";
}