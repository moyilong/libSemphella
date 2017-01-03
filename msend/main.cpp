#include "perfect.h"


argment args;
string smtp, target, username, password,xsend;
string title = "undefine";
void procd(string name, string value)
{
	switch (name.at(0))
	{
	case 'u':
		username = value;
		break;
	case 'p':
		password = value;
		break;
	case 's':
		smtp = value;
		break;
	case 't':
		target = value;
		break;
	case 'd':
		xsend = value;
		break;
	case 'n':
		title = value;
		break;
	case 'v':
		if (strequal(name.data(), "verbose"))
			KERNEL.SetDebugStat(true);
		break;
	}
}

int main(int argc, char *argv[])
{
	args.load(argc, argv);
	args.for_each(procd);
	if (smtp.empty() || target.empty() || username.empty() || password.empty() || xsend.empty())
	{
		cout << "No Enougth Argment!" << endl;
		cout << "-u\tAccess user name" << endl;
		cout << "-p\tPassword" << endl;
		cout << "-s\tSMTP Server" << endl;
		cout << "-d\tSend Data" << endl;
		cout << "-n\tSend Title" << endl;
		return -1;
	}
	if (xsend.at(0) == 'f' && xsend.at(1) == ':')
	{
		string fname = xsend.substr(2);
		file fi;
		fi.open(fname,"r");
		if (!fi.is_open())
		{
			cout << "Open File Faild!" << fname << endl;
		}
		char *buffers = (char *)malloc(fi.tell_len());
		fi.read(buffers, fi.tell_len());
		xsend = buffers;
		free(buffers);
	}
	return SendEmail(smtp, username, password, target, xsend,title);
}