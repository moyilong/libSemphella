#include "estring.h"

vector<estring> estring::Split(char arr,bool auto_trim)
{
	vector<estring> ret;
	uint64_t last = 0;
	string temp;
	for (uint64_t n = 0; n < size(); n++)
	{
		if (at(n) == arr || n + 1 == size())
		{
			if (n + 1 == size())
				temp = substr(last);
			else
				temp = substr(last, n - last);
			debug << "Splite Str:" << temp << endl;
			if (auto_trim)
				temp = Trim(temp);
			ret.push_back(temp);
			last = n + 1;
			n++;
		}
	}
	return ret;
}
