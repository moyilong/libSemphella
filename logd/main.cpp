#include <libSemphella/main.h>
#include <libSemphella/logd.h>
using namespace LogDaemon;
#include <libSemphella/argment.h>
string path;
enum WORK_MODE {
	SHOW,
	APPEND,
};
WORK_MODE mode=SHOW;
bool create = false;
string pwd;
string appval;
void PA(string name, string val)
{
	switch (name.at(0))
	{
	case 'a':
		mode = APPEND;
		appval = val;
		break;
	case 's':
		mode = SHOW;
		break;
	case 'c':
		create = true;
		break;
	case 'm':
		pwd = val;
		break;
	case 'p':
		path = val;
		break;
	}
}

int _main(int argc, char *argv[])
{
	argment args;
	args.load(argc, argv);
	args.for_each(PA);
	if (path.empty())
	{
		cout << "No Path!" << endl;
		return -1;
	}
	logd log(path, pwd, create);
	if (mode == SHOW)
	{
		for (uint64_t n = 0; n < log.getLogSize(); n++)
			cout << "[" << log.GetLogTime(n) << "] " << log.GetLogDescription(n) << endl;
		return log.getLogSize();
	}
	else if (mode == APPEND)
	{
		log.Append(appval);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	try {
		return _main(argc, argv);
	}
	catch (EXCEPTION except)
	{
		cout << "LogDaemon Error:" << except << endl;
	}
	return -1;
}