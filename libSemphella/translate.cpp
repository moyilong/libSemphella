#include "translate.h"
#include "string.h"
API vector<TRANSLATE_BLOCK> tpoll;

API bool LoadFromFile(string filename)
{
	ifstream tin;
	tin.open(filename.data());
	if (!tin.is_open())
		return false;
	while (!tin.eof())
	{
		string str;
		string fin;
		getline(tin, str);
		TRANSLATE_BLOCK tblock;
		fin = convert_process(str.data());
		int ev = -1;
		for (int n = 0; n < fin.size() && ev == -1; n++)
			if (fin.at(n) == '=' && fin.at(n - 1) == ':')
				ev = n;
		tblock.orig = fin.substr(0, ev);
		tblock.trans = fin.substr(ev + 1);
		//tpoll.push_back(tblock);
		Register(tblock);
	}
}

API void Register(TRANSLATE_BLOCK tblock)
{
	bool check_stat = true;
#pragma omp parallel for
	for (int n = 0; n < tpoll.size(); n++)
		if (streval(tpoll.at(n).orig.data(), tblock.orig.data()))
			check_stat = false;
	if (!check_stat)
		cout << "Warring: " << tblock.orig << " => " << tblock.trans << " insert faild! is already exist!" << endl;
	else
		tpoll.push_back(tblock);

}


API string SearchStr(string orig)
{
	for (int n = 0; n < tpoll.size(); n++)
		if (streval(tpoll.at(n).orig.data(), orig.data()))
			return tpoll.at(n).trans;
}