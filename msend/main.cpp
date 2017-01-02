#include "perfect.h"


argment args;
string smtp, target, username, password,xsend;

void procd(string name, string value)
{
	switch (name.at(0))
	{
	case 'n':
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
	}
}

int main(int argc, char *argv[])
{
	args.load(argc, argv);
	args.for_each(procd);
	if (smtp.empty() || target.empty() || username.empty() || password.empty() || xsend.empty())
	{
		cout << "No Enougth Argment!" << endl;
		return -1;
	}
	return SendEmail(smtp, username, password, target, xsend);
}