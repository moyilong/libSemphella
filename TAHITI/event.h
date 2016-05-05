#pragma once
enum EVENT_TYPE{
	UNDEFINE,
	SERVER_INIT,			//TAHITI服务器初始化
	CLIENT_DAEMON_INIT,		//TAHITI客户端守护进程初始化
	MODULES_LOAD,			//模块加载（不建议）
	NETWORK_BREAK,			//网络故障
	SERVER_BACK,			//服务器回传消息
	SERVER_ERR_BACK,		//服务器回传错误消息
	SOCK_FAILD,				//创建Sock端口失败
	BIND_FAILD,				//绑定端口失败
	LISTEN_FAILD,			//监听失败
	API_MISMATCH,			//数据包版本故障
	SECURE_CHECK_FAILD,		//安全检查失败
	RECV_DATA_UNKNOW,		//未知数据寻获
	ENDIAN_ERROR			//CPU大小端错误
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


