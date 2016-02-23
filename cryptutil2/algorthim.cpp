#include "algorthim.h"

#define STD_DBU	debug<<"[Alghrthim]"

ALGHRTHIM::ALGHRTHIM(password_algrthom p, crypt_algrthom c, sum_algrthom s, int xid)
{
	id = xid;
	pa = p;
	ca = c;
	sa = s;
	STD_DBU << "Insert Alghrthim " << id << " @ " << APOLL.size() << endl;
	APOLL.push_back(*this);
}


char _cached_trans_from = -1;
char _cached_trans_to = -1;

int trans_id(int id)
{
	if (_cached_trans_from == id)
		return _cached_trans_to;
	char ret = -1;
#pragma omp parallel for
	for (int n = 0; n < APOLL.size(); n++)
		if (APOLL.at(n).id == id)
			ret = id;
	_cached_trans_from = id;
	_cached_trans_to = ret;
	if (ret == -1)
		debug << "Try to Access a un exist alg! ID=" << id << endl;
	return ret;
}
