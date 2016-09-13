#include "feature.h"
#include <libSemphella/crypt.h>
void CreateUID(char id[UID_LEN])
{
	emmx mem=mpSum((char*)time(0), sizeof(time(0)), UID_LEN);
	memcpy(id, mem.ptr, UID_LEN);
}

#define PRESET_BLOCK	3
#define PRESET_OFFSET	sizeof(int32_t)*PRESET_BLOCK
