#pragma once
#include "config.h"
#ifndef __ALLOW_LOWMEM_PREFER
#define MAX_BUFF_SIZE   512
#else
#define MAX_BUFF_SIZE	4096
#endif
#define API_VER         160
#define CORE_NAME       "libSemphella-Phoenix"

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

#ifdef __LINUX__

#define CAPI

#else

#ifdef __DLL_EXPORT
#define CAPI	 __declspec(dllexport)
#else
#define CAPI	 __declspec(dllimport)
#endif

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