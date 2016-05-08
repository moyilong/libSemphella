#include "stdafx.h"

#define DEBUG	debug<<"[Protoco V2]"

struct V2_PRO_HEAD {
	char API_A;
	char API_B;
	unsigned long long io_len;
	unsigned long long checksum;
	inline V2_PRO_HEAD()
	{
		API_A = rand();
		API_B = GetHeadCheck(API_A);
		io_len = -1;
	}
};

#define V2_HEAD_LEN	sizeof(V2_PRO_HEAD)

unsigned long long caculate_checksum(const char *data, unsigned long long len)
{
	unsigned long long ret = 0;
	for (int n = 0; n < len; n++)
		ret += data[n] * 1000 + 0xFF9;
	return ret;
}

void copy_vector_to_char(vector<char>a, char *b, long long cplen)
{
#pragma omp parallel for
	for (long long n = 0; n < cplen; n++)
	{
		b[n] = a.at(n);
	}
}

void RecvRAWSocketData(SOCKET sock, char *data, int wlen)
{
#pragma omp parallel for
	for (int n = 0; n < wlen; n++)
		data[n] = 0;
	recv(sock, data, wlen, 0);
	xorcrypt(data, wlen);
}

void SendRAWSocketData(SOCKET sock, char *data, int wlen)
{
	char *_data = (char*)malloc(sizeof(char)*wlen);
	memset(_data, 0, sizeof(data));
#pragma omp parallel for
	for (int n = 0; n < wlen; n++)
		_data[n] = data[n];
	xorcrypt(_data, wlen);
	send(sock, _data, wlen, 0);
	free(_data);
}

void ProtocoRecvData(SOCKET sock, vector<char>&data, int wlen)
{
	V2_PRO_HEAD head;
	RecvRAWSocketData(sock, (char*)&head, V2_HEAD_LEN);
	DEBUG << "Recving Data @" << head.checksum << endl;;
	data.clear();
	char *tdata = (char*)malloc(sizeof(char)*head.io_len);
	RecvRAWSocketData(sock, tdata, wlen);
	for (int n = 0; n < wlen; n++)
		data.push_back(tdata[n]);
	unsigned long long csum = caculate_checksum(tdata, wlen);
	DEBUG << "Recving ReadData @" << csum << endl;
	if (caculate_checksum(tdata, wlen) != head.checksum)
	{
		DEBUG << "Protoco Package Checksum Faild!" << endl;
	}
	free(tdata);
	tdata = NULL;
}

void ProtocoSendData(SOCKET sock, char *data, int len)
{
	V2_PRO_HEAD head;
	head.io_len = len;
	head.checksum = caculate_checksum(data, len);
	DEBUG << "Sending Data @" << head.checksum << endl;
	SendRAWSocketData(sock, (char*)&head, V2_HEAD_LEN);
	SendRAWSocketData(sock, data, len);
}

int v2_recv_data(SOCKET sock, DATA_FORMAT &data)
{
	vector<char>xbuff;
	ProtocoRecvData(sock, xbuff, SEND_LEN);
	copy_vector_to_char(xbuff, (char*)&data, SEND_LEN);
	return ERR_NO_ERROR;
}

int v2_send_data(SOCKET sock, DATA_FORMAT data)
{
	ProtocoSendData(sock, (char*)&data, SEND_LEN);
	return ERR_NO_ERROR;
}