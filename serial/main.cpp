#include <libSemphella/string.h>
#include <libSemphella/hal.h>
#include <libSemphella/argment.h>
#include <thread>

Serial serial;

void proc_config(string lab, string val)
{
	SerialConfig cfg = serial.GetConfig();
	switch (lab.at(0))
	{
	case 's':
		serial.SetPath(val);
		break;
	case 'd':
		cfg.data_bit = atoi(val.data());
		break;
	case 't':
		cfg.stop_bit = atoi(val.data());
		break;
	case 'b':
		cfg.speed = atoi(val.data());
		break;
	}
	serial.SetConfig(cfg);
}

void monitor()
{
	while (true)
	{
		char bit[8];
		memset(bit, '\0', sizeof(bit));
		int val = 8;
		int get=serial.read(bit, val);
		if (get == 0)
		{
			cout << "Get Zero Length!" << endl;
		}
		for (int n = 0; n < get; n++)
			cout << bit[n];
	}
}

int _main(int argc, char *argv[])
{
	argment args;
	args.load(argc, argv);
	args.for_each(proc_config);
	serial.open();
	if (!serial.is_opened())
	{
		cout << "Open Port Faild!" << endl;
		return 1;
	}
	thread tm(monitor);
	tm.join();
	while (true)
	{
		char val[8];
		memset(val, '\0', sizeof(val));
		cin.read(val, 8);
		debug << "GetValue:" << val << endl;
		serial.write(val, 8);
	}
	return 0;
}

LOADDEF(_main,NULL)