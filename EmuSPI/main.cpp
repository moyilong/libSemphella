#include "EMU_EEPROM.h"
#include "modules.h"
#include <libSemphella/crypt.h>
FLASH poll[128];
uint64_t xsize = 0;
FLASH::FLASH(const char*na, int ps, PARTITION * pt, uint64_t len)
{
	//name = na;
	memset(name, 0, sizeof(name));
	strcpy(name, na);
	part = pt;
	length = len;
	partition_size = ps;
	debug << "Insert:" << na << ":" << human_read_storage_str(len) << "=" << ps << endl;
	poll[xsize] = *this;
	xsize++;
}
PARTITION FLASH::GetPart(string xname) const
{
	for (int n = 0; n < partition_size; n++)
		if (streval(part[n].name, xname.data()))
			return part[n];
	cout << "Invode Partition!" << endl;
	cout << "Unexist:" << xname << endl;
	exit(-1);
}
PARTITION FLASH::GetPart(int id) const
{
	if (id >= partition_size)
	{
		cout << "Invoke Partiton! Maxed Out!" << endl;
		exit(-9);
	}
	return part[id];
}

FLASH GetFlash(string mfname)
{
	for (int n = 0; n < xsize; n++)
	{
		debug << "Matching:" << poll[n].name << endl;
		if (streval(mfname.data(), poll[n].name))
			return poll[n];
	}
	cout << "Invoke Device:" << mfname << endl;
	exit(-3);
}
argment args;
string device_name = "";
string partition_name = "";
string filename = "";
bool dump_mode = false;
bool see_mode = false;
void argprobe(string name, string val)
{
	switch (name.at(0))
	{
	case 'd':
		dump_mode = true;
		break;
	case 'f':
		filename = val;
		break;
	case 'v':
		device_name = val;
		break;
	case 'p':
		partition_name = val;
		break;
	case 's':
		see_mode = true;
		break;
	}
}
int main(int argc, char *argv[])
{
	KERNEL.SetDebugStat(true);
	debug << "All Insert:" << xsize << endl;
	args.load(argc, argv);
	args.for_each(argprobe);
	if (device_name.empty())
		return -2;
	if (filename.empty())
		return -1;
	file fi;
	fi.open(filename, "r");
	const FLASH device = GetFlash(device_name);
	debug << "Allocing Memory:" << human_read_storage_str(device.length) << endl;
	char *buff = (char*)malloc(device.length);
	fi.read(buff, device.length);
	fi.close();
	if (see_mode)
	{
		__display_dump(buff, device.length);
		exit(0);
	}
	if (dump_mode)
	{
		cout << "System is Dump Mode!" << endl;
		cout << "Caculated Hash Code:" << getsumV2(buff, device.length) << endl;
		debug << "EEPROM is Readed!" << endl;
		if (!partition_name.empty())
		{
			const PARTITION part = device.GetPart(partition_name);
			file fo;
			debug << "Opening:" << part.name << endl;
			debug << "Exporting form " << part.begin << " +" << part.length << endl;
			fo.open(part.name, "w");
			char *dbuff = (char*)malloc(part.length);
			memcpy(dbuff, buff + part.begin, part.length);
			fo.write(dbuff, part.length);
			fo.close();
			free(dbuff);
		}
		else {
			cout << "Dump all partition...." << endl;
			for (int n = 0; n < device.partition_size; n++)
			{
				const PARTITION part = device.GetPart(n);
				debug << "Exporting form " << part.begin << " +" << part.length << endl;
				cout << device.name << " -> " << part.name << endl;
				file fo;
				fo.open(part.name, "w");
				char *dbuff = (char*)malloc(part.length);
				memcpy(dbuff, buff + part.begin, part.length);
				fo.write(dbuff, part.length);
				fo.close();
				free(dbuff);
			}
		}
	}
	else {
		fi.open(filename, "w");
		if (partition_name.empty())
		{
			const PARTITION part = device.GetPart(partition_name);
			file fo;
			fo.open(partition_name, "r");
			char *pbuff = (char*)malloc(part.length);
			fo.read(pbuff, part.length);
			memcpy(buff + part.begin, pbuff, part.length);
			fo.close();
		}
		else {
			for (int n = 0; n < device.partition_size; n++)
			{
				const PARTITION part = device.GetPart(n);
				file fo;
				fo.open(partition_name, "r");
				char *pbuff = (char*)malloc(part.length);
				fo.read(pbuff, part.length);
				memcpy(buff + part.begin, pbuff, part.length);
				fo.close();
			}
		}
		fi.seekp(0);
		fi.write(buff, device.length);
	}
	return 0;
}