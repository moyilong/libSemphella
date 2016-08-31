#pragma once
#include "main.h"
#include "libSemphella.h"
#include "string.h"

#include "math.h"

struct API ValueInfo {
	double avg = 0;
	double max = 0;
	double min = 0;
	double last = 0;
	uint64_t all_count = 0;
	void clean();
	ValueInfo();
};

API  void ShowProcessBar(float percent, string display, char finish = '=', char splite = '>', char inprocess = '.', int bis = 60);
API void ShowProcessBarEx(int all, int st_1, int st_2, string display, char st1_ch = '=', char st2_ch = '*', char st3_ch = '.');
API void ValueDisplay(double data, string tips, ValueInfo &info, char end_char = '\r');
API string shell(string cmds);