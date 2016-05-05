#pragma once
enum EVENT_TYPE{
	UNDEFINE,
	SERVER_INIT,			//TAHITI��������ʼ��
	CLIENT_DAEMON_INIT,		//TAHITI�ͻ����ػ����̳�ʼ��
	MODULES_LOAD,			//ģ����أ������飩
	NETWORK_BREAK,			//�������
	SERVER_BACK,			//�������ش���Ϣ
	SERVER_ERR_BACK,		//�������ش�������Ϣ
	SOCK_FAILD,				//����Sock�˿�ʧ��
	BIND_FAILD,				//�󶨶˿�ʧ��
	LISTEN_FAILD,			//����ʧ��
	API_MISMATCH,			//���ݰ��汾����
	SECURE_CHECK_FAILD,		//��ȫ���ʧ��
	RECV_DATA_UNKNOW,		//δ֪����Ѱ��
	ENDIAN_ERROR			//CPU��С�˴���
};

#define LEVEL_MAX	255
#define LEVEL_MIN	0
#define LEVEL_NOR	128

#ifndef MAX_EVENT_SIZE
#define MAX_EVENT_SIZE	16384
#endif

#define ALLOW_PARALLEL_LEVEL_EVENT_PARALLEL_EXEC

struct ENV_DATA{
	DATA_FORMAT data;
	string insert_modules;
	int insert_position;
	SOCKET conn;
};

typedef void(*EVENT_API)(ENV_DATA);

class EVENT{
public:
	EVENT(EVENT_TYPE ctype, EVENT_API capi, unsigned char clevel=LEVEL_NOR,bool conline=true);
	inline EVENT()
	{
	}
	~EVENT();
	void trigger(ENV_DATA data);
	unsigned char level=LEVEL_NOR;
	EVENT_TYPE type=UNDEFINE;
	int get_call_count();
	void get_online();
	void get_offline();
	bool get_stat();
private:
	bool online=true;
	EVENT_API fun;
	int call_count=0;
};

void trigger(EVENT_TYPE type, ENV_DATA data);
inline void trigger(EVENT_TYPE type)
{
	ENV_DATA  temp;
	trigger(type, temp);
}


