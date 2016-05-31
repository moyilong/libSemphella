// s2s.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/main.h>
#include <libSemphella/crypt.h>
#include <time.h>
#include <math.h>
#include <libSemphella/sum.h>

#include <limits>
#include <libSemphella/utils.h>
#include <libSemphella/files.h>
#include <libSemphella/argment.h>
#include <EV_APP/call.h>

bool file_mode = false;
string lfile;
void each(string name, string value)
{
	switch (name.at(0))
	{
	case 'f':
			file_mode = true;
			lfile = value;
			break;
	case 's':
		file_mode = false;
		lfile = value;
		break;
	}
}

int _main(argment args)
{
	args.for_each(each);
	if (file_mode)
	{
		file load;
		load.open(lfile, "r");
		if (!load.is_open())
		{
			cout << "Faild Open File!" << endl;
			return -1;
		}
		printf("%x", load.getsum());
		exit(0);
	}
	uint64_t ret = getsumV2(lfile.data(), lfile.size());
	cout << ull2s(ret) << endl;
	return 0;
}

MODULES("String2String", APP_VER(1, 0, 0, 0), _main, NULL, NULL);