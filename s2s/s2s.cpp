// s2s.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/main.h>
#include <libSemphella/crypt.h>
#include <time.h>
#include <math.h>
const char *strtbl = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz~`!@#$%^&*()-=_+{}[]\\|;':\",.<>/?/+*-";


void str_add(char *g)
{
	bool nex_plus = true;
	for (int n = 0; n < MAX_BUFF_SIZE; n++)
	{
		if (g[n] == '\0'&&nex_plus)
		{
			g[n] = strtbl[0];
			nex_plus = false;
			continue;
		}
		if (g[n] == '\0')
			continue;
		int now = -1;
		for (int x = 0; x < strlen(strtbl); x++)
			if (g[n] == strtbl[x])
				now = x;
		if (now == -1)
			continue;
		if (nex_plus)
		{
			if (g[n] == strtbl[strlen(strtbl) - 1])
			{
				nex_plus = true;
				g[n] = strtbl[n];
				continue;
			}
			else{
				g[n] = strtbl[now + 1];
				nex_plus = false;
				continue;
			}
		}

	}
}

bool strequal(const char *a, const char *b)
{
	if (strlen(a) != strlen(b))
		return false;
	for (int n = 0; n < strlen(a); n++)
		if (a[n] != b[n])
			return false;
	return true;
}

ofstream out("logdump.log");
#include <limits>
#include <libSemphella/utils.h>
#include <libSemphella/files.h>
int main(int argc, char *argv[])
{
	if (argv[1][0] == '-'&&argv[1][1] == 't')
	{
		goto TEST;
		return 0;
	}
	if (argv[1][0] == '-' && argv[1][1] == 's')
	{
		const uint64_t stor = getsumV2(argv[2], strlen(argv[2]));
#undef min
#undef max
		for (int n = numeric_limits<int>::min(); n < numeric_limits<int>::max(); n++)
		{
			if (stor != getsumV2(argv[2], strlen(argv[2])))
			{
				cout << "System is unsecured!" << endl;
				cout << "Test Value:" << n << endl;
				exit(-1);
			}
			if (n % 10 == 0)
			{
				long double val = (long)n / ((long)numeric_limits<int>::max() - (long)numeric_limits<int>::min());
				if (val <0 )
					val=-val;
				ShowProcessBar(val, ull2s(getsumV2(argv[2],strlen(argv[2]))));
			}
		}
		
	}
	printf("%lld\n", getsumV2(argv[1],strlen(argv[1])));
	return 0;
TEST:
	struct INFO{
		string str;
		uint64_t data;
		size_t match_size = 0;
	};
	vector<INFO>ipoll;
	char str[MAX_BUFF_SIZE];
	memset(str, '\0', MAX_BUFF_SIZE);
	for (int n = 0; n < 6; n++)
		str[n] = strtbl[0];
	int count = 0;
	int match = 0;
	float last = 0;
	float liops = 0;
	clock_t clast = clock();
	while (true)
	{
		INFO inf;
		inf.str = str;
		inf.data=getsumV2(inf.str.data(), inf.str.size());
		bool stat = true;
#pragma omp parallel for
		for (int n = 0; n < ipoll.size();n++)
			if (stat)
			if (ipoll.at(n).data == inf.data&&!strequal(inf.str.data(), ipoll.at(n).str.data()))
			{
				stat = false;
				match++;
				ipoll.at(n).match_size++;
				cout << "FindN:" <<hex<< inf.data<<oct <<" " <<inf.str <<"=="<<ipoll.at(n).str<<"  Match:"<<ipoll.at(n).match_size<<endl;
				//out << "Find Equal!" << endl;
				//out << n<<"Find At:" <<  "(" << inf.data << ") == " << ipoll.at(n).str << "(" << ipoll.at(n).data << ")" << endl;
			}
		str_add(str);
		if (count % 1000 == 0)
		{
			float now = ((float)match / (float)count)*(float)100;
			float iops = 1000 / (float)(clock() - clast);
			cout << "Count:" << count << " Match:" << match << " " << now << "% add:" << now - last << "%\tTimeout:" << clock() - clast << "\t " << iops <<" IOPS\t"<<liops-iops<<endl;
			last = now;
			clast = clock();
			liops = iops;
		}
		count++;
		if (stat)
			ipoll.push_back(inf);
	}
	return 0;

}


