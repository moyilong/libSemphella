#pragma once
#include <time.h>

struct PT_TIME {
	time_t start;
	time_t mem_alloc;
	time_t caculate_c_time;
	time_t caculate_d_time;
	time_t caculate_l_time;
	time_t random;
	time_t xfree;
};
 PT_TIME PT();
