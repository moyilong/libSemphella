#include "libSemphella.h"
#include "efstream.h"

API fpos_t filelen(ifstream& file)
{
	fstream::pos_type cur_pos = file.tellg();
	file.seekg(0L, ios::end);
	fstream::pos_type end_pos = file.tellg();
	file.seekg(cur_pos, ios::beg);
	return end_pos.seekpos();
}

API fpos_t filelen(ofstream& file)
{
	fstream::pos_type cur_pos = file.tellp();
	file.seekp(0L, ios::end);
	fstream::pos_type end_pos = file.tellp();
	file.seekp(cur_pos, ios::beg);
	return end_pos.seekpos();
}
