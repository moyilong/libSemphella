#pragma once
#define MAX_MOD_SIZE	64
#include "feature_define.h"
#include "permission.h"

typedef void(*ESS_Modules)(struct DATA_FORMAT, struct DATA_FORMAT &, SOCKET &);
typedef void(*ESS_CMOD)(struct DATA_FORMAT &);
typedef int(*CLI)(vector<string>);
typedef void(*KSAPI)();

void ErrSetBack(struct DATA_FORMAT &ret, const char *ERR_INFO);

#define CLI_FEATURE		0xFFFFFFFF

class Modules {
public:
	Modules();
	Modules(feature_t _mode, string name, ESS_Modules __server_entry = NULL, ESS_CMOD __clinet_entry_to = NULL, ESS_CMOD __client_entry_ret = NULL, permission_t level = GUEST, CLI kcli = NULL, KSAPI vlink = NULL);
	Modules(string name, CLI link, KSAPI vlink = NULL);
	ESS_Modules get_server_entry();
	ESS_CMOD get_client_to();
	ESS_CMOD get_client_ret();
	feature_t get_api();
	string get_name();
	CLI get_cli();
	permission_t get_level();
	void reg_client(ESS_CMOD x_clint_to, ESS_CMOD x_client_ret);
	void reg_cli(CLI xcli);
	void reg_ser(ESS_Modules srv);
private:
	feature_t mode;
	ESS_CMOD entry_client_to;
	ESS_CMOD entry_client_ret;
	ESS_Modules entry_server;
	CLI cmd_link;
	char sname[MAX_BUFF_SIZE];
	permission_t plevel;
protected:
	void init();
	void reg();
};

Modules get_mod(uint64_t ID);
int get_modules_size();

#define ERR_API_MISMATCH	-1
#define ERR_UNKNOW_DATA		-2
#define ERR_NO_ERROR		0

//MODULES API
DATA_FORMAT network_trans(DATA_FORMAT to, DATA_FORMAT &ret);	//客户端网路传输
string cli_preprocess(vector<string>poll, string getd);	//获取参数
SOCKET create_connect(int port = -1);				//仅创建连接

int v1_data_send(DATA_FORMAT data, SOCKET conn);	//发送数据
int v1_data_recv(SOCKET conn, DATA_FORMAT &ret);		//接受数据

int v2_recv_data(SOCKET sock, DATA_FORMAT &data);
int v2_send_data(SOCKET sock, DATA_FORMAT data);

int data_recv(SOCKET conn, DATA_FORMAT &ret);
int data_send(DATA_FORMAT data, SOCKET conn);

void copy_vector_to_char(vector<char>a, char *b, long long cplen);
void ProtocoRecvData(SOCKET sock, vector<char>&data, int wlen);
void ProtocoSendData(SOCKET sock, char *data, int len);

uint16_t fast_server_online_test();	//服务器快速检测