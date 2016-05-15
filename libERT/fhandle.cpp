#include "fhandle.h"
FHANDLE FPOLL[FHANDLE_MAX];
int fsize = 0;
FHANDLE::FHANDLE(CFHANDLE hand, int lid)
{
	id = lid;
	this->hand = hand;
	FPOLL[fsize] = *this;
	fsize++;
}

FHANDLE::FHANDLE(fhand_t ht)
{
	id = ht.id;
	hand = ht.hand;
	FPOLL[fsize] = *this;
	fsize++;
}

CFHANDLE get_fhandle(int n)
{
	for (int x = 0; x < fsize; x++)
		if (FPOLL[x].id == n)
			return FPOLL[x].hand;
	cout << "Handle " << n << " is not exist!" << endl;
	KERNEL.error("Try to Use a unexist fhandle!");
	return NULL;
}