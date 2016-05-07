#pragma once

#ifndef __LINUX

#ifdef LIBERT_EXPORTS
#define LIBERT_API __declspec(dllexport)
#else
#define LIBERT_API __declspec(dllimport)
#endif

#else

#define LIBERT_API

#endif
