#include <libSemphella/apd.h>
#include <libSemphella/main.h>
#include <libSemphella/argment.h>
argment args;
enum {
	DUMP,
	CONVERT,
}work_mode;
string input;
string output;
void for_each(string name, string val)
{
	switch (name[0])
	{
	case 'd':
		work_mode = DUMP;
		break;
	case 'c':
		work_mode = CONVERT;
		break;
	case 'i':
		input = val;
		break;
	case 'o':
		output = val;
		break;
	}
}

void node_each(NODE link, COUNT_TYPE id)
{
	cout << "-> " << link.n_name << "." << id << ":" << link.label.size() << endl;
	for (uint64_t n = 0; n < link.label.size(); n++)
		cout << "  -> \"" << link.label.at(n).name << "\" = \"" << link.label.at(n).data <<"\"" <<endl;
}

void dump()
{
	APD read(input);
	read.node_for_each(node_each,true_each_check,false);
}

void convert()
{
	APD read;
	read.load(input);
	cout << "Status is Reset!" << endl;
	read.bin_mode = !read.bin_mode;
	if (!output.empty())
		read.filename = output;
	read.save();
}

int main(int argc, char *argv[])
{
	work_mode = DUMP;
	args.load(argc, argv);
	args.for_each(for_each);
	if (input.empty())
	{
		cout << "Empty Input!" << endl;
		return -1;
	}
	switch (work_mode)
	{
	case DUMP:
		dump();
		break;
	case CONVERT:
		convert();
		break;
	}
	return 0;
}