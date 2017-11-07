#include "stdafx.h"
#include "compiler.h"
#include <libSemphella/files.h>
#include "feature.h"
CMD parse_line(const string str)
{
	vector<estring> get =estring(str.data()).Split(',', true);
	CMD ret;
	ret.arg = 0;
	ret.arg2 = 0;
	ret.bptr = 0;
	ret.type = search(get.at(0));
	ret.arg = atoll(get.at(1).data());
	ret.arg2 = atoll(get.at(2).data());
	ret.bptr = atoll(get.at(3).data());
	return ret;
}

vector<CMD> parse_file(const string xfile)
{
	file fi;
	fi.open(xfile.data(), "r");
	if (fi.is_open())
		return vector<CMD>();
	vector<CMD> ret;
	while (fi.is_eof())
	{
		string line = fi.getline();
		if (line.size() == 0 || line.at(0) == '#')
			continue;
		ret.push_back(parse_line(line));
	}
}

void Compile(const string iname, const string oname)
{
	parse_file(iname);

}
