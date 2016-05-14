#include "fhandle.h"
#include "algorthim.h"
#include "headproto.h"

void FileProcess(HEAD head, file in, file out, uint64_t &sum, int len, uint64_t op_addr, bool decrypt, bool std_out)
{
	int external_size = 0;
	if (head.ext[EXT_EXTABLE] == 1)
	{
		external_size += ex.length + sizeof(EXT);
	}
	if (decrypt)
	{
		debug << "in decrypt mode" << endl;
		in.seekp(op_addr + sizeof(HEAD) + external_size);
		if (!std_out)
			out.seekp(op_addr);
	}
	else {
		debug << "in crypt mode" << endl;
		in.seekp(op_addr);
		out.seekp(op_addr + sizeof(HEAD) + external_size);
	}
	char *buff = (char*)malloc(len);
	in.read(buff, len);
	uint64_t vsu = 0;
	if (!decrypt)
		vsu = APOLL[trans_id(head.algrthom)].sa(buff, len);
	int doff = 0;
	if (decrypt)
		doff = sizeof(HEAD);
	APOLL[trans_id(head.algrthom)].ca(buff, len, in.tellp() - len - doff);
	if (decrypt)
		vsu = APOLL[trans_id(head.algrthom)].sa(buff, len);
	if (!std_out)
		out.write(buff, len);
	else
		//		cout<<buff;
		fprintf(stdout, "%s", buff);
	sum += vsu;
	free(buff);
}

XFHANDLE BasicProcess(FileProcess, 0);
