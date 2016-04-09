#include "algorthim.h"
#include "cryptutil2.h"
#include <libSemphella/license.h>

int create_license()
{
	if (output.empty())
	{
		cout << "Output is empty!" << endl;
		return false;
	}
	ofstream out;
	if (!std_out)
	{
		out.open(output.data());
		if (!out.is_open())
		{
			cout << "Open File Faild!" << endl;
			return false;
		}
	}
	cp2 << "Creating License..." << endl;
	uint64_t seed = rand()*time(0);
	cp2 << "License Seed:" << seed << endl;
	LICENSE licc = CreateLicense(seed, bs);
	string str = licc.operator=(licc);
	if (std_out)
		cout << str << endl;
	else
		out.write(str.data(), str.size());
	return 0;
}