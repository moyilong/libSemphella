#include "libSemphella.h"
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
using namespace std;
#include <string>
#include <vector>
#include <string.h>


#include <fstream>
#include <time.h>
#include "libbios.h"
#include "crypt.h"
#ifdef __LINUX__
#include "linux.h"
#endif

#include "debug.h"
libDebug bios("libBIOS");

char *check_string = CHECK_STRING;


BIOS::~BIOS()
{
	write();
	iobit.close();
}


BIOS::BIOS(string file, string sign, bool create)
{
	passwd = sign;
	iobit.open(file.data(), ios::binary | ios::in | ios::out);
	if (!iobit.is_open())
	{
		bios << "Open " << file << " Faild!" << endl;
		last_stat = OPEN_FAID;
		return;
	}
#ifndef	__WNO_CREATE_BIOS
	if (!create)
	{
		bool flush_when_read = false;
		char buff[sizeof(BIOS_INFO)] = { 0x00 };
		iobit.read(buff, sizeof(BIOS_INFO));

#ifndef __WNO_BIOS_CRYPT
		//crypt(buff, sizeof(BIOS_INFO), sign);
       //xor_crypt( sizeof(BIOS_INFO),(unsigned char*)buff);
		xor_crypt(CHECK_STRING, buff, sizeof(BIOS_INFO));
#endif
		memcpy(&info, buff, sizeof(BIOS_INFO));
		bool stat = true;
		for (int n = 0; n < strlen(CHECK_STRING); n++)
			if (buff[n] != info.check_area[n])
				stat = false;
		if (!stat)
		{
			bios << "Check arear faild" << endl;
			last_stat = CHECK_FAILD;
			return;
		}
		if (flush_when_read)
			write();
	}
	else
#endif
	{
		strcpy(info.OS, OS_TYPE);
		strcpy(info.version_info, "ELONE DragonBoard Different System Description Table Version 1.0\nProduct and Vendor Define file\nDragonOS Basic IO Sheet\nELONE Firmware Interface");
		strcpy(info.check_area, CHECK_STRING);
		info.EFI.first_init_time = -1;
		memset(info.dsdt, 0, sizeof(info.dsdt));
		write();
	}

}

BIOS::BIOS( string sign,bool create,string file)
{
	BIOS(file,sign,create);
}

void BIOS::setenv(int addr, char value)
{
	info.dsdt[addr] = value;
	write();
}

int BIOS::readenv(int addr)
{
	return info.dsdt[addr];
}

void BIOS::write()
{
#ifndef _WNO_WRITE
	if (!info.write_skip)
	{
		DEBUG_LINE bios << "Writting File...." << endl;
		iobit.seekp(0);
		char buff[sizeof(BIOS_INFO)];
		memcpy(buff, &info, sizeof(BIOS_INFO));
#ifndef __WNO_BIOS_CRYPT
		xor_crypt(CHECK_STRING, buff, sizeof(BIOS_INFO));
#endif
	}
	else{
		bios << "BIOS is Protect" << endl;
	}
#else
	bios << "BIOS is Protect" << endl;
#endif
}

