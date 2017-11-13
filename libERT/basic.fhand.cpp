#include "fhandle.h"
#include "algorthim.h"
#include "headproto.h"

void FileProcess(HEAD head, file in, file out, uint64_t &sum, int len, uint64_t op_addr, bool decrypt, bool std_out, int mps)
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

	char **buff = (char**)malloc(mps * sizeof(char*));
	for (int n = 0; n < mps; n++)
	{
		buff[n] = (char*)malloc(len);
		in.read(buff[n], len);
		if (!decrypt)
			sum += APOLL[trans_id(head.algrthom)].sa(buff[n], len);
	}
#pragma omp parallel for
	for (int n = 0; n < mps; n++)
		APOLL[trans_id(head.algrthom)].ca(buff[n], len, 0);
	for (int n = 0; n < mps; n++)
	{
		if (decrypt)
			sum += APOLL[trans_id(head.algrthom)].sa(buff[n], len);
		if (!std_out)
			out.write(buff[n], len);
		else
			fprintf(stdout, "%s", buff[n]);
		free(buff[n]);
	}
	free(buff);
}

XFHANDLE BasicProcess(FileProcess, 0);

void FileProcessSingal(HEAD head, file in, file out, uint64_t &sum, int len, uint64_t op_addr, bool decrypt, bool std_out, int mps)
{
	for (int n = 0; n < mps; n++)
		FileProcess(head, in, out, sum, len, op_addr, decrypt, std_out, 1);
}

XFHANDLE BasicSingle(FileProcessSingal, 1);