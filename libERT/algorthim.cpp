#include "algorthim.h"

ALGHRTHIM APOLL[AMAX];
int xsize = 0;

ALGHRTHIM::ALGHRTHIM(password_algrthom p, crypt_algrthom c, sum_algrthom s, int xid, string _doc, get_password_checksum pc, bool _can_be_pt)
{
	id = xid;
	pa = p;
	ca = c;
	sa = s;
	px = pc;
	doc = _doc;
	debug << "Insert Alghrthim " << id << " @ " << xsize << endl;
	//APOLL.push_back(*this);
	can_be_pt = _can_be_pt;
	for (int n = 0; n < xsize; n++)
		if (APOLL[n].id == xid)
		{
			debug << "Error of ID REMAP!" << endl;
			debug << "ID:" << id << " stored at " << n << " is remaped!" << endl;
			exit(-1);
		}
	APOLL[xsize++] = *this;
}

char _cached_trans_from = -1;
char _cached_trans_to = -1;

uint64_t password_type(string passwd)
{
	return getsumV2(passwd.data(), passwd.size());
}

int trans_id(int id)
{
	if (_cached_trans_from == id)
		return _cached_trans_to;
	int ret = -1;
	for (int n = 0; n < xsize; n++)
		if (APOLL[n].id == id)
		{
			ret = n;
			debug << "Trans ID " << id << " => " << ret << endl;
			_cached_trans_from = id;
			_cached_trans_to = ret;
			return ret;
		}
	debug << "Id not found!" << endl;
	debug << "Founted ID:" << id << endl;
	cout << "ID " << id << " was not exist!" << endl;
	return -1;
}