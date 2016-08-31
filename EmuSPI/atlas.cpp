#include "modules.h"

PARTITION atlas[] = {
	{"uboot",0,0,0x30000},
	{"uboot-env",1,0x30000,0x10000},
	{"factory",2,0x40000,0x100000},
	{"firmware",3,0x50000,0xf90000},
	{"efi",4,0xfe0000,0x10000}
};

DEVICE atlas_part("atlas", 5, atlas, 16 * MB);
DEVICE sirius_part("sirius", 5, atlas, 16 * MB);

PARTITION mtall[] = {
	{ "uboot",0,0,0x30000 },
	{ "uboot-env",1,0x30000,0x10000 },
	{ "factory",2,0x40000,0x100000 },
	{ "firmware",3,0x50000,0x1fa0000 },
	{ "efi",4,0x1ff0000,0x10000 }
};

DEVICE mtall_part("mtall", 5, mtall, 32 * MB);