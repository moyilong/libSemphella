#include "main.h"
#include "files.h"
#include <fstream>

API uint64_t get_file_len(string filename)
{
	if (!file_test(filename))
		return -1;
	FILE *op = fopen(filename.data(), "r");
	uint64_t ret = ftell(op);
	fseek(op, 0, SEEK_END);
	ret = ftell(op) - ret;
	fclose(op);
	return ret;
}

API bool file_test(string filename)
{
	FILE *op = fopen(filename.data(), "r");
	if (op == NULL)
		return false;
	fclose(op);
	return true;
}
