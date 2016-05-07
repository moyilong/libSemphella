#include "fhandle.h"
#include <libSemphella/data_recovery.h>
#include "libERT.h"
#include "headproto.h"
#include "algorthim.h"
#include "fhandle.h"
#define cp2 debug
uint64_t xcount = 0;
void Rechandle(HEAD head, file in, file out, uint64_t &sum, int len, uint64_t op_addr,bool decrypt,bool std_out)
{
	int rclen = get_block_len(len);
	char *recdata = (char*)malloc(rclen);
	uint64_t mov_in = 0, mov_out = 0;
	uint64_t rec_off = xcount*get_block_len(head.bs);
	cp2 << "Setting Recovery Offset:" << rec_off << endl;
	if (decrypt)
	{
		mov_in = op_addr + sizeof(HEAD) + rec_off;
		mov_out = op_addr;
	}
	else {
		mov_in = op_addr;
		mov_out = op_addr + sizeof(HEAD) + rec_off;
	}
	in.seekp(mov_in);
	out.seekp(mov_out);
	char *buff = (char*)malloc(len);
	in.read(buff, len);
	in.read(recdata, rclen);
	if (decrypt)
	{
		if (VerifyRecoveryData(recdata, rclen) != OK)
		{
			cout << "Error: VerifyData Error!" << endl;
			exit(-1);
		}
		STAT rcstat = Recovery(buff, len, recdata);
		if (rcstat != OK && rcstat != Fixed)
		{
			cout << "Error: Unexpect Error @ " << op_addr << endl;
			exit(-1);
		}
		else {
			cp2 << "Recovery Data is OK!" << endl;
		}
	}
	else {
#ifndef WHITE_CRYPT
		APOLL[trans_id(head.algrthom)].ca(recdata, rclen, 0);
#endif
		cp2 << "Caculating Recovery Data... (" << rclen << ")" << endl;
		CaculateRecovery(buff, len, recdata);
	}
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
	if (!decrypt)
	{
#ifndef WHITE_CRYPT
		APOLL[trans_id(head.algrthom)].ca(recdata, rclen, 0);
#endif
		cp2 << "Recovery data is writed!" << endl;
		if (!std_out)
			out.write(recdata, rclen);
		else
			fprintf(stdout, "%s", recdata);
	}
	sum += vsu;
	free(buff);
	free(recdata);
	xcount++;
}

XFHANDLE RechandleX1(Rechandle, 1);