// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#ifndef __LINUX__
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#endif



// TODO:  在此处引用程序需要的其他头文件

#include <libSemphella/libSemphella.h>
#include <libSemphella/debug.h>

#define SCRIPT_DIR	getenv("SCRIPT_DIR")
#define CONFIG_DIR	getenv("CONFIG_DIR")

#ifndef __LINUX__
#define EXEC	"cmd.exe /C "
#define _DEF_SCRIPT_DIR "./script"
#define _DEF_CONFIG_DIR "./config"
#else
#define EXEC	"sh "
#define _DEF_SCRIPT_DIR	"/etc/time_lize/script"
#define _DEF_CONFIG_DIR "/etc/time_lize"
#endif
