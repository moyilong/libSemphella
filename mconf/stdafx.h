// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include <iostream>
#include <fstream>
using namespace std;

#include <libSemphella/main.h>
#include <libSemphella/string.h>

#include <libSemphella/kernel.h>
#include <libSemphella/debug.h>
#include <libSemphella/math.h>

// TODO:  在此处引用程序需要的其他头文件

//extern libDebug mconf_debug;

#define DEBUG cout<<"[" << __FILE__<<"]["<<__LINE__<<"]"