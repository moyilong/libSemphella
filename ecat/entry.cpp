#include "include.h"
#include "modules.h"
#include <libSemphella/files.h>
argment args;

vector<string> infile;
string fapi_call = "normally";
_CAT_MOD mod;
int block = 128;
void args_process(string name, string val)
{
	switch (name.at(0))
	{
	case 'i':
		//infile = val;
		infile.push_back(val);
		break;
	case 'm':
		fapi_call = val;
		break;
	case 'b':
		block = atoi(val.data());
		break;

	}
}

void catfile(string filename)
{
	file open;
	open.open(filename.data(), "r");
	if (!open.is_open())
	{
		cout << "Open File Faild:" << filename << endl;
		return;
	}
	uint64_t len = open.tell_len();
	uint64_t bs = len / block;
	uint64_t fx = len - (bs*block);
	char *buff = (char*)malloc(bs);
	mod.dinit_call(args);
	for (int n = 0; n < bs; n++)
	{
		open.read(buff, bs);
		mod.dapi(buff, bs, args);
	}
	open.read(buff, fx);
	mod.dapi(buff, fx, args);
	mod.dendcall(args);
}

int main(int argc, char *argv[])
{
	args.load(argc, argv);
	args.for_each(args_process);
	if (infile.empty())
	{
		cout << "No Input File!" << endl;
		return -1;
	}
	try {
		mod=Search(fapi_call);
	}
	catch (...)
	{
		cout << "Not found Vmod:" << fapi_call << endl;
		return -1;
	}
	for (int n = 0; n < infile.size(); n++)
		catfile(infile.at(n));
}