#pragma once
#include "main.h"
#include "libSemphella.h"


API void ShowProcessBar(double percent, string display, char finish = '*',char splite='>',char inprocess = '=',int bis=32);


struct API ARGMENT {
	string name;
	string value;
	bool is_bool();
};

API void ProcessArgment(int argc, char *argv[], vector<ARGMENT>&poll);
API bool HaveArgment(string name, vector<ARGMENT>&poll);
API string GetValue(string name, vector<ARGMENT>&poll);