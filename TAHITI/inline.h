void server_main();
void client_main();
#include "feature_define.h"
struct KERNEL{
	string server = "localhost";
	int port = PORT;
	string device_name="unknow_test";
	long long sleep_time = 1000;
};
KERNEL kernel();