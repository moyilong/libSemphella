#include "translate.h"

struct TBLOCK{
	string origin;
	string target;
};

vector<TBLOCK>tpoll;

void load_asci(string filename)
{
	ifstream input;
	input.open(filename.data());
	if (!input.is_open())
	{
		cout<<"Translate file load error!"<<endl;
		return;
	}
	int line=0;
	while (!input.eof())
	{
		line++;
		string buff;
		getline(input, buff);
		vector<int>p;
		for (unsigned int n=0;n<buff.size();n++)
			if (buff.at(n)=='\"')
				p.push_back(n);
		if (p.size()%2!=0)
		{
			cout<<"Translate file Syntax Error!"<<endl;
			cout<<"Apped at:"<<line<<endl;
			cout << "parser error!" << endl;
			input.close();
			return;
		}
		vector<string> spoll;
		for (unsigned int n=0;n<p.size();n+=2)
			spoll.push_back(buff.substr(p.at(n),p.at(n+1)));
		if (spoll.size()!=2)
		{
			cout<<"Translast file Syntax Error"<<endl;
			cout<<"Append at:"<<line<<endl;
			cout<<"Error:splite error!"<<endl;
			input.close();
			return;
		}
		TBLOCK temp;
		temp.origin=spoll.at(0);
		temp.target=spoll.at(1);
		tpoll.push_back(temp);
		}
	if (input.is_open())
		input.close();
}


struct TBIN_HEAD{
	int size;
};

struct TBIN_INFO{
	int target_len;
	int origin_len;
};


void load_bin(string filename)
{
	ifstream input;
	input.open(filename.data());
	if (!input.is_open())
	{
		cout << "Translate file load error!" << endl;
		return;
	}
	TBIN_HEAD head;
	input.read((char*)&head, sizeof(TBIN_HEAD));
	for (int n = 0; n < head.size; n++)
	{
		TBIN_INFO bin;
		input.read((char*)&bin, sizeof(TBIN_INFO));
		TBLOCK tinfo;
		char *buff = (char*)calloc(sizeof(char), max(bin.target_len, bin.origin_len));
		input.read(buff, bin.origin_len);
		tinfo.origin = buff;
		memset(buff, 0, sizeof(buff));
		input.read(buff, bin.target_len);
		tinfo.target = buff;
		free(buff);
	}
	input.close();
}
		
void dump_bin(string filename)
{
	ofstream output;
	output.open(filename.data());
	if (!output.is_open())
	{
		cout << "Dump file Load Faild!" << endl;
		return;
	}
	TBIN_HEAD temp;
	temp.size = tpoll.size();
	output.write((char*)&temp, sizeof(TBIN_HEAD));
	for (unsigned int n = 0; n < tpoll.size(); n++)
	{
		TBIN_INFO w;
		w.origin_len = tpoll.at(n).origin.size();
		w.target_len = tpoll.at(n).target.size();
		output.write((char*)&w, sizeof(TBIN_INFO));
		output.write(tpoll.at(n).origin.data(), w.origin_len);
		output.write(tpoll.at(n).target.data(), w.target_len);
	}
	output.close();
}

inline bool estrcmp(string a, string b)
{
	if (a.size() != b.size())
		return false;
	bool ret = true;
#pragma omp parallel for 
	for (long n = 0; n < a.size(); n++)
		if (a.at(n) != b.at(n))
			ret = false;
	return ret;
}
string translate(string origin)
{
	int r_back = -1;
#pragma omp parallel for
	for (long n = 0; n < tpoll.size(); n++)
		if (estrcmp(origin, tpoll.at(n).origin))
			r_back = n;
	if (r_back == -1 || tpoll.at(r_back).target.empty())
		return origin;
	else
		return tpoll.at(r_back).target;
}