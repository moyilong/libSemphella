#include "main.h"
#include "debug.h"
#include "string.h"
#include "secload.h"

API bool mDebugStat = MDEBUG_STAT;

#define err cout<<"[ERROR: "<<__FILE__<<"@"<<__LINE__<<"]"

struct mtracert {
	void *ptr;
	string file;
	uint64_t line;
	uint64_t addr;
	uint64_t size;
};

vector<mtracert> poll;

API int secure_init(int argc, char *argv[], tmain entry, tmain preload)
{
	if (entry == NULL)
	{
		err << "Defined Entry is " << entry << endl;
		return -1;
	}
	try {
		if (preload != NULL)
			preload(argc, argv);
		return entry(argc, argv);
	}
	catch (const ERRNO value)
	{
		err << "Program throw an defined error:" << error_list[value] << endl;
		return value;
	}
	catch (...)
	{
		err << "Undefined Error Information!" << endl;
		return -1;
	}
	return 0;
}