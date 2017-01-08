#pragma once
#include "error.h"

typedef int(*tmain)(int argc, char *argv[]);

API int secure_init(int argc, char *argv[], tmain entry,tmain preload=NULL);

#define SEC_LOADER int main(int argc,char *argv[]) \
		{ \
			return secure_init(argc,argv,_main);\
		}


