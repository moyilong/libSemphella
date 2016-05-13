#pragma once


#ifndef __LINUX__

#ifdef LIBERT_EXPORTS
#define LIBERT_API extern "C" __declspec(dllexport) 
#else
#define LIBERT_API extern "C" __declspec(dllimport) 
#endif

#else

#define LIBERT_API

#endif
