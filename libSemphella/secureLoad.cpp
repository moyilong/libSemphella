#include "libSemphella.h"
#include "debug.h"
#include "string.h"
#include "secload.h"

API bool mDebugStat = MDEBUG_STAT;

#define err cout<<"[ERROR: "<<__FILE__<<"@"<<__LINE__<<"]"

API int secure_init(int argc, char *argv[], tmain entry, tmain preload)
{
	debug << "Secure Init is Load!" << endl;
	if (entry == NULL)
	{
		err << "Defined Entry is Empty!" << endl;
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
	catch (const string val)
	{
		err << "String Error Dump Catched:" << val << endl;
		return -1;
	}
	catch (...)
	{
		err << "Undefined Error Information!" << endl;
		return -1;
	}
	return 0;
}