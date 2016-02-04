#pragma once

#define MAX_BUFF_SIZE   4096
#define API_VER         128
#define CORE_NAME       "libSemphella-Core"

#ifndef _WNO_CPP
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
using namespace std;
#endif

#ifndef _WNO_OMP
#include <omp.h>
#endif

#define DEFAULT_DEBUG_STAT  true

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

#define API extern "C" CAPI

#include <time.h>

#ifndef __LINUX__
#include <amp.h>
#include <amp_math.h>
using namespace concurrency;
using namespace concurrency::fast_math;
#define __USE_CPP_AMP__
#endif