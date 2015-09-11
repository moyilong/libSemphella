void server_main();
void client_main();
#include "feature_define.h"
struct KERNEL{
	string server = "localhost";
	int port = PORT;
};
KERNEL kernel();