// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#ifndef __LINUX__
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#endif



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�

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
