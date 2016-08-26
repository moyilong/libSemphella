#include <libSemphella/main.h>
#include <libSemphella/string.h>
#include <libSemphella/crypt.h>
#include <libSemphella/apd.h>
#include <libSemphella/AES.h>
#include <libQr/libQr.h>
#include <libSemphella/argment.h>
string getstr;
string out;
string pread;
int prescale = 8;
void for_each_cfg(string name, string val)
{
	switch (name.at(0))
	{
	case 'i':
		pread = val;
		break;
	case 'o':
		out = val;
		break;
	case 's':
		getstr = val;
		break;
	case 'p':
		prescale = atoi(val.data());
		break;
	}
}
int main(int argc, char *argv[])
{
	argment args;
	args.load(argc, argv);
	args.for_each(for_each_cfg);
	if (out.empty())
	{
		cout << "unknow output!" << endl;
		return false;
	}
	char *ptr;
	uint64_t len;
	file output;
	output.open(out, "w");
	if (pread.empty())
	{
		CreateBMPBUff(getstr, ptr, len, prescale);
	}
	else {
		file in;
		in.open(pread, "r");
		if (in.tell_len() > 1684)
		{
			cout << "File too big!" << endl;
			exit(- 1);
		}
		char *buff = (char*)malloc(in.tell_len());
		in.read(buff, in.tell_len());
		CreateDataBuff(buff, in.tell_len(), ptr, len, prescale);
		in.close();
		free(buff);
	}
	output.write(ptr, len);
	output.close();
}