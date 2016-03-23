#include "license.h"

#define ALL_ALLOWED_STRING	"~bcdefgRuvwxyz`{}|Shijklmn!@#$#34OPQTUVopqrst;5$%^&*()a6789LMN':\",.<>WX*-+\\FGABCDE-=_+[]?012HIJKYZ"

LICENSE LICENSE::operator =(string _str)
{
	LICENSE lic;
	string str;
	for (int n = 0; n < _str.size(); n++)
		if (_str.at(n) == '\n')
			continue;
		else
			str += _str.at(n);
	int u = -1;
	for (int n = 0; n < str.size() && u == -1; n++)
		if (str.at(n) == '/')
			u = n;
	if (u == -1)
	{
		cout << "Error! Input String Faild!" << endl;
		return lic;
	}
	lic.main = str.substr(0, u);
	lic.check = str.substr(u);
	if (!CheckLicense(lic))
	{
		cout << "License Check Faild!" << endl;
	}
	return lic;
}


string LICENSE::operator =(LICENSE lic)
{
	string temp=lic.main + "/" + lic.check;
	string ret;
	for (int n = 0; n < temp.size(); n++)
	{
		ret += temp.at(n);
		if ((n+1) % 64 == 0)
			ret += "\n";
	}
	return ret;
}

#ifndef LICENSE_VIRFY_DEEP
#define LICENSE_VIRFY_DEEP  128
#endif

string MainToCheck(string lmain)
{
    uint64_t buff[LICENSE_VIRFY_DEEP];
    char str_buff[sizeof(uint64_t)*LICENSE_VIRFY_DEEP];
#pragma omp parallel for
    for (int n=0; n<LICENSE_VIRFY_DEEP;n++)
    {
        buff[n]=getsumV2(lmain.data(),lmain.size());
        buff[n]<<8;
        buff[n] += n;
        memcpy(str_buff+sizeof(uint64_t)*n,&buff[n],sizeof(uint64_t));
    }
    string ret;
    for (int n=0;n<LICENSE_VIRFY_DEEP;n++)
    {
        int x;
        char val;
        x=LICENSE_VIRFY_DEEP * n;
        x=sin(x)*LICENSE_VIRFY_DEEP;
        x=abs(x);
        val=str_buff[x];
        val=sin(val)*strlen(ALL_ALLOWED_STRING);
        val=abs(val);
        ret+=ALL_ALLOWED_STRING[val];
    }
    return ret;
}

API char ArgmentGetValue(LICENSE lic, uint64_t arg1, uint64_t arg2)
{
	char ret;
	int ret_a = sin(arg1 >> 8 - arg2 << 4)*lic.main.size();
	int ret_b = sin(arg1 << 4 + arg2 >> 8)*lic.check.size();
	ret_a = abs(ret_a);
	ret_b = abs(ret_b);
	char a = lic.main.at(ret_a);
	char b = lic.check.at(ret_b);
	ret = a + b - ret_a - ret_b;
	ret = ret >> 8;
	ret += ret_a * ret_b - a + b;
	return ret;
}



API LICENSE CreateLicense(uint64_t _seed,uint64_t leng_bit)
{
	LICENSE ret;
	for (int n = 0; n < leng_bit; n++)
	{
		uint64_t x = n + _seed + leng_bit - time(0);
		x = x << 8;
		x += leng_bit ^ _seed - rand();
		x += rand() ^ 0x44;
		x += time(0) - clock();
		int value = sin(x)*strlen(ALL_ALLOWED_STRING);
		value = abs(value);
		ret.main += ALL_ALLOWED_STRING[value];
	}
	ret.check = MainToCheck(ret.main);
	return ret;
}

API bool CheckLicense(LICENSE lic)
{
	return streval(lic.check.data(), MainToCheck(lic.main).data());
}
