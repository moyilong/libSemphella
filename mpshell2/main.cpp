#include "include.h"
#include <libSemphella/files.h>
argment args;
libMps mps;

void inf(const string fname)
{
	file i;
	i.open(fname, "r");
	if (!i.is_open())
		return;
	while (!i.is_eof())
		mps += i.getline();
}

void foreach(string lab, string name)
{
	vector<estring> spl = estring(name).Split( ',', true);
	int ridx = 10;
	int beg, step, end;
	switch (lab.at(0))
	{
	case 's':
		mps += name;
		break;
	case 'l':
		if (spl.size() < 3)
		{
			cout << "Not Enougth Argment!";
			exit(-1);
		}
		if (spl.size() == 4)
			ridx = atoi(spl.at(3).data());
		beg = atoi(spl.at(0).data());
		step = atoi(spl.at(1).data());
		end = atoi(spl.at(2).data());
		debug << "Loop form " << beg << " to " << end << " step " << step << endl;
		if (step == 0)
		{
			cout << "Step is 0!" << endl;
			exit(-1);
		}
		for (int n = beg; n <= end; n += step)
		{
			debug << "Add Command " << n << endl;
			mps += eitoa(n, ridx);
		}
		break;
	case 'c':
		mps.add_cmd(name);
		break;
	case 'f':
		inf(name);
		break;
	case 'n':
		mps.mpsize = atoi(name.data());
		break;
	case 'd':
		mps.dry_run = true;
		break;
	case 'v':
		KERNEL.SetDebugStat(true);
		break;
	default:
		break;
	}
}

int _main(int argc, char *argv[])
{
	args.load(argc, argv);
	args.for_each(foreach);
	debug << "Run Command: " << mps.cmd_size() << " cmd && " << mps.poll_size() << " args" << endl;
	mps.execute();
	return 0;
}

LOADDEF(_main, NULL);