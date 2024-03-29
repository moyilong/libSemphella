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
		debug << "Serail:\"" << val << "\"" << endl;
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
		int val = 1;
		if (serial.read(bit, val))
			//printf("%02x ", (unsigned char)bit[0]);
			cout << bit[0];
	}
}

void xwrite()
{
	while (true)
	{
		char buf[2];
		buf[0] = cin.get();
		buf[1] = '\0';
		serial.write(buf, 1);
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
	thread wr(xwrite);
	tm.join();
	wr.join();

	return 0;
}

LOADDEF(_main,NULL)