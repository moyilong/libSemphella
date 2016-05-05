#include "cryptutil2.h"

void ext_dump()
{
	if (input.empty())
	{
		cout << "Error of Input File!" << endl;
		exit(-1);
	}
	file in;
	in.open(input, "r");
	if (!in.is_open())
	{
		cout << "Error of Open Input File!" << endl;
		exit(-1);
	}
	HEAD head;
	in.read(&head, 1);
	if (!head.check())
	{
		cout << "Error of File format!" << endl;
		exit(-1);
	}
	if (head.ext[EXT_EXTABLE] != 1)
	{
		cout << "Error file not support ext data!" << endl;
		exit(-1);
	}
	in.read(&ex, 1);
	if (ex.length == 0 || ex.checksum == 0)
	{
		cout << "Warring File not Have Ext Data!" << endl;
		exit(0);
	}
	ext_data = (char*)malloc(ex.length);
	memset(ext_data, 0, sizeof(ex.length));
	in.read(ext_data, ex.length);
	debug << "Data was been find!" << endl;
	debug << "Find " << ex.length << "@" << ex.checksum << endl;
	mask(ext_data, ex.length);
	cout << ext_data;
}