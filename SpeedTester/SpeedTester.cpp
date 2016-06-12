// SpeedTester.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/argment.h>
#include <libNetUtils/libNetUtils.h>


argment  args;

bool server_mode = false;

int port = 3375;
string server;
int block = 12288;

void arg_proc(string name, string value)
{
	switch (name.at(0))
	{
	case 's':
		server_mode = true;
		break;
	case 'p':
		port = atoi(value.data());
		break;
	case 't':
		server = value;
		break;
	}
}

int _main(int argc, char *argv[])
{
	args.load(argc, argv);
	args.for_each(arg_proc);
	cout << "Working Port:" << port << endl;
	if (server_mode)
	{
		cout << "Entry Server mode!" << endl;
		service_main(port);
		exit(0);
	}
	cout << "Settings Server:" << server << ":" << port << endl;
	cout << "Testing Download..." << endl;
	double down = Download(server, block, port);
	cout << "Testing Upload..." << endl;
	double uplo = Upload(server, block, port);
	cout << "Download Speed:" << human_read_storage_str(down)<<endl;
	cout << "Upload Speed:" << human_read_storage_str(uplo) << endl;
}

int main(int argc, char *argv[])
{
	try {
		return _main(argc, argv);
	}
	catch (throw_t t)
	{
		cout << "ErrValue Get:" << t << endl;
		return t;
	}
	catch (...)
	{
		cout << "Program have a  problem!" << endl;
		return -1;
	}
}