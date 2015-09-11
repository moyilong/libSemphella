#pragma once
#include <iostream>
#include <string>
using namespace std;

typedef  unsigned long long feature_t;

#define S_API_VERSION			'B'		//此文件版本号
#define S_API_ALLOW_MIN			'A'			//支持的最低版本号

#define LOCAL_DEVICE			"__LOCALE_DEVICE__"		//本设备
#define MAX_DEVICE				512			//单route最大支持device
#ifndef MAX_BUFF_SIZE
#define MAX_BUFF_SIZE 4096										//缓冲区大小
#endif

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



#define NO			0xFFFFFFFF
#define ERR_BACK	0xFFFFFF00







#define DATA_LEN	4096					//定长传输

#define PORT 		2258					//端口号
#define IP "127.0.0.1"





/*加密函数*/
/*data :要加密/解密的数据*/
/*len  :数据长度*/

inline void Convert(char *data, int len)
{
for (int n = sizeof(char) * 3; n < len; n++)
	data[n] ^=  n;
}

inline char GetHeadCheck(char HEAD_CHECK_A)
{
	return HEAD_CHECK_A * 0x2B - 0xAE ^ HEAD_CHECK_A;
}

#define DEVICE_ID	unsigned long long

struct DEVICE{
	DEVICE_ID id;
	char name[DEVICE_NAME_LEN];
	time_t last;
};

struct DATA_FORMAT{
	char HEAD_CHECK_A ;			//确认辨别
	char HEAD_CHECK_B ;
	char API_LEVEL;			//API版本号定义
	int real_data_len = -1;				//真实数据长度
	feature_t def=NO;				//数据类型
	char buff[DATA_LEN];				//数据		
	int protoco = -1;			//匹配协议号
	struct DEVICE dev;
	inline DATA_FORMAT()
	{
		API_LEVEL = S_API_VERSION;
		HEAD_CHECK_A = 0xA9;
		HEAD_CHECK_B = GetHeadCheck(HEAD_CHECK_A);
		real_data_len = -1;
	}
	
};



struct ServerInfo{
	char server_name[DEVICE_NAME_LEN];			//服务器名称
	int server_io_count = 0;					//服务器IO计数
	int start_time = -1;						//服务器启动时间
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