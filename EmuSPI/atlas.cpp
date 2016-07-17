#include "modules.h"


PARTITION atlas[] = {
	{"uboot",0,0,0x30000},
	{"uboot-env",1,0x30000,0x10000},
	{"factory",2,0x40000,0x100000},
	{"firmware",3,0x50000,0xfa0000}
};

DEVICE atlas_part("atlas", 4, atlas, 16 * MB);
DEVICE sirius_part("sirius", 4, atlas, 16 * MB);