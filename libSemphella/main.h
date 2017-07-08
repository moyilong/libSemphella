#pragma once
#ifndef __LINUX__
#pragma warning(disable:4251 4309 4244 4237 4018)
#endif
#include "config.h"
#ifndef __ALLOW_LOWMEM_PREFER
#define MAX_BUFF_SIZE   512
#else
#define MAX_BUFF_SIZE	4096
#endif
#define API_VER         180
#define CORE_NAME       "Phoenix"

#ifdef __ANDROID__
#define nullptr NULL
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
using namespace std;

#ifndef APP_BUILD
#include <omp.h>
#else
#define omp_get_num_procs() 1
#endif

#ifdef __LINUX__
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#endif

#ifndef DEFAULT_WORD_WHITE_LIST
#define DEFAULT_WORD_WHITE_LIST "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#endif

#ifndef DEFAULT_WORD_BLACK_LIST
#define DEFAULT_WORD_BLACK_LIST "MUsXGH1bEgTYijNwdklm7ZIJaxSAVWPQtn2340FfOCDuvh8yz56cBoR9qrepKL"
#endif

#ifndef APP_BUILD
#define API  CAPI
#else
#define API CAPI
#endif

#include <time.h>

#include "translate.h"
#include <limits>
#include <stdlib.h>

#ifndef __LINUX__
#pragma warning(disable:4996)
#endif

#define KB	1024
#define MB	1024*KB
#define GB	1024*MB
#define TB	1024*GB

#include "error.h"
#include "secload.h"