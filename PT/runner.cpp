// runner.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <omp.h>
#include <libSemphella/libSemphella.h>
#include "libPT.h"

using namespace std;

int main()
{

	bool signal = false;
	PT_TIME ret;

	ret = PT();
		cout << "Memalloc Time:" << ret.mem_alloc - ret.start << endl;
		cout << "Caculate char Time:" << ret.caculate_c_time - ret.mem_alloc << endl;
		cout << "Caculate long Time:" << ret.caculate_l_time - ret.caculate_c_time << endl;
		cout << "Caculate double Time:" << ret.caculate_d_time - ret.caculate_l_time << endl;
		cout << "Random:" << ret.random - ret.caculate_d_time << endl;
		signal = true;
#ifndef __LINUX__
		system("pause");
#endif
return 0;
}

