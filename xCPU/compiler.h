#pragma once
#include "stdafx.h"

CMD parse_line(const string str);
vector<CMD> parse_file(const string xfile);

void WriteBinaryToFile(const string fname);
void Compile(const string iname, const string oname);