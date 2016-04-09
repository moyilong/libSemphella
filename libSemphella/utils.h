#pragma once
#include "main.h"
#include "libSemphella.h"
#include "string.h"

#include "math.h"
API  void ShowProcessBar(double percent, string display, char finish = '=', char splite = '>', char inprocess = '.', int bis = 48);
API void ShowProcessBarEx(int all, int st_1, int st_2, string display, char st1_ch = '=', char st2_ch = '*', char st3_ch = '.');
