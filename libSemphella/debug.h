#pragma once
#include "kernel.h"
#include "config.h"

#ifdef __ALLOW_DYMANIC_DEBUG
#define DEBUG_LINE	if (KERNEL.GetDebugStat())
#else
#define DEBUG_LINE if(__DEFAULT_DEBUG_STAT)
#endif

#define debug DEBUG_LINE cout<<"["<<__FILE__<<"]["<<__LINE__<<"]"

#define DEBUG_CALL_TRACK_ARGMENT	string __file_name=__FILE__ ,int __file_line=__LINE__
#define DEBUG_CALL_TRACK_ARGMENT_FUNC	string __file_name,int __file_line

API void __display_dump(const char *data, long long len, int x = 5, DEBUG_CALL_TRACK_ARGMENT);

API void *d_malloc(size_t len);
API void d_free(void *ptr);


#define display_dump DEBUG_LINE __display_dump
#ifndef DISABLE_MALLOC_DEBUG
#define malloc d_malloc
#define free d_free
#endif