#include "libNetUtils.h"
#include "protoco.h"
#include "modules.h"
#define DEFAULT_TRANSFAR_LEN	4096

void DownloadTest(API_ARGS_DEF)
{
	char *t_send_ptr = (char*)malloc(DEFAULT_TRANSFAR_LEN);
#pragma omp parallel for
	for (int n = 0; n < DEFAULT_TRANSFAR_LEN; n++)
		t_send_ptr[n] = n^rand() ^ time(0);
	send_ptr = t_send_ptr;
	send_len = DEFAULT_TRANSFAR_LEN;
}

MOD DownloadRegister(DOWNLOAD_TEST, DownloadTest);

NETAPI double Download(string targsrv, int block_size, int port)
{
	vector<time_t> col;
	for (int n = 0; n < block_size; n++)
	{
		SOCKET sock = create_connect(port, targsrv);
		if (sock == INVALID_SOCKET)
			throw  CREATE_SOCKET_FAILD;
		char *buff = nullptr;
		uint64_t len = 0;
		time_t beg = time(0);
		PortSend(sock, NULL, 0, DOWNLOAD_TEST);
		PortGet(sock, buff, len);
		time_t end = time(0);
		col.push_back(end - beg);
		close(sock);
	}
	double ret = col.at(0);
	for (int n = 1; n < block_size; n++)
		ret += col.at(n) / 2;
	return ret;
}

void UploadTest(API_ARGS_DEF)
{
	send_ptr = NULL;
	send_len = 0;
}
MOD UploadRegister(UPLOAD_TEST, UploadTest);
NETAPI double Upload(string targsrv, int block_size, int port)
{
	vector<time_t> col;
	for (int n = 0; n < block_size; n++)
	{
		SOCKET sock = create_connect(port, targsrv);
		if (sock == INVALID_SOCKET)
			throw  CREATE_SOCKET_FAILD;
		char *t_send_ptr = (char*)malloc(DEFAULT_TRANSFAR_LEN);
#pragma omp parallel for
		for (int n = 0; n < DEFAULT_TRANSFAR_LEN; n++)
			t_send_ptr[n] = n^rand() ^ time(0);
		time_t beg = time(0);
		PortSend(sock, t_send_ptr, DEFAULT_TRANSFAR_LEN, UPLOAD_TEST);
		char *buff = nullptr;
		uint64_t len;
		PortGet(sock, buff, len);
		time_t end = time(0);
		col.push_back(end - beg);
		close(sock);
	}
	double ret = col.at(0);
	for (int n = 1; n < block_size; n++)
		ret += col.at(n) / 2;
	return ret;
}