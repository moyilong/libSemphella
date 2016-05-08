#pragma once
#include <iostream>
#include <string>
#include <time.h>
#include <libSemphella/crypt.h>
using namespace std;

typedef  uint64_t feature_t;

#define S_API_VERSION			'X'			//此文件版本号
#define S_API_ALLOW_MIN			'X'			//支持的最低版本号

#ifndef MAX_BUFF_SIZE
#define MAX_BUFF_SIZE 4096										//内建缓冲区大小
#endif

#define DEFAULT_CRYPT_PASSWORD	"moyilong_crypt_enhanced"

#define DEVICE_NAME_LEN			32			//设备名称长度

/*      ERROR API     */
#define _ERROR_SEPECI_STRING			"__ERROR_SPECIAL_STRING___"			//特殊字符错误
#define _ERROR_API_MISMATCH				"__ERROR_API_MISMATCH_____"			//API匹配错误
#define _ERROR_DATA_NOT_EXIST			"_ERROR_NOT_EXIST_________"			//请求的数据不存在
#define _ERROR_NO_ERROR					"__SERVER_NO_ERROR________"			//无错误
#define _ERROR_DEFINE_UNKNOW			"__UNKNOW_FEATURE_________"			//定义的功能未知
#define _ERROR_DATA_UNCONFIRM			"________UN_CONFIRM_DATA__"			//数据不完整
#define _ERROR_COMMAND_NOT_FIND			"SERVER_COMMAND_NOT_FIND__"			//服务器指令未寻获
#define _ERROR_SERVER_MODULES_NO_THIS	"___SERVER_NO_THIS_FEATURE"			//此指令没有服务器版本
#define _ERROR_PERMISSION_DINED			"___PERMISSION_DINED______"			//权限不足
#define _ERROR_AUTH_FAILD				"__SRC_AUTH_FAILD_________"			//授权失败
#define _ERROR_ENDINA_ERROR				"__CPU_ARCH_ENDINA_UNMATCH"			//CPU大小端错误

#define NO			0xFFFFFFFF
#define ERR_BACK	0xFFFFFF00

#define AUTH_CODE_LEN	32		//验证密钥长度

#define DATA_LEN	MAX_BUFF_SIZE					//定长传输

#define PORT 		2258					//端口号
#define IP "127.0.0.1"

inline void xorcrypt(char *data, int len, string password = DEFAULT_CRYPT_PASSWORD)
{
	fastCrypt(data, len, password, 16);
}
#define Convert xorcrypt
char GetHeadCheck(char HDA);

#define DEVICE_ID	uint64_t

struct DEVICE {
	DEVICE_ID id;
	char name[DEVICE_NAME_LEN];
	clock_t last;
	inline DEVICE()
	{
		memset(name, 0, DEVICE_NAME_LEN);
	}
};
#ifndef BIG_ENDIAN
#define BIG_ENDIAN	0
#endif
#ifndef LIT_ENDIAN
#define LIT_ENDIAN	1
#endif

char ENDIAN_TEST();

struct DATA_FORMAT {
	char HEAD_CHECK_A;				//确认辨别
	char HEAD_CHECK_B;				//计算辨别
	char API_LEVEL;					//API版本号定义
	char endia_stat;				//大小端确认
	int real_data_len = -1;			//真实数据长度
	feature_t def = NO;				//数据类型
	DEVICE dev;						//设备
	char auth_key[AUTH_CODE_LEN];	//授权密钥
	char buff[DATA_LEN];			//数据
	inline DATA_FORMAT()
	{
		API_LEVEL = S_API_VERSION;
		HEAD_CHECK_A = 0xA9;
		HEAD_CHECK_B = GetHeadCheck(HEAD_CHECK_A);
		real_data_len = -1;
		memset(auth_key, 0, sizeof(auth_key));
		endia_stat = ENDIAN_TEST();
	}
};

struct ROUTING_INFO {
	feature_t def;
	char name[DEVICE_NAME_LEN];
	char data[DATA_LEN / 2];
	inline ROUTING_INFO()
	{
		memset(name, 0, sizeof(name));
		memset(data, 0, sizeof(data));
	}
};

struct ServerInfo {
	char server_name[DEVICE_NAME_LEN];			//服务器名称
	int reg_dev_size;
	int routing_size;
};

#define SEND_LEN	sizeof(DATA_FORMAT)

/*

通讯步骤说明:

服务器-->

					|---<-------------<----------<-------<---------
					|											  |
初始化----->accept等待连接--(链接传入)-->按需求转发数据-->结束，终止通讯--|
								|							|
								|							|
								|							|
								|							|
客户端-->						|							|
								|							|
初始化----->创建和服务器的链接->|							|------>从服务器获得数据--->处理-----|
					|						     												 |
					|-----------<----------<----------<----------<---------等待间隔<--=-----------

*/
