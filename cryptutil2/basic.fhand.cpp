#include "fhandle.h"


void FileProcess(HEAD head, file in, file out, uint64_t &sum, int len, uint64_t op_addr)
{
	if (decrypt)
	{
		in.seekp(op_addr + sizeof(HEAD));
		if (!std_out)
			out.seekp(op_addr);
	}
	else {
		in.seekp(op_addr);
		out.seekp(op_addr + sizeof(HEAD));
	}
	char *buff = (char*)malloc(len);
	in.read(buff, len);
	uint64_t vsu = 0;
	if (!decrypt)
		vsu = APOLL[trans_id(head.algrthom)].sa(buff, len);
	int doff = 0;
	if (decrypt)
		doff = sizeof(HEAD);
#ifndef WHITE_CRYPT
	APOLL[trans_id(head.algrthom)].ca(buff, len, in.tellp() - len - doff);
#endif
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