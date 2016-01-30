#pragma once

struct MP_BLOCK {
	int ilen;
	double iops;
	uint64_t count = 0;
	string temp;
	uint64_t n = 0;
	double per;
};