#include "string.h"

vector<estring> estring::Split(char arr, bool auto_trim)
{
	vector<estring> ret;
	uint64_t last = 0;
	estring temp;
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
				temp = temp.Trim();
			ret.push_back(temp);
			last = n + 1;
			n++;
		}
	}
	return ret;
}
estring estring::ToUpper()
{
	char *buff = (char*)malloc(size());
#pragma omp parallel for
	for (int n = 0; n < size(); n++)
		if (!is_upper(at(n)))
			buff[n] = set_upper(at(n), true);
		else
			buff[n] = at(n);
	return estring(buff);
}

estring estring::ToLower()
{
	char *buff = (char*)malloc(size());
#pragma omp parallel for
	for (int n = 0; n < size(); n++)
		if (is_upper(at(n)))
			buff[n] = set_upper(at(n), false);
		else
			buff[n] = at(n);
	return estring(buff);
}

estring estring::Trim()
{
	estring ret;
	uint64_t beg = 0;
	uint64_t end = 0;
	for (uint64_t n = 0; n < size(); n++)
		if (at(n) != ' ')
		{
			beg = n;
			break;
		}
	for (uint64_t n = size() - 1; n >= 0; n++)
		if (at(n) != ' ')
		{
			end = n;
			break;
		}
	return substr(beg, end - beg + 1);
}

estring estring::RemoveCharArray(const char *arr)
{
	estring ret = "";
	for (int n = 0; n < length(); n++)
	{
		bool stat = true;
		for (int n = 0; n<strlen(arr); n++)
			if (arr[n] == at(n))
			{
				stat = false;
				break;
			}
		if (stat)
			ret += at(n);
	}
	return ret;
}